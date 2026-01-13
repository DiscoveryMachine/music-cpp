// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <nlohmann/detail/conversions/to_json.hpp>
#include <nlohmann/json.hpp>

#include "message/music_message_header.h"
#include "util/validate.h"

namespace music {
// Validate
void MusicMessageHeader::Validate() const {
  if (exercise_id_ < 0 ||
      exercise_id_ > std::numeric_limits<unsigned char>::max()) {
    LogError(
        "MusicMessageHeader::validate - exercise_id_ out of range (0-255): " +
        std::to_string(exercise_id_));
  }
  if (timestamp_ < 0) {
    LogError("MusicMessageHeader::validate - timestamp_ cannot be negative: " +
             std::to_string(timestamp_));
  }
  if (timestamp_ > std::numeric_limits<std::uint64_t>::max()) {
    LogError(
        "MusicMessageHeader::validate - timestamp_ out of range "
        "(0-2^64-1): "
        "" +
        std::to_string(timestamp_));
  }
}

// JSON serialization
void from_json(const nlohmann::json &j, MusicMessageHeader &header) {
  j.at(MusicMessageHeader::kExerciseIdKey).get_to(header.exercise_id_);
  j.at(MusicMessageHeader::kTimestampKey).get_to(header.timestamp_);
}

void to_json(nlohmann::json &j, const MusicMessageHeader &header) {
  j = {{MusicMessageHeader::kExerciseIdKey, header.exercise_id_},
       {MusicMessageHeader::kTimestampKey, header.timestamp_}};
}

}  // namespace music