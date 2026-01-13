
// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_DIS7_MESSAGE_SERIALIZER_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_DIS7_MESSAGE_SERIALIZER_H_

#include <dis7/ActionRequestPdu.h>
#include <dis7/ActionResponsePdu.h>
#include <dis7/CollisionPdu.h>
#include <dis7/DetonationPdu.h>
#include <dis7/FirePdu.h>
#include <dis7/Pdu.h>
#include <dis7/SetDataPdu.h>

#include <memory>

#include "message/collision/collision_message.h"
#include "message/command/constructcontrol/remove_construct_message.h"
#include "message/command/constructcontrol/stop_construct_message.h"
#include "message/command/entity/transfer_construct_id_message.h"
#include "message/command/music_command_request_message.h"
#include "message/command/music_command_response_message.h"
#include "message/command/playercontrol/control_initiated_message.h"
#include "message/command/playercontrol/control_released_message.h"
#include "message/command/scenario/create_construct_request_message.h"
#include "message/command/scenario/create_construct_response_message.h"
#include "message/command/scenario/create_environment_request_message.h"
#include "message/command/scenario/parameterize_construct_request_message.h"
#include "message/command/scenario/parameterize_construct_response_message.h"
#include "message/command/scenario/scenario_start_request_message.h"
#include "message/command/simulation/display_messages_message.h"
#include "message/command/time/request_simulation_time_message.h"
#include "message/command/time/set_simulation_time_message.h"
#include "message/command/time/simulation_time_message.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/state_field_message.h"
#include "message/control/construct_control_request_message.h"
#include "message/control/construct_control_response_message.h"
#include "message/control/construct_primary_control_request_message.h"
#include "message/control/control_granted_message.h"
#include "message/control/control_lost_message.h"
#include "message/control/control_reclamation_message.h"
#include "message/control/control_regained_message.h"
#include "message/control/control_relinquished_message.h"
#include "message/control/control_transfer_granted_message.h"
#include "message/control/control_transfer_request_message.h"
#include "message/control/primary_control_granted_message.h"
#include "message/control/primary_control_relinquished_message.h"
#include "message/control/set_current_controller_message.h"
#include "message/event/event_message.h"
#include "message/interaction/construct_interaction_request_message.h"
#include "message/interaction/construct_interaction_response_message.h"
#include "message/munition/detonation_message.h"
#include "message/munition/fire_message.h"
#include "message/music_request_message.h"
#include "message/music_response_message.h"
#include "message/perception/perception_data_message.h"
#include "message/waypoint/waypoint_data_message.h"
#include "serialize/music_message_serializer.h"
#include "util/music_byte_buffer.h"

namespace music {
class Dis7MessageSerializer : public MUSICMessageSerializer<MusicByteBuffer> {
 public:
  static std::weak_ptr<Dis7MessageSerializer> GetInstance() {
    if (_instance == nullptr) {
      _instance = std::make_shared<Dis7MessageSerializer>();
    }
    return _instance;
  }

  // clang-format off
  MusicByteBuffer SerializeMusicMessage(const MusicMessage &message) override;
  // clang-format on

 private:
  static std::shared_ptr<Dis7MessageSerializer> _instance;
  // clang-format off

  MusicByteBuffer SerializeMusicRequestMessage(const MusicRequestMessage &message);
  MusicByteBuffer SerializeMusicResponseMessage(const MusicResponseMessage &message);

  // collision
  MusicByteBuffer SerializeCollisionMessage(const CollisionMessage &message);

  // command
  void SerializeMusicCommandRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const MusicCommandRequestMessage &message);
  void SerializeMusicCommandResponseMessage(DIS::ActionResponsePdu &action_response_pdu, const MusicCommandResponseMessage &message);

  // command/constructcontrol
  void SerializeRemoveConstructMessage(DIS::ActionRequestPdu &action_request_pdu, const RemoveConstructMessage &message);  
  void SerializeStopConstructMessage(DIS::ActionRequestPdu &action_request_pdu, const StopConstructMessage &message);

