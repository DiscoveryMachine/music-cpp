// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_ENVIRONMENT_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_ENVIRONMENT_RESPONSE_MESSAGE_H_

#include "message/command/music_command_response_message.h"

namespace music {
// MessageType must be MusicMessageType::kCreateEnvironmentResponse
class CreateEnvironmentResponseMessage : public MusicCommandResponseMessage {
 public:
  // Constructor
  CreateEnvironmentResponseMessage() {
    type_ = MusicMessageType::kCreateEnvironmentResponse;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kCreateEnvironmentResponse)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const CreateEnvironmentResponseMessage &message);
  friend void from_json(const nlohmann::json &j,
                        CreateEnvironmentResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static CreateEnvironmentResponseMessage FromJson(const nlohmann::json &j) {
    CreateEnvironmentResponseMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(
      std::ostream &os, const CreateEnvironmentResponseMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "CreateEnvironmentResponseMessage: " + ToJson().dump();
  }

 protected:
  // No new members
};
// This class has no new members, but will still need its own builder. The
// fields contained in its super classes can be set through this builderand this
// class' default constructor will be called which will set its message type and
// command identifier.
class CreateEnvironmentResponseMessageBuilder
    : public CommandResponseMessageBuilder<
          CreateEnvironmentResponseMessageBuilder> {
 public:
  CreateEnvironmentResponseMessageBuilder() {
    create_environment_message_ =
        std::make_shared<CreateEnvironmentResponseMessage>();
    SetBuiltMessage(create_environment_message_);
  }
  CreateEnvironmentResponseMessage Build() {
    EnforceSystemToSystemEntities();
    return *create_environment_message_;
  }

 protected:
  std::shared_ptr<CreateEnvironmentResponseMessage> create_environment_message_;

  void SetBuiltMessage(
      std::shared_ptr<CreateEnvironmentResponseMessage> new_message) {
    create_environment_message_ = new_message;
    CommandResponseMessageBuilder<
        CreateEnvironmentResponseMessageBuilder>::SetBuiltMessage(new_message);
  }

  void EnforceSystemToSystemEntities() {
    // enforce origin and receiver entity numbers to be 0
    auto new_origin_id = create_environment_message_->GetOriginId();
    new_origin_id.SetEntityNumber(0);
    auto new_receiver_id = create_environment_message_->GetReceiverId();
    new_receiver_id.SetEntityNumber(0);
    create_environment_message_->SetOriginId(new_origin_id);
    create_environment_message_->SetReceiverId(new_receiver_id);
  }
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_ENVIRONMENT_RESPONSE_MESSAGE_H_