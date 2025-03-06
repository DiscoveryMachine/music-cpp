/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_PRIMARY_CONTROL_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_PRIMARY_CONTROL_REQUEST_MESSAGE_H

#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class PrimaryControlRequestMessage : public MUSICRequestMessage
	{
	public:
		PrimaryControlRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID);

		PrimaryControlRequestMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const PrimaryControlRequestMessage& obj) const;
		};

		bool operator == (const PrimaryControlRequestMessage &message2);

		bool operator != (const PrimaryControlRequestMessage &message2);

	protected:
		std::shared_ptr<PrimaryControlRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<PrimaryControlRequestMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_PRIMARY_CONTROL_REQUEST_MESSAGE_H