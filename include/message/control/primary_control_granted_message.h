// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_PRIMARY_CONTROL_GRANTED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_PRIMARY_CONTROL_GRANTED_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

// Message sent by the target construct of a Construct Primary Control Request
// to the requester construct informing that it is now the primary controller of
// the sending construct. This message is sent after this construct sets its
// primary controller attribute.
class PrimaryControlGrantedMessage : public MusicRequestMessage {
  template <class T>
  friend class PrimaryControlGrantedMessageBuilderTemplate;

 public:
  // Constructor
  PrimaryControlGrantedMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kPrimaryControlGranted;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const PrimaryControlGrantedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        PrimaryControlGrantedMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static PrimaryControlGrantedMessage FromJson(const nlohmann::json &j) {
    PrimaryControlGrantedMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const PrimaryControlGrantedMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "PrimaryControlGrantedMessage: " + ToJson().dump();
  }
};

// Template Builder
template <class T>
class PrimaryControlGrantedMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<PrimaryControlGrantedMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ =
        std::static_pointer_cast<PrimaryControlGrantedMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete Builder
class PrimaryControlGrantedMessageBuilder
    : public PrimaryControlGrantedMessageBuilderTemplate<
          PrimaryControlGrantedMessageBuilder> {
 public:
  PrimaryControlGrantedMessageBuilder() {
    SetBuiltMessage(std::make_shared<PrimaryControlGrantedMessage>());
  }
  PrimaryControlGrantedMessage Build() { return *message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_PRIMARY_CONTROL_GRANTED_MESSAGE_H_