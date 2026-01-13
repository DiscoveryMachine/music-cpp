// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_MESSAGE_HEADER_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_MESSAGE_HEADER_H_

#include <nlohmann/json.hpp>
#include <string>

namespace music {
class MusicMessageHeader {
  friend class MusicMessageHeaderBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kExerciseIdKey = "exerciseID";
  inline static const std::string kTimestampKey = "timestamp";

  // Getters
  const int GetExerciseId() const { return exercise_id_; }
  const long GetTimestamp() const { return timestamp_; }

  // Setters
  void SetExerciseId(int exercise_id) { exercise_id_ = exercise_id; }
  void SetTimestamp(long timestamp) { timestamp_ = timestamp; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const MusicMessageHeader &header);
  friend void from_json(const nlohmann::json &j, MusicMessageHeader &header);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static MusicMessageHeader FromJson(const nlohmann::json &j) {
    MusicMessageHeader header;
    from_json(j, header);
    return header;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const MusicMessageHeader &header) {
    os << header.ToString();
    return os;
  }

  const std::string ToString() const {
    return "MusicMessageHeader: " + ToJson().dump();
  }

 protected:
  // Stores the Exercise ID of this field, which is a unique Identifier for each
  // unique exercise. 8 bit unsigned integer.
  int exercise_id_{std::numeric_limits<int>::min()};

  // Stores the time at which this message data was valid in milliseconds since
  // epoch. Unsigned 64-bit integer.
  long timestamp_{std::numeric_limits<long>::min()};
};

class MusicMessageHeaderBuilder {
 private:
  MusicMessageHeader message_header_;

 public:
  // Setters that return 'this' (the builder itself)
  MusicMessageHeaderBuilder &SetExerciseId(int exercise_id) {
    message_header_.exercise_id_ = exercise_id;
    return *this;
  }
  MusicMessageHeaderBuilder &SetTimestamp(long timestamp) {
    message_header_.timestamp_ = timestamp;
    return *this;
  }

  // The final 'build' method that creates the MusicMessageHeader object
  MusicMessageHeader Build() const { return message_header_; }
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_MESSAGE_HEADER_H_
