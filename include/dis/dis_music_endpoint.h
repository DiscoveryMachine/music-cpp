// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_MUSIC_ENDPOINT_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_MUSIC_ENDPOINT_H_

#include <asio/ip/udp.hpp>
#include <cstddef>

#include "dis_music_configuration.h"
#include "message/music_message.h"
#include "music_endpoint.h"
#include "music_endpoint_user.h"

namespace music {
class DISMusicEndpoint : public music::MusicEndpoint {
public:
  void Connect();
  void Disconnect();
  void RegisterUser(music::MusicEndpointUser &user) { this->user = &user; }
  void DeregisterUser(music::MusicEndpointUser &user) {
    if (this->user == &user) {
      this->user = nullptr;
    }
  }
  void SendMessage(music::MusicMessage &mesg);

  void HandleReceive(const asio::error_code &error, std::size_t bytes_transferred);

  DISMusicEndpoint(music::MusicConfiguration *config, int exercise_id, SimulationAddress addr)
      : config(static_cast<music::DISMusicConfiguration *>(config)), exercise_id(exercise_id),
        user(nullptr), endpoint_address(addr) {}

private:
  music::MusicEndpointUser *user;
  int exercise_id;
  music::DISMusicConfiguration *config;
  music::SimulationAddress endpoint_address;

  asio::ip::udp::socket *receiver_socket;
  asio::ip::udp::socket *sender_socket;

  std::array<char, 128> recv_buf;
};
} // namespace music

#endif // MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_MUSIC_ENDPOINT_H_