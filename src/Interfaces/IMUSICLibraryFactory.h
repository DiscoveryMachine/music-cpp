/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IMUSIC_LIBRARY_FACTORY_H
#define DMI_INTERFACES_IMUSIC_LIBRARY_FACTORY_H

#include "IMUSICLibrary.h"


namespace MUSICLibrary::Interfaces
{
	class IMUSICLibraryFactory
	{
	public:
		virtual std::shared_ptr<IMUSICLibrary> Create(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteApp, unsigned int numLocalBatches, unsigned int numRemoteBatches, unsigned int localBatcherInterval, unsigned int remoteBatcherInterval) = 0;
	};
}

#endif // !DMI_INTERFACES_IMUSIC_LIBRARY_FACTORY_H