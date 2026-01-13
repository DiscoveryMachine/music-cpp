// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "config/config.h"
#include "message/waypoint/waypoint_record.h"

namespace music::test {

TEST_GROUP(WaypointRecordTest){
    // SETUP
};

TEST(WaypointRecordTest, DefaultConstructor) {
  WaypointRecord waypoint_record;
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              waypoint_record.GetEstimatedArrivalTime());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              waypoint_record.GetArrivalTimeError());
  CHECK_TRUE(std::isnan(waypoint_record.GetWorldCoordinateRecord().GetX()));
  CHECK_TRUE(std::isnan(waypoint_record.GetWorldCoordinateRecord().GetY()));
  CHECK_TRUE(std::isnan(waypoint_record.GetWorldCoordinateRecord().GetZ()));
}

TEST(WaypointRecordTest, SettersAndGetters) {
  WaypointRecord waypoint_record;
  music::Location location;
  location.SetX(1.0);
  location.SetY(2.0);
  location.SetZ(3.0);
  waypoint_record.SetWorldCoordinateRecord(location);
  waypoint_record.SetEstimatedArrivalTime(100);
  waypoint_record.SetArrivalTimeError(10);

  CHECK_EQUAL(1.0, waypoint_record.GetWorldCoordinateRecord().GetX());
  CHECK_EQUAL(2.0, waypoint_record.GetWorldCoordinateRecord().GetY());
  CHECK_EQUAL(3.0, waypoint_record.GetWorldCoordinateRecord().GetZ());
  LONGS_EQUAL(100, waypoint_record.GetEstimatedArrivalTime());
  LONGS_EQUAL(10, waypoint_record.GetArrivalTimeError());
}

TEST(WaypointRecordTest, Builder) {
  auto waypoint_record =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(1765571723)
          .SetArrivalTimeError(10)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(0.0).SetY(1.0).SetZ(2.2).Build())
          .Build();
  LONGS_EQUAL(1765571723, waypoint_record.GetEstimatedArrivalTime());
  LONGS_EQUAL(10, waypoint_record.GetArrivalTimeError());
  DOUBLES_EQUAL(0.0, waypoint_record.GetWorldCoordinateRecord().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(1.0, waypoint_record.GetWorldCoordinateRecord().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.2, waypoint_record.GetWorldCoordinateRecord().GetZ(),
                kDoublePrecision);
}

TEST(WaypointRecordTest, Validate) {
  auto valid_location = LocationBuilder().SetX(0.0).SetY(1.0).SetZ(2.2).Build();
  auto waypoint_record = WaypointRecordBuilder()
                             .SetEstimatedArrivalTime(1765571723)
                             .SetArrivalTimeError(10)
                             .SetWorldCoordinateRecord(valid_location)
                             .Build();

  try {
    waypoint_record.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid ETA
  waypoint_record.SetEstimatedArrivalTime(-1000);
  CHECK_THROWS(std::exception, waypoint_record.Validate());

  waypoint_record.SetEstimatedArrivalTime(1765571723);

  try {
    waypoint_record.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid arrival time error
  waypoint_record.SetArrivalTimeError(-5);
  CHECK_THROWS(std::exception, waypoint_record.Validate());

  waypoint_record.SetArrivalTimeError(10);
  try {
    waypoint_record.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid location
  waypoint_record.SetWorldCoordinateRecord(Location());
  CHECK_THROWS(std::exception, waypoint_record.Validate());

  waypoint_record.SetWorldCoordinateRecord(valid_location);
  try {
    waypoint_record.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
}

TEST(WaypointRecordTest, ToJson) {
  auto waypoint_record =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(1765571723)
          .SetArrivalTimeError(10)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(0.0).SetY(1.0).SetZ(2.2).Build())
          .Build();

  nlohmann::json json = waypoint_record.ToJson();

  LONGS_EQUAL(1765571723, json["estimatedArrivalTime"].get<long>());
  LONGS_EQUAL(10, json["arrivalTimeError"].get<long>());
  DOUBLES_EQUAL(0.0, json["worldCoordinateRecord"]["x"].get<double>(),
                kDoublePrecision);
  DOUBLES_EQUAL(1.0, json["worldCoordinateRecord"]["y"].get<double>(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.2, json["worldCoordinateRecord"]["z"].get<double>(),
                kDoublePrecision);
}

TEST(WaypointRecordTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(

{
    "arrivalTimeError": 10,
    "estimatedArrivalTime": 1765571723,
    "worldCoordinateRecord": {
        "x": 0.0,
        "y": 1.0,
        "z": 2.2
    }
}

)");

  auto waypoint_record = WaypointRecord::FromJson(json);
  LONGS_EQUAL(1765571723, waypoint_record.GetEstimatedArrivalTime());
  LONGS_EQUAL(10, waypoint_record.GetArrivalTimeError());
  DOUBLES_EQUAL(0.0, waypoint_record.GetWorldCoordinateRecord().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(1.0, waypoint_record.GetWorldCoordinateRecord().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.2, waypoint_record.GetWorldCoordinateRecord().GetZ(),
                kDoublePrecision);
}

}  // namespace music::test