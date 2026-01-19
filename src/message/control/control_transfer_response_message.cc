// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/music_message_type.h"
#include "message/control/control_transfer_response_message.h"
#include "util/validate.h"

namespace music {
// Validate
void ControlTransferResponseMessage::Validate() const {
  if (type_ != MusicMessageType::kControlTransferResponse) {
    LogError(
        "ControlTransferResponseMessage::Validate - type_ must be "
        "kControlTransferResponse");
  }
  MusicResponseMessage::Validate();
}

// JSON Serialization
void to_json(nlohmann::json &j, const ControlTransferResponseMessage &message) {
  to_json(j, static_cast<const MusicResponseMessage &>(message));
  // No additional fields to serialize
}
void from_json(const nlohmann::json &j,
               ControlTransferResponseMessage &message) {
  from_json(j, static_cast<MusicResponseMessage &>(message));
  // No additional fields to extract
}
}  // namespace music