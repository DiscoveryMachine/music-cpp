/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PrimaryControlGrantedMessage.h"
#include "MUSICRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	PrimaryControlGrantedMessage::PrimaryControlGrantedMessage(unsigned int exerciseID, unsigned int requestID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
	}

	PrimaryControlGrantedMessage::PrimaryControlGrantedMessage(std::shared_ptr<boost::json::object> jobj) : MUSICRequestMessage(jobj)
	{
	}

	void PrimaryControlGrantedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitPrimaryControlGranted(shared_from_this());
	}

	bool PrimaryControlGrantedMessage::Equals(std::any obj)
	{
		bool isEquals = false;

		if (obj.type() == typeid(std::shared_ptr<PrimaryControlGrantedMessage>))
		{
			std::shared_ptr<PrimaryControlGrantedMessage> other = std::any_cast<std::shared_ptr<PrimaryControlGrantedMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> requestMessage = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID(), other->getRequestID());

			isEquals = MUSICRequestMessage::Equals(requestMessage);
		}

		return isEquals;
	}

	int PrimaryControlGrantedMessage::GetHashCode() const
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

	size_t PrimaryControlGrantedMessage::HashFunction::operator()(const PrimaryControlGrantedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool PrimaryControlGrantedMessage::operator == (const PrimaryControlGrantedMessage &message2)
	{
		return this->Equals(std::make_shared<PrimaryControlGrantedMessage>(message2));
	}

	bool PrimaryControlGrantedMessage::operator != (const PrimaryControlGrantedMessage &message2)
	{
		return !(*this == message2);
	}
}