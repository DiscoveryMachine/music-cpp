// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>

#include "message/event_id.h"
#include "message/simulation_address.h"

namespace music::test {
TEST_GROUP(EventIdTest){void setup(){
    // No setup needed
}

                        void teardown(){
                            // No teardown needed
                        }};

TEST(EventIdTest, DefaultConstructor) {
  music::MusicEventId event_id;
  LONGS_EQUAL(std::numeric_limits<int>::min(), event_id.GetEventId());
}

TEST(EventIdTest, SettersAndGetters) {
  music::MusicEventId event_id;
  event_id.SetSimulationAddress(
      SimulationAddressBuilder().SetSite(100).SetApplication(200).Build());
  event_id.SetEventId(300);

  LONGS_EQUAL(100, event_id.GetSimulationAddress().GetSite());
  LONGS_EQUAL(200, event_id.GetSimulationAddress().GetApplication());
  LONGS_EQUAL(300, event_id.GetEventId());

  SimulationAddress set_addr =
      SimulationAddressBuilder().SetSite(65535).SetApplication(6700).Build();
  event_id.SetSimulationAddress(set_addr);
  event_id.SetEventId(12345);

  CHECK_TRUE(set_addr == event_id.GetSimulationAddress());
  LONGS_EQUAL(12345, event_id.GetEventId());
}

TEST(EventIdTest, Builder) {
  music::MusicEventIdBuilder builder;
  SimulationAddress sim_addr =
      SimulationAddressBuilder().SetSite(10).SetApplication(20).Build();
  music::MusicEventId event_id =
      builder.SetSimulationAddress(sim_addr).SetEventId(30).Build();

  CHECK_TRUE(sim_addr == event_id.GetSimulationAddress());
  LONGS_EQUAL(30, event_id.GetEventId());
}

TEST(EventIdTest, Validate) {
  SimulationAddress sim_addr =
      SimulationAddressBuilder().SetSite(5).SetApplication(10).Build();
  music::MusicEventId event_id = music::MusicEventIdBuilder()
                                     .SetSimulationAddress(sim_addr)
                                     .SetEventId(15)
                                     .Build();
  // Should not throw any exceptions
  event_id.Validate();

  // Test with invalid simulation addresss - Validate() will throw
  event_id = music::MusicEventId();  // Reset to default
  event_id.SetEventId(15);
  CHECK_THROWS(std::exception, event_id.Validate());

  // Test with invalid event_id - Validate() will throw
  event_id = music::MusicEventId();
  event_id.SetSimulationAddress(sim_addr);
  CHECK_THROWS(std::exception, event_id.Validate());

  // Test with out-of-range event_id - Validate() will throw
  event_id = music::MusicEventId();
  event_id.SetSimulationAddress(sim_addr);
  event_id.SetEventId(65536);  // Out of range
  CHECK_THROWS(std::exception, event_id.Validate());

  // Test with valid values again - should not throw
  event_id = music::MusicEventIdBuilder()
                 .SetSimulationAddress(sim_addr)
                 .SetEventId(15000)
                 .Build();
  event_id.Validate();
}

// Test JSON serialization and deserialization
TEST(EventIdTest, ToJson) {
  SimulationAddress sim_addr =
      SimulationAddressBuilder().SetSite(123).SetApplication(456).Build();
  music::MusicEventId event_id;
  event_id.SetSimulationAddress(sim_addr);
  event_id.SetEventId(789);
  nlohmann::json j;
  to_json(j, event_id);
  LONGS_EQUAL(123, j[MusicEventId::kSiteIdKey].get<int>());
  LONGS_EQUAL(456, j[MusicEventId::kApplicationIdKey].get<int>());
  LONGS_EQUAL(789, j[MusicEventId::kEventIdKey].get<int>());
}

TEST(EventIdTest, FromJson) {
  SimulationAddress sim_addr =
      SimulationAddressBuilder().SetSite(123).SetApplication(456).Build();

  nlohmann::json j = nlohmann::json{{MusicEventId::kSiteIdKey, 123},
                                    {MusicEventId::kApplicationIdKey, 456},
                                    {MusicEventId::kEventIdKey, 333}};
  MusicEventId event_id;
  from_json(j, event_id);
  CHECK_TRUE(sim_addr == event_id.GetSimulationAddress());
  LONGS_EQUAL(333, event_id.GetEventId());
}
}  // namespace music::test
