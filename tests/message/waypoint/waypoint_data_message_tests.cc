// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>

#include "config/config.h"
#include "message/entity_id.h"
#include "message/waypoint/waypoint_data_message.h"
#include "message/waypoint/waypoint_record.h"

namespace music::test {

TEST_GROUP(WaypointDataMessageTest){
    // SETUP
};

TEST(WaypointDataMessageTest, DefaultConstructor) {
  WaypointDataMessage message;
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetOriginId().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetApplication());
  CHECK_FALSE(message.GetStartOrAppend());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetCurrentWaypointIndex());
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetNumWaypoints());
  CHECK_TRUE(message.GetWaypointRecords().empty());
}
TEST(WaypointDataMessageTest, SettersAndGetters) {
  WaypointDataMessage message;
  EntityId origin_id = EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(50)
                           .SetSite(300)
                           .Build();
  message.SetOriginId(origin_id);
  message.SetStartOrAppend(true);
  message.SetCurrentWaypointIndex(5);
  std::vector<WaypointRecord> records = {WaypointRecord(), WaypointRecord()};
  message.SetWaypointRecords(records);
  message.SetNumWaypoints(1);

  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(300, message.GetOriginId().GetSite());
  LONGS_EQUAL(50, message.GetOriginId().GetApplication());
  CHECK_TRUE(message.GetStartOrAppend());
  LONGS_EQUAL(5, message.GetCurrentWaypointIndex());
  LONGS_EQUAL(1, message.GetNumWaypoints());
  CHECK_EQUAL(2u, message.GetWaypointRecords().size());
}

TEST(WaypointDataMessageTest, Builder) {
  EntityId origin_id = EntityIdBuilder()
                           .SetEntityNumber(10)
                           .SetApplication(20)
                           .SetSite(30)
                           .Build();
  std::vector<WaypointRecord> records = {WaypointRecord(), WaypointRecord(),
                                         WaypointRecord()};

  WaypointDataMessage message = WaypointDataMessageBuilder()
                                    .SetOriginId(origin_id)
                                    .SetStartOrAppend(true)
                                    .SetCurrentWaypointIndex(7)
                                    .SetNumWaypoints(1)
                                    .SetWaypointRecords(records)
                                    .Build();

  LONGS_EQUAL(10, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(30, message.GetOriginId().GetSite());
  LONGS_EQUAL(20, message.GetOriginId().GetApplication());
  CHECK_TRUE(message.GetStartOrAppend());
  LONGS_EQUAL(7, message.GetCurrentWaypointIndex());
  LONGS_EQUAL(1, message.GetNumWaypoints());
}

TEST(WaypointDataMessageTest, Validate) {
  WaypointDataMessage message;
  CHECK_THROWS(std::exception, message.Validate());

  EntityId valid_origin_id =
      EntityIdBuilder().SetEntityNumber(1).SetApplication(2).SetSite(3).Build();
  message.SetOriginId(valid_origin_id);
  message.SetStartOrAppend(true);
  message.SetCurrentWaypointIndex(0);

  WaypointRecord valid_record =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(1765571723)
          .SetArrivalTimeError(10)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(0.0).SetY(1.0).SetZ(2.2).Build())
          .Build();
  message.SetWaypointRecords({valid_record});
  message.SetNumWaypoints(1);

  // first check should pass with valid build
  try {
    message.Validate();
  } catch (std::exception &e) {
    FAIL("Validation should have passed but threw an exception.");
  }

  // Test invalid origin id
  EntityId invalid_origin_id = EntityIdBuilder()
                                   .SetEntityNumber(-1)
                                   .SetApplication(2)
                                   .SetSite(3)
                                   .Build();
  message.SetOriginId(invalid_origin_id);
  CHECK_THROWS(std::exception, message.Validate());

  message.SetOriginId(valid_origin_id);

  // Test negative current_waypoint_index
  message.SetCurrentWaypointIndex(-5);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetCurrentWaypointIndex(0);

  // Test set empty waypoint records
  message.SetWaypointRecords({});
  CHECK_THROWS(std::exception, message.Validate());

  // Test invalid record in waypoint records
  WaypointRecord invalid_record =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(-1000)
          .SetArrivalTimeError(10)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(0.0).SetY(1.0).SetZ(2.2).Build())
          .Build();
  message.SetWaypointRecords({invalid_record});
  CHECK_THROWS(std::exception, message.Validate());

  message.SetWaypointRecords({valid_record});
  try {
    message.Validate();
  } catch (std::exception &e) {
    FAIL("Validation should have passed but threw an exception.");
  }

  // Test num_waypoints exceeds size of waypoint_records
  message.SetNumWaypoints(5);
  CHECK_THROWS(std::exception, message.Validate());

  // Test negative num_waypoints
  message.SetNumWaypoints(-3);
  CHECK_THROWS(std::exception, message.Validate());
}

