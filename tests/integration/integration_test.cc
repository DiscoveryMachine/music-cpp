// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <spdlog/spdlog.h>

#include "dis/dis_music_endpoint.h"
#include "message/collision/collision_message.h"
#include "message/command/constructcontrol/remove_construct_message.h"
#include "message/command/constructcontrol/stop_construct_message.h"
#include "message/command/entity/transfer_construct_id_message.h"
#include "message/command/playercontrol/control_initiated_message.h"
#include "message/command/playercontrol/control_released_message.h"
#include "message/command/scenario/create_construct_request_message.h"
#include "message/command/scenario/create_construct_response_message.h"
#include "message/command/scenario/create_environment_request_message.h"
#include "message/command/scenario/create_environment_response_message.h"
#include "message/command/scenario/finalize_scenario_request_message.h"
#include "message/command/scenario/finalize_scenario_response_message.h"
#include "message/command/scenario/parameterize_construct_request_message.h"
#include "message/command/scenario/parameterize_construct_response_message.h"
#include "message/command/scenario/scenario_start_request_message.h"
#include "message/command/simulation/display_messages_message.h"
#include "message/command/time/request_simulation_time_message.h"
#include "message/command/time/set_simulation_time_message.h"
#include "message/command/time/simulation_time_message.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/state_field_message.h"
#include "message/control/construct_control_request_message.h"
#include "message/control/construct_control_response_message.h"
#include "message/control/construct_primary_control_request_message.h"
#include "message/control/construct_primary_control_response_message.h"
#include "message/control/control_granted_message.h"
#include "message/control/control_lost_message.h"
#include "message/control/control_reclamation_message.h"
#include "message/control/control_regained_message.h"
#include "message/control/control_relinquished_message.h"
#include "message/control/control_transfer_granted_message.h"
#include "message/control/control_transfer_request_message.h"
#include "message/control/control_transfer_response_message.h"
#include "message/control/primary_control_granted_message.h"
#include "message/control/primary_control_relinquished_message.h"
#include "message/control/set_current_controller_message.h"
#include "message/event/event_message.h"
#include "message/interaction/construct_interaction_request_message.h"
#include "message/interaction/construct_interaction_response_message.h"
#include "message/munition/detonation_message.h"
#include "message/munition/fire_message.h"
#include "message/perception/perception_data_message.h"
#include "message/simulation_address.h"
#include "message/waypoint/waypoint_data_message.h"

using namespace music;

CollisionMessage BuildCollisionMessage() {
  return CollisionMessage::FromJson(nlohmann::json::parse(R"(
{
    "collidingID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "collisionType": 0,
    "eventID": {
        "appID": 50,
        "eventID": 3,
        "siteID": 300
    },
    "header": {
        "exerciseID": 2,
        "timestamp": 1767724638340
    },
    "issuingID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "location": {
        "x": 123,
        "y": 90.2,
        "z": 0.23
    },
    "mass": 2000.0,
    "velocity": {
        "x": 123,
        "y": 90.2,
        "z": 0.23
    }
}

)"));
}

