// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_SIMULATION_ID_H
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_SIMULATION_ID_H

#include <limits>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace music {
// Address a simulation participating in a distributed training exercise.
class SimulationAddress {
 public:
  template <class T>
  friend class SimulationAddressBuilderBase;

  // Constructor must be defined here: a protected, parameterized one is also
  // defined and used.
  SimulationAddress() {}

  // Assumes unsigned short == 16 bits
  inline static const int kLowestAddress = 0;
  inline static const int kHighestAddress =
      std::numeric_limits<unsigned short>::max();

  inline static const std::string kSiteJsonKey = "siteID";
  inline static const std::string kApplicationJsonKey = "appID";

  // Obtain a SimulationAddress that addresses all applications with the given
  // application number at all sites.
  static SimulationAddress AppAtAllSites(int application) {
    return SimulationAddress{kHighestAddress, application};
  }

  // Obtain a SimulationAddress that addresses all applications at the site
  // specified by the given site number.
  static SimulationAddress AllAppsAtSite(int site) {
    return SimulationAddress{site, kHighestAddress};
  }

  // Obtain a universal SimulationAddress - address all applications.
  static SimulationAddress AllApps() {
    return SimulationAddress{kHighestAddress, kHighestAddress};
  }

  const int GetSite() const { return site_; }
  const int GetApplication() const { return application_; }

  void SetSite(int site) { site_ = site; }
  void SetApplication(int application) { application_ = application; }

  void Validate() const;

  std::string ToString() const {
    return "SimulationAddress: " + ToJson().dump();
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const SimulationAddress &addr) {
    os << "AppId: " << addr.ToJson().dump();
    return os;
  }

  friend void to_json(nlohmann::json &j, const SimulationAddress &addr) {
    j[kSiteJsonKey] = addr.site_;
    j[kApplicationJsonKey] = addr.application_;
  }

  friend void from_json(const nlohmann::json &j, SimulationAddress &addr) {
    addr.site_ = j[kSiteJsonKey];
    addr.application_ = j[kApplicationJsonKey];
  }

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static SimulationAddress FromJson(const nlohmann::json &j) {
    SimulationAddress addr;
    from_json(j, addr);
    return addr;
  }

  bool operator==(const music::SimulationAddress &other) const {
    return other.site_ == site_ && other.application_ == application_;
  }

  bool operator!=(const music::SimulationAddress &other) const {
    return !(other == *this);
  }

 protected:
  // A site is defined as a facility, installation, organizational unit or a
  // geographic location that has one or more simulation applications capable of
  // participating in a distributed event.
  int site_{std::numeric_limits<int>::min()};

  // An application is defined as a software program that is used to generate
  // and process distributed simulation data including live, virtual and
  // constructive data.
  int application_{std::numeric_limits<int>::min()};

  SimulationAddress(int site, int application)
      : site_(site), application_(application) {}
};

// Builder classes

template <class T>
class SimulationAddressBuilderBase {
 public:
  T &SetSite(int site) {
    wip_->SetSite(site);
    return static_cast<T &>(*this);
  }

  T &SetApplication(int application) {
    wip_->SetApplication(application);
    return static_cast<T &>(*this);
  }

 protected:
  void SetWIP(std::shared_ptr<SimulationAddress> addr) { wip_ = addr; }
  std::shared_ptr<SimulationAddress> wip_;
};

class SimulationAddressBuilder
    : public SimulationAddressBuilderBase<SimulationAddressBuilder> {
 public:
  SimulationAddressBuilder() { SetWIP(std::make_shared<SimulationAddress>()); }
  SimulationAddress Build() { return *wip_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_SIMULATION_ID_H