// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_CONSTRUCT_DATA_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_CONSTRUCT_DATA_MESSAGE_H_

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "construct_information_record.h"
#include "damage.h"
#include "dead_reckoning.h"
#include "enum/force.h"
#include "message/Location.h"
#include "message/Orientation.h"
#include "message/Vector.h"
#include "message/entity_id.h"
#include "message/entity_type.h"
#include "message/music_message.h"

namespace music {
class ConstructDataMessage : public MusicMessage {
  friend class ConstructDataMessageBuilder;

 public:
  ConstructDataMessage() { type_ = MusicMessageType::kConstructData; }

  // JSON Keys
  static inline const std::string kOriginIdKey = "originID";

  static inline const std::string kPrimaryControllerIdKey =
      "primaryControllerID";

  static inline const std::string kCurrentControllerIdKey =
      "currentControllerID";

  static inline const std::string kConstructInformationRecordKey =
      "constructInformationRecord";

  static inline const std::string kGhostedIdKey = "ghostedID";

  static inline const std::string kForceKey = "force";

  static inline const std::string kEntityTypeKey = "entityType";

  static inline const std::string kLocationKey = "location";

  static inline const std::string kOrientationKey = "orientation";

  static inline const std::string kVelocityKey = "velocity";

  static inline const std::string kDeadReckoningKey = "deadReck";

  static inline const std::string kDamageKey = "damageRecord";

  static inline const std::string kCallsignKey = "callsign";

  static inline const std::string kConstructNameKey = "constructName";

  static inline const std::string kInteractionRecordKey = "interactionRecord";

  // JSON serialization
  friend void to_json(nlohmann::json &jsn,
                      const ConstructDataMessage &constructData);
  friend void from_json(const nlohmann::json &jsn,
                        ConstructDataMessage &constructData);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static ConstructDataMessage FromJson(const nlohmann::json &j) {
    ConstructDataMessage message;
    from_json(j, message);
    return message;
  }

  // Getters
  const EntityId &GetOriginId() const { return origin_id_; }
  const EntityId &GetPrimaryControllerId() const {
    return primary_controller_id_;
  }
  const EntityId &GetCurrentControllerId() const {
    return current_controller_id_;
  }
  const music::ConstructInformationRecord &GetConstructInformationRecord()
      const {
    return construct_information_record_;
  }
  const std::optional<EntityId> &GetGhostedId() const { return ghosted_id_; }
  const std::optional<music::Force> &GetForce() const { return force_; }
  const std::optional<music::EntityType> &GetEntityType() const {
    return entity_type_;
  }
  const std::optional<music::Location> &GetLocation() const {
    return location_;
  }
  const std::optional<music::Orientation> &GetOrientation() const {
    return orientation_;
  }
  const std::optional<music::Vector> &GetVelocity() const { return velocity_; }
  const std::optional<music::DeadReckoning> &GetDeadReckoning() const {
    return dead_reckoning_;
  }
  const std::optional<music::Damage> &GetDamage() const { return damage_; }
  const std::string &GetCallsign() const { return callsign_; }
  const std::string &GetConstructName() const { return construct_name_; }
  // A comma-delimited list of all the names of the available interactions for
  // this construct that can be requested at this time.
  const std::string &GetInteractionRecord() const {
    return interaction_record_;
  }

  // Setters
  void SetOriginId(const EntityId id) { origin_id_ = id; }
  void SetPrimaryControllerId(const EntityId id) {
    primary_controller_id_ = id;
  }
  void SetCurrentControllerId(const EntityId id) {
    current_controller_id_ = id;
  }
  void SetConstructInformationRecord(
      const music::ConstructInformationRecord rec) {
    construct_information_record_ = rec;
  }
  void SetGhostedId(const EntityId id) { ghosted_id_ = id; }
  void SetForce(music::Force f) { force_ = f; }
  void SetEntityType(const music::EntityType et) { entity_type_ = et; }
  void SetLocation(const music::Location loc) { location_ = loc; }
  void SetOrientation(const music::Orientation ori) { orientation_ = ori; }
  void SetVelocity(const music::Vector vel) { velocity_ = vel; }
  void SetDeadReckoning(const music::DeadReckoning dr) { dead_reckoning_ = dr; }
  void SetDamage(const music::Damage dmg) { damage_ = dmg; }
  void SetCallsign(const std::string cs) { callsign_ = cs; }
  void SetConstructName(const std::string name) { construct_name_ = name; }
  void SetInteractionRecord(const std::string &rec) {
    interaction_record_ = rec;
  }