ConstructDataMessage BuildConstructDataMessage() {
  //   return ConstructDataMessageBuilder()
  //       .SetHeader(MusicMessageHeaderBuilder()
  //                      .SetExerciseId(5)
  //                      .SetTimestamp(123456789L)
  //                      .Build())
  //       .SetOriginId(EntityIdBuilder()
  //                        .SetSite(1)
  //                        .SetApplication(2)
  //                        .SetEntityNumber(3)
  //                        .Build())
  //       .SetPrimaryControllerId(EntityIdBuilder()
  //                                   .SetSite(4)
  //                                   .SetApplication(5)
  //                                   .SetEntityNumber(6)
  //                                   .Build())
  //       .SetCurrentControllerId(EntityIdBuilder()
  //                                   .SetSite(7)
  //                                   .SetApplication(8)
  //                                   .SetEntityNumber(9)
  //                                   .Build())
  //       .SetConstructInformationRecord(
  //           ConstructInformationRecordBuilder()
  //               .SetConstructCategoryType(ConstructCategoryType::kAggregate)
  //               .SetConstructRenderType(ConstructRenderType::kRenderedPhysical)
  //               .Build())
  //       .SetForce(Force::kOpposing)
  //       .SetEntityType(EntityTypeBuilder()
  //                          .SetKind(1)
  //                          .SetDomain(2)
  //                          .SetCountry(225)
  //                          .SetCategory(3)
  //                          .SetSubcategory(4)
  //                          .SetSpecific(5)
  //                          .SetExtra(6)
  //                          .Build())
  //       .SetLocation(LocationBuilder().SetX(1.0).SetY(2.0).SetZ(3.0).Build())
  //       .SetOrientation(
  //           OrientationBuilder().SetPhi(0.1).SetTheta(0.2).SetPsi(0.3).Build())
  //       .SetVelocity(VectorBuilder().SetX(1.0).SetY(2.0).SetZ(3.0).Build())
  //       .SetDeadReckoning(
  //           DeadReckoningBuilder()
  //               .SetAlgorithmType(DeadReckoningAlgorithmType::kStatic)
  //               .SetAngularVelocity(
  //                   VectorBuilder().SetX(4.0).SetY(5.0).SetZ(6.0).Build())
  //               .SetLinearAcceleration(
  //                   VectorBuilder().SetX(7).SetY(8).SetZ(9).Build())
  //               .Build())
  //       .SetDamage(MusicDamageBuilder()
  //                      .SetCatastrophicDamage(0.1)
  //                      .SetMobilityDamage(0.2)
  //                      .SetIsFirepowerDisabled(0)
  //                      .Build())
  //       .SetCallsign("Test")
  //       .SetConstructName("Test")
  //       .SetInteractionRecord("land")
  //       .Build();
  return ConstructDataMessage::FromJson(nlohmann::json::parse(R"(
  {
      "callsign": "FOX55",
      "constructInformationRecord": {
          "constructRender": 5,
          "constructType": 2
      },
      "constructName":
      "com.discoverymachine.Entity.AbstractVehicle.AbstractAircraft.AbstractFixedWingAircraft.F16AEntity",
      "currentControllerID": {
          "appID": 50,
          "entityID": 5,
          "siteID": 300
      },
      "damageRecord": {
          "catastrophicDamage": 0.0,
          "firepowerDisabled": false,
          "mobilityDamage": 0.20000000298023224
      },
      "deadReck": {
          "angularVelocity": {
              "x": 0.03400000184774399,
              "y": 0.020999999716877937,
              "z": 0.017000000923871994
          },
          "deadReckAlgorithm": 2,
          "linearAcceleration": {
              "x": 21.149999618530273,
              "y": 1.850000023841858,
              "z": -2.0
          }
      },
      "entityType": {
          "category": 1,
          "country": 225,
          "domain": 2,
          "extra": 0,
          "kind": 1,
          "specific": 1,
          "subcategory": 3
      },
      "force": 1,
      "header": {
          "exerciseID": 2,
          "timestamp": 1767724722296
      },
      "interactionRecord": "land, fireWeaponAtTarget, fireWeaponAtLocation",
      "location": {
          "x": 21841.15,
          "y": 154787.85,
          "z": 1533021.0
      },
      "orientation": {
          "phi": 5.75959,
          "psi": 0.785398,
          "theta": 0.2356194
      },
      "originID": {
          "appID": 50,
          "entityID": 3,
          "siteID": 300
      },
      "primaryControllerID": {
          "appID": 50,
          "entityID": 4,
          "siteID": 300
      },
      "velocity": {
          "x": 75.8499984741211,
          "y": 21.149999618530273,
          "z": 10.0
      }
  }
      )"));
}

StateFieldMessage BuildStateFieldMessage() {
  return StateFieldMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 3,
        "timestamp": 1767724770974
    },
    "originID": {
        "appID": 50,
        "entityID": 4,
        "siteID": 300
    },
    "stateData": {
        "airspeedIndicator": 120,
        "altitudeGaugeValue": 580.3,
        "altitudeIndicator": 3,
        "currentGoal": "Land",
        "flapsValue": 2,
        "nextWaypointLocation": {
            "x": 21841.15,
            "y": 154787.85,
            "z": 0.0
        },
        "verticalSpeedIndicatorValue": -1.1
    }
}
)"));
}

PerceptionDataMessage BuildPerceptionDataMessage() {
  return PerceptionDataMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 4,
        "timestamp": 1767724847692
    },
    "numPerceptions": 1,
    "originID": {
        "appID": 17,
        "entityID": 16,
        "siteID": 18
    },
    "perceptionRecords": [
        {
            "accuracy": 0.999,
            "altitudeError": 50,
            "bearingFactor": 1,
            "entityType": {
                "category": 1,
                "country": 222,
                "domain": 2,
                "extra": 0,
                "kind": 1,
                "specific": 1,
                "subcategory": 4
            },
            "force": 2,
            "perceptionID": {
                "appID": 50,
                "entityID": 8,
                "siteID": 300
            },
            "rangeError": 30,
            "system": 2
        }
    ]
}
)"));
}

WaypointDataMessage BuildWaypointDataMessage() {
  return WaypointDataMessage::FromJson(nlohmann::json::parse(R"(
{
    "currentWaypointIndex": 2,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767724879278
    },
    "numWaypoints": 3,
    "originID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "startOrAppend": true,
    "waypointRecords": [
        {
            "arrivalTimeError": 0,
            "estimatedArrivalTime": 0,
            "worldCoordinateRecord": {
                "x": 21841.15,
                "y": 154787.85,
                "z": 1533021.0
            }
        },
        {
            "arrivalTimeError": 60,
            "estimatedArrivalTime": -672136704,
            "worldCoordinateRecord": {
                "x": 21843.47,
                "y": 154787.41,
                "z": 1533021.0
            }
        },
        {
            "arrivalTimeError": 60,
            "estimatedArrivalTime": -671963772,
            "worldCoordinateRecord": {
                "x": 21845.05,
                "y": 154786.9,
                "z": 1533021.0
            }
        }
    ]
}
  )"));
}

