// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_EVENT_EVENT_MESSAGE_H
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_EVENT_EVENT_MESSAGE_H

#include <memory>
#include <string>

#include "enum/music_message_type.h"
#include "message/entity_id.h"
#include "message/music_message.h"

namespace music {

// Encapsulate the information required to define an event.
//
// Directly from the standard (section 12):
// "Events are sent by constructs to the system to denote that an event has
// occurred. Each event message is generic in structure, however the payload of
// the event must have its structure defined by an event payload definition. The
// event payload type definition defines the structure of the data that will be
// sent out in an event message...This event data is used to carry any relevant
// information for that event such as location data that a subscriber to that
// type might wish to consume and reason over."
class EventMessage : public MusicMessage {
  template <class T>
  friend class EventMessageBuilderBase;

 public:
  inline static const std::string kEventTypeJsonKey = "eventType";
  inline static const std::string kEventDataJsonKey = "eventData";

  // Used in DIS serialization
  inline static const long kEventTypeId = 454310001L;
  inline static const long kEventDataId = 454310002L;

  EventMessage() : MusicMessage() { type_ = MusicMessageType::kEvent; }

  const EntityId &GetOriginId() const { return origin_id_; }
  const std::string &GetEventType() const { return event_type_; }
  const nlohmann::json &GetEventData() const { return event_data_; }

  void SetOriginId(const EntityId &origin_id) { origin_id_ = origin_id; }
  void SetEventType(const std::string &event_type) { event_type_ = event_type; }
  void SetEventData(const nlohmann::json &event_data) {
    event_data_ = event_data;
  }
  void Validate() const;

  friend void to_json(nlohmann::json &j, const EventMessage &mesg);
  friend void from_json(const nlohmann::json &j, EventMessage &mesg);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static EventMessage FromJson(const nlohmann::json &j) {
    EventMessage mesg;
    from_json(j, mesg);
    return mesg;
  }

 protected:
  // The originator of the event
  EntityId origin_id_{};

  // Event payload definition
  std::string event_type_{};

  // The event payload itself
  nlohmann::json event_data_{};
};

template <class T>
class EventMessageBuilderBase : public MessageBuilder<T> {
 public:
  T &SetOriginId(const EntityId &origin_id) {
    wip_->origin_id_ = origin_id;
    return static_cast<T &>(*this);
  }

  T &SetEventType(const std::string &event_type) {
    wip_->event_type_ = event_type;
    return static_cast<T &>(*this);
  }

  T &SetEventData(const nlohmann::json &event_data) {
    wip_->event_data_ = event_data;
    return static_cast<T &>(*this);
  }

  T &SetEventData(const std::string &event_data) {
    wip_->event_data_ = event_data;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<EventMessage> wip_;
  void SetWIP(std::shared_ptr<EventMessage> event_mesg) {
    wip_ = event_mesg;
    MessageBuilder<T>::SetBuiltMessage(wip_);
  }
};

class EventMessageBuilder
    : public EventMessageBuilderBase<EventMessageBuilder> {
 public:
  EventMessageBuilder() { SetWIP(std::make_shared<EventMessage>()); }
  EventMessage Build() { return *wip_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_EVENT_EVENT_MESSAGE_H