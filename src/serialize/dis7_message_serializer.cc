// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <dis7/ActionRequestPdu.h>
#include <dis7/CollisionPdu.h>
#include <dis7/DataQueryPdu.h>
#include <dis7/DetonationPdu.h>
#include <dis7/EntityID.h>
#include <dis7/EntityType.h>
#include <dis7/EulerAngles.h>
#include <dis7/EventIdentifier.h>
#include <dis7/FirePdu.h>
#include <dis7/FixedDatum.h>
#include <dis7/MunitionDescriptor.h>
#include <dis7/Pdu.h>
#include <dis7/PduSuperclass.h>
#include <dis7/SimulationAddress.h>
#include <dis7/VariableDatum.h>
#include <dis7/Vector3Double.h>
#include <dis7/Vector3Float.h>
#include <dis7/utils/DataStream.h>
#include <dis7/utils/Endian.h>
#include <sys/types.h>

#include <cstdint>
#include <cstring>
#include <vector>

#include "dis7/SetDataPdu.h"
#include "enum/construct_render_type.h"
#include "message/command/music_command_request_message.h"
#include "message/command/playercontrol/control_initiated_message.h"
#include "message/command/playercontrol/control_released_message.h"
#include "message/command/scenario/create_construct_request_message.h"
#include "message/command/scenario/create_construct_response_message.h"
#include "message/command/scenario/create_environment_request_message.h"
#include "message/command/scenario/create_environment_response_message.h"
#include "message/command/scenario/finalize_scenario_request_message.h"
#include "message/command/scenario/finalize_scenario_response_message.h"
#include "message/command/scenario/parameterize_construct_request_message.h"
#include "message/command/simulation/display_messages_message.h"
#include "message/command/time/request_simulation_time_message.h"
#include "message/command/time/set_simulation_time_message.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/construct_information_record.h"
#include "message/control/construct_control_request_message.h"
#include "message/control/construct_control_response_message.h"
#include "message/control/construct_primary_control_request_message.h"
#include "message/control/construct_primary_control_response_message.h"
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
#include "message/interaction/construct_interaction_request_message.h"
#include "message/interaction/construct_interaction_response_message.h"
#include "message/music_message.h"
#include "message/perception/perception_data_message.h"
#include "message/waypoint/waypoint_data_message.h"
#include "serialize/dis7_message_serializer.h"
#include "util/dis_conversion_utils.h"
#include "util/music_byte_buffer.h"
#include "util/validate.h"

