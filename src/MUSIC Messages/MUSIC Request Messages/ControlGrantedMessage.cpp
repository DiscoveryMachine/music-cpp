/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlGrantedMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	ControlGrantedMessage::ControlGrantedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
	}

	ControlGrantedMessage::ControlGrantedMessage(std::shared_ptr<boost::json::object> jobj) : MUSICRequestMessage(jobj)
	{
	}

	void ControlGrantedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlGranted(shared_from_this());
	}

	bool ControlGrantedMessage::Equals(std::any obj)
	{
		bool isEquals = false;

		if (obj.type() == typeid(std::shared_ptr<ControlGrantedMessage>))
		{
			std::shared_ptr<ControlGrantedMessage> other = std::any_cast<std::shared_ptr<ControlGrantedMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> requestMessage = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID(), other->getRequestID());

			isEquals = MUSICRequestMessage::Equals(requestMessage);
		}

		return isEquals;
	}

	int ControlGrantedMessage::GetHashCode() const
	{
		// NOTE: The following hash code variable was created arbitrarily and was not converted over from C#.
		// The only goal in its creation was to make a unique int.
		auto hashCode = 265193601;
		hashCode = hashCode * -1331134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1331134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1331134295 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1331134295 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		return hashCode;
	}

	size_t ControlGrantedMessage::HashFunction::operator()(const ControlGrantedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlGrantedMessage::operator == (const ControlGrantedMessage &message2)
	{
		return this->Equals(std::make_shared<ControlGrantedMessage>(message2));
	}

	bool ControlGrantedMessage::operator != (const ControlGrantedMessage &message2)
	{
		return !(*this == message2);
	}
}