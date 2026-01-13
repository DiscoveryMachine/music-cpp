// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_DAMAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_DAMAGE_H_
#include <nlohmann/json.hpp>
#include <string>

namespace music {
class Damage {
  friend class MusicDamageBuilder;

 public:
  static inline const std::string kCatastrophicDamageKey = "catastrophicDamage";
  static inline const std::string kMobilityDamageKey = "mobilityDamage";
  static inline const std::string kFirepowerDisabledKey = "firepowerDisabled";

  friend void to_json(nlohmann::json &j, const Damage &damage);
  friend void from_json(const nlohmann::json &j, Damage &damage);

  const float GetCatastrophicDamage() const { return catastrophic_damage_; }

  const float GetMobilityDamage() const { return mobility_damage_; }

  const bool IsFirepowerDisabled() const { return firepower_disabled_; }

  void SetCatastrophicDamage(float damage) { catastrophic_damage_ = damage; }

  void SetMobilityDamage(float damage) { mobility_damage_ = damage; }
  void SetFirepowerDisabled(bool disabled) { firepower_disabled_ = disabled; }

  void Validate() const;
  bool operator==(const music::Damage &other) const {
    return std::abs(other.catastrophic_damage_ - catastrophic_damage_) <
               std::numeric_limits<float>::epsilon() &&
           std::abs(other.mobility_damage_ - mobility_damage_) <
               std::numeric_limits<float>::epsilon() &&
           other.firepower_disabled_ == firepower_disabled_;
  }

  bool operator!=(const music::Damage &other) const {
    return !(other == *this);
  }

 protected:
  float catastrophic_damage_{std::numeric_limits<float>::quiet_NaN()};

  float mobility_damage_{std::numeric_limits<float>::quiet_NaN()};

  bool firepower_disabled_{};
};
class MusicDamageBuilder {
 public:
  MusicDamageBuilder() { damage_ = Damage(); }
  MusicDamageBuilder &SetCatastrophicDamage(float new_damage) {
    damage_.catastrophic_damage_ = new_damage;
    return *this;
  }

  MusicDamageBuilder &SetMobilityDamage(float new_mobility_damage) {
    damage_.mobility_damage_ = new_mobility_damage;
    return *this;
  }

  MusicDamageBuilder &SetIsFirepowerDisabled(bool is_firepower_disabled) {
    damage_.firepower_disabled_ = is_firepower_disabled;
    return *this;
  }

  Damage Build() { return damage_; }

 protected:
  Damage damage_;
};

}  // namespace music
#endif