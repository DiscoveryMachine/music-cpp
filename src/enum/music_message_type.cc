// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <stdexcept>
#include <string>

#include "enum/music_message_type.h"

using namespace music;

MusicMessageTypeInfo MusicMessageTypeHelper::GetInfoForType(
    MusicMessageType type) {
  switch (type) {
    case MusicMessageType::kOtherUnknown:
      return MusicMessageTypeInfo(0, 0, "");
    case MusicMessageType::kUnknownResponse:
      return MusicMessageTypeInfo(17, 0, "");
    case MusicMessageType::kConstructData:
      return MusicMessageTypeInfo(230, 0, "ConstructData");
    case MusicMessageType::kStateField:
      return MusicMessageTypeInfo(233, 0, "StateFieldData");
    case MusicMessageType::kConstructInteractionRequest:
      return MusicMessageTypeInfo(16, 77, "ConstructInteractionRequest");
    case MusicMessageType::kConstructInteractionResponse:
      return MusicMessageTypeInfo(17, 177, "ConstructInteractionResponse");
    case MusicMessageType::kPrimaryControlRequest:
      return MusicMessageTypeInfo(16, 78, "PrimaryControlRequest");
    case MusicMessageType::kPrimaryControlResponse:
      return MusicMessageTypeInfo(17, 178, "PrimaryControlResponse");
    case MusicMessageType::kPrimaryControlGranted:
      return MusicMessageTypeInfo(16, 81, "PrimaryControlGranted");
    case MusicMessageType::kControlRequest:
      return MusicMessageTypeInfo(16, 79, "ConstructControlRequest");
    case MusicMessageType::kControlResponse:
      return MusicMessageTypeInfo(17, 179, "ConstructControlResponse");
    case MusicMessageType::kControlGranted:
      return MusicMessageTypeInfo(16, 82, "ConstructControlGranted");
    case MusicMessageType::kSetCurrentController:
      return MusicMessageTypeInfo(16, 84, "SetCurrentController");
    case MusicMessageType::kControlLost:
      return MusicMessageTypeInfo(16, 85, "ControlLost");
    case MusicMessageType::kControlRelinquished:
      return MusicMessageTypeInfo(16, 86, "ControlRelinquished");
    case MusicMessageType::kControlRegained:
      return MusicMessageTypeInfo(16, 87, "ControlRegained");
    case MusicMessageType::kPrimaryControlRelinquished:
      return MusicMessageTypeInfo(16, 88, "PrimaryControlRelinquished");
    case MusicMessageType::kControlTransferRequest:
      return MusicMessageTypeInfo(16, 80, "ControlTransferRequest");
    case MusicMessageType::kControlTransferResponse:
      return MusicMessageTypeInfo(17, 180, "ControlTransferResponse");
    case MusicMessageType::kControlTransferGranted:
      return MusicMessageTypeInfo(16, 83, "ControlTransferGranted");
    case MusicMessageType::kControlReclamation:
      return MusicMessageTypeInfo(16, 89, "ControlReclamation");
    case MusicMessageType::kPerception:
      return MusicMessageTypeInfo(232, 0, "PerceptionData");
    case MusicMessageType::kWaypoint:
      return MusicMessageTypeInfo(234, 0, "WaypointData");
    case MusicMessageType::kEvent:
      return MusicMessageTypeInfo(19, 454310001, "EventData");
    case MusicMessageType::kCreateEnvironmentRequest:
      return MusicMessageTypeInfo(16, 454013000, "Command");
    case MusicMessageType::kCreateEnvironmentResponse:
      return MusicMessageTypeInfo(17, 454013001, "Command");
    case MusicMessageType::kCreateConstructRequest:
      return MusicMessageTypeInfo(16, 454013002, "Command");
    case MusicMessageType::kCreateConstructResponse:
      return MusicMessageTypeInfo(17, 454013003, "Command");
    case MusicMessageType::kParameterizeConstructRequest:
      return MusicMessageTypeInfo(16, 454013004, "Command");
    case MusicMessageType::kParameterizeConstructResponse:
      return MusicMessageTypeInfo(17, 454013005, "Command");
    case MusicMessageType::kFinalizeScenarioRequest:
      return MusicMessageTypeInfo(16, 454013006, "Command");
    case MusicMessageType::kFinalizeScenarioResponse:
      return MusicMessageTypeInfo(17, 454013007, "Command");
    case MusicMessageType::kScenarioStart:
      return MusicMessageTypeInfo(16, 454013008, "Command");
    case MusicMessageType::kTransferConstructId:
      return MusicMessageTypeInfo(16, 454000002, "Command");
    case MusicMessageType::kDisplayMessages:
      return MusicMessageTypeInfo(16, 454004005, "Command");
    case MusicMessageType::kStopConstruct:
      return MusicMessageTypeInfo(16, 454007001, "Command");
    case MusicMessageType::kRemoveConstruct:
      return MusicMessageTypeInfo(16, 454007002, "Command");
    case MusicMessageType::kRequestSimulationTime:
      return MusicMessageTypeInfo(16, 454009000, "Command");
    case MusicMessageType::kSimulationTime:
      return MusicMessageTypeInfo(17, 454009999, "Command");
    case MusicMessageType::kSetSimulationTime:
      return MusicMessageTypeInfo(16, 454009001, "Command");
    case MusicMessageType::kControlInitiated:
      return MusicMessageTypeInfo(16, 454999001, "Command");
    case MusicMessageType::kControlReleased:
      return MusicMessageTypeInfo(16, 454999002, "Command");
    case MusicMessageType::kFire:
      return MusicMessageTypeInfo(2, 0, "Fire");
    case MusicMessageType::kDetonation:
      return MusicMessageTypeInfo(3, 0, "Detonation");
    case MusicMessageType::kCollision:
      return MusicMessageTypeInfo(4, 0, "Collision");

    case MusicMessageType::_count:
    case MusicMessageType::kUnset:
      throw std::invalid_argument(
          "Unknown type in MusicMessageTypeHelper::GetInfoForType");
      break;
  }

  // How did we get here??
  throw std::invalid_argument(
      "Unknown type in MusicMessageTypeHelper::GetInfoForType");
}

int MusicMessageTypeHelper::GetPDUIdForType(MusicMessageType type) {
  return MusicMessageTypeHelper::GetInfoForType(type).pdu_type;
}

MusicMessageType MusicMessageTypeHelper::FromValues(int pdu_type,
                                                    int action_id) {
  for (int value = 0; value != static_cast<int>(MusicMessageType::_count);
       ++value) {
    MusicMessageType type = static_cast<MusicMessageType>(value);
    MusicMessageTypeInfo info = GetInfoForType(type);
    if (info.pdu_type == pdu_type && info.action_type == action_id) {
      return type;
    }
  }
  throw std::invalid_argument("Unknown type with pdu type " +
                              std::to_string(pdu_type) + " and action ID " +
                              std::to_string(action_id));
}