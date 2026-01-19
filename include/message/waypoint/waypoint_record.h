// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_WAYPOINT_WAYPOINT_RECORD_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_WAYPOINT_WAYPOINT_RECORD_H_

#include <nlohmann/json.hpp>
#include <string>

#include "message/Location.h"

namespace music {
class WaypointRecord {
 public:
  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kWorldCoordinateRecord =
      "worldCoordinateRecord";
  inline static const std::string kEstimatedArrivalTime =
      "estimatedArrivalTime";
  inline static const std::string kArrivalTimeError = "arrivalTimeError";

  // Getters
  const Location &GetWorldCoordinateRecord() const {
    return world_coordinate_record_;
  }
  const int GetEstimatedArrivalTime() const { return estimated_arrival_time_; }
  const int GetArrivalTimeError() const { return arrival_time_error_; }

  // Setters
  void SetWorldCoordinateRecord(
      const music::Location &world_coordinate_record) {
    world_coordinate_record_ = world_coordinate_record;
  }
  void SetEstimatedArrivalTime(int estimated_arrival_time) {
    estimated_arrival_time_ = estimated_arrival_time;
  }
  void SetArrivalTimeError(int arrival_time_error) {
    arrival_time_error_ = arrival_time_error;
  }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const WaypointRecord &waypoint_record);
  friend void from_json(const nlohmann::json &j,
                        WaypointRecord &waypoint_record);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static WaypointRecord FromJson(const nlohmann::json &j) {
    WaypointRecord record;
    from_json(j, record);
    return record;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const WaypointRecord &waypoint_record) {
    os << waypoint_record.ToString();
    return os;
  }

  std::string ToString() const { return "WaypointRecord: " + ToJson().dump(); }

 protected:
  // The coordinates of a waypoint.
  Location world_coordinate_record_{};

  // An estimation of the epoch time in seconds when this waypoint will be
  // reached by the construct. If this waypoint has already been reached by the
  // construct, a value of zero shall be used here. If it is expected the
  // construct will repeat the route, then waypoints that have already been
  // reached should show the ETA of when it will reach the waypoint the next
  // time.
  int estimated_arrival_time_{std::numeric_limits<int>::min()};

  // The arrival time estimation error in seconds.
  int arrival_time_error_{std::numeric_limits<int>::min()};
};
// Builder
class WaypointRecordBuilder {
 private:
  WaypointRecord waypoint_record_;

 public:
  // Setters that return 'this' (the builder itself)
  WaypointRecordBuilder &SetWorldCoordinateRecord(
      const music::Location &world_coordinate_record) {
    waypoint_record_.SetWorldCoordinateRecord(world_coordinate_record);
    return *this;
  }
  WaypointRecordBuilder &SetEstimatedArrivalTime(int estimated_arrival_time) {
    waypoint_record_.SetEstimatedArrivalTime(estimated_arrival_time);
    return *this;
  }
  WaypointRecordBuilder &SetArrivalTimeError(int arrival_time_error) {
    waypoint_record_.SetArrivalTimeError(arrival_time_error);
    return *this;
  }

  // Build method to create WaypointRecord instance
  WaypointRecord Build() { return waypoint_record_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_WAYPOINT_WAYPOINT_RECORD_H_