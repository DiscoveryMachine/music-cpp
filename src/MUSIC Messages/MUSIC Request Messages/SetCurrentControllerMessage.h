/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_SET_CURRENT_CONTROLLER_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_SET_CURRENT_CONTROLLER_MESSAGE_H

#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class SetCurrentControllerMessage : public MUSICRequestMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> CurrentController;

	public:
		std::shared_ptr<EntityIDRecord> getCurrentController() const;
		void setCurrentController(std::shared_ptr<EntityIDRecord> value);

		SetCurrentControllerMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> currentController);

		SetCurrentControllerMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const SetCurrentControllerMessage& obj) const;
		};

		bool operator == (const SetCurrentControllerMessage &message2);

		bool operator != (const SetCurrentControllerMessage &message2);

	protected:
		std::shared_ptr<SetCurrentControllerMessage> shared_from_this()
		{
			return std::static_pointer_cast<SetCurrentControllerMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_SET_CURRENT_CONTROLLER_MESSAGE_H