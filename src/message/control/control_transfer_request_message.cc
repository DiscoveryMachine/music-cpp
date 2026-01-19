// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/music_message_type.h"
#include "message/control/control_transfer_request_message.h"
#include "util/validate.h"

namespace music {
// Validate
void ControlTransferRequestMessage::Validate() const {
  if (type_ != MusicMessageType::kControlTransferRequest) {
    LogError(
        "ControlTransferRequestMessage::Validate - type_ must be "
        "kControlTransferRequest");
  }
  if (context_.empty()) {
    LogError("Context is empty on ControlTransferRequestMessage!");
  }
  target_construct_.Validate();
  proposed_controller_.Validate();
  MusicRequestMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const ControlTransferRequestMessage &message) {
  to_json(j, static_cast<const MusicRequestMessage &>(message));
  j.update({{ControlTransferRequestMessage::kTargetConstructKey,
             message.target_construct_},
            {ControlTransferRequestMessage::kContextKey, message.context_},
            {ControlTransferRequestMessage::kProposedControllerKey,
             message.proposed_controller_}});
}
void from_json(const nlohmann::json &j,
               ControlTransferRequestMessage &message) {
  from_json(j, static_cast<MusicRequestMessage &>(message));
  j.at(ControlTransferRequestMessage::kTargetConstructKey)
      .get_to(message.target_construct_);
  j.at(ControlTransferRequestMessage::kContextKey).get_to(message.context_);
  j.at(ControlTransferRequestMessage::kProposedControllerKey)
      .get_to(message.proposed_controller_);
}

}  // namespace music