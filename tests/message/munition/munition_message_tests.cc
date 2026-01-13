// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "config/config.h"
#include "message/event_id.h"
#include "message/munition/munition_message.h"
#include "message/simulation_address.h"

namespace music::test {

TEST_GROUP(MunitionMessageTest){
    // SETUP
};

void CheckCustomValues(const MunitionMessage &message) {
  // Check header
  LONGS_EQUAL(100, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765894109000, message.GetHeader().GetTimestamp());

  // Check shooter ID
  LONGS_EQUAL(1, message.GetShooterId().GetSite());
  LONGS_EQUAL(2, message.GetShooterId().GetApplication());
  LONGS_EQUAL(3, message.GetShooterId().GetEntityNumber());

  // Check target ID
  LONGS_EQUAL(4, message.GetTargetId()->GetSite());
  LONGS_EQUAL(5, message.GetTargetId()->GetApplication());
  LONGS_EQUAL(6, message.GetTargetId()->GetEntityNumber());

  // Check munition ID
  LONGS_EQUAL(7, message.GetMunitionId().GetSite());
  LONGS_EQUAL(8, message.GetMunitionId().GetApplication());
  LONGS_EQUAL(9, message.GetMunitionId().GetEntityNumber());

  // Check velocity
  DOUBLES_EQUAL(10.0, message.GetVelocity().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(20.0, message.GetVelocity().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(30.0, message.GetVelocity().GetZ(), kDoublePrecision);
  // Check location
  DOUBLES_EQUAL(40.0, message.GetLocation().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(50.0, message.GetLocation().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(60.0, message.GetLocation().GetZ(), kDoublePrecision);

  // Check munition
  LONGS_EQUAL(7, message.GetMunition().GetKind());
  LONGS_EQUAL(8, message.GetMunition().GetDomain());
  LONGS_EQUAL(225, message.GetMunition().GetCountry());
  LONGS_EQUAL(9, message.GetMunition().GetCategory());
  LONGS_EQUAL(10, message.GetMunition().GetSubcategory());
  LONGS_EQUAL(11, message.GetMunition().GetSpecific());
  LONGS_EQUAL(12, message.GetMunition().GetExtra());

  // Check event ID
  LONGS_EQUAL(13, message.GetEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(14, message.GetEventId().GetSimulationAddress().GetApplication());
  LONGS_EQUAL(15, message.GetEventId().GetEventId());

  // Check warhead and fuse
  LONGS_EQUAL(static_cast<long>(Warhead::kHighExplosive),
              static_cast<long>(message.GetWarhead()));
  LONGS_EQUAL(static_cast<long>(FuseType::kProximity),
              static_cast<long>(message.GetFuse()));

  // Check quantity and rate
  LONGS_EQUAL(5, message.GetQuantity());
  LONGS_EQUAL(10, message.GetRate());
}

TEST(MunitionMessageTest, DefaultConstructor) {
  MunitionMessage message;

  // Shooter ID Entity ID
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetShooterId().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetShooterId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetShooterId().GetEntityNumber());

  // Target ID Entity ID (optional, should be std::nullopt)
  CHECK_FALSE(message.GetTargetId().has_value());

  // Velocity Vector
  CHECK_TRUE(std::isnan(message.GetVelocity().GetX()));
  CHECK_TRUE(std::isnan(message.GetVelocity().GetY()));
  CHECK_TRUE(std::isnan(message.GetVelocity().GetZ()));

  // Location
  CHECK_TRUE(std::isnan(message.GetLocation().GetX()));
  CHECK_TRUE(std::isnan(message.GetLocation().GetY()));
  CHECK_TRUE(std::isnan(message.GetLocation().GetZ()));

  // Munitition EntityType
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetMunition().GetKind());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetDomain());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetCountry());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetCategory());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetSubcategory());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetSpecific());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetExtra());

  // Event ID
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetEventId().GetSimulationAddress().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetEventId().GetEventId());

  // Warhead and Fuse
  LONGS_EQUAL(Warhead::kUnset, static_cast<long>(message.GetWarhead()));
  LONGS_EQUAL(FuseType::kUnset, static_cast<long>(message.GetFuse()));

  // Quantity and Rate
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetQuantity());
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetRate());
}

