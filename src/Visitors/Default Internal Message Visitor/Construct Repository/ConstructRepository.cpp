/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ConstructRepository.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository::Batcher;

namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository
{

	std::shared_ptr<ConstructBatches> ConstructRepository::getLocalConstructs() const
	{
		return LocalConstructs;
	}

	void ConstructRepository::setLocalConstructs(std::shared_ptr<ConstructBatches> value)
	{
		LocalConstructs = value;
	}

	std::shared_ptr<ConstructBatches> ConstructRepository::getRemoteConstructs() const
	{
		return RemoteConstructs;
	}

	void ConstructRepository::setRemoteConstructs(std::shared_ptr<ConstructBatches> value)
	{
		RemoteConstructs = value;
	}

	std::shared_ptr<IMUSICTransmitter> ConstructRepository::getTransmitter() const
	{
		return Transmitter;
	}

	std::shared_ptr<IConstructBatcher> ConstructRepository::getLocalBatcher() const
	{
		return LocalBatcher;
	}

	void ConstructRepository::setLocalBatcher(std::shared_ptr<IConstructBatcher> value)
	{
		LocalBatcher = value;
	}

	std::shared_ptr<IConstructBatcher> ConstructRepository::getRemoteBatcher() const
	{
		return RemoteBatcher;
	}

	void ConstructRepository::setRemoteBatcher(std::shared_ptr<IConstructBatcher> value)
	{
		RemoteBatcher = value;
	}

	const std::wstring ConstructRepository::ENTITY_ID_ZERO_ERROR_MSG_FORMAT = 
		L"[Warning]: Attempted to {0}, but the EntityID was 0. An EntityIDRecord.EntityID of 0 is a reserved value for an EntityIDRecord to refer to the simulation itself rather than an individual construct.";

	ConstructRepository::ConstructRepository(std::shared_ptr<IMUSICTransmitter> transmitter, unsigned int numLocalBatches, unsigned int numRemoteBatches, std::shared_ptr<IConstructBatcher> localBatcher, std::shared_ptr<IConstructBatcher> remoteBatcher)
	{
		setLocalConstructs(std::make_shared<ConstructBatches>(numLocalBatches));
		setRemoteConstructs(std::make_shared<ConstructBatches>(numRemoteBatches));
		Transmitter = transmitter;
		setLocalBatcher(localBatcher);
		setRemoteBatcher(remoteBatcher);
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> & ConstructRepository::operator [](std::shared_ptr<EntityIDRecord> eid)
	{
		if (eid->getEntityID() == 0)
		{
			throw EntityIDIsZeroException(StringHelper::wstringToString(ENTITY_ID_ZERO_ERROR_MSG_FORMAT + L"get a construct by its id"));
		}

		try
		{
			std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> selectConstruct = RemoteConstructs->GetConstructByID(eid);
			return selectConstruct;
		}
		catch (const ConstructNotFoundException)
		{
			// No op to check local constructs
		}

		try
		{
			const std::shared_ptr<ConstructBatches> lConstructs = getLocalConstructs();
			std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> selectConstruct = lConstructs->GetConstructByID(eid);
			return selectConstruct;
		}
		catch (const ConstructNotFoundException)
		{
			// Throw a more descriptive version of the exception
			throw ConstructNotFoundException("Construct not found for ConstructID: " + serialize(*eid->ToJsonObject()));
		}
	}

	void ConstructRepository::AddConstruct(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> construct)
	{
		if (construct->GetID()->getEntityID() == 0) 
		{
			throw EntityIDIsZeroException(StringHelper::wstringToString(ENTITY_ID_ZERO_ERROR_MSG_FORMAT + L"add an entry into the construct map with an id"));
		}
		else 
		{
			if (construct->IsRemote()) 
			{
				std::shared_ptr<ConstructBatches> rConstructs = getRemoteConstructs();
				rConstructs->Add(construct);
				setRemoteConstructs(rConstructs);
			}
			else 
			{
				std::shared_ptr<ConstructBatches> lConstructs = getLocalConstructs();
				lConstructs->Add(construct);
				setLocalConstructs(lConstructs);
			}
		}
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> ConstructRepository::GetConstructByID(std::shared_ptr<EntityIDRecord> eid)
	{
		return this->operator[](eid);
	}

	int ConstructRepository::GetConstructCount()
	{
		return static_cast<int>(getLocalConstructs()->count + getRemoteConstructs()->count);
	}

	int ConstructRepository::GetLocalConstructCount()
	{
		return static_cast<int>(getLocalConstructs()->count);
	}

	int ConstructRepository::GetRemoteConstructCount()
	{
		return static_cast<int>(getRemoteConstructs()->count);
	}

	void ConstructRepository::StartBatchThreads()
	{
		std::shared_ptr<ConstructHeartbeat> heartbeat = std::make_shared<ConstructHeartbeat>(getTransmitter());
		getLocalBatcher()->StartBatchThreads(heartbeat, getLocalConstructs());

		std::shared_ptr<ConstructDeadReckoning> deadReck = std::make_shared<ConstructDeadReckoning>();
		getRemoteBatcher()->StartBatchThreads(deadReck, getRemoteConstructs());
	}

	bool ConstructRepository::ConstructExists(std::shared_ptr<EntityIDRecord> eid)
	{
		try
		{
			getRemoteConstructs()->GetConstructByID(eid);
			return true;
		}
		catch (const ConstructNotFoundException)
		{
			// No op to check local constructs
		}

		try
		{
			getLocalConstructs()->GetConstructByID(eid);
			return true;
		}
		catch (const ConstructNotFoundException)
		{
			return false;
		}
	}
}
