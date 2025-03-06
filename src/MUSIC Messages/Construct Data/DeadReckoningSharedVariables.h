/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_DEAD_RECKONING_SHARED_VARIABLES_H
#define DMI_CONSTRUCT_DATA_DEAD_RECKONING_SHARED_VARIABLES_H

//System files
#define _USE_MATH_DEFINES
#include "EulerAngleHelper.h"
#include "PhysicalRecord.h"
#include "MatrixOperations.h"


namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	class DeadReckoningSharedVariables
	{
	public:
		static double minRotationRate;
		double angularSquared = -1;
		double angularSquaredRoot = 0.0;

		std::shared_ptr<std::array<std::array<double, 3>, 3>> R1Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::EmptyMatrix());
		std::shared_ptr<std::array<std::array<double, 3>, 3>> R2Matrix = std::make_shared<std::array<std::array<double, 3>, 3>>(MatrixOperations::EmptyMatrix());

		std::shared_ptr<EulerAngleHelper> InitialOrientation;

		std::shared_ptr<std::array<std::array<double, 3>, 3>> angularMatrix;
		std::shared_ptr<std::array<std::array<double, 3>, 3>> skewMatrix;
		std::shared_ptr<std::array<std::array<double, 3>, 3>> rpwMatrix = { {0} };

		void initialize(std::shared_ptr<PhysicalRecord> record);
		void setRow(std::shared_ptr<std::array<std::array<double, 3>, 3>> mat, int row, std::array<double, 3> rowVals);
		void setPrivateMatrix(float seconds);

		void MakeR1Matrix(float seconds);
		void MakeR2Matrix(float seconds);
	};
}

#endif // !DMI_CONSTRUCT_DATA_DEAD_RECKONING_SHARED_VARIABLES_H