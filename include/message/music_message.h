// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_MESSAGE_H_

#include <memory>
#include <nlohmann/json.hpp>

#include "enum/music_message_type.h"
#include "music_message_header.h"

namespace music {

class MusicMessage {
  template <class T>
  friend class MessageBuilder;

 public:
  // Virtual destructor needed to use dynamic_cast for derived classes
  virtual ~MusicMessage() = default;

  // Validate
  void Validate() const;

  // Getters
  const MusicMessageHeader &GetHeader() const { return header_; }
  const MusicMessageType GetType() const { return type_; }

  // Setters
  void SetHeader(MusicMessageHeader header) { header_ = header; }
  void SetType(MusicMessageType type) { type_ = type; }

  // JSON Keys
  inline static const std::string kHeaderJsonKey = "header";
  inline static const std::string kTypeJsonKey = "type";

  // JSON serialization
  friend void to_json(nlohmann::json &j, const MusicMessage &message);
  friend void from_json(const nlohmann::json &j, MusicMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static MusicMessage FromJson(const nlohmann::json &j) {
    MusicMessage message;
    from_json(j, message);
    return message;
  }

  // Stream output and ToString
  friend std::ostream &operator<<(std::ostream &os,
                                  const MusicMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const { return "MusicMessage: " + ToJson().dump(); }

 protected:
  MusicMessageHeader header_{};
  MusicMessageType type_{MusicMessageType::kUnset};
};
// Builder class
// Template for inheritance
template <class T>
class MessageBuilder {
 protected:
  std::shared_ptr<MusicMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicMessage> new_message) {
    message_ = new_message;
  }

 public:
  T &SetHeader(MusicMessageHeader header) {
    message_->header_ = header;
    return static_cast<T &>(*this);
  }

  T &SetType(MusicMessageType type) {
    message_->type_ = type;
    return static_cast<T &>(*this);
  }
};
// Concrete builder
class MusicMessageBuilder : public MessageBuilder<MusicMessageBuilder> {
 public:
  MusicMessageBuilder() {
    message_ = std::make_shared<MusicMessage>();
    SetBuiltMessage(message_);
  }
  MusicMessage Build() const { return *message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_MESSAGE_H_