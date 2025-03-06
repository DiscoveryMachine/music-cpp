/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ConstructFactory.h"
#include "../../../tangible_exception_helper.h"
#include <iostream>

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;

namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Factory
{

	std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> ConstructFactory::getRegisteredLocalConstructs() const
	{
		return RegisteredLocalConstructs;
	}

	void ConstructFactory::setRegisteredLocalConstructs(const std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> &value)
	{
		RegisteredLocalConstructs = value;
	}

	std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> ConstructFactory::getRegisteredRemoteConstructs() const
	{
		return RegisteredRemoteConstructs;
	}

	void ConstructFactory::setRegisteredRemoteConstructs(const std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> &value)
	{
		RegisteredRemoteConstructs = value;
	}

	std::shared_ptr<IMUSICTransmitter> ConstructFactory::getTransmitter() const
	{
		return Transmitter;
	}

	void ConstructFactory::setTransmitter(std::shared_ptr<IMUSICTransmitter> value)
	{
		Transmitter = value;
	}

	std::shared_ptr<IConstructRepository> ConstructFactory::getRepository() const
	{
		return Repository;
	}

	void ConstructFactory::setRepository(std::shared_ptr<IConstructRepository> value)
	{
		Repository = value;
	}

	const std::wstring ConstructFactory::CREATE_ERROR_MSG_FORMAT = L"Cannot create construct of type {{{%s}}} because it is not registered. " 
																	"The most common cause of this exception is registering a local construct and " 
																	"attempting to create a remote construct with a ConstructDataMessage or vice versa.";
	const std::wstring ConstructFactory::INCOMPLETE_MESSAGE_FORMAT = L"ConstructFactory.Create({%s}) could not create a construct "
																	  "because the message was incomplete. Missing required field: {%s}";
	const std::wstring ConstructFactory::INVALID_ENTITY_ID_MESSAGE_FORMAT = L"Construct {{{%s}}} from Site:{{{%d}}}, App:{{{%d}}} has an invalid entity ID of 0. "
																			 "Entity IDs of 0 are reserved to reference the simulation at the site and app ID. Construct entity IDs must be greater than 0.";
	const std::wstring ConstructFactory::CONSTRUCT_DATA_PARAM = L"ConstructDataMessage constructData";
	const std::wstring ConstructFactory::CREATE_ERROR_MESSAGE_FORMAT = L"[Warning]: Attempted to create a construct of type {%s}, but a MissingMethodException was thrown. " 
																		"ConstructFactory.Create has returned a null construct.\nTypical causes of this exception are:\n{%s}\n";
	const std::wstring ConstructFactory::CREATE_ERROR_MESSAGE_CAUSE1 = L"A local construct was registered as a remote construct, then when the factory tries to instantiate "
																		"the local construct as a remote construct, there is no constructor present in the local construct that matches the remote construct by design.\n";
	const std::wstring ConstructFactory::CREATE_ERROR_MESSAGE_CAUSE2 = L"The given remote construct does not contain a constructor which matches the signature of: "
																		"MyRemoteConstruct(ConstructDataMessage constructData)\n";

	ConstructFactory::ConstructFactory(std::shared_ptr<IConstructRepository> repository, std::shared_ptr<IMUSICTransmitter> transmitter)
	{
		setRegisteredLocalConstructs(std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>>());
		setRegisteredRemoteConstructs(std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>>());
		setRepository(repository);
		setTransmitter(transmitter);
		nextEntityID = 1;
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> ConstructFactory::Create(std::shared_ptr<ConstructDataMessage> constructData)
	{
		if (constructData->getName() == L"")
		{
			std::string missingField = std::string("constructData.name");

			// Determine length of message with all fields + 1 for terminating null character and allocate that space
			// malloc creates a void pointer; cast to char pointer
			int length = INCOMPLETE_MESSAGE_FORMAT.length() + CONSTRUCT_DATA_PARAM.length() + missingField.length() + 1;
			char* messagePointer = (char*) malloc(length);

			// Format the string with the given fields and throw the exception with the full message
			snprintf(messagePointer, length, (char*) INCOMPLETE_MESSAGE_FORMAT.c_str(), CONSTRUCT_DATA_PARAM, missingField);
			throw std::invalid_argument(messagePointer);
		}

		if (constructData->getOriginID() == nullptr)
		{

			std::string missingField = std::string("constructData.OriginID");

			// Determine length of message with all fields + 1 for terminating null character and allocate that space
			// malloc creates a void pointer; cast to char pointer
			int length = INCOMPLETE_MESSAGE_FORMAT.length() + CONSTRUCT_DATA_PARAM.length() + missingField.length() + 1;
			char* messagePointer = (char*)malloc(length);

			// Format the string with the given fields and throw the exception with the full message
			snprintf(messagePointer, length, (char*) INCOMPLETE_MESSAGE_FORMAT.c_str(), CONSTRUCT_DATA_PARAM, missingField);
			throw std::invalid_argument(messagePointer);
		}

		if (constructData->getOriginID()->getEntityID() == 0)
		{
			unsigned int site = constructData->getOriginID()->getSiteID();
			unsigned int app = constructData->getOriginID()->getAppID();
			std::wstring name = constructData->getName();

			// Determine length of message with all fields (site and app which are 4 bytes each) + 1 for terminating null character and allocate that space
			// malloc creates a void pointer; cast to char pointer
			int length = INVALID_ENTITY_ID_MESSAGE_FORMAT.length() + name.length() + 8 + 1;
			char* messagePointer = (char*)malloc(length);

			// Format the string with the given fields and throw the exception with the full message
			snprintf(messagePointer, length, (char*) INVALID_ENTITY_ID_MESSAGE_FORMAT.c_str(), name, site, app);
			throw std::invalid_argument(messagePointer);
		}

		if (getRegisteredRemoteConstructs().count(constructData->getName()) == 0)
		{
			std::wstring name = constructData->getName();

			// Determine length of message with all fields + 1 for terminating null character and allocate that space
			// malloc creates a void pointer; cast to char pointer
			int length = CREATE_ERROR_MSG_FORMAT.length() + name.length() + 1;
			char* messagePointer = (char*)malloc(length);

			// Format the string with the given fields and throw the exception with the full message
			snprintf(messagePointer, length, (char*) CREATE_ERROR_MSG_FORMAT.c_str(), name);
			throw InvalidOperationException(messagePointer);
		}

		try
		{
			std::shared_ptr<AbstractConstructCreateInfo> createInfo = getRegisteredRemoteConstructs()[constructData->getName()];
			return createInfo->create(constructData, getTransmitter(), getRepository());
		}
		catch (const MissingMethodException &e)
		{
			std::cerr << e.what();

			// Determine length of message with all fields + 1 for terminating null character and allocate that space
			// malloc creates a void pointer; cast to char pointer
			int length = CREATE_ERROR_MSG_FORMAT.length() + CREATE_ERROR_MESSAGE_FORMAT.length() + constructData->getName().length() + CREATE_ERROR_MESSAGE_CAUSE1.length() + CREATE_ERROR_MESSAGE_CAUSE2.length() + 1;
			char* messagePointer = (char*)malloc(length);

			// Format the string with the given fields and throw the exception with the full message
			snprintf(messagePointer, length, (char*) CREATE_ERROR_MSG_FORMAT.c_str(), CREATE_ERROR_MESSAGE_FORMAT + constructData->getName() + CREATE_ERROR_MESSAGE_CAUSE1 + CREATE_ERROR_MESSAGE_CAUSE2);
			std::cerr << *messagePointer;
			throw e;
		}
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> ConstructFactory::Create(std::wstring fullyQualifiedName, std::wstring callsign)
	{
		// Make sure this construct is registered first.
		if (getRegisteredLocalConstructs().count(fullyQualifiedName) == 0)
		{
			throw InvalidOperationException((char*) (CREATE_ERROR_MSG_FORMAT + fullyQualifiedName).c_str());
		}

		// Get its create info and modify it
		std::shared_ptr<AbstractConstructCreateInfo> createInfo = RegisteredLocalConstructs.at(fullyQualifiedName);

		// Since these are set with every create call and are part of every create info, no need to create a clone
		createInfo->setTransmitter(getTransmitter());
		createInfo->setConstructID(std::make_shared<EntityIDRecord>(getTransmitter()->GetSiteAndAppID(), nextEntityID++));
		createInfo->setRepository(getRepository());
		createInfo->setCallsign(callsign);

		try
		{
			return createInfo->create();
		}
		catch (const MissingMethodException)
		{
			// Determine length of message with all fields + 1 for terminating null character and allocate that space
			// malloc creates a void pointer; cast to char pointer
			int length = CREATE_ERROR_MSG_FORMAT.length() + CREATE_ERROR_MESSAGE_FORMAT.length() + fullyQualifiedName.length() + CREATE_ERROR_MESSAGE_CAUSE1.length() + CREATE_ERROR_MESSAGE_CAUSE2.length() + 1;
			char* messagePointer = (char*)malloc(length);

			// Format the string with the given fields and throw the exception with the full message
			snprintf(messagePointer, length, (char*) CREATE_ERROR_MSG_FORMAT.c_str(), CREATE_ERROR_MESSAGE_FORMAT + fullyQualifiedName + CREATE_ERROR_MESSAGE_CAUSE1 + CREATE_ERROR_MESSAGE_CAUSE2);
			std::wcout << *messagePointer;
			return nullptr;
		}
	}

	void ConstructFactory::RegisterLocalConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo)
	{
		if (getRegisteredLocalConstructs().count(createInfo->getQualifiedName()) == 0)
		{
			RegisteredLocalConstructs.emplace(createInfo->getQualifiedName(), createInfo);
		}
	}

	void ConstructFactory::RegisterRemoteConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo)
	{
		RegisteredRemoteConstructs.emplace(createInfo->getQualifiedName(), createInfo);
	}

	bool ConstructFactory::IsConstructRegistered(const std::wstring &fullyQualifiedName)
	{
		return ((getRegisteredLocalConstructs().count(fullyQualifiedName) != 0) || (getRegisteredRemoteConstructs().count(fullyQualifiedName) != 0));
	}
}
