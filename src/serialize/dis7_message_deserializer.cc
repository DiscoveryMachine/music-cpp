// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <dis7/ActionRequestPdu.h>
#include <dis7/ActionResponsePdu.h>
#include <dis7/CollisionPdu.h>
#include <dis7/DetonationPdu.h>
#include <dis7/EntityType.h>
#include <dis7/EulerAngles.h>
#include <dis7/FirePdu.h>
#include <dis7/Pdu.h>
#include <dis7/PduSuperclass.h>
#include <dis7/SetDataPdu.h>
#include <dis7/VariableDatum.h>
#include <dis7/Vector3Float.h>
#include <dis7/utils/DataStream.h>
#include <dis7/utils/Endian.h>

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "enum/dead_reckoning_algorithm_type.h"
#include "enum/detonation_result_type.h"
#include "enum/force.h"
#include "enum/music_message_type.h"
#include "enum/sensor_type.h"
#include "message/collision/collision_message.h"
#include "message/command/constructcontrol/remove_construct_message.h"
#include "message/command/constructcontrol/stop_construct_message.h"
#include "message/command/entity/transfer_construct_id_message.h"
#include "message/command/playercontrol/control_initiated_message.h"
#include "message/command/playercontrol/control_released_message.h"
#include "message/command/scenario/create_construct_request_message.h"
#include "message/command/scenario/create_construct_response_message.h"
#include "message/command/scenario/create_environment_request_message.h"
#include "message/command/scenario/create_environment_response_message.h"
#include "message/command/scenario/finalize_scenario_request_message.h"
#include "message/command/scenario/finalize_scenario_response_message.h"
#include "message/command/scenario/parameterize_construct_request_message.h"
#include "message/command/scenario/parameterize_construct_response_message.h"
#include "message/command/scenario/scenario_start_request_message.h"
#include "message/command/simulation/display_messages_message.h"
#include "message/command/time/request_simulation_time_message.h"
#include "message/command/time/set_simulation_time_message.h"
#include "message/command/time/simulation_time_message.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/damage.h"
#include "message/construct/dead_reckoning.h"
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
#include "message/control/control_transfer_response_message.h"
#include "message/control/primary_control_granted_message.h"
#include "message/control/primary_control_relinquished_message.h"
#include "message/control/set_current_controller_message.h"
#include "message/event/event_message.h"
#include "message/interaction/construct_interaction_request_message.h"
#include "message/interaction/construct_interaction_response_message.h"
#include "message/munition/detonation_message.h"
#include "message/music_request_message.h"
#include "message/perception/perception_data_message.h"
#include "message/perception/perception_record.h"
#include "message/waypoint/waypoint_data_message.h"
#include "message/waypoint/waypoint_record.h"
#include "serialize/dis7_message_deserializer.h"
#include "util/dis_conversion_utils.h"
#include "util/music_byte_buffer.h"
#include "util/validate.h"

namespace music {

std::shared_ptr<MusicMessage> Dis7MessageDeserializer::DeserializeMusicMessage(
    MusicByteBuffer &buffer) {
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());

  // Peek data from PDU superclass
  int protocol_version = (int)buffer.ReadValueSequential<unsigned char>();
  int exercise_id = (int)buffer.ReadValueSequential<unsigned char>();
  int pdu_type = (int)buffer.ReadValueSequential<unsigned char>();

  buffer.ResetReadPointerToBeginning();

  DIS::DataStream stream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  switch (pdu_type) {
    case 2: {  // Fire PDU
      DIS::FirePdu pdu;
      pdu.unmarshal(stream);
      return DeserializeFirePdu(pdu);
    }
    case 3: {  // Detonation PDU
      DIS::DetonationPdu pdu;
      pdu.unmarshal(stream);
      return DeserializeDetonationPdu(pdu);
    }
    case 4: {  // Collision PDU
      DIS::CollisionPdu pdu;
      pdu.unmarshal(stream);
      return DeserializeCollisionPdu(pdu);
    }
    case 16: {  // Action Request PDU
      DIS::ActionRequestPdu pdu;
      pdu.unmarshal(stream);
      return DeserializeActionRequestPdu(pdu);
    }
    case 17: {  // Action Response PDU
      DIS::ActionResponsePdu pdu;
      pdu.unmarshal(stream);
      MusicMessageType deserialized_type = GetTypeFromResponsePdu(pdu);
      return DeserializeActionResponsePdu(pdu, deserialized_type);
    }
    case 19: {  // Set Data PDU
      DIS::SetDataPdu pdu;
      pdu.unmarshal(stream);
      return DeserializeSetDataPdu(pdu);
    }
    case 230: {  // CUSTOM: ConstructData PDU
      return DeserializeConstructDataMessage(buffer);
    }
    case 232: {  // CUSTOM: PerceptionData PDU
      return DeserializePerceptionDataMessage(buffer);
    }
    case 233: {  // CUSTOM: StateFieldData PDU
      return DeserializeStateFieldMessage(buffer);
    }
    case 234: {  // CUSTOM: WaypointData PDU
      return DeserializeWaypointDataMessage(buffer);
    }
    default: {
      LogError(
          "Dis7MessageDeserializer::DeserializeMusicMessage - Unsupported PDU "
          "type");
      // Send base Music Message with header only if invalid PDU type
      DIS::Pdu pdu;
      pdu.unmarshal(stream);
      MusicMessageBuilder builder =
          MusicMessageBuilder().SetHeader(HeaderFromPDU(pdu));
      return std::make_shared<MusicMessage>(builder.Build());
    }
  }
}

