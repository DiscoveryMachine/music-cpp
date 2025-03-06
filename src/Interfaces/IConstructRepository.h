/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ICONSTRUCT_REPOSITORY_H
#define DMI_INTERFACES_ICONSTRUCT_REPOSITORY_H

#include "IConstruct.h"

namespace MUSICLibrary::Interfaces
{
	class IConstructRepository
	{
	public:
		virtual void AddConstruct(std::shared_ptr<IConstruct> construct) = 0;
		virtual int GetConstructCount() = 0;
		virtual std::shared_ptr<IConstruct> GetConstructByID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> eid) = 0;
		virtual bool ConstructExists(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> eid) = 0;
	};
}

#endif // !DMI_INTERFACES_ICONSTRUCT_REPOSITORY_H