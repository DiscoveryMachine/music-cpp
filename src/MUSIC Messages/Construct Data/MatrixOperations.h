/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_MATRIX_OPERATIONS_H
#define DMI_CONSTRUCT_DATA_MATRIX_OPERATIONS_H

#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <memory>
#include <array>

//CURRENTLY IMPLEMENTED FOR ONLY 3X3 MATRICES 

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	class MatrixOperations
	{
		public:
			//returns a matrix of 3x3 with all 0s
			static std::array<std::array<double, 3>, 3> EmptyMatrix();

			//returns a matrix of 3x3 with 1 at the diagonal
			static std::array<std::array<double, 3>, 3> IdentityMatrix();

			//adds 2nd matrix with the 1st matrix
			static std::array<std::array<double, 3>, 3 > MatrixAddition(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix2);

			//subtracts 2nd matrix with the 1st matrix
			static std::array<std::array<double, 3>, 3 > MatrixSubtraction(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix2);

			//multiplies the scalar with the matrix
			static std::array<std::array<double, 3>, 3 > MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, double scalar);
			static std::array<std::array<double, 3>, 3 > MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, float scalar);

			//multiplies the vector with the matrix
			static std::array<double, 3> MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, std::shared_ptr<std::array<double, 3>> arr);
			//multiplies 2nd matrix with the  1st matrix
			static std::array<std::array<double, 3>, 3 > MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix2);

			static std::array<double, 3> VectorAddition(std::array<double, 3> vector1, std::array<double, 3> vector2);

			static std::array<double, 3> VectorMultiplication(std::array<double, 3> vector1, double value);
	};
}

#endif // !DMI_CONSTRUCT_DATA_MATRIX_OPERATIONS_H
