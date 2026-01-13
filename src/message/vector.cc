// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <nlohmann/json.hpp>

#include "message/vector.h"
#include "util/validate.h"

namespace music {
// Validate
void Vector::Validate() const {
  if (std::isnan(x_)) {
    LogError("Vector::validate - x_ is not set (NaN)");
  }

  if (std::isnan(y_)) {
    LogError("Vector::validate - y_ is not set (NaN)");
  }

  if (std::isnan(z_)) {
    LogError("Vector::validate - z_ is not set (NaN)");
  }
}

// JSON serialization
void to_json(nlohmann::json &j, const Vector &vector) {
  j = {{"x", vector.x_}, {"y", vector.y_}, {"z", vector.z_}};
}

void from_json(const nlohmann::json &j, Vector &vector) {
  j.at(Vector::kXKey).get_to(vector.x_);
  j.at(Vector::kYKey).get_to(vector.y_);
  j.at(Vector::kZKey).get_to(vector.z_);
}
}  // namespace music