namespace music {

MusicByteBuffer Dis7MessageSerializer::SerializeMusicMessage(
    const MusicMessage &message) {
  const MusicRequestMessage *request_message =
      dynamic_cast<const MusicRequestMessage *>(&message);
  if (request_message != nullptr) {
    return SerializeMusicRequestMessage(*request_message);
  }

  const MusicResponseMessage *response_message =
      dynamic_cast<const MusicResponseMessage *>(&message);
  if (response_message != nullptr) {
    return SerializeMusicResponseMessage(*response_message);
  }

  const EventMessage *event_message =
      dynamic_cast<const EventMessage *>(&message);
  if (event_message != nullptr) {
    return SerializeEventMessage(*event_message);
  }

  const CollisionMessage *collision_message =
      dynamic_cast<const CollisionMessage *>(&message);
  if (collision_message != nullptr) {
    return SerializeCollisionMessage(*collision_message);
  }

  const MunitionMessage *munition_message =
      dynamic_cast<const MunitionMessage *>(&message);
  if (munition_message != nullptr) {
    return SerializeMunitionMessage(*munition_message);
  }

  const ConstructDataMessage *construct_data_message =
      dynamic_cast<const ConstructDataMessage *>(&message);
  if (construct_data_message != nullptr) {
    return SerializeConstructDataMessage(*construct_data_message);
  }

  const StateFieldMessage *state_field_message =
      dynamic_cast<const StateFieldMessage *>(&message);
  if (state_field_message != nullptr) {
    return SerializeStateFieldMessage(*state_field_message);
  }

  const PerceptionDataMessage *perception_data_message =
      dynamic_cast<const PerceptionDataMessage *>(&message);
  if (perception_data_message != nullptr) {
    return SerializePerceptionDataMessage(*perception_data_message);
  }

  const WaypointDataMessage *waypoint_data_message =
      dynamic_cast<const WaypointDataMessage *>(&message);
  if (waypoint_data_message != nullptr) {
    return SerializeWaypointDataMessage(*waypoint_data_message);
  }

  LogError(
      "Dis7MessageSerializer::SerializeMusicMessage: "
      "Unsupported MusicMessage type for DIS7 serialization.");

  return MusicByteBuffer();
}
MusicByteBuffer Dis7MessageSerializer::SerializeMusicRequestMessage(
    const MusicRequestMessage &message) {
  // Implementation of MusicRequestMessage serialization

  // Check message type and call appropriate serialization method

  // All subtypes of MusicRequestMessage will be an action request pdu
  DIS::ActionRequestPdu action_request_pdu = DIS::ActionRequestPdu();
  SetPDUBaseData(action_request_pdu, message);

  // Deserialization uses (pduType, actionID) to infer MusicMessageType.
  // Command messages may override this later, but non-command request
  // messages (e.g., ConstructInteractionRequest) still require it.
  action_request_pdu.setActionID(static_cast<unsigned int>(
      MusicMessageTypeHelper::GetInfoForType(message.GetType()).action_type));

  // Add originID, receiverID, and requestID fields, common to each request
  // message
  action_request_pdu.setOriginatingEntityID(
      ToDisEntityId(message.GetOriginId()));
  action_request_pdu.setReceivingEntityID(
      ToDisEntityId(message.GetReceiverId()));
  action_request_pdu.setRequestID(
      static_cast<unsigned int>(message.GetRequestId()));

  // Set Fixed and Variable Datums to empty vectors for now; specific request
  // messages will replace as needed
  action_request_pdu.setFixedDatums(std::vector<DIS::FixedDatum>());
  action_request_pdu.setVariableDatums(std::vector<DIS::VariableDatum>());

  const MusicCommandRequestMessage *command_request_message =
      dynamic_cast<const MusicCommandRequestMessage *>(&message);
  if (command_request_message != nullptr) {
    command_request_message->Validate();
    SerializeMusicCommandRequestMessage(action_request_pdu,
                                        *command_request_message);
  }

  const ConstructInteractionRequestMessage *interaction_request_message =
      dynamic_cast<const ConstructInteractionRequestMessage *>(&message);
  if (interaction_request_message != nullptr) {
    interaction_request_message->Validate();
    SerializeConstructInteractionRequestMessage(action_request_pdu,
                                                *interaction_request_message);
  }

  // check for control request messages
  const ConstructControlRequestMessage *construct_control_request_message =
      dynamic_cast<const ConstructControlRequestMessage *>(&message);
  if (construct_control_request_message != nullptr) {
    construct_control_request_message->Validate();
    SerializeConstructControlRequestMessage(action_request_pdu,
                                            *construct_control_request_message);
  }

  const ConstructPrimaryControlRequestMessage
      *construct_primary_control_request_message =
          dynamic_cast<const ConstructPrimaryControlRequestMessage *>(&message);
  if (construct_primary_control_request_message != nullptr) {
    construct_primary_control_request_message->Validate();
    // No implementation needed; ConstructPrimaryControlRequestMessage has no
    // fixed/variable datum fields
  }

  const ControlGrantedMessage *control_granted_message =
      dynamic_cast<const ControlGrantedMessage *>(&message);
  if (control_granted_message != nullptr) {
    control_granted_message->Validate();
    // No implementation needed.
  }

  const ControlLostMessage *control_lost_message =
      dynamic_cast<const ControlLostMessage *>(&message);
  if (control_lost_message != nullptr) {
    control_lost_message->Validate();
    // No implementation needed.
  }

  const ControlReclamationMessage *control_reclamation_message =
      dynamic_cast<const ControlReclamationMessage *>(&message);
  if (control_reclamation_message != nullptr) {
    control_reclamation_message->Validate();
    // No implementation needed.
  }

  const ControlRegainedMessage *control_regained_message =
      dynamic_cast<const ControlRegainedMessage *>(&message);
  if (control_regained_message != nullptr) {
    control_regained_message->Validate();
    // No implementation needed.
  }

  const ControlRelinquishedMessage *control_relinquished_message =
      dynamic_cast<const ControlRelinquishedMessage *>(&message);
  if (control_relinquished_message != nullptr) {
    control_relinquished_message->Validate();
    // No implementation needed.
  }

  const ControlTransferGrantedMessage *control_transfer_granted_message =
      dynamic_cast<const ControlTransferGrantedMessage *>(&message);
  if (control_transfer_granted_message != nullptr) {
    control_transfer_granted_message->Validate();
    // No implementation needed.
  }

  const ControlTransferRequestMessage *control_transfer_request_message =
      dynamic_cast<const ControlTransferRequestMessage *>(&message);
  if (control_transfer_request_message != nullptr) {
    control_transfer_request_message->Validate();
    SerializeControlTransferRequestMessage(action_request_pdu,
                                           *control_transfer_request_message);
  }

  const PrimaryControlGrantedMessage *primary_control_granted_message =
      dynamic_cast<const PrimaryControlGrantedMessage *>(&message);
  if (primary_control_granted_message != nullptr) {
    primary_control_granted_message->Validate();
    // No implementation needed.
  }

  const PrimaryControlRelinquishedMessage
      *primary_control_relinquished_message =
          dynamic_cast<const PrimaryControlRelinquishedMessage *>(&message);
  if (primary_control_relinquished_message != nullptr) {
    primary_control_relinquished_message->Validate();
    // No implementation needed.
  }

  const SetCurrentControllerMessage *set_current_controller_message =
      dynamic_cast<const SetCurrentControllerMessage *>(&message);
  if (set_current_controller_message != nullptr) {
    set_current_controller_message->Validate();
    SerializeSetCurrentControllerMessage(action_request_pdu,
                                         *set_current_controller_message);
  }

  return MarshalPduToBuffer(action_request_pdu);
}

void Dis7MessageSerializer::SerializeCreateConstructRequestMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const CreateConstructRequestMessage &message) {
  // Number Fixed Datums will always be 0 for Create Construct Request, as they
  // are not used.

  // Variable Datums (will be 2 or 4 depending on whether location + orientation
  // are included)
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();

  // Construct type
  DIS::VariableDatum construct_type_datum = DIS::VariableDatum();
  construct_type_datum.setVariableDatumID(
      CreateConstructRequestMessage::kConstructTypeId);
  {
    std::vector<char> bytes = Get8BytePaddedString(message.GetConstructType());
    construct_type_datum.setVariableDatumLength(
        static_cast<unsigned int>(bytes.size() * 8));
    construct_type_datum.setVariableDatums(bytes.data(), bytes.size());
  }
  variable_datums.push_back(construct_type_datum);

  // Construct callsign
  DIS::VariableDatum construct_callsign_datum = DIS::VariableDatum();
  construct_callsign_datum.setVariableDatumID(
      CreateConstructRequestMessage::kCallsignId);
  {
    std::vector<char> bytes =
        Get8BytePaddedString(message.GetConstructCallsign());
    construct_callsign_datum.setVariableDatumLength(
        static_cast<unsigned int>(bytes.size() * 8));
    construct_callsign_datum.setVariableDatums(bytes.data(), bytes.size());
  }
  variable_datums.push_back(construct_callsign_datum);

  // Construct location
  if (message.GetConstructLocation().has_value()) {
    DIS::VariableDatum location_datum = DIS::VariableDatum();

    AddLocationToVariableDatum(location_datum,
                               message.GetConstructLocation().value(),
                               CreateConstructRequestMessage::kLocationId);

    variable_datums.push_back(location_datum);
  }

  // Construct Orientation
  if (message.GetConstructOrientation().has_value()) {
    DIS::VariableDatum orientation_datum = DIS::VariableDatum();

    AddOrientationToVariableDatum(
        orientation_datum, message.GetConstructOrientation().value(),
        CreateConstructRequestMessage::kOrientationId);

    variable_datums.push_back(orientation_datum);
  }

  action_request_pdu.setVariableDatums(variable_datums);
};

void Dis7MessageSerializer::SerializeCreateEnvironmentRequestMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const CreateEnvironmentRequestMessage &message) {
  // Number Fixed Datums will always be 0 for Create Environment Request, as
  // they are not used.

  // Variable Datums (will always be 2)
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();
  // First variable datum is ID 1 and represents the environment name, padded to
  // a multiple of 8 bytes

  DIS::VariableDatum environment_name_datum = DIS::VariableDatum();
  AddStringToVariableDatum(environment_name_datum, message.GetEnvironmentName(),
                           CreateEnvironmentRequestMessage::kEnvironmentNameId);
  variable_datums.push_back(environment_name_datum);

  // Second variable datum is ID 2 and represents the environment metadata,
  // padded to a multiple of 8 bytes
  DIS::VariableDatum environment_metadata_datum = DIS::VariableDatum();
  AddJsonToVariableDatum(
      environment_metadata_datum, message.GetEnvironmentMetadata(),
      CreateEnvironmentRequestMessage::kEnvironmentMetadataId);
  variable_datums.push_back(environment_metadata_datum);

  action_request_pdu.setVariableDatums(variable_datums);
}

