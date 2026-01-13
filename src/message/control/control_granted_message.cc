// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/music_message_type.h"
#include "message/control/control_granted_message.h"
#include "util/validate.h"

namespace music {
// Validate
void ControlGrantedMessage::Validate() const {
  if (type_ != MusicMessageType::kControlGranted) {
    LogError("ControlGrantedMessage::Validate - type_ must be kControlGranted");
  }
  MusicRequestMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const ControlGrantedMessage &message) {
  to_json(j, static_cast<const MusicRequestMessage &>(message));
  // No additional fields to serialize
}
void from_json(const nlohmann::json &j, ControlGrantedMessage &message) {
  from_json(j, static_cast<MusicRequestMessage &>(message));
  // No additional fields to extract
}
}  // namespace music