// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <nlohmann/json.hpp>

#include "message/entity_type.h"
#include "util/validate.h"

namespace music {

void EntityType::Validate() const {
  if (kind_ < 0 || kind_ > std::numeric_limits<unsigned char>::max()) {
    LogError("EntityType::validate - kind_ out of range (0-255): " +
             std::to_string(kind_));
  }
  if (domain_ < 0 || domain_ > std::numeric_limits<unsigned char>::max()) {
    LogError("EntityType::validate - domain_ out of range (0-255): " +
             std::to_string(domain_));
  }
  if (country_ < 0 || country_ > std::numeric_limits<unsigned short>::max()) {
    LogError("EntityType::validate - country_ out of range (0-65535): " +
             std::to_string(country_));
  }
  if (category_ < 0 || category_ > std::numeric_limits<unsigned char>::max()) {
    LogError("EntityType::validate - category_ out of range (0-255): " +
             std::to_string(category_));
  }
  if (subcategory_ < 0 ||
      subcategory_ > std::numeric_limits<unsigned char>::max()) {
    LogError("EntityType::validate - subcategory_ out of range (0-255): " +
             std::to_string(subcategory_));
  }
  if (specific_ < 0 || specific_ > std::numeric_limits<unsigned char>::max()) {
    LogError("EntityType::validate - specific_ out of range (0-255): " +
             std::to_string(specific_));
  }
  if (extra_ < 0 || extra_ > std::numeric_limits<unsigned char>::max()) {
    LogError("EntityType::validate - extra_ out of range (0-255): " +
             std::to_string(extra_));
  }
}

void to_json(nlohmann::json &j, const EntityType &entity_type) {
  j = {
      {EntityType::kKindKey, entity_type.kind_},
      {EntityType::kDomainKey, entity_type.domain_},
      {EntityType::kCountryKey, entity_type.country_},
      {EntityType::kCategoryKey, entity_type.category_},
      {EntityType::kSubcategoryKey, entity_type.subcategory_},
      {EntityType::kSpecificKey, entity_type.specific_},
      {EntityType::kExtraKey, entity_type.extra_},
  };
}
void from_json(const nlohmann::json &j, EntityType &entity_type) {
  j.at(EntityType::kKindKey).get_to(entity_type.kind_);
  j.at(EntityType::kDomainKey).get_to(entity_type.domain_);
  j.at(EntityType::kCountryKey).get_to(entity_type.country_);
  j.at(EntityType::kCategoryKey).get_to(entity_type.category_);
  j.at(EntityType::kSubcategoryKey).get_to(entity_type.subcategory_);
  j.at(EntityType::kSpecificKey).get_to(entity_type.specific_);
  j.at(EntityType::kExtraKey).get_to(entity_type.extra_);
}

}  // namespace music