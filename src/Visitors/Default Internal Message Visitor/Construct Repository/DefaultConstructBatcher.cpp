/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DefaultConstructBatcher.h"
#include <iostream>

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher;
using namespace std::chrono;
using namespace std;

namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository
{

	DefaultConstructBatcher::DefaultConstructBatcher(unsigned int interval)
	{
		this->interval = interval;
	}

	void DefaultConstructBatcher::Task(std::shared_ptr<IBatchOperation> op, std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> batch)
	{
		while (true)
		{
			op->ApplyTo(batch);
			std::this_thread::sleep_for(std::chrono::seconds(interval));
		}
	}

	void DefaultConstructBatcher::StartBatchThreads(std::shared_ptr<IBatchOperation> op, std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> batches)
	{
		std::vector<std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>> batchVector = batches->getConstructs();

		for (auto batch : batchVector)
		{
			std::thread worker = std::thread([this, op, batch] {this->Task(op, batch); });
			worker.detach();

			///testing purposes only
			//workers.emplace_back(std::thread([this, op, batch] {this->Task(op, batch); }));
			//workers.back().detach(); //sub-threads will run until main thread closes

			std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Give a 20ms gap between each batch
		}
	}
}
