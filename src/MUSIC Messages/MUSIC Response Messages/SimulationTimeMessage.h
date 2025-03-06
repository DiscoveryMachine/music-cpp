/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_RESPONSE_MESSAGES_SIMULATION_TIME_MESSAGE_H
#define DMI_MUSIC_RESPONSE_MESSAGES_SIMULATION_TIME_MESSAGE_H

#include "../../Interfaces/IMUSICCommandMessage.h"
#include "MUSICResponseMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"
#include "../MUSIC Request Messages/RequestSimulationTimeMessage.h"

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	class SimulationTimeMessage : public MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::MUSICResponseMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		unsigned int SimTime = 0;

	public:
		unsigned int getSimTime() const;
		void setSimTime(unsigned int value);

		SimulationTimeMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, unsigned int requestID, 
			MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, unsigned int simTime);

		SimulationTimeMessage(std::shared_ptr<boost::json::object> obj);

		SimulationTimeMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::RequestSimulationTimeMessage> msg,
			MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, unsigned int simTime);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const SimulationTimeMessage& obj) const;
		};

		bool operator == (const SimulationTimeMessage &message2);

		bool operator != (const SimulationTimeMessage &message2);

	protected:
		std::shared_ptr<SimulationTimeMessage> shared_from_this()
		{
			return std::static_pointer_cast<SimulationTimeMessage>(MUSICResponseMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_RESPONSE_MESSAGES_SIMULATION_TIME_MESSAGE_H