// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_CONTROL_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_CONTROL_RESPONSE_MESSAGE_H_

#include "message/music_response_message.h"
namespace music {

// Response to a Request to gain current control of a construct.
class ConstructControlResponseMessage : public MusicResponseMessage {
  template <class T>
  friend class ConstructControlResponseMessageBuilderTemplate;

 public:
  // Constructor
  ConstructControlResponseMessage() : MusicResponseMessage() {
    type_ = MusicMessageType::kControlResponse;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ConstructControlResponseMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ConstructControlResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ConstructControlResponseMessage FromJson(const nlohmann::json &j) {
    ConstructControlResponseMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(
      std::ostream &os, const ConstructControlResponseMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ConstructControlResponseMessage: " + ToJson().dump();
  }
};

// Template Builder
template <class T>
class ConstructControlResponseMessageBuilderTemplate
    : public ResponseMessageBuilder<T> {
 protected:
  std::shared_ptr<ConstructControlResponseMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicResponseMessage> new_message) {
    message_ =
        std::static_pointer_cast<ConstructControlResponseMessage>(new_message);
    ResponseMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
class ConstructControlResponseMessageBuilder
    : public ConstructControlResponseMessageBuilderTemplate<
          ConstructControlResponseMessageBuilder> {
 public:
  ConstructControlResponseMessageBuilder() {
    SetBuiltMessage(std::make_shared<ConstructControlResponseMessage>());
  }

  ConstructControlResponseMessage Build() { return *message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_CONTROL_RESPONSE_MESSAGE_H_