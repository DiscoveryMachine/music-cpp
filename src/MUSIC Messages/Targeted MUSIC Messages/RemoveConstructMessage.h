/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGE_REMOVE_CONSTRUCT_MESSAGE_H
#define DMI_TARGETED_MUSIC_MESSAGE_REMOVE_CONSTRUCT_MESSAGE_H

#include "../../Interfaces/IMUSICCommandMessage.h"
#include "TargetedMUSICMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{
	class RemoveConstructMessage : public TargetedMUSICMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> RemovedConstruct;

	public:
		std::shared_ptr<EntityIDRecord> getRemovedConstruct() const;
		void setRemovedConstruct(std::shared_ptr<EntityIDRecord> value);

		RemoveConstructMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, std::shared_ptr<EntityIDRecord> removeConstruct);

		RemoveConstructMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const RemoveConstructMessage& obj) const;
		};

		bool operator == (const RemoveConstructMessage &message2);

		bool operator != (const RemoveConstructMessage &message2);

	protected:
		std::shared_ptr<RemoveConstructMessage> shared_from_this()
		{
			return std::static_pointer_cast<RemoveConstructMessage>(TargetedMUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_TARGETED_MUSIC_MESSAGE_REMOVE_CONSTRUCT_MESSAGE_H