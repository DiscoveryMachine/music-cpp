#include <dis7/EntityID.h>
#include <dis7/EntityType.h>
#include <dis7/SimulationAddress.h>
#include <dis7/utils/DataStream.h>

#include "dis/dis_seven_construct_data_pdu.h"

void DISSevenConstructDataPdu::unmarshal(DIS::DataStream &dataStream) {
  constructDataMessage = music::ConstructDataMessage();
  DIS::Pdu::unmarshal(dataStream);

  // Unmarshal construct entity ID
  DIS::EntityID constructEntityID;
  constructEntityID.unmarshal(dataStream);

  music::EntityId construct_entity_id = music::EntityId();
  construct_entity_id.SetSite(
      constructEntityID.getSimulationAddress().getSite());
  construct_entity_id.SetApplication(
      constructEntityID.getSimulationAddress().getApplication());
  construct_entity_id.SetEntityNumber(constructEntityID.getEntityNumber());

  constructDataMessage.SetOriginId(construct_entity_id);

  // Unmarshal primary controller ID
  DIS::EntityID primaryControllerID;
  primaryControllerID.unmarshal(dataStream);
  music::EntityId primary_controller_id = music::EntityId();
  primary_controller_id.SetSite(
      primaryControllerID.getSimulationAddress().getSite());
  primary_controller_id.SetApplication(
      primaryControllerID.getSimulationAddress().getApplication());
  primary_controller_id.SetEntityNumber(primaryControllerID.getEntityNumber());
  constructDataMessage.SetPrimaryControllerId(primary_controller_id);

  // Unmarshal current controller ID
  DIS::EntityID currentControllerID;
  currentControllerID.unmarshal(dataStream);
  music::EntityId current_controller_id = music::EntityId();
  current_controller_id.SetSite(
      currentControllerID.getSimulationAddress().getSite());
  current_controller_id.SetApplication(
      currentControllerID.getSimulationAddress().getApplication());
  current_controller_id.SetEntityNumber(currentControllerID.getEntityNumber());
  constructDataMessage.SetCurrentControllerId(current_controller_id);

  // Unmarshal Construct Information Record
  uint8_t renderTypeValue;
  dataStream >> renderTypeValue;
  music::ConstructInformationRecord construct_info_record =
      music::ConstructInformationRecord();
  music::ConstructRenderType render_type =
      static_cast<music::ConstructRenderType>(renderTypeValue);
  construct_info_record.SetConstructRender(render_type);

  uint8_t categoryTypeValue;
  dataStream >> categoryTypeValue;

  construct_info_record.SetConstructCategory(
      static_cast<music::ConstructCategoryType>(categoryTypeValue));
  constructDataMessage.SetConstructInformationRecord(construct_info_record);

  music::ConstructRenderType render =
      constructDataMessage.GetConstructInformationRecord().GetConstructRender();
  if (render == music::ConstructRenderType::kGhostedConstruct ||
      render == music::ConstructRenderType::kGhostedLegacy) {
    // Unmarshal ghosted entity ID
    DIS::EntityID ghostedEntityID;
    ghostedEntityID.unmarshal(dataStream);
    music::EntityId ghosted_entity_id = music::EntityId();
    ghosted_entity_id.SetSite(ghostedEntityID.getSimulationAddress().getSite());
    ghosted_entity_id.SetApplication(
        ghostedEntityID.getSimulationAddress().getApplication());
    ghosted_entity_id.SetEntityNumber(ghostedEntityID.getEntityNumber());
    constructDataMessage.SetGhostedId(ghosted_entity_id);
    // Skip padding
    uint16_t padding;
    dataStream >> padding;
  } else if (render == music::ConstructRenderType::kRenderedPhysical ||
             render == music::ConstructRenderType::kUnrenderedPhysical) {
    // Unmarshal physical representation data
    uint8_t force;
    dataStream >> force;
    constructDataMessage.SetForce(static_cast<music::Force>(force));

    DIS::EntityType dis_entity_type;
    dis_entity_type.unmarshal(dataStream);

    music::EntityType entity_type = music::EntityType();
    entity_type.SetKind(dis_entity_type.getEntityKind());
    entity_type.SetDomain(dis_entity_type.getDomain());
    entity_type.SetCountry(dis_entity_type.getCountry());
    entity_type.SetCategory(dis_entity_type.getCategory());
    entity_type.SetSubcategory(dis_entity_type.getSubcategory());
    entity_type.SetSpecific(dis_entity_type.getSpecific());
    entity_type.SetExtra(dis_entity_type.getExtra());

    constructDataMessage.SetEntityType(entity_type);

    double x, y, z;
    dataStream >> x;
    dataStream >> y;
    dataStream >> z;
    music::Location location = music::Location();
    location.SetX(x);
    location.SetY(y);
    location.SetZ(z);
    constructDataMessage.SetLocation(location);

    float psi, theta, phi;
    dataStream >> psi;
    dataStream >> theta;
    dataStream >> phi;
    music::Orientation orientation = music::Orientation();
    orientation.SetPsi(psi);
    orientation.SetTheta(theta);
    orientation.SetPhi(phi);
    constructDataMessage.SetOrientation(orientation);

    float x_vel, y_vel, z_vel;
    dataStream >> x_vel;
    dataStream >> y_vel;
    dataStream >> z_vel;
    music::Vector velocity = music::Vector();
    velocity.SetX(x_vel);
    velocity.SetY(y_vel);
    velocity.SetZ(z_vel);
    constructDataMessage.SetVelocity(velocity);

    music::DeadReckoning dead_reckoning = music::DeadReckoning();
    uint8_t dr_type;
    dataStream >> dr_type;
    dead_reckoning.SetDeadReckAlgorithm(
        static_cast<music::DeadReckoningAlgorithmType>(dr_type));
    float ang_x, ang_y, ang_z;
    dataStream >> ang_x;
    dataStream >> ang_y;
    dataStream >> ang_z;
    music::Vector angular_velocity = music::Vector();
    angular_velocity.SetX(ang_x);
    angular_velocity.SetY(ang_y);
    angular_velocity.SetZ(ang_z);
    dead_reckoning.SetAngularVelocity(angular_velocity);
    float lin_x, lin_y, lin_z;
    dataStream >> lin_x;
    dataStream >> lin_y;
    dataStream >> lin_z;
    music::Vector linear_acceleration = music::Vector();
    linear_acceleration.SetX(lin_x);
    linear_acceleration.SetY(lin_y);
    linear_acceleration.SetZ(lin_z);
    dead_reckoning.SetLinearAcceleration(linear_acceleration);
    constructDataMessage.SetDeadReckoning(dead_reckoning);

    music::Damage damage = music::Damage();
    uint32_t catastrophic_damage;
    dataStream >> catastrophic_damage;
    damage.SetCatastrophicDamage(catastrophic_damage);
    uint32_t mobility_damage;
    dataStream >> mobility_damage;
    damage.SetMobilityDamage(mobility_damage);
    uint8_t firepower_disabled;
    dataStream >> firepower_disabled;
    damage.SetFirepowerDisabled(firepower_disabled);
    constructDataMessage.SetDamage(damage);
    uint16_t padding;
    dataStream >> padding;
    dataStream >> padding;
    dataStream >> padding;
  }

  uint16_t callsignLength;
  dataStream >> callsignLength;
  std::string callsign = "";
  for (uint16_t i = 0; i < callsignLength; ++i) {
    char c;
    dataStream >> c;
    callsign += c;
  }
  constructDataMessage.SetCallsign(callsign);

  uint16_t constructNameLength;
  dataStream >> constructNameLength;
  std::string constructName = "";
  for (uint16_t i = 0; i < constructNameLength; ++i) {
    char c;
    dataStream >> c;
    constructName += c;
  }
  constructDataMessage.SetConstructName(constructName);

  uint16_t interactionRecordLength;
  dataStream >> interactionRecordLength;
  std::string interactionRecord = "";
  for (uint16_t i = 0; i < interactionRecordLength; ++i) {
    char c;
    dataStream >> c;
    interactionRecord += c;
  }
  constructDataMessage.SetInteractionRecord(interactionRecord);

  int16_t padding;
  dataStream >> padding;
}

