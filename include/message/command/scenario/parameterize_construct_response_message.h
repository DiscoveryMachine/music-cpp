// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_PARAMETERIZE_CONSTRUCT_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_PARAMETERIZE_CONSTRUCT_RESPONSE_MESSAGE_H_

#include "message/command/music_command_response_message.h"

namespace music {
class ParameterizeConstructResponseMessage
    : public MusicCommandResponseMessage {
 public:
  // Constructor
  ParameterizeConstructResponseMessage() {
    type_ = MusicMessageType::kParameterizeConstructResponse;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kParameterizeConstructResponse)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ParameterizeConstructResponseMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ParameterizeConstructResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ParameterizeConstructResponseMessage FromJson(
      const nlohmann::json &j) {
    ParameterizeConstructResponseMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(
      std::ostream &os, const ParameterizeConstructResponseMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "ParameterizeConstructResponseMessage: " + ToJson().dump();
  }

 protected:
  // No new fields needed for this message
};
// This class has no new members, but will still need its own builder. The
// fields contained in its super classes can be set through this builderand this
// class' default constructor will be called which will set its message type and
// command identifier.

template <class T>
class ParameterizeConstructResponseMessageBuilderTemplate
    : public CommandResponseMessageBuilder<T> {
 protected:
  std::shared_ptr<ParameterizeConstructResponseMessage>
      parameterize_construct_response_message_;
  void SetBuiltMessage(
      std::shared_ptr<ParameterizeConstructResponseMessage> new_message) {
    parameterize_construct_response_message_ = new_message;
    CommandResponseMessageBuilder<T>::SetBuiltMessage(new_message);
  }
  void EnforceSystemToSystemEntities() {
    // enforce receiver entity number to be 0
    auto new_receiver_id =
        parameterize_construct_response_message_->GetReceiverId();
    new_receiver_id.SetEntityNumber(0);
    parameterize_construct_response_message_->SetReceiverId(new_receiver_id);
  }
};
class ParameterizeConstructResponseMessageBuilder
    : public ParameterizeConstructResponseMessageBuilderTemplate<
          ParameterizeConstructResponseMessageBuilder> {
 public:
  ParameterizeConstructResponseMessageBuilder() {
    parameterize_construct_response_message_ =
        std::make_shared<ParameterizeConstructResponseMessage>();
    SetBuiltMessage(parameterize_construct_response_message_);
  }
  ParameterizeConstructResponseMessage Build() {
    EnforceSystemToSystemEntities();
    return *parameterize_construct_response_message_;
  }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_PARAMETERIZE_CONSTRUCT_RESPONSE_MESSAGE_H_