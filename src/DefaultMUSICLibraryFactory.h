/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_LIBRARY_DEFAULT_MUSIC_LIBRARY_FACTORY_H
#define DMI_MUSIC_LIBRARY_DEFAULT_MUSIC_LIBRARY_FACTORY_H

#include "Interfaces/IMUSICLibraryFactory.h"
#include "DefaultInternalVisitorFactory.h"
#include "MessageFilters/DefaultMessageFilter.h"
#include "DefaultMUSICLibrary.h"
#include "Visitors/Default Internal Message Visitor/Construct Repository/ConstructRepository.h"
#include "Visitors/Default Internal Message Visitor/Construct Repository/DefaultConstructBatcher.h"
#include "Visitors/Default Internal Message Visitor/Construct Factory/ConstructFactory.h"


namespace MUSICLibrary
{
	class DefaultMUSICLibraryFactory : public  MUSICLibrary::Interfaces::IMUSICLibraryFactory
	{
	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICLibrary> Create(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteApp, unsigned int numLocalBatches, unsigned int numRemoteBatches, unsigned int localBatcherInterval, unsigned int remoteBatcherInterval) override;
	};
}

#endif // !DMI_MUSIC_LIBRARY_DEFAULT_MUSIC_LIBRARY_FACTORY_H