/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICTransactionMonitor.h"

using namespace MUSICLibrary::Interfaces;

namespace MUSICLibrary::MUSIC_Transactions
{

	MUSICTransactionMonitor::MUSICTransactionMonitor(std::shared_ptr<IMUSICTransmitter> transmitter)
	{
		this->transmitter = transmitter;
		threadIDToTransactionMap = std::unordered_map<std::thread::id, std::shared_ptr<MUSICTransaction>>();
	}

	void MUSICTransactionMonitor::BeginTransaction()
	{
		std::thread::id currentThreadID = std::this_thread::get_id();
		if (threadIDToTransactionMap.count(currentThreadID))
		{
			throw TransactionAlreadyExistsForThreadException("A transaction already exists for the current thread");
		}

		std::shared_ptr<MUSICTransaction> transaction = std::make_shared<MUSICTransaction>(transmitter);
		threadIDToTransactionMap.emplace(currentThreadID, transaction);
	}

	bool MUSICTransactionMonitor::InTransaction()
	{
		return threadIDToTransactionMap.size() > 0;
	}
}
