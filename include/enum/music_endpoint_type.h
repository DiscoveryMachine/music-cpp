// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_MUSIC_ENDPOINT_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_MUSIC_ENDPOINT_TYPE_H_

#include <string>

namespace music {

// All mediums and protocols currently supported for `music::MusicEndpoint`
// instantiation.
//
// Mediums:
// - Redis
//
// Protocols:
// - DIS :: Distributed Interactive Simulation - IEEE-1278.1
// - HLA :: High Level Architecture - IEEE 1516-2025
enum class MusicEndpointType { kRedis, kDis, kHla };

class MusicEndpointTypeConverter {
 public:
  // Convert a string to a MusicEndpointType.
  //
  // Throws:
  // - std::invalid_argument
  static MusicEndpointType FromString(const std::string &s);

  static std::string ToString(MusicEndpointType t);
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_MUSIC_ENDPOINT_TYPE_H_
