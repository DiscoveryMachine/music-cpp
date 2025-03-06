/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ConstructBatches.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;

namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher
{
	std::vector<std::unordered_map<std::shared_ptr<EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>> ConstructBatches::getConstructs() const
	{
		return constructs;
	}

	ConstructBatches::ConstructBatches(unsigned int batchCount)
	{
		constructs = std::vector<std::unordered_map<std::shared_ptr<EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>>();

		if (batchCount == 0)
		{
			throw InvalidOperationException();
		}

		for (unsigned int i = 0; i < batchCount; i++)
		{
			constructs.push_back(std::unordered_map<std::shared_ptr<EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>());
		}
	}

	void ConstructBatches::Add(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> msg)
	{
		//Puts other threads on pause until lock is obtained
		std::mutex lock;
		lock.lock();

		try
		{
			// Find the smallest batch in constructs
			// Need to get pointer otherwise msg will be put into a separate map and not the one in constructs
			std::unordered_map<std::shared_ptr<EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>* smallestBatch = &constructs[0];
			for (int i = 0; i < constructs.size(); i++)
			{
				// If this construct is already in constructs, return
				if (constructs[i].count(msg->GetID()))
				{
					lock.unlock();
					return;
				}

				// If this dictionary is shorter than dict, assign it to dict
				if (constructs[i].size() < smallestBatch->size())
				{
					smallestBatch = &constructs[i];
				}
			}

			// Add this new construct to dict and increment count
			smallestBatch->emplace(msg->GetID(), msg);
			count++;

			//returns lock on object.
			lock.unlock();
		}
		catch (const std::runtime_error &err)
		{
			//returns lock on object.
			lock.unlock();
			throw err;
		}
	}

	void ConstructBatches::Remove(std::shared_ptr<EntityIDRecord> id)
	{
		std::mutex lock;
		lock.lock();

		try
		{
			// For every batch in constructs, try to call erase on the given ID
			for (auto batch : constructs)
			{
				// If the construct is in this batch and was deleted, update count, unlock mutex, and return
				if (batch.erase(id))
				{
					count--;
					lock.unlock();
					return;
				}
			}

			lock.unlock();
		}
		catch (const std::runtime_error &err)
		{
			lock.unlock();
			throw err;
		}
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> ConstructBatches::GetConstructByID(std::shared_ptr<EntityIDRecord> id)
	{
		// For each batch in constructs, search for the construct with the given ID
		for (auto batch : constructs)
		{
			// For each construct in this batch, check if it contains the given id as a key
			for (const auto &[eid, construct] : batch)
			{
				// If this eid is equal to the given id, return the associated construct
				if (eid->Equals(id))
				{
					return construct;
				}
			}
		}

		throw ConstructNotFoundException();
	}

	std::vector<std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> ConstructBatches::GetEnumerator()
	{
		std::vector<std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> allValues;

		for (std::unordered_map<std::shared_ptr<EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> batch : getConstructs())
		{
			for (const auto &[eid, construct] : batch)
			{
				allValues.push_back(construct);
			}
		}
		return allValues;
	}
}
