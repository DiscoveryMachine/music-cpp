// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <cmath>
#include <nlohmann/json.hpp>

#include "message/orientation.h"
#include "util/validate.h"

namespace music {

void Orientation::Validate() const {
  if (std::isnan(phi_)) {
    LogError("Orientation::validate - phi_ is not set (NaN)");
  }

  if (std::isnan(psi_)) {
    LogError("Orientation::validate - psi_ is not set (NaN)");
  }

  if (std::isnan(theta_)) {
    LogError("Orientation::validate - theta_ is not set (NaN)");
  }

  if (psi_ < -M_PI || psi_ > M_PI) {
    LogError("Orientation::validate - psi_ out of range [-π, π]: " +
             std::to_string(psi_));
  }

  if (theta_ < -M_PI_2 || theta_ > M_PI_2) {
    LogError("Orientation::validate - theta_ out of range [-π/2, π/2]: " +
             std::to_string(theta_));
  }

  if (phi_ < -M_PI || phi_ > M_PI) {
    LogError("Orientation::validate - phi_ out of range [-π, π]: " +
             std::to_string(phi_));
  }
}

// JSON serialization
void to_json(nlohmann::json &j, const Orientation &orientation) {
  j = nlohmann::json{{Orientation::kPsiKey, orientation.psi_},
                     {Orientation::kThetaKey, orientation.theta_},
                     {Orientation::kPhiKey, orientation.phi_}};
}

void from_json(const nlohmann::json &j, Orientation &orientation) {
  j.at(Orientation::kPsiKey).get_to(orientation.psi_);
  j.at(Orientation::kThetaKey).get_to(orientation.theta_);
  j.at(Orientation::kPhiKey).get_to(orientation.phi_);
}
}  // namespace music