EventMessage BuildEventMessage() {
  return EventMessage::FromJson(nlohmann::json::parse(R"(
{
    "eventData": {
        "location": {
            "x": 21845.05,
            "y": 154786.9,
            "z": 1533021.0
        }
    },
    "eventType": "com.discoverymachine.event.PlayerPickedUp",
    "header": {
        "exerciseID": 2,
        "timestamp": 1767724905979
    },
    "originID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    }
}
  )"));
}

FireMessage BuildFireMessage() {
  return FireMessage::FromJson(nlohmann::json::parse(R"(
{
    "eventID": {
        "appID": 50,
        "eventID": 3,
        "siteID": 300
    },
    "fuse": 0,
    "header": {
        "exerciseID": 2,
        "timestamp": 1605645402861
    },
    "location": {
        "x": 123.0,
        "y": 90.2,
        "z": 0.23
    },
    "munition": {
        "category": 1,
        "country": 222,
        "domain": 2,
        "extra": 0,
        "kind": 1,
        "specific": 1,
        "subcategory": 4
    },
    "munitionID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "quantity": 0,
    "rangeToTarget": 0.0,
    "rate": 0,
    "shooterID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "targetID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "velocity": {
        "x": 123.0,
        "y": 90.19999694824219,
        "z": 0.23000000417232513
    },
    "warhead": 0
}
  )"));
}

DetonationMessage BuildDetonationMessage() {
  return DetonationMessage::FromJson(nlohmann::json::parse(R"(
{
    "eventID": {
        "appID": 50,
        "eventID": 3,
        "siteID": 300
    },
    "fuse": 0,
    "header": {
        "exerciseID": 2,
        "timestamp": 1605645402861
    },
    "location": {
        "x": 123.0,
        "y": 90.2,
        "z": 0.23
    },
    "munition": {
        "category": 1,
        "country": 222,
        "domain": 2,
        "extra": 0,
        "kind": 1,
        "specific": 1,
        "subcategory": 4
    },
    "munitionID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "quantity": 0,
    "rate": 0,
    "result": 0,
    "shooterID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "targetID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "velocity": {
        "x": 123.0,
        "y": 90.19999694824219,
        "z": 0.23000000417232513
    },
    "warhead": 0
}
  )"));
}

RemoveConstructMessage BuildRemoveConstructMessage() {
  return RemoveConstructMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454007002,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767724928195
    },
    "originID": {
        "appID": 30,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 65535,
        "entityID": 0,
        "siteID": 65535
    },
    "removedConstruct": {
        "appID": 30,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 12345
}
  )"));
}

StopConstructMessage BuildStopConstructMessage() {
  return StopConstructMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454007001,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767725408927
    },
    "originID": {
        "appID": 0,
        "entityID": 300,
        "siteID": 30
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 67890
}
)"));
}

TransferConstructIdMessage BuildTransferConstructIdMessage() {
  // return TransferConstructIdMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767726217643)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(13579)
  //     .SetOldId(EntityIdBuilder()
  //                   .SetSite(300)
  //                   .SetApplication(50)
  //                   .SetEntityNumber(3)
  //                   .Build())
  //     .SetNewId(EntityIdBuilder()
  //                   .SetSite(300)
  //                   .SetApplication(50)
  //                   .SetEntityNumber(8)
  //                   .Build())
  //     .Build();
  return TransferConstructIdMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454000002,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767726217643
    },
    "newID": {
        "appID": 50,
        "entityID": 8,
        "siteID": 300
    },
    "oldID": {
        "appID": 50,
        "entityID": 3,
        "siteID": 300
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 13579
}
)"));
}

ControlInitiatedMessage BuildControlInitiatedMessage() {
  // return ControlInitiatedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767726760033)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(24680)
  //     .Build();

  return ControlInitiatedMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454999001,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767726760033
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 24680
}
  )"));
}

ControlReleasedMessage BuildControlReleasedMessage() {
  // return ControlReleasedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767726801264)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(11223)
  //     .Build();

  return ControlReleasedMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454999002,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767726801264
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 11223
}
)"));
}

CreateConstructRequestMessage BuildCreateConstructRequestMessage() {
  // return CreateConstructRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767727046550)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(33445)
  //     .SetConstructCallsign("MERCHANT01")
  //     .SetConstructType("com.discoverymachine.entity.Merchant")
  //     .SetConstructLocation(
  //         LocationBuilder().SetX(123).SetY(90.2).SetZ(0.23).Build())
  //     .SetConstructOrientation(
  //         OrientationBuilder().SetPhi(0.23).SetTheta(1.23).SetPsi(.902).Build())
  //     .Build();
  return CreateConstructRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013002,
    "constructCallsign": "MERCHANT01",
    "constructLocation": {
        "x": 123.0,
        "y": 90.2,
        "z": 0.23
    },
    "constructOrientation": {
        "phi": 0.23,
        "psi": 0.902,
        "theta": 1.23
    },
    "constructType": "com.discoverymachine.entity.Merchant",
    "header": {
        "exerciseID": 2,
        "timestamp": 1767727046550
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 33445
}
)"));
}

