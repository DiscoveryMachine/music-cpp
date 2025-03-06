/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_RESPONSE_MESSAGES_FINALIZE_SCENARIO_RESPONSE_MESSAGE_H
#define DMI_MUSIC_RESPONSE_MESSAGES_FINALIZE_SCENARIO_RESPONSE_MESSAGE_H

#include "MUSICResponseMessage.h"
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "../MUSIC Request Messages/FinalizeScenarioRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	class FinalizeScenarioResponseMessage : public MUSICResponseMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage	
	{
	public:
		FinalizeScenarioResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status);

		FinalizeScenarioResponseMessage(std::shared_ptr<boost::json::object> obj);

		FinalizeScenarioResponseMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::FinalizeScenarioRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const FinalizeScenarioResponseMessage& obj) const;
		};

		bool operator == (const FinalizeScenarioResponseMessage &message2);

		bool operator != (const FinalizeScenarioResponseMessage &message2);

	protected:
		std::shared_ptr<FinalizeScenarioResponseMessage> shared_from_this()
		{
			return std::static_pointer_cast<FinalizeScenarioResponseMessage>(MUSICResponseMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_RESPONSE_MESSAGES_FINALIZE_SCENARIO_RESPONSE_MESSAGE_H