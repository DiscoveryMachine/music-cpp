/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DefaultInternalVisitorFactory.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::Visitors;

namespace MUSICLibrary
{

	std::shared_ptr<IInternalMessageVisitor> DefaultInternalVisitorFactory::Create(unsigned int exerciseID, std::shared_ptr<SiteAndAppID> siteAndApp)
	{
		return std::make_shared<DefaultInternalMessageVisitor>(exerciseID, siteAndApp);
	}
}
