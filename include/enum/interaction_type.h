// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_INTERACTION_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_INTERACTION_TYPE_H_

namespace music {

enum class InteractionType : long {
  kUnset = -1,  // INVALID VALUE
  kUnassigned = 0,
  kRequest = 454110001L,
  kPause = 454110002L,
  kStop = 454110003L,
  kResume = 454110004L
};
}
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_INTERACTION_TYPE_H_