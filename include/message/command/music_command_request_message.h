// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_MUSIC_COMMAND_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_MUSIC_COMMAND_REQUEST_MESSAGE_H_

#include "message/music_request_message.h"

namespace music {
class MusicCommandRequestMessage : public MusicRequestMessage {
  template <class T>
  friend class CommandRequestMessageBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kCommandIdKey = "commandIdentifier";

  // Getters
  const long GetCommandIdentifier() const { return command_identifier_; }

  // Setters
  void SetCommandIdentifier(long command_identifier) {
    command_identifier_ = command_identifier;
  }

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const MusicCommandRequestMessage &message);
  friend void from_json(const nlohmann::json &j,
                        MusicCommandRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static MusicCommandRequestMessage FromJson(const nlohmann::json &j) {
    MusicCommandRequestMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const MusicCommandRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "MusicCommandRequestMessage: " + ToJson().dump();
  }

 protected:
  // The identifier for the MUSIC® Command so receiving entities can tell
  // which command they are receiving. Unsigned 32-bit integer
  long command_identifier_{std::numeric_limits<long>::min()};
};

// Builder
template <class T>
class CommandRequestMessageBuilder : public RequestMessageBuilder<T> {
 public:
  T &SetCommandIdentifier(long command_identifier) {
    command_request_message_->command_identifier_ = command_identifier;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<MusicCommandRequestMessage> command_request_message_;
  void SetBuiltMessage(
      std::shared_ptr<MusicCommandRequestMessage> new_message) {
    command_request_message_ = new_message;
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

class MusicCommandRequestMessageBuilder
    : public CommandRequestMessageBuilder<MusicCommandRequestMessageBuilder> {
 public:
  MusicCommandRequestMessageBuilder() {
    command_request_message_ = std::make_shared<MusicCommandRequestMessage>();
    SetBuiltMessage(command_request_message_);
  }
  MusicCommandRequestMessage Build() const { return *command_request_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_MUSIC_COMMAND_REQUEST_MESSAGE_H_