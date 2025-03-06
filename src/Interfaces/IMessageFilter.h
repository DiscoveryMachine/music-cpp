/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IMESSAGE_FILTER_H
#define DMI_INTERFACES_IMESSAGE_FILTER_H

#include "../MUSIC Messages/MUSICMessage.h"

namespace MUSICLibrary::Interfaces
{
	class IMessageFilter
	{
	public:
		IMessageFilter() {}
		virtual bool ShouldDiscard(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message) = 0;

		virtual void OnHandledMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message) = 0;
	};
}

#endif // !DMI_INTERFACES_IMESSAGE_FILTER_H