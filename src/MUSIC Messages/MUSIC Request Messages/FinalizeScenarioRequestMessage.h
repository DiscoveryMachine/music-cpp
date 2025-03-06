/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_FINALIZE_SCENARIO_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_FINALIZE_SCENARIO_REQUEST_MESSAGE_H

#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class FinalizeScenarioRequestMessage : public MUSICRequestMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	public:
		FinalizeScenarioRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID);

		FinalizeScenarioRequestMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const FinalizeScenarioRequestMessage& obj) const;
		};

		bool operator == (const FinalizeScenarioRequestMessage &message2);

		bool operator != (const FinalizeScenarioRequestMessage &message2);

	protected:
		std::shared_ptr<FinalizeScenarioRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<FinalizeScenarioRequestMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_FINALIZE_SCENARIO_REQUEST_MESSAGE_H