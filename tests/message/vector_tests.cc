// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

// Unit tests for the Vector class
#include <CppUTest/TestHarness.h>

#include "message/vector.h"

namespace music::test {
TEST_GROUP(MUSIC_VECTOR){
    // DEFINE ANY SETUP HERE
};

// Tests that validation FAILS when a Vector has un-initialized values
TEST(MUSIC_VECTOR, Test_SetNullParameters_ValidationFails) {
  bool failed_test = true;

  // X is null to begin with (should be the first thrown error)
  VectorBuilder vector = VectorBuilder();
  try {
    vector.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false;
  }
  if (failed_test) {
    FAIL("No Validation error was thrown when X was null.");
  }

  // Make X valid and non-null, Y is still null
  failed_test = true;
  vector.SetX(0.0);
  try {
    vector.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false;
  }
  if (failed_test) {
    FAIL("No Validation error was thrown when Y was null");
  }
  // Make Y valid and non-null, Z is still null
  failed_test = true;
  vector.SetY(0.0);
  try {
    vector.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false;
  }
  if (failed_test) {
    FAIL("No Validation error was thrown when Z was null");
  }
};

// Tests that validation passes when a Vector is provided with good data
TEST(MUSIC_VECTOR, Test_SetValidParameters_ValidationPasses) {
  VectorBuilder vector = VectorBuilder().SetX(1.0).SetY(2.0).SetZ(3.0);
  try {
    vector.Build().Validate();
  } catch (std::runtime_error err) {
    FAIL("A validation error was thrown for a valid vector definition");
  }
};

// Test that passes when from_json with valid json values is used
TEST(MUSIC_VECTOR, Test_FromJson_ValidationPasses) {
  nlohmann::json j = {{"x", 1.0}, {"y", 2.0}, {"z", 3.0}};
  Vector vector = Vector::FromJson(j);
  try {
    vector.Validate();
  } catch (std::runtime_error err) {
    FAIL(
        "A validation error was thrown for a valid vector definition from "
        "JSON");
  }
};

// Test that fails when from_json with invalid json values is used
TEST(MUSIC_VECTOR, Test_FromJson_InvalidValues_ValidationFails) {
  bool failed_test = true;
  nlohmann::json j = {{"x", NAN}, {"y", 2.0}, {"z", 3.0}};
  try {
    Vector vector = Vector::FromJson(j);
    vector.Validate();
  } catch (const std::runtime_error& err) {
    failed_test = false;
  }
  if (failed_test) {
    FAIL(
        "No Validation error was thrown for an invalid vector definition from "
        "JSON");
  }
};

// Test that to_json produces correct json output
TEST(MUSIC_VECTOR, Test_ToJson_CorrectOutput) {
  Vector vector = VectorBuilder().SetX(1.0).SetY(2.0).SetZ(3.0).Build();
  nlohmann::json j;
  to_json(j, vector);
  LONGS_EQUAL(1.0, j["x"].get<double>());
  LONGS_EQUAL(2.0, j["y"].get<double>());
  LONGS_EQUAL(3.0, j["z"].get<double>());
}
}  // namespace music::test