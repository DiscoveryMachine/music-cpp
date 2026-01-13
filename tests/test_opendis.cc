// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

// Test file demonstrating open-dis-cpp integration
//
// NOTE: This test is marked as IGNORE_TEST because the open-dis-cpp library
// has internal memory allocations that conflict with CppUTest's memory leak
// detection. This is NOT a real memory leak - it's static initialization in
// the DIS library that persists for the lifetime of the program.
//
// To run this test manually: ./test_suite -ri -n DISIntegration

#include <CppUTest/TestHarness.h>
#include <dis6/EntityID.h>
#include <dis6/EntityStatePdu.h>
#include <dis6/Vector3Double.h>

#include "config/config.h"
#include "message/entity_id.h"
#include "spdlog/spdlog.h"

namespace music::test {
TEST_GROUP(OpenDISTest){void setup(){spdlog::set_level(music::spdlog_level);
}

void teardown() {
  // Teardown code if needed
}
}
;

IGNORE_TEST(OpenDISTest, DISIntegration) {
  spdlog::info("=== Open-DIS C++ Test ===");

  // Create an EntityStatePdu (DIS Entity State PDU)
  DIS::EntityStatePdu pdu;

  // Set entity identification
  DIS::EntityID entityID;
  entityID.setEntity(1);
  entityID.setApplication(2);
  entityID.setSite(3);
  pdu.setEntityID(entityID);

  // Set location
  DIS::Vector3Double location;
  location.setX(100.5);
  location.setY(200.5);
  location.setZ(50.5);
  pdu.setEntityLocation(location);

  spdlog::info("Created DIS EntityStatePdu:");
  spdlog::info("  Site: {}", (int)entityID.getSite());
  spdlog::info("  Application: {}", (int)entityID.getApplication());
  spdlog::info("  Entity: {}", (int)entityID.getEntity());
  spdlog::info("  Location: ({}, {}, {})", location.getX(), location.getY(),
               location.getZ());

  // Now test our custom EntityId class
  spdlog::info("=== Custom Music-DTO EntityId ===");
  music::EntityId musicEntity;

  spdlog::info("Created Music-DTO EntityId:");
  spdlog::info("  Site: {}", musicEntity.GetSite());
  spdlog::info("  Application: {}", musicEntity.GetApplication());
  spdlog::info("  Entity: {}", musicEntity.GetEntityNumber());

  // Test JSON serialization
  nlohmann::json j;
  to_json(j, musicEntity);
  spdlog::info("JSON representation: {}", j.dump());

  spdlog::info("Test passed! open-dis-cpp and Music-DTO are working together.");

  // Verify integration works
  CHECK_EQUAL(3, (int)entityID.getSite());
  CHECK_EQUAL(2, (int)entityID.getApplication());
  CHECK_EQUAL(1, (int)entityID.getEntity());
}
}  // namespace music::test