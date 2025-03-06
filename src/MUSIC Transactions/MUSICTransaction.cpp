/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICTransaction.h"

using namespace MUSICLibrary::Interfaces;

namespace MUSICLibrary::MUSIC_Transactions
{

	MUSICTransaction::MUSICTransaction(std::shared_ptr<IMUSICTransmitter> transmitter)
	{
		this->transmitter = transmitter;
	}

	int MUSICTransaction::GetOperationCount()
	{
		return TransactionExecuted.size();
	}

	void MUSICTransaction::OnTransactionExecuted()
	{
		for (std::function<void(std::shared_ptr<MUSICTransaction>)> func : TransactionExecuted)
		{
			if (func != NULL) 
			{
				func(shared_from_this());
			}
		}
	}
}
