// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "config/config.h"
#include "message/location.h"

namespace music::test {
TEST_GROUP(LocationTest){

};

TEST(LocationTest, DefaultConstructor) {
  Location location;
  CHECK_TRUE(std::isnan(location.GetX()));
  CHECK_TRUE(std::isnan(location.GetY()));
  CHECK_TRUE(std::isnan(location.GetZ()));
}

TEST(LocationTest, SettersAndGetters) {
  Location location;
  location.SetX(10.0);
  location.SetY(20.0);
  location.SetZ(30.0);

  DOUBLES_EQUAL(10.0, location.GetX(), kDoublePrecision);
  DOUBLES_EQUAL(20.0, location.GetY(), kDoublePrecision);
  DOUBLES_EQUAL(30.0, location.GetZ(), kDoublePrecision);
}

TEST(LocationTest, Builder) {
  LocationBuilder builder;
  Location location = builder.SetX(1.0).SetY(2.0).SetZ(3.0).Build();

  DOUBLES_EQUAL(1.0, location.GetX(), kDoublePrecision);
  DOUBLES_EQUAL(2.0, location.GetY(), kDoublePrecision);
  DOUBLES_EQUAL(3.0, location.GetZ(), kDoublePrecision);
}

TEST(LocationTest, Validate) {
  Location location = LocationBuilder().SetX(5.0).SetY(10.0).SetZ(15.0).Build();
  // Should not throw any exceptions
  location.Validate();

  // Test with invalid (NaN) X - Validate() will throw
  location = Location();  // Reset to default (all NaN)
  location.SetY(10.0);
  location.SetZ(15.0);
  // X is NaN, should throw
  CHECK_THROWS(std::exception, location.Validate());

  // Test with invalid (NaN) Y - Validate() will throw
  location = Location();
  location.SetX(5.0);
  location.SetZ(15.0);
  // Y is NaN, should throw
  CHECK_THROWS(std::exception, location.Validate());

  // Test with invalid (NaN) Z - Validate() will throw
  location = Location();
  location.SetX(5.0);
  location.SetY(10.0);
  // Z is NaN, should throw
  CHECK_THROWS(std::exception, location.Validate());

  // Test with all invalid (NaN) - Validate() will throw
  location = Location();
  CHECK_THROWS(std::exception, location.Validate());

  // Test with manual nan assignment
  location.SetX(std::numeric_limits<double>::quiet_NaN());
  location.SetY(10.0);
  location.SetZ(15.0);
  CHECK_THROWS(std::exception, location.Validate());

  // Set to all valid values again
  location.SetX(5.0);
  location.SetY(10.0);
  location.SetZ(15.0);

  // Should not throw any exceptions
  location.Validate();
}

TEST(LocationTest, ToJson) {
  Location location;
  location.SetX(12.34);
  location.SetY(56.78);
  location.SetZ(90.12);

  nlohmann::json j;
  to_json(j, location);

  DOUBLES_EQUAL(12.34, j["x"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(56.78, j["y"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(90.12, j["z"].get<double>(), kDoublePrecision);
}

TEST(LocationTest, FromJson) {
  nlohmann::json j = {{"x", 21.43}, {"y", 65.87}, {"z", 10.29}};

  Location location;
  from_json(j, location);

  DOUBLES_EQUAL(21.43, location.GetX(), kDoublePrecision);
  DOUBLES_EQUAL(65.87, location.GetY(), kDoublePrecision);
  DOUBLES_EQUAL(10.29, location.GetZ(), kDoublePrecision);
}

TEST(LocationTest, ToString) {
  Location location;
  location.SetX(7.89);
  location.SetY(1.23);
  location.SetZ(4.56);

  std::ostringstream os;
  os << location;

  std::string expected_json = location.ToJson().dump();
  std::string expected_output = "Location: " + expected_json;

  CHECK_EQUAL(expected_output, os.str());
}
}  // namespace music::test