/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_DEAD_RECKONING_PARAMETERS_RECORD_H
#define DMI_CONSTRUCT_DATA_DEAD_RECKONING_PARAMETERS_RECORD_H

#include <vector>
#include <optional>
#include <codecvt>
#include "../../Interfaces/IToJSON.h"
#include "../MUSICVector3.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class DeadReckoningParametersRecord : public  MUSICLibrary::Interfaces::IToJSON
	{
	private:
		std::shared_ptr<MUSICVector3> linearAcceleration;
		std::shared_ptr<MUSICVector3> angularVelocity;
		unsigned char algorithmValue;

	public:
		unsigned char getAlgorithmValue() const;
		void setAlgorithmValue(unsigned char value);
		std::shared_ptr<MUSICVector3> getLinearAcceleration() const;
		void setLinearAcceleration(std::shared_ptr<MUSICVector3> value);
		std::shared_ptr<MUSICVector3> getAngularVelocity() const;
		void setAngularVelocity(std::shared_ptr<MUSICVector3> value);

		//Default Constructor
		DeadReckoningParametersRecord();

		//Constructor 
		DeadReckoningParametersRecord(unsigned char algorithmVal, std::shared_ptr<MUSICVector3> linearAcceleration, std::shared_ptr<MUSICVector3> angularVelocity);

		//JSON Constructor 
		DeadReckoningParametersRecord(std::shared_ptr<boost::json::object> obj);

		void Update(std::shared_ptr<DeadReckoningParametersRecord> record);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		bool operator == (const DeadReckoningParametersRecord &dr2);

		bool operator != (const DeadReckoningParametersRecord &dr2);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const DeadReckoningParametersRecord& obj) const;
		};
	};
}

#endif // !DMI_CONSTRUCT_DATA_DEAD_RECKONING_PARAMETERS_RECORD_H