// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_PRIMARY_CONTROL_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_PRIMARY_CONTROL_REQUEST_MESSAGE_H_

#include "message/music_request_message.h"

namespace music {
// Request to gain primary control of a construct.
class ConstructPrimaryControlRequestMessage : public MusicRequestMessage {
  template <class T>
  friend class ConstructPrimaryControlRequestMessageBuilderTemplate;

 public:
  // Constructor
  ConstructPrimaryControlRequestMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kPrimaryControlRequest;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ConstructPrimaryControlRequestMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ConstructPrimaryControlRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ConstructPrimaryControlRequestMessage FromJson(
      const nlohmann::json &j) {
    ConstructPrimaryControlRequestMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(
      std::ostream &os, const ConstructPrimaryControlRequestMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ConstructPrimaryControlRequestMessage: " + ToJson().dump();
  }
};
// Template Builder
template <class T>
class ConstructPrimaryControlRequestMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ConstructPrimaryControlRequestMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ = std::static_pointer_cast<ConstructPrimaryControlRequestMessage>(
        new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
class ConstructPrimaryControlRequestMessageBuilder
    : public ConstructPrimaryControlRequestMessageBuilderTemplate<
          ConstructPrimaryControlRequestMessageBuilder> {
 public:
  ConstructPrimaryControlRequestMessageBuilder() {
    SetBuiltMessage(std::make_shared<ConstructPrimaryControlRequestMessage>());
  }
  ConstructPrimaryControlRequestMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONSTRUCT_PRIMARY_CONTROL_REQUEST_MESSAGE_H_