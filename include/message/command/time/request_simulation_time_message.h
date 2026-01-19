// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_REQUEST_SIMULATION_TIME_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_REQUEST_SIMULATION_TIME_MESSAGE_H_

#include "message/command/music_command_request_message.h"

namespace music {

class RequestSimulationTimeMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class RequestSimulationTimeMessageBuilderTemplate;

 public:
  // Constructor
  RequestSimulationTimeMessage() {
    type_ = MusicMessageType::kRequestSimulationTime;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kRequestSimulationTime)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const RequestSimulationTimeMessage &message);
  friend void from_json(const nlohmann::json &j,
                        RequestSimulationTimeMessage &message);
  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static RequestSimulationTimeMessage FromJson(const nlohmann::json &j) {
    RequestSimulationTimeMessage message;
    from_json(j, message);
    return message;
  }

  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const RequestSimulationTimeMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "RequestSimulationTimeMessage: " + ToJson().dump();
  }

 protected:
  // No new fields needed for this message
};

// Template builder class
template <class T>
class RequestSimulationTimeMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 public:
  RequestSimulationTimeMessageBuilderTemplate() {
    request_sim_time_message_ =
        std::make_shared<RequestSimulationTimeMessage>();
    SetBuiltMessage(request_sim_time_message_);
  }

 protected:
  std::shared_ptr<RequestSimulationTimeMessage> request_sim_time_message_;
  void SetBuiltMessage(
      std::shared_ptr<RequestSimulationTimeMessage> new_message) {
    request_sim_time_message_ =
        std::static_pointer_cast<RequestSimulationTimeMessage>(new_message);
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete builder
class RequestSimulationTimeMessageBuilder
    : public RequestSimulationTimeMessageBuilderTemplate<
          RequestSimulationTimeMessageBuilder> {
 public:
  RequestSimulationTimeMessageBuilder()
      : RequestSimulationTimeMessageBuilderTemplate<
            RequestSimulationTimeMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<RequestSimulationTimeMessage>());
  }
  RequestSimulationTimeMessage Build() { return *request_sim_time_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_TIME_REQUEST_SIMULATION_TIME_MESSAGE_H_