/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICRequestMonitor.h"
#include <iostream>

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages;

namespace MUSICLibrary
{

	std::unordered_set<std::shared_ptr<MUSICRequestStatus>> MUSICRequestMonitor::getAliveRequests() const
	{
		return AliveRequests;
	}

	void MUSICRequestMonitor::setAliveRequests(const std::unordered_set<std::shared_ptr<MUSICRequestStatus>> &value)
	{
		AliveRequests = value;
	}

	MUSICRequestMonitor::MUSICRequestMonitor(unsigned int exerciseID)
	{
		this->exerciseID = exerciseID;
		nextRequestID = 0;
		setAliveRequests(std::unordered_set<std::shared_ptr<MUSICRequestStatus>>());
	}

	std::shared_ptr<MUSICRequestStatus> MUSICRequestMonitor::CreateRequest(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> sender, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> receiver)
	{
		std::shared_ptr<MUSICRequestStatus> status = std::make_shared<MUSICRequestStatus>(exerciseID, nextRequestID++, sender, receiver);
		std::unordered_set<std::shared_ptr<MUSICRequestStatus>> AliveMap = getAliveRequests();
		AliveMap.insert(status);
		setAliveRequests(AliveMap);
		return status;
	}

	std::shared_ptr<MUSICRequestStatus> MUSICRequestMonitor::CreateRequest(unsigned int id, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> sender, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> receiver)
	{
		std::shared_ptr<MUSICRequestStatus> status = std::make_shared<MUSICRequestStatus>(exerciseID, id, sender, receiver);
		std::unordered_set<std::shared_ptr<MUSICRequestStatus>> AliveMap = getAliveRequests();
		AliveMap.insert(status);
		setAliveRequests(AliveMap);
		return status;
	}

	std::shared_ptr<MUSICRequestStatus> MUSICRequestMonitor::GetRequestByID(unsigned int requestID)
	{
		for (std::shared_ptr<MUSICLibrary::MUSICRequestStatus> id : getAliveRequests()) 
		{
			if (id->getRequestID() == requestID)
			{
				return id;
			}
		}

		throw RequestStatusNotFoundException();
	}

	void MUSICRequestMonitor::Reset()
	{
		for (std::shared_ptr<MUSICLibrary::MUSICRequestStatus> id : getAliveRequests())
		{
			id->Notify(RequestStatus::Aborted);
		}
		AliveRequests.clear();
	}

	void MUSICRequestMonitor::RemoveRequestStatus(std::shared_ptr<MUSICRequestStatus> status)
	{
		  AliveRequests.erase(status);
	}

	int MUSICRequestMonitor::GetAliveRequestsCount()
	{
		return getAliveRequests().size();
	}

	bool MUSICRequestMonitor::ContainsRequest(unsigned int requestID)
	{
		try
		{
			auto status = GetRequestByID(requestID);
			return status != nullptr;
		}
		catch (const RequestStatusNotFoundException)
		{
			return false;
		}
	}
}