MusicByteBuffer Dis7MessageSerializer::SerializeMusicResponseMessage(
    const MusicResponseMessage &message) {
  // All subtypes of MusicResponseMessage will be an action response pdu
  DIS::ActionResponsePdu action_response_pdu = DIS::ActionResponsePdu();

  // Add PDU base data
  SetPDUBaseData(action_response_pdu, message);

  // Add originID, receiverID, requestID, and request status fields common to
  // each response message
  action_response_pdu.setOriginatingEntityID(
      ToDisEntityId(message.GetOriginId()));
  action_response_pdu.setReceivingEntityID(
      ToDisEntityId(message.GetReceiverId()));
  action_response_pdu.setRequestID(
      static_cast<unsigned int>(message.GetRequestId()));
  action_response_pdu.setRequestStatus(
      static_cast<unsigned char>(message.GetRequestStatus()));

  // Set Variable Datums to empty vectors for now; specific response
  // messages will replace as needed
  action_response_pdu.setVariableDatums(std::vector<DIS::VariableDatum>());

  // Set Fixed Datums to include the action type to more easily identify the
  // response message type
  std::vector<DIS::FixedDatum> fixed_datums = std::vector<DIS::FixedDatum>();
  DIS::FixedDatum action_type_datum = DIS::FixedDatum();
  action_type_datum.setFixedDatumID(MusicResponseMessage::kActionTypeId);
  action_type_datum.setFixedDatumValue(static_cast<unsigned int>(
      MusicMessageTypeHelper::GetInfoForType(message.GetType()).action_type));
  fixed_datums.push_back(action_type_datum);
  action_response_pdu.setFixedDatums(fixed_datums);

  const MusicCommandResponseMessage *command_response_message =
      dynamic_cast<const MusicCommandResponseMessage *>(&message);
  if (command_response_message != nullptr) {
    command_response_message->Validate();
    SerializeMusicCommandResponseMessage(action_response_pdu,
                                         *command_response_message);
  }

  const ConstructInteractionResponseMessage
      *construct_interaction_response_message =
          dynamic_cast<const ConstructInteractionResponseMessage *>(&message);
  if (construct_interaction_response_message != nullptr) {
    construct_interaction_response_message->Validate();
    SerializeConstructInteractionResponseMessage(
        action_response_pdu, *construct_interaction_response_message);
  }

  const ConstructControlResponseMessage *construct_control_response_message =
      dynamic_cast<const ConstructControlResponseMessage *>(&message);
  if (construct_control_response_message != nullptr) {
    construct_control_response_message->Validate();
    // No implementation needed.
  }

  const ConstructPrimaryControlResponseMessage
      *construct_primary_control_response_message =
          dynamic_cast<const ConstructPrimaryControlResponseMessage *>(
              &message);
  if (construct_primary_control_response_message != nullptr) {
    construct_primary_control_response_message->Validate();
    // No implementation needed.
  }

  // TODO: other response message types.
  return MarshalPduToBuffer(action_response_pdu);
}

void Dis7MessageSerializer::SerializeMusicCommandRequestMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const MusicCommandRequestMessage &message) {
  action_request_pdu.setActionID(
      static_cast<unsigned int>(message.GetCommandIdentifier()));

  // Check message type and call appropriate serialization method

  // message/command/constructcontrol/*
  const StopConstructMessage *stop_construct_message =
      dynamic_cast<const StopConstructMessage *>(&message);
  if (stop_construct_message != nullptr) {
    stop_construct_message->Validate();
    // No implementation needed; Stop Construct has no Fixed / Variable datums
    return;
  }

  const RemoveConstructMessage *remove_construct_message =
      dynamic_cast<const RemoveConstructMessage *>(&message);
  if (remove_construct_message != nullptr) {
    remove_construct_message->Validate();
    SerializeRemoveConstructMessage(action_request_pdu,
                                    *remove_construct_message);
    return;
  }

  // message/command/playercontrol/*
  const ControlInitiatedMessage *control_initiated_message =
      dynamic_cast<const ControlInitiatedMessage *>(&message);
  if (control_initiated_message != nullptr) {
    control_initiated_message->Validate();
    // No implementation needed; Control Initiated has no Fixed / Variable
    // datums
    return;
  }

  const ControlReleasedMessage *control_released_message =
      dynamic_cast<const ControlReleasedMessage *>(&message);
  if (control_released_message != nullptr) {
    control_released_message->Validate();
    // No implementation needed; Control Released has no Fixed / Variable datums
    return;
  }

  // message/command/scenario/*
  const CreateConstructRequestMessage *create_construct_request_message =
      dynamic_cast<const CreateConstructRequestMessage *>(&message);
  if (create_construct_request_message != nullptr) {
    create_construct_request_message->Validate();
    SerializeCreateConstructRequestMessage(action_request_pdu,
                                           *create_construct_request_message);
    return;
  }

  const CreateEnvironmentRequestMessage *create_environment_request_message =
      dynamic_cast<const CreateEnvironmentRequestMessage *>(&message);
  if (create_environment_request_message != nullptr) {
    create_environment_request_message->Validate();
    SerializeCreateEnvironmentRequestMessage(
        action_request_pdu, *create_environment_request_message);
    return;
  }

  const FinalizeScenarioRequestMessage *finalize_scenario_request_message =
      dynamic_cast<const FinalizeScenarioRequestMessage *>(&message);
  if (finalize_scenario_request_message != nullptr) {
    finalize_scenario_request_message->Validate();
    // No implementation needed; Finalize Scenario Request has no Fixed /
    // Variable datums
    return;
  }

  const ParameterizeConstructRequestMessage
      *parameterize_construct_request_message =
          dynamic_cast<const ParameterizeConstructRequestMessage *>(&message);
  if (parameterize_construct_request_message != nullptr) {
    parameterize_construct_request_message->Validate();
    SerializeParameterizeConstructRequestMessage(
        action_request_pdu, *parameterize_construct_request_message);
    return;
  }

  const ScenarioStartRequestMessage *scenario_start_request_message =
      dynamic_cast<const ScenarioStartRequestMessage *>(&message);
  if (scenario_start_request_message != nullptr) {
    scenario_start_request_message->Validate();
    // No implementation needed; Scenario Start Request has no Fixed /
    // Variable datums
    return;
  }
  // END COMMAND - SCENARIO REQUEST MESSAGES --------------------------

  // message/command/simulation/*
  const DisplayMessagesMessage *display_message =
      dynamic_cast<const DisplayMessagesMessage *>(&message);
  if (display_message != nullptr) {
    SerializeDisplayMessageMessage(action_request_pdu, *display_message);
    return;
  }

  // message/command/time/*
  const RequestSimulationTimeMessage *request_time_message =
      dynamic_cast<const RequestSimulationTimeMessage *>(&message);
  if (request_time_message != nullptr) {
    request_time_message->Validate();
    // No implementation needed; no fixed/variable datums.
    return;
  }

  const SetSimulationTimeMessage *set_simulation_time_message =
      dynamic_cast<const SetSimulationTimeMessage *>(&message);
  if (set_simulation_time_message != nullptr) {
    SerializeSetSimulationTimeMessage(action_request_pdu,
                                      *set_simulation_time_message);
    return;
  }

  const TransferConstructIdMessage *transfer_construct_id_message =
      dynamic_cast<const TransferConstructIdMessage *>(&message);
  if (transfer_construct_id_message != nullptr) {
    SerializeTransferConstructIdMessage(action_request_pdu,
                                        *transfer_construct_id_message);
    return;
  }

  LogError(
      "Dis7MessageSerializer::SerializeMusicCommandRequestMessage: "
      "Unsupported MusicCommandRequestMessage type for DIS7 serialization.");
}

