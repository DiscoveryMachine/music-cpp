// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_SIMULATION_TIME_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_SIMULATION_TIME_MESSAGE_H_

#include "message/command/music_command_response_message.h"

namespace music {

class SimulationTimeMessage : public MusicCommandResponseMessage {
  template <class T>
  friend class SimulationTimeMessageBuilderTemplate;

 public:
  // Constructor
  SimulationTimeMessage() {
    type_ = MusicMessageType::kSimulationTime;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kSimulationTime)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // Serialization Keys
  inline static const std::string kSimTimeKey = "simTime";
  inline static const long kSimTimeId = 1L;

  // Getters
  const long GetSimTime() const { return sim_time_; }

  // Setters
  void SetSimTime(long sim_time) { sim_time_ = sim_time; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const SimulationTimeMessage &message);
  friend void from_json(const nlohmann::json &j,
                        SimulationTimeMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static SimulationTimeMessage FromJson(const nlohmann::json &j) {
    SimulationTimeMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const SimulationTimeMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "SimulationTimeMessage: " + ToJson().dump();
  }

 protected:
  // The simulation's world time in seconds since epoch.
  long sim_time_{std::numeric_limits<long>::min()};
};

// Template builder class
template <class T>
class SimulationTimeMessageBuilderTemplate
    : public CommandResponseMessageBuilder<T> {
 public:
  SimulationTimeMessageBuilderTemplate() {
    sim_time_message_ = std::make_shared<SimulationTimeMessage>();
    SetBuiltMessage(sim_time_message_);
  }
  T &SetSimTime(long sim_time) {
    sim_time_message_->sim_time_ = sim_time;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<SimulationTimeMessage> sim_time_message_;
  void SetBuiltMessage(
      std::shared_ptr<MusicCommandResponseMessage> new_message) {
    sim_time_message_ =
        std::static_pointer_cast<SimulationTimeMessage>(new_message);
    CommandResponseMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete builder
class SimulationTimeMessageBuilder
    : public SimulationTimeMessageBuilderTemplate<
          SimulationTimeMessageBuilder> {
 public:
  SimulationTimeMessageBuilder()
      : SimulationTimeMessageBuilderTemplate<SimulationTimeMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<SimulationTimeMessage>());
  }
  SimulationTimeMessage Build() { return *sim_time_message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_SIMULATION_TIME_MESSAGE_H_