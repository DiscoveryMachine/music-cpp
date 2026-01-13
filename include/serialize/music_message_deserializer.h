// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_MUSIC_MESSAGE_DESERIALIZER_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_MUSIC_MESSAGE_DESERIALIZER_H_

#include "message/music_message.h"

namespace music {
template <class SourceType>
class MUSICMessageDeserializer {
 public:
  virtual std::shared_ptr<MusicMessage> DeserializeMusicMessage(
      SourceType &source) = 0;
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_MUSIC_MESSAGE_DESERIALIZER_H_