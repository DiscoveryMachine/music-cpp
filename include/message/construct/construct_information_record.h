// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_CONSTRUCT_INFORMATION_RECORD_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_CONSTRUCT_INFORMATION_RECORD_H_

#include <nlohmann/json.hpp>
#include <string>

#include "enum/construct_category_type.h"
#include "enum/construct_render_type.h"

namespace music {
// Stores details of a Constructs information. Generally rendering and category.
class ConstructInformationRecord {
  friend class ConstructInformationRecordBuilder;

 public:
  static inline const std::string kConstructRenderTypeKey = "constructRender";
  static inline const std::string kConstructCategoryKey = "constructType";

  // Accessors for constructRender
  const ConstructRenderType GetConstructRender() const {
    return construct_render_type_;
  }
  const ConstructCategoryType GetConstructCategory() const {
    return construct_category_;
  }

  void SetConstructRender(const ConstructRenderType render_type) {
    construct_render_type_ = render_type;
  }
  void SetConstructCategory(const ConstructCategoryType &value) {
    construct_category_ = value;
  }

  friend void to_json(nlohmann::json &jsn,
                      const ConstructInformationRecord &record);
  friend void from_json(const nlohmann::json &jsn,
                        ConstructInformationRecord &record);

  void Validate() const;

  bool operator==(const music::ConstructInformationRecord &other) const {
    return other.construct_category_ == construct_category_ &&
           other.construct_render_type_ == construct_render_type_;
  }

  bool operator!=(const music::ConstructInformationRecord &other) const {
    return !(other == *this);
  }

 protected:
  // Signifies how the construct should be rendered.
  ConstructRenderType construct_render_type_{ConstructRenderType::kUnset};

  // The category of the construct. Sometimes referred to as construct type.
  ConstructCategoryType construct_category_{ConstructCategoryType::kUnset};
};

class ConstructInformationRecordBuilder {
 private:
  ConstructInformationRecord info_record_;

 public:
  ConstructInformationRecordBuilder() {
    info_record_ = ConstructInformationRecord();
  }

  ConstructInformationRecordBuilder &SetConstructRenderType(
      ConstructRenderType render_type) {
    info_record_.construct_render_type_ = render_type;
    return *this;
  }
  ConstructInformationRecordBuilder &SetConstructCategoryType(
      ConstructCategoryType category_type) {
    info_record_.construct_category_ = category_type;
    return *this;
  }

  ConstructInformationRecord Build() { return info_record_; }
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_CONSTRUCT_INFORMATION_RECORD_H_