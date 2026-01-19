// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/fuse_type.h"
#include "enum/warhead.h"
#include "message/munition/munition_message.h"
#include "util/validate.h"

namespace music {

// Validate
void MunitionMessage::Validate() const {
  if (!WarheadHelper::IsValid(static_cast<int>(warhead_))) {
    LogError("MunitionMessage::Validate - warhead_ has invalid value: " +
             std::to_string(static_cast<int>(warhead_)));
  }

  if (!FuseTypeHelper::IsValid(static_cast<int>(fuse_))) {
    LogError("MunitionMessage::Validate - fuse_ has invalid value: " +
             std::to_string(static_cast<int>(fuse_)));
  }

  if (rate_ < 0) {
    LogError("MunitionMessage::Validate - rate_ cannot be negative: " +
             std::to_string(rate_));
  }

  if (quantity_ < 0) {
    LogError("MunitionMessage::Validate - quantity_ cannot be negative: " +
             std::to_string(quantity_));
  }

  shooter_id_.Validate();
  if (target_id_.has_value()) {
    target_id_->Validate();
  }
  munition_id_.Validate();
  event_id_.Validate();
  velocity_.Validate();
  location_.Validate();
  munition_.Validate();
  MusicMessage::Validate();
}

void to_json(nlohmann::json &j, const MunitionMessage &message) {
  // Serialize super class
  to_json(j, static_cast<const MusicMessage &>(message));

  // Append MunitionMessage fields
  j.update(nlohmann::json{
      {MunitionMessage::kShooterIdKey, message.shooter_id_},
      {MunitionMessage::kMunitionIdKey, message.munition_id_},
      {MunitionMessage::kEventIdKey, message.event_id_},
      {MunitionMessage::kVelocityKey, message.velocity_},
      {MunitionMessage::kLocationKey, message.location_},
      {MunitionMessage::kMunitionKey, message.munition_},
      {MunitionMessage::kWarheadKey, static_cast<int>(message.warhead_)},
      {MunitionMessage::kFuseKey, static_cast<int>(message.fuse_)},
      {MunitionMessage::kRateKey, message.rate_},
      {MunitionMessage::kQuantityKey, message.quantity_}});

  if (message.target_id_.has_value()) {
    j.update({{MunitionMessage::kTargetIdKey, message.target_id_.value()}});
  }
}

void from_json(const nlohmann::json &j, MunitionMessage &message) {
  // Deserialize super class
  from_json(j, static_cast<MusicMessage &>(message));

  // Extract MunitionMessage fields
  j.at(MunitionMessage::kShooterIdKey).get_to(message.shooter_id_);
  if (j.contains(MunitionMessage::kTargetIdKey) &&
      !j.at(MunitionMessage::kTargetIdKey).is_null()) {
    j.at(MunitionMessage::kTargetIdKey).get_to(message.target_id_);
  } else {
    message.target_id_ = std::nullopt;
  }
  j.at(MunitionMessage::kMunitionIdKey).get_to(message.munition_id_);
  j.at(MunitionMessage::kEventIdKey).get_to(message.event_id_);
  j.at(MunitionMessage::kVelocityKey).get_to(message.velocity_);
  j.at(MunitionMessage::kLocationKey).get_to(message.location_);
  j.at(MunitionMessage::kMunitionKey).get_to(message.munition_);
  int warhead_int;
  j.at(MunitionMessage::kWarheadKey).get_to(warhead_int);
  message.warhead_ = static_cast<Warhead>(warhead_int);
  int fuse_int;
  j.at(MunitionMessage::kFuseKey).get_to(fuse_int);
  message.fuse_ = static_cast<FuseType>(fuse_int);
  j.at(MunitionMessage::kRateKey).get_to(message.rate_);
  j.at(MunitionMessage::kQuantityKey).get_to(message.quantity_);
}

}  // namespace music
