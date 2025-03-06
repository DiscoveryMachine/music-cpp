/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_INTERACTION_TYPE_H
#define DMI_MUSIC_REQUEST_MESSAGES_INTERACTION_TYPE_H


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	enum class InteractionType
	{
		InteractionRequest = 454110001,
		PauseInteraction = 454110002,
		StopInteraction = 454110003,
		ResumeInteraction = 454110004
	};
}

#endif DMI_MUSIC_REQUEST_MESSAGES_INTERACTION_TYPE_H