CreateConstructResponseMessage BuildCreateConstructResponseMessage() {
  // return CreateConstructResponseMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767727593564)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetConstructId(EntityIdBuilder()
  //                         .SetSite(300)
  //                         .SetApplication(50)
  //                         .SetEntityNumber(10)
  //                         .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(33445)
  //     .SetRequestStatus(RequestStatus::kComplete)
  //     .Build();
  return CreateConstructResponseMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013003,
    "constructID": {
        "appID": 50,
        "entityID": 10,
        "siteID": 300
    },
    "header": {
        "exerciseID": 2,
        "timestamp": 1767727593564
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 33445,
    "status": 4
}
)"));
}

CreateEnvironmentRequestMessage BuildCreateEnvironmentRequestMessage() {
  // return CreateEnvironmentRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767727859646)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(55667)
  //     .SetEnvironmentName("desertscene")
  //     .SetEnvironmentMetadata(nlohmann::json{
  //         {"sceneTime", "11:53:06:PM"},
  //         {"originLocation",
  //          nlohmann::json{{"x", 123}, {"y", 90.2}, {"z", 0.23}}}})
  //     .Build();
  return CreateEnvironmentRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013000,
    "environmentMetadata": {
        "originLocation": {
            "x": 123,
            "y": 90.2,
            "z": 0.23
        },
        "sceneTime": "11:53:06:PM"
    },
    "environmentName": "desertscene",
    "header": {
        "exerciseID": 2,
        "timestamp": 1767727859646
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 55667
}
)"));
}

CreateEnvironmentResponseMessage BuildCreateEnvironmentResponseMessage() {
  // return CreateEnvironmentResponseMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767727901234)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(55667)
  //     .SetRequestStatus(RequestStatus::kComplete)
  //     .Build();
  return CreateEnvironmentResponseMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013001,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767727901234
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 55667,
    "status": 4
}
)"));
}

FinalizeScenarioRequestMessage BuildFinalizeScenarioRequestMessage() {
  // return FinalizeScenarioRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767728932350)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(77889)
  //     .Build();

  return FinalizeScenarioRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013006,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767728932350
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 77889
}
)"));
}

FinalizeScenarioResponseMessage BuildFinalizeScenarioResponseMessage() {
  // return FinalizeScenarioResponseMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767729004188)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(77889)
  //     .SetRequestStatus(RequestStatus::kComplete)
  //     .Build();
  return FinalizeScenarioResponseMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013007,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767729004188
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 77889,
    "status": 4
}
)"));
}

ParameterizeConstructRequestMessage BuildParameterizeConstructRequestMessage() {
  // return ParameterizeConstructRequestMessageBuilder()
  //     .SetHeader(
  //         MusicMessageHeaderBuilder().SetExerciseId(2).SetTimestamp(1767729475392).Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(99001)
  //     .SetGhostedId(EntityIdBuilder()
  //                       .SetSite(300)
  //                       .SetApplication(50)
  //                       .SetEntityNumber(15)
  //                       .Build())
  //     .SetConstructParameters(
  //         nlohmann::json{{"parameter1", "value1"}, {"parameter2", 42}})
  //     .Build();
  return ParameterizeConstructRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013004,
    "constructParameters": {
        "parameter1": "value1",
        "parameter2": 42
    },
    "ghostedID": {
        "appID": 50,
        "entityID": 15,
        "siteID": 300
    },
    "header": {
        "exerciseID": 2,
        "timestamp": 1767729475392
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 99001
}
)"));
}

ParameterizeConstructResponseMessage
BuildParameterizeConstructResponseMessage() {
  // return ParameterizeConstructResponseMessageBuilder()
  //     .SetHeader(
  //         MusicMessageHeaderBuilder().SetExerciseId(2).SetTimestamp(1767729492832).Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(99001)
  //     .SetRequestStatus(RequestStatus::kExecuting)
  //     .Build();
  return ParameterizeConstructResponseMessage::FromJson(
      nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013005,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767729492832
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 99001,
    "status": 2
}
)"));
}

ScenarioStartRequestMessage BuildScenarioStartRequestMessage() {
  // return ScenarioStartRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767729569200)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(235711)
  //     .Build();
  return ScenarioStartRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013008,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767729569200
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 235711
}
)"));
}

DisplayMessagesMessage BuildDisplayMessagesMessage() {
  // return DisplayMessagesMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder().SetExerciseId(2).SetTimestamp(1767729858323).Build())
  //     .SetOriginId(EntityIdBuilder().SetSite(300).SetApplication(50).SetEntityNumber(0).Build())
  //     .SetReceiverId(EntityIdBuilder().SetSite(300).SetApplication(50).SetEntityNumber(5).Build())
  //     .SetMessages({"Lorem", "Ipsum", "Dolor", "Sit", "Amet"})
  //     .SetRequestId(11235813)
  //     .SetTimeout(5)
  //     .Build();
  return DisplayMessagesMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454004005,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767729858323
    },
    "messages": [
        "Lorem",
        "Ipsum",
        "Dolor",
        "Sit",
        "Amet"
    ],
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 11235813,
    "timeout": 5
}
)"));
}

