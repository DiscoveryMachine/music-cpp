// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "message/command/entity/transfer_construct_id_message.h"
#include "message/command/music_command_request_message.h"
#include "util/validate.h"

namespace music {

// Validate
void TransferConstructIdMessage::Validate() const {
  if (type_ != MusicMessageType::kTransferConstructId) {
    LogError(
        "TransferConstructIdMessage::Validate - type_ must be "
        "kTransferConstructId");
  }
  old_id_.Validate();
  new_id_.Validate();
  MusicCommandRequestMessage::Validate();
}

void from_json(const nlohmann::json &jsn, TransferConstructIdMessage &msg) {
  jsn.at(TransferConstructIdMessage::kNewIdKey).get_to(msg.new_id_);
  jsn.at(TransferConstructIdMessage::kOldIdKey).get_to(msg.old_id_);
  from_json(jsn, static_cast<MusicCommandRequestMessage &>(msg));
}

void to_json(nlohmann::json &jsn, const TransferConstructIdMessage &msg) {
  to_json(jsn, (MusicCommandRequestMessage)msg);
  jsn.update({{TransferConstructIdMessage::kNewIdKey, msg.new_id_},
              {TransferConstructIdMessage::kOldIdKey, msg.old_id_}},
             true);
}
}  // namespace music