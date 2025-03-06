/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_LIBRARY_DEFAULT_INTERNAL_VISITOR_FACTORY_H
#define DMI_MUSIC_LIBRARY_DEFAULT_INTERNAL_VISITOR_FACTORY_H

#include "Interfaces/IInternalVisitorFactory.h"
#include "Visitors/Default Internal Message Visitor/DefaultInternalMessageVisitor.h"


namespace MUSICLibrary
{
	class DefaultInternalVisitorFactory : public  MUSICLibrary::Interfaces::IInternalVisitorFactory
	{
	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> Create(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndApp) override;
	};
}

#endif // !DMI_MUSIC_LIBRARY_DEFAULT_INTERNAL_VISITOR_FACTORY_H