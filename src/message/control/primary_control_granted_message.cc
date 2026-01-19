// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/music_message_type.h"
#include "message/control/primary_control_granted_message.h"
#include "util/validate.h"

namespace music {
// Validate
void PrimaryControlGrantedMessage::Validate() const {
  if (type_ != MusicMessageType::kPrimaryControlGranted) {
    LogError(
        "PrimaryControlGrantedMessage::Validate - type_ must be "
        "kPrimaryControlGranted");
  }
  MusicRequestMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const PrimaryControlGrantedMessage &message) {
  to_json(j, static_cast<const MusicRequestMessage &>(message));
  // No additional fields to serialize
}
void from_json(const nlohmann::json &j, PrimaryControlGrantedMessage &message) {
  from_json(j, static_cast<MusicRequestMessage &>(message));
  // No additional fields to extract
}
}  // namespace music
