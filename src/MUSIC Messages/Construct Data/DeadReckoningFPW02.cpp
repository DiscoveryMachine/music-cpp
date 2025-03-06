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

#include "DeadReckoningFPW02.h"

using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;
using namespace MUSICLibrary::MUSIC_Messages;

namespace MUSICLibrary::MUSIC_Messages
{
	void DeadReckoningFPW02::ApplyDeadReckoning(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PhysicalRecord> record, float secondsToProject)
	{
		std::shared_ptr<MUSICVector3> velocityData = record->getLinearVelocity();
		std::shared_ptr<MUSICVector3> locationData = record->getLocation();

		//new Position = oldPosition + velocity(time)
		double newX = locationData->getX() + velocityData->getX() * secondsToProject;
		double newY = locationData->getY() + velocityData->getY() * secondsToProject;
		double newZ = locationData->getZ() + velocityData->getZ() * secondsToProject;

		std::shared_ptr<MUSICVector3> newLocation = std::make_shared<MUSICVector3>(newX, newY, newZ);
		std::shared_ptr<PhysicalRecord> newPhysical = std::make_shared<PhysicalRecord>();

		newPhysical->setLocation(newLocation);
		record->Update(newPhysical);
	}

	unsigned char DeadReckoningFPW02::GetAlgorithm()
	{
		return 2;
	}

	bool DeadReckoningFPW02::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningFPW02>))
		{
			return true;
		}

		return false;
	}

	bool DeadReckoningFPW02::operator == (const DeadReckoningFPW02 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningFPW02>(dr2));
	}

	bool DeadReckoningFPW02::operator != (const DeadReckoningFPW02 &dr2)
	{
		return !(*this == dr2);
	}
}
