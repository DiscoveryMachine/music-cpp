// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_FUSE_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_FUSE_TYPE_H_

namespace music {
enum class FuseType {
  kUnset = -1,  // INVALID VALUE
  // Other
  kOtherUnknown = 0,
  // Intelligent Influence
  kIntelligentInfluence = 10,
  // Sensor
  kSensor = 20,
  // Self-destruct
  kSelfDestruct = 30,
  // Ultra Quick
  kUltraQuick = 40,
  // Body
  kBody = 50,
  // Deep Intrusion
  kDeepIntrusion = 60,
  // Multifunction
  kMultifunction = 100,
  // Point Detonation (PD)
  kPointDetonation = 200,
  // Base Detonation (BD)
  kBaseDetonation = 300,
  // Contact
  kContact = 1000,
  // Contact, Instant (Impact)
  kContactInstantImpact = 1100,
  // Contact, Delayed
  kContactDelayed = 1200,
  // Contact, Electronic (Oblique Contact)
  kContactElectronicObliqueContact = 1300,
  // Contact, Graze
  kContactGraze = 1400,
  // Contact, Crush
  kContactCrush = 1500,
  // Contact, Hydrostatic
  kContactHydrostatic = 1600,
  // Contact, Mechanical
  kContactMechanical = 1700,
  // Contact, Chemical
  kContactChemical = 1800,
  // Contact, Piezoelectric
  kContactPiezoelectric = 1900,
  // Contact, Point Initiating
  kContactPointInitiating = 1910,
  // Contact, Point Initiating, Base Detonating
  kContactPointInitiatingBaseDetonating = 1920,
  // Contact, Base Detonating
  kContactBaseDetonating = 1930,
  // Contact, Ballistic Cap and Base
  kContactBallisticCapAndBase = 1940,
  // Contact, Base
  kContactBase = 1950,
  // Contact, Nose
  kContactNose = 1960,
  // Contact, Fitted in Standoff Probe
  kContactFittedInStandoffProbe = 1970,
  // Contact, Non-aligned
  kContactNonAligned = 1980,
  // Timed
  kTimed = 2000,
  // Timed, Programmable
  kTimedProgrammable = 2100,
  // Timed, Burnout
  kTimedBurnout = 2200,
  // Timed, Pyrotechnic
  kTimedPyrotechnic = 2300,
  // Timed, Electronic
  kTimedElectronic = 2400,
  // Timed, Base Delay
  kTimedBaseDelay = 2500,
  // Timed, Reinforced Nose Impact Delay
  kTimedReinforcedNoseImpactDelay = 2600,
  // Timed, Short Delay Impact
  kTimedShortDelayImpact = 2700,
  // Timed, Nose Mounted Variable Delay
  kTimedNoseMountedVariableDelay = 2800,
  // Timed, Long Delay Side
  kTimedLongDelaySide = 2900,
  // Timed, Selectable Delay
  kTimedSelectableDelay = 2910,
  // Timed, Impact
  kTimedImpact = 2920,
  // Timed, Sequence
  kTimedSequence = 2930,
  // Proximity
  kProximity = 3000,
  // Proximity, Active Laser
  kProximityActiveLaser = 3100,
  // Proximity, Magnetic (Magpolarity)
  kProximityMagnetic = 3200,
  // Proximity, Active Radar (Doppler Radar)
  kProximityActiveRadar = 3300,
  // Proximity, Radio Frequency (RF)
  kProximityRadioFrequency = 3400,
  // Proximity, Programmable
  kProximityProgrammable = 3500,
  // Proximity, Programmable, Prefragmented
  kProximityProgrammablePrefragmented = 3600,
  // Proximity, Infrared
  kProximityInfrared = 3700,
  // Command
  kCommand = 4000,
  // Command, Electronic, Remotely Set
  kCommandElectronicRemotelySet = 4100,
  // Altitude
  kAltitude = 5000,
  // Altitude, Radio Altimeter
  kAltitudeRadioAltimeter = 5100,
  // Altitude, Air Burst
  kAltitudeAirBurst = 5200,
  // Depth
  kDepth = 6000,
  // Acoustic
  kAcoustic = 7000,
  // Pressure
  kPressure = 8000,
  // Pressure, Delay
  kPressureDelay = 8010,
  // Inert
  kInert = 8100,
  // Dummy
  kDummy = 8110,
  // Practice
  kPractice = 8120,
  // Plug Representing
  kPlugRepresenting = 8130,
  // Training
  kTraining = 8150,
  // Pyrotechnic
  kPyrotechnic = 9000,
  // Pyrotechnic, Delay
  kPyrotechnicDelay = 9010,
  // Electro-optical
  kElectroOptical = 9100,
  // Electromechanical
  kElectromechanical = 9110,
  // Electromechanical, Nose
  kElectromechanicalNose = 9120,
  // Strikerless
  kStrikerless = 9200,
  // Strikerless, Nose Impact
  kStrikerlessNoseImpact = 9210,
  // Strikerless, Compression-Ignition
  kStrikerlessCompressionIgnition = 9220,
  // Compression-Ignition
  kCompressionIgnition = 9300,
  // Compression-Ignition, Strikerless, Nose Impact
  kCompressionIgnitionStrikerlessNoseImpact = 9310,
  // Percussion
  kPercussion = 9400,
  // Percussion, Instantaneous
  kPercussionInstantaneous = 9410,
  // Electronic
  kElectronic = 9500,
  // Electronic, Internally Mounted
  kElectronicInternallyMounted = 9510,
  // Electronic, Range Setting
  kElectronicRangeSetting = 9520,
  // Electronic, Programmed
  kElectronicProgrammed = 9530,
  // Mechanical
  kMechanical = 9600,
  // Mechanical, Nose
  kMechanicalNose = 9610,
  // Mechanical, Tail
  kMechanicalTail = 9620

};

class FuseTypeHelper {
 public:
  // Check if an integer value corresponds to a valid FuseType enum value
  static bool IsValid(int value);

  // Safely convert an integer to FuseType, throws if invalid
  static FuseType FromInt(int value);
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_FUSE_TYPE_H_