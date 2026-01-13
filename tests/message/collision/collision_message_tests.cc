// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <limits>
#include <nlohmann/json_fwd.hpp>

#include "config/config.h"
#include "message/collision/collision_message.h"
#include "message/event_id.h"
#include "message/music_message_header.h"
#include "message/simulation_address.h"

namespace music::test {
TEST_GROUP(CollisionMessageTest){
    // DEFINE ANY SETUP HERE
};

TEST(CollisionMessageTest, DefaultConstructor) {
  CollisionMessage collision_message;

  CHECK_TRUE(std::isnan(collision_message.GetMass()));
  LONGS_EQUAL(static_cast<int>(CollisionType::kUnset),
              static_cast<int>(collision_message.GetCollisionType()));
  CHECK_TRUE(std::isnan(collision_message.GetCollisionLocation().GetX()));
  CHECK_TRUE(std::isnan(collision_message.GetCollisionLocation().GetY()));
  CHECK_TRUE(std::isnan(collision_message.GetCollisionLocation().GetZ()));
  CHECK_TRUE(std::isnan(collision_message.GetCollisionVelocity().GetX()));
  CHECK_TRUE(std::isnan(collision_message.GetCollisionVelocity().GetY()));
  CHECK_TRUE(std::isnan(collision_message.GetCollisionVelocity().GetZ()));
  CHECK_TRUE(SimulationAddressBuilder().Build() ==
             collision_message.GetCollisionEventId().GetSimulationAddress());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              collision_message.GetCollisionEventId().GetEventId());
}

