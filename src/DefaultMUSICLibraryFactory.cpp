/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DefaultMUSICLibraryFactory.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MessageFilters;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::Visitors;
using namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Factory;
using namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository;

namespace MUSICLibrary
{

	std::shared_ptr<IMUSICLibrary> DefaultMUSICLibraryFactory::Create(unsigned int exerciseID, std::shared_ptr<SiteAndAppID> siteApp, unsigned int numLocalBatches, unsigned int numRemoteBatches, unsigned int localBatcherInterval, unsigned int remoteBatcherInterval)
	{
		std::shared_ptr<DefaultInternalVisitorFactory> internalFactory = std::make_shared<DefaultInternalVisitorFactory>();
		std::shared_ptr<DefaultInternalMessageVisitor> dimv = std::static_pointer_cast<DefaultInternalMessageVisitor>(internalFactory->Create(exerciseID, siteApp));
		std::shared_ptr<DefaultMUSICLibrary> lib = std::make_shared<DefaultMUSICLibrary>(siteApp, exerciseID, std::make_shared<DefaultMessageFilter>(siteApp, exerciseID), dimv);
		std::shared_ptr<ConstructRepository> repository = std::make_shared<ConstructRepository>(lib, numLocalBatches, numRemoteBatches, std::make_shared<DefaultConstructBatcher>(localBatcherInterval), std::make_shared<DefaultConstructBatcher>(localBatcherInterval));
		std::shared_ptr<ConstructFactory> constructFactory = std::make_shared<ConstructFactory>(repository, lib);
		dimv->Initialize(constructFactory, repository);
		return lib;
	}
}