TEST(MunitionMessageTest, SettersAndGetters) {
  MunitionMessage message;

  // Header
  MusicMessageHeader header;
  header.SetExerciseId(100);
  header.SetTimestamp(1765894109000);
  message.SetHeader(header);

  // Shooter ID
  EntityId shooter_id;
  shooter_id.SetSite(1);
  shooter_id.SetApplication(2);
  shooter_id.SetEntityNumber(3);
  message.SetShooterId(shooter_id);

  // Target ID
  EntityId target_id;
  target_id.SetSite(4);
  target_id.SetApplication(5);
  target_id.SetEntityNumber(6);
  message.SetTargetId(target_id);

  // Munition ID
  EntityId munition_id;
  munition_id.SetSite(7);
  munition_id.SetApplication(8);
  munition_id.SetEntityNumber(9);
  message.SetMunitionId(munition_id);

  // Velocity
  Vector velocity;
  velocity.SetX(10.0);
  velocity.SetY(20.0);
  velocity.SetZ(30.0);
  message.SetVelocity(velocity);

  // Location
  Location location;
  location.SetX(40.0);
  location.SetY(50.0);
  location.SetZ(60.0);
  message.SetLocation(location);

  // Munition EntityType
  EntityType munition;
  munition.SetKind(7);
  munition.SetDomain(8);
  munition.SetCountry(225);
  munition.SetCategory(9);
  munition.SetSubcategory(10);
  munition.SetSpecific(11);
  munition.SetExtra(12);
  message.SetMunition(munition);

  // Event ID
  MusicEventId event_id =
      MusicEventIdBuilder()
          .SetEventId(15)
          .SetSimulationAddress(
              SimulationAddressBuilder().SetApplication(14).SetSite(13).Build())
          .Build();
  message.SetEventId(event_id);

  // Warhead and Fuse
  message.SetWarhead(Warhead::kHighExplosive);
  message.SetFuse(FuseType::kProximity);

  // Quantity and Rate
  message.SetQuantity(5);
  message.SetRate(10);

  CheckCustomValues(message);
}

TEST(MunitionMessageTest, Validate) {
  // Valid header
  MusicMessageHeader valid_header;
  valid_header.SetExerciseId(100);
  valid_header.SetTimestamp(1765894109000);

  // Valid shooter id
  EntityId valid_shooter_id;
  valid_shooter_id.SetSite(1);
  valid_shooter_id.SetApplication(2);
  valid_shooter_id.SetEntityNumber(3);

  // Valid target id
  EntityId valid_target_id;
  valid_target_id.SetSite(4);
  valid_target_id.SetApplication(5);
  valid_target_id.SetEntityNumber(6);

  // Valid munition id
  EntityId valid_munition_id;
  valid_munition_id.SetSite(7);
  valid_munition_id.SetApplication(8);
  valid_munition_id.SetEntityNumber(9);

  // Valid velocity
  Vector valid_velocity;
  valid_velocity.SetX(10.0);
  valid_velocity.SetY(20.0);
  valid_velocity.SetZ(30.0);

  // Valid location
  Location valid_location;
  valid_location.SetX(40.0);
  valid_location.SetY(50.0);
  valid_location.SetZ(60.0);

  // Valid munition entity type
  EntityType valid_munition;
  valid_munition.SetKind(7);
  valid_munition.SetDomain(8);
  valid_munition.SetCountry(225);
  valid_munition.SetCategory(9);
  valid_munition.SetSubcategory(10);
  valid_munition.SetSpecific(11);
  valid_munition.SetExtra(12);

  // Valid event id
  MusicEventId valid_event_id;
  SimulationAddress sim_address;
  sim_address.SetApplication(14);
  sim_address.SetSite(13);
  valid_event_id.SetSimulationAddress(sim_address);
  valid_event_id.SetEventId(15);

  MunitionMessage message = MunitionMessageBuilder()
                                .SetHeader(valid_header)
                                .SetShooterId(valid_shooter_id)
                                .SetTargetId(valid_target_id)
                                .SetMunitionId(valid_munition_id)
                                .SetVelocity(valid_velocity)
                                .SetLocation(valid_location)
                                .SetMunition(valid_munition)
                                .SetEventId(valid_event_id)
                                .SetWarhead(Warhead::kHighExplosive)
                                .SetFuse(FuseType::kProximity)
                                .SetQuantity(5)
                                .SetRate(10)
                                .Build();

  CheckCustomValues(message);

  // Initial validation should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Initial validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }
  // Set invalid warhead
  // (Default is OtherUnknown which is invalid)
  message.SetWarhead(Warhead::kOtherUnknown);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetWarhead(Warhead::kHighExplosive);

  // Set invalid fuse
  // (Default is OtherUnknown which is invalid)
  message.SetFuse(FuseType::kOtherUnknown);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetFuse(FuseType::kProximity);

  // Set invalid shooter id
  // (Default is all min values which is invalid)
  message.SetShooterId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetShooterId(valid_shooter_id);

  // Set invalid target id
  // (Default is std::nullopt which is valid, so set to invalid value)
  message.SetTargetId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetTargetId(valid_target_id);

  // Set invalid munition id
  // (Default is all min values which is invalid)
  message.SetMunitionId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetMunitionId(valid_munition_id);

  // Set invalid velocity
  // (Default is all NaN which is invalid)
  message.SetVelocity(Vector());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetVelocity(valid_velocity);

  // Set invalid location
  // (Default is all NaN which is invalid)
  message.SetLocation(Location());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetLocation(valid_location);

  // Set invalid munition entity type
  // (Default is all -1 which is invalid)
  message.SetMunition(EntityType());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetMunition(valid_munition);

  // Set invalid event id
  message.SetEventId(MusicEventId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetEventId(valid_event_id);

  // Set invalid quantity
  message.SetQuantity(-5);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetQuantity(5);

  // Set invalid rate
  message.SetRate(-10);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetRate(10);

  // Final check should pass
  // (all fields are now valid)
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Final validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }
}

