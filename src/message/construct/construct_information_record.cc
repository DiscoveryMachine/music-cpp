// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/construct/construct_information_record.h"
#include "util/validate.h"

using json = nlohmann::json;

namespace music {
void to_json(json &jsn, const ConstructInformationRecord &record) {
  jsn = {{ConstructInformationRecord::kConstructCategoryKey,
          record.construct_category_},
         {ConstructInformationRecord::kConstructRenderTypeKey,
          record.construct_render_type_}};
}

void from_json(const nlohmann::json &jsn, ConstructInformationRecord &record) {
  jsn.at(ConstructInformationRecord::kConstructCategoryKey)
      .get_to(record.construct_category_);
  jsn.at(ConstructInformationRecord::kConstructRenderTypeKey)
      .get_to(record.construct_render_type_);
}

void ConstructInformationRecord::Validate() const {
  if (construct_category_ == ConstructCategoryType::kUnset) {
    LogError(
        "No construct category was set for a "
        "music::ConstructInformationRecord");
  }
  if (construct_render_type_ == ConstructRenderType::kUnset) {
    LogError(
        "No construct render type was set for a "
        "music::ConstructInformationRecord");
  }
}

}  // namespace music