TEST(CollisionMessageTest, SettersAndGetters) {
  Location location;
  location.SetX(10.0);
  location.SetY(20.0);
  location.SetZ(30.0);

  Vector velocity;
  velocity.SetX(1.0);
  velocity.SetY(2.0);
  velocity.SetZ(3.0);

  MusicEventId event_id;
  event_id.SetSimulationAddress(
      SimulationAddressBuilder().SetSite(100).SetApplication(200).Build());
  event_id.SetEventId(300);

  CollisionMessage collision_message;
  collision_message.SetMass(1500.0);
  collision_message.SetCollisionType(CollisionType::kElastic);
  collision_message.SetCollisionLocation(location);
  collision_message.SetCollisionVelocity(velocity);
  collision_message.SetCollisionEventId(event_id);

  DOUBLES_EQUAL(1500.0, collision_message.GetMass(), kDoublePrecision);
  LONGS_EQUAL(static_cast<int>(CollisionType::kElastic),
              static_cast<int>(collision_message.GetCollisionType()));
  DOUBLES_EQUAL(10.0, collision_message.GetCollisionLocation().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(20.0, collision_message.GetCollisionLocation().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(30.0, collision_message.GetCollisionLocation().GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(1.0, collision_message.GetCollisionVelocity().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.0, collision_message.GetCollisionVelocity().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(3.0, collision_message.GetCollisionVelocity().GetZ(),
                kDoublePrecision);
  CHECK_TRUE(
      SimulationAddressBuilder().SetSite(100).SetApplication(200).Build() ==
      collision_message.GetCollisionEventId().GetSimulationAddress());
  LONGS_EQUAL(300, collision_message.GetCollisionEventId().GetEventId());
}

TEST(CollisionMessageTest, Validate) {
  Location location = LocationBuilder().SetX(0.0).SetY(0.0).SetZ(0.0).Build();

  Vector velocity = VectorBuilder().SetX(1.0).SetY(1.0).SetZ(1.0).Build();

  MusicEventId event_id =
      MusicEventIdBuilder()
          .SetSimulationAddress(
              SimulationAddressBuilder().SetSite(10).SetApplication(20).Build())
          .SetEventId(1)
          .Build();

  CollisionMessage collision_message;
  collision_message.SetMass(1000.0);
  collision_message.SetCollisionType(CollisionType::kInElastic);
  collision_message.SetCollisionLocation(location);
  collision_message.SetCollisionVelocity(velocity);
  collision_message.SetCollisionEventId(event_id);

  // Should not throw any exceptions
  collision_message.Validate();

  // Test with invalid mass - Validate() will throw
  collision_message.SetMass(-50.0);
  CHECK_THROWS(std::exception, collision_message.Validate());
  collision_message.SetMass(NAN);
  CHECK_THROWS(std::exception, collision_message.Validate());
  collision_message.SetMass(1000.0);  // Reset to valid

  // Test with invalid collision type - Validate() will throw
  collision_message.SetCollisionType(static_cast<CollisionType>(-1));
  CHECK_THROWS(std::exception, collision_message.Validate());
}

TEST(CollisionMessageTest, Builder) {
  Location location = LocationBuilder().SetX(5.0).SetY(10.0).SetZ(15.0).Build();

  Vector velocity = VectorBuilder().SetX(2.0).SetY(4.0).SetZ(6.0).Build();

  MusicEventId event_id =
      MusicEventIdBuilder()
          .SetSimulationAddress(
              SimulationAddressBuilder().SetSite(10).SetApplication(20).Build())
          .SetEventId(30)
          .Build();

  CollisionMessage collision_message =
      CollisionMessageBuilder()
          .SetMass(2000.0)
          .SetCollisionType(CollisionType::kOther)
          .SetCollisionLocation(location)
          .SetCollisionVelocity(velocity)
          .SetCollisionEventId(event_id)
          .Build();

  DOUBLES_EQUAL(2000.0, collision_message.GetMass(), kDoublePrecision);
  LONGS_EQUAL(static_cast<int>(CollisionType::kOther),
              static_cast<int>(collision_message.GetCollisionType()));
  DOUBLES_EQUAL(5.0, collision_message.GetCollisionLocation().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(10.0, collision_message.GetCollisionLocation().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(15.0, collision_message.GetCollisionLocation().GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.0, collision_message.GetCollisionVelocity().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(4.0, collision_message.GetCollisionVelocity().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(6.0, collision_message.GetCollisionVelocity().GetZ(),
                kDoublePrecision);
  LONGS_EQUAL(
      10,
      collision_message.GetCollisionEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(20, collision_message.GetCollisionEventId()
                      .GetSimulationAddress()
                      .GetApplication());
  LONGS_EQUAL(30, collision_message.GetCollisionEventId().GetEventId());
}

TEST(CollisionMessageTest, ToJson) {
  Location location = LocationBuilder().SetX(7.0).SetY(14.0).SetZ(21.0).Build();

  Vector velocity = VectorBuilder().SetX(3.0).SetY(6.0).SetZ(9.0).Build();

  MusicEventId event_id =
      MusicEventIdBuilder()
          .SetSimulationAddress(
              SimulationAddressBuilder().SetSite(11).SetApplication(22).Build())
          .SetEventId(33)
          .Build();

  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(1)
                                  .SetTimestamp(1234567890L)
                                  .Build();

  CollisionMessage collision_message =
      CollisionMessageBuilder()
          .SetMass(2500.0)
          .SetCollisionType(CollisionType::kElastic)
          .SetCollisionLocation(location)
          .SetCollisionVelocity(velocity)
          .SetCollisionEventId(event_id)
          .SetHeader(header)
          .SetType(MusicMessageType::kCollision)
          .Build();

  nlohmann::json j = collision_message.ToJson();

  // check mass
  DOUBLES_EQUAL(2500.0, j["mass"].get<double>(), kDoublePrecision);
  // check collision type
  LONGS_EQUAL(static_cast<int>(CollisionType::kElastic),
              j["collisionType"].get<int>());
  // Check location
  DOUBLES_EQUAL(7.0, j["location"]["x"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(14.0, j["location"]["y"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(21.0, j["location"]["z"].get<double>(), kDoublePrecision);
  // Check velocity
  DOUBLES_EQUAL(3.0, j["velocity"]["x"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(6.0, j["velocity"]["y"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(9.0, j["velocity"]["z"].get<double>(), kDoublePrecision);
  // Check event ID
  LONGS_EQUAL(11, j["eventID"]["siteID"].get<int>());
  LONGS_EQUAL(22, j["eventID"]["appID"].get<int>());
  LONGS_EQUAL(33, j["eventID"]["eventID"].get<int>());
  // Check header
  LONGS_EQUAL(1, j["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(1234567890L, j["header"]["timestamp"].get<long>());
}

TEST(CollisionMessageTest, FromJson) {
  nlohmann::json j = nlohmann::json::parse(R"(
{
    "collidingID": {
        "appID": 20,
        "entityID": 2,
        "siteID": 10
    },
    "collisionType": 1,
    "eventID": {
        "appID": 24,
        "eventID": 36,
        "siteID": 12
    },
    "header": {
        "exerciseID": 50,
        "timestamp": 1625894109000
    },
    "issuingID": {
        "appID": 30,
        "entityID": 3,
        "siteID": 15
    },
    "location": {
        "x": 8.0,
        "y": 16.0,
        "z": 24.0
    },
"mass": 3000.0,
    "velocity": {
        "x": 4.0,
        "y": 8.0,
        "z": 12.0
  }
    })");

  CollisionMessage collision_message = CollisionMessage::FromJson(j);

  // check colliding ID
  LONGS_EQUAL(10, collision_message.GetCollidingId().GetSite());
  LONGS_EQUAL(20, collision_message.GetCollidingId().GetApplication());
  LONGS_EQUAL(2, collision_message.GetCollidingId().GetEntityNumber());

  // Check collision type
  LONGS_EQUAL(static_cast<int>(CollisionType::kInElastic),
              static_cast<int>(collision_message.GetCollisionType()));

  // Check event ID
  LONGS_EQUAL(
      12,
      collision_message.GetCollisionEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(24, collision_message.GetCollisionEventId()
                      .GetSimulationAddress()
                      .GetApplication());
  LONGS_EQUAL(36, collision_message.GetCollisionEventId().GetEventId());

  // Check header
  LONGS_EQUAL(50, collision_message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1625894109000, collision_message.GetHeader().GetTimestamp());

  // Check issuing ID
  LONGS_EQUAL(15, collision_message.GetIssuingId().GetSite());
  LONGS_EQUAL(30, collision_message.GetIssuingId().GetApplication());
  LONGS_EQUAL(3, collision_message.GetIssuingId().GetEntityNumber());

  // Check location
  DOUBLES_EQUAL(8.0, collision_message.GetCollisionLocation().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(16.0, collision_message.GetCollisionLocation().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(24.0, collision_message.GetCollisionLocation().GetZ(),
                kDoublePrecision);

  // Check mass
  DOUBLES_EQUAL(3000.0, collision_message.GetMass(), kDoublePrecision);

  // Check velocity
  DOUBLES_EQUAL(4.0, collision_message.GetCollisionVelocity().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(8.0, collision_message.GetCollisionVelocity().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(12.0, collision_message.GetCollisionVelocity().GetZ(),
                kDoublePrecision);
}

TEST(CollisionMessageTest, ToString) {
  Location location = LocationBuilder().SetX(7.0).SetY(14.0).SetZ(21.0).Build();

  Vector velocity = VectorBuilder().SetX(3.0).SetY(6.0).SetZ(9.0).Build();

  MusicEventId event_id =
      MusicEventIdBuilder()
          .SetSimulationAddress(
              SimulationAddressBuilder().SetSite(11).SetApplication(22).Build())
          .SetEventId(33)
          .Build();

  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(1)
                                  .SetTimestamp(1234567890L)
                                  .Build();

  CollisionMessage collision_message =
      CollisionMessageBuilder()
          .SetMass(3500.0)
          .SetCollisionType(CollisionType::kOther)
          .SetCollisionLocation(location)
          .SetCollisionVelocity(velocity)
          .SetCollisionEventId(event_id)
          .SetHeader(header)
          .SetType(MusicMessageType::kCollision)
          .Build();

  std::string str = collision_message.ToString();

  // Verify the string contains expected content
  CHECK_TRUE(str.find("CollisionMessage") != std::string::npos);
  CHECK_TRUE(str.find("3500") != std::string::npos);
}
}  // namespace music::test