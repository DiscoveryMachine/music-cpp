// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <CppUTest/TestHarness.h>

#include "message/command/entity/transfer_construct_id_message.h"
#include "message/entity_id.h"
#include "message/music_message_header.h"

namespace music::test {
TEST_GROUP(TransferConstructIdMessageTests){
    // DEFINE ANY SETUP HERE
};

bool CheckBuildThrowsError(TransferConstructIdMessageBuilder builder) {
  try {
    builder.Build();
  } catch (std::runtime_error err) {
    return true;
  }
  return false;
}

TEST(TransferConstructIdMessageTests, Test_FromJson_MatchesJsonData) {
  nlohmann::json sample_json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454000002,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767726217643
    },
    "newID": {
        "appID": 50,
        "entityID": 8,
        "siteID": 300
    },
    "oldID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 13579
}
    )");
  TransferConstructIdMessage parsed_message;
  from_json(sample_json, parsed_message);

  bool isValid = true;
  try {
    parsed_message.Validate();
  } catch (std::runtime_error err) {
    isValid = false;
  }

  // Parse produces valid object
  CHECK_TRUE(isValid);

  // New id field validation
  LONGS_EQUAL(parsed_message.GetNewId().GetApplication(), 50);
  LONGS_EQUAL(parsed_message.GetNewId().GetEntityNumber(), 8);
  LONGS_EQUAL(parsed_message.GetNewId().GetSite(), 300);
  // Old id validation
  LONGS_EQUAL(parsed_message.GetOldId().GetApplication(), 50);
  LONGS_EQUAL(parsed_message.GetOldId().GetEntityNumber(), 3);
  LONGS_EQUAL(parsed_message.GetOldId().GetSite(), 300);
}

TEST(TransferConstructIdMessageTests, Test_ToJson_JsonContainsAllFields) {
  TransferConstructIdMessage message =
      TransferConstructIdMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(5)
                         .SetTimestamp(100000L)
                         .Build())
          .SetNewId(EntityIdBuilder()
                        .SetApplication(6)
                        .SetEntityNumber(7)
                        .SetSite(8)
                        .Build())
          .SetOldId(EntityIdBuilder()
                        .SetApplication(2)
                        .SetEntityNumber(3)
                        .SetSite(4)
                        .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(10)
                           .SetApplication(20)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(10)
                             .SetApplication(20)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(54321)
          .Build();

  // Convert to JSON
  nlohmann::json json_message = nlohmann::json::object();
  to_json(json_message, message);

  // Validate presence of expected fields
  // clang-format off

  // Newid
  CHECK_EQUAL(json_message[TransferConstructIdMessage::kOldIdKey][SimulationAddress::kApplicationJsonKey].get<int>(), 2);
  CHECK_EQUAL(json_message[TransferConstructIdMessage::kOldIdKey][EntityId::kEntityNumberJsonKey].get<int>(), 3);
  CHECK_EQUAL(json_message[TransferConstructIdMessage::kOldIdKey][SimulationAddress::kSiteJsonKey].get<int>(), 4);

  // OldId
  CHECK_EQUAL(json_message[TransferConstructIdMessage::kNewIdKey][SimulationAddress::kApplicationJsonKey].get<int>(), 6);
  CHECK_EQUAL(json_message[TransferConstructIdMessage::kNewIdKey][EntityId::kEntityNumberJsonKey].get<int>(), 7);
  CHECK_EQUAL(json_message[TransferConstructIdMessage::kNewIdKey][SimulationAddress::kSiteJsonKey].get<int>(), 8);

  // Header
  CHECK_EQUAL(json_message[MusicMessage::kHeaderJsonKey][MusicMessageHeader::kExerciseIdKey].get<int>(), 5);
  CHECK_EQUAL(json_message[MusicMessage::kHeaderJsonKey][MusicMessageHeader::kTimestampKey].get<long>(), 100000);

  // clang-format on
}
}  // namespace music::test