/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_RESPONSE_MESSAGES_INTERACTION_RESPONSE_MESSAGE_H
#define DMI_MUSIC_RESPONSE_MESSAGES_INTERACTION_RESPONSE_MESSAGE_H

#include "MUSICResponseMessage.h"
#include "../MUSIC Request Messages/InteractionRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	class InteractionResponseMessage : public MUSICResponseMessage
	{
	private:
		std::shared_ptr<boost::json::object> OptionalData;

	public:
		std::shared_ptr<boost::json::object> getOptionalData() const;
		void setOptionalData(std::shared_ptr<boost::json::object> value);

	private:
		static const std::wstring OPTIONAL_DATA_KEY;

	public:
		InteractionResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, std::shared_ptr<boost::json::object> optionalData = nullptr);

		InteractionResponseMessage(std::shared_ptr<boost::json::object> jobj);

		InteractionResponseMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus requestStatus, std::shared_ptr<boost::json::object> optionalData = nullptr);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const InteractionResponseMessage& obj) const;
		};

		bool operator == (const InteractionResponseMessage &message2);

		bool operator != (const InteractionResponseMessage &message2);

	protected:
		std::shared_ptr<InteractionResponseMessage> shared_from_this()
		{
			return std::static_pointer_cast<InteractionResponseMessage>(MUSICResponseMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_RESPONSE_MESSAGES_INTERACTION_RESPONSE_MESSAGE_H