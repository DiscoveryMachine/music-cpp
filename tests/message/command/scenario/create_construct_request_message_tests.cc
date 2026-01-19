// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <cmath>
#include <string>

#include "config/config.h"
#include "enum/music_message_type.h"
#include "message/command/music_command_request_message.h"
#include "message/command/scenario/create_construct_request_message.h"
#include "message/entity_id.h"
#include "message/music_message_header.h"

namespace music::test {
TEST_GROUP(CreateConstructRequestMessageTest){
    // SETUP
};

TEST(CreateConstructRequestMessageTest, DefaultConstructor) {
  music::CreateConstructRequestMessage message;
  STRCMP_EQUAL("", message.GetConstructType().c_str());
  STRCMP_EQUAL("", message.GetConstructCallsign().c_str());
  CHECK_TRUE(message.GetConstructLocation().has_value() == false);
  CHECK_TRUE(message.GetConstructOrientation().has_value() == false);

  // CommandID must be this given value for this type of message
  LONGS_EQUAL(454013002L, message.GetCommandIdentifier());

  // Other values default to invalid values (-1 for unsigned integers)
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetOriginId().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetSite());
  LONGS_EQUAL(std::numeric_limits<long>::min(), message.GetRequestId());

  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetHeader().GetExerciseId());
  LONGS_EQUAL(std::numeric_limits<long>::min(),
              message.GetHeader().GetTimestamp());
  LONGS_EQUAL(
      static_cast<int>(music::MusicMessageType::kCreateConstructRequest),
      static_cast<int>(message.GetType()));
}