int DISSevenConstructDataPdu::getMarshalledSize() const {
  int marshalSize = 0;
  marshalSize += DIS::Pdu::getMarshalledSize();
  // construct entity id
  marshalSize += 6;
  // primary controller id
  marshalSize += 6;
  // current controller id
  marshalSize += 6;

  marshalSize += 2;  // construct information record

  music::ConstructRenderType render =
      constructDataMessage.GetConstructInformationRecord().GetConstructRender();

  if (render == music::ConstructRenderType::kGhostedConstruct ||
      render == music::ConstructRenderType::kGhostedLegacy) {
    marshalSize += 6;  // ghosted entity id
    marshalSize += 2;  // padding
  } else if (render == music::ConstructRenderType::kRenderedPhysical ||
             render == music::ConstructRenderType::kUnrenderedPhysical) {
    marshalSize += 1;   // force
    marshalSize += 8;   // entity type
    marshalSize += 24;  // location
    marshalSize += 12;  // orientation
    marshalSize += 12;  // linear velocity
    marshalSize += 40;  // dead reckoning
    marshalSize += 4;   // catastrophic damage
    marshalSize += 4;   // mobility damage
    marshalSize += 4;   // firepower disabled
    marshalSize += 6;   // padding
  }

  marshalSize += 2;  // callsign length
  marshalSize += static_cast<int>(std::ceil(
      constructDataMessage.GetCallsign().length() / 8.0));  // callsign

  marshalSize += 2;  // construct name length
  marshalSize += static_cast<int>(
      std::ceil(constructDataMessage.GetConstructName().length() /
                8.0));  // construct name

  marshalSize += 2;  // interaction name length
  marshalSize += static_cast<int>(
      std::ceil(constructDataMessage.GetInteractionRecord().length() /
                8.0));  // interaction name

  marshalSize += 2;  // padding

  return marshalSize;
}

