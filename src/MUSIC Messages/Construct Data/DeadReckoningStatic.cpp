/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DeadReckoningStatic.h"

namespace MUSICLibrary::MUSIC_Messages
{

	void DeadReckoningStatic::ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject)
	{
		//Static means no movement, therefore nothing is done
	}

	unsigned char DeadReckoningStatic::GetAlgorithm()
	{
		return 1;
	}

	bool DeadReckoningStatic::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningStatic>))
		{
			return true;
		}

		return false;
	}

	bool DeadReckoningStatic::operator == (const DeadReckoningStatic &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningStatic>(dr2));
	}

	bool DeadReckoningStatic::operator != (const DeadReckoningStatic &dr2)
	{
		return !(*this == dr2);
	}
}
