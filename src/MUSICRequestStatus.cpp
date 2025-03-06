/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICRequestStatus.h"

using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages;
using namespace boost::json;
using namespace MUSICLibrary::Interfaces;

namespace MUSICLibrary
{

	std::shared_ptr<IConstruct> MUSICRequestStatus::getSender() const
	{
		return Sender;
	}

	void MUSICRequestStatus::setSender(std::shared_ptr<IConstruct> value)
	{
		Sender = value;
	}

	std::shared_ptr<IConstruct> MUSICRequestStatus::getReceiver() const
	{
		return Receiver;
	}

	void MUSICRequestStatus::setReceiver(std::shared_ptr<IConstruct> value)
	{
		Receiver = value;
	}

	unsigned int MUSICRequestStatus::getExerciseID() const
	{
		return ExerciseID;
	}

	void MUSICRequestStatus::setExerciseID(unsigned int value)
	{
		ExerciseID = value;
	}

	unsigned int MUSICRequestStatus::getRequestID() const
	{
		return RequestID;
	}

	void MUSICRequestStatus::setRequestID(unsigned int value)
	{
		RequestID = value;
	}

	std::optional<RequestStatus> MUSICRequestStatus::getLastStatus() const
	{
		return LastStatus;
	}

	void MUSICRequestStatus::setLastStatus(const std::optional<RequestStatus> &value)
	{
		LastStatus = value;
	}

	MUSICRequestStatus::MUSICRequestStatus(unsigned int exerciseID, unsigned int requestID, std::shared_ptr<IConstruct> sender, std::shared_ptr<IConstruct> receiver)
	{
		handlers = std::unordered_map<std::optional<RequestStatus>, MUSIC_REQ_HANDLER_FUNC>();
		notifiedStatuses = std::unordered_map<std::optional<RequestStatus>, std::shared_ptr<object>>();
		setSender(sender);
		setReceiver(receiver);
		setRequestID(requestID);
		setExerciseID(exerciseID);
		setLastStatus(std::nullopt);
	}

	void MUSICRequestStatus::Notify(RequestStatus status, std::shared_ptr<object> optionalData)
	{
		if (LastStatusWasTerminal())
		{
			throw InvalidOperationException("Notifications are no longer accepted by this request as it has already received a request terminating status.");
		}

		if (handlers.count(status))
		{
			ExecuteHandlerThenRemoveFromStatusSet(status, optionalData);
		}
		else
		{
			notifiedStatuses.emplace(status, optionalData);
		}

		setLastStatus(status);
	}

	void MUSICRequestStatus::RegisterHandler(RequestStatus status, MUSIC_REQ_HANDLER_FUNC handler)
	{
		handlers.emplace(status, handler);

		if (notifiedStatuses.count(status))
		{
			ExecuteHandlerThenRemoveFromStatusSet(status, notifiedStatuses[status]);
		}
	}

	void MUSICRequestStatus::ExecuteHandlerThenRemoveFromStatusSet(RequestStatus status, std::shared_ptr<object> optionalData)
	{
		handlers[status](ExerciseID, Sender, Receiver, RequestID, optionalData);
		notifiedStatuses.erase(status);
	}

	bool MUSICRequestStatus::LastStatusWasTerminal()
	{
		return getLastStatus() == RequestStatus::Aborted || getLastStatus() == RequestStatus::Complete || getLastStatus() == RequestStatus::PartiallyComplete;
	}
}
