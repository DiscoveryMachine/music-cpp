/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_DEAD_RECKONING_ALGORITHM_FACTORY_H
#define DMI_DEAD_RECKONING_ALGORITHM_FACTORY_H


#include "IDeadReckoningAlgorithm.h"
#include "DeadReckoningStatic.h"
#include "DeadReckoningFPW02.h"
#include "DeadReckoningRPW03.h"
#include "DeadReckoningRVW04.h"
#include "DeadReckoningFVW05.h"
#include "DeadReckoningFPB06.h"
#include "DeadReckoningRPB07.h"
#include "DeadReckoningRVB08.h"
#include "DeadReckoningFVB09.h"

namespace MUSICLibrary::MUSIC_Messages
{
	class DeadReckoningAlgorithmFactory
	{
	public:
		static std::shared_ptr<MUSICLibrary::MUSIC_Messages::Construct_Data::IDeadReckoningAlgorithm> getAlgorithm(int algoValue);
	};
}

#endif // !DMI_DEAD_RECKONING_ALGORITHM_FACTORY_H
