// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_DEAD_RECKONING_ALGORITHM_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_DEAD_RECKONING_ALGORITHM_TYPE_H_

namespace music {

// The Dead Reckoning Algorithm Type enumeration signifies the algorithm used
// for dead reckoning.
enum class DeadReckoningAlgorithmType {
  kUnset = -1, // INVALID VALUE
  // Unknown or other algorithm
  kOther = 0,
  // Entity Does Not Move
  kStatic = 1,
  kDrm_F_P_W = 2,
  kDrm_R_P_W = 3,
  kDrm_R_V_W = 4,
  kDrm_F_V_W = 5,
  kDrm_F_P_B = 6,
  kDrm_R_P_B = 7,
  kDrm_R_V_B = 8,
  kDrm_F_V_B = 9
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_DEAD_RECKONING_ALGORITHM_TYPE_H_