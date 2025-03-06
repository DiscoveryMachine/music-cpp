/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DisplayMessagesMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	std::vector<std::wstring> DisplayMessagesMessage::getMessages() const
	{
		return Messages;
	}

	void DisplayMessagesMessage::setMessages(const std::vector<std::wstring> &value)
	{
		Messages = value;
	}

	unsigned int DisplayMessagesMessage::getTimeout() const
	{
		return Timeout;
	}

	void DisplayMessagesMessage::setTimeout(unsigned int value)
	{
		Timeout = value;
	}

	DisplayMessagesMessage::DisplayMessagesMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, std::vector<std::wstring> &messages, unsigned int timeout) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
		this->setMessages(messages);
		this->setTimeout(timeout);
	}

	DisplayMessagesMessage::DisplayMessagesMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
		object objVal = *obj;

		array messagesArray = objVal["messages"].as_array();
		std::vector messages = std::vector<std::wstring>();

		// Put each message into the vector
		for (int i = 0; i < messagesArray.size(); i++)
		{
			messages.push_back(StringHelper::stringToWString((messagesArray[i].as_string())));
		}

		setMessages(messages);

		setTimeout(value_to<unsigned int>(objVal["timeout"]));
	}

	void DisplayMessagesMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitDisplayMessages(shared_from_this());
	}

	std::any DisplayMessagesMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::vector<std::wstring> clonedMessages(Messages);
		std::shared_ptr<DisplayMessagesMessage> clonedMessage = std::make_shared<DisplayMessagesMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, clonedMessages, getTimeout());

		return clonedMessage;
	}

	unsigned int DisplayMessagesMessage::GetCommandIdentifier()
	{
		return 454004005;
	}

	std::shared_ptr<object> DisplayMessagesMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		std::shared_ptr<array> messages = std::make_shared<array>();
		std::for_each(Messages.begin(), Messages.end(), [this, messages](std::wstring msg)
		{
			messages->push_back(value(StringHelper::wstringToString(msg)));
		});
		objVal["messages"] = *messages;

		objVal["timeout"] = getTimeout();

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool DisplayMessagesMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DisplayMessagesMessage>))
		{
			std::shared_ptr<DisplayMessagesMessage> other = std::any_cast<std::shared_ptr<DisplayMessagesMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent) && getTimeout() == other->getTimeout())
			{
				int i = 0;
				for (auto msg : getMessages()) 
				{
					if (!(StringHelper::wstringToString(msg) == StringHelper::wstringToString(other->getMessages()[i]))) 
					{
						return false;
					}
					i++;
				}
				return true;
			}
		}

		return false;
	}

	int DisplayMessagesMessage::GetHashCode() const
	{
		std::wstring listString = L"";
		for (auto msg : getMessages())
		{
			listString += msg;
		}

		auto hashCode = 1689407557;
		hashCode = hashCode * -1521134295 + TargetedMUSICMessage::GetHashCode();
		hashCode = hashCode * -1521134295 + std::hash<std::wstring>{}(listString);
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getTimeout());
		return hashCode;
	}

	size_t DisplayMessagesMessage::HashFunction::operator()(const DisplayMessagesMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool DisplayMessagesMessage::operator == (const DisplayMessagesMessage &message2)
	{
		return this->Equals(std::make_shared<DisplayMessagesMessage>(message2));
	}

	bool DisplayMessagesMessage::operator != (const DisplayMessagesMessage &message2)
	{
		return !(*this == message2);
	}
}
