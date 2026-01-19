// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/dead_reckoning_algorithm_type.h"
#include "message/Vector.h"
#include "message/construct/dead_reckoning.h"
#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>

const double DOUBLE_TOLERANCE = 0.001;

namespace music::test {
TEST_GROUP(MusicDeadReckoningTest){
    // NO-OP
};

TEST(MusicDeadReckoningTest, Test_InvalidDefaultData_ThrowsValidationError) {
  DeadReckoningBuilder builder = DeadReckoningBuilder();
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());
  builder.SetAngularVelocity(VectorBuilder().SetX(1).SetY(1).SetZ(1).Build());

  CHECK_THROWS(std::runtime_error, builder.Build().Validate());
  builder.SetLinearAcceleration(VectorBuilder().SetX(1).SetY(1).SetZ(1).Build());

  // Should have all valid data now- fail if the validate finds an issue
  try {
    { builder.Build().Validate(); }
  } catch (std::runtime_error err) {
    FAIL("music::DeadReckoning threw a validation error with all valid data!");
  }
}

TEST(MusicDeadReckoningTest, Test_SetDataWithBuilder_BuiltObjectValuesMatch) {
  DeadReckoningBuilder builder =
      DeadReckoningBuilder()
          .SetAngularVelocity(VectorBuilder().SetX(1).SetY(2).SetZ(3).Build())
          .SetLinearAcceleration(VectorBuilder().SetX(4).SetY(5).SetZ(6).Build())
          .SetAlgorithmType(DeadReckoningAlgorithmType::kDrm_R_V_B);

  DeadReckoning final = builder.Build();
  // Angular Velocity matches
  DOUBLES_EQUAL(1, final.GetAngularVelocity().GetX(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(2, final.GetAngularVelocity().GetY(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(3, final.GetAngularVelocity().GetZ(), DOUBLE_TOLERANCE);

  // Linear Accel. matches
  DOUBLES_EQUAL(4, final.GetLinearAcceleration().GetX(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(5, final.GetLinearAcceleration().GetY(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(6, final.GetLinearAcceleration().GetZ(), DOUBLE_TOLERANCE);

  // Algo type matches
  CHECK_EQUAL(static_cast<int>(DeadReckoningAlgorithmType::kDrm_R_V_B),
              static_cast<int>(final.GetDeadReckAlgorithm()));
}

TEST(MusicDeadReckoningTest, Test_ConstructFromJson_FinalValuesMatchJson) {

  // Setup: Assumes the to_json works (as validated in other tests)
  DeadReckoningBuilder builder =
      DeadReckoningBuilder()
          .SetAngularVelocity(VectorBuilder().SetX(1).SetY(2).SetZ(3).Build())
          .SetLinearAcceleration(VectorBuilder().SetX(4).SetY(5).SetZ(6).Build())
          .SetAlgorithmType(DeadReckoningAlgorithmType::kDrm_R_V_B);
  DeadReckoning original = builder.Build();
  original.Validate(); // Make sure it's valid for the purpose of this test
  nlohmann::json jsn;
  nlohmann::to_json(jsn, original);

  // Parse and validate the damage
  DeadReckoning parsed = DeadReckoning();
  from_json(jsn, parsed);

  // Angular Velocity matches
  DOUBLES_EQUAL(original.GetAngularVelocity().GetX(), parsed.GetAngularVelocity().GetX(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(original.GetAngularVelocity().GetY(), parsed.GetAngularVelocity().GetY(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(original.GetAngularVelocity().GetZ(), parsed.GetAngularVelocity().GetZ(),
                DOUBLE_TOLERANCE);

  // Linear Accel. matches
  DOUBLES_EQUAL(original.GetLinearAcceleration().GetX(), parsed.GetLinearAcceleration().GetX(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(original.GetLinearAcceleration().GetY(), parsed.GetLinearAcceleration().GetY(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(original.GetLinearAcceleration().GetZ(), parsed.GetLinearAcceleration().GetZ(),
                DOUBLE_TOLERANCE);

  // Algo type matches
  CHECK_EQUAL(static_cast<int>(original.GetDeadReckAlgorithm()),
              static_cast<int>(parsed.GetDeadReckAlgorithm()));

  // Fail if the parsed object is invalid
  try {
    { parsed.Validate(); }
  } catch (std::runtime_error err) {
    FAIL("music::Damage parsed from JSON to an invalid object!");
  }
}

TEST(MusicDeadReckoningTest, Test_ConvertToJson_JsonValuesMatchObject) {
  DeadReckoningBuilder builder =
      DeadReckoningBuilder()
          .SetAngularVelocity(VectorBuilder().SetX(1).SetY(2).SetZ(3).Build())
          .SetLinearAcceleration(VectorBuilder().SetX(4).SetY(5).SetZ(6).Build())
          .SetAlgorithmType(DeadReckoningAlgorithmType::kDrm_R_V_B);

  nlohmann::json final_json;
  to_json(final_json, builder.Build());

  DOUBLES_EQUAL(final_json[DeadReckoning::kAngularVelocityKey][Vector::kXKey].get<float>(), 1,
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(final_json[DeadReckoning::kAngularVelocityKey][Vector::kYKey].get<float>(), 2,
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(final_json[DeadReckoning::kAngularVelocityKey][Vector::kZKey].get<float>(), 3,
                DOUBLE_TOLERANCE);

  DOUBLES_EQUAL(final_json[DeadReckoning::kLinearAccelerationkey][Vector::kXKey].get<float>(), 4,
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(final_json[DeadReckoning::kLinearAccelerationkey][Vector::kYKey].get<float>(), 5,
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(final_json[DeadReckoning::kLinearAccelerationkey][Vector::kZKey].get<float>(), 6,
                DOUBLE_TOLERANCE);

  CHECK_EQUAL(static_cast<int>(
                  final_json[DeadReckoning::kAlgorithmTypeKey].get<DeadReckoningAlgorithmType>()),
              static_cast<int>(DeadReckoningAlgorithmType::kDrm_R_V_B));
}

} // namespace music::test