// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <nlohmann/json.hpp>

#include "message/location.h"
#include "util/validate.h"

namespace music {

void Location::Validate() const {
  if (std::isnan(x_)) {
    LogError("Location::validate - x_ is not set (NaN)");
  }

  if (std::isnan(y_)) {
    LogError("Location::validate - y_ is not set (NaN)");
  }

  if (std::isnan(z_)) {
    LogError("Location::validate - z_ is not set (NaN)");
  }
}

// JSON serialization
void to_json(nlohmann::json& j, const Location& location) {
  j = nlohmann::json{{Location::kXKey, location.x_},
                     {Location::kYKey, location.y_},
                     {Location::kZKey, location.z_}};
}

void from_json(const nlohmann::json& j, Location& location) {
  j.at(Location::kXKey).get_to(location.x_);
  j.at(Location::kYKey).get_to(location.y_);
  j.at(Location::kZKey).get_to(location.z_);
}

}  // namespace music