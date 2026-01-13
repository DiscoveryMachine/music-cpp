// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>

#include "enum/warhead.h"
#include "util/validate.h"

namespace music {

bool WarheadHelper::IsValid(int value) {
  static constexpr int valid_values[] = {
      10,   20,   30,   31,   32,   33,   1000, 1100, 1200, 1300, 1400, 1500,
      1600, 1610, 1615, 1620, 1625, 1630, 1635, 1640, 1645, 1650, 1655, 1660,
      1665, 1670, 1675, 1680, 1685, 2000, 3000, 4000, 5000, 6000, 7000, 7010,
      8000, 8100, 8110, 8115, 8120, 8200, 8210, 8215, 8220, 8300, 8310, 8315,
      8320, 8325, 8330, 8335, 8340, 8345, 8350, 8355, 8360, 8365, 8370, 8375,
      8380, 9000, 9100, 9200, 9300, 9400, 9500};

  return std::find(std::begin(valid_values), std::end(valid_values), value) !=
         std::end(valid_values);
}

Warhead WarheadHelper::FromInt(int value) {
  if (!IsValid(value)) {
    LogError("Invalid Warhead value: " + std::to_string(value));
  }
  return static_cast<Warhead>(value);
}

}  // namespace music