RequestSimulationTimeMessage BuildRequestSimulationTimeMessage() {
  // return RequestSimulationTimeMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767730119956)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(31415926)
  //     .Build();

  return RequestSimulationTimeMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454009000,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767730119956
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 31415926
}
)"));
}

SetSimulationTimeMessage BuildSetSimulationTimeMessage() {
  // return SetSimulationTimeMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767730160182)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetNewTime(1767800000000)
  //     .SetRequestId(161803)
  //     .Build();
  return SetSimulationTimeMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454009001,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767730160182
    },
    "newTime": 1767800000000,
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 161803
}
)"));
}

SimulationTimeMessage BuildSimulationTimeMessage() {
  //   return SimulationTimeMessageBuilder()
  //       .SetHeader(MusicMessageHeaderBuilder()
  //                      .SetExerciseId(2)
  //                      .SetTimestamp(1767730306547)
  //                      .Build())
  //       .SetOriginId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(0)
  //                        .Build())
  //       .SetReceiverId(EntityIdBuilder()
  //                          .SetSite(300)
  //                          .SetApplication(50)
  //                          .SetEntityNumber(125)
  //                          .Build())
  //       .SetSimTime(1767800000)
  //       .SetRequestId(161803)
  //       .SetRequestStatus(RequestStatus::kComplete)
  //       .Build();
  return SimulationTimeMessage::FromJson(nlohmann::json::parse(R"(
{
    "commandIdentifier": 454009999,
    "header": {
        "exerciseID": 2,
        "timestamp": 1767730306547
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 125,
        "siteID": 300
    },
    "requestID": 161803,
    "simTime": 1767800000,
    "status": 4
}
)"));
}

ConstructControlRequestMessage BuildConstructControlRequestMessage() {
  // return ConstructControlRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767732560018)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetTargetConstruct(EntityIdBuilder()
  //                             .SetSite(300)
  //                             .SetApplication(50)
  //                             .SetEntityNumber(10)
  //                             .Build())
  //     .SetContext("com.discoverymachine.context.Arrest")
  //     .SetRequestId(42734273)
  //     .Build();

  return ConstructControlRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "context": "com.discoverymachine.context.Arrest",
    "header": {
        "exerciseID": 2,
        "timestamp": 1767732560018
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 42734273,
    "targetConstruct": {
        "appID": 50,
        "entityID": 10,
        "siteID": 300
    }
}
)"));
}

ConstructControlResponseMessage BuildConstructControlResponseMessage() {
  //   return ConstructControlResponseMessageBuilder()
  //       .SetHeader(MusicMessageHeaderBuilder()
  //                      .SetExerciseId(2)
  //                      .SetTimestamp(1767732581323)
  //                      .Build())
  //       .SetOriginId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(0)
  //                        .Build())
  //       .SetReceiverId(EntityIdBuilder()
  //                          .SetSite(300)
  //                          .SetApplication(50)
  //                          .SetEntityNumber(5)
  //                          .Build())
  //       .SetRequestStatus(RequestStatus::kComplete)
  //       .SetRequestId(42734273)
  //       .Build();

  return ConstructControlResponseMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767732581323
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 42734273,
    "status": 4
}
)"));
}

ConstructPrimaryControlRequestMessage
BuildConstructPrimaryControlRequestMessage() {
  // return ConstructPrimaryControlRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767732708132)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(8675309)
  //     .Build();

  return ConstructPrimaryControlRequestMessage::FromJson(
      nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767732708132
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 8675309
}
)"));
}

ConstructPrimaryControlResponseMessage
BuildConstructPrimaryControlResponseMessage() {
  // return ConstructPrimaryControlResponseMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767732735728)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestStatus(RequestStatus::kComplete)
  //     .SetRequestId(8675309)
  //     .Build();

  return ConstructPrimaryControlResponseMessage::FromJson(
      nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767732735728
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 8675309,
    "status": 4
}
)"));
}

ControlGrantedMessage BuildControlGrantedMessage() {
  // return ControlGrantedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767732853771)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(7427466)
  //     .Build();
  return ControlGrantedMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767732853771
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 7427466
}
)"));
}

ControlLostMessage BuildControlLostMessage() {
  // return ControlLostMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767732906578)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(6283185)
  //     .Build();
  return ControlLostMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767732906578
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 6283185
}
)"));
}

ControlReclamationMessage BuildControlReclamationMessage() {
  // return ControlReclamationMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733022582)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(112358)
  //     .Build();

  return ControlReclamationMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733022582
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 112358
}
)"));
}

ControlRegainedMessage BuildControlRegainedMessage() {
  // return ControlRegainedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733089793)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(13213455)
  //     .Build();
  return ControlRegainedMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733089793
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 13213455
}
)"));
}

ControlRelinquishedMessage BuildControlRelinquishedMessage() {
  // return ControlRelinquishedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733112943)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(89144233)
  //     .Build();

  return ControlRelinquishedMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733112943
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 89144233
}
)"));
}

