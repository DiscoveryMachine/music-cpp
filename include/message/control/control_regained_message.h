// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_REGAINED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_REGAINED_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

// Message sent as part of Control Relinquished informing the primary controller
// construct receiving this message that it has regained control over the given
// construct.
class ControlRegainedMessage : public MusicRequestMessage {
  template <class T>
  friend class ControlRegainedMessageBuilderTemplate;

 public:
  // Constructor
  ControlRegainedMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlRegained;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j, const ControlRegainedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlRegainedMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlRegainedMessage FromJson(const nlohmann::json &j) {
    ControlRegainedMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const ControlRegainedMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlRegainedMessage: " + ToJson().dump();
  }
};

// Template Builder
template <class T>
class ControlRegainedMessageBuilderTemplate : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlRegainedMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ = std::static_pointer_cast<ControlRegainedMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete Builder
class ControlRegainedMessageBuilder
    : public ControlRegainedMessageBuilderTemplate<
          ControlRegainedMessageBuilder> {
 public:
  ControlRegainedMessageBuilder()
      : ControlRegainedMessageBuilderTemplate<ControlRegainedMessageBuilder>() {
    SetBuiltMessage(std::make_shared<ControlRegainedMessage>());
  }
  ControlRegainedMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_REGAINED_MESSAGE_H_