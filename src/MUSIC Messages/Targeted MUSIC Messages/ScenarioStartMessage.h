/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGE_SCENARIO_START_MESSAGE_H
#define DMI_TARGETED_MUSIC_MESSAGE_SCENARIO_START_MESSAGE_H

#include "../../Interfaces/IMUSICCommandMessage.h"
#include "TargetedMUSICMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{
	class ScenarioStartMessage : public TargetedMUSICMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	public:
		ScenarioStartMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID);

		ScenarioStartMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ScenarioStartMessage& obj) const;
		};

		bool operator == (const ScenarioStartMessage &message2);

		bool operator != (const ScenarioStartMessage &message2);

	protected:
		std::shared_ptr<ScenarioStartMessage> shared_from_this()
		{
			return std::static_pointer_cast<ScenarioStartMessage>(TargetedMUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_TARGETED_MUSIC_MESSAGE_SCENARIO_START_MESSAGE_H