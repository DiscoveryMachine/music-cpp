// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/construct/state_field_message.h"

namespace music::test {

TEST_GROUP(StateFieldMessageTest){};

TEST(StateFieldMessageTest, ValidationAndBuilder) {
  StateFieldMessage default_mesg;

  // A default-constructed StateFieldMessage begins in an invalid state.
  CHECK_THROWS(std::exception, default_mesg.Validate());

  StateFieldMessage valid_mesg =
      StateFieldMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(100)
                         .SetTimestamp(1767635267596)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(1)
                           .SetApplication(2)
                           .SetEntityNumber(3)
                           .Build())
          .SetStateData(nlohmann::json{{"key", "value"}})
          .Build();

  // The message should now be valid.
  valid_mesg.Validate();
}

TEST(StateFieldMessageTest, Json_RoundTrip) {
  StateFieldMessage original_mesg =
      StateFieldMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(100)
                         .SetTimestamp(1767635267596)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(1)
                           .SetApplication(2)
                           .SetEntityNumber(3)
                           .Build())
          .SetStateData(nlohmann::json{{"key", "value"}})
          .Build();

  nlohmann::json json = original_mesg.ToJson();
  StateFieldMessage deserialized_mesg = StateFieldMessage::FromJson(json);

  // Verify that the original and deserialized messages are equivalent.
  LONGS_EQUAL(original_mesg.GetHeader().GetExerciseId(),
              deserialized_mesg.GetHeader().GetExerciseId());
  LONGS_EQUAL(original_mesg.GetHeader().GetTimestamp(),
              deserialized_mesg.GetHeader().GetTimestamp());
  LONGS_EQUAL(original_mesg.GetOriginId().GetSite(),
              deserialized_mesg.GetOriginId().GetSite());
  LONGS_EQUAL(original_mesg.GetOriginId().GetApplication(),
              deserialized_mesg.GetOriginId().GetApplication());
  LONGS_EQUAL(original_mesg.GetOriginId().GetEntityNumber(),
              deserialized_mesg.GetOriginId().GetEntityNumber());
  STRCMP_EQUAL(original_mesg.GetStateData().dump().c_str(),
               deserialized_mesg.GetStateData().dump().c_str());
}

}  // namespace music::test