ControlTransferGrantedMessage BuildControlTransferGrantedMessage() {
  // return ControlTransferGrantedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733157701)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(377610)
  //     .Build();

  return ControlTransferGrantedMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733157701
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 377610
}
)"));
}

ControlTransferRequestMessage BuildControlTransferRequestMessage() {
  // return ControlTransferRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733202267)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetTargetConstruct(EntityIdBuilder()
  //                             .SetSite(300)
  //                             .SetApplication(50)
  //                             .SetEntityNumber(60)
  //                             .Build())
  //     .SetContext("com.discoverymachine.context.Patrol")
  //     .SetProposedController(EntityIdBuilder()
  //                                .SetSite(300)
  //                                .SetApplication(50)
  //                                .SetEntityNumber(75)
  //                                .Build())
  //     .SetRequestId(9871597)
  //     .Build();
  return ControlTransferRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "context": "com.discoverymachine.context.Patrol",
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733202267
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "proposedController": {
        "appID": 50,
        "entityID": 75,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 9871597,
    "targetConstruct": {
        "appID": 50,
        "entityID": 60,
        "siteID": 300
    }
}
)"));
}

ControlTransferResponseMessage BuildControlTransferResponseMessage() {
  // return ControlTransferResponseMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733618423)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestStatus(RequestStatus::kComplete)
  //     .SetRequestId(9871597)
  //     .Build();

  return ControlTransferResponseMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733618423
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 9871597,
    "status": 4
}
)"));
}

PrimaryControlGrantedMessage BuildPrimaryControlGrantedMessage() {
  // return PrimaryControlGrantedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733630185)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(25844181)
  //     .Build();
  return PrimaryControlGrantedMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733630185
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 25844181
}
)"));
}

PrimaryControlRelinquishedMessage BuildPrimaryControlRelinquishedMessage() {
  // return PrimaryControlRelinquishedMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733682011)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(676510946)
  //     .Build();
  return PrimaryControlRelinquishedMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733682011
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 676510946
}    
)"));
}

SetCurrentControllerMessage BuildSetCurrentControllerMessage() {
  // return SetCurrentControllerMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767733716959)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetCurrentControllerId(EntityIdBuilder()
  //                                 .SetSite(300)
  //                                 .SetApplication(50)
  //                                 .SetEntityNumber(85)
  //                                 .Build())
  //     .SetRequestId(17711)
  //     .Build();

  return SetCurrentControllerMessage::FromJson(nlohmann::json::parse(R"(
{
    "currentControllerID": {
        "appID": 50,
        "entityID": 85,
        "siteID": 300
    },
    "header": {
        "exerciseID": 2,
        "timestamp": 1767733716959
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 17711
}
)"));
}

ConstructInteractionRequestMessage BuildConstructInteractionRequestMessage() {
  // return ConstructInteractionRequestMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767734508300)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(28657)
  //     .SetInteractionType(InteractionType::kRequest)
  //     .SetInteractionName("com.discoverymachine.interaction.SupplyDrop")
  //     .SetInteractionData(nlohmann::json{
  //         {"levelOfZoom", 2},
  //         {"eyeLocation",
  //          nlohmann::json{{"x", 123.0}, {"y", 90.2}, {"z", 0.23}}}})
  //     .Build();
  return ConstructInteractionRequestMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767734508300
    },
    "interactionData": {
        "eyeLocation": {
            "x": 123.0,
            "y": 90.2,
            "z": 0.23
        },
        "levelOfZoom": 2
    },
    "interactionName": "com.discoverymachine.interaction.SupplyDrop",
    "interactionType": 454110001,
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 28657
}
)"));
}

ConstructInteractionResponseMessage BuildConstructInteractionResponseMessage() {
  // return ConstructInteractionResponseMessageBuilder()
  //     .SetHeader(MusicMessageHeaderBuilder()
  //                    .SetExerciseId(2)
  //                    .SetTimestamp(1767734748171)
  //                    .Build())
  //     .SetOriginId(EntityIdBuilder()
  //                      .SetSite(300)
  //                      .SetApplication(50)
  //                      .SetEntityNumber(0)
  //                      .Build())
  //     .SetReceiverId(EntityIdBuilder()
  //                        .SetSite(300)
  //                        .SetApplication(50)
  //                        .SetEntityNumber(5)
  //                        .Build())
  //     .SetRequestId(28657)
  //     .SetRequestStatus(RequestStatus::kComplete)
  //     .SetOptionalData(nlohmann::json{{"weaponStatus",
  //     "AcquiringTargetLock"}}) .Build();

  return ConstructInteractionResponseMessage::FromJson(nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 2,
        "timestamp": 1767734748171
    },
    "optionalData": {
        "weaponStatus": "AcquiringTargetLock"
    },
    "originID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 5,
        "siteID": 300
    },
    "requestID": 28657,
    "status": 4
}
)"));
}

