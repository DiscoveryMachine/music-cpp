// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>

#include <exception>
#include <stdexcept>

#include "message/entity_id.h"
#include "message/interaction/construct_interaction_response_message.h"
#include "message/music_message_header.h"
#include "message/music_response_message.h"

namespace music::test {

TEST_GROUP(ConstructInteractionResponseMessageTests) {
  MusicMessageHeader header;
  EntityId origin_id;
  EntityId receiver_id;
  ConstructInteractionResponseMessage test_mesg;

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

    const auto &optional_data =
        nlohmann::json{{"Optional data key", "Optional data value"}}.dump();

    test_mesg = ConstructInteractionResponseMessageBuilder()
                    .SetHeader(header)
                    .SetOriginId(origin_id)
                    .SetReceiverId(receiver_id)
                    .SetRequestId(1)
                    .SetRequestStatus(RequestStatus::kExecuting)
                    .SetOptionalData(optional_data)
                    .Build();
  }

  void teardown() {}
};

TEST(ConstructInteractionResponseMessageTests, ValidationTest) {
  ConstructInteractionResponseMessage default_mesg;

  // A default-constructed ConstructInteractionResponseMessage begins in an
  // invalid state.
  CHECK_THROWS(std::exception, default_mesg.Validate());

  // The test message, as constructed, is well-formed and valid.
  test_mesg.Validate();

  const nlohmann::json &bad_json = nlohmann::json::object();
  test_mesg.SetOptionalData(bad_json);
  CHECK_THROWS(std::runtime_error, test_mesg.Validate());
}

TEST(ConstructInteractionResponseMessageTests, ToJsonTest) {
  // Assumes well formed parent
  auto req_mesg_json = static_cast<MusicResponseMessage>(test_mesg).ToJson();

  const auto &optional_data =
      nlohmann::json{{"Optional data key", "Optional data value"}};

  req_mesg_json
      [ConstructInteractionResponseMessage::kInteractionOptionalDataJsonKey] =
          optional_data;

  test_mesg.SetOptionalData(optional_data);

  STRCMP_EQUAL(req_mesg_json.dump().c_str(), test_mesg.ToJson().dump().c_str());
}

TEST(ConstructInteractionResponseMessageTests, FromJsonTest) {
  ConstructInteractionResponseMessage from_json =
      ConstructInteractionResponseMessage::FromJson(test_mesg.ToJson());

  CHECK_TRUE(test_mesg.GetOptionalData().has_value() &&
             from_json.GetOptionalData().has_value() &&
             test_mesg.GetOptionalData().value() ==
                 from_json.GetOptionalData().value());
}

}  // namespace music::test