// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_STATE_FIELD_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_STATE_FIELD_MESSAGE_H_

#include <nlohmann/json_fwd.hpp>

#include "message/entity_id.h"
#include "message/music_message.h"
namespace music {

class StateFieldMessage : public MusicMessage {
  template <class T>
  friend class StateFieldMessageBuilderTemplate;

 public:
  StateFieldMessage() { type_ = MusicMessageType::kStateField; }

  void Validate();

  // Serialization Keys
  static inline const std::string kOriginIdKey = "originID";
  static inline const std::string kStateDataKey = "stateData";

  // Getters
  const nlohmann::json &GetStateData() const { return state_data_; }
  const EntityId &GetOriginId() const { return origin_id_; }

  // Setters
  void SetStateData(const nlohmann::json &data) { state_data_ = data; }
  void SetOriginId(const EntityId &id) { origin_id_ = id; }

  // JSON Serialization
  friend void to_json(nlohmann::json &json,
                      const StateFieldMessage &stateField);
  friend void from_json(const nlohmann::json &json,
                        StateFieldMessage &stateField);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static StateFieldMessage FromJson(const nlohmann::json &j) {
    StateFieldMessage message;
    from_json(j, message);
    return message;
  }

 protected:
  nlohmann::json state_data_{};
  EntityId origin_id_{};
};

template <class T>
class StateFieldMessageBuilderTemplate : public MessageBuilder<T> {
 public:
  T &SetStateData(const nlohmann::json &data) {
    message_->state_data_ = data;
    return static_cast<T &>(*this);
  }

  T &SetOriginId(const EntityId &id) {
    message_->origin_id_ = id;
    return static_cast<T &>(*this);
  }

 protected:
  void SetBuiltMessage(std::shared_ptr<StateFieldMessage> new_message) {
    MessageBuilder<T>::SetBuiltMessage(message_ = new_message);
  }
  std::shared_ptr<StateFieldMessage> message_;
};

class StateFieldMessageBuilder
    : public StateFieldMessageBuilderTemplate<StateFieldMessageBuilder> {
 public:
  StateFieldMessageBuilder() {
    SetBuiltMessage(std::make_shared<StateFieldMessage>());
  }
  StateFieldMessage Build() { return *message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONSTRUCT_STATE_FIELD_MESSAGE_H_