int main(int ac, char **av) {
  spdlog::info("-- Running integration test 1 --");

  music::DISMusicConfiguration config = music::DISMusicConfiguration();
  config.SetSendAddress("192.168.254.157");
  config.SetSendPort(3000);
  config.SetSendProtocol(music::DISProtocol::kSeven);

  config.SetReceiveAddress("192.168.254.57");
  config.SetReceivePort(3020);

  spdlog::info("-- Built MUSIC Configuration --");

  music::SimulationAddress addr = music::SimulationAddress();
  addr.SetApplication(5);
  addr.SetSite(1);

  int exercise_id = 1;

  music::DISMusicEndpoint endpoint =
      music::DISMusicEndpoint(&config, exercise_id, addr);

  spdlog::info("-- Built MUSIC Endpoint --");

  endpoint.Connect();

  spdlog::info("-- Connected MUSIC Endpoint --");
  FireMessage fire_message = BuildFireMessage();
  spdlog::info("-- Built Fire Message --");
  endpoint.SendMessage(fire_message);

  DetonationMessage detonation_message = BuildDetonationMessage();
  spdlog::info("-- Built Detonation Message --");
  endpoint.SendMessage(detonation_message);

  CollisionMessage built_message = BuildCollisionMessage();
  spdlog::info("-- Built Collision Message --");
  endpoint.SendMessage(built_message);

  // command/constructcontrol
  RemoveConstructMessage remove_construct_message =
      BuildRemoveConstructMessage();
  spdlog::info("-- Built Remove Construct Message --");
  endpoint.SendMessage(remove_construct_message);

  StopConstructMessage stop_construct_message = BuildStopConstructMessage();
  spdlog::info("-- Built Stop Construct Message --");
  endpoint.SendMessage(stop_construct_message);

  // command/entity
  TransferConstructIdMessage transfer_construct_id_message =
      BuildTransferConstructIdMessage();
  spdlog::info("-- Built Transfer Construct ID Message --");
  endpoint.SendMessage(transfer_construct_id_message);

  // command/playercontrol
  ControlInitiatedMessage control_initiated_message =
      BuildControlInitiatedMessage();
  spdlog::info("-- Built Control Initiated Message --");
  endpoint.SendMessage(control_initiated_message);

  ControlReleasedMessage control_released_message =
      BuildControlReleasedMessage();
  spdlog::info("-- Built Control Released Message --");
  endpoint.SendMessage(control_released_message);

  // command/scenario
  CreateConstructRequestMessage create_construct_request_message =
      BuildCreateConstructRequestMessage();
  spdlog::info("-- Built Create Construct Request Message --");
  endpoint.SendMessage(create_construct_request_message);

  CreateConstructResponseMessage create_construct_response_message =
      BuildCreateConstructResponseMessage();
  spdlog::info("-- Built Create Construct Response Message --");
  endpoint.SendMessage(create_construct_response_message);

  CreateEnvironmentRequestMessage create_environment_request_message =
      BuildCreateEnvironmentRequestMessage();
  spdlog::info("-- Built Create Environment Request Message --");
  endpoint.SendMessage(create_environment_request_message);

  CreateEnvironmentResponseMessage create_environment_response_message =
      BuildCreateEnvironmentResponseMessage();
  spdlog::info("-- Built Create Environment Response Message --");
  endpoint.SendMessage(create_environment_response_message);

  FinalizeScenarioRequestMessage finalize_scenario_request_message =
      BuildFinalizeScenarioRequestMessage();
  spdlog::info("-- Built Finalize Scenario Request Message --");
  endpoint.SendMessage(finalize_scenario_request_message);

  FinalizeScenarioResponseMessage finalize_scenario_response_message =
      BuildFinalizeScenarioResponseMessage();
  spdlog::info("-- Built Finalize Scenario Response Message --");
  endpoint.SendMessage(finalize_scenario_response_message);

  ParameterizeConstructRequestMessage parameterize_construct_request_message =
      BuildParameterizeConstructRequestMessage();
  spdlog::info("-- Built Parameterize Construct Request Message --");
  endpoint.SendMessage(parameterize_construct_request_message);

  ParameterizeConstructResponseMessage parameterize_construct_response_message =
      BuildParameterizeConstructResponseMessage();
  spdlog::info("-- Built Parameterize Construct Response Message --");
  endpoint.SendMessage(parameterize_construct_response_message);

  ScenarioStartRequestMessage scenario_start_request_message =
      BuildScenarioStartRequestMessage();
  spdlog::info("-- Built Scenario Start Request Message --");
  endpoint.SendMessage(scenario_start_request_message);

  // command/simulation
  DisplayMessagesMessage display_messages_message =
      BuildDisplayMessagesMessage();
  spdlog::info("-- Built Display Messages Message --");
  endpoint.SendMessage(display_messages_message);

  // command/time
  RequestSimulationTimeMessage request_simulation_time_message =
      BuildRequestSimulationTimeMessage();
  spdlog::info("-- Built Request Simulation Time Message --");
  endpoint.SendMessage(request_simulation_time_message);

  SetSimulationTimeMessage set_simulation_time_message =
      BuildSetSimulationTimeMessage();
  spdlog::info("-- Built Set Simulation Time Message --");
  endpoint.SendMessage(set_simulation_time_message);

  SimulationTimeMessage simulation_time_message = BuildSimulationTimeMessage();
  spdlog::info("-- Built Simulation Time Message --");
  endpoint.SendMessage(simulation_time_message);

  // control
  ConstructControlRequestMessage construct_control_request_message =
      BuildConstructControlRequestMessage();
  spdlog::info("-- Built Construct Control Request Message --");
  endpoint.SendMessage(construct_control_request_message);

  ConstructControlResponseMessage construct_control_response_message =
      BuildConstructControlResponseMessage();
  spdlog::info("-- Built Construct Control Response Message --");
  endpoint.SendMessage(construct_control_response_message);

  ConstructPrimaryControlRequestMessage
      construct_primary_control_request_message =
          BuildConstructPrimaryControlRequestMessage();
  spdlog::info("-- Built Construct Primary Control Request Message --");
  endpoint.SendMessage(construct_primary_control_request_message);

  ConstructPrimaryControlResponseMessage
      construct_primary_control_response_message =
          BuildConstructPrimaryControlResponseMessage();
  spdlog::info("-- Built Construct Primary Control Response Message --");
  endpoint.SendMessage(construct_primary_control_response_message);

  ControlGrantedMessage control_granted_message = BuildControlGrantedMessage();
  spdlog::info("-- Built Control Granted Message --");
  endpoint.SendMessage(control_granted_message);

  ControlLostMessage control_lost_message = BuildControlLostMessage();
  spdlog::info("-- Built Control Lost Message --");
  endpoint.SendMessage(control_lost_message);

  ControlReclamationMessage control_reclamation_message =
      BuildControlReclamationMessage();
  spdlog::info("-- Built Control Reclamation Message --");
  endpoint.SendMessage(control_reclamation_message);
  endpoint.SendMessage(control_reclamation_message);

  ControlRegainedMessage control_regained_message =
      BuildControlRegainedMessage();
  spdlog::info("-- Built Control Regained Message --");
  endpoint.SendMessage(control_regained_message);

  ControlRelinquishedMessage control_relinquished_message =
      BuildControlRelinquishedMessage();
  spdlog::info("-- Built Control Relinquished Message --");
  endpoint.SendMessage(control_relinquished_message);

  ControlTransferGrantedMessage control_transfer_granted_message =
      BuildControlTransferGrantedMessage();
  spdlog::info("-- Built Control Transfer Granted Message --");
  endpoint.SendMessage(control_transfer_granted_message);

  ControlTransferRequestMessage control_transfer_request_message =
      BuildControlTransferRequestMessage();
  spdlog::info("-- Built Control Transfer Request Message --");
  endpoint.SendMessage(control_transfer_request_message);

  ControlTransferResponseMessage control_transfer_response_message =
      BuildControlTransferResponseMessage();
  spdlog::info("-- Built Control Transfer Response Message --");
  endpoint.SendMessage(control_transfer_response_message);

  PrimaryControlGrantedMessage primary_control_granted_message =
      BuildPrimaryControlGrantedMessage();
  spdlog::info("-- Built Primary Control Granted Message --");
  endpoint.SendMessage(primary_control_granted_message);

  PrimaryControlRelinquishedMessage primary_control_relinquished_message =
      BuildPrimaryControlRelinquishedMessage();
  spdlog::info("-- Built Primary Control Relinquished Message --");
  endpoint.SendMessage(primary_control_relinquished_message);

  SetCurrentControllerMessage set_current_controller_message =
      BuildSetCurrentControllerMessage();
  spdlog::info("-- Built Set Current Controller Message --");
  endpoint.SendMessage(set_current_controller_message);

  // event
  EventMessage event_message = BuildEventMessage();
  spdlog::info("-- Built Event Message --");
  endpoint.SendMessage(event_message);

  // interaction
  ConstructInteractionRequestMessage construct_interaction_request_message =
      BuildConstructInteractionRequestMessage();
  spdlog::info("-- Built Construct Interaction Request Message --");
  endpoint.SendMessage(construct_interaction_request_message);

  ConstructInteractionResponseMessage construct_interaction_response_message =
      BuildConstructInteractionResponseMessage();
  spdlog::info("-- Built Construct Interaction Response Message --");
  endpoint.SendMessage(construct_interaction_response_message);

  // Custom PDUs
  ConstructDataMessage construct_message = BuildConstructDataMessage();
  spdlog::info("-- Built Construct Data Message --");
  endpoint.SendMessage(construct_message);

  PerceptionDataMessage perception_data_message = BuildPerceptionDataMessage();
  spdlog::info("-- Built Perception Data Message --");
  endpoint.SendMessage(perception_data_message);

  StateFieldMessage state_field_message = BuildStateFieldMessage();
  spdlog::info("-- Built State Field Message --");
  endpoint.SendMessage(state_field_message);

  WaypointDataMessage waypoint_data_message = BuildWaypointDataMessage();
  spdlog::info("-- Built Waypoint Data Message --");
  endpoint.SendMessage(waypoint_data_message);

  spdlog::info("-- Message Sent --");

  endpoint.Disconnect();

  spdlog::info("-- Disconnected MUSIC Endpoint --");
  return 0;
}