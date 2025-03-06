/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_FORCE_H
#define DMI_MUSIC_MESSAGES_FORCE_H


namespace MUSICLibrary::MUSIC_Messages
{
	enum class Force
	{
		Other,
		Friendly,
		Opposing,
		Neutral
	};
}

#endif // !DMI_MUSIC_MESSAGES_FORCE_H