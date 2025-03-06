/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_BATCHER_CONSTRUCT_DEAD_RECKONING_H
#define DMI_BATCHER_CONSTRUCT_DEAD_RECKONING_H

#include "../../../../Interfaces/IBatchOperation.h"


namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher
{
	class ConstructDeadReckoning : public MUSICLibrary::Interfaces::IBatchOperation
	{
	private:
		static constexpr double FPS = 30;

	public:
		void ApplyTo(std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> &batch) override;
	};
}

#endif // !DMI_BATCHER_CONSTRUCT_DEAD_RECKONING_H