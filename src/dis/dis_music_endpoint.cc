// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <dis6/utils/Endian.h>
#include <dis7/EntityID.h>
#include <dis7/Pdu.h>
#include <dis7/utils/DataStream.h>
#include <spdlog/spdlog.h>

#include <iomanip>
#include <sstream>

#include "asio/io_context.hpp"
#include "asio/ip/udp.hpp"
#include "config/config.h"
#include "dis/dis_music_endpoint.h"
#include "serialize/dis7_message_serializer.h"
#include "util/music_byte_buffer.h"

namespace music {
namespace {
std::string UdpEndpointToString(const asio::ip::udp::endpoint &ep) {
  std::ostringstream oss;
  oss << ep.address().to_string() << ":" << ep.port();
  return oss.str();
}

std::string HexPreview(const char *data, std::size_t len,
                       std::size_t max_bytes = 32) {
  if (data == nullptr || len == 0) {
    return "";
  }
  const std::size_t n = std::min(len, max_bytes);
  std::ostringstream oss;
  oss << std::hex << std::setfill('0');
  for (std::size_t i = 0; i < n; ++i) {
    oss << std::setw(2)
        << (static_cast<unsigned int>(static_cast<unsigned char>(data[i])));
    if (i + 1 != n) {
      oss << ' ';
    }
  }
  if (n != len) {
    oss << " ...";
  }
  return oss.str();
}
}  // namespace

void DISMusicEndpoint::Connect() {
  spdlog::set_level(music::spdlog_level);
  spdlog::debug("DISMusicEndpoint::Connect send={}:{} receive={}:{}",
                config->GetSendAddress(), config->GetSendPort(),
                config->GetReceiveAddress(), config->GetReceivePort());

  asio::io_context send_io_context;
  asio::ip::udp::resolver send_resolver(send_io_context);
  asio::error_code ec;
  auto send_results =
      send_resolver.resolve(asio::ip::udp::v4(), config->GetSendAddress(),
                            std::to_string(config->GetSendPort()), ec);
  if (ec) {
    spdlog::error("DISMusicEndpoint::Connect resolve(send) failed: {}",
                  ec.message());
    return;
  }
  asio::ip::udp::endpoint sender_endpoint = *send_results.begin();
  spdlog::debug("DISMusicEndpoint::Connect resolved send endpoint={}",
                UdpEndpointToString(sender_endpoint));

  sender_socket = new asio::ip::udp::socket(send_io_context);

  if (const asio::error_code open_ec =
          sender_socket->open(asio::ip::udp::v4(), ec);
      open_ec) {
    spdlog::error("DISMusicEndpoint::Connect sender_socket.open failed: {}",
                  open_ec.message());
    return;
  }

  auto local_send_ep = sender_socket->local_endpoint(ec);
  if (!ec) {
    spdlog::debug("DISMusicEndpoint::Connect sender local_endpoint={}",
                  UdpEndpointToString(local_send_ep));
  }

  asio::io_context receive_io_context;
  asio::ip::udp::resolver receive_resolver(receive_io_context);
  auto recv_results =
      receive_resolver.resolve(asio::ip::udp::v4(), config->GetReceiveAddress(),
                               std::to_string(config->GetReceivePort()), ec);
  if (ec) {
    spdlog::error("DISMusicEndpoint::Connect resolve(receive) failed: {}",
                  ec.message());
    return;
  }
  asio::ip::udp::endpoint receiver_endpoint = *recv_results.begin();
  spdlog::debug("DISMusicEndpoint::Connect resolved receive endpoint={}",
                UdpEndpointToString(receiver_endpoint));

  receiver_socket = new asio::ip::udp::socket(receive_io_context);

  if (const asio::error_code open_ec =
          receiver_socket->open(asio::ip::udp::v4(), ec);
      open_ec) {
    spdlog::error("DISMusicEndpoint::Connect receiver_socket.open failed: {}",
                  open_ec.message());
    return;
  }

  // receiver_socket.async_receive_from(asio::buffer(recv_buf),
  // receiver_endpoint,
  //                                    std::bind(&DISMusicEndpoint::handle_receive,
  //                                    this,
  //                                              std::placeholders::_1,
  //                                              std::placeholders::_2));
}

void DISMusicEndpoint::HandleReceive(const asio::error_code &error,
                                     std::size_t bytes_transferred) {
  if (!error && bytes_transferred > 0) {
    DIS::Pdu pdu;

    DIS::DataStream ds(reinterpret_cast<const char *>(recv_buf.data()),
                       bytes_transferred, DIS::Endian::BIG);
    pdu.unmarshal(ds);
    music::MusicMessage message;

    user->HandleMessage(message);

    // Process the received data
  }
}

void DISMusicEndpoint::Disconnect() {
  spdlog::debug("DISMusicEndpoint::Disconnect");
  receiver_socket->close();
  sender_socket->close();
}

void DISMusicEndpoint::SendMessage(music::MusicMessage &mesg) {
  spdlog::debug("DISMusicEndpoint::SendMessage dest: {}:{}",
                config->GetSendAddress(), config->GetSendPort());
  Dis7MessageSerializer serializer = Dis7MessageSerializer();

  MusicByteBuffer buff = serializer.SerializeMusicMessage(mesg);

  const auto len = buff.GetLength();
  spdlog::debug("DISMusicEndpoint::SendMessage payload_bytes: {}", len);
  if (len == 0) {
    spdlog::warn("DISMusicEndpoint::SendMessage WARNING: empty payload");
  }

  if (sender_socket == nullptr) {
    spdlog::error(
        "DISMusicEndpoint::SendMessage ERROR: sender_socket is null (Connect "
        "not called or failed)");
    return;
  }
  spdlog::debug("DISMusicEndpoint::SendMessage sender_socket.is_open: {}",
                sender_socket->is_open() ? "true" : "false");

  asio::error_code ec;
  auto local_ep = sender_socket->local_endpoint(ec);
  if (!ec) {
    spdlog::debug("DISMusicEndpoint::SendMessage local_endpoint: {}",
                  UdpEndpointToString(local_ep));
  }

  asio::io_context send_io_context;
  asio::ip::udp::resolver send_resolver(send_io_context);
  auto results =
      send_resolver.resolve(asio::ip::udp::v4(), config->GetSendAddress(),
                            std::to_string(config->GetSendPort()), ec);
  if (ec) {
    spdlog::error("DISMusicEndpoint::SendMessage resolve failed: {}",
                  ec.message());
    return;
  }
  asio::ip::udp::endpoint sender_endpoint = *results.begin();
  spdlog::debug("DISMusicEndpoint::SendMessage resolved dest={}",
                UdpEndpointToString(sender_endpoint));

  char *send_buf = new char[len];

  for (unsigned int i = 0; i < len; i++) {
    send_buf[i] = buff[i];
  }

  spdlog::debug("DISMusicEndpoint::SendMessage payload_hex_preview={}",
                HexPreview(send_buf, len));

  const std::size_t sent = sender_socket->send_to(asio::buffer(send_buf, len),
                                                  sender_endpoint, 0, ec);
  if (ec) {
    spdlog::error("DISMusicEndpoint::SendMessage send_to failed: {}",
                  ec.message());
  } else {
    spdlog::debug("DISMusicEndpoint::SendMessage sent_bytes: {}", sent);
  }

  delete[] send_buf;
}
}  // namespace music