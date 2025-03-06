/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ICONSTRUCT_FACTORY_H
#define DMI_INTERFACES_ICONSTRUCT_FACTORY_H

#include "AbstractConstructCreateInfo.h"

namespace MUSICLibrary::Interfaces
{
	class IConstructFactory
	{
	public:
		virtual std::shared_ptr<IConstruct> Create(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> constructData) = 0;
		virtual std::shared_ptr<IConstruct> Create(std::wstring fullyQualifiedName, std::wstring callsign) = 0;
		virtual bool IsConstructRegistered(const std::wstring &fullyQualifiedName) = 0;
		virtual void RegisterLocalConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo) = 0;
		virtual void RegisterRemoteConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo) = 0;
	};
}

#endif // !DMI_INTERFACES_ICONSTRUCT_FACTORY_H