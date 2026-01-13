// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/time/set_simulation_time_message.h"
#include "util/validate.h"

namespace music {

// Validate
void SetSimulationTimeMessage::Validate() const {
  if (type_ != MusicMessageType::kSetSimulationTime) {
    LogError(
        "SetSimulationTimeMessage::Validate - type_ must be "
        "kSetSimulationTime");
  }
  if (new_time_ < 0 || new_time_ > std::numeric_limits<unsigned int>::max()) {
    LogError(
        "SetSimulationTimeMessage::Validate - new_time_ cannot be "
        "out of valid range [0, 4294967295]: " +
        std::to_string(new_time_));
  }
  MusicCommandRequestMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const SetSimulationTimeMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // Append SetSimulationTimeMessage fields
  j.update({{SetSimulationTimeMessage::kNewTimeKey, message.new_time_}});
}

void from_json(const nlohmann::json &j, SetSimulationTimeMessage &message) {
  // extract super class json
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
  // Extract SetSimulationTimeMessage fields
  j.at(SetSimulationTimeMessage::kNewTimeKey).get_to(message.new_time_);
}

}  // namespace music