void Dis7MessageSerializer::SerializeRemoveConstructMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const RemoveConstructMessage &message) {
  // No Fixed Datums

  // Removed Construct ID is saved as Entity ID in Variable Datum with ID 1
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();
  DIS::VariableDatum removed_construct_id_datum = DIS::VariableDatum();
  AddEntityIdToVariableDatum(removed_construct_id_datum,
                             message.GetRemovedConstruct(),
                             RemoveConstructMessage::kRemovedConstructId);
  variable_datums.push_back(removed_construct_id_datum);

  action_request_pdu.setVariableDatums(variable_datums);
}

void Dis7MessageSerializer::SerializeConstructInteractionRequestMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const ConstructInteractionRequestMessage &message) {
  std::vector<DIS::VariableDatum> datums = std::vector<DIS::VariableDatum>();

  // Variable Datums contains 2 records:
  // Interaction name (string), ID of 454110000
  DIS::VariableDatum name_datum = DIS::VariableDatum();
  AddStringToVariableDatum(
      name_datum, message.GetInteractionName(),
      ConstructInteractionRequestMessage::kInteractionNameId);
  datums.push_back(name_datum);
  // interaction data (JSON string), with an ID of:
  // 454110001 = Interaction Request
  // 454110002 = Pause Interaction
  // 454110003 = Stop Interaction
  // 454110004 = Resume Interaction
  DIS::VariableDatum interaction_data_datum = DIS::VariableDatum();
  AddJsonToVariableDatum(interaction_data_datum, message.GetInteractionData(),
                         (uint32_t)message.GetInteractionType());
  datums.push_back(interaction_data_datum);

  action_request_pdu.setVariableDatums(datums);
}

void Dis7MessageSerializer::SerializeConstructControlRequestMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const ConstructControlRequestMessage &message) {
  // No Fixed Datums

  // Variable Datums contains 2 records: The first with an ID of 454210001,
  // with a value of the Entity ID Record describing the target, padded to a
  // multiple of 64-bits.

  // The second with an ID of 454210002, with a value of a JSON object
  // representing the context of the Construct Control Request.
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();
  // First datum: target entity ID
  DIS::VariableDatum target_entity_datum = DIS::VariableDatum();

  AddEntityIdToVariableDatum(
      target_entity_datum, message.GetTargetConstruct(),
      ConstructControlRequestMessage::kTargetConstructId);
  variable_datums.push_back(target_entity_datum);

  // Second datum: context JSON
  DIS::VariableDatum context_datum = DIS::VariableDatum();

  AddJsonToVariableDatum(context_datum, message.GetContextJson(),
                         ConstructControlRequestMessage::kContextId);

  variable_datums.push_back(context_datum);

  action_request_pdu.setVariableDatums(variable_datums);
}

void Dis7MessageSerializer::SerializeControlTransferRequestMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const ControlTransferRequestMessage &message) {
  // No Fixed Datums

  // Variable Datums contains 3 records
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();

  // First record with an ID of 454210001, representing the target construct,
  // value of the Entity ID record of the target construct
  DIS::VariableDatum target_construct_datum = DIS::VariableDatum();
  AddEntityIdToVariableDatum(target_construct_datum,
                             message.GetTargetConstruct(),
                             ControlTransferRequestMessage::kTargetConstructId);
  variable_datums.push_back(target_construct_datum);

  // Second record with an ID of 454210002, representing the context of the
  // transfer request, with a value of a JSON object
  DIS::VariableDatum context_datum = DIS::VariableDatum();
  AddJsonToVariableDatum(context_datum, message.GetContext(),
                         ControlTransferRequestMessage::kContextId);
  variable_datums.push_back(context_datum);

  // Third record with an ID of 454210003, representing the proposed new
  // controller, with a value of the Entity ID record of the proposed new
  // controller
  DIS::VariableDatum proposed_controller_datum = DIS::VariableDatum();
  AddEntityIdToVariableDatum(
      proposed_controller_datum, message.GetProposedController(),
      ControlTransferRequestMessage::kProposedControllerId);
  variable_datums.push_back(proposed_controller_datum);

  action_request_pdu.setVariableDatums(variable_datums);
}

void Dis7MessageSerializer::SerializeDisplayMessageMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const DisplayMessagesMessage &message) {
  // Fixed Datums will contain 1 record of ID 1 that contains a 32-bit integer
  // for the time in seconds that the messages should display before
  // disappearing. A time of 0 indicates an indefinite display of messages.
  std::vector<DIS::FixedDatum> fixed_datums = std::vector<DIS::FixedDatum>();
  DIS::FixedDatum display_time_datum = DIS::FixedDatum();
  display_time_datum.setFixedDatumID(1);
  uint32_t display_time_seconds = static_cast<uint32_t>(message.GetTimeout());
  display_time_datum.setFixedDatumValue(display_time_seconds);
  fixed_datums.push_back(display_time_datum);
  action_request_pdu.setFixedDatums(fixed_datums);

  // Message text
  // NOTE: The MUSIC Spec does *NOT* specify what should be stored in the
  // variable datum as it mistakenly has what should be stored for a "Transfer
  // Construct ID" message and not a "Display Messages Message". We're
  // assuming based on context what should go here. The message is a list of
  // string messages. We will pad each string to a multiple of 8 bytes (64
  // bits), and give each an index starting with 1.

  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();

  for (size_t i = 0; i < message.GetMessages().size(); ++i) {
    DIS::VariableDatum text_datum = DIS::VariableDatum();
    AddStringToVariableDatum(text_datum, message.GetMessages()[i],
                             static_cast<uint32_t>(i + 1));
    variable_datums.push_back(text_datum);
  }

  action_request_pdu.setVariableDatums(variable_datums);
}

void Dis7MessageSerializer::SerializeSetCurrentControllerMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const SetCurrentControllerMessage &set_current_controller_message) {
  // No Fixed Datums

  // Variable Datums contains 1 record: The first with an ID of 454210004, value
  // of an Entity ID record describing the new current controller to be set plus
  // a padding to a multiple of 64 bits.
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();
  DIS::VariableDatum new_controller_datum = DIS::VariableDatum();

  AddEntityIdToVariableDatum(
      new_controller_datum,
      set_current_controller_message.GetCurrentControllerId(),
      SetCurrentControllerMessage::kCurrentControllerId);
  variable_datums.push_back(new_controller_datum);

  action_request_pdu.setVariableDatums(variable_datums);
}

