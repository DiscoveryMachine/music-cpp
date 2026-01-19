// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/scenario/create_construct_response_message.h"
#include "util/validate.h"

namespace music {

// Validate
void CreateConstructResponseMessage::Validate() const {
  if (type_ != MusicMessageType::kCreateConstructResponse) {
    LogError(
        "CreateConstructResponseMessage::Validate - type_ does not match "
        "expected value for CreateConstructResponseMessage: " +
        std::to_string(static_cast<int>(type_)));
  }

  if (command_identifier_ != MusicMessageTypeHelper::GetInfoForType(
                                 MusicMessageType::kCreateConstructResponse)
                                 .action_type) {
    LogError(
        "CreateConstructResponseMessage::Validate - command_identifier_ "
        "does not match expected value for CreateConstructResponseMessage: " +
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
  construct_id_.Validate();
  MusicCommandResponseMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const CreateConstructResponseMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandResponseMessage &>(message));
  // append derived class members
  j.update(nlohmann::json{{CreateConstructResponseMessage::kConstructIdKey,
                           message.construct_id_}});
}

void from_json(const nlohmann::json &j,
               CreateConstructResponseMessage &message) {
  // fill out super class members
  from_json(j, static_cast<MusicCommandResponseMessage &>(message));

  // add derived class members
  j.at(CreateConstructResponseMessage::kConstructIdKey)
      .get_to(message.construct_id_);
}

}  // namespace music