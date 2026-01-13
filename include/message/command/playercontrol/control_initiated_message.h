// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_PLAYERCONTROL_CONTROL_INITIATED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_PLAYERCONTROL_CONTROL_INITIATED_MESSAGE_H_

#include "message/command/music_command_request_message.h"
namespace music {

class ControlInitiatedMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class ControlInitiatedMessageBuilderTemplate;

 public:
  // Constructor
  ControlInitiatedMessage() {
    type_ = MusicMessageType::kControlInitiated;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kControlInitiated)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ControlInitiatedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlInitiatedMessage &message);
  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static ControlInitiatedMessage FromJson(const nlohmann::json &j) {
    ControlInitiatedMessage message;
    from_json(j, message);
    return message;
  }

  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const ControlInitiatedMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "ControlInitiatedMessage: " + ToJson().dump();
  }

 protected:
  // No new fields needed for this message
};

// Template builder class
template <class T>
class ControlInitiatedMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlInitiatedMessage> control_initiated_message_;
  void SetBuiltMessage(
      std::shared_ptr<MusicCommandRequestMessage> new_message) {
    control_initiated_message_ =
        std::static_pointer_cast<ControlInitiatedMessage>(new_message);
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete builder
class ControlInitiatedMessageBuilder
    : public ControlInitiatedMessageBuilderTemplate<
          ControlInitiatedMessageBuilder> {
 public:
  ControlInitiatedMessageBuilder()
      : ControlInitiatedMessageBuilderTemplate<
            ControlInitiatedMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<ControlInitiatedMessage>());
  }
  ControlInitiatedMessage Build() { return *control_initiated_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_PLAYERCONTROL_CONTROL_INITIATED_MESSAGE_H_