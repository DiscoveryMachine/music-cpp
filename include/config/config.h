// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_CONFIG_CONFIG_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_CONFIG_CONFIG_H_

#include <spdlog/spdlog.h>
namespace music {
inline bool throw_exceptions_on_error = false;
inline spdlog::level::level_enum spdlog_level = spdlog::level::level_enum::info;

inline double const kDoublePrecision = 0.0000001;
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_CONFIG_CONFIG_H_