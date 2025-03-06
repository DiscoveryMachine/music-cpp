/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_DEAD_RECKONING_STATIC_H
#define DMI_CONSTRUCT_DATA_DEAD_RECKONING_STATIC_H

#include <any>
#include "IDeadReckoningAlgorithm.h"
#include "PhysicalRecord.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class DeadReckoningStatic : public MUSICLibrary::MUSIC_Messages::Construct_Data::IDeadReckoningAlgorithm
	{
	public:
		void ApplyDeadReckoning(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PhysicalRecord> record, float secondsToProject);

		unsigned char GetAlgorithm() override;

		bool Equals(std::any obj);

		bool operator == (const DeadReckoningStatic &dr2);

		bool operator != (const DeadReckoningStatic &dr2);
	};
}

#endif // !DMI_CONSTRUCT_DATA_DEAD_RECKONING_STATIC_H