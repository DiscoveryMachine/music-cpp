// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_FINALIZE_SCENARIO_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_FINALIZE_SCENARIO_REQUEST_MESSAGE_H_

#include "message/command/music_command_request_message.h"

namespace music {

class FinalizeScenarioRequestMessage : public MusicCommandRequestMessage {
 public:
  // Constructor
  FinalizeScenarioRequestMessage() {
    type_ = MusicMessageType::kFinalizeScenarioRequest;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kFinalizeScenarioRequest)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const FinalizeScenarioRequestMessage &message);
  friend void from_json(const nlohmann::json &j,
                        FinalizeScenarioRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static FinalizeScenarioRequestMessage FromJson(const nlohmann::json &j) {
    FinalizeScenarioRequestMessage message;
    from_json(j, message);
    return message;
  }
  friend std::ostream &operator<<(
      std::ostream &os, const FinalizeScenarioRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "FinalizeScenarioRequestMessage: " + ToJson().dump();
  }

 protected:
  // No new fields needed for this message
};
// This class has no new members, but will still need its own builder. The
// fields contained in its super classes can be set through this builderand this
// class' default constructor will be called which will set its message type and
// command identifier.
class FinalizeScenarioRequestMessageBuilder
    : public CommandRequestMessageBuilder<
          FinalizeScenarioRequestMessageBuilder> {
 public:
  FinalizeScenarioRequestMessageBuilder() {
    finalize_scenario_message_ =
        std::make_shared<FinalizeScenarioRequestMessage>();
    SetBuiltMessage(finalize_scenario_message_);
  }
  FinalizeScenarioRequestMessage Build() {
    EnforceSystemToSystemEntities();
    return *finalize_scenario_message_;
  }

 protected:
  std::shared_ptr<FinalizeScenarioRequestMessage> finalize_scenario_message_;

  void SetBuiltMessage(
      std::shared_ptr<FinalizeScenarioRequestMessage> new_message) {
    finalize_scenario_message_ = new_message;
    CommandRequestMessageBuilder<
        FinalizeScenarioRequestMessageBuilder>::SetBuiltMessage(new_message);
  }

  void EnforceSystemToSystemEntities() {
    // enforce origin and receiver entity numbers to be 0
    auto new_origin_id = finalize_scenario_message_->GetOriginId();
    new_origin_id.SetEntityNumber(0);
    auto new_receiver_id = finalize_scenario_message_->GetReceiverId();
    new_receiver_id.SetEntityNumber(0);
    finalize_scenario_message_->SetOriginId(new_origin_id);
    finalize_scenario_message_->SetReceiverId(new_receiver_id);
  }
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_FINALIZE_SCENARIO_REQUEST_MESSAGE_H_