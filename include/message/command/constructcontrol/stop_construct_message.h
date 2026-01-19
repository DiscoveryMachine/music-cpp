// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_CONSTRUCTCONTROL_STOP_CONSTRUCT_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_CONSTRUCTCONTROL_STOP_CONSTRUCT_MESSAGE_H_

#include "message/command/music_command_request_message.h"
namespace music {

// This command stops all the specified construct’s current interactions. Any
// active interactions should have their status set to "Aborted". (Note: The
// Receiving ID is the ID of the construct to stop) .Command id should be
// 454007001

// The receiver_id_ field of the MusicCommandRequestMessage refers to the
// construct that is to be stopped, and the origin_id_ field refers to the
// entity issuing the command.

class StopConstructMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class StopConstructMessageBuilderTemplate;

 public:
  // Constructor
  StopConstructMessage() {
    type_ = MusicMessageType::kStopConstruct;
    command_identifier_ =
        MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kStopConstruct)
            .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j, const StopConstructMessage &message);
  friend void from_json(const nlohmann::json &j, StopConstructMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static StopConstructMessage FromJson(const nlohmann::json &j) {
    StopConstructMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const StopConstructMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "StopConstructMessage: " + ToJson().dump();
  }
};
// Template builder class
template <class T>
class StopConstructMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 public:
  StopConstructMessageBuilderTemplate() {
    stop_construct_message_ = std::make_shared<StopConstructMessage>();
    SetBuiltMessage(stop_construct_message_);
  }

 protected:
  std::shared_ptr<StopConstructMessage> stop_construct_message_;
  void SetBuiltMessage(
      std::shared_ptr<MusicCommandRequestMessage> new_message) {
    stop_construct_message_ =
        std::static_pointer_cast<StopConstructMessage>(new_message);
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete builder
class StopConstructMessageBuilder
    : public StopConstructMessageBuilderTemplate<StopConstructMessageBuilder> {
 public:
  StopConstructMessageBuilder()
      : StopConstructMessageBuilderTemplate<StopConstructMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<StopConstructMessage>());
  }
  StopConstructMessage Build() { return *stop_construct_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_CONSTRUCTCONTROL_STOP_CONSTRUCT_MESSAGE_H_