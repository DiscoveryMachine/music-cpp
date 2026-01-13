// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_DIS7_MESSAGE_DESERIALIZER_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_DIS7_MESSAGE_DESERIALIZER_H_

#include <dis7/ActionRequestPdu.h>
#include <dis7/ActionResponsePdu.h>
#include <dis7/CollisionPdu.h>
#include <dis7/DetonationPdu.h>
#include <dis7/FirePdu.h>
#include <dis7/Pdu.h>
#include <dis7/SetDataPdu.h>

#include <memory>

#include "message/collision/collision_message.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/state_field_message.h"
#include "message/event/event_message.h"
#include "message/interaction/construct_interaction_response_message.h"
#include "message/munition/detonation_message.h"
#include "message/munition/fire_message.h"
#include "message/music_request_message.h"
#include "message/music_response_message.h"
#include "message/perception/perception_data_message.h"
#include "message/waypoint/waypoint_data_message.h"
#include "serialize/music_message_deserializer.h"
#include "util/music_byte_buffer.h"

namespace music {

class Dis7MessageDeserializer
    : public MUSICMessageDeserializer<MusicByteBuffer> {
 public:
  static std::weak_ptr<Dis7MessageDeserializer> GetInstance() {
    if (_instance == nullptr) {
      _instance = std::make_shared<Dis7MessageDeserializer>();
    }
    return _instance;
  }

  // clang-format off
  std::shared_ptr<MusicMessage> DeserializeMusicMessage(MusicByteBuffer &buffer) override;
  // clang-format on

 private:
  static std::shared_ptr<Dis7MessageDeserializer> _instance;

  // clang-format off
  std::shared_ptr<MusicRequestMessage> DeserializeActionRequestPdu(const DIS::ActionRequestPdu &pdu);
  std::shared_ptr<MusicResponseMessage> DeserializeActionResponsePdu(const DIS::ActionResponsePdu &pdu, MusicMessageType type);
  std::shared_ptr<EventMessage> DeserializeSetDataPdu(const DIS::SetDataPdu &pdu);  
  std::shared_ptr<CollisionMessage> DeserializeCollisionPdu(const DIS::CollisionPdu &pdu);
  std::shared_ptr<FireMessage> DeserializeFirePdu(const DIS::FirePdu &pdu);
  std::shared_ptr<DetonationMessage> DeserializeDetonationPdu(const DIS::DetonationPdu &pdu);
  std::shared_ptr<ConstructInteractionResponseMessage> DeserializeConstructInteractionResponseMessage(const DIS::ActionResponsePdu &pdu, std::shared_ptr<MusicResponseMessage> base_message);

  std::shared_ptr<ConstructDataMessage> DeserializeConstructDataMessage(MusicByteBuffer &data_buffer);
  std::shared_ptr<StateFieldMessage> DeserializeStateFieldMessage(MusicByteBuffer &data_buffer);
  std::shared_ptr<PerceptionDataMessage> DeserializePerceptionDataMessage(MusicByteBuffer &data_buffer);
  std::shared_ptr<WaypointDataMessage> DeserializeWaypointDataMessage(MusicByteBuffer &data_buffer);
  // clang-format on
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_DIS7_MESSAGE_DESERIALIZER_H_