/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_CREATE_CONSTRUCT_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_CREATE_CONSTRUCT_REQUEST_MESSAGE_H

#include <string>
#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "../MUSICVector3.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class CreateConstructRequestMessage : public MUSICRequestMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		std::wstring constructType;
		std::wstring constructCallsign;
		std::shared_ptr<MUSICVector3> constructLocation;
		std::shared_ptr<MUSICVector3> constructOrientation;

		// Constants
		static const std::wstring CONSTRUCT_TYPE;
		static const std::wstring CONSTRUCT_CALLSIGN;
		static const std::wstring CONSTRUCT_LOCATION;
		static const std::wstring CONSTRUCT_ORIENTATION;

	public:
		std::wstring getConstructType() const;
		void setConstructType(const std::wstring &value);
		std::wstring getConstructCallsign() const;
		void setConstructCallsign(const std::wstring &value);
		std::shared_ptr<MUSICVector3> getConstructLocation() const;
		void setConstructLocation(std::shared_ptr<MUSICVector3> value);
		std::shared_ptr<MUSICVector3> getConstructOrientation() const;
		void setConstructOrientation(std::shared_ptr<MUSICVector3> value);

		CreateConstructRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, const std::wstring &constructType, const std::wstring &constructCallsign, std::shared_ptr<MUSICVector3> constructLocation, std::shared_ptr<MUSICVector3> constructOrientation);

		CreateConstructRequestMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		unsigned int GetCommandIdentifier() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const CreateConstructRequestMessage& obj) const;
		};

		bool operator == (const CreateConstructRequestMessage &message2);

		bool operator != (const CreateConstructRequestMessage &message2);

	protected:
		std::shared_ptr<CreateConstructRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<CreateConstructRequestMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_CREATE_CONSTRUCT_REQUEST_MESSAGE_H