std::shared_ptr<ConstructDataMessage>
Dis7MessageDeserializer::DeserializeConstructDataMessage(
    MusicByteBuffer &data_buffer) {
  ConstructDataMessageBuilder builder = ConstructDataMessageBuilder();
  DIS::Pdu base_pdu = DIS::Pdu();
  DIS::DataStream stream =
      DIS::DataStream(data_buffer.GetDataAt(data_buffer.GetLength()).data(),
                      data_buffer.GetLength(), DIS::Endian::BIG);
  base_pdu.unmarshal(stream);
  // Header is 96 bits
  builder.SetHeader(HeaderFromPDU(base_pdu));
  data_buffer.GetNextBytes(
      base_pdu.getMarshalledSize());  // Advance the read position to the start
                                      // of custom data

  // Origin ID (Construct ID) 48 bits
  // Primary Controller ID 48 bits
  // Current Controller ID 48 bits
  builder.SetOriginId(ReadEntityID(data_buffer))
      .SetPrimaryControllerId(ReadEntityID(data_buffer))
      .SetCurrentControllerId(ReadEntityID(data_buffer));

  // Construct Information Record 16 bits (2x 8bit enums)

  // Construct Render Type (first 8 bits of Construct Information Record)
  // 0 = Non Physical
  // 4 = Unrendered Physical
  // 5 = Rendered Physical
  // 6 = Ghosted Construct
  // 7 = Ghosted Legacy

  // Construct Type (Category) (second 8 bits of Construct Information Record)
  // 0 = Aggregate
  // 1 = Device
  // 2 = Entity
  // 3= Aux Aide
  ConstructInformationRecord info_record =
      ConstructInformationRecordBuilder()
          .SetConstructRenderType(static_cast<ConstructRenderType>(
              data_buffer.ReadValueSequential<uint8_t>()))
          .SetConstructCategoryType(static_cast<ConstructCategoryType>(
              data_buffer.ReadValueSequential<uint8_t>()))
          .Build();
  builder.SetConstructInformationRecord(info_record);

  if (info_record.GetConstructRender() ==
          ConstructRenderType::kGhostedConstruct ||
      info_record.GetConstructRender() == ConstructRenderType::kGhostedLegacy) {
    // --- Ghosted constructs ---
    // Ghosted ID 48 bits
    builder.SetGhostedId(ReadEntityID(data_buffer));
    // 16 bits of padding
    data_buffer.GetNextBytes(sizeof(uint16_t));
  } else if (info_record.GetConstructRender() ==
                 ConstructRenderType::kRenderedPhysical ||
             info_record.GetConstructRender() ==
                 ConstructRenderType::kUnrenderedPhysical) {
    // --- Physical constructs ---
    // Force (8 bits)
    // Entity Type (64 bits)
    // Location (192 bits)
    // Orientation (96 bits)
    // Linear Velocity (96 bits)
    builder
        .SetForce(
            static_cast<Force>(data_buffer.ReadValueSequential<uint8_t>()))
        .SetEntityType(ReadEntityType(data_buffer))
        .SetLocation(ReadDisVector3DoubleLocation(data_buffer))
        .SetOrientation(ReadOrientation(data_buffer))
        .SetVelocity(ReadVector3FloatVector(data_buffer));

    // Dead Reckoning Parameters Record
    DeadReckoningBuilder dead_reck_builder = DeadReckoningBuilder();
    // Dead Reckoning Algorithm Type (8 bits)
    dead_reck_builder.SetAlgorithmType(static_cast<DeadReckoningAlgorithmType>(
        data_buffer.ReadValueSequential<uint8_t>()));
    // Read forward though the padding (120 bits)
    data_buffer.GetNextBytes(
        15);  // 120 bits = 15 bytes
              // Dead Reckoning - Linear Acceleration (96 bits)
              // Dead Reckoning - Angular Velocity (96 bits)
    dead_reck_builder.SetLinearAcceleration(ReadVector3FloatVector(data_buffer))
        .SetAngularVelocity(ReadVector3FloatVector(data_buffer));
    builder.SetDeadReckoning(dead_reck_builder.Build());

    // Catastrophic Damage (32 bits)
    // Mobility Damage (32 bits)
    // Firepower Disabled (8 bits)
    Damage damage =
        MusicDamageBuilder()
            .SetCatastrophicDamage(data_buffer.ReadValueSequential<float>())
            .SetMobilityDamage(data_buffer.ReadValueSequential<float>())
            .SetIsFirepowerDisabled(data_buffer.ReadValueSequential<bool>())
            .Build();
    builder.SetDamage(damage);
    // Read past 48 bits of padding
    data_buffer.GetNextBytes(6);  // Read past padding
  }

  // Callsign length (16 bits)
  int callsign_length = data_buffer.ReadValueSequential<uint16_t>();
  // Callsign string (padded to a multiple of 64 bits/8 bytes)
  std::string callsign =
      TrimNullTerminatorPadding(data_buffer.GetNextBytes(callsign_length / 8));

  // Construct Name length (16 bits)
  int construct_name_length = data_buffer.ReadValueSequential<uint16_t>();
  // Construct Name string (padded to a multiple of 64 bits/8 bytes)
  std::string construct_name = TrimNullTerminatorPadding(
      data_buffer.GetNextBytes(construct_name_length / 8));

  // Interaction Record length (16 bits)
  int interaction_record_length = data_buffer.ReadValueSequential<uint16_t>();
  // Interaction Record string (padded to a multiple of 64 bits/8 bytes)

  std::string interaction_record = TrimNullTerminatorPadding(
      data_buffer.GetNextBytes(interaction_record_length / 8));

  builder.SetCallsign(callsign)
      .SetConstructName(construct_name)
      .SetInteractionRecord(interaction_record);

  // Padding of 16 bits
  data_buffer.GetNextBytes(sizeof(uint16_t));

  return std::make_shared<ConstructDataMessage>(builder.Build());
}

