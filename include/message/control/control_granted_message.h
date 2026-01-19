// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_GRANTED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_GRANTED_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

// Message sent by the target of a Construct Control Request to the original
// requester informing the requester construct that it is now in control of this
// construct, and that this constructs current controller attribute is set to
// the requester.
class ControlGrantedMessage : public MusicRequestMessage {
  template <class T>
  friend class ControlGrantedMessageBuilderTemplate;

 public:
  // Constructor
  ControlGrantedMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlGranted;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j, const ControlGrantedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlGrantedMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlGrantedMessage FromJson(const nlohmann::json &j) {
    ControlGrantedMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const ControlGrantedMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlGrantedMessage: " + ToJson().dump();
  }
};
// Template Builder
template <class T>
class ControlGrantedMessageBuilderTemplate : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlGrantedMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ = std::static_pointer_cast<ControlGrantedMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete builder
class ControlGrantedMessageBuilder
    : public ControlGrantedMessageBuilderTemplate<
          ControlGrantedMessageBuilder> {
 public:
  ControlGrantedMessageBuilder()
      : ControlGrantedMessageBuilderTemplate<ControlGrantedMessageBuilder>() {
    SetBuiltMessage(std::make_shared<ControlGrantedMessage>());
  }
  ControlGrantedMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_GRANTED_MESSAGE_H_