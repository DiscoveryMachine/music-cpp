/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MatrixOperations.h"

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	//Empty Matrix 
	std::array<std::array<double, 3>, 3> MatrixOperations::EmptyMatrix()
	{
		std::array<std::array<double, 3>, 3> identity = { {0,0,0} };
		identity[1] = { 0,0,0 };
		identity[2] = { 0,0,0 };

		return identity;
	}

	//Identity Matrix 
	std::array<std::array<double, 3>, 3> MatrixOperations::IdentityMatrix()
	{
		std::array<std::array<double, 3>, 3> identity = { {1,0,0} };
		identity[1] = { 0,1,0 };
		identity[2] = { 0,0,1 };

		return identity;
	}

	//Scalar matrix multilplication
	std::array<std::array<double, 3>, 3> MatrixOperations::MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3>> matrix, double scalar)
	{
		std::array<std::array<double, 3>, 3 > temp = { {0} };
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				temp[row][col] = (*matrix)[row][col] * scalar;
			}
		}

		return temp;
	}

	std::array<std::array<double, 3>, 3> MatrixOperations::MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3>> matrix, float scalar)
	{
		std::array<std::array<double, 3>, 3 > temp = { {0} };
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				temp[row][col] = (*matrix)[row][col] * scalar;
			}
		}

		return temp;
	}

	//Vector matrix mulitiplication
	std::array<double, 3> MatrixOperations::MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3>> matrix, std::shared_ptr<std::array<double, 3>> arr)
	{
		std::array<double, 3 > temp = { 0 };
		for (int row = 0; row < 3; row++)
		{
			double val = 0;
			for (int col = 0; col < 3; col++)
			{
				val += (*matrix)[row][col] * (*arr)[col];
			}
			temp[row] = val;
		}

		return temp;
	}

	//Matrix by Matrix multiplication
	std::array<std::array<double, 3>, 3 > MatrixOperations::MatrixMultiplication(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, std::shared_ptr<std::array<std::array<double, 3>, 3>> matrix2)
	{
		std::array<std::array<double, 3>, 3 > temp = { {0} };

		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				for (int z = 0; z < 3; z++)
				{
					temp[x][y] += (*matrix)[x][z] * (*matrix2)[z][y];
				}
			}
		}

		return temp;
	}

	//Addition of Matrix by Matrix
	std::array<std::array<double, 3>, 3 > MatrixOperations::MatrixAddition(std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix, std::shared_ptr<std::array<std::array<double, 3>, 3 >> matrix2) 
	{
		std::array<std::array<double, 3>, 3 > temp = { {0} };

		for (int row = 0; row < 3; row++) 
		{
			for (int col = 0; col < 3; col++)
			{
				temp[row][col] = (*matrix)[row][col] + (*matrix2)[row][col];
			}
		}

		return temp;
	}

	//Subtraction of Matrix by Matrix
	std::array<std::array<double, 3>, 3> MatrixOperations::MatrixSubtraction(std::shared_ptr<std::array<std::array<double, 3>, 3>> matrix, std::shared_ptr<std::array<std::array<double, 3>, 3>> matrix2)
	{
		std::array<std::array<double, 3>, 3 > temp = { {0} };

		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				temp[row][col] = (*matrix)[row][col] - (*matrix2)[row][col];
			}
		}

		return temp;
	}

	//Addition of vector by vector
	std::array<double, 3> MatrixOperations::VectorAddition(std::array<double, 3> vector1, std::array<double, 3> vector2)
	{
		return {vector1[0] + vector2[0],
				vector1[1] + vector2[1],
				vector1[2] + vector2[2] };
	}

	//Multiplying vector by a scalar
	std::array<double, 3> MatrixOperations::VectorMultiplication(std::array<double, 3> vector, double value)
	{
		return { vector[0] * value,
				 vector[1] * value,
				 vector[2] * value };
	}
}

