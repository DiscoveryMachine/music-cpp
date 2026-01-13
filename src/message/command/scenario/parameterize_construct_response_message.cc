// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/scenario/parameterize_construct_response_message.h"
#include "util/validate.h"

namespace music {

// Validate
void ParameterizeConstructResponseMessage::Validate() const {
  if (type_ != MusicMessageType::kParameterizeConstructResponse) {
    LogError(
        "ParameterizeConstructResponseMessage::Validate - type_ incorrect for "
        "ParameterizeConstructResponseMessage");
  }
  if (command_identifier_ !=
      MusicMessageTypeHelper::GetInfoForType(
          MusicMessageType::kParameterizeConstructResponse)
          .action_type) {
    LogError(
        "ParameterizeConstructResponseMessage::Validate - "
        "command_identifier_ incorrect for "
        "ParameterizeConstructResponseMessage");
  }
  if (receiver_id_.GetEntityNumber() != 0) {
    LogError(
        "ParameterizeConstructResponseMessage::Validate - receiver_id_ entity "
        "number "
        "must be 0 for sytem-received messages");
  }
  MusicCommandResponseMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j,
             const ParameterizeConstructResponseMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandResponseMessage &>(message));
  // no new members to append
}

void from_json(const nlohmann::json &j,
               ParameterizeConstructResponseMessage &message) {
  // fill out super class members
  from_json(j, static_cast<MusicCommandResponseMessage &>(message));
}

}  // namespace music