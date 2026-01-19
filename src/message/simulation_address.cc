// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/simulation_address.h"

#include "util/validate.h"
#include <string>

namespace music {
void SimulationAddress::Validate() const {
  if (site_ == std::numeric_limits<int>::min()) {
    LogError("EntityId::validate - site_id_ is not set (min int)");
  }

  if (application_ == std::numeric_limits<int>::min()) {
    LogError("EntityId::validate - app_id_ is not set (min int)");
  }

  std::string address_range =
      "(" + std::to_string(kLowestAddress) + "-" + std::to_string(kHighestAddress) + ")";

  if (site_ < kLowestAddress || site_ > kHighestAddress) {
    LogError("EntityId::validate - site_id_ out of range " + address_range + ": " +
             std::to_string(site_));
  }

  if (application_ < kLowestAddress || application_ > kHighestAddress) {
    LogError("EntityId::validate - application_ out of range " + address_range + ": " +
             std::to_string(application_));
  }
}
} // namespace music
