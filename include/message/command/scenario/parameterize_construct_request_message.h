// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_PARAMETERIZE_CONSTRUCT_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_PARAMETERIZE_CONSTRUCT_REQUEST_MESSAGE_H_

#include <optional>

#include "message/command/music_command_request_message.h"
namespace music {

class ParameterizeConstructRequestMessage : public MusicCommandRequestMessage {
 public:
  // Constructor
  ParameterizeConstructRequestMessage() {
    type_ = MusicMessageType::kParameterizeConstructRequest;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kParameterizeConstructRequest)
                              .action_type;
  }

  // Getters
  const std::optional<EntityId> &GetGhostedId() const { return ghosted_id_; }
  const nlohmann::json &GetConstructParameters() const {
    return construct_parameters_;
  }

  // Setters
  void SetGhostedId(const EntityId &ghosted_id) { ghosted_id_ = ghosted_id; }
  void SetGhostedId(const std::nullopt_t &) { ghosted_id_ = std::nullopt; }
  void SetConstructParameters(const nlohmann::json &construct_parameters) {
    construct_parameters_ = construct_parameters;
  }

  // Validate
  void Validate() const;

  // Variable Datum IDs
  inline static const long kGhostedConstructId = 1L;
  inline static const long kConstructParametersId = 2L;

  // JSON Keys
  inline static std::string kGhostedIdKey = "ghostedID";
  inline static std::string kConstructParametersKey = "constructParameters";

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ParameterizeConstructRequestMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ParameterizeConstructRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ParameterizeConstructRequestMessage FromJson(const nlohmann::json &j) {
    ParameterizeConstructRequestMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(
      std::ostream &os, const ParameterizeConstructRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "ParameterizeConstructRequestMessage: " + ToJson().dump();
  }

 protected:
  // The Site, App, and ID of the entity to ghost on, if applicable.
  std::optional<EntityId> ghosted_id_{};
  // The JSON containing all the relevant parameters to the target construct.
  nlohmann::json construct_parameters_{};
};

// This class has no new members, but will still need its own builder. The
// fields contained in its super classes can be set through this builderand this
// class' default constructor will be called which will set its message type and
// command identifier.

// Builder
template <class T>
class ParameterizeConstructRequestMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 public:
  T &SetGhostedId(const EntityId &ghosted_id) {
    parameterize_construct_message_->SetGhostedId(ghosted_id);
    return static_cast<T &>(*this);
  }

  T &SetConstructParameters(const nlohmann::json &construct_parameters) {
    parameterize_construct_message_->SetConstructParameters(
        construct_parameters);
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<ParameterizeConstructRequestMessage>
      parameterize_construct_message_;

  void SetBuiltMessage(
      std::shared_ptr<ParameterizeConstructRequestMessage> new_message) {
    parameterize_construct_message_ = new_message;
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }

  void EnforceSystemToSystemEntities() {
    // enforce origin entity number to be 0
    auto new_origin_id = parameterize_construct_message_->GetOriginId();
    new_origin_id.SetEntityNumber(0);
    parameterize_construct_message_->SetOriginId(new_origin_id);
  }
};

class ParameterizeConstructRequestMessageBuilder
    : public ParameterizeConstructRequestMessageBuilderTemplate<
          ParameterizeConstructRequestMessageBuilder> {
 public:
  ParameterizeConstructRequestMessageBuilder() {
    parameterize_construct_message_ =
        std::make_shared<ParameterizeConstructRequestMessage>();
    SetBuiltMessage(parameterize_construct_message_);
  }
  ParameterizeConstructRequestMessage Build() {
    EnforceSystemToSystemEntities();
    return *parameterize_construct_message_;
  }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_PARAMETERIZE_CONSTRUCT_REQUEST_MESSAGE_H_