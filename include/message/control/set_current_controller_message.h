// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_SET_CURRENT_CONTROLLER_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_SET_CURRENT_CONTROLLER_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

class SetCurrentControllerMessage : public MusicRequestMessage {
  template <class T>
  friend class SetCurrentControllerMessageBuilderTemplate;

 public:
  // Constructor
  SetCurrentControllerMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kSetCurrentController;
  }

  // Validate
  void Validate() const;

  // Getters
  const EntityId &GetCurrentControllerId() const {
    return current_controller_id_;
  }

  // Setters
  void SetCurrentControllerId(const EntityId &current_controller_id) {
    current_controller_id_ = current_controller_id;
  }

  // JSON and ID Keys
  inline static const std::string kCurrentControllerIdKey =
      "currentControllerID";
  inline static const long kCurrentControllerId = 454210004L;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const SetCurrentControllerMessage &message);
  friend void from_json(const nlohmann::json &j,
                        SetCurrentControllerMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static SetCurrentControllerMessage FromJson(const nlohmann::json &j) {
    SetCurrentControllerMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const SetCurrentControllerMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "SetCurrentControllerMessage: " + ToJson().dump();
  }

 protected:
  // The EntityID of the target constructs new current controller.
  EntityId current_controller_id_{};
};

// Template Builder
template <class T>
class SetCurrentControllerMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 public:
  T &SetCurrentControllerId(const EntityId &current_controller_id) {
    message_->current_controller_id_ = current_controller_id;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<SetCurrentControllerMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ =
        std::static_pointer_cast<SetCurrentControllerMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete Builder
class SetCurrentControllerMessageBuilder
    : public SetCurrentControllerMessageBuilderTemplate<
          SetCurrentControllerMessageBuilder> {
 public:
  SetCurrentControllerMessageBuilder()
      : SetCurrentControllerMessageBuilderTemplate<
            SetCurrentControllerMessageBuilder>() {
    SetBuiltMessage(std::make_shared<SetCurrentControllerMessage>());
  }
  SetCurrentControllerMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_SET_CURRENT_CONTROLLER_MESSAGE_H_