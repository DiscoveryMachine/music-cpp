/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ICONSTRUCT_BATCHER_H
#define DMI_INTERFACES_ICONSTRUCT_BATCHER_H

#include "../Visitors/Default Internal Message Visitor/Construct Repository/Batcher/ConstructBatches.h"
#include "IBatchOperation.h"


namespace MUSICLibrary::Interfaces
{
	class IConstructBatcher
	{
	public:
		virtual void StartBatchThreads(std::shared_ptr<IBatchOperation> op, std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> batches) = 0;
	};
}

#endif // !DMI_INTERFACES_ICONSTRUCT_BATCHER_H