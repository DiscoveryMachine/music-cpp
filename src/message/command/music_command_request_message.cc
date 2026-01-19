// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/music_command_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void MusicCommandRequestMessage::Validate() const {
  if (command_identifier_ < 0 || command_identifier_ > 4294967295) {
    LogError(
        "MusicCommandRequestMessage::Validate - command_identifier_ out of "
        "range [0, 4294967295]: " +
        std::to_string(command_identifier_));
  }
  MusicRequestMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const MusicCommandRequestMessage &message) {
  // serialize super class members
  to_json(j, static_cast<const MusicRequestMessage &>(message));
  // append derived class members
  j.update({{MusicCommandRequestMessage::kCommandIdKey,
             message.command_identifier_}});
}

void from_json(const nlohmann::json &j, MusicCommandRequestMessage &message) {
  j.at(MusicCommandRequestMessage::kCommandIdKey)
      .get_to(message.command_identifier_);
  // fill out super class members
  from_json(j, static_cast<MusicRequestMessage &>(message));
}
}  // namespace music
