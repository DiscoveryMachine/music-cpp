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

#include "DeadReckoningFVB09.h"
#include "MatrixOperations.h"

using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;

namespace MUSICLibrary::MUSIC_Messages
{

	std::shared_ptr<MUSICVector3> DeadReckoningFVB09::GetNewLocation(std::shared_ptr<PhysicalRecord> record, float seconds)
	{
		double velX = record->getLinearVelocity()->getX();
		double velY = record->getLinearVelocity()->getY();
		double velZ = record->getLinearVelocity()->getZ();

		double accX = record->getDeadReckoningParameters()->getLinearAcceleration()->getX();
		double accY = record->getDeadReckoningParameters()->getLinearAcceleration()->getY();
		double accZ = record->getDeadReckoningParameters()->getLinearAcceleration()->getZ();

		double locX = record->getLocation()->getX();
		double locY = record->getLocation()->getY();
		double locZ = record->getLocation()->getZ();

		std::array<double, 3> velocityVector = { velX, velY, velZ };
		std::array<double, 3> accelerationVector = { accX * (.5 * seconds), accY * (.5 * seconds), accZ * (.5 * seconds) };

		std::array<double, 3> vector3D = (MatrixOperations::VectorAddition(velocityVector, accelerationVector));
		// Could make this a function in the matrix ops file rather than doing it by hand here.
		vector3D[0] *= seconds;
		vector3D[1] *= seconds;
		vector3D[2] *= seconds;

		std::shared_ptr<MUSICVector3> MUSICvector3D = std::make_shared<MUSICVector3>(vector3D[0], vector3D[1], vector3D[2]);

		std::shared_ptr<MUSICVector3> updated = InitialOrientation->ApplyInverseTo(MUSICvector3D);

		std::shared_ptr<MUSICVector3> newLoc = std::make_shared<MUSICVector3>(locX + updated->getX(), locY + updated->getY(), locZ + updated->getZ());

		return newLoc;
	}

	std::shared_ptr<MUSICVector3> DeadReckoningFVB09::GetNewLinearVelocity(std::shared_ptr<PhysicalRecord> record, float seconds)
	{
		double velX = record->getLinearVelocity()->getX();
		double velY = record->getLinearVelocity()->getY();
		double velZ = record->getLinearVelocity()->getZ();

		double accX = record->getDeadReckoningParameters()->getLinearAcceleration()->getX();
		double accY = record->getDeadReckoningParameters()->getLinearAcceleration()->getY();
		double accZ = record->getDeadReckoningParameters()->getLinearAcceleration()->getZ();

		double newVelX = velX + accX * seconds;
		double newVelY = velY + accY * seconds;
		double newVelZ = velZ + accZ * seconds;

		std::shared_ptr<MUSICVector3> newVelocity = std::make_shared<MUSICVector3>(newVelX, newVelY, newVelZ);

		return newVelocity;
	}

	void DeadReckoningFVB09::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		initialize(record);

		std::shared_ptr<PhysicalRecord> updater = std::make_shared<PhysicalRecord>();

		updater->setLocation(GetNewLocation(record, secondsToProject));

		updater->setLinearVelocity(GetNewLinearVelocity(record, secondsToProject));

		record->Update(updater);

	}

	unsigned char DeadReckoningFVB09::GetAlgorithm()
	{
		return 9;
	}

	bool DeadReckoningFVB09::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningFVB09>))
		{
			return true;
		}
		return false;
	}

	bool DeadReckoningFVB09::operator == (const DeadReckoningFVB09 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningFVB09>(dr2));
	}

	bool DeadReckoningFVB09::operator != (const DeadReckoningFVB09 &dr2)
	{
		return !(*this == dr2);
	}
}
