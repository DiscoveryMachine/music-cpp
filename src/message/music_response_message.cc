// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <nlohmann/json_fwd.hpp>
#include <string>

#include "message/music_response_message.h"
#include "util/validate.h"

namespace music {

void MusicResponseMessage::Validate() const {
  if (request_id_ < 0 || request_id_ > std::numeric_limits<uint32_t>::max()) {
    LogError(
        "MusicResponseMessage::Validate - request_id_ out of range [0, "
        "4294967295]");
  }

  if (request_status_ <= RequestStatus::kOtherUnknown ||
      request_status_ > RequestStatus::kAborted) {
    LogError(
        "MusicResponseMessage::Validate - request_status_ out of valid "
        "range: " +
        std::to_string(static_cast<int>(request_status_)));
  }

  origin_id_.Validate();
  receiver_id_.Validate();
  MusicMessage::Validate();
}

// JSON serialization
void to_json(nlohmann::json &j, const MusicResponseMessage &message) {
  // Serialize base class members
  to_json(j, static_cast<const MusicMessage &>(message));
  // Append derived class members
  j.update(nlohmann::json{
      {MusicResponseMessage::kOriginIdKey, message.origin_id_},
      {MusicResponseMessage::kReceiverIdKey, message.receiver_id_},
      {MusicResponseMessage::kRequestIdKey, message.request_id_},
      {MusicResponseMessage::kRequestStatusKey,
       static_cast<int>(message.request_status_)}});
}

void from_json(const nlohmann::json &j, MusicResponseMessage &message) {
  j.at(MusicResponseMessage::kOriginIdKey).get_to(message.origin_id_);
  j.at(MusicResponseMessage::kReceiverIdKey).get_to(message.receiver_id_);
  j.at(MusicResponseMessage::kRequestIdKey).get_to(message.request_id_);
  int status_int;
  j.at(MusicResponseMessage::kRequestStatusKey).get_to(status_int);
  message.request_status_ = static_cast<RequestStatus>(status_int);
  // fill out super class members
  from_json(j, static_cast<MusicMessage &>(message));
}
}  // namespace music
