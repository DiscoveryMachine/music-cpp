/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_PERCEPTION_DATA_PERCEPTION_ERRORS_H
#define DMI_PERCEPTION_DATA_PERCEPTION_ERRORS_H

#include <string>
#include <codecvt>
#include "../../tangible_string_helper.h"
#include "../../Interfaces/IToJSON.h"

namespace MUSICLibrary::MUSIC_Messages
{
	class PerceptionErrors : public MUSICLibrary::Interfaces::IToJSON
	{
	private:
		unsigned int bearingError = 0;
		unsigned int altitudeError = 0;
		unsigned int rangeError = 0;

	public:
		unsigned int getBearingError() const;
		void setBearingError(unsigned int value);
		unsigned int getAltitudeError() const;
		void setAltitudeError(unsigned int value);
		unsigned int getRangeError() const;
		void setRangeError(unsigned int value);

		// Constants for Key look ups
		static const std::wstring BEARING_ERROR;
		static const std::wstring ALTITUDE_ERROR;
		static const std::wstring RANGE_ERROR;

		PerceptionErrors(unsigned int bearingError, unsigned int altitudeError, unsigned int rangeError);

		PerceptionErrors(std::shared_ptr<boost::json::object> jobj);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const PerceptionErrors& obj) const;
		};

		bool operator == (const PerceptionErrors &pe2);

		bool operator != (const PerceptionErrors &pe2);
	};
}

#endif // !DMI_PERCEPTION_DATA_PERCEPTION_ERRORS_H