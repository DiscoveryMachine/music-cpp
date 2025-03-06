/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_IDEAD_RECKONING_ALGORITHM_H
#define DMI_CONSTRUCT_DATA_IDEAD_RECKONING_ALGORITHM_H

#include <memory>


namespace MUSICLibrary::MUSIC_Messages
{
	class PhysicalRecord;
}

namespace MUSICLibrary::MUSIC_Messages::Construct_Data
{
	class IDeadReckoningAlgorithm
	{
	public:
		virtual void ApplyDeadReckoning(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PhysicalRecord> record, float secondsToProject) {};

		virtual unsigned char GetAlgorithm() 
		{
			return 0;
		}
	};
}

#endif // !DMI_CONSTRUCT_DATA_IDEAD_RECKONING_ALGORITHM_H