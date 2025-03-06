/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_RESPONSE_MESSAGES_MUSIC_RESPONSE_MESSAGE_H
#define DMI_MUSIC_RESPONSE_MESSAGES_MUSIC_RESPONSE_MESSAGE_H

#include <string>
#include <optional>
#include "../MUSIC Request Messages/MUSICRequestMessage.h"
#include "RequestStatus.h"

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	class MUSICResponseMessage : public MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::MUSICRequestMessage
	{
	private:
		MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus RequestStatus = static_cast<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus>(0);

		static const std::wstring STATUS_KEY;

	public:
		MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus getRequestStatus() const;
		void setRequestStatus(MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus value);

	public:
		MUSICResponseMessage();

		MUSICResponseMessage(std::shared_ptr<boost::json::object> jobj);

		MUSICResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status);

		//Only switches when constructing a response message from a request message
	private:
		MUSICResponseMessage(std::shared_ptr<MUSICRequestMessage> msg); // This base constructor swaps origin and receiver IDs. It is not a mistake.

	public:
		MUSICResponseMessage(std::shared_ptr<MUSICRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus requestStatus);

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const MUSICResponseMessage& obj) const;
		};

		bool operator == (const MUSICResponseMessage &message2);

		bool operator != (const MUSICResponseMessage &message2);

	protected:
		
		std::shared_ptr<MUSICResponseMessage> shared_from_this()
		{
			return std::static_pointer_cast<MUSICResponseMessage>(MUSICRequestMessage::shared_from_this());
		}
		
	};
}

#endif // !DMI_MUSIC_RESPONSE_MESSAGES_MUSIC_RESPONSE_MESSAGE_H