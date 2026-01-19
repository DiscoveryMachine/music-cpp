// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_PLAYERCONTROL_CONTROL_RELEASED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_PLAYERCONTROL_CONTROL_RELEASED_MESSAGE_H_

#include "message/command/music_command_request_message.h"

namespace music {

class ControlReleasedMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class ControlReleasedMessageBuilderTemplate;

 public:
  // Constructor
  ControlReleasedMessage() {
    type_ = MusicMessageType::kControlReleased;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kControlReleased)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j, const ControlReleasedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlReleasedMessage &message);
  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static ControlReleasedMessage FromJson(const nlohmann::json &j) {
    ControlReleasedMessage message;
    from_json(j, message);
    return message;
  }

  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const ControlReleasedMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "ControlReleasedMessage: " + ToJson().dump();
  }

 protected:
  // No new fields needed for this message
};

// Template builder class
template <class T>
class ControlReleasedMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlReleasedMessage> control_released_message_;
  void SetBuiltMessage(
      std::shared_ptr<MusicCommandRequestMessage> new_message) {
    control_released_message_ =
        std::static_pointer_cast<ControlReleasedMessage>(new_message);
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete builder
class ControlReleasedMessageBuilder
    : public ControlReleasedMessageBuilderTemplate<
          ControlReleasedMessageBuilder> {
 public:
  ControlReleasedMessageBuilder()
      : ControlReleasedMessageBuilderTemplate<ControlReleasedMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<ControlReleasedMessage>());
  }
  ControlReleasedMessage Build() { return *control_released_message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_PLAYERCONTROL_CONTROL_RELEASED_MESSAGE_H_
