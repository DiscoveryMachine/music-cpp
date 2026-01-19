// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/scenario/create_environment_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void CreateEnvironmentRequestMessage::Validate() const {
  if (environment_name_.empty()) {
    LogError(
        "CreateEnvironmentRequestMessage::Validate - environment_name_ cannot "
        "be empty");
  }
  if (environment_metadata_.is_null() || environment_metadata_.empty()) {
    LogError(
        "CreateEnvironmentRequestMessage::Validate - environment_metadata_ "
        "cannot be null or empty");
  }

  if (command_identifier_ != MusicMessageTypeHelper::GetInfoForType(
                                 MusicMessageType::kCreateEnvironmentRequest)
                                 .action_type) {
    LogError(
        "CreateEnvironmentRequestMessage::Validate - command_identifier_ "
        "does not match expected value for CreateEnvironmentRequestMessage: " +
        std::to_string(command_identifier_));
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
  if (type_ != MusicMessageType::kCreateEnvironmentRequest) {
    LogError(
        "CreateEnvironmentRequestMessage::Validate - type_ does not match "
        "expected value for CreateEnvironmentRequestMessage: " +
        std::to_string(static_cast<int>(type_)));
  }

  MusicCommandRequestMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j,
             const CreateEnvironmentRequestMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // append derived class members
  j.update(
      nlohmann::json{{CreateEnvironmentRequestMessage::kEnvironmentNameKey,
                      message.environment_name_},
                     {CreateEnvironmentRequestMessage::kEnvironmentMetadataKey,
                      message.environment_metadata_}});
}

void from_json(const nlohmann::json &j,
               CreateEnvironmentRequestMessage &message) {
  // fill out super class members
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));

  // Add derived class members
  j.at(CreateEnvironmentRequestMessage::kEnvironmentNameKey)
      .get_to(message.environment_name_);
  j.at(CreateEnvironmentRequestMessage::kEnvironmentMetadataKey)
      .get_to(message.environment_metadata_);
}

}  // namespace music
