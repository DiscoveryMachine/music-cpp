// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/music_command_response_message.h"
#include "util/validate.h"

namespace music {

// Validate
void MusicCommandResponseMessage::Validate() const {
  if (command_identifier_ < 0 || command_identifier_ > 4294967295) {
    LogError(
        "MusicCommandResponseMessage::Validate - command_identifier_ out of "
        "range [0, 4294967295]: " +
        std::to_string(command_identifier_));
  }
  MusicResponseMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const MusicCommandResponseMessage &message) {
  // serialize super class members
  to_json(j, static_cast<const MusicResponseMessage &>(message));
  // append derived class members
  j.update({{MusicCommandResponseMessage::kCommandIdKey,
             message.command_identifier_}});
}

void from_json(const nlohmann::json &j, MusicCommandResponseMessage &message) {
  j.at(MusicCommandResponseMessage::kCommandIdKey)
      .get_to(message.command_identifier_);
  // fill out super class members
  from_json(j, static_cast<MusicResponseMessage &>(message));
}

}  // namespace music
