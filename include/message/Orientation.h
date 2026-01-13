// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ORIENTATION_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ORIENTATION_H_

#include <nlohmann/json.hpp>
#include <string>

namespace music {
class Orientation {
  friend class OrientationBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kPsiKey = "psi";
  inline static const std::string kThetaKey = "theta";
  inline static const std::string kPhiKey = "phi";

  // Getters
  const double GetPsi() const { return psi_; }
  const double GetTheta() const { return theta_; }
  const double GetPhi() const { return phi_; }

  // Setters
  void SetPsi(double psi) { psi_ = psi; }
  void SetTheta(double theta) { theta_ = theta; }
  void SetPhi(double phi) { phi_ = phi; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const Orientation &orientation);
  friend void from_json(const nlohmann::json &j, Orientation &orientation);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static Orientation FromJson(const nlohmann::json &j) {
    Orientation orientation;
    from_json(j, orientation);
    return orientation;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const Orientation &orientation) {
    os << "Orientation: " << orientation.ToJson().dump();
    return os;
  }

  bool operator==(const music::Orientation &other) const {
    return other.phi_ == phi_ && other.psi_ == psi_ && other.theta_ == theta_;
  }
  bool operator!=(const music::Orientation &other) const {
    return !(other == *this);
  }

 protected:
  // The psi value in radians. valid: [-pi, pi]
  double psi_{std::numeric_limits<double>::quiet_NaN()};

  // The theta value in radians. valid: [-pi/2, pi/2]
  double theta_{std::numeric_limits<double>::quiet_NaN()};

  // The phi value in radians. valid: [-pi, pi]
  double phi_{std::numeric_limits<double>::quiet_NaN()};
};

// Builder
class OrientationBuilder {
 public:
  // Setters that return 'this' (the builder itself)
  OrientationBuilder &SetPsi(double psi) {
    orientation_.psi_ = psi;
    return *this;
  }
  OrientationBuilder &SetTheta(double theta) {
    orientation_.theta_ = theta;
    return *this;
  }
  OrientationBuilder &SetPhi(double phi) {
    orientation_.phi_ = phi;
    return *this;
  }

  // The final 'build' method that creates the Orientation object
  Orientation Build() const { return orientation_; }

 protected:
  Orientation orientation_{};
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ORIENTATION_H_