TEST(MunitionMessageTest, Builder) {
  MunitionMessage message =
      MunitionMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(100)
                         .SetTimestamp(1765894109000)
                         .Build())
          .SetShooterId(EntityIdBuilder()
                            .SetSite(1)
                            .SetApplication(2)
                            .SetEntityNumber(3)
                            .Build())
          .SetTargetId(EntityIdBuilder()
                           .SetSite(4)
                           .SetApplication(5)
                           .SetEntityNumber(6)
                           .Build())
          .SetMunitionId(EntityIdBuilder()
                             .SetSite(7)
                             .SetApplication(8)
                             .SetEntityNumber(9)
                             .Build())
          .SetVelocity(VectorBuilder().SetX(10.0).SetY(20.0).SetZ(30.0).Build())
          .SetLocation(
              LocationBuilder().SetX(40.0).SetY(50.0).SetZ(60.0).Build())
          .SetMunition(EntityTypeBuilder()
                           .SetKind(7)
                           .SetDomain(8)
                           .SetCountry(225)
                           .SetCategory(9)
                           .SetSubcategory(10)
                           .SetSpecific(11)
                           .SetExtra(12)
                           .Build())
          .SetEventId(MusicEventIdBuilder()
                          .SetSimulationAddress(SimulationAddressBuilder()
                                                    .SetSite(13)
                                                    .SetApplication(14)
                                                    .Build())
                          .SetEventId(15)
                          .Build())
          .SetWarhead(Warhead::kHighExplosive)
          .SetFuse(FuseType::kProximity)
          .SetQuantity(5)
          .SetRate(10)
          .Build();

  CheckCustomValues(message);
}

