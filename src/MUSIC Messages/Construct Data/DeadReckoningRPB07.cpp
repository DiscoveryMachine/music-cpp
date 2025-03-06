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

#include "DeadReckoningRPB07.h"

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	std::shared_ptr<MUSICVector3> DeadReckoningRPB07::GetNewLocation(std::shared_ptr<MUSICVector3> location, std::shared_ptr<MUSICVector3> linearVelocity, std::shared_ptr<MUSICVector3> orientation)
	{
		std::array<double,3> velocityVector = { linearVelocity->getX(), linearVelocity->getY(), linearVelocity->getZ() };

		EulerAngleHelper currOrientation = EulerAngleHelper(orientation->getX(), orientation->getY(), orientation->getZ());

		std::array<double, 3> newCalculation = MatrixOperations::MatrixMultiplication(R1Matrix, std::make_shared<std::array<double, 3>>(velocityVector));

		std::shared_ptr<MUSICVector3> updatedV = currOrientation.ApplyInverseTo(std::make_shared<MUSICVector3>(newCalculation[0], newCalculation[1], newCalculation[2]));

		double newX = location->getX() + updatedV->getX();
		double newY = location->getY() + updatedV->getY();
		double newZ = location->getZ() + updatedV->getZ();

		return std::make_shared<MUSICVector3>(newX, newY, newZ);
	}

	std::shared_ptr<MUSICVector3> DeadReckoningRPB07::GetNewOrientation()
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

	void DeadReckoningRPB07::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		initialize(record);

		MakeR1Matrix(secondsToProject);

		std::shared_ptr<PhysicalRecord> updater = std::make_shared<PhysicalRecord>();

		updater->setLocation(GetNewLocation(record->getLocation(), record->getLinearVelocity(), record->getOrientation()));

		setPrivateMatrix(secondsToProject);

		// Necessary to stop orientation from changing with 0 time passing.
		updater->setOrientation((secondsToProject == 0) ? record->getOrientation() : GetNewOrientation());

		record->Update(updater);
	}

	unsigned char DeadReckoningRPB07::GetAlgorithm()
	{
		return 7;
	}

	bool DeadReckoningRPB07::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningRPB07>))
		{
			return true;
		}

		return false;
	}

	bool DeadReckoningRPB07::operator == (const DeadReckoningRPB07 &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningRPB07>(dr2));
	}

	bool DeadReckoningRPB07::operator != (const DeadReckoningRPB07 &dr2)
	{
		return !(*this == dr2);
	}
}
