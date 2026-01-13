// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_COLLISION_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_COLLISION_TYPE_H_

namespace music {
enum class CollisionType {
  kUnset = -1,
  kElastic = 0,
  kInElastic = 1,
  kOther = 2
};
}
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_COLLISION_TYPE_H_