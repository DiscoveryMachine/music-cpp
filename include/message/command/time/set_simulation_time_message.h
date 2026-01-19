// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_SET_SIMULATION_TIME_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_SET_SIMULATION_TIME_MESSAGE_H_

#include <limits>

#include "message/command/music_command_request_message.h"
namespace music {

class SetSimulationTimeMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class SetSimulationTimeMessageBuilderTemplate;

 public:
  // Constructor
  SetSimulationTimeMessage() {
    type_ = MusicMessageType::kSetSimulationTime;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kSetSimulationTime)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // Serialization Keys
  inline static const std::string kNewTimeKey = "newTime";
  inline static const long kNewTimeId = 1L;

  // Getters
  const long GetNewTime() const { return new_time_; }

  // Setters
  void SetNewTime(long new_time) { new_time_ = new_time; }

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const SetSimulationTimeMessage &message);
  friend void from_json(const nlohmann::json &j,
                        SetSimulationTimeMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static SetSimulationTimeMessage FromJson(const nlohmann::json &j) {
    SetSimulationTimeMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const SetSimulationTimeMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "SetSimulationTimeMessage: " + ToJson().dump();
  }

 protected:
  // The time to set the receiving simulation's world time to in seconds since
  // epoch
  long new_time_{std::numeric_limits<long>::min()};
};
// Template builder class
template <class T>
class SetSimulationTimeMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 public:
  T &SetNewTime(long new_time) {
    set_sim_time_message_->new_time_ = new_time;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<SetSimulationTimeMessage> set_sim_time_message_;
  void SetBuiltMessage(std::shared_ptr<SetSimulationTimeMessage> new_message) {
    set_sim_time_message_ =
        std::static_pointer_cast<SetSimulationTimeMessage>(new_message);
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete builder
class SetSimulationTimeMessageBuilder
    : public SetSimulationTimeMessageBuilderTemplate<
          SetSimulationTimeMessageBuilder> {
 public:
  SetSimulationTimeMessageBuilder()
      : SetSimulationTimeMessageBuilderTemplate<
            SetSimulationTimeMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<SetSimulationTimeMessage>());
  }
  SetSimulationTimeMessage Build() { return *set_sim_time_message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_SET_SIMULATION_TIME_MESSAGE_H_