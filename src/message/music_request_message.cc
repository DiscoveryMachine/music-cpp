// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <nlohmann/json_fwd.hpp>
#include <string>

#include "message/music_request_message.h"
#include "util/validate.h"

namespace music {

void MusicRequestMessage::Validate() const {
  if (request_id_ < 0 || request_id_ > std::numeric_limits<uint32_t>::max()) {
    LogError(
        "MusicRequestMessage::Validate - request_id_ out of range [0, "
        "4294967295]");
  }
  origin_id_.Validate();
  receiver_id_.Validate();
  MusicMessage::Validate();
}

void to_json(nlohmann::json &j, const MusicRequestMessage &message) {
  // Serialize super class members
  to_json(j, static_cast<const MusicMessage &>(message));

  // Append derived class members
  j.update({{MusicRequestMessage::kOriginIdKey, message.origin_id_},
            {MusicRequestMessage::kReceiverIdKey, message.receiver_id_},
            {MusicRequestMessage::kRequestIdKey, message.request_id_}});
}

void from_json(const nlohmann::json &j, MusicRequestMessage &message) {
  j.at(MusicRequestMessage::kOriginIdKey).get_to(message.origin_id_);
  j.at(MusicRequestMessage::kReceiverIdKey).get_to(message.receiver_id_);
  j.at(MusicRequestMessage::kRequestIdKey).get_to(message.request_id_);
  // fill out super class members
  from_json(j, static_cast<MusicMessage &>(message));
}

}  // namespace music
