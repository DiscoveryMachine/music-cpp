// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/playercontrol/control_released_message.h"
#include "util/validate.h"

namespace music {

// Validate
void ControlReleasedMessage::Validate() const {
  if (type_ != MusicMessageType::kControlReleased) {
    LogError(
        "ControlReleasedMessage::Validate - type_ must be kControlReleased");
  }
  if (command_identifier_ !=
      MusicMessageTypeHelper::GetInfoForType(type_).action_type) {
    LogError(
        "ControlReleasedMessage::Validate - command_identifier_ does not "
        "match type_: " +
        std::to_string(command_identifier_));
  }
  MusicCommandRequestMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const ControlReleasedMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // No additional fields to serialize
}

void from_json(const nlohmann::json &j, ControlReleasedMessage &message) {
  // extract super class json
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
  // No additional fields to extract
}

}  // namespace music