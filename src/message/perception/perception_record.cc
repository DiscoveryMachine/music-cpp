// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/perception/perception_record.h"
#include "util/validate.h"

namespace music {

// Validate
void PerceptionRecord::Validate() const {
  perception_id_.Validate();

  // Force enum validation
  if (force_ < Force::kOtherUnknown || force_ > Force::kNeutral) {
    LogError("PerceptionRecord::Validate - force_ has invalid value: " +
             std::to_string(static_cast<int>(force_)));
  }

  // SensorType enum validation
  if (system_ <= SensorType::kOtherUnknown || system_ > SensorType::kAuditory) {
    LogError("PerceptionRecord::Validate - system_ has invalid value: " +
             std::to_string(static_cast<int>(system_)));
  }

  if (accuracy_ < 0.0 || accuracy_ > 1.0) {
    LogError(
        "PerceptionRecord::Validate - accuracy_ out of range [0.0, 1.0]: " +
        std::to_string(accuracy_));
  }
  if (bearing_factor_ < 0) {
    LogError(
        "PerceptionRecord::Validate - bearing_factor_ cannot be negative: " +
        std::to_string(bearing_factor_));
  }
  if (altitude_error_ < 0) {
    LogError(
        "PerceptionRecord::Validate - altitude_error_ cannot be negative: " +
        std::to_string(altitude_error_));
  }
  if (range_error_ < 0) {
    LogError("PerceptionRecord::Validate - range_error_ cannot be negative: " +
             std::to_string(range_error_));
  }

  entity_type_.Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const PerceptionRecord &record) {
  j = nlohmann::json{
      {PerceptionRecord::kPerceptionIdKey, record.perception_id_},
      {PerceptionRecord::kForceKey, static_cast<int>(record.force_)},
      {PerceptionRecord::kEntityTypeKey, record.entity_type_},
      {PerceptionRecord::kSystemKey, static_cast<int>(record.system_)},
      {PerceptionRecord::kAccuracyKey, record.accuracy_},
      {PerceptionRecord::kBearingFactorKey, record.bearing_factor_},
      {PerceptionRecord::kAltitudeErrorKey, record.altitude_error_},
      {PerceptionRecord::kRangeErrorKey, record.range_error_}};
}

void from_json(const nlohmann::json &j, PerceptionRecord &record) {
  j.at(PerceptionRecord::kPerceptionIdKey).get_to(record.perception_id_);
  j.at(PerceptionRecord::kForceKey).get_to(record.force_);
  j.at(PerceptionRecord::kEntityTypeKey).get_to(record.entity_type_);
  j.at(PerceptionRecord::kSystemKey).get_to(record.system_);
  j.at(PerceptionRecord::kAccuracyKey).get_to(record.accuracy_);
  j.at(PerceptionRecord::kBearingFactorKey).get_to(record.bearing_factor_);
  j.at(PerceptionRecord::kAltitudeErrorKey).get_to(record.altitude_error_);
  j.at(PerceptionRecord::kRangeErrorKey).get_to(record.range_error_);
}

}  // namespace music