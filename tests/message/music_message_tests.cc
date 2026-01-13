// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/music_message.h"

namespace music::test {
TEST_GROUP(MusicMessageTest){void setup(){
    // No setup needed
}};

TEST(MusicMessageTest, DefaultConstructor) {
  MusicMessage message;
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kUnset),
              static_cast<int>(message.GetType()));
  MusicMessageHeader header = message.GetHeader();
  LONGS_EQUAL(std::numeric_limits<int>::min(), header.GetExerciseId());
  LONGS_EQUAL(std::numeric_limits<long>::min(), header.GetTimestamp());
}

TEST(MusicMessageTest, SettersAndGetters) {
  MusicMessageHeader header;
  header.SetExerciseId(42);
  header.SetTimestamp(1747051200000L);

  MusicMessage message = MusicMessageBuilder()
                             .SetHeader(header)
                             .SetType(MusicMessageType::kDetonation)
                             .Build();

  LONGS_EQUAL(42, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1747051200000L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kDetonation),
              static_cast<int>(message.GetType()));
}

TEST(MusicMessageTest, Builder) {
  MusicMessage message = MusicMessageBuilder()
                             .SetHeader(MusicMessageHeaderBuilder()
                                            .SetExerciseId(7)
                                            .SetTimestamp(1625097600000L)
                                            .Build())
                             .SetType(MusicMessageType::kControlReclamation)
                             .Build();

  LONGS_EQUAL(7, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1625097600000L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlReclamation),
              static_cast<int>(message.GetType()));
}

TEST(MusicMessageTest, Validate) {
  MusicMessageHeader header =
      MusicMessageHeaderBuilder().SetExerciseId(5).SetTimestamp(10L).Build();
  MusicMessage message = MusicMessageBuilder()
                             .SetHeader(header)
                             .SetType(MusicMessageType::kDisplayMessages)
                             .Build();
  // Should not throw any exceptions
  message.Validate();

  // Test with invalid type - Validate() will throw
  message = MusicMessageBuilder()
                .SetHeader(header)
                .SetType(MusicMessageType::_count)
                .Build();
  CHECK_THROWS(std::exception, message.Validate());

  // Test with invalid type - Validate() will throw
  message = MusicMessageBuilder()
                .SetHeader(header)
                .SetType(MusicMessageType::kUnset)
                .Build();
  CHECK_THROWS(std::exception, message.Validate());

  // Test with invalid header - Validate() will throw
  MusicMessageHeader invalid_header;
  message = MusicMessageBuilder()
                .SetHeader(invalid_header)
                .SetType(MusicMessageType::kDisplayMessages)
                .Build();
  CHECK_THROWS(std::exception, message.Validate());
}

TEST(MusicMessageTest, ToJson) {
  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(3)
                                  .SetTimestamp(1234567890L)
                                  .Build();

  MusicMessage message =
      MusicMessageBuilder()
          .SetHeader(header)
          .SetType(MusicMessageType::kParameterizeConstructRequest)
          .Build();

  nlohmann::json j;
  to_json(j, message);

  LONGS_EQUAL(3, j[MusicMessage::kHeaderJsonKey]["exerciseID"].get<int>());
  LONGS_EQUAL(1234567890L,
              j[MusicMessage::kHeaderJsonKey]["timestamp"].get<long>());
}

TEST(MusicMessageTest, FromJson) {
  nlohmann::json j = {{MusicMessage::kHeaderJsonKey,
                       {{"exerciseID", 8}, {"timestamp", 9876543210L}}},
                      {MusicMessage::kTypeJsonKey,
                       static_cast<int>(MusicMessageType::kScenarioStart)}};

  MusicMessage message = MusicMessage::FromJson(j);

  LONGS_EQUAL(8, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(9876543210L, message.GetHeader().GetTimestamp());
}

TEST(MusicMessageTest, ToString) {
  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(9)
                                  .SetTimestamp(1122334455L)
                                  .Build();

  MusicMessage message = MusicMessageBuilder().SetHeader(header).Build();

  std::string message_str = message.ToString();
  std::string expected_str =
      "MusicMessage: "
      "{\"header\":{\"exerciseID\":9,\"timestamp\":1122334455}}";

  CHECK_EQUAL(expected_str, message_str);
}
}  // namespace music::test