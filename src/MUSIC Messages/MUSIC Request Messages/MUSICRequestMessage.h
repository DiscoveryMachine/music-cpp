/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_MUSIC_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_MUSIC_REQUEST_MESSAGE_H

#include <string>
#include "../Targeted MUSIC Messages/TargetedMUSICMessage.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class MUSICRequestMessage : public MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::TargetedMUSICMessage
	{
	private:
		unsigned int RequestID = 0;

		static const std::wstring REQUEST_ID_KEY;

	public:
		unsigned int getRequestID() const;
		void setRequestID(unsigned int value);

		MUSICRequestMessage();

		MUSICRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID);

		MUSICRequestMessage(std::shared_ptr<boost::json::object> jobj);

		bool Equals(std::any obj);

		int GetHashCode() const;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		struct HashFunction
		{
			size_t operator()(const MUSICRequestMessage& obj) const;
		};

		bool operator == (const MUSICRequestMessage &message2);

		bool operator != (const MUSICRequestMessage &message2);

	protected:
		std::shared_ptr<MUSICRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<MUSICRequestMessage>(TargetedMUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_MUSIC_REQUEST_MESSAGE_H