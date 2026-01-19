// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <nlohmann/json.hpp>
#include <string>

#include "message/entity_id.h"
#include "message/simulation_address.h"
#include "util/validate.h"


namespace music {

void EntityId::Validate() const {
  sim_addr_.Validate();

  if (entity_number_ == std::numeric_limits<int>::min()) {
    LogError("EntityId::validate - entity_number_ is not set (min int)");
  }

  std::string entity_number_range = "(" + std::to_string(kLowestEntityNumber) +
                                    "-" + std::to_string(kHighestEntityNumber) +
                                    ")";

  if (entity_number_ < kLowestEntityNumber ||
      entity_number_ > kHighestEntityNumber) {
    LogError("EntityId::validate - entity_number_ out of range " +
             entity_number_range + ": " + std::to_string(entity_number_));
  }
}

void to_json(nlohmann::json &j, const EntityId &entity_id) {
  j.update(entity_id.sim_addr_.ToJson());
  j[EntityId::kEntityNumberJsonKey] = entity_id.entity_number_;
};

void from_json(const nlohmann::json &j, EntityId &entity_id) {
  entity_id.sim_addr_ = SimulationAddress::FromJson(j);
  entity_id.entity_number_ = j[EntityId::kEntityNumberJsonKey];
}

}  // namespace music