// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_PRIMARY_CONTROL_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_PRIMARY_CONTROL_RESPONSE_MESSAGE_H_

#include "message/music_response_message.h"
namespace music {

// Response to a Request to gain primary control of a construct.
class ConstructPrimaryControlResponseMessage : public MusicResponseMessage {
  template <class T>
  friend class ConstructPrimaryControlResponseMessageBuilderTemplate;

 public:
  // Constructor
  ConstructPrimaryControlResponseMessage() : MusicResponseMessage() {
    type_ = MusicMessageType::kPrimaryControlResponse;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ConstructPrimaryControlResponseMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ConstructPrimaryControlResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ConstructPrimaryControlResponseMessage FromJson(
      const nlohmann::json &j) {
    ConstructPrimaryControlResponseMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(
      std::ostream &os, const ConstructPrimaryControlResponseMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ConstructPrimaryControlResponseMessage: " + ToJson().dump();
  }
};

// Template Builder
template <class T>
class ConstructPrimaryControlResponseMessageBuilderTemplate
    : public ResponseMessageBuilder<T> {
 protected:
  std::shared_ptr<ConstructPrimaryControlResponseMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicResponseMessage> new_message) {
    message_ = std::static_pointer_cast<ConstructPrimaryControlResponseMessage>(
        new_message);
    ResponseMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
class ConstructPrimaryControlResponseMessageBuilder
    : public ConstructPrimaryControlResponseMessageBuilderTemplate<
          ConstructPrimaryControlResponseMessageBuilder> {
 public:
  ConstructPrimaryControlResponseMessageBuilder() {
    SetBuiltMessage(std::make_shared<ConstructPrimaryControlResponseMessage>());
  }
  ConstructPrimaryControlResponseMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_PRIMARY_CONTROL_RESPONSE_MESSAGE_H_