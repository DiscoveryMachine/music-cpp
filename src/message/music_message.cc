// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include "enum/music_message_type.h"
#include "message/music_message.h"
#include "message/music_message_header.h"
#include "util/validate.h"

namespace music {

// Validate
void MusicMessage::Validate() const {
  // Should not be kOtherUnknown or out of range
  if (type_ < MusicMessageType::kOtherUnknown ||
      type_ >= MusicMessageType::_count) {
    LogError("MusicMessage::Validate - type_ out of range");
  }

  header_.Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const MusicMessage &message) {
  j = nlohmann::json{{MusicMessage::kHeaderJsonKey, message.header_}};
}

void from_json(const nlohmann::json &j, MusicMessage &message) {
  j.at(MusicMessage::kHeaderJsonKey).get_to(message.header_);
}

}  // namespace music