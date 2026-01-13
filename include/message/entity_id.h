// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ENTITY_ID_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ENTITY_ID_H_

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "simulation_address.h"


namespace music {
// Identify an entity.
class EntityId {
 public:
  template <class T>
  friend class EntityIdBuilderBase;

  // Assumes unsigned short == 16 bits
  inline static const int kLowestEntityNumber = 0;
  inline static const int kHighestEntityNumber =
      std::numeric_limits<unsigned short>::max();

  inline static const std::string kEntityNumberJsonKey = "entityID";
  inline static const std::string kSimulationAddressJsonKey =
      "simulationAddress";

  EntityId() {
    sim_addr_ = SimulationAddressBuilder().Build();
    entity_number_ = std::numeric_limits<int>::min();
  }

  const int GetSite() const { return sim_addr_.GetSite(); }
  const int GetApplication() const { return sim_addr_.GetApplication(); }
  const int GetEntityNumber() const { return entity_number_; }

  void SetSite(int site) { sim_addr_.SetSite(site); }
  void SetApplication(int application) {
    sim_addr_.SetApplication(application);
  }
  void SetEntityNumber(int entity_number) { entity_number_ = entity_number; }

  void Validate() const;

  const std::string ToString() const { return "EntityId: " + ToJson().dump(); }

  friend std::ostream &operator<<(std::ostream &os, const EntityId &entityId) {
    os << entityId.ToString();
    return os;
  }

  friend void to_json(nlohmann::json &j, const EntityId &entityId);
  friend void from_json(const nlohmann::json &j, EntityId &entityId);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static EntityId FromJson(const nlohmann::json &j) {
    EntityId id;
    from_json(j, id);
    return id;
  }
  bool operator==(const music::EntityId &other) const {
    return other.entity_number_ == entity_number_ &&
           other.sim_addr_ == sim_addr_;
  }

  bool operator!=(const music::EntityId &other) const {
    return !(other == *this);
  }

 protected:
  SimulationAddress sim_addr_{};
  int entity_number_{std::numeric_limits<int>::min()};
};

template <class T>
class EntityIdBuilderBase {
 public:
  T &SetSite(int site) {
    wip_->SetSite(site);
    return static_cast<T &>(*this);
  }

  T &SetApplication(int application) {
    wip_->SetApplication(application);
    return static_cast<T &>(*this);
  }

  T &SetEntityNumber(int entity_number) {
    wip_->SetEntityNumber(entity_number);
    return static_cast<T &>(*this);
  }

 protected:
  void SetWIP(std::shared_ptr<EntityId> entity_id) { wip_ = entity_id; }
  std::shared_ptr<EntityId> wip_;
};

class EntityIdBuilder : public EntityIdBuilderBase<EntityIdBuilder> {
 public:
  EntityIdBuilder() { SetWIP(std::make_shared<EntityId>()); }
  EntityId Build() { return *wip_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ENTITY_ID_H_