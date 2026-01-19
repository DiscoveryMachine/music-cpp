// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_FINALIZE_SCENARIO_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_FINALIZE_SCENARIO_RESPONSE_MESSAGE_H_

#include "message/command/music_command_response_message.h"
namespace music {

class FinalizeScenarioResponseMessage : public MusicCommandResponseMessage {
 public:
  // Constructor
  FinalizeScenarioResponseMessage() {
    type_ = MusicMessageType::kFinalizeScenarioResponse;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kFinalizeScenarioResponse)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const FinalizeScenarioResponseMessage &message);
  friend void from_json(const nlohmann::json &j,
                        FinalizeScenarioResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static FinalizeScenarioResponseMessage FromJson(const nlohmann::json &j) {
    FinalizeScenarioResponseMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(
      std::ostream &os, const FinalizeScenarioResponseMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "FinalizeScenarioResponseMessage: " + ToJson().dump();
  }

 protected:
  // No new fields needed for this message
};

// This class has no new members, but will still need its own builder. The
// fields contained in its super classes can be set through this builder and
// this class' default constructor will be called which will set its message
// type and command identifier.
class FinalizeScenarioResponseMessageBuilder
    : public CommandResponseMessageBuilder<
          FinalizeScenarioResponseMessageBuilder> {
 public:
  FinalizeScenarioResponseMessageBuilder() {
    finalize_scenario_message_ =
        std::make_shared<FinalizeScenarioResponseMessage>();
    SetBuiltMessage(finalize_scenario_message_);
  }
  FinalizeScenarioResponseMessage Build() {
    EnforceSystemToSystemEntities();
    return *finalize_scenario_message_;
  }

 protected:
  std::shared_ptr<FinalizeScenarioResponseMessage> finalize_scenario_message_;

  void SetBuiltMessage(
      std::shared_ptr<FinalizeScenarioResponseMessage> new_message) {
    finalize_scenario_message_ = new_message;
    CommandResponseMessageBuilder<
        FinalizeScenarioResponseMessageBuilder>::SetBuiltMessage(new_message);
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
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_FINALIZE_SCENARIO_RESPONSE_MESSAGE_H_