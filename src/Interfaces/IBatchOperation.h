/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IBATCH_OPERATION_H
#define DMI_INTERFACES_IBATCH_OPERATION_H

#include <unordered_map>
#include "IConstruct.h"


namespace MUSICLibrary::Interfaces
{
	class IBatchOperation
	{
	public:
		virtual void ApplyTo(std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<IConstruct>> &batch) = 0;
	};
}

#endif // !DMI_INTERFACES_IBATCH_OPERATION_H