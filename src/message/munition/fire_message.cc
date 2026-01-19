// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/munition/fire_message.h"
#include "util/validate.h"

namespace music {
// Validate
void FireMessage::Validate() const {
  if (range_to_target_ < 0.0) {
    LogError("FireMessage::Validate - range_to_target_ cannot be negative: " +
             std::to_string(range_to_target_));
  }
  if (type_ != MusicMessageType::kFire) {
    LogError("FireMessage::Validate - type_ must be kFire");
  }
  MunitionMessage::Validate();
}

void to_json(nlohmann::json &j, const FireMessage &message) {
  // Serialize super class
  to_json(j, static_cast<const MunitionMessage &>(message));

  // Append FireMessage fields
  j.update(nlohmann::json{
      {FireMessage::kRateKey, message.rate_},
      {FireMessage::kQuantityKey, message.quantity_},
      {FireMessage::kRangeToTargetKey, message.range_to_target_}});
}
void from_json(const nlohmann::json &j, FireMessage &message) {
  // Deserialize super class
  from_json(j, static_cast<MunitionMessage &>(message));

  // Extract FireMessage fields
  j.at(FireMessage::kRateKey).get_to(message.rate_);
  j.at(FireMessage::kQuantityKey).get_to(message.quantity_);
  j.at(FireMessage::kRangeToTargetKey).get_to(message.range_to_target_);
}

}  // namespace music
