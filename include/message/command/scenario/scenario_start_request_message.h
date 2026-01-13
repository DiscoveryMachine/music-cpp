// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_SCENARIO_START_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_SCENARIO_START_REQUEST_MESSAGE_H_

#include "message/command/music_command_request_message.h"
namespace music {

class ScenarioStartRequestMessage : public MusicCommandRequestMessage {
 public:
  // Constructor
  ScenarioStartRequestMessage() {
    type_ = MusicMessageType::kScenarioStart;
    command_identifier_ =
        MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kScenarioStart)
            .action_type;
    request_id_ = 0;  // not used, setting to a valid value for Validate()
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ScenarioStartRequestMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ScenarioStartRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ScenarioStartRequestMessage FromJson(const nlohmann::json &j) {
    ScenarioStartRequestMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const ScenarioStartRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "ScenarioStartRequestMessage: " + ToJson().dump();
  }

 protected:
  // No new fields needed for this message
};

// This class has no new members, but will still need its own builder. The
// fields contained in its super classes can be set through this builderand this
// class' default constructor will be called which will set its message type and
// command identifier.
template <class T>
class ScenarioStartRequestMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ScenarioStartRequestMessage> scenario_start_request_message_;
  void SetBuiltMessage(
      std::shared_ptr<ScenarioStartRequestMessage> new_message) {
    scenario_start_request_message_ = new_message;
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
  void EnforceSystemToSystemEntities() {
    // enforce origin and receiver entity numbers to be 0
    auto new_origin_id = scenario_start_request_message_->GetOriginId();
    new_origin_id.SetEntityNumber(0);
    auto new_receiver_id = scenario_start_request_message_->GetReceiverId();
    new_receiver_id.SetEntityNumber(0);
    scenario_start_request_message_->SetOriginId(new_origin_id);
    scenario_start_request_message_->SetReceiverId(new_receiver_id);
  }
};
class ScenarioStartRequestMessageBuilder
    : public ScenarioStartRequestMessageBuilderTemplate<
          ScenarioStartRequestMessageBuilder> {
 public:
  ScenarioStartRequestMessageBuilder() {
    scenario_start_request_message_ =
        std::make_shared<ScenarioStartRequestMessage>();
    SetBuiltMessage(scenario_start_request_message_);
  }
  ScenarioStartRequestMessage Build() {
    EnforceSystemToSystemEntities();
    return *scenario_start_request_message_;
  }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_SCENARIO_START_REQUEST_MESSAGE_H_