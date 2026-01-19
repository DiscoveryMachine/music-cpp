// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/scenario/scenario_start_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void ScenarioStartRequestMessage::Validate() const {
  if (type_ != MusicMessageType::kScenarioStart) {
    LogError(
        "ScenarioStartRequestMessage::Validate - type_ incorrect for "
        "ScenarioStartRequestMessage");
  }
  if (command_identifier_ !=
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kScenarioStart)
          .action_type) {
    LogError(
        "ScenarioStartRequestMessage::Validate - command_identifier_ "
        "incorrect for ScenarioStartRequestMessage");
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
void to_json(nlohmann::json &j, const ScenarioStartRequestMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // no new members to append
}

void from_json(const nlohmann::json &j, ScenarioStartRequestMessage &message) {
  // fill out base class members
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
  // no new members to fill out
}

}  // namespace music