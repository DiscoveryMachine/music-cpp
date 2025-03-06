/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_RESPONSE_MESSAGES_PARAMETERIZE_CONSTRUCT_RESPONSE_MESSAGE_H
#define DMI_MUSIC_RESPONSE_MESSAGES_PARAMETERIZE_CONSTRUCT_RESPONSE_MESSAGE_H

#include "MUSICResponseMessage.h"
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "../MUSIC Request Messages/ParameterizeConstructRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	class ParameterizeConstructResponseMessage : public MUSICResponseMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	public:
		ParameterizeConstructResponseMessage(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> originID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status);

		ParameterizeConstructResponseMessage(std::shared_ptr<boost::json::object> jobj);

		ParameterizeConstructResponseMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ParameterizeConstructRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ParameterizeConstructResponseMessage& obj) const;
		};

		bool operator == (const ParameterizeConstructResponseMessage &message2);

		bool operator != (const ParameterizeConstructResponseMessage &message2);

	protected:
		
		std::shared_ptr<ParameterizeConstructResponseMessage> shared_from_this()
		{
			return std::static_pointer_cast<ParameterizeConstructResponseMessage>(MUSICResponseMessage::shared_from_this());
		}
		
	};
}

#endif // !DMI_MUSIC_RESPONSE_MESSAGES_PARAMETERIZE_CONSTRUCT_RESPONSE_MESSAGE_H