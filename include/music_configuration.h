// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_CONFIGURATION_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_CONFIGURATION_H_

#include "message/simulation_address.h"
#include "music_endpoint.h"

namespace music {
// Obtain a `music::MusicEndpoint` for communication with a simulation
// conducting a training exercise.
class MusicConfiguration {
  // virtual MusicEndpoint GetEndpoint(int exercise_id,
  //                                   SimulationAddress addr) = 0;
};
} // namespace music

#endif // MUSIC_DTO_CPP_MUSIC_INCLUDE_MUSIC_CONFIGURATION_H_