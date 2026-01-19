// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_DEAD_RECKONING_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_DEAD_RECKONING_H_

#include <string>

#include "enum/dead_reckoning_algorithm_type.h"
#include "message/Vector.h"

namespace music {

// The Dead Reckoning class represents dead reckoning information for an entity.
// which includes the algorithm and velocities used for dead reckoning.
class DeadReckoning {
  friend class DeadReckoningBuilder;

 public:
  friend void to_json(nlohmann::json &j, const DeadReckoning &deadReckoning);
  friend void from_json(const nlohmann::json &j, DeadReckoning &deadReckoning);
  static inline const std::string kAlgorithmTypeKey = "deadReckAlgorithm";
  static inline const std::string kAngularVelocityKey = "angularVelocity";
  static inline const std::string kLinearAccelerationkey = "linearAcceleration";

  const DeadReckoningAlgorithmType &GetDeadReckAlgorithm() const {
    return dead_reck_algorithm_;
  }
  const Vector &GetAngularVelocity() const { return angular_velocity_; }
  const Vector &GetLinearAcceleration() const { return linear_acceleration_; }

  void SetLinearAcceleration(const Vector acceleration) {
    linear_acceleration_ = acceleration;
  }
  void SetAngularVelocity(const Vector velocity) {
    angular_velocity_ = velocity;
  }
  void SetDeadReckAlgorithm(const DeadReckoningAlgorithmType algorithm_type) {
    dead_reck_algorithm_ = algorithm_type;
  }

  void Validate() const;

  bool operator==(const music::DeadReckoning &other) const {
    return other.dead_reck_algorithm_ == dead_reck_algorithm_ &&
           other.angular_velocity_ == angular_velocity_ &&
           other.linear_acceleration_ == linear_acceleration_;
  }

  bool operator!=(const music::DeadReckoning &other) const {
    return !(other == *this);
  }

 protected:
  DeadReckoningAlgorithmType dead_reck_algorithm_{
      DeadReckoningAlgorithmType::kUnset};

  Vector angular_velocity_{};

  Vector linear_acceleration_{};
};

class DeadReckoningBuilder {
 private:
  DeadReckoning dead_reck_;

 public:
  DeadReckoningBuilder() { dead_reck_ = DeadReckoning(); }
  DeadReckoningBuilder &SetAlgorithmType(
      DeadReckoningAlgorithmType algorithm_type) {
    dead_reck_.dead_reck_algorithm_ = algorithm_type;
    return *this;
  }
  DeadReckoningBuilder &SetAngularVelocity(Vector angular_velocity) {
    dead_reck_.angular_velocity_ = angular_velocity;
    return *this;
  }
  DeadReckoningBuilder &SetLinearAcceleration(Vector linear_acceleration) {
    dead_reck_.linear_acceleration_ = linear_acceleration;
    return *this;
  }
  DeadReckoning Build() { return dead_reck_; }
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_DEAD_RECKONING_H_