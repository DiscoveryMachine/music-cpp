// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/music_endpoint_type.h"

#include <stdexcept>
#include <unordered_map>

namespace music {

static const std::unordered_map<MusicEndpointType, const std::string> kTypeStrs{
    {MusicEndpointType::kRedis, "REDIS"},
    {MusicEndpointType::kDis, "DIS"},
    {MusicEndpointType::kHla, "HLA"}};

static const std::string kUnrecognized = "Unrecognized";

MusicEndpointType
MusicEndpointTypeConverter::FromString(const std::string &endpt_type) {
  std::string valid_type_strs;
  for (const auto &[t, s] : kTypeStrs) {
    if (s == endpt_type) {
      return t;
    }

    valid_type_strs += "\"" + s + "\", ";
  }

  throw std::invalid_argument(
      endpt_type +
      " not recognized as a MusicEndpointType. "
      "Valid types include: " +
      valid_type_strs.substr(0, valid_type_strs.size() - 2));
}

std::string MusicEndpointTypeConverter::ToString(MusicEndpointType type) {
  const auto search = kTypeStrs.find(type);
  if (search == kTypeStrs.end()) {
    return kUnrecognized;
  }

  return search->second;
}

} // namespace music