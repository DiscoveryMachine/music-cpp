// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_ENVIRONMENT_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_ENVIRONMENT_REQUEST_MESSAGE_H_

#include "message/command/music_command_request_message.h"

namespace music {

class CreateEnvironmentRequestMessage : public MusicCommandRequestMessage {
  friend class CreateEnvironmentRequestMessageBuilder;

 public:
  // Constructor
  CreateEnvironmentRequestMessage() {
    type_ = MusicMessageType::kCreateEnvironmentRequest;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kCreateEnvironmentRequest)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kEnvironmentNameKey = "environmentName";
  inline static const std::string kEnvironmentMetadataKey =
      "environmentMetadata";

  // Constants
  inline static const long kEnvironmentNameId = 1L;
  inline static const long kEnvironmentMetadataId = 2L;

  // Getters
  const std::string &GetEnvironmentName() const { return environment_name_; }
  const nlohmann::json &GetEnvironmentMetadata() const {
    return environment_metadata_;
  }

  // Setters
  void SetEnvironmentName(const std::string &environment_name) {
    environment_name_ = environment_name;
  }
  void SetEnvironmentMetadata(const nlohmann::json &environment_metadata) {
    environment_metadata_ = environment_metadata;
  }

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const CreateEnvironmentRequestMessage &message);

  friend void from_json(const nlohmann::json &j,
                        CreateEnvironmentRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static CreateEnvironmentRequestMessage FromJson(const nlohmann::json &j) {
    CreateEnvironmentRequestMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(
      std::ostream &os, const CreateEnvironmentRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "CreateEnvironmentRequestMessage: " + ToJson().dump();
  }

 protected:
  // The name of the scene equivalent the target application should load.
  std::string environment_name_{};

  // The collection of metadata including the coordinates of the origin of
  // the scene for coordinate conversions, the time of day, and potentially
  // other data needed to initialize the environment
  nlohmann::json environment_metadata_{};
};

// Builder
class CreateEnvironmentRequestMessageBuilder
    : public CommandRequestMessageBuilder<
          CreateEnvironmentRequestMessageBuilder> {
 public:
  CreateEnvironmentRequestMessageBuilder() {
    create_environment_message_ =
        std::make_shared<CreateEnvironmentRequestMessage>();
    SetBuiltMessage(create_environment_message_);
  }
  // Setters that return 'this' (the builder itself)
  CreateEnvironmentRequestMessageBuilder &SetEnvironmentName(
      const std::string &environment_name) {
    create_environment_message_->environment_name_ = environment_name;
    return *this;
  }
  CreateEnvironmentRequestMessageBuilder &SetEnvironmentMetadata(
      const nlohmann::json &environment_metadata) {
    create_environment_message_->environment_metadata_ = environment_metadata;
    return *this;
  }
  CreateEnvironmentRequestMessage Build() {
    EnforceSystemToSystemEntities();
    return *create_environment_message_;
  }

 protected:
  std::shared_ptr<CreateEnvironmentRequestMessage> create_environment_message_;

  void SetBuiltMessage(
      std::shared_ptr<CreateEnvironmentRequestMessage> new_message) {
    create_environment_message_ = new_message;
    CommandRequestMessageBuilder<
        CreateEnvironmentRequestMessageBuilder>::SetBuiltMessage(new_message);
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

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_ENVIRONMENT_REQUEST_MESSAGE_H_