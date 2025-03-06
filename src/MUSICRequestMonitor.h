/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_LIBRARY_MUSIC_REQUEST_MONITOR_H
#define DMI_MUSIC_LIBRARY_MUSIC_REQUEST_MONITOR_H

#include <string>
#include <unordered_set>
#include <stdexcept>
#include "MUSICRequestStatus.h"
#include "tangible_exception_helper.h"


namespace MUSICLibrary::Interfaces
{
	class IConstruct;
}

namespace MUSICLibrary
{
	class MUSICRequestMonitor
	{
	private:
		std::unordered_set<std::shared_ptr<MUSICRequestStatus>> AliveRequests;

		std::unordered_set<std::shared_ptr<MUSICRequestStatus>> getAliveRequests() const;
		void setAliveRequests(const std::unordered_set<std::shared_ptr<MUSICRequestStatus>> &value);

		unsigned int nextRequestID = 0;
		unsigned int exerciseID = 0;

	public:
		MUSICRequestMonitor(unsigned int exerciseID);

		/// <summary>
		/// Creates then adds a new request to AliveRequests by instantiating its own MUSICRequestID.
		/// </summary>
		/// <param name="receiver"></param>
		/// <returns></returns>
		std::shared_ptr<MUSICRequestStatus> CreateRequest(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> sender, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> receiver);

		/// <summary>
		/// Creates then adds a request to AliveRequests using the given MUSICRequestID.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="receiver"></param>
		/// <returns></returns>
		std::shared_ptr<MUSICRequestStatus> CreateRequest(unsigned int id, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> sender, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> receiver);

		std::shared_ptr<MUSICRequestStatus> GetRequestByID(unsigned int requestID);

		void Reset();

		void RemoveRequestStatus(std::shared_ptr<MUSICRequestStatus> status);

		int GetAliveRequestsCount();

		bool ContainsRequest(unsigned int requestID);
	};
}

#endif // !DMI_MUSIC_LIBRARY_MUSIC_REQUEST_MONITOR_H