// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/waypoint/waypoint_data_message.h"
#include "util/validate.h"

namespace music {

// Validate
void WaypointDataMessage::Validate() const {
  origin_id_.Validate();
  if (waypoint_records_.empty()) {
    LogError(
        "WaypointDataMessage::Validate - waypoint_records_ cannot be empty");
  }

  // No validation check possible for boolean start_or_append_

  if (current_waypoint_index_ < 0) {
    LogError(
        "WaypointDataMessage::validate - current_waypoint_index_ cannot be "
        "negative: " +
        std::to_string(current_waypoint_index_));
  }

  if (num_waypoints_ < 0) {
    LogError(
        "WaypointDataMessage::validate - num_waypoints_ cannot be negative: " +
        std::to_string(num_waypoints_));
  }

  if (waypoint_records_.empty()) {
    LogError(
        "WaypointDataMessage::Validate - waypoint_records_ cannot be empty");
  }

  if (num_waypoints_ > static_cast<int>(waypoint_records_.size())) {
    LogError(
        "WaypointDataMessage::Validate - num_waypoints_ exceeds the "
        "size of waypoint_records_: " +
        std::to_string(num_waypoints_) + " vs " +
        std::to_string(waypoint_records_.size()));
  }

  for (const auto &waypoint_record : waypoint_records_) {
    waypoint_record.Validate();
  }
}

// JSON serialization
void to_json(nlohmann::json &j, const WaypointDataMessage &message) {
  // serialize super class members
  to_json(j, static_cast<const MusicMessage &>(message));
  // append derived class members
  j.update(nlohmann::json{
      {WaypointDataMessage::kOriginIdKey, message.origin_id_},
      {WaypointDataMessage::kStartOrAppendKey, message.start_or_append_},
      {WaypointDataMessage::kCurrentWaypointIndexKey,
       message.current_waypoint_index_},
      {WaypointDataMessage::kNumWaypointsKey, message.num_waypoints_},
      {WaypointDataMessage::kWaypointRecordsKey, message.waypoint_records_}});
}

void from_json(const nlohmann::json &j, WaypointDataMessage &message) {
  // fill out super class members
  from_json(j, static_cast<MusicMessage &>(message));
  // extract derived class members
  j.at(WaypointDataMessage::kOriginIdKey).get_to(message.origin_id_);
  j.at(WaypointDataMessage::kStartOrAppendKey).get_to(message.start_or_append_);
  j.at(WaypointDataMessage::kCurrentWaypointIndexKey)
      .get_to(message.current_waypoint_index_);
  j.at(WaypointDataMessage::kNumWaypointsKey).get_to(message.num_waypoints_);
  j.at(WaypointDataMessage::kWaypointRecordsKey)
      .get_to(message.waypoint_records_);
}
}  // namespace music
