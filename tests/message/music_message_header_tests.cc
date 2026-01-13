// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/music_message_header.h"

namespace music::test {
TEST_GROUP(MusicMessageHeaderTest){void setup(){
    // No setup needed
}};

TEST(MusicMessageHeaderTest, Builder) {
  MusicMessageHeaderBuilder builder;
  MusicMessageHeader header = builder.SetExerciseId(1).SetTimestamp(2L).Build();

  LONGS_EQUAL(1, header.GetExerciseId());
  LONGS_EQUAL(2L, header.GetTimestamp());
}

TEST(MusicMessageHeaderTest, Validate) {
  MusicMessageHeader header =
      MusicMessageHeaderBuilder().SetExerciseId(5).SetTimestamp(10L).Build();
  // Should not throw any exceptions
  header.Validate();

  // Test with no exercise_id - Validate() will throw
  header = MusicMessageHeader();  // Reset to default
  header.SetTimestamp(10L);
  CHECK_THROWS(std::exception, header.Validate());

  // Test with no timestamp - Validate() will throw
  header = MusicMessageHeader();
  header.SetExerciseId(5);
  CHECK_THROWS(std::exception, header.Validate());

  // Test with negative timestamp - Validate() will throw
  header = MusicMessageHeader();
  header.SetExerciseId(5);
  header.SetTimestamp(-1L);
  CHECK_THROWS(std::exception, header.Validate());

  // Test with negative exercise_id - Validate() will throw
  header = MusicMessageHeader();
  header.SetExerciseId(-1);
  header.SetTimestamp(10L);
  CHECK_THROWS(std::exception, header.Validate());

  // Test with invalid values (exercise_id out of range) - Validate() will
  // throw
  header = MusicMessageHeader();
  header.SetExerciseId(256);  // Out of range
  header.SetTimestamp(10L);
  CHECK_THROWS(std::exception, header.Validate());

  // Test with valid values again - should not throw
  header = MusicMessageHeaderBuilder()
               .SetExerciseId(5)
               .SetTimestamp(

                   10L)
               .Build();
  header.Validate();
}

TEST(MusicMessageHeaderTest, ToJson) {
  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(1)
                                  .SetTimestamp(1234567890L)
                                  .Build();
  nlohmann::json j = header.ToJson();
  LONGS_EQUAL(1, j[MusicMessageHeader::kExerciseIdKey].get<int>());
  LONGS_EQUAL(1234567890L, j[MusicMessageHeader::kTimestampKey].get<long>());
}

TEST(MusicMessageHeaderTest, FromJson) {
  nlohmann::json j = {{MusicMessageHeader::kExerciseIdKey, 42},
                      {MusicMessageHeader::kTimestampKey, 9876543210L}};

  MusicMessageHeader header = MusicMessageHeader::FromJson(j);

  LONGS_EQUAL(42, header.GetExerciseId());
  LONGS_EQUAL(9876543210L, header.GetTimestamp());
}

}  // namespace music::test