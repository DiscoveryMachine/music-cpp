/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DeadReckoningSharedVariables.h"
#include "MatrixOperations.h"

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	double DeadReckoningSharedVariables::minRotationRate = .2 * M_PI / 180;

	void DeadReckoningSharedVariables::initialize(std::shared_ptr<PhysicalRecord> record)
	{
		//used to set up angular and skew matrix
		double angularX = record->getDeadReckoningParameters()->getAngularVelocity()->getX();
		double angularY = record->getDeadReckoningParameters()->getAngularVelocity()->getY();
		double angularZ = record->getDeadReckoningParameters()->getAngularVelocity()->getZ();

		//orientation
		if (InitialOrientation == nullptr)
		{
			InitialOrientation = std::make_shared<EulerAngleHelper>(record->getOrientation()->getX(), record->getOrientation()->getY(), record->getOrientation()->getZ());
		}

		//initializing angular matrix
		if (angularMatrix == nullptr)
		{
			std::array<std::array<double,3>,3> temp = { {0} };
			angularMatrix = std::make_shared<std::array<std::array<double,3>,3>>(temp);

			std::array<double,3> row = {std::pow(angularX, 2), angularX * angularY, angularX * angularZ};
			setRow(angularMatrix, 0, row);

			row = {angularX * angularY, std::pow(angularY, 2), angularY * angularZ};
			setRow(angularMatrix, 1, row);

			row = {angularX * angularZ, angularY * angularZ, std::pow(angularZ, 2)};
			setRow(angularMatrix, 2, row);
		}

		//initializing matrix
		if (skewMatrix == nullptr)
		{
			std::array<std::array<double,3>,3> temp = { {0} };
			skewMatrix = std::make_shared<std::array<std::array<double,3>,3>>(temp);

			std::array<double,3> row = {0, -angularZ, angularY};
			setRow(skewMatrix, 0, row);

			row = {angularZ, 0, -angularX};
			setRow(skewMatrix, 1, row);

			row = {-angularY, angularX, 0};
			setRow(skewMatrix, 2, row);
		}

		//setting angularSquared and squared root
		if (angularSquared < 0)
		{
			angularSquared = std::pow(angularX, 2) + std::pow(angularY, 2) + std::pow(angularZ, 2);
			angularSquaredRoot = std::sqrt(angularSquared);
		}
	}

	void DeadReckoningSharedVariables::setRow(std::shared_ptr<std::array<std::array<double,3>,3>> mat, int row, std::array<double, 3> rowVals)
	{
		(*mat)[row][0] = rowVals.at(0);
		(*mat)[row][1] = rowVals.at(1);
		(*mat)[row][2] = rowVals.at(2);
	}

	void DeadReckoningSharedVariables::setPrivateMatrix(float seconds)
	{

		//if(angular velocity magnitude < minRotationRate) → get an identity matrix
		if (angularSquaredRoot < minRotationRate)
		{
			std::array<std::array<double, 3>, 3> temp = MatrixOperations::IdentityMatrix();
			rpwMatrix = std::make_shared < std::array<std::array<double, 3>, 3>>(temp);
			return;
		}

		double delta = angularSquaredRoot * seconds; //if update count is relevant multiply delta by updateCount
		double cosDelta = std::cos(delta);

		//getting scalar values based on delta
		double angularScalar = (1 - cosDelta) / angularSquared;
		double identityScalar = cosDelta;
		double skewScalar = std::sin(delta) / angularSquaredRoot;

		//applying scalars to temp matrices
		std::shared_ptr<std::array<std::array<double,3>,3>> angularTmp = std::make_shared<std::array<std::array<double,3>,3>>(MatrixOperations::MatrixMultiplication(angularMatrix, angularScalar));
		std::shared_ptr<std::array<std::array<double,3>,3>> skewTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(skewMatrix, skewScalar));

		std::shared_ptr<std::array<std::array<double,3>,3>> temp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::IdentityMatrix());
		std::shared_ptr<std::array<std::array<double,3>,3>> identTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(temp, identityScalar));

		//applying temp matricies to private matrix
		rpwMatrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixAddition(angularTmp, identTmp));

		rpwMatrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixSubtraction(rpwMatrix, skewTmp));
	}

	void DeadReckoningSharedVariables::MakeR1Matrix(float seconds)
	{
		//if(angular velocity magnitude < minRotationRate) → get an identity matrix
		if (angularSquaredRoot < minRotationRate)
		{
			std::shared_ptr<std::array<std::array<double, 3>, 3>> iMatrix = std::make_shared < std::array<std::array<double, 3>, 3>>(MatrixOperations::IdentityMatrix());
			R1Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(iMatrix, seconds)); // if update count is relevant, then don't use 0
			return;
		}

		std::shared_ptr<std::array<std::array<double, 3>, 3>> ident = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::IdentityMatrix());

		double delta = angularSquaredRoot * seconds; //if update count is relevant multiply delta by updateCount

		//getting scalar values based on delta
		double angularScalar = (delta - std::sin(delta)) / (angularSquared * angularSquaredRoot);
		double identityScalar = std::sin(delta) / angularSquaredRoot;
		double skewScalar = (1.0 - std::cos(delta)) / angularSquared;

		////applying scalars to temp matrices
		std::shared_ptr<std::array<std::array<double, 3>, 3>> angularTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(angularMatrix, angularScalar));

		std::shared_ptr<std::array<std::array<double, 3>, 3>> identTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(ident, identityScalar));

		std::shared_ptr<std::array<std::array<double, 3>, 3>> skewTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(skewMatrix, skewScalar));

		//applying temp matricies to private matrix

		R1Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixAddition(angularTmp, identTmp));
		R1Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixAddition(R1Matrix, skewTmp));
	}

	void DeadReckoningSharedVariables::MakeR2Matrix(float seconds)
	{
		//if(angular velocity magnitude < minRotationRate) → get an identity matrix
		if (angularSquaredRoot < minRotationRate)
		{
			R2Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::IdentityMatrix()), (seconds * seconds / 2.0))); // if update count is relevant, then don't use 0
			return;
		}

		double delta = angularSquaredRoot * seconds;

		double angularScalar = .5 * angularSquared * seconds * seconds;
		angularScalar -= std::cos(delta);
		angularScalar -= delta * std::sin(delta);
		angularScalar++;
		angularScalar /= angularSquared * angularSquared;

		double identityScalar = std::cos(delta) + delta * std::sin(delta) - 1;
		identityScalar /= angularSquared;

		double skewScalar = std::sin(delta) - delta * std::cos(delta);
		skewScalar /= angularSquared * angularSquaredRoot;

		//applying scalars to temp matrices
		std::shared_ptr<std::array<std::array<double, 3>, 3>> angularTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(angularMatrix, angularScalar));

		std::shared_ptr<std::array<std::array<double, 3>, 3>> temp = std::make_shared < std::array<std::array<double, 3>, 3>>(MatrixOperations::IdentityMatrix());
		std::shared_ptr<std::array<std::array<double, 3>, 3>> identTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(temp, identityScalar));

		std::shared_ptr<std::array<std::array<double, 3>, 3>> skewTmp = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixMultiplication(skewMatrix, skewScalar));

		R2Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixAddition(angularTmp, identTmp));
		R2Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::MatrixAddition(R2Matrix, skewTmp));
	}
}
