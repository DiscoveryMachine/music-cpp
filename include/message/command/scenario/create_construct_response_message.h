// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_CONSTRUCT_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_CONSTRUCT_RESPONSE_MESSAGE_H_

#include <ostream>

#include "message/command/music_command_response_message.h"
#include "message/entity_id.h"

namespace music {

// This command returns the construct ID of the created construct as well as a
// status to inform if there were any issues. Command id should be 454013003.

class CreateConstructResponseMessage : public MusicCommandResponseMessage {
  friend class CreateConstructResponseMessageBuilder;

 public:
  // Constructor
  CreateConstructResponseMessage() {
    type_ = MusicMessageType::kCreateConstructResponse;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kCreateConstructResponse)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // Getters
  const EntityId &GetConstructId() const { return construct_id_; }

  // Setters
  void SetConstructId(const EntityId &construct_id) {
    construct_id_ = construct_id;
  }

  // Static constants
  inline static const long kConstructId = 1L;
  inline static const long kCommandId =
      MusicMessageTypeHelper::GetInfoForType(
          MusicMessageType::kCreateConstructResponse)
          .action_type;

  // Serialization Keys
  inline static const std::string kConstructIdKey = "constructID";
  inline static const long kConstructIdId = 1L;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const CreateConstructResponseMessage &message);

  friend void from_json(const nlohmann::json &j,
                        CreateConstructResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static CreateConstructResponseMessage FromJson(const nlohmann::json &j) {
    CreateConstructResponseMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(
      std::ostream &os, const CreateConstructResponseMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "CreateConstructResponseMessage: " + ToJson().dump();
  }

 protected:
  // The ID of the construct that was created as a result of this request.
  EntityId construct_id_{};
};

// Builder
class CreateConstructResponseMessageBuilder
    : public CommandResponseMessageBuilder<
          CreateConstructResponseMessageBuilder> {
 public:
  CreateConstructResponseMessageBuilder() {
    create_construct_message_ =
        std::make_shared<CreateConstructResponseMessage>();
    SetBuiltMessage(create_construct_message_);
  }
  // Setters that return 'this' (the builder itself)

  CreateConstructResponseMessageBuilder &SetConstructId(
      const EntityId &construct_id) {
    create_construct_message_->construct_id_ = construct_id;
    return *this;
  }
  // Build method to create the final object
  CreateConstructResponseMessage Build() {
    EnforceSystemToSystemEntities();
    return *create_construct_message_;
  }

 protected:
  std::shared_ptr<CreateConstructResponseMessage> create_construct_message_;

  void SetBuiltMessage(
      std::shared_ptr<CreateConstructResponseMessage> new_message) {
    create_construct_message_ = new_message;
    CommandResponseMessageBuilder<
        CreateConstructResponseMessageBuilder>::SetBuiltMessage(new_message);
  }

  void EnforceSystemToSystemEntities() {
    // enforce origin and receiver entity numbers to be 0
    auto new_origin_id = create_construct_message_->GetOriginId();
    new_origin_id.SetEntityNumber(0);
    auto new_receiver_id = create_construct_message_->GetReceiverId();
    new_receiver_id.SetEntityNumber(0);
    create_construct_message_->SetOriginId(new_origin_id);
    create_construct_message_->SetReceiverId(new_receiver_id);
  }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_CONSTRUCT_RESPONSE_MESSAGE_H__MUSIC_COMMAND_RESPONSE_MESSAGE_H_