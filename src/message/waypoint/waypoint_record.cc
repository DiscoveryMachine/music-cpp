// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/location.h"
#include "message/waypoint/waypoint_record.h"
#include "util/validate.h"

namespace music {

void WaypointRecord::Validate() const {
  if (estimated_arrival_time_ < 0) {
    LogError(
        "WaypointRecord::validate - estimated_arrival_time_ cannot be "
        "negative: " +
        std::to_string(estimated_arrival_time_));
  }
  if (arrival_time_error_ < 0) {
    LogError(
        "WaypointRecord::validate - arrival_time_error_ cannot be negative: " +
        std::to_string(arrival_time_error_));
  }
  world_coordinate_record_.Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const WaypointRecord &waypoint_record) {
  j = nlohmann::json{
      {WaypointRecord::kWorldCoordinateRecord,
       waypoint_record.world_coordinate_record_},
      {WaypointRecord::kEstimatedArrivalTime,
       waypoint_record.estimated_arrival_time_},
      {WaypointRecord::kArrivalTimeError, waypoint_record.arrival_time_error_}};
}

void from_json(const nlohmann::json &j, WaypointRecord &waypoint_record) {
  j.at(WaypointRecord::kWorldCoordinateRecord)
      .get_to(waypoint_record.world_coordinate_record_);
  j.at(WaypointRecord::kEstimatedArrivalTime)
      .get_to(waypoint_record.estimated_arrival_time_);
  j.at(WaypointRecord::kArrivalTimeError)
      .get_to(waypoint_record.arrival_time_error_);
}

}  // namespace music