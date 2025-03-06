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

#include "DeadReckoningRVB08.h"

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{

	void DeadReckoningRVB08::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		initialize(record);

		MakeR1Matrix(secondsToProject);
		MakeR2Matrix(secondsToProject);

		std::shared_ptr<PhysicalRecord> updater = std::make_shared<PhysicalRecord>();

		updater->setLocation(GetNewLocation(record, secondsToProject));

		setPrivateMatrix(secondsToProject);

		updater->setLinearVelocity(GetNewLinearVelocity(record, secondsToProject));

		// Necessary to stop orientation from changing with 0 time passing.
		updater->setOrientation((secondsToProject == 0) ? record->getOrientation() : GetNewOrientation());

		record->Update(updater);
	}

	std::shared_ptr<MUSICVector3> DeadReckoningRVB08::GetNewLocation(std::shared_ptr<PhysicalRecord> record, float seconds)
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

		std::array<double,3> velocityVector = { velX, velY, velZ };
		std::array<double, 3> accelerationVector = { accX, accY, accZ };

		std::shared_ptr<EulerAngleHelper> currOrien = std::make_shared<EulerAngleHelper>(record->getOrientation()->getX(), record->getOrientation()->getY(), record->getOrientation()->getZ());

		MakeR1Matrix(seconds);
		MakeR2Matrix(seconds);

		std::array<double, 3> updated1 = MatrixOperations::MatrixMultiplication(R1Matrix, std::make_shared<std::array<double,3>>(velocityVector));
		std::array<double, 3> updated2 = MatrixOperations::MatrixMultiplication(R2Matrix, std::make_shared<std::array<double, 3>>(accelerationVector));

		std::array<double, 3> vectorsAdded = MatrixOperations::VectorAddition(updated1, updated2);
		std::shared_ptr<MUSICVector3> vectorsAdded3D = std::make_shared<MUSICVector3>(vectorsAdded[0], vectorsAdded[1], vectorsAdded[2]);

		std::shared_ptr<MUSICVector3> updated = currOrien->ApplyInverseTo(vectorsAdded3D);

		std::shared_ptr<MUSICVector3> newLoc = std::make_shared<MUSICVector3>(locX + updated->getX(), locY + updated->getY(), locZ + updated->getZ());

		return newLoc;
	}

	std::shared_ptr<MUSICVector3> DeadReckoningRVB08::GetNewLinearVelocity(std::shared_ptr<PhysicalRecord> record, float seconds)
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

	std::shared_ptr<MUSICVector3> DeadReckoningRVB08::GetNewOrientation()
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

	unsigned char DeadReckoningRVB08::GetAlgorithm()
	{
		return 8;
	}

	bool DeadReckoningRVB08::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningRVB08>))
		{
			return true;
		}

		return false;
	}

	bool DeadReckoningRVB08::operator == (const DeadReckoningRVB08 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningRVB08>(dr2));
	}

	bool DeadReckoningRVB08::operator != (const DeadReckoningRVB08 &dr2)
	{
		return !(*this == dr2);
	}
}
