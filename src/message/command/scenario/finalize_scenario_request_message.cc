// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/scenario/finalize_scenario_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void FinalizeScenarioRequestMessage::Validate() const {
  if (command_identifier_ != MusicMessageTypeHelper::GetInfoForType(
                                 MusicMessageType::kFinalizeScenarioRequest)
                                 .action_type) {
    LogError(
        "FinalizeScenarioRequestMessage::Validate - command_identifier_ "
        "does not match expected value for FinalizeScenarioRequestMessage: " +
        std::to_string(command_identifier_));
  }
  if (type_ != MusicMessageType::kFinalizeScenarioRequest) {
    LogError(
        "FinalizeScenarioRequestMessage::Validate - type_ does not match "
        "expected value for FinalizeScenarioRequestMessage: " +
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
  MusicCommandRequestMessage::Validate();
}
// JSON Serialization
void to_json(nlohmann::json &j, const FinalizeScenarioRequestMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // no new members to append
}

void from_json(const nlohmann::json &j,
               FinalizeScenarioRequestMessage &message) {
  // fill out base class members
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
}

}  // namespace music