std::shared_ptr<PerceptionDataMessage>
Dis7MessageDeserializer::DeserializePerceptionDataMessage(
    MusicByteBuffer &data_buffer) {
  PerceptionDataMessageBuilder builder = PerceptionDataMessageBuilder();
  DIS::Pdu base_pdu = DIS::Pdu();
  DIS::DataStream stream =
      DIS::DataStream(data_buffer.GetDataAt(data_buffer.GetLength()).data(),
                      data_buffer.GetLength(), DIS::Endian::BIG);
  base_pdu.unmarshal(stream);
  builder.SetHeader(HeaderFromPDU(base_pdu));
  data_buffer.GetNextBytes(
      base_pdu.getMarshalledSize());  // Advance the read position to the start
                                      // of custom data

  builder.SetOriginId(ReadEntityID(data_buffer));
  data_buffer.ReadValueSequential<long>();  // padding
  int amount_of_perception_records = data_buffer.ReadValueSequential<short>();

  std::vector<PerceptionRecord> perception_records =
      std::vector<PerceptionRecord>();
  for (int i = 0; i < amount_of_perception_records; i++) {
    PerceptionRecordBuilder record_builder = PerceptionRecordBuilder();
    record_builder.SetPerceptionId(ReadEntityID(data_buffer))
        .SetForce(static_cast<Force>(data_buffer.ReadValueSequential<char>()))
        .SetEntityType(ReadEntityType(data_buffer))
        .SetSystem(
            static_cast<SensorType>(data_buffer.ReadValueSequential<char>()))
        .SetAccuracy(data_buffer.ReadValueSequential<float>())
        .SetBearingFactor(data_buffer.ReadValueSequential<int>())
        .SetAltitudeError(data_buffer.ReadValueSequential<int>())
        .SetRangeError(data_buffer.ReadValueSequential<int>());

    perception_records.push_back(record_builder.Build());
  }

  builder.SetNumPerceptions(amount_of_perception_records)
      .SetPerceptionRecords(perception_records);

  return std::make_shared<PerceptionDataMessage>(builder.Build());
}

std::shared_ptr<StateFieldMessage>
Dis7MessageDeserializer::DeserializeStateFieldMessage(
    MusicByteBuffer &data_buffer) {
  StateFieldMessageBuilder builder = StateFieldMessageBuilder();
  DIS::Pdu base_pdu = DIS::Pdu();
  DIS::DataStream stream =
      DIS::DataStream(data_buffer.GetDataAt(data_buffer.GetLength()).data(),
                      data_buffer.GetLength(), DIS::Endian::BIG);
  base_pdu.unmarshal(stream);
  builder.SetHeader(HeaderFromPDU(base_pdu));
  data_buffer.GetNextBytes(base_pdu.getMarshalledSize());

  // Origin ID
  builder.SetOriginId(ReadEntityID(data_buffer));
  // Serializer pads the 48-bit DIS EntityID (6 bytes) to 64 bits (8 bytes).
  data_buffer.ReadValueSequential<uint16_t>();  // padding

  // State Data
  uint32_t payload_length_bits = data_buffer.ReadValueSequential<uint32_t>();
  // Read the JSON payload (may be NUL-padded to 8-byte boundary).
  std::vector<char> payload_bytes =
      data_buffer.GetNextBytes(payload_length_bits / 8);
  std::string json_string = TrimNullTerminatorPadding(payload_bytes);
  nlohmann::json state_data = nlohmann::json::parse(json_string);
  builder.SetStateData(state_data);

  return std::make_shared<StateFieldMessage>(builder.Build());
}

