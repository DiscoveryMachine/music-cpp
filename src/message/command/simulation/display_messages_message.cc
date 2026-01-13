// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/simulation/display_messages_message.h"
#include "util/validate.h"

namespace music {

// Validate
void DisplayMessagesMessage::Validate() const {
  if (type_ != MusicMessageType::kDisplayMessages) {
    LogError(
        "DisplayMessagesMessage::Validate - type_ must be kDisplayMessages");
  }
  if (command_identifier_ !=
      MusicMessageTypeHelper::GetInfoForType(type_).action_type) {
    LogError(
        "DisplayMessagesMessage::Validate - command_identifier_ does not "
        "match type_: " +
        std::to_string(command_identifier_));
  }
  if (timeout_ < 0 || timeout_ > std::numeric_limits<unsigned int>::max()) {
    LogError(
        "DisplayMessagesMessage::Validate - timeout_ cannot be negative: " +
        std::to_string(timeout_));
  }
  if (messages_.empty()) {
    LogError("DisplayMessagesMessage::Validate - messages_ cannot be empty");
  }
  MusicCommandRequestMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const DisplayMessagesMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // Append DisplayMessagesMessage fields
  j.update({{DisplayMessagesMessage::kMessagesKey, message.messages_},
            {DisplayMessagesMessage::kTimeoutKey, message.timeout_}});
}
void from_json(const nlohmann::json &j, DisplayMessagesMessage &message) {
  // extract super class json
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
  // Extract DisplayMessagesMessage fields
  j.at(DisplayMessagesMessage::kMessagesKey).get_to(message.messages_);
  j.at(DisplayMessagesMessage::kTimeoutKey).get_to(message.timeout_);
}

}  // namespace music