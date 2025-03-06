/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ConstructDeadReckoning.h"
#include "../../../../MUSIC Messages/Construct Data/DeadReckoningAlgorithmFactory.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::MUSIC_Messages::Construct_Data;

namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher
{

	void ConstructDeadReckoning::ApplyTo(std::unordered_map<std::shared_ptr<EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> &batch)
	{
		for (auto &[eid, construct] : batch)
		{
			//if construct is local
			if (!construct->IsRemote())
			{
				throw InvalidOperationException();
			}

			std::shared_ptr<ConstructDataMessage> ConstructDataMsg = construct->GetConstructData();

			if (ConstructDataMsg->getPhysical() != nullptr && (ConstructDataMsg->getPhysical()->getDeadReckoningParameters()) != nullptr && ConstructDataMsg->getPhysical()->getDeadReckoningParameters()->getAlgorithmValue())
			{
				std::shared_ptr<IDeadReckoningAlgorithm> algo = DeadReckoningAlgorithmFactory::getAlgorithm(ConstructDataMsg->getPhysical()->getDeadReckoningParameters()->getAlgorithmValue()); //ConstructDataMsg->getPhysical()->getDeadReckoningParameters()->getAlgorithm();
				algo->ApplyDeadReckoning(ConstructDataMsg->getPhysical(), static_cast<float>(1.0 / FPS));
			}
		}
	}
}
