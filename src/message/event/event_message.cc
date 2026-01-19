// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/event/event_message.h"
#include "message/music_message.h"
#include "message/music_request_message.h"
#include "util/validate.h"

namespace music {

static const std::string kErrorMesgPrefix = "EventMessage::Validate: ";

void EventMessage::Validate() const {
  MusicMessage::Validate();
  origin_id_.Validate();

  if (event_type_.empty()) {
    LogError(kErrorMesgPrefix + "No event type provided.");
  }

  if (event_data_.empty()) {
    LogError(kErrorMesgPrefix + "No event data provided.");
  }

  if (event_data_.empty()) {
    LogError(kErrorMesgPrefix + "Event data is not properly formatted JSON.");
  }
}

void to_json(nlohmann::json &j, const EventMessage &mesg) {
  to_json(j, static_cast<const MusicMessage &>(mesg));
  j.update(nlohmann::json{
      {MusicRequestMessage::kOriginIdKey, mesg.origin_id_.ToJson()},
      {EventMessage::kEventTypeJsonKey, mesg.event_type_},
      {EventMessage::kEventDataJsonKey, mesg.event_data_}});
}

void from_json(const nlohmann::json &j, EventMessage &mesg) {
  from_json(j, static_cast<MusicMessage &>(mesg));
  j.at(MusicRequestMessage::kOriginIdKey).get_to(mesg.origin_id_);
  j.at(EventMessage::kEventTypeJsonKey).get_to(mesg.event_type_);
  mesg.event_data_ = j.at(EventMessage::kEventDataJsonKey);
}

}  // namespace music