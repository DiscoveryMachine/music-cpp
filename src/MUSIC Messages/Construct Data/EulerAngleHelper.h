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

#ifndef DMI_CONSTRUCT_DATA_EULER_ANGLE_HELPER_H
#define DMI_CONSTRUCT_DATA_EULER_ANGLE_HELPER_H

#include <vector>
#include <iostream>
#include <cmath>
#include <array>
#include "../MUSICVector3.h"
#include "../../tangible_rectangular_vectors.h"


// This class was originally Rotation.java from a library we used in Java to
// get the job done.  Unfortunately, the library did not exist in C# at the 
// time of writing this so we brought over the bare minimum required and made changes
// where necessary.  The original Rotation class can be found at the following link
// for a better understanding of what's going on here:
// https://commons.apache.org/proper/commons-math/javadocs/api-3.6.1/org/apache/commons/math3/geometry/euclidean/threed/package-summary.html

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	class EulerAngleHelper : public std::enable_shared_from_this<EulerAngleHelper>
	{
		/** Scalar coordinate of the quaternion. */
	private:
		double q0 = 0.0;

		/** First coordinate of the vector part of the quaternion. */
		double q1 = 0.0;

		/** Second coordinate of the vector part of the quaternion. */
		double q2 = 0.0;

		/** Third coordinate of the vector part of the quaternion. */
		double q3 = 0.0;

		// The following values are needed to determine rotation order and ultimately
		// set the initial orientation and should not change.
		const std::shared_ptr<MUSICVector3> V1 = std::make_shared<MUSICVector3>(0.0, 0.0, 1.0); // PLUS_K
		const std::shared_ptr<MUSICVector3> V2 = std::make_shared<MUSICVector3>(0.0, 1.0, 0.0); // PLUS_J
		const std::shared_ptr<MUSICVector3> V3 = std::make_shared<MUSICVector3>(1.0, 0.0, 0.0); // PLUS_I

		// Multidemensional array approach
	public:
		EulerAngleHelper(std::shared_ptr<std::array<std::array<double, 3>, 3>> m, double threshold);

		EulerAngleHelper(double alpha1, double alpha2, double alpha3);

		EulerAngleHelper(std::shared_ptr<MUSICVector3> axis, double angle);

	private:
		std::shared_ptr<EulerAngleHelper> ComposeInternal(std::shared_ptr<EulerAngleHelper> h);

	public:
		EulerAngleHelper(double q0, double q1, double q2, double q3, bool normalize);

	private:
		std::vector<std::vector<double>> OrthogonalizeMatrix(std::shared_ptr<std::array<std::array<double, 3>, 3>> m, double threshold);

		std::vector<double> GetRow(std::vector<std::vector<double>> &m, int rowNumber);

		std::vector<double> ToQuat(std::vector<std::vector<double>> ort);

	public:
		std::shared_ptr<MUSICVector3> ApplyTo(std::shared_ptr<MUSICVector3> mv3);

		std::shared_ptr<EulerAngleHelper> ApplyTo(std::shared_ptr<EulerAngleHelper> helper);

		// true boolean represents Vector Operator, false is Frame Transform.
		std::shared_ptr<EulerAngleHelper> Compose(std::shared_ptr<EulerAngleHelper> helper, bool boolean);

		//Does not yet require parameters as rotating convention
		// is always expected to be a Frame Transformation and rotating order is always ZYX.
		std::vector<double> GetAngles();

		std::shared_ptr<MUSICVector3> ApplyInverseTo(std::shared_ptr<MUSICVector3> v);
	};
}

#endif // !DMI_CONSTRUCT_DATA_EULER_ANGLE_HELPER_H