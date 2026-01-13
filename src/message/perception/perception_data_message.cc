// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/perception/perception_data_message.h"
#include "util/validate.h"

namespace music {

// Validate
void PerceptionDataMessage::Validate() const {
  if (num_perceptions_ < 0) {
    LogError(
        "PerceptionDataMessage::Validate - num_perceptions_ cannot be "
        "negative: " +
        std::to_string(num_perceptions_));
  }

  if (static_cast<size_t>(num_perceptions_) != perception_records_.size()) {
    LogError(
        "PerceptionDataMessage::Validate - num_perceptions_ does not match "
        "size of perception_records_: " +
        std::to_string(num_perceptions_) + " vs " +
        std::to_string(perception_records_.size()));
  }

  for (const auto &record : perception_records_) {
    record.Validate();
  }

  origin_id_.Validate();

  MusicMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const PerceptionDataMessage &message) {
  // Serialize super class
  to_json(j, static_cast<const MusicMessage &>(message));

  // Append PerceptionDataMessage fields
  j.update(nlohmann::json{
      {PerceptionDataMessage::kOriginIdKey, message.origin_id_},
      {PerceptionDataMessage::kNumPerceptionsKey, message.num_perceptions_},
      {PerceptionDataMessage::kPerceptionRecordsKey,
       message.perception_records_}});
}

void from_json(const nlohmann::json &j, PerceptionDataMessage &message) {
  // Deserialize super class
  from_json(j, static_cast<MusicMessage &>(message));
  // Extract PerceptionDataMessage fields
  j.at(PerceptionDataMessage::kOriginIdKey).get_to(message.origin_id_);
  j.at(PerceptionDataMessage::kNumPerceptionsKey)
      .get_to(message.num_perceptions_);
  j.at(PerceptionDataMessage::kPerceptionRecordsKey)
      .get_to(message.perception_records_);
}

}  // namespace music