// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SIMULATION_DISPLAY_MESSAGES_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SIMULATION_DISPLAY_MESSAGES_MESSAGE_H_

#include <limits>

#include "message/command/music_command_request_message.h"

namespace music {

class DisplayMessagesMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class DisplayMessagesMessageBuilderTemplate;

 public:
  // Constructor
  DisplayMessagesMessage() {
    type_ = MusicMessageType::kDisplayMessages;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kDisplayMessages)
                              .action_type;
  }
  // Validate
  void Validate() const;

  // Serialization Keys
  inline static const std::string kMessagesKey = "messages";
  inline static const std::string kTimeoutKey = "timeout";
  inline static const long kTimeoutId = 1L;
  inline static const long kMessagesId = 2L;

  // Getters
  const std::vector<std::string> &GetMessages() const { return messages_; }
  const long GetTimeout() const { return timeout_; }

  // Setters
  void SetMessages(const std::vector<std::string> &messages) {
    messages_ = messages;
  }

  void SetTimeout(long timeout) { timeout_ = timeout; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const DisplayMessagesMessage &message);
  friend void from_json(const nlohmann::json &j,
                        DisplayMessagesMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static DisplayMessagesMessage FromJson(const nlohmann::json &j) {
    DisplayMessagesMessage message;
    from_json(j, message);
    return message;
  }

  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const DisplayMessagesMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "DisplayMessageMessage: " + ToJson().dump();
  }

 protected:
  // A list of text to be displayed as messages. Each element of the array is
  // another message to display on a separate line.
  std::vector<std::string> messages_{};

  // Time in seconds that the messages specified above should display to the
  // player before disappearing. A value of 0 will display the messages
  // indefinitely unless the simulation receives another Display Messages
  // command to override this one.
  long timeout_{std::numeric_limits<long>::min()};
};

// Template builder class
template <class T>
class DisplayMessagesMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 public:
  T &SetMessages(const std::vector<std::string> &messages) {
    display_messages_message_->messages_ = messages;
    return static_cast<T &>(*this);
  }
  T &SetTimeout(long timeout) {
    display_messages_message_->timeout_ = timeout;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<DisplayMessagesMessage> display_messages_message_;
  void SetBuiltMessage(
      std::shared_ptr<MusicCommandRequestMessage> new_message) {
    display_messages_message_ =
        std::static_pointer_cast<DisplayMessagesMessage>(new_message);
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete builder
class DisplayMessagesMessageBuilder
    : public DisplayMessagesMessageBuilderTemplate<
          DisplayMessagesMessageBuilder> {
 public:
  DisplayMessagesMessageBuilder()
      : DisplayMessagesMessageBuilderTemplate<DisplayMessagesMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<DisplayMessagesMessage>());
  }
  DisplayMessagesMessage Build() { return *display_messages_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SIMULATION_DISPLAY_MESSAGES_MESSAGE_H_