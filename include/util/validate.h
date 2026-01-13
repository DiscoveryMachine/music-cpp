// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_VALIDATE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_VALIDATE_H_

#include <string>

#include "config/config.h"
namespace music {

inline void LogError(const std::string &message) {
  if (throw_exceptions_on_error) {
    throw std::runtime_error(message);
  }
  spdlog::error(message);
}

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_VALIDATE_H_