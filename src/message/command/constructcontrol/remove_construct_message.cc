// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/command/constructcontrol/remove_construct_message.h"
#include "util/validate.h"

namespace music {
// Validate
void RemoveConstructMessage::Validate() const {
  if (type_ != MusicMessageType::kRemoveConstruct) {
    LogError(
        "RemoveConstructMessage::Validate - type_ must be kRemoveConstruct");
  }
  if (command_identifier_ != 454007002L) {
    LogError(
        "RemoveConstructMessage::Validate - command_identifier_ must be "
        "454007002");
  }
  if (origin_id_.GetEntityNumber() != 0) {
    LogError(
        "RemoveConstructMessage::Validate - origin_id_ entity number must be "
        "0");
  }
  if (receiver_id_.GetEntityNumber() != 0) {
    LogError(
        "RemoveConstructMessage::Validate - receiver_id_ entity number must be "
        "0");
  }
  removed_construct_.Validate();
  MusicCommandRequestMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const RemoveConstructMessage &message) {
  // fill out super class json
  to_json(j, static_cast<const MusicCommandRequestMessage &>(message));
  // Append RemoveConstructMessage fields
  j.update(nlohmann::json{{RemoveConstructMessage::kRemovedConstructKey,
                           message.removed_construct_}});
}

void from_json(const nlohmann::json &j, RemoveConstructMessage &message) {
  // extract super class json
  from_json(j, static_cast<MusicCommandRequestMessage &>(message));
  // Extract RemoveConstructMessage fields
  j.at(RemoveConstructMessage::kRemovedConstructKey)
      .get_to(message.removed_construct_);
}

}  // namespace music