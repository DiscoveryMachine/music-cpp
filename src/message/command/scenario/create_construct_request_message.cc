// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/Orientation.h"
#include "message/command/scenario/create_construct_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void CreateConstructRequestMessage::Validate() const {
  if (construct_type_.empty()) {
    LogError(
        "CreateConstructRequestMessage::Validate - construct_type_ cannot "
        "be empty");
  }
  if (callsign_.empty()) {
    LogError(
        "CreateConstructRequestMessage::Validate - callsign_ cannot be "
        "empty");
  }
  if (construct_location_.has_value()) {
    construct_location_.value().Validate();
  }
  if (construct_orientation_.has_value()) {
    construct_orientation_.value().Validate();
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

void to_json(nlohmann::json &j, const CreateConstructRequestMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));

  // append derived class members
  j.update(nlohmann::json{{CreateConstructRequestMessage::kConstructTypeKey,
                           message.construct_type_},
                          {CreateConstructRequestMessage::kConstructCallsignKey,
                           message.callsign_}});
  if (message.construct_location_.has_value()) {
    j.update({{CreateConstructRequestMessage::kConstructLocationKey,
               message.construct_location_.value()}});
    message.construct_location_.value();
  }
  if (message.construct_orientation_.has_value()) {
    j.update({{CreateConstructRequestMessage::kConstructOrientationKey,
               message.construct_orientation_.value()}});
  }
}

void from_json(const nlohmann::json &j,
               CreateConstructRequestMessage &message) {
  // fill out super class members
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));

  // Add derived class members
  j.at(CreateConstructRequestMessage::kConstructLocationKey)
      .get_to(message.construct_location_);
  j.at(CreateConstructRequestMessage::kConstructOrientationKey)
      .get_to(message.construct_orientation_);
  j.at(CreateConstructRequestMessage::kConstructTypeKey)
      .get_to(message.construct_type_);
  j.at(CreateConstructRequestMessage::kConstructCallsignKey)
      .get_to(message.callsign_);
}
}  // namespace music