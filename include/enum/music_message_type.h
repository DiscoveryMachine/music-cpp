// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_MUSIC_MESSAGE_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_MUSIC_MESSAGE_TYPE_H_

#include <string>

namespace music {
struct MusicMessageTypeInfo {
  /* The PDU type for this message. */
  int pdu_type;

  /* The action type of this message. */
  int action_type;

  /* The name of this message. */
  std::string channel;

  /* Full Constructor */
  MusicMessageTypeInfo(int pdu_type, int action_type, std::string channel) {
    this->pdu_type = pdu_type;
    this->action_type = action_type;
    this->channel = channel;
  }
};

enum class MusicMessageType {
  kUnset = -1,  // INVALID VALUE
  kOtherUnknown,
  kUnknownResponse,
  kConstructData,
  kStateField,
  kConstructInteractionRequest,
  kConstructInteractionResponse,
  kPrimaryControlRequest,
  kPrimaryControlResponse,
  kPrimaryControlGranted,
  kControlRequest,
  kControlResponse,
  kControlGranted,
  kSetCurrentController,
  kControlLost,
  kControlRelinquished,
  kControlRegained,
  kPrimaryControlRelinquished,
  kControlTransferRequest,
  kControlTransferResponse,
  kControlTransferGranted,
  kControlReclamation,
  kPerception,
  kWaypoint,
  kEvent,
  kCreateEnvironmentRequest,
  kCreateEnvironmentResponse,
  kCreateConstructRequest,
  kCreateConstructResponse,
  kParameterizeConstructRequest,
  kParameterizeConstructResponse,
  kFinalizeScenarioRequest,
  kFinalizeScenarioResponse,
  kScenarioStart,
  kTransferConstructId,
  kDisplayMessages,
  kStopConstruct,
  kRemoveConstruct,
  kRequestSimulationTime,
  kSimulationTime,
  kSetSimulationTime,
  kControlInitiated,
  kControlReleased,
  kFire,
  kDetonation,
  kCollision,

  // helpers
  _count
};

class MusicMessageTypeHelper {
 public:
  static MusicMessageTypeInfo GetInfoForType(MusicMessageType type);
  static int GetPDUIdForType(MusicMessageType type);
  static MusicMessageType FromValues(int pduType, int actionId);
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_MUSIC_MESSAGE_TYPE_H_