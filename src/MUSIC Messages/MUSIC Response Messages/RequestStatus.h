/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_RESPONSE_MESSAGES_REQUEST_STATUS_H
#define DMI_MUSIC_RESPONSE_MESSAGES_REQUEST_STATUS_H 

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	enum class RequestStatus
	{
		Other,
		Pending,
		Executing,
		PartiallyComplete,
		Complete,
		Aborted,
		Paused
	};
}

#endif // !DMI_MUSIC_RESPONSE_MESSAGES_REQUEST_STATUS_H