// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>

#include "enum/fuse_type.h"
#include "util/validate.h"

namespace music {

bool FuseTypeHelper::IsValid(int value) {
  static constexpr int valid_values[] = {
      10,   20,   30,   40,   50,   60,   100,  200,  300,  1000, 1100, 1200,
      1300, 1400, 1500, 1600, 1700, 1800, 1900, 1910, 1920, 1930, 1940, 1950,
      1960, 1970, 1980, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800,
      2900, 2910, 2920, 2930, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700,
      4000, 4100, 5000, 5100, 5200, 6000, 7000, 8000, 8010, 8100, 8110, 8120,
      8130, 8150, 9000, 9010, 9100, 9110, 9120, 9200, 9210, 9220, 9300, 9310,
      9400, 9410, 9500, 9510, 9520, 9530, 9600, 9610, 9620};

  return std::find(std::begin(valid_values), std::end(valid_values), value) !=
         std::end(valid_values);
}

FuseType FuseTypeHelper::FromInt(int value) {
  if (!IsValid(value)) {
    LogError("Invalid FuseType value: " + std::to_string(value));
  }
  return static_cast<FuseType>(value);
}

}  // namespace music
