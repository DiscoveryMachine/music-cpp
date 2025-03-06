/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICHeader.h"
#include "../tangible_string_helper.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	unsigned int MUSICHeader::getExerciseID() const
	{
		return ExerciseID;
	}

	void MUSICHeader::setExerciseID(unsigned int value)
	{
		ExerciseID = value;
	}

	unsigned long long MUSICHeader::getTimestamp() const
	{
		return Timestamp;
	}

	void MUSICHeader::setTimestamp(unsigned long long value)
	{
		Timestamp = value;
	}

	MUSICHeader::MUSICHeader(unsigned int exerciseID, unsigned long long timestamp)
	{
		setExerciseID(exerciseID);
		setTimestamp(timestamp);
	}

	std::shared_ptr<object> MUSICHeader::ToJsonObject()
	{
		object jsonVal;

		jsonVal["exerciseID"] = getExerciseID();
		jsonVal["timestamp"] = getTimestamp();

		return std::make_shared<object>(jsonVal);
	}

	int MUSICHeader::GetHashCode() const
	{
		return -2000878970 + std::hash<unsigned int>{}(getExerciseID());
	}

	size_t MUSICHeader::HashFunction::operator()(const MUSICHeader& obj) const
	{
		return obj.GetHashCode();
	}

	bool MUSICHeader::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<MUSICHeader>))
		{
			std::shared_ptr<MUSICHeader> other = std::any_cast<std::shared_ptr<MUSICHeader>>(obj);

			if (getExerciseID() == other->getExerciseID())
			{
				return true;
			}
		}

		return false;
	}

	bool MUSICHeader::operator == (const MUSICHeader &header2)
	{
		return this->Equals(std::make_shared<MUSICHeader>(header2));
	}

	bool MUSICHeader::operator != (const MUSICHeader &header2)
	{
		return !(*this == header2);
	}
}
