// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_EVENT_ID_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_EVENT_ID_H_
#include <cmath>
#include <limits>
#include <nlohmann/json.hpp>

#include "message/simulation_address.h"

namespace music {
class MusicEventId {
  friend class MusicEventIdBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  static inline const std::string kSiteIdKey = "siteID";
  static inline const std::string kApplicationIdKey = "appID";
  static inline const std::string kEventIdKey = "eventID";

  // Getters
  const int &GetEventId() const { return event_id_; }
  const SimulationAddress &GetSimulationAddress() const { return sim_address_; }

  // Setters
  void SetSimulationAddress(SimulationAddress address) {
    sim_address_ = address;
  }
  void SetEventId(int event_id) { event_id_ = event_id; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const MusicEventId &header);
  friend void from_json(const nlohmann::json &j, MusicEventId &header);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static MusicEventId FromJson(const nlohmann::json &j) {
    MusicEventId header;
    from_json(j, header);
    return header;
  }

  bool operator==(const music::MusicEventId &other) const {
    return other.sim_address_ == sim_address_ && other.event_id_ == event_id_;
  }
  bool operator!=(const music::MusicEventId &other) const {
    return !(other == *this);
  }

  // Stream operator and ToString
  friend std::ostream &operator<<(std::ostream &os,
                                  const MusicEventId &music_event_id) {
    os << music_event_id.ToString();
    return os;
  }

  std::string ToString() const { return "MusicEventId: " + ToJson().dump(); }

 protected:
  SimulationAddress sim_address_{};
  // The id of this event. Unsigned 16-bit integer
  int event_id_{std::numeric_limits<int>::min()};
};

// Builder for MusicEventId
class MusicEventIdBuilder {
 public:
  MusicEventIdBuilder &SetSimulationAddress(SimulationAddress sim_address) {
    music_event_id_.sim_address_ = sim_address;
    return *this;
  }
  MusicEventIdBuilder &SetEventId(int event_id) {
    music_event_id_.event_id_ = event_id;
    return *this;
  }
  MusicEventId Build() { return music_event_id_; }

 protected:
  MusicEventId music_event_id_;
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_EVENT_ID_H_