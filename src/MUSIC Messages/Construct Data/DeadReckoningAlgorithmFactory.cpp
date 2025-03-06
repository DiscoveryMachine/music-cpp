/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "DeadReckoningAlgorithmFactory.h"

using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;

namespace MUSICLibrary::MUSIC_Messages
{
	std::shared_ptr<IDeadReckoningAlgorithm> DeadReckoningAlgorithmFactory::getAlgorithm(int algoValue)
	{
		switch (algoValue)
		{
		case 1:
			return std::make_shared<DeadReckoningStatic>();
		case 2:
			return std::make_shared<DeadReckoningFPW02>();
		case 3:
			return std::make_shared<DeadReckoningRPW03>();
		case 4:
			return std::make_shared<DeadReckoningRVW04>();
		case 5:
			return std::make_shared<DeadReckoningFVW05>();
		case 6:
			return std::make_shared<DeadReckoningFPB06>();
		case 7:
			return std::make_shared<DeadReckoningRPB07>();
		case 8:
			return std::make_shared<DeadReckoningRVB08>();
		case 9:
			return std::make_shared<DeadReckoningFVB09>();
		default:
			return nullptr;
		}
	}
}