TEST(CreateConstructRequestMessageTest, SettersAndGetters) {
  music::CreateConstructRequestMessage message;
  message.SetConstructType("TypeA");
  message.SetConstructCallsign("CallsignA");
  music::Location loc =
      music::LocationBuilder().SetX(1.0).SetY(2.0).SetZ(3.0).Build();
  message.SetConstructLocation(loc);
  music::Orientation ori =
      music::OrientationBuilder().SetPhi(0.1).SetTheta(0.2).SetPsi(0.3).Build();
  message.SetConstructOrientation(ori);

  STRCMP_EQUAL("TypeA", message.GetConstructType().c_str());
  STRCMP_EQUAL("CallsignA", message.GetConstructCallsign().c_str());
  DOUBLES_EQUAL(1.0, message.GetConstructLocation().value().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.0, message.GetConstructLocation().value().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(3.0, message.GetConstructLocation().value().GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.1, message.GetConstructOrientation().value().GetPhi(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.2, message.GetConstructOrientation().value().GetTheta(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.3, message.GetConstructOrientation().value().GetPsi(),
                kDoublePrecision);
}

TEST(CreateConstructRequestMessageTest, Validate) {
  music::CreateConstructRequestMessage message;
  CHECK_THROWS(std::exception, message.Validate());
  message.SetConstructType("TypeB");
  message.SetConstructCallsign("CallsignB");
  message.SetConstructLocation(
      music::LocationBuilder().SetX(4.0).SetY(5.0).SetZ(6.0).Build());
  message.SetConstructOrientation(music::OrientationBuilder()
                                      .SetPhi(0.12)
                                      .SetTheta(0.23)
                                      .SetPsi(0.34)
                                      .Build());
  message.SetOriginId(music::EntityIdBuilder()
                          .SetApplication(1)
                          .SetEntityNumber(0)
                          .SetSite(3)
                          .Build());
  message.SetReceiverId(music::EntityIdBuilder()
                            .SetApplication(9)
                            .SetEntityNumber(0)
                            .SetSite(7)
                            .Build());
  message.SetRequestId(12345);
  message.SetHeader(music::MusicMessageHeaderBuilder()
                        .SetExerciseId(123)
                        .SetTimestamp(1234567890123L)
                        .Build());
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        "Validation should not throw an exception after setting all required "
        "fields");
  }
}

TEST(CreateConstructRequestMessageTest, Builder) {
  music::CreateConstructRequestMessage message =
      music::CreateConstructRequestMessageBuilder()
          .SetConstructType("TypeC")
          .SetConstructCallsign("CallsignC")
          .SetConstructLocation(
              music::LocationBuilder().SetX(7.0).SetY(8.0).SetZ(9.0).Build())
          .SetConstructOrientation(music::OrientationBuilder()
                                       .SetPhi(0.21)
                                       .SetTheta(0.32)
                                       .SetPsi(0.43)
                                       .Build())
          .SetOriginId(music::EntityIdBuilder()
                           .SetApplication(4)
                           .SetEntityNumber(5)
                           .SetSite(6)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetApplication(10)
                             .SetEntityNumber(11)
                             .SetSite(12)
                             .Build())
          .SetRequestId(67890)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(123)
                         .SetTimestamp(9876543210987L)
                         .Build())
          .Build();
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        "Validation should not throw an exception after building a valid "
        "message");
  }

  // Verify each of the fields set by the build
  STRCMP_EQUAL("TypeC", message.GetConstructType().c_str());
  STRCMP_EQUAL("CallsignC", message.GetConstructCallsign().c_str());
  DOUBLES_EQUAL(7.0, message.GetConstructLocation().value().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(8.0, message.GetConstructLocation().value().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(9.0, message.GetConstructLocation().value().GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.21, message.GetConstructOrientation().value().GetPhi(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.32, message.GetConstructOrientation().value().GetTheta(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.43, message.GetConstructOrientation().value().GetPsi(),
                kDoublePrecision);
  LONGS_EQUAL(4, message.GetOriginId().GetApplication());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(6, message.GetOriginId().GetSite());
  LONGS_EQUAL(10, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());
  LONGS_EQUAL(67890, message.GetRequestId());
  LONGS_EQUAL(123, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(9876543210987L, message.GetHeader().GetTimestamp());
}

TEST(CreateConstructRequestMessageTest, ToJson) {
  music::CreateConstructRequestMessage message =
      music::CreateConstructRequestMessageBuilder()
          .SetConstructType("TypeD")
          .SetConstructCallsign("CallsignD")
          .SetConstructLocation(music::LocationBuilder()
                                    .SetX(111.0)
                                    .SetY(222.0)
                                    .SetZ(333.0)
                                    .Build())
          .SetConstructOrientation(music::OrientationBuilder()
                                       .SetPhi(0.45)
                                       .SetTheta(0.56)
                                       .SetPsi(-0.67)
                                       .Build())
          .SetOriginId(music::EntityIdBuilder()
                           .SetApplication(7)
                           .SetEntityNumber(8)
                           .SetSite(9)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetApplication(13)
                             .SetEntityNumber(14)
                             .SetSite(15)
                             .Build())
          .SetRequestId(67890)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(128)
                         .SetTimestamp(9876543210123L)
                         .Build())
          .Build();
  nlohmann::json json = message.ToJson();

  STRCMP_EQUAL("TypeD",
               json[music::CreateConstructRequestMessage::kConstructTypeKey]
                   .get<std::string>()
                   .c_str());
  STRCMP_EQUAL("CallsignD",
               json[music::CreateConstructRequestMessage::kConstructCallsignKey]
                   .get<std::string>()
                   .c_str());
  DOUBLES_EQUAL(
      111.0,
      json[music::CreateConstructRequestMessage::kConstructLocationKey]
          [music::Location::kXKey]
              .get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      222.0,
      json[music::CreateConstructRequestMessage::kConstructLocationKey]
          [music::Location::kYKey]
              .get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      333.0,
      json[music::CreateConstructRequestMessage::kConstructLocationKey]
          [music::Location::kZKey]
              .get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      0.45,
      json[music::CreateConstructRequestMessage::kConstructOrientationKey]
          [music::Orientation::kPhiKey]
              .get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      0.56,
      json[music::CreateConstructRequestMessage::kConstructOrientationKey]
          [music::Orientation::kThetaKey]
              .get<double>(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      -0.67,
      json[music::CreateConstructRequestMessage::kConstructOrientationKey]
          [music::Orientation::kPsiKey]
              .get<double>(),
      kDoublePrecision);
  LONGS_EQUAL(7, json[music::MusicRequestMessage::kOriginIdKey]
                     [music::SimulationAddress::kApplicationJsonKey]
                         .get<long>());
  LONGS_EQUAL(0, json[music::MusicRequestMessage::kOriginIdKey]
                     [music::EntityId::kEntityNumberJsonKey]
                         .get<long>());
  LONGS_EQUAL(9, json[music::MusicRequestMessage::kOriginIdKey]
                     [music::SimulationAddress::kSiteJsonKey]
                         .get<long>());
  LONGS_EQUAL(13, json[music::MusicRequestMessage::kReceiverIdKey]
                      [music::SimulationAddress::kApplicationJsonKey]
                          .get<long>());
  LONGS_EQUAL(0, json[music::MusicRequestMessage::kReceiverIdKey]
                     [music::EntityId::kEntityNumberJsonKey]
                         .get<long>());
  LONGS_EQUAL(15, json[music::MusicRequestMessage::kReceiverIdKey]
                      [music::SimulationAddress::kSiteJsonKey]
                          .get<long>());
  LONGS_EQUAL(67890,
              json[music::MusicRequestMessage::kRequestIdKey].get<long>());
  LONGS_EQUAL(128, json[music::MusicMessage::kHeaderJsonKey]
                       [music::MusicMessageHeader::kExerciseIdKey]
                           .get<long>());
  LONGS_EQUAL(9876543210123L, json[music::MusicMessage::kHeaderJsonKey]
                                  [music::MusicMessageHeader::kTimestampKey]
                                      .get<long>());
}

TEST(CreateConstructRequestMessageTest, FromJson) {
  nlohmann::json json = {
      {music::MusicCommandRequestMessage::kCommandIdKey,
       music::CreateConstructRequestMessage::kCommandIdentifier},
      {music::CreateConstructRequestMessage::kConstructTypeKey, "TypeE"},
      {music::CreateConstructRequestMessage::kConstructCallsignKey,
       "CallsignE"},
      {music::CreateConstructRequestMessage::kConstructLocationKey,
       {{music::Location::kXKey, 123.0},
        {music::Location::kYKey, 456.0},
        {music::Location::kZKey, 789.0}}},
      {music::CreateConstructRequestMessage::kConstructOrientationKey,
       {{music::Orientation::kPhiKey, 0.12},
        {music::Orientation::kThetaKey, .21},
        {music::Orientation::kPsiKey, -0.11}}},
      {music::MusicRequestMessage::kOriginIdKey,
       {{music::SimulationAddress::kApplicationJsonKey, 7},
        {music::EntityId::kEntityNumberJsonKey, 0},
        {music::SimulationAddress::kSiteJsonKey, 9}}},
      {music::MusicRequestMessage::kReceiverIdKey,
       {{music::SimulationAddress::kApplicationJsonKey, 13},
        {music::EntityId::kEntityNumberJsonKey, 0},
        {music::SimulationAddress::kSiteJsonKey, 15}}},
      {music::MusicRequestMessage::kRequestIdKey, 123456},
      {music::MusicMessage::kHeaderJsonKey,
       {{music::MusicMessageHeader::kExerciseIdKey, 200},
        {music::MusicMessageHeader::kTimestampKey, 123321456654789L}}},
      {music::MusicMessage::kTypeJsonKey,
       music::MusicMessageType::kCreateConstructRequest}};

  CreateConstructRequestMessage message;
  try {
    message = CreateConstructRequestMessage::FromJson(json);
  } catch (const std::exception &e) {
    FAIL((std::string("FromJson should not throw an exception: ") + e.what())
             .c_str());
  }

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception: ") + e.what())
             .c_str());
  }
}

}  // namespace music::test