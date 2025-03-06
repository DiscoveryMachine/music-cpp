/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

namespace MUSICLibrary::MUSIC_Messages
{

	std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> MUSICMessage::getMUSICHeader() const
	{
		return MUSICHeader;
	}

	void MUSICMessage::setMUSICHeader(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> value)
	{
		MUSICHeader = value;
	}

	std::shared_ptr<EntityIDRecord> MUSICMessage::getOriginID() const
	{
		return OriginID;
	}

	void MUSICMessage::setOriginID(std::shared_ptr<EntityIDRecord> value)
	{
		OriginID = value;
	}

	MUSICMessage::MUSICMessage()
	{
	}

	MUSICMessage::MUSICMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID)
	{
		setMUSICHeader(std::make_shared<MUSICLibrary::MUSIC_Messages::MUSICHeader>(exerciseID, 
			static_cast<unsigned long long>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())));
		setOriginID(originID);
	}

	MUSICMessage::MUSICMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> header, std::shared_ptr<EntityIDRecord> originID)
	{
		setMUSICHeader(header);
		setOriginID(originID);
	}

	MUSICMessage::MUSICMessage(object headerJson, boost::json::object originJson)
	{
		//object headerJson = json["header"].as_object();

		// Int and unsigned int cannot be extracted from the json using as_uint64 as they are not the same length
		// Doing so results in garbage and some possibly weird memory exceptions/behavior
		MUSICLibrary::MUSIC_Messages::MUSICHeader header = MUSICLibrary::MUSIC_Messages::MUSICHeader(value_to<unsigned int>(headerJson["exerciseID"]), value_to<unsigned long long>(headerJson["timestamp"]));
		setMUSICHeader(std::make_shared<MUSICLibrary::MUSIC_Messages::MUSICHeader>(header));

		setOriginID(MUSICMessage::extractEntityIDRecordFromJson(originJson));
	}

	std::shared_ptr<EntityIDRecord> MUSICMessage::extractEntityIDRecordFromJson(object entityIDJson)
	{
		EntityIDRecord record = EntityIDRecord(value_to<unsigned int>(entityIDJson["siteID"]),
			value_to<unsigned int>(entityIDJson["appID"]),
			entityIDJson["entityID"].is_null() ? 0 : value_to<unsigned int>(entityIDJson["entityID"]));
		return std::make_shared<EntityIDRecord>(record);
	}

	std::shared_ptr<EntityIDRecord> MUSICMessage::extractEntityIDRecordFromJson(object json, std::string entityIDKey)
	{
		object entityIDJson = json[entityIDKey].as_object();
		return extractEntityIDRecordFromJson(entityIDJson);
	}

	bool MUSICMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<MUSICMessage>))
		{
			std::shared_ptr<MUSICMessage> other = std::any_cast<std::shared_ptr<MUSICMessage>>(obj);

			if (getMUSICHeader()->Equals(other->getMUSICHeader()) && 
				getOriginID()->Equals(other->getOriginID()))
			{
				return true;
			}
		}

		return false;
	}

	int MUSICMessage::GetHashCode() const
	{
		auto hashCode = 1477022203;
		hashCode = hashCode * -1521134295 + ((getMUSICHeader() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getOriginID() != NULL) ? getOriginID()->GetHashCode() : 0);
		return hashCode;
	}

	size_t MUSICMessage::HashFunction::operator()(const MUSICMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool MUSICMessage::operator == (const MUSICMessage &message2)
	{
		return this->Equals(std::make_shared<MUSICMessage>(message2));
	}

	bool MUSICMessage::operator != (const MUSICMessage &message2)
	{
		return !(*this == message2);
	}
}