  void Validate() const;

 protected:
  EntityId origin_id_{};
  EntityId primary_controller_id_{};
  EntityId current_controller_id_{};
  ConstructInformationRecord construct_information_record_{};
  std::string callsign_{};
  std::string construct_name_{};
  // A comma-delimited list of all the names of the available interactions for
  // this construct that can be requested at this time.
  std::string interaction_record_{};

  // Optional fields;
  std::optional<EntityId> ghosted_id_{};
  std::optional<Force> force_{};
  std::optional<EntityType> entity_type_{};
  std::optional<Location> location_{};
  std::optional<Orientation> orientation_{};
  std::optional<Vector> velocity_{};
  std::optional<DeadReckoning> dead_reckoning_{};
  std::optional<Damage> damage_{};
};

class ConstructDataMessageBuilder
    : public MessageBuilder<ConstructDataMessageBuilder> {
 public:
  ConstructDataMessageBuilder() {
    message_ = std::make_shared<ConstructDataMessage>();
    SetBuiltMessage(message_);
  }

  ConstructDataMessageBuilder &SetPrimaryControllerId(
      const EntityId primary_controller_id) {
    message_->primary_controller_id_ = primary_controller_id;
    return *this;
  }
  ConstructDataMessageBuilder &SetCurrentControllerId(
      const EntityId current_controller_id) {
    message_->current_controller_id_ = current_controller_id;
    return *this;
  }
  ConstructDataMessageBuilder &SetConstructInformationRecord(
      const ConstructInformationRecord construct_information_record) {
    message_->construct_information_record_ = construct_information_record;
    return *this;
  }

  ConstructDataMessageBuilder &SetOriginId(const EntityId origin_id) {
    message_->origin_id_ = origin_id;
    return *this;
  }
  ConstructDataMessageBuilder &SetGhostedId(const EntityId id) {
    message_->ghosted_id_ = id;
    return *this;
  }
  ConstructDataMessageBuilder &SetForce(music::Force f) {
    message_->force_ = f;
    return *this;
  }
  ConstructDataMessageBuilder &SetEntityType(const music::EntityType et) {
    message_->entity_type_ = et;
    return *this;
  }
  ConstructDataMessageBuilder &SetLocation(const music::Location loc) {
    message_->location_ = loc;
    return *this;
  }
  ConstructDataMessageBuilder &SetOrientation(const music::Orientation ori) {
    message_->orientation_ = ori;
    return *this;
  }
  ConstructDataMessageBuilder &SetVelocity(const music::Vector vel) {
    message_->velocity_ = vel;
    return *this;
  }
  ConstructDataMessageBuilder &SetDeadReckoning(const music::DeadReckoning dr) {
    message_->dead_reckoning_ = dr;
    return *this;
  }
  ConstructDataMessageBuilder &SetDamage(const music::Damage dmg) {
    message_->damage_ = dmg;
    return *this;
  }
  ConstructDataMessageBuilder &SetCallsign(const std::string cs) {
    message_->callsign_ = cs;
    return *this;
  }
  ConstructDataMessageBuilder &SetConstructName(const std::string name) {
    message_->construct_name_ = name;
    return *this;
  }
  ConstructDataMessageBuilder &SetInteractionRecord(const std::string &rec) {
    message_->interaction_record_ = rec;
    return *this;
  }
  ConstructDataMessage Build() { return *message_; }

 protected:
  std::shared_ptr<ConstructDataMessage> message_;
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_CONSTRUCT_DATA_MESSAGE_H_