// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_LOST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_LOST_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

//  Message sent as part of a Construct Control Request and Control Reclamation
//  informing the construct receiving this message that it is no longer the
//  current controller of the given construct.
class ControlLostMessage : public MusicRequestMessage {
  template <class T>
  friend class ControlLostMessageBuilderTemplate;

 public:
  // Constructor
  ControlLostMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlLost;
  }
  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j, const ControlLostMessage &message);
  friend void from_json(const nlohmann::json &j, ControlLostMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlLostMessage FromJson(const nlohmann::json &j) {
    ControlLostMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const ControlLostMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlLostMessage: " + ToJson().dump();
  }
};
// Template Builder
template <class T>
class ControlLostMessageBuilderTemplate : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlLostMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ = std::static_pointer_cast<ControlLostMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete Builder
class ControlLostMessageBuilder
    : public ControlLostMessageBuilderTemplate<ControlLostMessageBuilder> {
 public:
  ControlLostMessageBuilder()
      : ControlLostMessageBuilderTemplate<ControlLostMessageBuilder>() {
    SetBuiltMessage(std::make_shared<ControlLostMessage>());
  }
  ControlLostMessage Build() { return *message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_LOST_MESSAGE_H_