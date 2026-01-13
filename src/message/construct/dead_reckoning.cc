// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/construct/dead_reckoning.h"

using json = nlohmann::json;
namespace music {

void to_json(nlohmann::json &jsn, const DeadReckoning &dr) {
  jsn = {{DeadReckoning::kAlgorithmTypeKey, dr.dead_reck_algorithm_},
         {DeadReckoning::kAngularVelocityKey, dr.angular_velocity_},
         {DeadReckoning::kLinearAccelerationkey, dr.linear_acceleration_}};
}

void from_json(const json &jsn, DeadReckoning &dr) {
  jsn.at(DeadReckoning::kAlgorithmTypeKey).get_to(dr.dead_reck_algorithm_);
  jsn.at(DeadReckoning::kAngularVelocityKey).get_to(dr.angular_velocity_);
  jsn.at(DeadReckoning::kLinearAccelerationkey).get_to(dr.linear_acceleration_);
}

void DeadReckoning::Validate() const {
  angular_velocity_.Validate();
  linear_acceleration_.Validate();
}
}  // namespace music