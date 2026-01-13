// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/munition/detonation_message.h"
#include "util/validate.h"

namespace music {

// Validate
void DetonationMessage::Validate() const {
  if (result_ <= DetonationResultType::kOtherUnknown ||
      result_ > DetonationResultType::kAirBust) {
    LogError("DetonationMessage::Validate - result_ out of range: " +
             std::to_string(static_cast<int>(result_)));
  }
  if (type_ != MusicMessageType::kDetonation) {
    LogError("DetonationMessage::Validate - type_ must be kDetonation");
  }
  MunitionMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const DetonationMessage &msg) {
  to_json(j, static_cast<const MunitionMessage &>(msg));
  j.update({{DetonationMessage::kResultKey, static_cast<int>(msg.result_)}});
}

void from_json(const nlohmann::json &j, DetonationMessage &msg) {
  from_json(j, static_cast<MunitionMessage &>(msg));
  msg.result_ = static_cast<DetonationResultType>(
      j.at(DetonationMessage::kResultKey).get<int>());
}

}  // namespace music
