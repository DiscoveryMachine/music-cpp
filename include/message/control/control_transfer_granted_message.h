// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_GRANTED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_GRANTED_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

// Message sent in response to Control Transfer Request informing the requester
// construct of the status of its request. Message sent by the target of a
// Control Transfer Request to the proposed controller construct informing that
// construct that it is now in control of this construct, and that this
// constructs current controller attribute is set to the proposed controller.
class ControlTransferGrantedMessage : public MusicRequestMessage {
  template <class T>
  friend class ControlTransferGrantedMessageBuilderTemplate;

 public:
  // Constructor
  ControlTransferGrantedMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlTransferGranted;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ControlTransferGrantedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlTransferGrantedMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlTransferGrantedMessage FromJson(const nlohmann::json &j) {
    ControlTransferGrantedMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(
      std::ostream &os, const ControlTransferGrantedMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlTransferGrantedMessage: " + ToJson().dump();
  }
};
// Template Builder
template <class T>
class ControlTransferGrantedMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlTransferGrantedMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ =
        std::static_pointer_cast<ControlTransferGrantedMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete builder
class ControlTransferGrantedMessageBuilder
    : public ControlTransferGrantedMessageBuilderTemplate<
          ControlTransferGrantedMessageBuilder> {
 public:
  ControlTransferGrantedMessageBuilder()
      : ControlTransferGrantedMessageBuilderTemplate<
            ControlTransferGrantedMessageBuilder>() {
    SetBuiltMessage(std::make_shared<ControlTransferGrantedMessage>());
  }
  ControlTransferGrantedMessage Build() { return *message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_GRANTED_MESSAGE_H_