TEST(MunitionMessageTest, ToJson) {
  MunitionMessage message =
      MunitionMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(100)
                         .SetTimestamp(1765894109000)
                         .Build())
          .SetShooterId(EntityIdBuilder()
                            .SetSite(1)
                            .SetApplication(2)
                            .SetEntityNumber(3)
                            .Build())
          .SetTargetId(EntityIdBuilder()
                           .SetSite(4)
                           .SetApplication(5)
                           .SetEntityNumber(6)
                           .Build())
          .SetMunitionId(EntityIdBuilder()
                             .SetSite(7)
                             .SetApplication(8)
                             .SetEntityNumber(9)
                             .Build())
          .SetVelocity(VectorBuilder().SetX(10.0).SetY(20.0).SetZ(30.0).Build())
          .SetLocation(
              LocationBuilder().SetX(40.0).SetY(50.0).SetZ(60.0).Build())
          .SetMunition(EntityTypeBuilder()
                           .SetKind(7)
                           .SetDomain(8)
                           .SetCountry(225)
                           .SetCategory(9)
                           .SetSubcategory(10)
                           .SetSpecific(11)
                           .SetExtra(12)
                           .Build())
          .SetEventId(MusicEventIdBuilder()
                          .SetSimulationAddress(SimulationAddressBuilder()
                                                    .SetSite(13)
                                                    .SetApplication(14)
                                                    .Build())
                          .SetEventId(15)
                          .Build())
          .SetWarhead(Warhead::kHighExplosive)
          .SetFuse(FuseType::kProximity)
          .SetQuantity(5)
          .SetRate(10)
          .Build();

  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(100, json["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(1765894109000, json["header"]["timestamp"].get<long>());
  // Check shooter ID
  LONGS_EQUAL(1, json["shooterID"]["siteID"].get<int>());
  LONGS_EQUAL(2, json["shooterID"]["appID"].get<int>());
  LONGS_EQUAL(3, json["shooterID"]["entityID"].get<int>());
  // Check target ID
  LONGS_EQUAL(4, json["targetID"]["siteID"].get<int>());
  LONGS_EQUAL(5, json["targetID"]["appID"].get<int>());
  LONGS_EQUAL(6, json["targetID"]["entityID"].get<int>());
  // Check munition ID
  LONGS_EQUAL(7, json["munitionID"]["siteID"].get<int>());
  LONGS_EQUAL(8, json["munitionID"]["appID"].get<int>());
  LONGS_EQUAL(9, json["munitionID"]["entityID"].get<int>());
  // Check velocity
  DOUBLES_EQUAL(10.0, json["velocity"]["x"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(20.0, json["velocity"]["y"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(30.0, json["velocity"]["z"].get<double>(), kDoublePrecision);
  // Check location
  DOUBLES_EQUAL(40.0, json["location"]["x"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(50.0, json["location"]["y"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(60.0, json["location"]["z"].get<double>(), kDoublePrecision);
  // Check munition
  LONGS_EQUAL(7, json["munition"]["kind"].get<int>());
  LONGS_EQUAL(8, json["munition"]["domain"].get<int>());
  LONGS_EQUAL(225, json["munition"]["country"].get<int>());
  LONGS_EQUAL(9, json["munition"]["category"].get<int>());
  LONGS_EQUAL(10, json["munition"]["subcategory"].get<int>());
  LONGS_EQUAL(11, json["munition"]["specific"].get<int>());
  LONGS_EQUAL(12, json["munition"]["extra"].get<int>());
  // Check event ID
  LONGS_EQUAL(13, json["eventID"]["siteID"].get<int>());
  LONGS_EQUAL(14, json["eventID"]["appID"].get<int>());
  LONGS_EQUAL(15, json["eventID"]["eventID"].get<int>());
  // Check warhead and fuse
  LONGS_EQUAL(static_cast<long>(Warhead::kHighExplosive),
              json["warhead"].get<long>());
  LONGS_EQUAL(static_cast<long>(FuseType::kProximity),
              json["fuse"].get<long>());
  // Check quantity and rate
  LONGS_EQUAL(5, json["quantity"].get<int>());
  LONGS_EQUAL(10, json["rate"].get<int>());
}

TEST(MunitionMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "eventID": {
        "appID": 14,
        "eventID": 15,
        "siteID": 13
    },
    "fuse": 3000,
    "header": {
        "exerciseID": 100,
        "timestamp": 1765894109000
    },
    "location": {
        "x": 40.0,
        "y": 50.0,
        "z": 60.0
    },
    "munition": {
        "category": 9,
        "country": 225,
        "domain": 8,
        "extra": 12,
        "kind": 7,
        "specific": 11,
        "subcategory": 10
    },
    "munitionID": {
        "appID": 8,
        "entityID": 9,
        "siteID": 7
    },
    "quantity": 5,
    "rate": 10,
    "shooterID": {
        "appID": 2,
        "entityID": 3,
        "siteID": 1
    },
    "targetID": {
        "appID": 5,
        "entityID": 6,
        "siteID": 4
    },
    "velocity": {
        "x": 10.0,
        "y": 20.0,
        "z": 30.0
    },
    "warhead": 1000
}
)");

  MunitionMessage message = MunitionMessage::FromJson(json);

  CheckCustomValues(message);
}

}  // namespace music::test