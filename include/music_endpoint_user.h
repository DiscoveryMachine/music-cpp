// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_ENDPOINT_USER_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_ENDPOINT_USER_H_

#include "enum/music_message_type.h"
#include "message/music_message.h"

namespace music {

// Allows for "subscription" to a particular `music::MusicEndpoint` with the
// added capability of filtering subscription messages base on message type.
//
// The `music::MusicEndpoint` merely sends and receives MUSIC messages. Without
// registering a user with the endpoint, namely this class, the endpoint is
// relatively inactive. `music::MusicEndpointUser` defines a callback interface
// that allows the endpoint to direct messages to the user.
//
// Example usage:
// auto user  = MyEndpointUser(...); // subclasses `music::MusicEndpointUser`
// auto endpt = music:RedisEndpoint(...);
// endpt.RegisterUser(user);
//
// // ...in endpt upon receipt of a new MUSIC message...
// if (user.IsListeningForMessageType(mesg.GetType())) {
//   user.HandleMessage(mesg);
// }
class MusicEndpointUser {
public:
  // Determine if the given `music::MusicMessageType` should be handled, i.e.,
  // if it should be fed to HandleMessage.
  virtual bool IsListeningForMessageType(const MusicMessageType &t) = 0;

  virtual void HandleMessage(const MusicMessage &mesg) = 0;
};

} // namespace music

#endif // MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_ENDPOINT_USER_H_