// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_RECLAMATION_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_RECLAMATION_MESSAGE_H_

#include "message/music_request_message.h"

namespace music {

// Message sent from the primary controller of a construct to the controlled
// construct indicating that it is reclaiming current control of the construct.
// The controlled construct should send a Control Lost message to its current
// controller and then update its current controller attribute.
class ControlReclamationMessage : public MusicRequestMessage {
  template <class T>
  friend class ControlReclamationMessageBuilderTemplate;

 public:
  // Constructor
  ControlReclamationMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlReclamation;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ControlReclamationMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlReclamationMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlReclamationMessage FromJson(const nlohmann::json &j) {
    ControlReclamationMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const ControlReclamationMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlReclamationMessage: " + ToJson().dump();
  }
};

// Template Builder
template <class T>
class ControlReclamationMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlReclamationMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ = std::static_pointer_cast<ControlReclamationMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete Builder
class ControlReclamationMessageBuilder
    : public ControlReclamationMessageBuilderTemplate<
          ControlReclamationMessageBuilder> {
 public:
  ControlReclamationMessageBuilder()
      : ControlReclamationMessageBuilderTemplate<
            ControlReclamationMessageBuilder>() {
    SetBuiltMessage(std::make_shared<ControlReclamationMessage>());
  }
  ControlReclamationMessage Build() { return *message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_RECLAMATION_MESSAGE_H_