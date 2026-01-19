// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_VECTOR_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_VECTOR_H_

#include <limits>
#include <nlohmann/json.hpp>

namespace music {
class Vector {
  friend class VectorBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kXKey = "x";
  inline static const std::string kYKey = "y";
  inline static const std::string kZKey = "z";

  // Getters
  const float GetX() const { return x_; }
  const float GetY() const { return y_; }
  const float GetZ() const { return z_; }

  // Setters
  void SetX(float x) { x_ = x; }
  void SetY(float y) { y_ = y; }
  void SetZ(float z) { z_ = z; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const Vector &vector);
  friend void from_json(const nlohmann::json &j, Vector &vector);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static Vector FromJson(const nlohmann::json &j) {
    Vector vector;
    from_json(j, vector);
    return vector;
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector &vector) {
    os << vector.ToString();
    return os;
  }

  std::string ToString() const { return "Vector: " + ToJson().dump(); }

  bool operator==(const music::Vector &other) const {
    return std::abs(other.x_ - x_) < std::numeric_limits<float>::epsilon() &&
           std::abs(other.y_ - y_) < std::numeric_limits<float>::epsilon() &&
           std::abs(other.z_ - z_) < std::numeric_limits<float>::epsilon();
  }
  bool operator!=(const music::Vector &other) const {
    return !(other == *this);
  }

 protected:
  // The x component of the vector.
  float x_{std::numeric_limits<float>::quiet_NaN()};

  // The y component of the vector.
  float y_{std::numeric_limits<float>::quiet_NaN()};

  // The z component of the vector.
  float z_{std::numeric_limits<float>::quiet_NaN()};
};

// Builder
class VectorBuilder {
 public:
  // Setters that return 'this' (the builder itself)
  VectorBuilder &SetX(float x) {
    vector_.x_ = x;
    return *this;
  }
  VectorBuilder &SetY(float y) {
    vector_.y_ = y;
    return *this;
  }
  VectorBuilder &SetZ(float z) {
    vector_.z_ = z;
    return *this;
  }

  // The final 'build' method that creates the Vector object
  Vector Build() const { return vector_; }

 protected:
  Vector vector_;
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_VECTOR_H_
