/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IMUSIC_MESSAGE_VISITOR_H
#define DMI_INTERFACES_IMUSIC_MESSAGE_VISITOR_H

#include "IConstructMessageVisitor.h"
#include "ISimMessageVisitor.h"


namespace MUSICLibrary::Interfaces
{
	class IMUSICMessageVisitor : public IConstructMessageVisitor, public ISimMessageVisitor
	{
	};
}

#endif // !DMI_INTERFACES_IMUSIC_MESSAGE_VISITOR_H