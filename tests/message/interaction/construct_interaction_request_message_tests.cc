// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>

#include <exception>

#include "enum/interaction_type.h"
#include "message/entity_id.h"
#include "message/interaction/construct_interaction_request_message.h"
#include "message/music_message_header.h"
#include "message/music_request_message.h"

namespace music::test {

TEST_GROUP(ConstructInteractionRequestMessageTests) {
  MusicMessageHeader header;
  EntityId origin_id;
  EntityId receiver_id;
  ConstructInteractionRequestMessage test_mesg;
  nlohmann::json interaction_data;

  void setup() {
    header = MusicMessageHeaderBuilder()
                 .SetExerciseId(3)
                 .SetTimestamp(1234567890L)
                 .Build();

    origin_id.SetSite(1);
    origin_id.SetApplication(2);
    origin_id.SetEntityNumber(3);

    receiver_id.SetSite(4);
    receiver_id.SetApplication(5);
    receiver_id.SetEntityNumber(6);
    interaction_data = nlohmann::json{
        {ConstructInteractionRequestMessage::kInteractionId, 12345}};

    test_mesg = ConstructInteractionRequestMessageBuilder()
                    .SetHeader(header)
                    .SetOriginId(origin_id)
                    .SetReceiverId(receiver_id)
                    .SetRequestId(1)
                    .SetInteractionName("Test Interaction")
                    .SetInteractionType(InteractionType::kPause)
                    .SetInteractionData(interaction_data)
                    .Build();
  }

  void teardown() {}
};

TEST(ConstructInteractionRequestMessageTests, ValidationTest) {
  ConstructInteractionRequestMessage default_mesg;

  // A default-constructed ConstructInteractionRequestMessage begins in an
  // invalid state.
  CHECK_THROWS(std::exception, default_mesg.Validate());

  // The test message, as constructed, is well-formed and valid.
  test_mesg.Validate();

  test_mesg.SetInteractionName("");
  CHECK_THROWS(std::exception, test_mesg.Validate());

  setup();
  test_mesg.SetInteractionType(InteractionType::kUnassigned);
  CHECK_THROWS(std::exception, test_mesg.Validate());

  setup();
  test_mesg.SetInteractionData(nlohmann::json::object());
  CHECK_THROWS(std::exception, test_mesg.Validate());

  setup();
  const nlohmann::json &bad_json = nlohmann::json::object();
  test_mesg.SetInteractionData(bad_json);
  CHECK_THROWS(std::exception, test_mesg.Validate());
}

TEST(ConstructInteractionRequestMessageTests, ToJsonTest) {
  // Assumes well formed parent
  auto req_mesg_json = static_cast<MusicRequestMessage>(test_mesg).ToJson();

  const auto &interaction_name = "My test interaction";
  const auto &interaction_type = InteractionType::kResume;
  const auto &interaction_data = nlohmann::json{
      {ConstructInteractionRequestMessage::kInteractionId, 11235813}};

  req_mesg_json[ConstructInteractionRequestMessage::kInteractionNameJsonKey] =
      interaction_name;
  req_mesg_json[ConstructInteractionRequestMessage::kInteractionTypeJsonKey] =
      static_cast<int>(interaction_type);
  req_mesg_json[ConstructInteractionRequestMessage::kInteractionDataJsonKey] =
      interaction_data;

  test_mesg.SetInteractionName(interaction_name);
  test_mesg.SetInteractionType(interaction_type);
  test_mesg.SetInteractionData(interaction_data);

  STRCMP_EQUAL(req_mesg_json.dump().c_str(), test_mesg.ToJson().dump().c_str());
}

TEST(ConstructInteractionRequestMessageTests, FromJsonTest) {
  ConstructInteractionRequestMessage from_json =
      ConstructInteractionRequestMessage::FromJson(test_mesg.ToJson());

  STRCMP_EQUAL(test_mesg.GetInteractionName().c_str(),
               from_json.GetInteractionName().c_str());

  CHECK_TRUE(test_mesg.GetInteractionType() == from_json.GetInteractionType());
  STRCMP_EQUAL(test_mesg.GetInteractionData().dump().c_str(),
               from_json.GetInteractionData().dump().c_str());
}

}  // namespace music::test