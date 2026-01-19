// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <CppUTest/TestHarness.h>

#include "message/command/constructcontrol/remove_construct_message.h"

namespace music::test {
TEST_GROUP(RemoveConstructMessageTest){
    // SETUP
};

TEST(RemoveConstructMessageTest, DefaultConstructor) {
  RemoveConstructMessage message;

  // Validate type and command identifier
  LONGS_EQUAL(static_cast<long>(MusicMessageType::kRemoveConstruct),
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(454007002L, message.GetCommandIdentifier());

  // Removed Construct Entity ID
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetRemovedConstruct().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetRemovedConstruct().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetRemovedConstruct().GetEntityNumber());
}

TEST(RemoveConstructMessageTest, SettersAndGetters) {
  RemoveConstructMessage message;

  EntityId removed_construct;
  removed_construct.SetSite(1);
  removed_construct.SetApplication(2);
  removed_construct.SetEntityNumber(3);

  message.SetRemovedConstruct(removed_construct);

  LONGS_EQUAL(1, message.GetRemovedConstruct().GetSite());
  LONGS_EQUAL(2, message.GetRemovedConstruct().GetApplication());
  LONGS_EQUAL(3, message.GetRemovedConstruct().GetEntityNumber());
}

TEST(RemoveConstructMessageTest, Builder) {
  // Build a RemoveConstructMessage using the builder
  RemoveConstructMessage message =
      RemoveConstructMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(5)
                         .SetTimestamp(100000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(10)
                           .SetApplication(20)
                           .SetEntityNumber(30)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(40)
                             .SetApplication(50)
                             .SetEntityNumber(60)
                             .Build())
          .SetRemovedConstruct(EntityIdBuilder()
                                   .SetSite(1)
                                   .SetApplication(2)
                                   .SetEntityNumber(3)
                                   .Build())
          .SetRequestId(12345)
          .Build();

  // Check header
  LONGS_EQUAL(5, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(100000L, message.GetHeader().GetTimestamp());

  // Check origin ID
  LONGS_EQUAL(10, message.GetOriginId().GetSite());
  LONGS_EQUAL(20, message.GetOriginId().GetApplication());
  LONGS_EQUAL(30, message.GetOriginId().GetEntityNumber());

  // Check receiver ID
  LONGS_EQUAL(40, message.GetReceiverId().GetSite());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(60, message.GetReceiverId().GetEntityNumber());

  // Check removed construct ID
  LONGS_EQUAL(1, message.GetRemovedConstruct().GetSite());
  LONGS_EQUAL(2, message.GetRemovedConstruct().GetApplication());
  LONGS_EQUAL(3, message.GetRemovedConstruct().GetEntityNumber());

  // Check command identifier
  LONGS_EQUAL(454007002L, message.GetCommandIdentifier());
}

TEST(RemoveConstructMessageTest, Validate) {
  // Build a full, complete RemoveConstructMessage

  MusicMessageHeader valid_header = MusicMessageHeaderBuilder()
                                        .SetExerciseId(5)
                                        .SetTimestamp(100000L)
                                        .Build();
  EntityId valid_origin_id = EntityIdBuilder()
                                 .SetSite(10)
                                 .SetApplication(20)
                                 .SetEntityNumber(0)
                                 .Build();
  EntityId valid_receiver_id = EntityIdBuilder()
                                   .SetSite(40)
                                   .SetApplication(50)
                                   .SetEntityNumber(0)
                                   .Build();
  EntityId valid_removed_construct =
      EntityIdBuilder().SetSite(1).SetApplication(2).SetEntityNumber(3).Build();

  RemoveConstructMessage message =
      RemoveConstructMessageBuilder()
          .SetHeader(valid_header)
          .SetOriginId(valid_origin_id)
          .SetReceiverId(valid_receiver_id)
          .SetRemovedConstruct(valid_removed_construct)
          .SetRequestId(12345)
          .Build();

  // Should be valid
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
  // Set invalid removed construct
  message.SetRemovedConstruct(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetRemovedConstruct(valid_removed_construct);

  // Set invalid origin id
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetOriginId(valid_origin_id);

  // Final check should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Final validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }
}

TEST(RemoveConstructMessageTest, ToJson) {
  RemoveConstructMessage message =
      RemoveConstructMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(5)
                         .SetTimestamp(100000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(10)
                           .SetApplication(20)
                           .SetEntityNumber(30)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(40)
                             .SetApplication(50)
                             .SetEntityNumber(60)
                             .Build())
          .SetRemovedConstruct(EntityIdBuilder()
                                   .SetSite(1)
                                   .SetApplication(2)
                                   .SetEntityNumber(3)
                                   .Build())
          .SetRequestId(12345)
          .Build();

  nlohmann::json j = message.ToJson();

  // Check command identifier
  LONGS_EQUAL(454007002L, j["commandIdentifier"].get<long>());

  // Check header
  LONGS_EQUAL(5, j["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(100000L, j["header"]["timestamp"].get<long>());

  // Check origin ID
  LONGS_EQUAL(10, j["originID"]["siteID"].get<int>());
  LONGS_EQUAL(20, j["originID"]["appID"].get<int>());
  LONGS_EQUAL(30, j["originID"]["entityID"].get<int>());

  // Check receiver ID
  LONGS_EQUAL(40, j["receiverID"]["siteID"].get<int>());
  LONGS_EQUAL(50, j["receiverID"]["appID"].get<int>());
  LONGS_EQUAL(60, j["receiverID"]["entityID"].get<int>());

  // Check removed construct ID
  LONGS_EQUAL(1, j["removedConstruct"]["siteID"].get<int>());
  LONGS_EQUAL(2, j["removedConstruct"]["appID"].get<int>());
  LONGS_EQUAL(3, j["removedConstruct"]["entityID"].get<int>());
}

TEST(RemoveConstructMessageTest, FromJson) {
  nlohmann::json j = nlohmann::json::parse(R"(
    {
      "commandIdentifier": 454007002,
      "header": {
          "exerciseID": 5,
          "timestamp": 100000
      },
      "originID": {
          "siteID": 10,
          "appID": 20,
          "entityID": 30
      },
      "receiverID": {
          "siteID": 40,
          "appID": 50,
          "entityID": 60
      },
      "removedConstruct": {
          "siteID": 1,
          "appID": 2,
          "entityID": 3
      },
      "requestID": 12345
    }
    )");

  RemoveConstructMessage message = RemoveConstructMessage::FromJson(j);

  // Check header
  LONGS_EQUAL(5, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(100000L, message.GetHeader().GetTimestamp());

  // Check origin ID
  LONGS_EQUAL(10, message.GetOriginId().GetSite());
  LONGS_EQUAL(20, message.GetOriginId().GetApplication());
  LONGS_EQUAL(30, message.GetOriginId().GetEntityNumber());

  // Check receiver ID
  LONGS_EQUAL(40, message.GetReceiverId().GetSite());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(60, message.GetReceiverId().GetEntityNumber());

  // Check removed construct ID
  LONGS_EQUAL(1, message.GetRemovedConstruct().GetSite());
  LONGS_EQUAL(2, message.GetRemovedConstruct().GetApplication());
  LONGS_EQUAL(3, message.GetRemovedConstruct().GetEntityNumber());

  // Check request ID
  LONGS_EQUAL(12345, message.GetRequestId());

  // Check command identifier
  LONGS_EQUAL(454007002L, message.GetCommandIdentifier());
}
}  // namespace music::test