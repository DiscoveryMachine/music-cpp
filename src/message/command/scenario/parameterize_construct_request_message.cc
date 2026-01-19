// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/scenario/parameterize_construct_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void ParameterizeConstructRequestMessage::Validate() const {
  if (type_ != MusicMessageType::kParameterizeConstructRequest) {
    LogError(
        "ParameterizeConstructRequestMessage::Validate - type_ incorrect for "
        "ParameterizeConstructRequestMessage");
  }

  if (command_identifier_ !=
      MusicMessageTypeHelper::GetInfoForType(
          MusicMessageType::kParameterizeConstructRequest)
          .action_type) {
    LogError(
        "ParameterizeConstructRequestMessage::Validate - "
        "command_identifier_ incorrect for "
        "ParameterizeConstructRequestMessage");
  }
  if (construct_parameters_.is_null() || construct_parameters_.empty()) {
    LogError(
        "ParameterizeConstructRequestMessage::Validate - construct_parameters_ "
        "is null or empty for "
        "ParameterizeConstructRequestMessage");
  }
  if (ghosted_id_.has_value()) {
    ghosted_id_.value().Validate();
  }
  if (origin_id_.GetEntityNumber() != 0) {
    LogError(
        "ParameterizeConstructRequestMessage::Validate - origin_id_ entity "
        "number "
        "must be 0 for system-origin messages");
  }
  MusicCommandRequestMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j,
             const ParameterizeConstructRequestMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // fill out derived class members
  j.update({
      {ParameterizeConstructRequestMessage::kConstructParametersKey,
       message.GetConstructParameters()},
  });
  if (message.ghosted_id_.has_value()) {
    j.update({{ParameterizeConstructRequestMessage::kGhostedIdKey,
               message.ghosted_id_.value()}});
  }
}

void from_json(const nlohmann::json &j,
               ParameterizeConstructRequestMessage &message) {
  // fill out base class members
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
  // fill out derived class members
  j.at(ParameterizeConstructRequestMessage::kConstructParametersKey)
      .get_to(message.construct_parameters_);
  j.at(ParameterizeConstructRequestMessage::kGhostedIdKey)
      .get_to(message.ghosted_id_);
}

}  // namespace music