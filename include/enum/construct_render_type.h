// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_CONSTRUCT_RENDER_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_CONSTRUCT_RENDER_TYPE_H_

namespace music {
// The Construct Render Type enumeration signifies how a construct should be
// rendered
enum class ConstructRenderType {
  kUnset = -1,  // INVALID VALUE
  // The construct is non-physical and should not be physically rendered
  kNonPhysical = 0,
  // The construct is physical but not currently rendered
  kUnrenderedPhysical = 4,
  // The construct is physical and currently rendered
  kRenderedPhysical = 5,
  // The construct is ghosted and should not be physically rendered
  kGhostedConstruct = 6,
  // The construct is a ghosting on a legacy entity and should not be physically
  // rendered
  kGhostedLegacy = 7
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_CONSTRUCT_RENDER_TYPE_H_