/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IMUSIC_COMMAND_MESSAGE_H
#define DMI_INTERFACES_IMUSIC_COMMAND_MESSAGE_H

namespace MUSICLibrary::Interfaces
{
	class IMUSICCommandMessage
	{
	public:
		virtual unsigned int GetCommandIdentifier() = 0;
	};
}

#endif // !DMI_INTERFACES_IMUSIC_COMMAND_MESSAGE_H