void Dis7MessageSerializer::SerializeSetSimulationTimeMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const SetSimulationTimeMessage &message) {
  // Fixed Datums will contain 1 item with an ID of 1  and a value of the
  // simulation's world time to be set in seconds since epoch, as a 32-bit
  // uint.

  std::vector<DIS::FixedDatum> fixed_datums = std::vector<DIS::FixedDatum>();
  DIS::FixedDatum new_time_datum = DIS::FixedDatum();
  new_time_datum.setFixedDatumID(1);
  new_time_datum.setFixedDatumValue(
      static_cast<uint32_t>(message.GetNewTime()));
  fixed_datums.push_back(new_time_datum);

  action_request_pdu.setFixedDatums(fixed_datums);

  // No variable datums
  // NOTE: The MUSIC Spec states that there is a length of variable datums of
  // 1, but does not list any (and there are no other fields to be saved).
}

void Dis7MessageSerializer::SerializeConstructInteractionResponseMessage(
    DIS::ActionResponsePdu &action_response_pdu,
    const ConstructInteractionResponseMessage &message) {  // TODO:
  std::vector<DIS::VariableDatum> datums = std::vector<DIS::VariableDatum>();

  // No value, so nothing to do
  if (!message.GetOptionalData().has_value()) {
    action_response_pdu.setVariableDatums(datums);
    return;
  }

  DIS::VariableDatum id_datum = DIS::VariableDatum();
  AddJsonToVariableDatum(id_datum, message.GetOptionalData().value(),
                         ConstructInteractionResponseMessage::kOptionalDataId);
  datums.push_back(id_datum);
  action_response_pdu.setVariableDatums(datums);
}

void Dis7MessageSerializer::SerializeMusicCommandResponseMessage(
    DIS::ActionResponsePdu &action_response_pdu,
    const MusicCommandResponseMessage &message) {
  // Check over each type of command response message
  // COMMAND - SCENARIO RESPONSE MESSAGES ------------------------------
  const CreateConstructResponseMessage *create_construct_response_message =
      dynamic_cast<const CreateConstructResponseMessage *>(&message);

  if (create_construct_response_message != nullptr) {
    create_construct_response_message->Validate();
    SerializeCreateConstructResponseMessage(action_response_pdu,
                                            *create_construct_response_message);
    return;
  }

  const CreateEnvironmentResponseMessage *create_env_response_message =
      dynamic_cast<const CreateEnvironmentResponseMessage *>(&message);
  if (create_env_response_message != nullptr) {
    create_env_response_message->Validate();
    // No implementation needed; Create Environment Response has no Fixed /
    // Variable datums
    return;
  }

  const FinalizeScenarioResponseMessage *finalize_scenario_response_message =
      dynamic_cast<const FinalizeScenarioResponseMessage *>(&message);
  if (finalize_scenario_response_message != nullptr) {
    finalize_scenario_response_message->Validate();
    // No implementation needed; Finalize Scenario Response has no Fixed /
    // Variable datums
    return;
  }

  const ParameterizeConstructResponseMessage
      *parameterize_construct_response_message =
          dynamic_cast<const ParameterizeConstructResponseMessage *>(&message);
  if (parameterize_construct_response_message != nullptr) {
    parameterize_construct_response_message->Validate();
    // No implementation needed; Parameterize Construct Response has no Fixed
    // / Variable datums
    return;
  }
  // END COMMAND - SCENARIO RESPONSE MESSAGES --------------------------

  // time

  const SimulationTimeMessage *sim_time_message =
      dynamic_cast<const SimulationTimeMessage *>(&message);
  if (sim_time_message != nullptr) {
    sim_time_message->Validate();
    SerializeSimulationTimeMessage(action_response_pdu, *sim_time_message);
    return;
  }
}

void Dis7MessageSerializer::SerializeCreateConstructResponseMessage(
    DIS::ActionResponsePdu &action_response_pdu,
    const CreateConstructResponseMessage &message) {
  // Fixed Datums will always be 0 for Create Construct Response, as they are
  // not used (except we add the action type ID for every response message,
  // which is handled in SerializeMusicResponseMessage)

  // Variable Datums
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();

  // Exactly one variable datum is used with an ID of 1, padded to a multiple
  // of 64-bits, the Entity ID record describing the created construct.
  DIS::VariableDatum construct_id_datum = DIS::VariableDatum();
  construct_id_datum.setVariableDatumID(1);
  DIS::EntityID construct_entity_id_dis =
      ToDisEntityId(message.GetConstructId());
  std::vector<char> construct_id_bytes =
      MarshalToBytes(construct_entity_id_dis);
  PadToEightBytes(construct_id_bytes);
  construct_id_datum.setVariableDatums(construct_id_bytes.data(),
                                       (unsigned int)construct_id_bytes.size());
  variable_datums.push_back(construct_id_datum);
  action_response_pdu.setVariableDatums(variable_datums);
}

void Dis7MessageSerializer::SerializeSimulationTimeMessage(
    DIS::ActionResponsePdu &action_response_pdu,
    const SimulationTimeMessage &message) {
  // Fixed Datums will contain 1 item with an ID of 1 and a value of
  // the simulation's world time in seconds since epoch (uint32).

  DIS::FixedDatum world_time_datum = DIS::FixedDatum();
  world_time_datum.setFixedDatumID(SimulationTimeMessage::kSimTimeId);
  world_time_datum.setFixedDatumValue(
      static_cast<uint32_t>(message.GetSimTime()));
  action_response_pdu.getFixedDatums().push_back(world_time_datum);

  // No variable datums
}

void Dis7MessageSerializer::SerializeParameterizeConstructRequestMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const ParameterizeConstructRequestMessage &message) {
  // No fixed datums

  // Variable Datums will always include a construct parameters variable. It
  // may also include an ID of a construct for this construct to ghost in.

  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();

  if (message.GetGhostedId().has_value()) {
    DIS::VariableDatum ghosted_id_datum = DIS::VariableDatum();
    ghosted_id_datum.setVariableDatumID(
        ParameterizeConstructRequestMessage::kGhostedConstructId);
    DIS::EntityID ghosted_entity_id_dis =
        ToDisEntityId(message.GetGhostedId().value());
    std::vector<char> ghosted_id_bytes = MarshalToBytes(ghosted_entity_id_dis);
    PadToEightBytes(ghosted_id_bytes);
    ghosted_id_datum.setVariableDatums(
        ghosted_id_bytes.data(),
        static_cast<unsigned int>(ghosted_id_bytes.size()));
    variable_datums.push_back(ghosted_id_datum);
  }
  // Construct parameters datum
  DIS::VariableDatum construct_params_datum = DIS::VariableDatum();
  construct_params_datum.setVariableDatumID(
      ParameterizeConstructRequestMessage::kConstructParametersId);
  std::vector<char> construct_params_bytes =
      Get8BytePaddedString(message.GetConstructParameters().dump());
  construct_params_datum.setVariableDatums(
      construct_params_bytes.data(),
      static_cast<unsigned int>(construct_params_bytes.size()));
  variable_datums.push_back(construct_params_datum);

  action_request_pdu.setVariableDatums(variable_datums);
}

