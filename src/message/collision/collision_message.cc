// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/collision/collision_message.h"
#include "util/validate.h"

namespace music {

void to_json(nlohmann::json &jsn, const CollisionMessage &collisionMessage) {
  // Serialize base
  to_json(jsn, ((MusicMessage)collisionMessage));
  // clang-format off
  jsn.update({
    {CollisionMessage::kMassKey, collisionMessage.mass_},             
    {CollisionMessage::kIssuingIdKey, collisionMessage.issuing_id_},
    {CollisionMessage::kCollidingIdKey, collisionMessage.colliding_id_},
    {CollisionMessage::kCollisionTypeKey, collisionMessage.collision_type_},
    {CollisionMessage::kLocationKey, collisionMessage.location_},
    {CollisionMessage::kVelocityKey, collisionMessage.velocity_},
    {CollisionMessage::kEventIdKey, collisionMessage.event_id_}
    });
  // clang-format on
}

void from_json(const nlohmann::json &jsn, CollisionMessage &collisionMessage) {
  from_json(jsn, static_cast<MusicMessage &>(collisionMessage));

  jsn.at(CollisionMessage::kMassKey).get_to(collisionMessage.mass_);
  jsn.at(CollisionMessage::kIssuingIdKey).get_to(collisionMessage.issuing_id_);
  jsn.at(CollisionMessage::kCollidingIdKey)
      .get_to(collisionMessage.colliding_id_);
  jsn.at(CollisionMessage::kCollisionTypeKey)
      .get_to(collisionMessage.collision_type_);
  jsn.at(CollisionMessage::kLocationKey).get_to(collisionMessage.location_);
  jsn.at(CollisionMessage::kVelocityKey).get_to(collisionMessage.velocity_);
  jsn.at(CollisionMessage::kEventIdKey).get_to(collisionMessage.event_id_);
}

void CollisionMessage::Validate() const {
  if (collision_type_ <= CollisionType::kUnset ||
      collision_type_ > CollisionType::kOther) {
    LogError("Invalid value for CollisionMessage::collision_type");
  }
  if (std::isnan(mass_) || mass_ < 0.0) {
    LogError("Invalid value for CollisionMessage::mass");
  }

  location_.Validate();
  velocity_.Validate();
  event_id_.Validate();
}
}  // namespace music