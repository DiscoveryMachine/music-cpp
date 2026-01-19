// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/Location.h"
#include "message/Orientation.h"
#include "message/Vector.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/dead_reckoning.h"
#include "message/entity_id.h"
#include "message/entity_type.h"
#include "message/music_message.h"
#include "util/validate.h"

namespace music {

void to_json(nlohmann::json &jsn, const ConstructDataMessage &constructData) {
  to_json(jsn, ((MusicMessage)constructData));
  // clang-format off
  jsn.update({
      {ConstructDataMessage::kOriginIdKey, constructData.origin_id_ },
      {ConstructDataMessage::kPrimaryControllerIdKey, constructData.primary_controller_id_ },
      {ConstructDataMessage::kCurrentControllerIdKey, constructData.current_controller_id_ },
      {ConstructDataMessage::kConstructInformationRecordKey, constructData.construct_information_record_ },
      {ConstructDataMessage::kCallsignKey, constructData.callsign_ },
      {ConstructDataMessage::kConstructNameKey, constructData.construct_name_ },
      {ConstructDataMessage::kInteractionRecordKey, constructData.interaction_record_ }
      });
  // clang-format on
  if (constructData.ghosted_id_.has_value()) {
    jsn.update({{ConstructDataMessage::kGhostedIdKey,
                 constructData.ghosted_id_.value()}});
  }
  if (constructData.force_.has_value()) {
    jsn.update(
        {{ConstructDataMessage::kForceKey, constructData.force_.value()}});
  }
  if (constructData.entity_type_.has_value()) {
    jsn.update({{ConstructDataMessage::kEntityTypeKey,
                 constructData.entity_type_.value()}});
  }
  if (constructData.location_.has_value()) {
    jsn.update({{ConstructDataMessage::kLocationKey,
                 constructData.location_.value()}});
  }
  if (constructData.orientation_.has_value()) {
    jsn.update({{ConstructDataMessage::kOrientationKey,
                 constructData.orientation_.value()}});
  }
  if (constructData.velocity_.has_value()) {
    jsn.update({{ConstructDataMessage::kVelocityKey,
                 constructData.velocity_.value()}});
  }
  if (constructData.dead_reckoning_.has_value()) {
    jsn.update({{ConstructDataMessage::kDeadReckoningKey,
                 constructData.dead_reckoning_.value()}});
  }
  if (constructData.damage_.has_value()) {
    jsn.update(
        {{ConstructDataMessage::kDamageKey, constructData.damage_.value()}});
  }
}  // namespace music
void from_json(const nlohmann::json &jsn, ConstructDataMessage &constructData) {
  // clang-format off
  jsn.at(ConstructDataMessage::kOriginIdKey).get_to(constructData.origin_id_);
  jsn.at(ConstructDataMessage::kPrimaryControllerIdKey).get_to(constructData.primary_controller_id_);
  jsn.at(ConstructDataMessage::kCurrentControllerIdKey).get_to(constructData.current_controller_id_);
  jsn.at(ConstructDataMessage::kConstructInformationRecordKey).get_to(constructData.construct_information_record_);
  jsn.at(ConstructDataMessage::kCallsignKey).get_to(constructData.callsign_);
  jsn.at(ConstructDataMessage::kConstructNameKey).get_to(constructData.construct_name_);
  jsn.at(ConstructDataMessage::kInteractionRecordKey).get_to(constructData.interaction_record_);

  // Optional fields
  constructData.ghosted_id_= jsn.value(ConstructDataMessage::kGhostedIdKey, std::optional<EntityId>());
  constructData.force_ = jsn.value(ConstructDataMessage::kForceKey, std::optional<Force>());
  constructData.entity_type_ = jsn.value(ConstructDataMessage::kEntityTypeKey, std::optional<EntityType>());
  constructData.location_ = jsn.value(ConstructDataMessage::kLocationKey, std::optional<Location>());
  constructData.orientation_ = jsn.value(ConstructDataMessage::kOrientationKey, std::optional<Orientation>());
  constructData.velocity_ = jsn.value(ConstructDataMessage::kVelocityKey, std::optional<Vector>());
  constructData.dead_reckoning_ = jsn.value(ConstructDataMessage::kDeadReckoningKey, std::optional<DeadReckoning>());
  constructData.damage_ = jsn.value(ConstructDataMessage::kDamageKey, std::optional<Damage>());

  // clang-format on
}

void ConstructDataMessage::Validate() const {
  ((MusicMessage) * this).Validate();
  origin_id_.Validate();
  primary_controller_id_.Validate();
  current_controller_id_.Validate();
  construct_information_record_.Validate();
  if (ghosted_id_.has_value()) {
    ghosted_id_->Validate();
  }

  if (force_.has_value() && force_.value() == Force::kUnset) {
    LogError("Invalid force value in ConstructDataMessage!");
  }
  if (entity_type_.has_value()) {
    entity_type_->Validate();
  }
  if (location_.has_value()) {
    location_->Validate();
  }
  if (orientation_.has_value()) {
    orientation_->Validate();
  }
  if (velocity_.has_value()) {
    velocity_->Validate();
  }
  if (dead_reckoning_.has_value()) {
    dead_reckoning_->Validate();
  }
  if (damage_.has_value()) {
    damage_->Validate();
  }

  if (callsign_ == "") {
    LogError("Invalid callsign value in ConstructDataMessage!");
  }
  if (construct_name_ == "") {
    LogError("Invalid construct name value in ConstructDataMessage!");
  }
  if (interaction_record_ == "") {
    LogError("Invalid interaction record value in ConstructDataMessage!");
  }
}
}  // namespace music