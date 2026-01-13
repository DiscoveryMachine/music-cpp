// Copyright (c) 2025 Discovery Machine®, Inc.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUNITION_FIRE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUNITION_FIRE_MESSAGE_H_

#include "message/munition/munition_message.h"

namespace music {

class FireMessage : public MunitionMessage {
  template <class T>
  friend class FireMessageBuilderTemplate;

 public:
  // Constructor
  FireMessage() : MunitionMessage() { type_ = MusicMessageType::kFire; }
  // Validate
  void Validate() const;

  // Getters
  const double GetRangeToTarget() const { return range_to_target_; }

  // Setters

  void SetRangeToTarget(double range_to_target) {
    range_to_target_ = range_to_target;
  }

  // JSON Keys
  inline static const std::string kRangeToTargetKey = "rangeToTarget";

  // JSON serialization
  friend void to_json(nlohmann::json &j, const FireMessage &message);
  friend void from_json(const nlohmann::json &j, FireMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static FireMessage FromJson(const nlohmann::json &j) {
    FireMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const FireMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const { return "FireMessage: " + ToJson().dump(); }

 protected:
  // This field shall specify the range that an entity's fire control system has
  // assumed in computing the fire control solution.This field shall be
  // represented by a 32 -bit floating point number in meters. For systems where
  // range is unknown or unavailable, this field shall contain a value of zero.
  double range_to_target_{std::numeric_limits<double>::quiet_NaN()};
};

// Template builder class
template <class T>
class FireMessageBuilderTemplate : public MunitionMessageBuilderTemplate<T> {
 protected:
  std::shared_ptr<FireMessage> fire_message_;
  void SetBuiltMessage(std::shared_ptr<MusicMessage> new_message) {
    fire_message_ = std::static_pointer_cast<FireMessage>(new_message);
    MunitionMessageBuilderTemplate<T>::SetBuiltMessage(new_message);
  }

 public:
  T &SetRangeToTarget(double range_to_target) {
    fire_message_->range_to_target_ = range_to_target;
    return static_cast<T &>(*this);
  }
};

class FireMessageBuilder
    : public FireMessageBuilderTemplate<FireMessageBuilder> {
 public:
  FireMessageBuilder() { SetBuiltMessage(std::make_shared<FireMessage>()); }

  FireMessage Build() { return *fire_message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUNITION_FIRE_MESSAGE_H_