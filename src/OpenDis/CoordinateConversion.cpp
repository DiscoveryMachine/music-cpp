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

#include "CoordinateConversion.h"

namespace MUSICLibrary::MUSIC_Messages::OpenDis
{
	std::vector<double> CoordinateConversions::xyzToLatLonRadians(double xyz[3]) 
	{
		double x = xyz[0];
		double y = xyz[1];
		double z = xyz[2];
		static std::vector<double> answer = { 0,0,0 };
		const double a = 6378137.0; //semi major axis
		const double b = 6356752.3142; //semi minor axis

		double eSquared; //first eccentricity squared
		double rSubN; //radius of the curvature of the prime vertical
		double ePrimeSquared; //second eccentritiy squared
		double w = sqrt((x*x) + (y*y));

		eSquared = ((a*a) - (b*b)) / (a*a);
		ePrimeSquared = ((a*a) - (b*b)) / (b*b);

		answer[1] = (x >= 0) ? atan(y / x) :
			(x < 0 && y >= 0) ? atan(y / x) + M_PI :
			atan(y / x) - M_PI;

		answer[1] = answer[1] * 180.0 / M_PI;

		double tanBZero = a*z / (b*w);
		double BZero = atan(tanBZero);
		double tanPhi = (z + (ePrimeSquared * b * pow(sin(BZero), 3))) / 
			(w - (a * eSquared * pow(cos(BZero),3)));
		double phi = atan(tanPhi);
		answer[0] = phi * 180.0/ M_PI;

		rSubN = (a*a / sqrt((a*a) * (cos(phi) * cos(phi)))) / 
		(b*b * (sin(phi) * sin(phi)));
		answer[2] = (w / cos(phi)) - rSubN;

		return answer;
	}
}