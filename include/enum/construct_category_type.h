#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_CONSTRUCT_CATEGORY_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_CONSTRUCT_CATEGORY_TYPE_H_

namespace music {
// The Construct Category Type enumeration signifies the category of the
// construct
enum class ConstructCategoryType {
  kUnset = -1,  // INVALID VALUE
  // The construct is an aggregate of multiple entities
  kAggregate = 0,
  // The construct is a single device. Generally seen as equipment.
  kDevice = 1,
  // The construct is a single physical entity, can be stationary or mobile.
  kEntity = 2,
  // The construct is an auxiliary aide. Generally all other constructs.
  kAuxiliaryAide = 3
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_CONSTRUCT_CATEGORY_TYPE_H_