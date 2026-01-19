// Copyright (c) 2025 Discovery Machine®, Inc.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_PERCEPTION_PERCEPTION_DATA_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_PERCEPTION_PERCEPTION_DATA_MESSAGE_H_

#include "message/entity_id.h"
#include "message/music_message.h"
#include "message/perception/perception_record.h"
namespace music {

class PerceptionDataMessage : public MusicMessage {
  template <class T>
  friend class PerceptionDataMessageBuilderTemplate;

 public:
  // Constructor
  PerceptionDataMessage() : MusicMessage() {
    type_ = MusicMessageType::kPerception;
  }

  // Validate
  void Validate() const;

  // Getters
  const EntityId &GetOriginId() const { return origin_id_; }
  const int GetNumPerceptions() const { return num_perceptions_; }
  const std::vector<PerceptionRecord> &GetPerceptionRecords() const {
    return perception_records_;
  }

  // Setters
  void SetOriginId(const EntityId &origin_id) { origin_id_ = origin_id; }
  void SetNumPerceptions(int num_perceptions) {
    num_perceptions_ = num_perceptions;
  }
  void SetPerceptionRecords(
      const std::vector<PerceptionRecord> &perception_records) {
    perception_records_ = perception_records;
  }

  // JSON Keys
  inline static const std::string kOriginIdKey = "originID";
  inline static const std::string kNumPerceptionsKey = "numPerceptions";
  inline static const std::string kPerceptionRecordsKey = "perceptionRecords";

  // JSON serialization
  friend void to_json(nlohmann::json &j, const PerceptionDataMessage &message);
  friend void from_json(const nlohmann::json &j,
                        PerceptionDataMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static PerceptionDataMessage FromJson(const nlohmann::json &j) {
    PerceptionDataMessage message;
    from_json(j, message);
    return message;
  }

  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const PerceptionDataMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "PerceptionDataMessage: " + ToJson().dump();
  }

 protected:
  EntityId origin_id_{};
  int num_perceptions_{std::numeric_limits<int>::min()};
  std::vector<PerceptionRecord> perception_records_{};
};

// Template Builder Class
template <class T>
class PerceptionDataMessageBuilderTemplate : public MessageBuilder<T> {
 protected:
  std::shared_ptr<PerceptionDataMessage> perception_message_;
  void SetBuiltMessage(std::shared_ptr<MusicMessage> new_message) {
    perception_message_ =
        std::static_pointer_cast<PerceptionDataMessage>(new_message);
    MessageBuilder<T>::SetBuiltMessage(new_message);
  }

 public:
  T &SetOriginId(const EntityId &origin_id) {
    perception_message_->origin_id_ = origin_id;
    return static_cast<T &>(*this);
  }
  T &SetNumPerceptions(int num_perceptions) {
    perception_message_->num_perceptions_ = num_perceptions;
    return static_cast<T &>(*this);
  }
  T &SetPerceptionRecords(
      const std::vector<PerceptionRecord> &perception_records) {
    perception_message_->perception_records_ = perception_records;
    return static_cast<T &>(*this);
  }
};
class PerceptionDataMessageBuilder
    : public PerceptionDataMessageBuilderTemplate<
          PerceptionDataMessageBuilder> {
 public:
  PerceptionDataMessageBuilder() {
    SetBuiltMessage(std::make_shared<PerceptionDataMessage>());
  }

  PerceptionDataMessage Build() { return *perception_message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_PERCEPTION_PERCEPTION_DATA_MESSAGE_H_