// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_REQUEST_MESSAGE_H_
#include <memory>
#include <string>

#include "message/entity_id.h"
#include "message/music_message.h"

namespace music {

class MusicRequestMessage : public MusicMessage {
  template <class T>
  friend class RequestMessageBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  static inline const std::string kOriginIdKey = "originID";
  static inline const std::string kReceiverIdKey = "receiverID";
  static inline const std::string kRequestIdKey = "requestID";

  // JSON serialization
  friend void to_json(nlohmann::json &j, const MusicRequestMessage &message);
  friend void from_json(const nlohmann::json &j, MusicRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static MusicRequestMessage FromJson(const nlohmann::json &j) {
    MusicRequestMessage message;
    from_json(j, message);
    return message;
  }

  // Getters/ Setters
  const EntityId &GetOriginId() const { return origin_id_; }
  const EntityId &GetReceiverId() const { return receiver_id_; }
  const long GetRequestId() const { return request_id_; }

  void SetOriginId(EntityId origin_id) { origin_id_ = origin_id; }
  void SetReceiverId(EntityId receiver_id) { receiver_id_ = receiver_id; }
  void SetRequestId(long request_id) { request_id_ = request_id; }

  friend std::ostream &operator<<(std::ostream &os,
                                  const MusicRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "MusicRequestMessage: " + ToJson().dump();
  }

 protected:
  EntityId origin_id_{};
  EntityId receiver_id_{};
  long request_id_{std::numeric_limits<long>::min()};
};

template <class T>
class RequestMessageBuilder : public MessageBuilder<T> {
 protected:
  std::shared_ptr<MusicRequestMessage> request_message_;
  void SetBuiltMessage(std::shared_ptr<MusicMessage> new_message) {
    request_message_ =
        std::static_pointer_cast<MusicRequestMessage>(new_message);
    MessageBuilder<T>::SetBuiltMessage(new_message);
  }

 public:
  T &SetOriginId(const EntityId &origin_id) {
    request_message_->origin_id_ = origin_id;
    return static_cast<T &>(*this);
  }
  T &SetReceiverId(const EntityId &receiver_id) {
    request_message_->receiver_id_ = receiver_id;
    return static_cast<T &>(*this);
  }
  T &SetRequestId(long request_id) {
    request_message_->request_id_ = request_id;
    return static_cast<T &>(*this);
  }
};

class MusicRequestMessageBuilder
    : public RequestMessageBuilder<MusicRequestMessageBuilder> {
 public:
  MusicRequestMessageBuilder() {
    request_message_ = std::make_shared<MusicRequestMessage>();
    SetBuiltMessage(request_message_);
  }

  MusicRequestMessage Build() const { return *request_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_REQUEST_MESSAGE_H_