std::shared_ptr<MusicRequestMessage>
Dis7MessageDeserializer::DeserializeActionRequestPdu(
    const DIS::ActionRequestPdu &pdu) {
  // Determine request type from the DIS PDU type + action ID.
  MusicMessageType type = MusicMessageType::kOtherUnknown;
  try {
    type =
        MusicMessageTypeHelper::FromValues(pdu.getPduType(), pdu.getActionID());
  } catch (const std::exception &e) {
    LogError(
        "Dis7MessageDeserializer::DeserializeActionRequestPdu - "
        "Unknown action request type: pduType=" +
        std::to_string(static_cast<int>(pdu.getPduType())) +
        ", actionID=" + std::to_string(static_cast<int>(pdu.getActionID())) +
        " (" + std::string(e.what()) + ")");
  }
  const auto &fixed_datums = pdu.getFixedDatums();
  const auto &variable_datums = pdu.getVariableDatums();

  switch (type) {
    // ---------------------------------------------------------------------
    // message/command/*
    // ---------------------------------------------------------------------

    // message/command/constructcontrol/*
    case MusicMessageType::kStopConstruct: {
      return DeserializeSimpleActionRequest<StopConstructMessageBuilder>(pdu,
                                                                         type);
    }

    case MusicMessageType::kRemoveConstruct: {
      RemoveConstructMessageBuilder builder;
      // Add removed construct id
      for (auto datum : variable_datums) {
        if (datum.getVariableDatumID() ==
            RemoveConstructMessage::kRemovedConstructId) {
          builder.SetRemovedConstruct(EntityIdFromDatumPayload(datum));
        }
      }
      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    // message/command/entity/*
    case MusicMessageType::kTransferConstructId: {
      TransferConstructIdMessageBuilder builder;
      for (auto datum : variable_datums) {
        if (datum.getVariableDatumID() == TransferConstructIdMessage::kNewId) {
          builder.SetNewId(EntityIdFromDatumPayload(datum));
        } else if (datum.getVariableDatumID() ==
                   TransferConstructIdMessage::kOldId) {
          builder.SetOldId(EntityIdFromDatumPayload(datum));
        }
      }
      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    // message/command/playercontrol/*
    case MusicMessageType::kControlInitiated: {
      return DeserializeSimpleActionRequest<ControlInitiatedMessageBuilder>(
          pdu, type);
    }

    case MusicMessageType::kControlReleased: {
      return DeserializeSimpleActionRequest<ControlReleasedMessageBuilder>(
          pdu, type);
    }

    // message/command/scenario/*
    case MusicMessageType::kCreateConstructRequest: {
      CreateConstructRequestMessageBuilder builder;

      for (const auto &datum : variable_datums) {
        const auto datum_id = static_cast<long>(datum.getVariableDatumID());

        if (datum_id == CreateConstructRequestMessage::kConstructTypeId) {
          builder.SetConstructType(DatumPayloadString(datum));
        } else if (datum_id == CreateConstructRequestMessage::kCallsignId) {
          builder.SetConstructCallsign(DatumPayloadString(datum));
        }

        // Optional fields.
        else if (datum_id == CreateConstructRequestMessage::kLocationId) {
          const std::vector<char> bytes = DatumBytes(datum);
          DIS::DataStream stream(bytes.data(), bytes.size(), DIS::Endian::BIG);
          DIS::Vector3Float dis_location;
          dis_location.unmarshal(stream);
          builder.SetConstructLocation(
              FromDisVector3FloatToLocation(dis_location));
        } else if (datum_id == CreateConstructRequestMessage::kOrientationId) {
          const std::vector<char> bytes = DatumBytes(datum);
          DIS::DataStream stream(bytes.data(), bytes.size(), DIS::Endian::BIG);
          DIS::EulerAngles dis_orientation;
          dis_orientation.unmarshal(stream);
          builder.SetConstructOrientation(FromDisEulerAngles(dis_orientation));
        }
      }

      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    case MusicMessageType::kCreateEnvironmentRequest: {
      CreateEnvironmentRequestMessageBuilder builder;

      for (const auto &datum : variable_datums) {
        const auto datum_id = static_cast<long>(datum.getVariableDatumID());

        if (datum_id == CreateEnvironmentRequestMessage::kEnvironmentNameId) {
          builder.SetEnvironmentName(DatumPayloadString(datum));
        } else if (datum_id ==
                   CreateEnvironmentRequestMessage::kEnvironmentMetadataId) {
          builder.SetEnvironmentMetadata(ParseJsonOrEmptyObject(
              DatumPayloadString(datum),
              "Dis7MessageDeserializer::DeserializeActionRequestPdu(" +
                  std::to_string(static_cast<int>(type)) + ")"));
        }
      }

      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    case MusicMessageType::kFinalizeScenarioRequest: {
      return DeserializeSimpleActionRequest<
          FinalizeScenarioRequestMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kParameterizeConstructRequest: {
      ParameterizeConstructRequestMessageBuilder builder;

      for (const auto &datum : variable_datums) {
        const auto datum_id = static_cast<long>(datum.getVariableDatumID());
        if (datum_id ==
            ParameterizeConstructRequestMessage::kGhostedConstructId) {
          builder.SetGhostedId(EntityIdFromDatumPayload(datum));
        } else if (datum_id == ParameterizeConstructRequestMessage::
                                   kConstructParametersId) {
          builder.SetConstructParameters(ParseJsonOrEmptyObject(
              DatumPayloadString(datum),
              "Dis7MessageDeserializer::DeserializeActionRequestPdu(" +
                  std::to_string(static_cast<int>(type)) + ")"));
        }
      }

      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    case MusicMessageType::kScenarioStart: {
      return DeserializeSimpleActionRequest<ScenarioStartRequestMessageBuilder>(
          pdu, type);
    }

    // message/command/simulation/*
    case MusicMessageType::kDisplayMessages: {
      DisplayMessagesMessageBuilder builder;
      // First set timeout from fixed datums
      for (const auto &fixed_datum : fixed_datums) {
        if (fixed_datum.getFixedDatumID() ==
            DisplayMessagesMessage::kTimeoutId) {
          builder.SetTimeout(fixed_datum.getFixedDatumValue());
        }
      }

      // Then set message text from variable datums
      std::vector<std::string> messages;
      for (const auto &datum : variable_datums) {
        std::string message_text = DatumPayloadString(datum);
        messages.push_back(message_text);
      }
      builder.SetMessages(messages);
      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    // message/command/time/*
    case MusicMessageType::kRequestSimulationTime: {
      return DeserializeSimpleActionRequest<
          RequestSimulationTimeMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kSetSimulationTime: {
      SetSimulationTimeMessageBuilder builder;
      // Set new time from request
      for (const auto &fixed_datum : fixed_datums) {
        if (fixed_datum.getFixedDatumID() ==
            SetSimulationTimeMessage::kNewTimeId) {
          builder.SetNewTime(fixed_datum.getFixedDatumValue());
        }
      }
      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

      // ---------------------------------------------------------------------
      // message/control/*
      // ---------------------------------------------------------------------

    case MusicMessageType::kControlRequest: {
      ConstructControlRequestMessageBuilder builder;

      for (const auto datum : variable_datums) {
        if (datum.getVariableDatumID() ==
            ConstructControlRequestMessage::kTargetConstructId) {
          builder.SetTargetConstruct(EntityIdFromDatumPayload(datum));
        } else if (datum.getVariableDatumID() ==
                   ConstructControlRequestMessage::kContextId) {
          // Assume context is JSON string like "{"context" :
          // "com.discoverymachine.context.Arrest"} We want to extract just the
          // string value for context.
          nlohmann::json context_json = ParseJsonOrEmptyObject(
              datum.getVariableDatums(),
              "Dis7MessageDeserializer::DeserializeActionRequestPdu(" +
                  std::to_string(static_cast<int>(type)) + ")");
          if (context_json.contains("context") &&
              context_json["context"].is_string()) {
            builder.SetContext(context_json["context"].get<std::string>());
          } else {
            builder.SetContext("");
          }
        }
      }

      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    case MusicMessageType::kPrimaryControlRequest: {
      return DeserializeSimpleActionRequest<
          ConstructPrimaryControlRequestMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kControlTransferRequest: {
      ControlTransferRequestMessageBuilder builder;

      for (auto variable_datum : variable_datums) {
        if (variable_datum.getVariableDatumID() ==
            ControlTransferRequestMessage::kTargetConstructId) {
          builder.SetTargetConstruct(EntityIdFromDatumPayload(variable_datum));
        } else if (variable_datum.getVariableDatumID() ==
                   ControlTransferRequestMessage::kProposedControllerId) {
          builder.SetProposedController(
              EntityIdFromDatumPayload(variable_datum));
        } else if (variable_datum.getVariableDatumID() ==
                   ControlTransferRequestMessage::kContextId) {
          builder.SetContext(ParseJsonOrEmptyObject(
              DatumPayloadString(variable_datum),
              "Dis7MessageDeserializer::DeserializeActionRequestPdu(" +
                  std::to_string(static_cast<int>(type)) + ")"));
        }
      }

      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    case MusicMessageType::kControlTransferGranted: {
      return DeserializeSimpleActionRequest<
          ControlTransferGrantedMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kPrimaryControlGranted: {
      return DeserializeSimpleActionRequest<
          PrimaryControlGrantedMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kPrimaryControlRelinquished: {
      return DeserializeSimpleActionRequest<
          PrimaryControlRelinquishedMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kSetCurrentController: {
      SetCurrentControllerMessageBuilder builder;

      for (auto datum : variable_datums) {
        if (datum.getVariableDatumID() ==
            SetCurrentControllerMessage::kCurrentControllerId) {
          builder.SetCurrentControllerId(EntityIdFromDatumPayload(datum));
        }
      }
      return BuildRequestMessageFromBuilder(pdu, type, builder);
    }

    case MusicMessageType::kControlGranted: {
      return DeserializeSimpleActionRequest<ControlGrantedMessageBuilder>(pdu,
                                                                          type);
    }

    case MusicMessageType::kControlLost: {
      return DeserializeSimpleActionRequest<ControlLostMessageBuilder>(pdu,
                                                                       type);
    }

    case MusicMessageType::kControlReclamation: {
      return DeserializeSimpleActionRequest<ControlReclamationMessageBuilder>(
          pdu, type);
    }

    case MusicMessageType::kControlRegained: {
      return DeserializeSimpleActionRequest<ControlRegainedMessageBuilder>(
          pdu, type);
    }

    case MusicMessageType::kControlRelinquished: {
      return DeserializeSimpleActionRequest<ControlRelinquishedMessageBuilder>(
          pdu, type);
    }

      // ---------------------------------------------------------------------
      // message/interaction/*
      // ---------------------------------------------------------------------

    case MusicMessageType::kConstructInteractionRequest: {
      std::shared_ptr<ConstructInteractionRequestMessage> message =
          std::static_pointer_cast<ConstructInteractionRequestMessage>(
              DeserializeSimpleActionRequest<
                  ConstructInteractionRequestMessageBuilder>(pdu, type));

      for (DIS::VariableDatum datum : variable_datums) {
        if (datum.getVariableDatumID() ==
            ConstructInteractionRequestMessage::kInteractionNameId) {
          message->SetInteractionName(DatumPayloadString(datum));
        } else {
          message->SetInteractionType(
              static_cast<InteractionType>(datum.getVariableDatumID()));
          // Parse second datum (interaction data)
          message->SetInteractionData(ParseJsonOrEmptyObject(
              DatumPayloadString(datum),
              "Dis7MessageDeserializer::DeserializeActionRequestPdu - "
              "ConstructInteractionRequest interactionData"));
        }
      }

      return message;
    }

    default: {
      LogError(
          "Dis7MessageDeserializer::DeserializeActionRequestPdu - "
          "Unsupported or unimplemented action request type " +
          std::to_string(static_cast<int>(type)));
      // Return a base request message so callers at least get header + IDs.
      return DeserializeSimpleActionRequest<MusicRequestMessageBuilder>(pdu,
                                                                        type);
    }
  }
}
std::shared_ptr<MusicResponseMessage>
Dis7MessageDeserializer::DeserializeActionResponsePdu(
    const DIS::ActionResponsePdu &pdu, MusicMessageType type) {
  const auto &fixed_datums = pdu.getFixedDatums();
  const auto &variable_datums = pdu.getVariableDatums();

  switch (type) {
      // ---------------------------------------------------------------------
      // message/command/*
      // ---------------------------------------------------------------------

      // message/command/scenario/*
    case MusicMessageType::kCreateConstructResponse: {
      CreateConstructResponseMessageBuilder builder;
      for (const auto &datum : variable_datums) {
        const auto datum_id = static_cast<long>(datum.getVariableDatumID());
        if (datum_id == CreateConstructResponseMessage::kConstructIdId) {
          builder.SetConstructId(EntityIdFromDatumPayload(datum));
        }
      }
      return BuildResponseMessageFromBuilder(pdu, type, builder);
    }
    case MusicMessageType::kCreateEnvironmentResponse: {
      return DeserializeSimpleActionResponse<
          CreateEnvironmentResponseMessageBuilder>(pdu, type);
    }
    case MusicMessageType::kFinalizeScenarioResponse: {
      return DeserializeSimpleActionResponse<
          FinalizeScenarioResponseMessageBuilder>(pdu, type);
    }
    case MusicMessageType::kParameterizeConstructResponse: {
      return DeserializeSimpleActionResponse<
          ParameterizeConstructResponseMessageBuilder>(pdu, type);
    }
      // command/time/*
    case MusicMessageType::kSimulationTime: {
      SimulationTimeMessageBuilder builder;
      for (const auto &fixed_datum : fixed_datums) {
        if (fixed_datum.getFixedDatumID() ==
            SimulationTimeMessage::kSimTimeId) {
          builder.SetSimTime(fixed_datum.getFixedDatumValue());
        }
      }
      return BuildResponseMessageFromBuilder(pdu, type, builder);
    }
    // ---------------------------------------------------------------------
    // message/control/*
    // ---------------------------------------------------------------------
    case MusicMessageType::kControlResponse: {
      return DeserializeSimpleActionResponse<
          ConstructControlResponseMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kPrimaryControlResponse: {
      return DeserializeSimpleActionResponse<
          ConstructPrimaryControlResponseMessageBuilder>(pdu, type);
    }

    case MusicMessageType::kControlTransferResponse: {
      return DeserializeSimpleActionResponse<
          ControlTransferResponseMessageBuilder>(pdu, type);
    }

      // ---------------------------------------------------------------------
      // message/interaction/*
      // ---------------------------------------------------------------------
    case MusicMessageType::kConstructInteractionResponse: {
      ConstructInteractionResponseMessageBuilder builder;
      for (const auto &datum : variable_datums) {
        if (datum.getVariableDatumID() ==
            ConstructInteractionResponseMessage::kOptionalDataId) {
          builder.SetOptionalData(
              nlohmann::json::parse(DatumPayloadString(datum)));
        }
      }
      return BuildResponseMessageFromBuilder(pdu, type, builder);
    }

    // ---------------------------------------------------------------------
    // Other message types
    // ---------------------------------------------------------------------
    default: {
      LogError(
          "Dis7MessageDeserializer::DeerializeActionResponsePdu - "
          "Unsupported or unimplemented action response type " +
          std::to_string(static_cast<int>(type)));
      // Return a base response message so callers at least get header + IDs.
      return DeserializeSimpleActionResponse<MusicResponseMessageBuilder>(pdu,
                                                                          type);
    }
  }
}

std::shared_ptr<WaypointDataMessage>
Dis7MessageDeserializer::DeserializeWaypointDataMessage(
    MusicByteBuffer &data_buffer) {
  WaypointDataMessageBuilder builder = WaypointDataMessageBuilder();
  DIS::Pdu base_pdu = DIS::Pdu();
  DIS::DataStream stream =
      DIS::DataStream(data_buffer.GetDataAt(data_buffer.GetLength()).data(),
                      data_buffer.GetLength(), DIS::Endian::BIG);
  base_pdu.unmarshal(stream);
  builder.SetHeader(HeaderFromPDU(base_pdu));
  data_buffer.GetNextBytes(
      base_pdu.getMarshalledSize());  // Advance the read position to the start
                                      // of custom data

  builder.SetOriginId(ReadEntityID(data_buffer))
      .SetStartOrAppend(
          static_cast<bool>(data_buffer.ReadValueSequential<std::byte>()));

  data_buffer.GetNextBytes(sizeof(std::byte));  // Padding
  builder.SetCurrentWaypointIndex(data_buffer.ReadValueSequential<short>());
  int num_waypoints = data_buffer.ReadValueSequential<short>();
  builder.SetNumWaypoints(num_waypoints);

  std::vector<WaypointRecord> waypoint_records = std::vector<WaypointRecord>();
  for (int i = 0; i < num_waypoints; i++) {
    // Read waypoint data
    WaypointRecordBuilder waypoint_record_builder = WaypointRecordBuilder();
    waypoint_record_builder
        .SetWorldCoordinateRecord(ReadDisVector3FloatLocation(data_buffer))
        .SetEstimatedArrivalTime(data_buffer.ReadValueSequential<int>())
        .SetArrivalTimeError(data_buffer.ReadValueSequential<int>());

    waypoint_records.push_back(waypoint_record_builder.Build());
  }

  builder.SetWaypointRecords(waypoint_records);
  return std::make_shared<WaypointDataMessage>(builder.Build());
}

std::shared_ptr<EventMessage> Dis7MessageDeserializer::DeserializeSetDataPdu(
    const DIS::SetDataPdu &pdu) {
  EventMessageBuilder builder = EventMessageBuilder();
  builder.SetHeader(HeaderFromPdu(pdu));
  builder.SetOriginId(FromDisEntityId(pdu.getOriginatingEntityID()));

  const std::vector<DIS::VariableDatum> &variable_datums =
      pdu.getVariableDatums();

  for (DIS::VariableDatum datum : variable_datums) {
    const long datum_id = static_cast<long>(datum.getVariableDatumID());
    if (datum_id == EventMessage::kEventTypeId) {
      builder.SetEventType(DatumPayloadString(datum));
    } else if (datum_id == EventMessage::kEventDataId) {
      builder.SetEventData(ParseJsonOrEmptyObject(DatumPayloadString(datum),
                                                  "Dis7MessageDeserializer::"
                                                  "DeserializeSetDataPdu"));
    }
  }

  return std::make_shared<EventMessage>(builder.Build());
}

std::shared_ptr<CollisionMessage>
Dis7MessageDeserializer::DeserializeCollisionPdu(const DIS::CollisionPdu &pdu) {
  CollisionMessageBuilder builder = CollisionMessageBuilder();
  builder.SetHeader(HeaderFromPdu(pdu));
  builder.SetCollisionType(static_cast<CollisionType>(pdu.getCollisionType()));
  builder.SetCollidingId(FromDisEntityId(pdu.getCollidingEntityID()));
  builder.SetIssuingId(FromDisEntityId(pdu.getIssuingEntityID()));
  builder.SetCollisionEventId(FromDisEventId(pdu.getEventID()));
  builder.SetCollisionLocation(
      FromDisVector3FloatToLocation(pdu.getLocation()));
  builder.SetCollisionVelocity(FromDisVector3Float(pdu.getVelocity()));
  builder.SetMass(pdu.getMass());

  return std::make_shared<CollisionMessage>(builder.Build());
}

std::shared_ptr<FireMessage> Dis7MessageDeserializer::DeserializeFirePdu(
    const DIS::FirePdu &pdu) {
  FireMessageBuilder builder = FireMessageBuilder();
  builder.SetHeader(HeaderFromPdu(pdu));

  builder.SetShooterId(FromDisEntityId(pdu.getFiringEntityID()));
  try {
    builder.SetTargetId(FromDisEntityId(pdu.getTargetEntityID()));
  } catch (const std::exception &e) {
    LogError(
        "Dis7MessageDeserializer::DeserializeFirePdu - Invalid target entity "
        "ID: " +
        std::string(e.what()));
    // This field is optional; leave unset if error occurs.
  }
  builder.SetMunitionId(FromDisEntityId(pdu.getMunitionExpendibleID()));
  builder.SetEventId(FromDisEventId(pdu.getEventID()));

  builder.SetMunition(FromDisEntityType(pdu.getDescriptor().getMunitionType()));
  builder.SetWarhead(static_cast<Warhead>(pdu.getDescriptor().getWarhead()));
  builder.SetFuse(static_cast<FuseType>(pdu.getDescriptor().getFuse()));
  builder.SetQuantity(static_cast<int>(pdu.getDescriptor().getQuantity()));
  builder.SetRate(static_cast<int>(pdu.getDescriptor().getRate()));

  builder.SetLocation(
      FromDisVector3Double(pdu.getLocationInWorldCoordinates()));
  builder.SetVelocity(FromDisVector3Float(pdu.getVelocity()));
  builder.SetRangeToTarget(pdu.getRange());

  return std::make_shared<FireMessage>(builder.Build());
}
std::shared_ptr<DetonationMessage>
Dis7MessageDeserializer::DeserializeDetonationPdu(
    const DIS::DetonationPdu &pdu) {
  DetonationMessageBuilder builder = DetonationMessageBuilder();
  builder.SetHeader(HeaderFromPdu(pdu));
  // Set Shooter ID
  builder.SetShooterId(FromDisEntityId(pdu.getFiringEntityID()));
  // Set Target ID
  try {
    builder.SetTargetId(FromDisEntityId(pdu.getTargetEntityID()));
  } catch (const std::exception &e) {
    LogError(
        "Dis7MessageDeserializer::DeserializeDetonationPdu - Invalid target "
        "entity ID: " +
        std::string(e.what()));
    // This field is optional; leave unset if error occurs.
  }
  // Set Munition ID
  builder.SetMunitionId(FromDisEntityId(pdu.getExplodingEntityID()));

  // Set Event ID
  builder.SetEventId(FromDisEventId(pdu.getEventID()));

  // Set Munition Type
  builder.SetMunition(FromDisEntityType(pdu.getDescriptor().getMunitionType()));
  // Set Warhead Type
  builder.SetWarhead(static_cast<Warhead>(pdu.getDescriptor().getWarhead()));
  // Set Fuse Type
  builder.SetFuse(static_cast<FuseType>(pdu.getDescriptor().getFuse()));
  // Set Quantity
  builder.SetQuantity(static_cast<int>(pdu.getDescriptor().getQuantity()));
  // Set Rate
  builder.SetRate(static_cast<int>(pdu.getDescriptor().getRate()));

  // Set Location
  builder.SetLocation(
      FromDisVector3Double(pdu.getLocationInWorldCoordinates()));
  // Set Velocity
  builder.SetVelocity(FromDisVector3Float(pdu.getVelocity()));
  // Set Detonation Result
  builder.SetResult(
      static_cast<DetonationResultType>(pdu.getDetonationResult()));

  return std::make_shared<DetonationMessage>(builder.Build());
}

}  // namespace music