// Copyright (c)  Discovery Machine®, Inc.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_WAYPOINT_WAYPOINT_DATA_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_WAYPOINT_WAYPOINT_DATA_MESSAGE_H_

#include <limits>
#include <nlohmann/json.hpp>

#include "message/entity_id.h"
#include "message/music_message.h"
#include "message/waypoint/waypoint_record.h"

namespace music {
class WaypointDataMessage : public MusicMessage {
  template <class T>
  friend class WaypointDataMessageBuilderTemplate;

 public:
  // Constructor
  WaypointDataMessage() : MusicMessage() {
    type_ = MusicMessageType::kWaypoint;
  }
  // Validate
  void Validate() const;

  // JSON Keys
  // Key for the Origin Id field.
  static const inline std::string kOriginIdKey = "originID";

  // Key for the start or append field.
  static const inline std::string kStartOrAppendKey = "startOrAppend";

  // Key for the Current waypoint index field.
  static const inline std::string kCurrentWaypointIndexKey =
      "currentWaypointIndex";

  // Key for the Number of waypoints field.
  static const inline std::string kNumWaypointsKey = "numWaypoints";

  // Key for the Waypoint Records field.
  static const inline std::string kWaypointRecordsKey = "waypointRecords";

  // Getters
  const EntityId &GetOriginId() const { return origin_id_; }
  const bool GetStartOrAppend() const { return start_or_append_; }
  const int GetCurrentWaypointIndex() const { return current_waypoint_index_; }
  const int GetNumWaypoints() const { return num_waypoints_; }
  const std::vector<WaypointRecord> &GetWaypointRecords() const {
    return waypoint_records_;
  }

  // Setters
  void SetOriginId(const EntityId &origin_id) { origin_id_ = origin_id; }
  void SetStartOrAppend(bool start_or_append) {
    start_or_append_ = start_or_append;
  }
  void SetCurrentWaypointIndex(int current_waypoint_index) {
    current_waypoint_index_ = current_waypoint_index;
  }
  void SetWaypointRecords(const std::vector<WaypointRecord> &waypoint_records) {
    waypoint_records_ = waypoint_records;
  }
  void SetNumWaypoints(int num_waypoints) { num_waypoints_ = num_waypoints; }

  // JSON serialization

  friend void to_json(nlohmann::json &j, const WaypointDataMessage &message);
  friend void from_json(const nlohmann::json &j, WaypointDataMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static WaypointDataMessage FromJson(const nlohmann::json &j) {
    WaypointDataMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const WaypointDataMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "WaypointDataMessage: " + ToJson().dump();
  }

 protected:
  // The entity identifier of this construct.
  EntityId origin_id_{};

  // true to mark start of new waypoint set, false to mark continuation of
  // existing set. A  continuation of an existing set would be used when a route
  // has many waypoints and needs to be broken up. For example, you might have a
  // very long route with 1000 waypoints and want to break it up into route sets
  // of 100 waypoints. When you need to send your updated route, you will send
  // the entire 1000 waypoint route but broken up into 10 messages of 100
  // waypoint routes. The first 100 waypoint route of these messages will be
  // marked as 'true' for the start of the set, and all following groups of 100
  // will be marked as '0' for a continuation.
  bool start_or_append_{};

  // The index of the waypoint to which the construct is currently moving.
  int current_waypoint_index_{std::numeric_limits<int>::min()};

  // The number of waypoint records to follow.
  int num_waypoints_{std::numeric_limits<int>::min()};

  // The waypoint data to be transmitted. Note: Waypoint records are not
  // discarded after the construct has reached them as they are still a part of
  // the route.
  std::vector<WaypointRecord> waypoint_records_{};
};

// Builder class
template <class T>
class WaypointDataMessageBuilderTemplate : public MessageBuilder<T> {
 protected:
  std::shared_ptr<WaypointDataMessage> waypoint_message_;
  void SetBuiltMessage(std::shared_ptr<MusicMessage> new_message) {
    waypoint_message_ =
        std::static_pointer_cast<WaypointDataMessage>(new_message);
    MessageBuilder<T>::SetBuiltMessage(new_message);
  }

 public:
  T &SetOriginId(const EntityId &origin_id) {
    waypoint_message_->origin_id_ = origin_id;
    return static_cast<T &>(*this);
  }
  T &SetStartOrAppend(bool start_or_append) {
    waypoint_message_->start_or_append_ = start_or_append;
    return static_cast<T &>(*this);
  }
  T &SetCurrentWaypointIndex(int current_waypoint_index) {
    waypoint_message_->current_waypoint_index_ = current_waypoint_index;
    return static_cast<T &>(*this);
  }
  T &SetNumWaypoints(int num_waypoints) {
    waypoint_message_->num_waypoints_ = num_waypoints;
    return static_cast<T &>(*this);
  }
  T &SetWaypointRecords(const std::vector<WaypointRecord> &waypoint_records) {
    waypoint_message_->waypoint_records_ = waypoint_records;
    return static_cast<T &>(*this);
  }
};
class WaypointDataMessageBuilder
    : public WaypointDataMessageBuilderTemplate<WaypointDataMessageBuilder> {
 public:
  WaypointDataMessageBuilder() {
    waypoint_message_ = std::make_shared<WaypointDataMessage>();
    SetBuiltMessage(waypoint_message_);
  }

  // The final 'build' method that creates the WaypointDataMessage object
  WaypointDataMessage Build() const { return *waypoint_message_; }
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_WAYPOINT_WAYPOINT_DATA_MESSAGE_H_