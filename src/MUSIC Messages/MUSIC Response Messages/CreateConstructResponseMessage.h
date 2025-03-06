/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_RESPONSE_MESSAGES_CREATE_CONSTRUCT_RESPONSE_MESSAGE_H
#define DMI_MUSIC_RESPONSE_MESSAGES_CREATE_CONSTRUCT_RESPONSE_MESSAGE_H

#include "MUSICResponseMessage.h"
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "../MUSIC Request Messages/CreateConstructRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	class CreateConstructResponseMessage : public MUSICResponseMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> ConstructID;

	public:
		std::shared_ptr<EntityIDRecord> getConstructID() const;
		void setConstructID(std::shared_ptr<EntityIDRecord> value);

		CreateConstructResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> constructID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status);

		CreateConstructResponseMessage(std::shared_ptr<boost::json::object> jobj);

		CreateConstructResponseMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateConstructRequestMessage> msg, std::shared_ptr<EntityIDRecord> constructID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const CreateConstructResponseMessage& obj) const;
		};

		bool operator == (const CreateConstructResponseMessage &message2);

		bool operator != (const CreateConstructResponseMessage &message2);

	protected:
		std::shared_ptr<CreateConstructResponseMessage> shared_from_this()
		{
			return std::static_pointer_cast<CreateConstructResponseMessage>(MUSICResponseMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_RESPONSE_MESSAGES_CREATE_CONSTRUCT_RESPONSE_MESSAGE_H