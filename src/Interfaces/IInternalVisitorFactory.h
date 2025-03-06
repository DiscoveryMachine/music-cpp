/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IINTERNAL_VISITOR_FACTORY_H
#define DMI_INTERFACES_IINTERNAL_VISITOR_FACTORY_H

#include "IInternalMessageVisitor.h"


namespace MUSICLibrary::Interfaces
{
	class IInternalVisitorFactory
	{
	public:
		virtual std::shared_ptr<IInternalMessageVisitor> Create(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndApp) = 0;
	};
}

#endif // !DMI_INTERFACES_IINTERNAL_VISITOR_FACTORY_H