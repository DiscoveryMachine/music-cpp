// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include "enum/music_message_type.h"
#include "message/control/set_current_controller_message.h"
#include "util/validate.h"

namespace music {
// Validate
void SetCurrentControllerMessage::Validate() const {
  if (type_ != MusicMessageType::kSetCurrentController) {
    LogError(
        "SetCurrentControllerMessage::Validate - type_ must be "
        "kSetCurrentController");
  }
  current_controller_id_.Validate();
  MusicRequestMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const SetCurrentControllerMessage &message) {
  to_json(j, static_cast<const MusicRequestMessage &>(message));
  j.update(nlohmann::json{{SetCurrentControllerMessage::kCurrentControllerIdKey,
                           message.current_controller_id_}});
}
void from_json(const nlohmann::json &j, SetCurrentControllerMessage &message) {
  from_json(j, static_cast<MusicRequestMessage &>(message));
  j.at(SetCurrentControllerMessage::kCurrentControllerIdKey)
      .get_to(message.current_controller_id_);
}
}  // namespace music