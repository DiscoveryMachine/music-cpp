// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_SENSOR_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_SENSOR_TYPE_H_

namespace music {
enum class SensorType {
  // INVALID VALUE
  kUnset = -1,
  kOtherUnknown = 0,
  kEsm = 1,
  kRadar = 2,
  kSonar = 3,
  kVisual = 4,
  kAuditory = 5
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_SENSOR_TYPE_H_