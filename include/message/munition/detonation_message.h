// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUNITION_DETONATION_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUNITION_DETONATION_MESSAGE_H_

#include <nlohmann/json_fwd.hpp>

#include "enum/detonation_result_type.h"
#include "message/munition/munition_message.h"

namespace music {

class DetonationMessage : public MunitionMessage {
  template <class T>
  friend class DetonationMessageBuilderTemplate;

 public:
  DetonationMessage() : MunitionMessage() {
    type_ = MusicMessageType::kDetonation;
  }

  // Validate
  void Validate() const;

  // Getters
  const DetonationResultType GetResult() const { return result_; }

  // Setters
  void SetResult(DetonationResultType result) { result_ = result; }

  // JSON Keys
  inline static const std::string kResultKey = "result";

  // JSON Serialization
  friend void to_json(nlohmann::json &j, const DetonationMessage &msg);

  friend void from_json(const nlohmann::json &j, DetonationMessage &msg);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static DetonationMessage FromJson(const nlohmann::json &j) {
    DetonationMessage msg;
    from_json(j, msg);
    return msg;
  }

  // String Representation
  std::string ToString() { return "DetonationMessage: " + ToJson().dump(); }

  // << override
  friend std::ostream &operator<<(std::ostream &os,
                                  const DetonationMessage &msg) {
    os << msg.ToJson().dump();
    return os;
  }

 protected:
  // An enumerated value that indicates the result of the detonation.
  DetonationResultType result_{DetonationResultType::kUnset};
};

// Template builder class
template <class T>
class DetonationMessageBuilderTemplate
    : public MunitionMessageBuilderTemplate<T> {
 protected:
  std::shared_ptr<DetonationMessage> detonation_message_;
  void SetBuiltMessage(std::shared_ptr<MusicMessage> new_message) {
    detonation_message_ =
        std::static_pointer_cast<DetonationMessage>(new_message);
    MunitionMessageBuilderTemplate<T>::SetBuiltMessage(new_message);
  }

 public:
  T &SetResult(DetonationResultType result) {
    detonation_message_->result_ = result;
    return static_cast<T &>(*this);
  }
};

class DetonationMessageBuilder
    : public DetonationMessageBuilderTemplate<DetonationMessageBuilder> {
 public:
  DetonationMessageBuilder()
      : DetonationMessageBuilderTemplate<DetonationMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<DetonationMessage>());
  }
  DetonationMessage Build() { return *detonation_message_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_MUNITION_DETONATION_MESSAGE_H_