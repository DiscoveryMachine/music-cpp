// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COLLISION_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COLLISION_MESSAGE_H_

#include "enum/collision_type.h"
#include "message/Location.h"
#include "message/Vector.h"
#include "message/entity_id.h"
#include "message/event_id.h"
#include "message/music_message.h"

namespace music {
class CollisionMessage : public MusicMessage {
  template <class T>
  friend class CollisionMessageBuilderTemplate;

 public:
  // JSON keys
  static inline const std::string kMassKey = "mass";
  static inline const std::string kCollisionTypeKey = "collisionType";
  static inline const std::string kLocationKey = "location";
  static inline const std::string kVelocityKey = "velocity";
  static inline const std::string kEventIdKey = "eventID";
  static inline const std::string kIssuingIdKey = "issuingID";
  static inline const std::string kCollidingIdKey = "collidingID";

  // Getters
  const CollisionType &GetCollisionType() const { return collision_type_; }
  const Location &GetCollisionLocation() const { return location_; }
  const Vector &GetCollisionVelocity() const { return velocity_; }
  const MusicEventId &GetCollisionEventId() const { return event_id_; }
  const double GetMass() const { return mass_; }
  const EntityId &GetIssuingId() const { return issuing_id_; }
  const EntityId &GetCollidingId() const { return colliding_id_; }

  // Setters
  void SetCollisionType(CollisionType collision_type) {
    collision_type_ = collision_type;
  }
  void SetCollisionLocation(Location location) { location_ = location; }
  void SetCollisionVelocity(Vector velocity) { velocity_ = velocity; }
  void SetCollisionEventId(MusicEventId event_id) { event_id_ = event_id; }
  void SetMass(double mass) { mass_ = mass; }
  void SetIssuingId(EntityId issuing_id) { issuing_id_ = issuing_id; }
  void SetCollidingId(EntityId colliding_id) { colliding_id_ = colliding_id; }

  // Constructor
  CollisionMessage() { type_ = MusicMessageType::kCollision; }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &jsn,
                      const CollisionMessage &collisionMessage);
  friend void from_json(const nlohmann::json &jsn,
                        CollisionMessage &collisionMessage);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static CollisionMessage FromJson(const nlohmann::json &j) {
    CollisionMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const CollisionMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "MusicCollisionMessage: " + ToJson().dump();
  }

 protected:
  double mass_{std::numeric_limits<double>::quiet_NaN()};
  EntityId issuing_id_{};
  EntityId colliding_id_{};
  CollisionType collision_type_{CollisionType::kUnset};
  Location location_{};
  Vector velocity_{};
  MusicEventId event_id_{};
};
template <class T>
class CollisionMessageBuilderTemplate : public MessageBuilder<T> {
 public:
  T &SetMass(double mass) {
    collision_message_->mass_ = mass;
    return static_cast<T &>(*this);
  }
  T &SetCollisionType(CollisionType collision_type) {
    collision_message_->collision_type_ = collision_type;
    return static_cast<T &>(*this);
  }
  T &SetCollisionLocation(Location location) {
    collision_message_->location_ = location;
    return static_cast<T &>(*this);
  }
  T &SetCollisionVelocity(Vector velocity) {
    collision_message_->velocity_ = velocity;
    return static_cast<T &>(*this);
  }
  T &SetCollisionEventId(MusicEventId event_id) {
    collision_message_->event_id_ = event_id;
    return static_cast<T &>(*this);
  }
  T &SetIssuingId(EntityId issuing_id) {
    collision_message_->issuing_id_ = issuing_id;
    return static_cast<T &>(*this);
  }
  T &SetCollidingId(EntityId colliding_id) {
    collision_message_->colliding_id_ = colliding_id;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<CollisionMessage> collision_message_;
  void SetBuiltMessage(std::shared_ptr<MusicMessage> new_message) {
    collision_message_ =
        std::static_pointer_cast<CollisionMessage>(new_message);
    MessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

class CollisionMessageBuilder
    : public CollisionMessageBuilderTemplate<CollisionMessageBuilder> {
 public:
  CollisionMessageBuilder() {
    SetBuiltMessage(std::make_shared<CollisionMessage>());
  }

  CollisionMessage Build() { return *collision_message_; }
};

}  // namespace music
#endif