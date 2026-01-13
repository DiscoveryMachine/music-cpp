// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_DETONATION_RESULT_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_DETONATION_RESULT_TYPE_H_

namespace music {

enum class DetonationResultType {
  kUnset = -1,  // INVALID VALUE
  kOtherUnknown = 0,
  kEntityImpact = 1,
  kEntityProximateDetonation = 2,
  kGroundImpact = 3,
  kGroundProximateDetonation = 4,
  kDetonation = 5,
  kNone = 6,
  kHeHitSmall = 7,
  kHeHitMedium = 8,
  KeHitLarge = 9,
  kArmorPiercingHit = 10,
  kDirtBlastSmall = 11,
  kDirtBlastMedium = 12,
  kDirtBlastLarge = 13,
  kWaterBlastSmall = 14,
  kWaterBlastMedium = 15,
  kWaterBlastLarge = 16,
  kAirHit = 17,
  kBuildingHitSmall = 18,
  kBuildingHitMedium = 19,
  kBuildingHitLarge = 20,
  kMineClearingLineCharge = 21,
  kEnvironmentObjectImpact = 22,
  kEnvironmentObjectProximateDetonation = 23,
  kWaterImpact = 24,
  kAirBust = 25
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_DETONATION_RESULT_TYPE_H_