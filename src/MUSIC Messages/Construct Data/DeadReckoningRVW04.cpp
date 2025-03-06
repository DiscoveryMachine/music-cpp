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

#include "DeadReckoningRVW04.h"

using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;

namespace MUSICLibrary::MUSIC_Messages
{
	std::shared_ptr<MUSICVector3> DeadReckoningRVW04::GetNewLocation(std::shared_ptr<MUSICVector3> location, std::shared_ptr<MUSICVector3> linearVelocity, std::shared_ptr<MUSICVector3> linearAcceleration, float seconds)
	{
		double newX = location->getX() + linearVelocity->getX() * seconds + .5 * linearAcceleration->getX() * std::pow(seconds, 2);
		double newY = location->getY() + linearVelocity->getY() * seconds + .5 * linearAcceleration->getY() * std::pow(seconds, 2);
		double newZ = location->getZ() + linearVelocity->getZ() * seconds + .5 * linearAcceleration->getZ() * std::pow(seconds, 2);

		return std::make_shared<MUSICVector3>(newX, newY, newZ);
	}

	std::shared_ptr<MUSICVector3> DeadReckoningRVW04::GetNewLinearVelocity(std::shared_ptr<MUSICVector3> linearVelocity, std::shared_ptr<MUSICVector3> linearAcceleration, float seconds)
	{
		double newX = linearVelocity->getX() + linearAcceleration->getX() * seconds;
		double newY = linearVelocity->getY() + linearAcceleration->getY() * seconds;
		double newZ = linearVelocity->getZ() + linearAcceleration->getZ() * seconds;

		return std::make_shared<MUSICVector3>(newX, newY, newZ);
	}

	std::shared_ptr<MUSICVector3> DeadReckoningRVW04::GetNewOrientation()
	{
		std::shared_ptr<EulerAngleHelper> helper = (std::make_shared<EulerAngleHelper>(rpwMatrix, 1e-15))->ApplyTo(InitialOrientation);

		std::vector<double> eulerAngles = helper->GetAngles();

		// verify we have numbers
		eulerAngles[0] = true == (std::isnan(eulerAngles[0])) ? 0 : eulerAngles[0];
		eulerAngles[1] = true == (std::isnan(eulerAngles[1])) ? 0 : eulerAngles[1];
		eulerAngles[2] = true == (std::isnan(eulerAngles[2])) ? 0 : eulerAngles[2];

		std::shared_ptr<MUSICVector3> newOrientation = std::make_shared<MUSICVector3>(eulerAngles[0], eulerAngles[1], eulerAngles[2]);

		return newOrientation;
	}

	void DeadReckoningRVW04::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		initialize(record);

		std::shared_ptr<PhysicalRecord> updater = std::make_shared<PhysicalRecord>();

		updater->setLocation(GetNewLocation(record->getLocation(), record->getLinearVelocity(), record->getDeadReckoningParameters()->getLinearAcceleration(), secondsToProject));

		updater->setLinearVelocity(GetNewLinearVelocity(record->getLinearVelocity(), record->getDeadReckoningParameters()->getLinearAcceleration(), secondsToProject));

		setPrivateMatrix(secondsToProject);

		// Necessary to stop orientation from changing with 0 time passing.
		updater->setOrientation((secondsToProject == 0) ? record->getOrientation() : GetNewOrientation());

		record->Update(updater);
	}

	unsigned char DeadReckoningRVW04::GetAlgorithm()
	{
		return 4;
	}

	bool DeadReckoningRVW04::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningRVW04>))
		{
			return true;
		}

		return false;
	}

	bool DeadReckoningRVW04::operator == (const DeadReckoningRVW04 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningRVW04>(dr2));
	}

	bool DeadReckoningRVW04::operator != (const DeadReckoningRVW04 &dr2)
	{
		return !(*this == dr2);
	}
}
