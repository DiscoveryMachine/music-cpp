/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_REQUEST_SIMULATION_TIME_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_REQUEST_SIMULATION_TIME_MESSAGE_H

#include "../../Interfaces/IMUSICCommandMessage.h"
#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class RequestSimulationTimeMessage : public MUSICRequestMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	public:
		RequestSimulationTimeMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID);

		RequestSimulationTimeMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const RequestSimulationTimeMessage& obj) const;
		};

		bool operator == (const RequestSimulationTimeMessage &message2);

		bool operator != (const RequestSimulationTimeMessage &message2);

	protected:
		std::shared_ptr<RequestSimulationTimeMessage> shared_from_this()
		{
			return std::static_pointer_cast<RequestSimulationTimeMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_REQUEST_SIMULATION_TIME_MESSAGE_H