// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/time/request_simulation_time_message.h"
#include "util/validate.h"

namespace music {
// Validate
void RequestSimulationTimeMessage::Validate() const {
  if (type_ != MusicMessageType::kRequestSimulationTime) {
    LogError(
        "RequestSimulationTimeMessage::Validate - type_ must be "
        "kRequestSimulationTime");
  }
  MusicCommandRequestMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const RequestSimulationTimeMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
}

void from_json(const nlohmann::json &j, RequestSimulationTimeMessage &message) {
  // extract super class json
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
}

}  // namespace music