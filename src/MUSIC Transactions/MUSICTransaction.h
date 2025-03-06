/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_TRANSACTIONS_MUSIC_TRANSACTION_H
#define DMI_MUSIC_TRANSACTIONS_MUSIC_TRANSACTION_H

#include "../Interfaces/IMUSICTransmitter.h"
#include "../tangible_event.h"
#include <memory>

namespace MUSICLibrary::MUSIC_Transactions
{
	class MUSICTransaction : public std::enable_shared_from_this<MUSICTransaction>
	{
	public:

		//When adding functions to TransactionExecuted, follow the formatting listed in the following link
		//https://en.cppreference.com/w/cpp/utility/functional/function
		//Note that the "Notes" section details what is a bad function pointer as well as acceptable ones
		//Also a pop and push function does exist for vectors
		std::vector<std::function<void(std::shared_ptr<MUSICTransaction>)>> TransactionExecuted = std::vector<std::function<void(std::shared_ptr<MUSICTransaction>)>>();
		//std::shared_ptr<TangibleEvent<System.Action<MUSICTransaction>>> TransactionExecuted = std::make_shared<TangibleEvent<System.Action<MUSICTransaction>>>();
		std::shared_ptr<boost::json::object> constructDataMessage;

		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter;

		MUSICTransaction(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter);

		int GetOperationCount();

		virtual void OnTransactionExecuted();

	protected:
		std::shared_ptr<MUSICTransaction> shared_from_this() {
			return std::enable_shared_from_this<MUSICTransaction>::shared_from_this();
		}
	};
}

#endif // !DMI_MUSIC_TRANSACTIONS_MUSIC_TRANSACTION_H