MusicByteBuffer Dis7MessageSerializer::SerializeEventMessage(
    const EventMessage &message) {
  message.Validate();
  DIS::SetDataPdu set_data_pdu = DIS::SetDataPdu();
  // Set PDU base data (96 bits)
  SetPDUBaseData(set_data_pdu, message);

  // Originating Entity ID (48 bits)
  set_data_pdu.setOriginatingEntityID(ToDisEntityId(message.GetOriginId()));
  // Receiving Entity ID (48 bits)
  // Set all 48 bits of the Receiving Entity ID Record to all 1s to indicate
  // that the event is intended for all applications.
  // (Not a field saved in MUSIC spec for EventMessage)
  set_data_pdu.setReceivingEntityID(AllOnesDisEntityId());

  // NOTE: Request ID Field is not set as the MUSIC spec does not specify its
  // use for an EventMessage, nor is it used in the Java Bindings.

  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();

  // Variable Datums will contain 2 records.
  // First record has an ID of 454310001 and a value of the Event Type fully
  // qualified name.

  DIS::VariableDatum eventTypeDatum = DIS::VariableDatum();
  AddStringToVariableDatum(eventTypeDatum, message.GetEventType(),
                           message.kEventTypeId);
  variable_datums.push_back(eventTypeDatum);

  // The second record has an ID of 454310002 and a value of the Event Data JSON
  // string, padded to a multiple of 8 bytes.
  DIS::VariableDatum eventDataDatum = DIS::VariableDatum();
  AddJsonToVariableDatum(eventDataDatum, message.GetEventData(),
                         message.kEventDataId);
  variable_datums.push_back(eventDataDatum);

  set_data_pdu.setVariableDatums(variable_datums);

  return MarshalPduToBuffer(set_data_pdu);
}

MusicByteBuffer Dis7MessageSerializer::SerializeCollisionMessage(
    const CollisionMessage &message) {
  message.Validate();
  DIS::CollisionPdu collision_pdu = DIS::CollisionPdu();

  // Set PDU base data
  SetPDUBaseData(collision_pdu, message);

  collision_pdu.setCollisionType(
      static_cast<unsigned char>(message.GetCollisionType()));
  collision_pdu.setCollidingEntityID(ToDisEntityId(message.GetCollidingId()));
  collision_pdu.setIssuingEntityID(ToDisEntityId(message.GetIssuingId()));
  collision_pdu.setEventID(ToDisEventId(message.GetCollisionEventId()));
  collision_pdu.setLocation(
      ToDisVectorFloatFromLocation(message.GetCollisionLocation()));
  collision_pdu.setVelocity(
      ToDisVectorFloatFromVector(message.GetCollisionVelocity()));
  collision_pdu.setMass(static_cast<float>(message.GetMass()));

  return MarshalPduToBuffer(collision_pdu);
}

void Dis7MessageSerializer::SerializeTransferConstructIdMessage(
    DIS::ActionRequestPdu &action_request_pdu,
    const TransferConstructIdMessage &message) {
  // No Fixed Datums for Transfer Construct ID PDUs
  std::vector<DIS::VariableDatum> variable_datums =
      std::vector<DIS::VariableDatum>();

  // Receiving EntityID should be set to all 1s to indicate broadcast to all
  action_request_pdu.setReceivingEntityID(AllOnesDisEntityId());

  // Set Old Construct ID Variable Datum
  DIS::VariableDatum old_id_datum = DIS::VariableDatum();
  AddEntityIdToVariableDatum(old_id_datum, message.GetOldId(),
                             TransferConstructIdMessage::kOldId);
  variable_datums.push_back(old_id_datum);

  // Set New Construct ID Variable Datum
  DIS::VariableDatum new_id_datum = DIS::VariableDatum();
  AddEntityIdToVariableDatum(new_id_datum, message.GetNewId(),
                             TransferConstructIdMessage::kNewId);

  variable_datums.push_back(new_id_datum);

  // Set variable datums
  action_request_pdu.setVariableDatums(variable_datums);
}

// Does not extend from same pdu as super class MunitionMessage
MusicByteBuffer Dis7MessageSerializer::SerializeFireMessage(
    const FireMessage &message) {
  DIS::FirePdu fire_pdu = DIS::FirePdu();
  // Add PDU base data
  SetPDUBaseData(fire_pdu, message);

  // Firing Entity ID Record
  fire_pdu.setFiringEntityID(ToDisEntityId(message.GetShooterId()));

  // Target Entity ID Record (optional)
  if (message.GetTargetId().has_value()) {
    fire_pdu.setTargetEntityID(ToDisEntityId(message.GetTargetId().value()));
  }

  // Munition ID Record
  fire_pdu.setMunitionExpendibleID(ToDisEntityId(message.GetMunitionId()));
  // Event ID Record
  fire_pdu.setEventID(ToDisEventId(message.GetEventId()));

  // Fire Mission Index Field (not used) See:
  // https://faculty.nps.edu/brutzman/vrtp/mil/navy/nps/disEnumerations/JdbeHtmlFiles/pdu/7d.htm

  // Munition Descriptor Record
  DIS::MunitionDescriptor munition_descriptor = DIS::MunitionDescriptor();
  munition_descriptor.setMunitionType(ToDisEntityType(message.GetMunition()));
  munition_descriptor.setWarhead(static_cast<int>(message.GetWarhead()));
  munition_descriptor.setFuse(static_cast<int>(message.GetFuse()));
  munition_descriptor.setQuantity(static_cast<int>(message.GetQuantity()));
  munition_descriptor.setRate(static_cast<int>(message.GetRate()));
  fire_pdu.setDescriptor(munition_descriptor);

  // Location in World Coordinates
  fire_pdu.setLocationInWorldCoordinates(
      ToDisVectorDoubleFromLocation(message.GetLocation()));

  // Velocity Record
  fire_pdu.setVelocity(ToDisVectorFloatFromVector(message.GetVelocity()));

  // Range Field
  fire_pdu.setRange(static_cast<float>(message.GetRangeToTarget()));

  return MarshalPduToBuffer(fire_pdu);
}

