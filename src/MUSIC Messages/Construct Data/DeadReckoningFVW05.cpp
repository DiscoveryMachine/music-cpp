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

#include "DeadReckoningFVW05.h"

using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;

namespace MUSICLibrary::MUSIC_Messages
{

	void DeadReckoningFVW05::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		std::shared_ptr<MUSICVector3> locationData = record->getLocation();
		std::shared_ptr<MUSICVector3> linearVelocityData = record->getLinearVelocity();
		std::shared_ptr<MUSICVector3> linearAccelData = record->getDeadReckoningParameters()->getLinearAcceleration();

		double newLocX = locationData->getX() + linearVelocityData->getX() * secondsToProject + .5 * linearAccelData->getX() * std::pow(secondsToProject,2);
		double newLocY = locationData->getY() + linearVelocityData->getY() * secondsToProject + .5 * linearAccelData->getY() * std::pow(secondsToProject, 2);
		double newLocZ = locationData->getZ() + linearVelocityData->getZ() * secondsToProject + .5 * linearAccelData->getZ() * std::pow(secondsToProject, 2);


		double newVelX = linearVelocityData->getX() + linearAccelData->getX() * secondsToProject;
		double newVelY = linearVelocityData->getY() + linearAccelData->getY() * secondsToProject;
		double newVelZ = linearVelocityData->getZ() + linearAccelData->getZ() * secondsToProject;

		std::shared_ptr<PhysicalRecord> newPhysical = std::make_shared<PhysicalRecord>();
		newPhysical->setLocation(std::make_shared<MUSICVector3>(newLocX, newLocY, newLocZ));
		newPhysical->setLinearVelocity(std::make_shared<MUSICVector3>(newVelX, newVelY, newVelZ));

		record->Update(newPhysical);
	}

	unsigned char DeadReckoningFVW05::GetAlgorithm()
	{
		return 5;
	}

	bool DeadReckoningFVW05::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningFVW05>))
		{
			return true;
		}

		return false;
	}

	bool DeadReckoningFVW05::operator == (const DeadReckoningFVW05 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningFVW05>(dr2));
	}

	bool DeadReckoningFVW05::operator != (const DeadReckoningFVW05 &dr2)
	{
		return !(*this == dr2);
	}
}