TEST(WaypointDataMessageTest, ToJson) {
  EntityId origin_id = EntityIdBuilder()
                           .SetEntityNumber(5)
                           .SetApplication(10)
                           .SetSite(15)
                           .Build();
  WaypointRecord record1 =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(1765809650)
          .SetArrivalTimeError(10)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(0.0).SetY(1.0).SetZ(2.2).Build())
          .Build();
  WaypointRecord record2 =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(1765809729)
          .SetArrivalTimeError(15)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(3.0).SetY(4.0).SetZ(5.2).Build())
          .Build();
  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(128)
                                  .SetTimestamp(1765809630192)
                                  .Build();

  WaypointDataMessage message = WaypointDataMessageBuilder()
                                    .SetHeader(header)
                                    .SetOriginId(origin_id)
                                    .SetStartOrAppend(false)
                                    .SetCurrentWaypointIndex(1)
                                    .SetWaypointRecords({record1, record2})
                                    .SetNumWaypoints(2)
                                    .Build();

  nlohmann::json j = message.ToJson();

  // header
  LONGS_EQUAL(128, j["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(1765809630192, j["header"]["timestamp"].get<long>());

  // origin id
  LONGS_EQUAL(5, j["originID"]["entityID"].get<int>());
  LONGS_EQUAL(15, j["originID"]["siteID"].get<int>());
  LONGS_EQUAL(10, j["originID"]["appID"].get<int>());

  // waypoint data message fields
  CHECK_FALSE(j["startOrAppend"].get<bool>());
  LONGS_EQUAL(1, j["currentWaypointIndex"].get<int>());
  LONGS_EQUAL(2, j["numWaypoints"].get<int>());

  // Waypoint records
  CHECK_EQUAL(2u, j["waypointRecords"].size());
  // record 0
  DOUBLES_EQUAL(
      0.0, j["waypointRecords"][0]["worldCoordinateRecord"]["x"].get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      1.0, j["waypointRecords"][0]["worldCoordinateRecord"]["y"].get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      2.2, j["waypointRecords"][0]["worldCoordinateRecord"]["z"].get<double>(),
      kDoublePrecision);
  LONGS_EQUAL(1765809650,
              j["waypointRecords"][0]["estimatedArrivalTime"].get<long>());
  LONGS_EQUAL(10, j["waypointRecords"][0]["arrivalTimeError"].get<long>());

  // record 1
  DOUBLES_EQUAL(
      3.0, j["waypointRecords"][1]["worldCoordinateRecord"]["x"].get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      4.0, j["waypointRecords"][1]["worldCoordinateRecord"]["y"].get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      5.2, j["waypointRecords"][1]["worldCoordinateRecord"]["z"].get<double>(),
      kDoublePrecision);
  LONGS_EQUAL(1765809729,
              j["waypointRecords"][1]["estimatedArrivalTime"].get<long>());
  LONGS_EQUAL(15, j["waypointRecords"][1]["arrivalTimeError"].get<long>());
}

TEST(WaypointDataMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "currentWaypointIndex": 1,
    "header": {
        "exerciseID": 128,
        "timestamp": 1765809630192
    },
    "numWaypoints": 2,
    "originID": {
        "appID": 10,
        "entityID": 5,
        "siteID": 15
    },
    "startOrAppend": false,
    "waypointRecords": [
        {
            "arrivalTimeError": 10,
            "estimatedArrivalTime": 1765809650,
            "worldCoordinateRecord": {
                "x": 0.0,
                "y": 1.0,
                "z": 2.2
            }
        },
        {
            "arrivalTimeError": 15,
            "estimatedArrivalTime": 1765809729,
            "worldCoordinateRecord": {
                "x": 3.0,
                "y": 4.0,
                "z": 5.2
            }
        }
    ]
}
  )");

  WaypointDataMessage message = WaypointDataMessage::FromJson(json);

  // header
  LONGS_EQUAL(128, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765809630192, message.GetHeader().GetTimestamp());

  // origin id
  LONGS_EQUAL(5, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(15, message.GetOriginId().GetSite());
  LONGS_EQUAL(10, message.GetOriginId().GetApplication());

  // waypoint data message fields
  CHECK_FALSE(message.GetStartOrAppend());
  LONGS_EQUAL(1, message.GetCurrentWaypointIndex());
  LONGS_EQUAL(2, message.GetNumWaypoints());

  // Waypoint records
  // record 0
  DOUBLES_EQUAL(
      0.0, message.GetWaypointRecords()[0].GetWorldCoordinateRecord().GetX(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      1.0, message.GetWaypointRecords()[0].GetWorldCoordinateRecord().GetY(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      2.2, message.GetWaypointRecords()[0].GetWorldCoordinateRecord().GetZ(),
      kDoublePrecision);
  LONGS_EQUAL(1765809650,
              message.GetWaypointRecords()[0].GetEstimatedArrivalTime());
  LONGS_EQUAL(10, message.GetWaypointRecords()[0].GetArrivalTimeError());

  // record 1
  DOUBLES_EQUAL(
      3.0, message.GetWaypointRecords()[1].GetWorldCoordinateRecord().GetX(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      4.0, message.GetWaypointRecords()[1].GetWorldCoordinateRecord().GetY(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      5.2, message.GetWaypointRecords()[1].GetWorldCoordinateRecord().GetZ(),
      kDoublePrecision);
  LONGS_EQUAL(1765809729,
              message.GetWaypointRecords()[1].GetEstimatedArrivalTime());
  LONGS_EQUAL(15, message.GetWaypointRecords()[1].GetArrivalTimeError());
  CHECK_EQUAL(2u, message.GetWaypointRecords().size());
}

}  // namespace music::test