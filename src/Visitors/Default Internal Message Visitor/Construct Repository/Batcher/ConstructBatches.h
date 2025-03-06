/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_BATCHER_CONSTRUCT_BATCHES_H
#define DMI_BATCHER_CONSTRUCT_BATCHES_H

#include "../../../../Interfaces/IConstruct.h"


namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher
{
	class ConstructBatches : public std::vector<std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>
	{
	private:
		std::vector<std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>> constructs;

	public:
		std::vector<std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>>> getConstructs() const;
		unsigned int count = 0;

		ConstructBatches(unsigned int batchCount);

		void Add(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> msg);

		void Remove(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> id);

		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> GetConstructByID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> id);

		std::vector<std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> GetEnumerator();
	};
}

#endif // !DMI_BATCHER_CONSTRUCT_BATCHES_H