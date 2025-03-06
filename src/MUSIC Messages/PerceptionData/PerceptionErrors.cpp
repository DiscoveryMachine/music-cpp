/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PerceptionErrors.h"
#include "../../tangible_string_helper.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	unsigned int PerceptionErrors::getBearingError() const
	{
		return bearingError;
	}

	void PerceptionErrors::setBearingError(unsigned int value)
	{
		bearingError = value;
	}

	unsigned int PerceptionErrors::getAltitudeError() const
	{
		return altitudeError;
	}

	void PerceptionErrors::setAltitudeError(unsigned int value)
	{
		altitudeError = value;
	}

	unsigned int PerceptionErrors::getRangeError() const
	{
		return rangeError;
	}

	void PerceptionErrors::setRangeError(unsigned int value)
	{
		rangeError = value;
	}

	const std::wstring PerceptionErrors::BEARING_ERROR = L"bearingError";
	const std::wstring PerceptionErrors::ALTITUDE_ERROR = L"altitudeError";
	const std::wstring PerceptionErrors::RANGE_ERROR = L"rangeError";

	PerceptionErrors::PerceptionErrors(unsigned int bearingError, unsigned int altitudeError, unsigned int rangeError)
	{
		this->setBearingError(bearingError);
		this->setAltitudeError(altitudeError);
		this->setRangeError(rangeError);
	}

	PerceptionErrors::PerceptionErrors(std::shared_ptr<object> jobj)
	{
		object objVal = *jobj;

		this->setBearingError(value_to<unsigned int>(objVal[StringHelper::wstringToString(BEARING_ERROR)]));
		this->setAltitudeError(value_to<unsigned int>(objVal[StringHelper::wstringToString(ALTITUDE_ERROR)]));
		this->setRangeError(value_to<unsigned int>(objVal[StringHelper::wstringToString(RANGE_ERROR)]));
	}

	std::shared_ptr<object> PerceptionErrors::ToJsonObject()
	{
		object objVal;

		objVal[StringHelper::wstringToString(BEARING_ERROR)] = getBearingError();
		objVal[StringHelper::wstringToString(ALTITUDE_ERROR)] = getAltitudeError();
		objVal[StringHelper::wstringToString(RANGE_ERROR)] = getRangeError();

		return std::make_shared<object>(objVal);
	}

	bool PerceptionErrors::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<PerceptionErrors>))
		{
			std::shared_ptr<PerceptionErrors> other = std::any_cast<std::shared_ptr<PerceptionErrors>>(obj);

			if (getBearingError() == other->getBearingError() && 
				getAltitudeError() == other->getAltitudeError() && 
				getRangeError() == other->getRangeError())
			{
				return true;
			}
		}

		return false;
	}

	int PerceptionErrors::GetHashCode() const
	{
		auto hashCode = 1019388724;
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getBearingError());
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getAltitudeError());
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getRangeError());
		return hashCode;
	}

	size_t PerceptionErrors::HashFunction::operator()(const PerceptionErrors& obj) const
	{
		return obj.GetHashCode();
	}

	bool PerceptionErrors::operator == (const PerceptionErrors &pe2)
	{
		return this->Equals(std::make_shared<PerceptionErrors>(pe2));
	}

	bool PerceptionErrors::operator != (const PerceptionErrors &pe2)
	{
		return !(*this == pe2);
	}
}
