// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/music_command_response_message.h"
#include "message/command/scenario/create_environment_response_message.h"
#include "util/validate.h"

namespace music {

// Validate
void CreateEnvironmentResponseMessage::Validate() const {
  if (command_identifier_ != MusicMessageTypeHelper::GetInfoForType(
                                 MusicMessageType::kCreateEnvironmentResponse)
                                 .action_type) {
    LogError(
        "CreateEnvironmentResponseMessage::Validate - command_identifier_ "
        "does not match expected value for CreateEnvironmentResponseMessage: " +
        std::to_string(command_identifier_));
  }
  if (type_ != MusicMessageType::kCreateEnvironmentResponse) {
    LogError(
        "CreateEnvironmentResponseMessage::Validate - type_ does not match "
        "expected value for CreateEnvironmentResponseMessage: " +
        std::to_string(static_cast<int>(type_)));
  }
  if (origin_id_.GetEntityNumber() != 0) {
    LogError(
        "CreateConstructRequestMessage::Validate - origin_id_ entity number "
        "must be 0 for system-to-system messages");
  }
  if (receiver_id_.GetEntityNumber() != 0) {
    LogError(
        "CreateConstructRequestMessage::Validate - receiver_id_ entity number "
        "must be 0 for system-to-system messages");
  }
  MusicCommandResponseMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j,
             const CreateEnvironmentResponseMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandResponseMessage &>(message));
  // no new members to append
}

void from_json(const nlohmann::json &j,
               CreateEnvironmentResponseMessage &message) {
  // fill out super class members
  from_json(j, static_cast<MusicCommandResponseMessage &>(message));
}

}  // namespace music
