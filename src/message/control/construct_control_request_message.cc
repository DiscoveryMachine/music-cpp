// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include "enum/music_message_type.h"
#include "message/control/construct_control_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void ConstructControlRequestMessage::Validate() const {
  if (type_ != MusicMessageType::kControlRequest) {
    LogError(
        "ConstructControlRequestMessage::Validate - type_ must be "
        "kControlRequest");
  }
  if (context_.empty()) {
    LogError("Context is empty on ConstructControlRequestMessage!");
  }
  target_construct_.Validate();
  MusicRequestMessage::Validate();
}

// JSON Serialization
void from_json(const nlohmann::json &jsn,
               ConstructControlRequestMessage &message) {
  from_json(jsn, static_cast<MusicRequestMessage &>(message));
  jsn.at(ConstructControlRequestMessage::kTargetConstructKey)
      .get_to(message.target_construct_);
  jsn.at(ConstructControlRequestMessage::kContextKey).get_to(message.context_);
}
void to_json(nlohmann::json &jsn,
             const ConstructControlRequestMessage &message) {
  to_json(jsn, static_cast<const MusicRequestMessage &>(message));
  jsn.update({{ConstructControlRequestMessage::kTargetConstructKey,
               message.target_construct_},
              {ConstructControlRequestMessage::kContextKey, message.context_}});
}

}  // namespace music