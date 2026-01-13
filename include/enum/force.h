// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_FORCE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_FORCE_H_

namespace music {
enum class Force {
  kUnset = -1,  // INVALID VALUE
  kOtherUnknown = 0,
  kFriendly = 1,
  kOpposing = 2,
  kNeutral = 3
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_FORCE_H_