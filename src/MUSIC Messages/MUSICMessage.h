/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_MUSIC_MESSAGE_H
#define DMI_MUSIC_MESSAGES_MUSIC_MESSAGE_H

#include <chrono>
#include <Windows.h>
#include <stringapiset.h>
#include "../Interfaces/IMessagePrototype.h"
#include "MUSICHeader.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class MUSICMessage : public MUSICLibrary::Interfaces::IMessagePrototype, public std::enable_shared_from_this<MUSICMessage>
	{
	private:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> MUSICHeader;
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> OriginID;

	public:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> getMUSICHeader() const;
		void setMUSICHeader(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> value);
		std::shared_ptr<EntityIDRecord> getOriginID() const;
		void setOriginID(std::shared_ptr<EntityIDRecord> value);

		MUSICMessage();

		MUSICMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID);

		MUSICMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> header, std::shared_ptr<EntityIDRecord> originID);

		MUSICMessage(boost::json::object jsonHeader, boost::json::object jsonOrigin);

		virtual void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) {}

		virtual std::any Clone() override
		{
			return this;
		}

		virtual std::shared_ptr<boost::json::object> ToJsonObject() override
		{
			return std::make_shared<boost::json::object>(boost::json::object());
		}

		std::shared_ptr<EntityIDRecord> extractEntityIDRecordFromJson(boost::json::object json, std::string entityIDKey) override;
		std::shared_ptr<EntityIDRecord> extractEntityIDRecordFromJson(boost::json::object json);

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const MUSICMessage& obj) const;
		};

		bool operator == (const MUSICMessage &message2);

		bool operator != (const MUSICMessage &message2);

	protected:
		
		std::shared_ptr<MUSICMessage> shared_from_this() {
			return std::enable_shared_from_this<MUSICMessage>::shared_from_this();
		}
		
	};
}

#endif // !DMI_MUSIC_MESSAGES_MUSIC_MESSAGE_H