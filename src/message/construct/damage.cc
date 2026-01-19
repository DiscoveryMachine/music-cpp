// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <cmath>
#include <nlohmann/json.hpp>

#include "message/construct/damage.h"
#include "util/validate.h"

using json = nlohmann::json;

namespace music {

void to_json(json &j, const Damage &damage) {
  j = json{{Damage::kCatastrophicDamageKey, damage.catastrophic_damage_},
           {Damage::kMobilityDamageKey, damage.mobility_damage_},
           {Damage::kFirepowerDisabledKey, damage.firepower_disabled_}};
}

void from_json(const json &j, Damage &damage) {
  j.at(Damage::kCatastrophicDamageKey).get_to(damage.catastrophic_damage_);
  j.at(Damage::kMobilityDamageKey).get_to(damage.mobility_damage_);
  j.at(Damage::kFirepowerDisabledKey).get_to(damage.firepower_disabled_);
}

void Damage::Validate() const {
  if (std::isnan(catastrophic_damage_)) {
    LogError("Invalid catastrophic damage value in music::Damage!");
  }
  if (std::isnan(mobility_damage_)) {
    LogError("Invalid mobility damage value in music::Damage!");
  }
}
}  // namespace music