/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGE_PRIMARY_CONTROL_RELINQUISHED_MESSAGE_H
#define DMI_TARGETED_MUSIC_MESSAGE_PRIMARY_CONTROL_RELINQUISHED_MESSAGE_H

#include "TargetedMUSICMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{
	class PrimaryControlRelinquishedMessage : public TargetedMUSICMessage
	{
	public:
		PrimaryControlRelinquishedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID);

		PrimaryControlRelinquishedMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const PrimaryControlRelinquishedMessage& obj) const;
		};

		bool operator == (const PrimaryControlRelinquishedMessage &message2);

		bool operator != (const PrimaryControlRelinquishedMessage &message2);

	protected:
		std::shared_ptr<PrimaryControlRelinquishedMessage> shared_from_this()
		{
			return std::static_pointer_cast<PrimaryControlRelinquishedMessage>(TargetedMUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_TARGETED_MUSIC_MESSAGE_PRIMARY_CONTROL_RELINQUISHED_MESSAGE_H