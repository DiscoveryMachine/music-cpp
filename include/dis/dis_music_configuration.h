// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_MUSIC_CONFIGURATION_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_MUSIC_CONFIGURATION_H_

#include "dis_protocol.h"
#include "music_configuration.h"
#include "music_endpoint.h"

namespace music {
// Configuration parameters for a DIS MUSIC endpoint.
class DISMusicConfiguration : public music::MusicConfiguration {
public:
  std::string GetSendAddress() const { return send_address; }
  int GetSendPort() const { return send_port; }
  music::DISProtocol GetSendProtocol() const { return send_protocol; }
  std::string GetReceiveAddress() const { return receive_address; }
  int GetReceivePort() const { return receive_port; }
  void SetSendAddress(const std::string &address) { send_address = address; }
  void SetSendPort(int port) { send_port = port; }
  void SetSendProtocol(music::DISProtocol protocol) { send_protocol = protocol; }
  void SetReceiveAddress(const std::string &address) { receive_address = address; }
  void SetReceivePort(int port) { receive_port = port; }

  // MusicEndpoint GetEndpoint(int exercise_id, SimulationAddress addr) override;

private:
  // The Address/Hostname/IP to send DIS Traffic to.
  std::string send_address;
  // The Port to send DIS Traffic to.
  int send_port;
  // The DIS protocol version to use when sending DIS Traffic.
  music::DISProtocol send_protocol;
  // The Address/Hostname/IP to receive DIS Traffic from.
  std::string receive_address;
  // The Port to receive DIS Traffic from.
  int receive_port;
};
} // namespace music

#endif // MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_MUSIC_CONFIGURATION_H_