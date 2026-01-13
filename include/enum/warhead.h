// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_WARHEAD_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_WARHEAD_H_

namespace music {
enum class Warhead {
  // INVALID VALUE
  kUnset = -1,
  // Other
  kOtherUnknown = 0,
  // Cargo (Variable Submunitions)
  kCargoVariableSubmunitions = 10,
  // Fuel/Air Explosive
  kFuelAirExplosive = 20,
  // Glass Blads
  kGlassBlads = 30,
  // 1 um
  kOneUm = 31,
  // 5 um
  kFiveUm = 32,
  // 10 um
  kTenUm = 33,
  // High Explosive(HE)
  kHighExplosive = 1000,
  // HE, Plastic
  kHePlastic = 1100,
  // HE, Incendiary
  kHeIncendiary = 1200,
  // HE, Fragmentation
  kHeFragmentation = 1300,
  // HE, Antitank
  kHeAntitank = 1400,
  // HE, Bomblets
  kHeBomblets = 1500,
  // HE, Shaped Charge
  kHeShapedCharge = 1600,
  // HE, Continuous Rod
  kHeContinuousRod = 1610,
  // HE, Tungsten Ball
  kHeTungstenBall = 1615,
  // HE, Blast Fragmentation
  kHeBlastFragmentation = 1620,
  // HE, Steerable Darts with HE
  kHeSteerableDartsWithHe = 1625,
  // HE, Darts
  kHeDarts = 1630,
  // HE, Flechet
  kHeFlechettes = 1635,
  // HE, Directed Fragmentation
  kHeDirectedFragmentation = 1640,
  // HE, Semi-Armor Piercing (SAP)
  kHeSemiArmorPiercing = 1645,
  // HE, Shaped Charge Fragmentation
  kHeShapedChargeFragmentation = 1650,
  // HE, Semi-Armor Piercing, Fragmentation
  kHeSemiArmorPiercingFragmentation = 1655,
  // HE, Hallow Charge
  kHeHallowCharge = 1660,
  // HE, Double Hallow Charge
  kHeDoubleHallowCharge = 1665,
  // HE, General Purpose
  kHeGeneralPurpose = 1670,
  // HE, Blast Penetrator
  kHeBlastPenetrator = 1675,
  // HE, Rod Penetrator
  kHeRodPenetrator = 1680,
  // HE, Antipersonnel
  kHeAntipersonnel = 1685,
  // Smoke
  kSmoke = 2000,
  // Illumination
  kIllumination = 3000,
  // Practice
  kPractice = 4000,
  // Kinetic
  kKinetic = 5000,
  // Mines
  kMines = 6000,
  // Nuclear
  kNuclear = 7000,
  // Nuclear, IMT
  kNuclearImt = 7010,
  // Chemical, General
  kChemicalGeneral = 8000,
  // Chemical, Blister Agent
  kChemicalBlisterAgent = 8100,
  // HD (Mustard)
  kHdMustard = 8110,
  // Thickened HD (Mustard)
  kThickenedHdMustard = 8115,
  // Dusty HD (Mustard)
  kDustyHdMustard = 8120,
  // Chemical, Blood Agent
  kChemicalBloodAgent = 8200,
  // AC (HCN)
  kAcHcn = 8210,
  // CK (CNCI)
  kCkCnci = 8215,
  // CG (Phosgene)
  kCgPhosgene = 8220,
  // Chemical, Nerve Agent
  kChemicalNerveAgent = 8300,
  // VX
  kVx = 8310,
  // Thickened VX
  kThickenedVx = 8315,
  // Dusty VX
  kDustyVx = 8320,
  // GA (Tabun)
  kGaTabun = 8325,
  // Thickened GA (Tabun)
  kThickenedGa = 8330,
  // Dusty GA (Tabun)
  kDustyGa = 8335,
  // GB (Sarin)
  kGbSarin = 8340,
  // Thickened GB (Sarin)
  kThickenedGbSarin = 8345,
  // Dusty GB (Sarin)
  kDustyGbSarin = 8350,
  // GD (Soman)
  kGdSoman = 8355,
  // Thickened GD (Soman)
  kThickenedGdSoman = 8360,
  // Dusty GD (Soman)
  kDustyGdSoman = 8365,
  // GF
  kGf = 8370,
  // Thickened GF
  kThickenedGf = 8375,
  // Dusty GF
  kDustyGf = 8380,
  // Biological
  kBiological = 9000,
  // Biological, Virus
  kBiologicalVirus = 9100,
  // Biological, Bacteria
  kBiologicalBacteria = 9200,
  // Biological, Rickettsia
  kBiologicalRickettsia = 9300,
  // Biological, Genetically Modified Micro-organisms
  kBiologicalGeneticallyModifiedMicroOrganism = 9400,
  // Biological, Toxin
  kBiologicalToxin = 9500
};

class WarheadHelper {
 public:
  // Check if an integer value corresponds to a valid Warhead enum value
  static bool IsValid(int value);

  // Safely convert an integer to Warhead, throws if invalid
  static Warhead FromInt(int value);
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_WARHEAD_H_