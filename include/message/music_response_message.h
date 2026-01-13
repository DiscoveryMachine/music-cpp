// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_RESPONSE_MESSAGE_H_
#include "enum/request_status.h"
#include "message/entity_id.h"
#include "message/music_message.h"
namespace music {
class MusicResponseMessage : public MusicMessage {
  template <class T>
  friend class ResponseMessageBuilder;

 public:
  // Validate
  void Validate() const;

  // Getters
  const EntityId &GetOriginId() const { return origin_id_; }
  const EntityId &GetReceiverId() const { return receiver_id_; }
  long GetRequestId() const { return request_id_; }
  const RequestStatus GetRequestStatus() const { return request_status_; }

  // Setters
  void SetOriginId(const EntityId &origin_id) { origin_id_ = origin_id; }
  void SetReceiverId(const EntityId &receiver_id) {
    receiver_id_ = receiver_id;
  }
  void SetRequestId(long request_id) { request_id_ = request_id; }
  void SetRequestStatus(const RequestStatus &request_status) {
    request_status_ = request_status;
  }

  // Serialization Keys
  static inline const std::string kOriginIdKey = "originID";
  static inline const std::string kReceiverIdKey = "receiverID";
  static inline const std::string kRequestIdKey = "requestID";
  static inline const std::string kRequestStatusKey = "status";
  static inline const long kActionTypeId = 0L;

  // JSON serialization
  friend void to_json(nlohmann::json &j, const MusicResponseMessage &message);
  friend void from_json(const nlohmann::json &j, MusicResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static MusicResponseMessage FromJson(const nlohmann::json &j) {
    MusicResponseMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const MusicResponseMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "MusicResponseMessage: " + ToJson().dump();
  }

 protected:
  EntityId origin_id_{};
  EntityId receiver_id_{};
  long request_id_{std::numeric_limits<long>::min()};
  RequestStatus request_status_{RequestStatus::kUnset};
};
// Bulder class
template <class T>
class ResponseMessageBuilder : public MessageBuilder<T> {
 protected:
  std::shared_ptr<MusicResponseMessage> response_message_;
  void SetBuiltMessage(std::shared_ptr<MusicResponseMessage> new_message) {
    response_message_ = new_message;
    MessageBuilder<T>::SetBuiltMessage(new_message);
  }

 public:
  T &SetOriginId(const EntityId &origin_id) {
    response_message_->origin_id_ = origin_id;
    return static_cast<T &>(*this);
  }
  T &SetReceiverId(const EntityId &receiver_id) {
    response_message_->receiver_id_ = receiver_id;
    return static_cast<T &>(*this);
  }
  T &SetRequestId(long request_id) {
    response_message_->request_id_ = request_id;
    return static_cast<T &>(*this);
  }
  T &SetRequestStatus(const RequestStatus &request_status) {
    response_message_->request_status_ = request_status;
    return static_cast<T &>(*this);
  }
};
class MusicResponseMessageBuilder
    : public ResponseMessageBuilder<MusicResponseMessageBuilder> {
 public:
  MusicResponseMessageBuilder() {
    response_message_ = std::make_shared<MusicResponseMessage>();
    SetBuiltMessage(response_message_);
  }

  MusicResponseMessage Build() const { return *response_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUSIC_RESPONSE_MESSAGE_H_