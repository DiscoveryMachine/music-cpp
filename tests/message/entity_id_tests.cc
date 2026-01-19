// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/entity_id.h"

namespace music::test {
TEST_GROUP(EntityIdTest){void setup(){
    // No setup needed
}

                         void teardown(){
                             // No teardown needed
                         }};

TEST(EntityIdTest, DefaultConstructor) {
  music::EntityId entity_id;
  CHECK_TRUE(std::numeric_limits<int>::min() == entity_id.GetSite());
  CHECK_TRUE(std::numeric_limits<int>::min() == entity_id.GetApplication());
  CHECK_TRUE(std::numeric_limits<int>::min() == entity_id.GetEntityNumber());
}

TEST(EntityIdTest, SettersAndGetters) {
  music::EntityId entity_id;
  entity_id.SetSite(100);
  entity_id.SetApplication(200);
  entity_id.SetEntityNumber(300);

  CHECK_EQUAL(100, entity_id.GetSite());
  CHECK_EQUAL(200, entity_id.GetApplication());
  CHECK_EQUAL(300, entity_id.GetEntityNumber());
}

TEST(EntityIdTest, Builder) {
  music::EntityIdBuilder builder;
  music::EntityId entity_id = builder.SetSite(10).SetApplication(20).SetEntityNumber(30).Build();

  CHECK_EQUAL(10, entity_id.GetSite());
  CHECK_EQUAL(20, entity_id.GetApplication());
  CHECK_EQUAL(30, entity_id.GetEntityNumber());
}

TEST(EntityIdTest, ToJson) {
  music::EntityId entity_id;
  entity_id.SetSite(123);
  entity_id.SetApplication(456);
  entity_id.SetEntityNumber(789);

  nlohmann::json j;
  to_json(j, entity_id);

  CHECK_EQUAL(123, j[music::SimulationAddress::kSiteJsonKey].get<int>());
  CHECK_EQUAL(456, j[music::SimulationAddress::kApplicationJsonKey].get<int>());
  CHECK_EQUAL(789, j[music::EntityId::kEntityNumberJsonKey].get<int>());
}

TEST(EntityIdTest, FromJson) {
  nlohmann::json j = {{music::SimulationAddress::kSiteJsonKey, 111},
                      {music::SimulationAddress::kApplicationJsonKey, 222},
                      {music::EntityId::kEntityNumberJsonKey, 333}};

  EntityId entity_id;
  from_json(j, entity_id);

  CHECK_EQUAL(111, entity_id.GetSite());
  CHECK_EQUAL(222, entity_id.GetApplication());
  CHECK_EQUAL(333, entity_id.GetEntityNumber());
}

TEST(EntityIdTest, JsonRoundTrip) {
  // Create an EntityId with specific values
  music::EntityId original;
  original.SetSite(999);
  original.SetApplication(888);
  original.SetEntityNumber(777);

  // Serialize to JSON
  nlohmann::json j;
  to_json(j, original);

  // Deserialize from JSON
  EntityId deserialized;
  from_json(j, deserialized);

  // Verify values match
  CHECK_EQUAL(999, deserialized.GetSite());
  CHECK_EQUAL(888, deserialized.GetApplication());
  CHECK_EQUAL(777, deserialized.GetEntityNumber());
}

TEST(EntityIdTest, ValidRangeValues) {
  EntityId entity_id;

  // Test boundary values (0-65535)
  entity_id.SetSite(0);
  entity_id.SetApplication(65535);
  entity_id.SetEntityNumber(32768);

  CHECK_EQUAL(0, entity_id.GetSite());
  CHECK_EQUAL(65535, entity_id.GetApplication());
  CHECK_EQUAL(32768, entity_id.GetEntityNumber());
}
} // namespace music::test
