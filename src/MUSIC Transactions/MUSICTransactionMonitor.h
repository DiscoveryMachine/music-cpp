/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_TRANSACTIONS_MUSIC_TRANSACTION_MONITOR_H
#define DMI_MUSIC_TRANSACTIONS_MUSIC_TRANSACTION_MONITOR_H

#include <string>
#include <thread>
#include <unordered_map>
#include <stdexcept>
#include "MUSICTransaction.h"
#include "../tangible_exception_helper.h"

namespace MUSICLibrary::MUSIC_Transactions
{
	class MUSICTransactionMonitor
	{
	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter;

		std::unordered_map<std::thread::id, std::shared_ptr<MUSICTransaction>> threadIDToTransactionMap;

		MUSICTransactionMonitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter);

		void BeginTransaction();

		bool InTransaction();
	};
}

#endif // !DMI_MUSIC_TRANSACTIONS_MUSIC_TRANSACTION_MONITOR_H