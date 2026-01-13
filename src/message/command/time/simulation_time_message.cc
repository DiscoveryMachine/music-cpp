// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/time/simulation_time_message.h"
#include "util/validate.h"

namespace music {

// Validate
void SimulationTimeMessage::Validate() const {
  if (type_ != MusicMessageType::kSimulationTime) {
    LogError(
        "SimulationTimeMessage::Validate - type_ must be "
        "kSimulationTime");
  }
  if (command_identifier_ !=
      MusicMessageTypeHelper::GetInfoForType(type_).action_type) {
    LogError(
        "SimulationTimeMessage::Validate - command_identifier_ does not "
        "match type_: " +
        std::to_string(command_identifier_));
  }
  if (sim_time_ < 0 || sim_time_ > std::numeric_limits<unsigned int>::max()) {
    LogError(
        "SimulationTimeMessage::Validate - sim_time_ cannot be "
        "out of valid range [0, 4294967295]: " +
        std::to_string(sim_time_));
  }
  MusicCommandResponseMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const SimulationTimeMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandResponseMessage &>(message));
  j.update({{SimulationTimeMessage::kSimTimeKey, message.sim_time_}});
}

void from_json(const nlohmann::json &j, SimulationTimeMessage &message) {
  // extract super class json
  from_json(j, static_cast<MusicCommandResponseMessage &>(message));
  // Extract SimulationTimeMessage fields
  j.at(SimulationTimeMessage::kSimTimeKey).get_to(message.sim_time_);
}

}  // namespace music
