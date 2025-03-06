/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IINTERNAL_MESSAGE_VISITOR_H
#define DMI_INTERFACES_IINTERNAL_MESSAGE_VISITOR_H

#include "IMUSICMessageVisitor.h"
#include "AbstractConstructCreateInfo.h"


namespace MUSICLibrary::Interfaces
{
	class IInternalMessageVisitor : public IMUSICMessageVisitor
	{
	public:
		IInternalMessageVisitor() {}
		virtual std::shared_ptr<IConstruct> InitializeLocalConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo) = 0;
	};
}

#endif // !DMI_INTERFACES_IINTERNAL_MESSAGE_VISITOR_H