void DISSevenConstructDataPdu::marshal(DIS::DataStream &dataStream) const {
  DIS::Pdu::marshal(dataStream);

  // Marshal construct entity ID
  DIS::EntityID constructEntityID;

  DIS::SimulationAddress constructSimAddr;
  constructSimAddr.setSite(constructDataMessage.GetOriginId().GetSite());
  constructSimAddr.setApplication(
      constructDataMessage.GetOriginId().GetApplication());

  constructEntityID.setSimulationAddress(constructSimAddr);
  constructEntityID.setEntityNumber(
      constructDataMessage.GetOriginId().GetEntityNumber());
  constructEntityID.marshal(dataStream);

  // Marshal primary controller ID
  DIS::EntityID primaryControllerID;

  DIS::SimulationAddress primaryControllerSimAddr;
  primaryControllerSimAddr.setSite(
      constructDataMessage.GetPrimaryControllerId().GetSite());
  primaryControllerSimAddr.setApplication(
      constructDataMessage.GetPrimaryControllerId().GetApplication());

  primaryControllerID.setSimulationAddress(primaryControllerSimAddr);
  primaryControllerID.setEntityNumber(
      constructDataMessage.GetPrimaryControllerId().GetEntityNumber());
  primaryControllerID.marshal(dataStream);

  // Marshal current controller ID
  DIS::EntityID currentControllerID;
  DIS::SimulationAddress currentControllerSimAddr;

  currentControllerSimAddr.setSite(
      constructDataMessage.GetCurrentControllerId().GetSite());
  currentControllerSimAddr.setApplication(
      constructDataMessage.GetCurrentControllerId().GetApplication());

  currentControllerID.setSimulationAddress(currentControllerSimAddr);
  currentControllerID.setEntityNumber(
      constructDataMessage.GetCurrentControllerId().GetEntityNumber());
  currentControllerID.marshal(dataStream);

  // Marshal Construct Information Record
  dataStream << static_cast<uint8_t>(
      constructDataMessage.GetConstructInformationRecord()
          .GetConstructRender());
  dataStream << static_cast<uint8_t>(
      constructDataMessage.GetConstructInformationRecord()
          .GetConstructCategory());

  music::ConstructRenderType render =
      constructDataMessage.GetConstructInformationRecord().GetConstructRender();

  if (render == music::ConstructRenderType::kGhostedConstruct ||
      render == music::ConstructRenderType::kGhostedLegacy) {
    // Marshal ghosted entity ID
    DIS::EntityID ghostedEntityID;
    DIS::SimulationAddress ghostedSimAddr;
    ghostedSimAddr.setSite(constructDataMessage.GetGhostedId()->GetSite());
    ghostedSimAddr.setApplication(
        constructDataMessage.GetGhostedId()->GetApplication());
    ghostedEntityID.setSimulationAddress(ghostedSimAddr);
    ghostedEntityID.setEntityNumber(
        constructDataMessage.GetGhostedId()->GetEntityNumber());
    ghostedEntityID.marshal(dataStream);
    // Padding
    dataStream << static_cast<uint16_t>(0);
  } else if (render == music::ConstructRenderType::kRenderedPhysical ||
             render == music::ConstructRenderType::kUnrenderedPhysical) {
    // Marshal physical representation data
    dataStream << static_cast<uint8_t>(constructDataMessage.GetForce().value());
    DIS::EntityType dis_entity_type;
    dis_entity_type.setEntityKind(
        constructDataMessage.GetEntityType()->GetKind());
    dis_entity_type.setDomain(
        constructDataMessage.GetEntityType()->GetDomain());
    dis_entity_type.setCountry(
        constructDataMessage.GetEntityType()->GetCountry());
    dis_entity_type.setCategory(
        constructDataMessage.GetEntityType()->GetCategory());
    dis_entity_type.setSubcategory(
        constructDataMessage.GetEntityType()->GetSubcategory());
    dis_entity_type.setSpecific(
        constructDataMessage.GetEntityType()->GetSpecific());
    dis_entity_type.setExtra(constructDataMessage.GetEntityType()->GetExtra());
    dis_entity_type.marshal(dataStream);

    dataStream << constructDataMessage.GetLocation()->GetX();
    dataStream << constructDataMessage.GetLocation()->GetY();
    dataStream << constructDataMessage.GetLocation()->GetZ();

    dataStream << constructDataMessage.GetOrientation()->GetPsi();
    dataStream << constructDataMessage.GetOrientation()->GetTheta();
    dataStream << constructDataMessage.GetOrientation()->GetPhi();

    dataStream << constructDataMessage.GetVelocity()->GetX();
    dataStream << constructDataMessage.GetVelocity()->GetY();
    dataStream << constructDataMessage.GetVelocity()->GetZ();
    dataStream << static_cast<uint8_t>(
        constructDataMessage.GetDeadReckoning()->GetDeadReckAlgorithm());
    dataStream
        << constructDataMessage.GetDeadReckoning()->GetAngularVelocity().GetX();
    dataStream
        << constructDataMessage.GetDeadReckoning()->GetAngularVelocity().GetY();
    dataStream
        << constructDataMessage.GetDeadReckoning()->GetAngularVelocity().GetZ();
    dataStream << constructDataMessage.GetDeadReckoning()
                      ->GetLinearAcceleration()
                      .GetX();
    dataStream << constructDataMessage.GetDeadReckoning()
                      ->GetLinearAcceleration()
                      .GetY();
    dataStream << constructDataMessage.GetDeadReckoning()
                      ->GetLinearAcceleration()
                      .GetZ();

    dataStream << constructDataMessage.GetDamage()->GetCatastrophicDamage();
    dataStream << constructDataMessage.GetDamage()->GetMobilityDamage();
    dataStream << constructDataMessage.GetDamage()->IsFirepowerDisabled();
    // Padding
    dataStream << static_cast<uint16_t>(0);
    dataStream << static_cast<uint16_t>(0);
    dataStream << static_cast<uint16_t>(0);
  }
  // Marshal callsign
  uint16_t callsignLength =
      static_cast<uint16_t>(constructDataMessage.GetCallsign().length());
  dataStream << callsignLength;
  for (char c : constructDataMessage.GetCallsign()) {
    dataStream << c;
  }
  // Marshal construct name
  uint16_t constructNameLength =
      static_cast<uint16_t>(constructDataMessage.GetConstructName().length());
  dataStream << constructNameLength;
  for (char c : constructDataMessage.GetConstructName()) {
    dataStream << c;
  }
  // Marshal interaction record
  uint16_t interactionRecordLength = static_cast<uint16_t>(
      constructDataMessage.GetInteractionRecord().length());
  dataStream << interactionRecordLength;
  for (char c : constructDataMessage.GetInteractionRecord()) {
    dataStream << c;
  }
  // Padding
  dataStream << static_cast<uint16_t>(0);
}