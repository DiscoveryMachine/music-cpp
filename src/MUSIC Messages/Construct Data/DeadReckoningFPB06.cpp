/*
 * Copyright (c) 2006-2011, Naval Postgraduate School, MOVES Institute All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Naval Postgraduate School, MOVES Institute, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY NPS AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "DeadReckoningFPB06.h"

using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;
using namespace MUSICLibrary::MUSIC_Messages::OpenDis;

namespace MUSICLibrary::MUSIC_Messages
{
	void DeadReckoningFPB06::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		if (secondsToProject == 0)
		{
			return;
		}

		std::shared_ptr<MUSICVector3> locationData = record->getLocation();
		std::shared_ptr<MUSICVector3> orientationData = record->getOrientation();
		std::shared_ptr<MUSICVector3> linearVelocityData = record->getLinearVelocity();

		//getting latitude and longitude...
		double xyz[3] = {0};
		xyz[0] = locationData->getX();
		xyz[1] = locationData->getY();
		xyz[2] = locationData->getZ();
		std::vector<double> latLonAlt = CoordinateConversions::xyzToLatLonRadians(xyz);

		double east = linearVelocityData->getX();
		double north = linearVelocityData->getY();
		double up = 0.0;

		//getting linearVelocity data...
		double t = std::cos(latLonAlt[0]) * up - std::sin(latLonAlt[0]) * north;

		//getting velocity vector...
		double u = std::cos(latLonAlt[1]) * t - std::sin(latLonAlt[1]) * east;
		double v = std::sin(latLonAlt[1]) * t + std::cos(latLonAlt[1]) * east;
		double w = std::sin(latLonAlt[0]) * up + std::cos(latLonAlt[0]) * north;

		//applying velocity vector to coordinates...
		std::vector<double> updated = {u * secondsToProject, v * secondsToProject, w * secondsToProject};

		//applying change to record
		std::shared_ptr<PhysicalRecord> newRecord = std::make_shared<PhysicalRecord>();
		newRecord->setLocation(std::make_shared<MUSICVector3>(updated[0] + xyz[0], updated[1] + xyz[1], updated[2] + xyz[2]));

		record->Update(newRecord);
	}

	unsigned char DeadReckoningFPB06::GetAlgorithm()
	{
		return 6;
	}

	bool DeadReckoningFPB06::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningFPB06>))
		{
			return true;
		}
		return false;
	}

	bool DeadReckoningFPB06::operator == (const DeadReckoningFPB06 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningFPB06>(dr2));
	}

	bool DeadReckoningFPB06::operator != (const DeadReckoningFPB06 &dr2)
	{
		return !(*this == dr2);
	}
}
