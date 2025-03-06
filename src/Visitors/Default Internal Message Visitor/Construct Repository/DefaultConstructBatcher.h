/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_REPOSITORY_DEFAULT_CONSTRUCT_BATCHER_H
#define DMI_CONSTRUCT_REPOSITORY_DEFAULT_CONSTRUCT_BATCHER_H

#include <vector>
#include <thread>
#include <chrono>
#include "../../../Interfaces/IConstructBatcher.h"


namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository
{
	class DefaultConstructBatcher : public  MUSICLibrary::Interfaces::IConstructBatcher
	{
	public:
		/// <summary>
		/// Time between a given batch is run again in seconds.
		/// </summary>
		unsigned int interval = 0;

		//for test package/debugging purposes only
		std::vector<std::thread> workers;

	public:
		DefaultConstructBatcher(unsigned int interval);
		void Task(std::shared_ptr<MUSICLibrary::Interfaces::IBatchOperation> op, std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> batch);
		void StartBatchThreads(std::shared_ptr<MUSICLibrary::Interfaces::IBatchOperation> op, std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> batches) override;
	};
}

#endif // !DMI_CONSTRUCT_REPOSITORY_DEFAULT_CONSTRUCT_BATCHER_H