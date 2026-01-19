// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_ENDPOINT_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_ENDPOINT_H_

#include "message/music_message.h"

namespace music {

class MusicEndpointUser;

// Establishes a communication endpoint from which to transmit MUSIC messages.
//
// The messaging protocol defined by the MUSIC standard is medium-agnostic and
// can be embedded within other protocols (namely the Distributed Interactive
// Simulation protocol, or DIS). Regardless of choice of medium or protocol, the
// most basic functionality that must be on offer by such an endpoint that
// endeavors to participate in the MUSIC messaging protocol is specified in the
// following class.
//
// The currently supported mediums are: Redis.
// The currently supported protocols are: DIS, HLA.
// See: `music::MusicEndpointType`.
//
// For historical reasons, mediums through, and protocols over, which MUSIC is
// transmitted are conflated often. We say "MUSIC over redis" or "MUSIC over
// DIS" perhaps intimating that the conjunction of these two is impossible.
// However, there is nothing technically limiting MUSIC from being embedded
// first in the DIS protocol, and then being "sent over" redis. Indeed this is a
// present limitation of the current design in that it precludes this
// possibility.
class MusicEndpoint {
public:
  // Connect to another MUSIC PSS/PCS
  //
  // Throws:
  // - std::exception
  virtual void Connect() = 0;

  // Disconnect from another MUSIC PSS/PCS
  //
  // Throws:
  // - std::exception
  virtual void Disconnect() = 0;

  // Register a `music::MusicEndpointUser` with the endpoint such that the user
  // will be notified to incoming MUSIC communication.
  virtual void RegisterUser(MusicEndpointUser &user) = 0;

  // Deregister a `music::MusicEndpointUser` with the endpoint such that the
  // user will no longer be notifited to the MUSIC traffic flowing through this
  // endpoint.
  virtual void DeregisterUser(MusicEndpointUser &user) = 0;

  // Send a MUSIC message from the endpoint.
  //
  // Throws
  // - std::exception
  virtual void SendMessage(MusicMessage &mesg) = 0;
};
} // namespace music

#endif // MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_ENDPOINT_H_