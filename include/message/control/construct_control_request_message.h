// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_CONTROL_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_CONTROL_REQUEST_MESSAGE_H_

#include <nlohmann/json_fwd.hpp>

#include "message/entity_id.h"
#include "message/music_request_message.h"

namespace music {
// Request to gain current control of a construct.
class ConstructControlRequestMessage : public MusicRequestMessage {
  template <class T>
  friend class ConstructControlRequestMessageBuilderTemplate;

 public:
  // Constructor
  ConstructControlRequestMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlRequest;
  }

  static inline const std::string kTargetConstructKey = "targetConstruct";
  static inline const std::string kContextKey = "context";
  static inline const long kContextId = 454210002L;
  static inline const long kTargetConstructId = 454210001L;

  // Setters/getters
  const EntityId &GetTargetConstruct() const { return target_construct_; }
  const std::string &GetContext() const { return context_; }
  // Helper/getter for DIS serialization
  const nlohmann::json GetContextJson() const {
    return nlohmann::json({{"context", context_}});
  }

  void SetTargetConstruct(const EntityId &new_construct_id) {
    target_construct_ = new_construct_id;
  }
  void SetContext(const nlohmann::json &new_context) { context_ = new_context; }

  friend void from_json(const nlohmann::json &jsn,
                        ConstructControlRequestMessage &message);
  friend void to_json(nlohmann::json &jsn,
                      const ConstructControlRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static ConstructControlRequestMessage FromJson(const nlohmann::json &j) {
    ConstructControlRequestMessage message;
    from_json(j, message);
    return message;
  }

  // Validate
  void Validate() const;

 protected:
  EntityId target_construct_{};
  std::string context_{};
};
// Template Builder
template <class T>
class ConstructControlRequestMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 public:
  T &SetTargetConstruct(EntityId new_construct_id) {
    message_->target_construct_ = new_construct_id;
    return static_cast<T &>(*this);
  }

  T &SetContext(std::string new_context) {
    message_->context_ = new_context;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<ConstructControlRequestMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ =
        std::static_pointer_cast<ConstructControlRequestMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

class ConstructControlRequestMessageBuilder
    : public ConstructControlRequestMessageBuilderTemplate<
          ConstructControlRequestMessageBuilder> {
 public:
  ConstructControlRequestMessageBuilder() {
    SetBuiltMessage(std::make_shared<ConstructControlRequestMessage>());
  }
  ConstructControlRequestMessage Build() { return *message_; }
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_CONTROL_REQUEST_MESSAGE_H_