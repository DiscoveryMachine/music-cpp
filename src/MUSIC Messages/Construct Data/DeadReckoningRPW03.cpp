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

#include "DeadReckoningRPW03.h"

using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;

namespace MUSICLibrary::MUSIC_Messages
{

	std::shared_ptr<MUSICVector3> DeadReckoningRPW03::GetNewLocation(std::shared_ptr<PhysicalRecord> record, float seconds)
	{
		std::shared_ptr<MUSICVector3> locationData = record->getLocation();
		std::shared_ptr<MUSICVector3> linearVelocityData = record->getLinearVelocity();
		std::shared_ptr<MUSICVector3> linearAccelData = record->getDeadReckoningParameters()->getLinearAcceleration();

		double newX = locationData->getX() + linearVelocityData->getX() * seconds + (.5 * linearAccelData->getX() * std::pow(seconds, 2));
		double newY = locationData->getY() + linearVelocityData->getY() * seconds + (.5 * linearAccelData->getY() * std::pow(seconds, 2));
		double newZ = locationData->getZ() + linearVelocityData->getZ() * seconds + (.5 * linearAccelData->getZ() * std::pow(seconds, 2));

		return std::make_shared<MUSICVector3>(newX, newY, newZ);
	}

	std::shared_ptr<MUSICVector3> DeadReckoningRPW03::GetNewOrientation()
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

	void DeadReckoningRPW03::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		//initializing members of parent class
		initialize(record);

		//creating update record
		std::shared_ptr<PhysicalRecord> newRecord = std::make_shared<PhysicalRecord>();

		//updating location
		newRecord->setLocation(GetNewLocation(record, secondsToProject));
		//setting private matrix
		setPrivateMatrix(secondsToProject);

		// Necessary to stop orientation from changing with 0 time passing.
		newRecord->setOrientation((secondsToProject == 0) ? record->getOrientation() : GetNewOrientation());

		//applying update record to actual record
		record->Update(newRecord);
	}

	unsigned char DeadReckoningRPW03::GetAlgorithm()
	{
		return 3;
	}

	bool DeadReckoningRPW03::Equals(std::any obj)
	{

		if (obj.type() == typeid(std::shared_ptr<DeadReckoningRPW03>))
		{
			std::shared_ptr<DeadReckoningRPW03> other = std::any_cast<std::shared_ptr<DeadReckoningRPW03>>(obj);
			if (rpwMatrix->size() == other->rpwMatrix->size() ||
				(*rpwMatrix)[0].size() == (*other->rpwMatrix)[0].size())
			{
				for (int i = 0; i < rpwMatrix->size(); i++) 
				{
					for (int x = 0; x < (*rpwMatrix)[0].size(); x++) 
					{
						if ((*rpwMatrix)[i, x] != (*other->rpwMatrix)[i, x])
						{
							return false;
						}
					}
				}
				return true;
			}
		}

		return false;
	}

	bool DeadReckoningRPW03::operator == (const DeadReckoningRPW03 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningRPW03>(dr2));
	}

	bool DeadReckoningRPW03::operator != (const DeadReckoningRPW03 &dr2)
	{
		return !(*this == dr2);
	}
}
