#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_CONFIG_CONFIG_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_CONFIG_CONFIG_H_

#include <spdlog/spdlog.h>
namespace music {
inline bool throw_exceptions_on_error = false;
inline spdlog::level::level_enum spdlog_level = spdlog::level::level_enum::info;

inline double const kDoublePrecision = 0.0000001;
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_CONFIG_CONFIG_H_