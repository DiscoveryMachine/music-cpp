/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

 /*
	* Licensed to the Apache Software Foundation (ASF) under one or more
	* contributor license agreements.  See the NOTICE file distributed with
	* this work for additional information regarding copyright ownership.
	* The ASF licenses this file to You under the Apache License, Version 2.0
	* (the "License"); you may not use this file except in compliance with
	* the License.  You may obtain a copy of the License at
	*
	*      http://www.apache.org/licenses/LICENSE-2.0
	*
	* Unless required by applicable law or agreed to in writing, software
	* distributed under the License is distributed on an "AS IS" BASIS,
	* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	* See the License for the specific language governing permissions and
	* limitations under the License.
	*/

#include "EulerAngleHelper.h"

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	EulerAngleHelper::EulerAngleHelper(std::shared_ptr<std::array<std::array<double,3>,3>> m, double threshold)
	{
		std::vector<std::vector<double>> oMatrix = OrthogonalizeMatrix(m, threshold);

		double det =
			oMatrix[0][0] * (oMatrix[1][1] * oMatrix[2][2] - oMatrix[2][1] * oMatrix[1][2]) -
			oMatrix[1][0] * (oMatrix[0][1] * oMatrix[2][2] - oMatrix[2][1] * oMatrix[0][2]) +
			oMatrix[2][0] * (oMatrix[0][1] * oMatrix[1][2] - oMatrix[1][1] * oMatrix[0][2]);

		if (det < 0.0)
		{
			std::wcout << L"The closest possible orthogonalized matrix has a negative determinant." << std::endl;
		}

		std::vector<double> quat = EulerAngleHelper::ToQuat(oMatrix);
		q0 = quat[0];
		q1 = quat[1];
		q2 = quat[2];
		q3 = quat[3];
	}

	EulerAngleHelper::EulerAngleHelper(double alpha1, double alpha2, double alpha3)
	{
		std::shared_ptr<EulerAngleHelper> helper1 = std::make_shared<EulerAngleHelper>(V1, alpha1);
		std::shared_ptr<EulerAngleHelper> helper2 = std::make_shared<EulerAngleHelper>(V2, alpha2);
		std::shared_ptr<EulerAngleHelper> helper3 = std::make_shared<EulerAngleHelper>(V3, alpha3);
		std::shared_ptr<EulerAngleHelper> composed = helper1->Compose(helper2->Compose(helper3, false), false);
		q0 = composed->q0;
		q1 = composed->q1;
		q2 = composed->q2;
		q3 = composed->q3;
	}

	EulerAngleHelper::EulerAngleHelper(std::shared_ptr<MUSICVector3> axis, double angle)
	{
		double x = axis->getX();
		double y = axis->getY();
		double z = axis->getZ();

		// Euclidean norm
		double norm = std::sqrt(x * x + y * y + z * z);
		if (norm == 0)
		{
			std::wcout << L"The norm for this rotation axis is 0" << std::endl;
		}

		double coefficient = std::sin(+.5 * angle) / norm;

		q0 = std::cos(+.5 * angle);
		q1 = coefficient * x;
		q2 = coefficient * y;
		q3 = coefficient * z;
	}

	std::shared_ptr<EulerAngleHelper> EulerAngleHelper::ComposeInternal(std::shared_ptr<EulerAngleHelper> h)
	{
		std::shared_ptr<EulerAngleHelper> newHelper = std::make_shared<EulerAngleHelper>(
			h->q0 * q0 - (h->q1 * q1 + h->q2 * q2 + h->q3 * q3), 
			h->q1 * q0 + h->q0 * q1 + (h->q2 * q3 - h->q3 * q2), 
			h->q2 * q0 + h->q0 * q2 + (h->q3 * q1 - h->q1 * q3), 
			h->q3 * q0 + h->q0 * q3 + (h->q1 * q2 - h->q2 * q1), false);

		return newHelper;
	}

	EulerAngleHelper::EulerAngleHelper(double q0, double q1, double q2, double q3, bool normalize)
	{
		if (normalize)
		{
			double adjustment = 1.0 / std::sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
			q0 *= adjustment;
			q1 *= adjustment;
			q2 *= adjustment;
			q3 *= adjustment;
		}

		this->q0 = q0;
		this->q1 = q1;
		this->q2 = q2;
		this->q3 = q3;
	}

	std::vector<std::vector<double>> EulerAngleHelper::OrthogonalizeMatrix(std::shared_ptr<std::array<std::array<double,3>,3>> m, double threshold)
	{
		std::array<double, 3> m0 = (*m)[0];
		std::array<double, 3> m1 = (*m)[1];
		std::array<double, 3> m2 = (*m)[2];
		double x00 = m0[0];
		double x01 = m0[1];
		double x02 = m0[2];
		double x10 = m1[0];
		double x11 = m1[1];
		double x12 = m1[2];
		double x20 = m2[0];
		double x21 = m2[1];
		double x22 = m2[2];
		double fn = 0;
		double fn1;

		std::vector<std::vector<double>> o = RectangularVectors::RectangularDoubleVector(3, 3);

		int counter = 0;

		// iterative correction: Xn+1 = Xn - 0.5 * (Xn.Mt.Xn - M)
		while (++counter < 11)
		{
			// Mt.Xn
			double mx00 = m0[0] * x00 + m1[0] * x10 + m2[0] * x20;
			double mx10 = m0[1] * x00 + m1[1] * x10 + m2[1] * x20;
			double mx20 = m0[2] * x00 + m1[2] * x10 + m2[2] * x20;
			double mx01 = m0[0] * x01 + m1[0] * x11 + m2[0] * x21;
			double mx11 = m0[1] * x01 + m1[1] * x11 + m2[1] * x21;
			double mx21 = m0[2] * x01 + m1[2] * x11 + m2[2] * x21;
			double mx02 = m0[0] * x02 + m1[0] * x12 + m2[0] * x22;
			double mx12 = m0[1] * x02 + m1[1] * x12 + m2[1] * x22;
			double mx22 = m0[2] * x02 + m1[2] * x12 + m2[2] * x22;

			// Xn+1
			o[0][0] = x00 - 0.5 * (x00 * mx00 + x01 * mx10 + x02 * mx20 - m0[0]);
			o[0][1] = x01 - 0.5 * (x00 * mx01 + x01 * mx11 + x02 * mx21 - m0[1]);
			o[0][2] = x02 - 0.5 * (x00 * mx02 + x01 * mx12 + x02 * mx22 - m0[2]);
			o[1][0] = x10 - 0.5 * (x10 * mx00 + x11 * mx10 + x12 * mx20 - m1[0]);
			o[1][1] = x11 - 0.5 * (x10 * mx01 + x11 * mx11 + x12 * mx21 - m1[1]);
			o[1][2] = x12 - 0.5 * (x10 * mx02 + x11 * mx12 + x12 * mx22 - m1[2]);
			o[2][0] = x20 - 0.5 * (x20 * mx00 + x21 * mx10 + x22 * mx20 - m2[0]);
			o[2][1] = x21 - 0.5 * (x20 * mx01 + x21 * mx11 + x22 * mx21 - m2[1]);
			o[2][2] = x22 - 0.5 * (x20 * mx02 + x21 * mx12 + x22 * mx22 - m2[2]);

			// correction on each element
			double corr00 = o[0][0] - m0[0];
			double corr01 = o[0][1] - m0[1];
			double corr02 = o[0][2] - m0[2];
			double corr10 = o[1][0] - m1[0];
			double corr11 = o[1][1] - m1[1];
			double corr12 = o[1][2] - m1[2];
			double corr20 = o[2][0] - m2[0];
			double corr21 = o[2][1] - m2[1];
			double corr22 = o[2][2] - m2[2];

			// Frobenius norm of the correction
			fn1 = 
				corr00 * corr00 + corr01 * corr01 + corr02 * corr02 + 
				corr10 * corr10 + corr11 * corr11 + corr12 * corr12 + 
				corr20 * corr20 + corr21 * corr21 + corr22 * corr22;

			if (std::abs(fn1 - fn) <= threshold)
			{
				return o;
			}

			// prepare for the next iteration.
			x00 = o[0][0];
			x01 = o[0][1];
			x02 = o[0][2];
			x10 = o[1][0];
			x11 = o[1][1];
			x12 = o[1][2];
			x20 = o[2][0];
			x21 = o[2][1];
			x22 = o[2][2];
			fn = fn1;
		}

		// Converge was not successful
		std::wcout << L"This matrix was not able to be orthogonalized." << std::endl;
		return std::vector<std::vector<double>>();
	}

	std::vector<double> EulerAngleHelper::GetRow(std::vector<std::vector<double>> &m, int rowNumber)
	{
		std::vector<double> rowArr = { 0, 0, 0 };
		for (int i = 0; i < m[rowNumber].size(); i++) 
		{
			rowArr[i] = m[rowNumber][i];
		}
		
		return rowArr;
	}

	std::vector<double> EulerAngleHelper::ToQuat(std::vector<std::vector<double>> ort)
	{
		std::vector<double> quat = { 0, 0, 0, 0 };

		double s = ort[0][0] + ort[1][1] + ort[2][2];
		if (s > -0.19)
		{
			quat[0] = 0.5 * std::sqrt(s + 1.0);
			double inv = 0.25 / quat[0];
			quat[1] = inv * (ort[1][2] - ort[2][1]);
			quat[2] = inv * (ort[2][0] - ort[0][2]);
			quat[3] = inv * (ort[0][1] - ort[1][0]);
			return quat;
		}

		s = ort[0][0] - ort[1][1] - ort[2][2];
		if (s > -0.19)
		{
			quat[1] = 0.5 * std::sqrt(s + 1.0);
			double inv = 0.25 / quat[1];
			quat[0] = inv * (ort[1][2] - ort[2][1]);
			quat[2] = inv * (ort[0][1] + ort[1][0]);
			quat[3] = inv * (ort[0][2] + ort[2][0]);
			return quat;
		}

		s = ort[1][1] - ort[0][0] - ort[2][2];
		if (s > -0.19)
		{
			quat[2] = 0.5 * std::sqrt(s + 1.0);
			double inv = 0.25 / quat[2];
			quat[0] = inv * (ort[2][0] - ort[0][2]);
			quat[1] = inv * (ort[0][1] + ort[1][0]);
			quat[3] = inv * (ort[2][1] + ort[1][2]);
			return quat;
		}

		s = ort[2][2] - ort[0][0] - ort[1][1];
		quat[3] = 0.5 * std::sqrt(s + 1.0);
		double inv = 0.25 / quat[3];
		quat[0] = inv * (ort[0][1] - ort[1][0]);
		quat[1] = inv * (ort[0][2] + ort[2][0]);
		quat[2] = inv * (ort[2][1] + ort[1][2]);
		return quat;
	}

	std::shared_ptr<MUSICVector3> EulerAngleHelper::ApplyTo(std::shared_ptr<MUSICVector3> mv3)
	{
		double x = mv3->getX();
		double y = mv3->getY();
		double z = mv3->getZ();

		double s = q1 * x + q2 * y + q3 * z;

		return std::make_shared<MUSICVector3>(
			2 * (q0 * (x * q0 - (q2 * z - q3 * y)) + s * q1) - x, 
			2 * (q0 * (y * q0 - (q3 * x - q1 * z)) + s * q2) - y, 
			2 * (q0 * (z * q0 - (q1 * y - q2 * x)) + s * q3) - z);
	}

	std::shared_ptr<EulerAngleHelper> EulerAngleHelper::ApplyTo(std::shared_ptr<EulerAngleHelper> helper)
	{
		return Compose(helper, true);
	}

	std::shared_ptr<EulerAngleHelper> EulerAngleHelper::Compose(std::shared_ptr<EulerAngleHelper> helper, bool boolean)
	{
		return boolean == (true) ? ComposeInternal(helper) : helper->ComposeInternal(shared_from_this());
	}

	std::vector<double> EulerAngleHelper::GetAngles()
	{
		std::shared_ptr<MUSICVector3> vector1 = ApplyTo(V1);
		std::shared_ptr<MUSICVector3> vector2 = ApplyInverseTo(V3);

		if ((vector2->getZ() < -.9999999999) || (vector2->getZ() > .9999999999))
		{
			std::wcout << L"Cardan Euler Singularity is off." << std::endl;
		}

		return std::vector<double> {std::atan2(vector2->getY(), vector2->getX()), -std::asin(vector2->getZ()), std::atan2(vector1->getY(), vector1->getZ())};
	}

	std::shared_ptr<MUSICVector3> EulerAngleHelper::ApplyInverseTo(std::shared_ptr<MUSICVector3> v)
	{
		double x = v->getX();
		double y = v->getY();
		double z = v->getZ();

		double s = q1 * x + q2 * y + q3 * z;
		double m0 = -q0;

		return std::make_shared<MUSICVector3>(
			2 * (m0 * (x * m0 - (q2 * z - q3 * y)) + s * q1) - x, 
			2 * (m0 * (y * m0 - (q3 * x - q1 * z)) + s * q2) - y, 
			2 * (m0 * (z * m0 - (q1 * y - q2 * x)) + s * q3) - z);
	}
}
