/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_REPOSITORY_CONSTRUCT_REPOSITORY_H
#define DMI_CONSTRUCT_REPOSITORY_CONSTRUCT_REPOSITORY_H

#include <string>
#include <stdexcept>
#include "../../../Interfaces/IConstructRepository.h"
#include "../../../Interfaces/IConstructBatcher.h"
#include "../../../tangible_string_helper.h"
#include "ConstructHeartbeat.h"
#include "Batcher/ConstructDeadReckoning.h"


namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository
{
	class ConstructRepository : public  MUSICLibrary::Interfaces::IConstructRepository
	{
	private:
		std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> LocalConstructs;
		std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> RemoteConstructs;
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> Transmitter;
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> LocalBatcher;
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> RemoteBatcher;

		std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> getLocalConstructs() const;
		void setLocalConstructs(std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> value);
		std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> getRemoteConstructs() const;
		void setRemoteConstructs(std::shared_ptr<MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher::ConstructBatches> value);
	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> getTransmitter() const;
	private:
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> getLocalBatcher() const;
		void setLocalBatcher(std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> value);
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> getRemoteBatcher() const;
		void setRemoteBatcher(std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> value);

		static const std::wstring ENTITY_ID_ZERO_ERROR_MSG_FORMAT;

	public:
		ConstructRepository(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter, unsigned int numLocalBatches, unsigned int numRemoteBatches, std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> localBatcher, std::shared_ptr<MUSICLibrary::Interfaces::IConstructBatcher> remoteBatcher);

		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> &operator [](std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> eid);

		void AddConstruct(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> construct) override;

		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> GetConstructByID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> eid) override;

		int GetConstructCount() override;

		int GetLocalConstructCount();

		int GetRemoteConstructCount();

		void StartBatchThreads();

		bool ConstructExists(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> eid) override;
	};
}

#endif // !DMI_CONSTRUCT_REPOSITORY_CONSTRUCT_REPOSITORY_H