// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_SEVEN_CONSTRUCT_DATA_PDU_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_SEVEN_CONSTRUCT_DATA_PDU_H_

#include "../message/construct/construct_data_message.h"
#include "dis7/Pdu.h"

class DISSevenConstructDataPdu : public DIS::Pdu {
 private:
  music::ConstructDataMessage constructDataMessage;

 public:
  DISSevenConstructDataPdu() : DIS::Pdu() {
    this->setProtocolFamily(
        42);                // Protocol Family for MUSIC® Protocol Family is 42
    this->setPduType(230);  // PDU Type for Construct Data PDU is 230
  }

  void marshal(DIS::DataStream &dataStream) const override;

  void unmarshal(DIS::DataStream &dataStream) override;

  virtual int getMarshalledSize() const override;
};

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_DIS_DIS_SEVEN_CONSTRUCT_DATA_PDU_H_