MusicByteBuffer Dis7MessageSerializer::SerializeDetonationMessage(
    const DetonationMessage &message) {
  DIS::DetonationPdu det_pdu = DIS::DetonationPdu();
  // Add PDU base data
  SetPDUBaseData(det_pdu, message);

  // Firing Entity ID Record
  det_pdu.setFiringEntityID(ToDisEntityId(message.GetShooterId()));

  // Target Entity ID Record (optional)
  if (message.GetTargetId().has_value()) {
    det_pdu.setTargetEntityID(ToDisEntityId(message.GetTargetId().value()));
  }

  // Munition ID Record
  det_pdu.setExplodingEntityID(ToDisEntityId(message.GetMunitionId()));
  // Event ID Record
  det_pdu.setEventID(ToDisEventId(message.GetEventId()));

  // Munition Descriptor Record
  DIS::MunitionDescriptor munition_descriptor = DIS::MunitionDescriptor();
  munition_descriptor.setMunitionType(ToDisEntityType(message.GetMunition()));
  munition_descriptor.setWarhead(static_cast<int>(message.GetWarhead()));
  munition_descriptor.setFuse(static_cast<int>(message.GetFuse()));
  munition_descriptor.setQuantity(static_cast<int>(message.GetQuantity()));
  munition_descriptor.setRate(static_cast<int>(message.GetRate()));
  det_pdu.setDescriptor(munition_descriptor);

  // Location in World Coordinates
  det_pdu.setLocationInWorldCoordinates(
      ToDisVectorDoubleFromLocation(message.GetLocation()));

  // Velocity Record
  det_pdu.setVelocity(ToDisVectorFloatFromVector(message.GetVelocity()));
  // Detonation Result
  det_pdu.setDetonationResult(static_cast<unsigned char>(message.GetResult()));

  // Not specified in MUSIC spec, using same as Location in World Coordinates
  det_pdu.setLocationOfEntityCoordinates(
      ToDisVectorFloatFromLocation(message.GetLocation()));

  // Articulation Record, aka Variable Parameters--not used in MUSIC spec so
  // left blank. See:
  // https://faculty.nps.edu/brutzman/vrtp/mil/navy/nps/disEnumerations/JdbeHtmlFiles/pdu/6d.htm
  det_pdu.setVariableParameters(std::vector<DIS::VariableParameter>());

  return MarshalPduToBuffer(det_pdu);
}

MusicByteBuffer Dis7MessageSerializer::SerializeMunitionMessage(
    const MunitionMessage &message) {
  const FireMessage *fire_message = dynamic_cast<const FireMessage *>(&message);
  if (fire_message != nullptr) {
    return SerializeFireMessage(*fire_message);
  }

  const DetonationMessage *detonation_message =
      dynamic_cast<const DetonationMessage *>(&message);
  if (detonation_message != nullptr) {
    return SerializeDetonationMessage(*detonation_message);
  }

  LogError("Attempted to serialize an unsupported muniton message type!");
  return MusicByteBuffer();
}

// CUSTOM PDU TYPES -------------------------------------------------
MusicByteBuffer Dis7MessageSerializer::SerializeConstructDataMessage(
    const ConstructDataMessage &message) {
  DIS::Pdu base_pdu = DIS::Pdu();
  SetPDUBaseData(base_pdu, message);
  MusicByteBuffer buff = MusicByteBuffer();

  // Base data 96bits
  std::vector<char> base_pdu_bytes = MarshalToBytes<DIS::Pdu>(base_pdu);
  buff.AppendCharVector(base_pdu_bytes);
  // Construct ID (originID) 48 bits
  buff.AppendCharVector(
      MarshalToBytes<DIS::EntityID>(ToDisEntityId(message.GetOriginId())));
  // Primary Controller ID 48 bits
  buff.AppendCharVector(MarshalToBytes<DIS::EntityID>(
      ToDisEntityId(message.GetPrimaryControllerId())));
  // Current Controller ID 48 bits
  buff.AppendCharVector(MarshalToBytes<DIS::EntityID>(
      ToDisEntityId(message.GetCurrentControllerId())));
  // Construct Information Record 16 bits (2x 8bit enums)
  ConstructRenderType render_type =
      message.GetConstructInformationRecord().GetConstructRender();
  // Construct Render Type (first 8 bits of Construct Information Record)
  // 0 = Non Physical
  // 4 = Unrendered Physical
  // 5 = Rendered Physical
  // 6 = Ghosted Construct
  // 7 = Ghosted Legacy
  buff.AppendValue<uint8_t>(
      static_cast<uint8_t>(
          message.GetConstructInformationRecord().GetConstructRender()),
      sizeof(uint8_t));
  // Construct Type (Category) (second 8 bits of Construct Information Record)
  // 0 = Aggregate
  // 1 = Device
  // 2 = Entity
  // 3= Aux Aide
  buff.AppendValue<uint8_t>(
      static_cast<uint8_t>(
          message.GetConstructInformationRecord().GetConstructCategory()),
      sizeof(uint8_t));

  // --- Ghosted constructs ---
  // clang-format off
  if (render_type == ConstructRenderType::kGhostedConstruct ||
        render_type == ConstructRenderType::kGhostedLegacy) {
    // clang-format on
    buff.AppendCharVector(MarshalToBytes<DIS::EntityID>(
        ToDisEntityId(message.GetGhostedId().value())));

    // Pad 16 bits
    buff.AppendValue<uint16_t>(static_cast<uint16_t>(0), sizeof(uint16_t));

  }
  // --- Physical constructs ---
  else if (render_type == ConstructRenderType::kRenderedPhysical ||
           render_type == ConstructRenderType::kUnrenderedPhysical) {
    // Force 8 bits
    buff.AppendValue<uint8_t>(static_cast<uint8_t>(message.GetForce().value()),
                              sizeof(uint8_t));
    // Entity Type 64 bits
    buff.AppendCharVector(MarshalToBytes<DIS::EntityType>(
        ToDisEntityType(message.GetEntityType().value())));

    // Location 192 bits
    buff.AppendCharVector(MarshalToBytes<DIS::Vector3Double>(
        ToDisVectorDoubleFromLocation(message.GetLocation().value())));

    // Orientation 96 bits
    buff.AppendCharVector(MarshalToBytes<DIS::EulerAngles>(
        ToDisEulerAngles(message.GetOrientation().value())));

    // Linear velocity 96 bits
    buff.AppendCharVector(MarshalToBytes<DIS::Vector3Float>(
        ToDisVectorFloatFromVector(message.GetVelocity().value())));

    // Dead Reckoning Parameters Record 320 bits
    // -- Dead Reckoning Algorithm 8 bits
    buff.AppendValue<uint8_t>(
        static_cast<uint8_t>(
            message.GetDeadReckoning().value().GetDeadReckAlgorithm()),
        sizeof(uint8_t));
    // -- Dead Reckoning Other Parameters 120 bits (15 bytes) of padding (empty)
    buff.AppendCharVector(std::vector<char>(15, 0));  // 120 bits = 15 bytes
    // -- Dead Reckoning Entity Linear Acceleration 96 bits
    buff.AppendCharVector(
        MarshalToBytes<DIS::Vector3Float>(ToDisVectorFloatFromVector(
            message.GetDeadReckoning().value().GetLinearAcceleration())));
    // -- Dead Reckoning Entity Angular Velocity 96 bits
    buff.AppendCharVector(
        MarshalToBytes<DIS::Vector3Float>(ToDisVectorFloatFromVector(
            message.GetDeadReckoning().value().GetAngularVelocity())));
    // End of Dead Reckoning Parameters Record

    // Catastrophic Damage 32 bits float
    buff.AppendValue<float>(message.GetDamage()->GetCatastrophicDamage(),
                            sizeof(float));
    // Mobility Damage 32 bits float
    buff.AppendValue<float>(message.GetDamage()->GetMobilityDamage(),
                            sizeof(float));
    // Firepower Disabled 8 bits
    // LSB set to 1 if this construct has been rendered incapable of firing
    // munitions
    buff.AppendValue(
        static_cast<unsigned char>(message.GetDamage()->IsFirepowerDisabled()),
        sizeof(u_char));
    // Write 48 bits of padding
    buff.AppendCharVector(std::vector<char>(6, 0));  // 48 bits = 6 bytes
  }
  std::string callsign = message.GetCallsign();
  std::vector<char> callsign_arr = Get8BytePaddedString(callsign);
  // Callsign length in bits (16 bits)
  uint16_t callsign_length_bits =
      static_cast<uint16_t>(callsign_arr.size() * 8);
  buff.AppendValue<uint16_t>(callsign_length_bits, sizeof(uint16_t));
  // Callsign string (padded to a multiple of 64 bits/8 bytes)
  buff.AppendCharVector(callsign_arr);

  std::vector<char> construct_arr =
      Get8BytePaddedString(message.GetConstructName());
  // Construct Name length in bits (16 bits)
  uint16_t construct_length_bits =
      static_cast<uint16_t>(construct_arr.size() * 8);
  buff.AppendValue<uint16_t>(construct_length_bits, sizeof(uint16_t));
  // Construct Name string (padded to a multiple of 64 bits/8 bytes)
  buff.AppendCharVector(construct_arr);

  std::vector<char> interaction_record_arr =
      Get8BytePaddedString(message.GetInteractionRecord());
  // Interaction Name length in bits (16 bits)
  uint16_t interaction_record_length_bits =
      static_cast<uint16_t>(interaction_record_arr.size() * 8);
  buff.AppendValue<uint16_t>(interaction_record_length_bits, sizeof(uint16_t));
  // Interaction Name string (padded to a multiple of 64 bits/8 bytes)
  buff.AppendCharVector(interaction_record_arr);

  // Padding of 16 bits
  buff.AppendCharVector(std::vector<char>(2, 0));  // 16 bits = 2 bytes

  return buff;
}

