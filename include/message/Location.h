// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_LOCATION_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_LOCATION_H_

#include <nlohmann/json.hpp>
#include <string>

namespace music {
class Location {
  friend class LocationBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kXKey = "x";
  inline static const std::string kYKey = "y";
  inline static const std::string kZKey = "z";

  // Getters
  const double GetX() const { return x_; }
  const double GetY() const { return y_; }
  const double GetZ() const { return z_; }

  // Setters
  void SetX(double x) { x_ = x; }
  void SetY(double y) { y_ = y; }
  void SetZ(double z) { z_ = z; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const Location &location);
  friend void from_json(const nlohmann::json &j, Location &location);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static Location FromJson(const nlohmann::json &j) {
    Location location;
    from_json(j, location);
    return location;
  }

  friend std::ostream &operator<<(std::ostream &os, const Location &location) {
    os << location.ToString();
    return os;
  }

  std::string ToString() const { return "Location: " + ToJson().dump(); }

  bool operator==(const music::Location &other) const {
    return other.x_ == x_ && other.y_ == y_ && other.z_ == z_;
  }
  bool operator!=(const music::Location &other) const {
    return !(other == *this);
  }

 protected:
  // The x coordinate of the location.
  double x_{std::numeric_limits<double>::quiet_NaN()};

  // The y coordinate of the location.
  double y_{std::numeric_limits<double>::quiet_NaN()};

  // The z coordinate of the location.
  double z_{std::numeric_limits<double>::quiet_NaN()};
};

// Builder
class LocationBuilder {
 private:
  Location location_;

 public:
  // Setters that return 'this' (the builder itself)
  LocationBuilder &SetX(double x) {
    location_.x_ = x;
    return *this;
  }
  LocationBuilder &SetY(double y) {
    location_.y_ = y;
    return *this;
  }
  LocationBuilder &SetZ(double z) {
    location_.z_ = z;
    return *this;
  }

  // The final 'build' method that creates the Location object
  Location Build() const { return location_; }
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_LOCATION_H_
