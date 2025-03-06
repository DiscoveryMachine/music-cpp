/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_INTERACTION_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_INTERACTION_REQUEST_MESSAGE_H

#include "MUSICRequestMessage.h"
#include "InteractionType.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class InteractionRequestMessage : public MUSICRequestMessage
	{
	private:
		std::wstring InteractionName;
		MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType InteractionType = static_cast<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType>(0);
		std::shared_ptr<boost::json::object> InteractionData;

	public:
		std::wstring getInteractionName() const;
		void setInteractionName(const std::wstring &value);
		MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType getInteractionType() const;
		void setInteractionType(MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType value);
		std::shared_ptr<boost::json::object> getInteractionData() const;
		void setInteractionData(std::shared_ptr<boost::json::object> value);

		InteractionRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, const std::wstring &interactionName, MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType interactionType, std::shared_ptr<boost::json::object> interactionData = nullptr);

		InteractionRequestMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const InteractionRequestMessage& obj) const;
		};

		bool operator == (const InteractionRequestMessage &message2);

		bool operator != (const InteractionRequestMessage &message2);

	protected:
		std::shared_ptr<InteractionRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<InteractionRequestMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_INTERACTION_REQUEST_MESSAGE_H