MusicByteBuffer Dis7MessageSerializer::SerializeStateFieldMessage(
    const StateFieldMessage &message) {
  DIS::Pdu base_pdu = DIS::Pdu();
  SetPDUBaseData(base_pdu, message);
  MusicByteBuffer buffer = MusicByteBuffer();
  // First 96 bits are base pdu (header + timestamp)
  buffer.AppendCharVector(MarshalToBytes(base_pdu));
  // Entity ID of this construct (origin ID) takes up 48 bits, pad to 64 bits
  // (+16b)
  std::vector<char> origin_id_bytes =
      MarshalToBytes(ToDisEntityId(message.GetOriginId()));
  PadToEightBytes(origin_id_bytes);
  buffer.AppendCharVector(origin_id_bytes);
  // Next is Payload Length (32 bits), including any padding to get payload
  // divisible by 64 bits
  std::vector<char> payload_bytes =
      Get8BytePaddedString(message.GetStateData().dump());
  uint32_t payload_size_bits = payload_bytes.size() * 8;

  buffer.AppendValue<uint32_t>(payload_size_bits, sizeof(uint32_t));
  // Then is the actual payload, padded to a multiple of 64 bits
  buffer.AppendCharVector(payload_bytes);
  return buffer;
}

MusicByteBuffer Dis7MessageSerializer::SerializePerceptionDataMessage(
    const PerceptionDataMessage &message) {
  DIS::Pdu base_pdu = DIS::Pdu();

  SetPDUBaseData(base_pdu, message);

  MusicByteBuffer buffer = MusicByteBuffer();
  buffer.AppendCharVector(MarshalToBytes(base_pdu));
  buffer.AppendCharVector(MarshalToBytes(ToDisEntityId(message.GetOriginId())));
  buffer.AppendValue(0L, sizeof(long));  // 32bit padding

  buffer.AppendValue(message.GetNumPerceptions(), sizeof(short));

  for (music::PerceptionRecord record : message.GetPerceptionRecords()) {
    buffer.AppendCharVector(
        MarshalToBytes(ToDisEntityId(record.GetPerceptionId())));
    buffer.AppendValue(static_cast<char>(record.GetForce()), sizeof(char));
    buffer.AppendCharVector(
        MarshalToBytes(ToDisEntityType(record.GetEntityType())));
    buffer.AppendValue(static_cast<char>(record.GetSystem()), sizeof(char));
    buffer.AppendValue((float)record.GetAccuracy(), sizeof(float));
    buffer.AppendValue(record.GetBearingFactor(), sizeof(int));
    buffer.AppendValue(record.GetAltitudeError(), sizeof(int));
    buffer.AppendValue(record.GetRangeError(), sizeof(int));
  }

  return buffer;
}

MusicByteBuffer Dis7MessageSerializer::SerializeWaypointDataMessage(
    const WaypointDataMessage &message) {
  DIS::Pdu base_pdu = DIS::Pdu();

  SetPDUBaseData(base_pdu, message);

  MusicByteBuffer buffer = MusicByteBuffer();
  buffer.AppendCharVector(MarshalToBytes(base_pdu));
  buffer.AppendCharVector(MarshalToBytes(ToDisEntityId(message.GetOriginId())));
  if (message.GetStartOrAppend()) {
    buffer.AppendValue((std::byte) true, sizeof(std::byte));
  } else {
    buffer.AppendValue((std::byte) false, sizeof(std::byte));
  }

  buffer.AppendValue((std::byte)0, sizeof(std::byte));  // 1byte of padding
  buffer.AppendValue((short)(message.GetCurrentWaypointIndex()), sizeof(short));
  buffer.AppendValue((short)(message.GetNumWaypoints()), sizeof(short));

  for (music::WaypointRecord record : message.GetWaypointRecords()) {
    buffer.AppendCharVector(MarshalToBytes(
        ToDisVectorFloatFromLocation(record.GetWorldCoordinateRecord())));
    buffer.AppendValue((int)record.GetEstimatedArrivalTime(), sizeof(int));
    buffer.AppendValue((int)record.GetArrivalTimeError(), sizeof(int));
  }

  return buffer;
}

std::shared_ptr<Dis7MessageSerializer> Dis7MessageSerializer::_instance =
    nullptr;
}  // namespace music