  // command/entity
  void SerializeTransferConstructIdMessage(DIS::ActionRequestPdu &action_request_pdu, const TransferConstructIdMessage &message);

  // command/playercontrol
  void SerializeControlInitiatedMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlInitiatedMessage &message);
  void SerializeControlReleasedMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlReleasedMessage &message);

  // command/scenario
  void SerializeCreateConstructRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const CreateConstructRequestMessage &message);
  void SerializeCreateConstructResponseMessage(DIS::ActionResponsePdu &action_response_pdu, const CreateConstructResponseMessage &message);
  void SerializeCreateEnvironmentRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const CreateEnvironmentRequestMessage &message);
  void SerializeParameterizeConstructRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const ParameterizeConstructRequestMessage &message);
  void SerializeParameterizeConstructResponseMessage(DIS::ActionResponsePdu &action_response_pdu, const ParameterizeConstructResponseMessage &message);
  void SerializeScenarioStartRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const ScenarioStartRequestMessage &message);

  // command/simulation
  void SerializeDisplayMessageMessage(DIS::ActionRequestPdu &action_request_pdu, const DisplayMessagesMessage &message);

  // command/time
  void SerializeRequestSimulationTimeMessage(DIS::ActionRequestPdu &action_request_pdu, const RequestSimulationTimeMessage &message);
  void SerializeSetSimulationTimeMessage(DIS::ActionRequestPdu &action_request_pdu, const SetSimulationTimeMessage &message);
  void SerializeSimulationTimeMessage(DIS::ActionResponsePdu &action_response_pdu, const SimulationTimeMessage &message);

  // control
  void SerializeConstructControlRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const ConstructControlRequestMessage &message);
  void SerializeConstructPrimaryControlRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const ConstructPrimaryControlRequestMessage &message);
  void SerializeConstructPrimaryControlResponseMessage(DIS::ActionResponsePdu &action_response_pdu, const ConstructControlResponseMessage &message);
  void SerializeControlGrantedMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlGrantedMessage &message);
  void SerializeControlLostMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlLostMessage &message);
  void SerializeControlReclamationMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlReclamationMessage &message);
  void SerializeControlRegainedMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlRegainedMessage &message);
  void SerializeControlRelinquishedMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlRelinquishedMessage &message);
  void SerializeControlTransferGrantedMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlTransferGrantedMessage &message);
  void SerializeControlTransferRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const ControlTransferRequestMessage &message);
  void SerializePrimaryControlGrantedMessage(DIS::ActionRequestPdu &action_request_pdu, const PrimaryControlGrantedMessage &message);
  void SerializePrimaryControlRelinquishedMessage(DIS::ActionRequestPdu &action_request_pdu, const PrimaryControlRelinquishedMessage &message);
  void SerializeSetCurrentControllerMessage(DIS::ActionRequestPdu &action_request_pdu, const SetCurrentControllerMessage &message);

  // event
  MusicByteBuffer SerializeEventMessage(const EventMessage &message);

  // interaction
  void SerializeConstructInteractionRequestMessage(DIS::ActionRequestPdu &action_request_pdu, const ConstructInteractionRequestMessage &message);
  void SerializeConstructInteractionResponseMessage(DIS::ActionResponsePdu &action_response_pdu, const ConstructInteractionResponseMessage &message);

  // munition (detonation/fire)
  MusicByteBuffer SerializeMunitionMessage(const MunitionMessage &message);
  MusicByteBuffer SerializeDetonationMessage(const DetonationMessage &message);
  MusicByteBuffer SerializeFireMessage(const FireMessage &message);

  // -- custom PDUs -- 
  // construct data
  MusicByteBuffer SerializeConstructDataMessage(const ConstructDataMessage &message);

  // state field
  MusicByteBuffer SerializeStateFieldMessage(const StateFieldMessage &message);

  // perception
  MusicByteBuffer SerializePerceptionDataMessage(const PerceptionDataMessage &message);

  // waypoint
  MusicByteBuffer SerializeWaypointDataMessage(const WaypointDataMessage &message);

  // clang-format on
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_SERIALIZE_DIS7_MESSAGE_SERIALIZER_H_