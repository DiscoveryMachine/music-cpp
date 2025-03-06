/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DefaultInternalMessageVisitor.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages;
using namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages;

namespace MUSICLibrary::Visitors
{

	std::shared_ptr<IConstructFactory> DefaultInternalMessageVisitor::getConstructFactory() const
	{
		return ConstructFactory;
	}

	void DefaultInternalMessageVisitor::setConstructFactory(std::shared_ptr<IConstructFactory> value)
	{
		ConstructFactory = value;
	}

	std::shared_ptr<IConstructRepository> DefaultInternalMessageVisitor::getConstructRepository() const
	{
		return ConstructRepository;
	}

	void DefaultInternalMessageVisitor::setConstructRepository(std::shared_ptr<IConstructRepository> value)
	{
		ConstructRepository = value;
	}

	std::shared_ptr<IMUSICTransmitter> DefaultInternalMessageVisitor::getTransmitter() const
	{
		return Transmitter;
	}

	void DefaultInternalMessageVisitor::setTransmitter(std::shared_ptr<IMUSICTransmitter> value)
	{
		Transmitter = value;
	}

	std::unordered_map<std::wstring, std::vector<std::function<void(std::shared_ptr<boost::json::object>)>>> DefaultInternalMessageVisitor::getEventHandlers() const
	{
		return EventHandlers;
	}

	void DefaultInternalMessageVisitor::setEventHandlers(const std::unordered_map<std::wstring, std::vector<std::function<void(std::shared_ptr<boost::json::object>)>>> &value)
	{
		EventHandlers = value;
	}

	//Need to cast at least two wstring otherwise it reads them has pointers and you can not add 2+ pointers
	//Rest of strings will be implcitly converted to strings
	const std::wstring DefaultInternalMessageVisitor::LOCAL_EVENT_MESSAGE_ERROR_MSG = L"[Warning]: The OriginID's SiteAndAppID of the MUSICEventMessage matches that of the local simulation. " + 
	std::wstring(L"This simulation should not receive a message to handle an event that a local construct or the simulation itself has triggered. ") + 
	L"A common cause of this error is an errorneous OriginID.";

	const std::wstring DefaultInternalMessageVisitor::WRONG_EXERCISE_ID_ERROR_MSG_FORMAT = L"[Warning]: The event message's ExerciseID did not match the stored ExerciseID. " + 
	std::wstring (L"DefaultInternalMessageVisitorExerciseID: {0}, MUSICEventMessage ExerciseID: {1}\n") + 
	L"1 of 2 things may cause this exception to be thrown:\n\t" + 
	L"1: The message has an erroneous ExerciseID that somehow made it through message filtering.\n\t" + 
	L"2: This instance of a DefaultInternalMessageVisitor was instantiated with an ExerciseID that does not match the current simulation.";

	const std::wstring DefaultInternalMessageVisitor::EVENT_NOT_FOUND_ERROR_MSG_FORMAT = L"[Warning]: The event of type: {0} could not be found within the EventHandlers dictionary. " + 
	std::wstring(L"A common cause of this error is not registering the appropriate MUSICEventType before the library subscribes to MUSIC.");

	const std::wstring DefaultInternalMessageVisitor::HANDLER_NOT_FOUND_ERROR_MSG_FORMAT = L"[Warning]: An event handler was not found for event type: {0}. " + 
	std::wstring(L"When registering a handler for this event, ensure that the handler function registered to the event ") + 
	L"type is not null.";

	const std::wstring DefaultInternalMessageVisitor::CONSTRUCT_NOT_FOUND_ERROR_MSG = L"[Warning]: The default internal message visitor could not make the state field message accept the construct visitor. Construct not found.";

	DefaultInternalMessageVisitor::DefaultInternalMessageVisitor(unsigned int exerciseID, std::shared_ptr<SiteAndAppID> siteAndApp)
	{
		this->exerciseID = exerciseID;
		this->siteAndApp = siteAndApp;
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> DefaultInternalMessageVisitor::InitializeLocalConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo)
	{
		ConstructFactory->RegisterLocalConstruct(createInfo);
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> localConstruct = getConstructFactory()->Create(createInfo->getQualifiedName(), createInfo->getCallsign());
		ConstructRepository->AddConstruct(localConstruct);
		createInfo->getTransmitter()->Transmit(localConstruct->GetConstructData());
		return localConstruct;
	}

	void DefaultInternalMessageVisitor::RegisterEventHandler(const std::wstring &eventName, std::function<void(std::shared_ptr<boost::json::object>)> eventHandler)
	{
		if (EventHandlers.find(eventName) == EventHandlers.end())
		{
			EventHandlers[eventName] = std::vector<std::function<void(std::shared_ptr<boost::json::object>)>>();
		}
		EventHandlers[eventName].push_back(eventHandler);
	}

	void DefaultInternalMessageVisitor::DeregisterEventHandler(const std::wstring &eventName)
	{
		EventHandlers.erase(eventName);
	}

	void DefaultInternalMessageVisitor::Initialize(std::shared_ptr<IConstructFactory> constructFactory, std::shared_ptr<IConstructRepository> constructRepository)
	{
		setConstructFactory(constructFactory);
		setConstructRepository(constructRepository);
		setEventHandlers(std::unordered_map<std::wstring, std::vector<std::function<void(std::shared_ptr<boost::json::object>)>>>());
	}

	void DefaultInternalMessageVisitor::VisitConstructData(std::shared_ptr<ConstructDataMessage> message)
	{
		if (ConstructRepository->ConstructExists(message->getOriginID()))
		{
			std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> construct = ConstructRepository->GetConstructByID(message->getOriginID());
			if (construct->IsRemote())
			{
				construct->GetConstructData()->Update(message);
			}
			else
			{
				throw InvalidOperationException("Local constructs should never receive an update through a construct data message.");
			}
		}
		else
		{
			ConstructRepository->AddConstruct(getConstructFactory()->Create(message));
		}
	}

	void DefaultInternalMessageVisitor::VisitInteractionRequest(std::shared_ptr<InteractionRequestMessage> message)
	{

		std::shared_ptr<IConstruct> construct = getConstructRepository()->GetConstructByID(message->getReceiverID());
		
		message->AcceptVisitor(construct);
	}

	void DefaultInternalMessageVisitor::VisitPrimaryControlGranted(std::shared_ptr<PrimaryControlGrantedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlGranted(std::shared_ptr<ControlGrantedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlTransferGranted(std::shared_ptr<ControlTransferGrantedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitInteractionResponse(std::shared_ptr<InteractionResponseMessage> message)
	{
		message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
	}

	void DefaultInternalMessageVisitor::VisitMUSICEvent(std::shared_ptr<MUSICEventMessage> message)
	{
		if (message->getOriginID()->GetSiteAndApp()->Equals(siteAndApp))
		{
			std::wcout << LOCAL_EVENT_MESSAGE_ERROR_MSG << std::endl;
			return;
		}

		if (message->getMUSICHeader()->getExerciseID() != exerciseID)
		{
			std::wstring errMsg = WRONG_EXERCISE_ID_ERROR_MSG_FORMAT;
			int index = errMsg.find(L"{0}",0);
			errMsg.replace(index, 3, std::to_wstring(exerciseID));
			index = errMsg.find(L"{1}", 0);
			errMsg.replace(index, 3, std::to_wstring(message->getMUSICHeader()->getExerciseID()));

			std::wcout << errMsg << std::endl;
			return;
		}

		if (EventHandlers.find(message->getEventType()) == EventHandlers.end())
		{
			std::wstring errMsg = EVENT_NOT_FOUND_ERROR_MSG_FORMAT;
			int index = errMsg.find(L"{0}",0);
			errMsg.replace(index, 3, message->getEventType());
			std::wcout << errMsg << std::endl;
			return;
		}

		TriggerEvent(message->getEventType(), message->getEventData());
	}

	void DefaultInternalMessageVisitor::TriggerEvent(const std::wstring &eventType, std::shared_ptr<boost::json::object> eventData)
	{
		// If there are no handlers for this event type, throw exception
		if (EventHandlers[eventType].size() == 0)
		{
			std::wstring errMsg = HANDLER_NOT_FOUND_ERROR_MSG_FORMAT;
			int index = errMsg.find(L"{0}", 0);
			errMsg.replace(index, 3, eventType);

			throw HandlerNotFoundException(StringHelper::wstring_to_string(errMsg));
		}

		// It is safe for the user to catch this exception and ignore if necessary
		for (auto handler : EventHandlers[eventType])
		{
			if (handler == nullptr)
			{
				std::wstring errMsg = HANDLER_NOT_FOUND_ERROR_MSG_FORMAT;
				int index = errMsg.find(L"{0}", 0);
				errMsg.replace(index, 3, eventType);

				throw HandlerNotFoundException(StringHelper::wstring_to_string(errMsg));
			}
			else
			{
				if (handler != nullptr)
				{
					std::invoke(handler, eventData);
				}
			}
		}
	}

	void DefaultInternalMessageVisitor::VisitStateField(std::shared_ptr<StateFieldMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getOriginID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getOriginID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlGained(std::shared_ptr<ControlGainedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiver()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiver()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitWaypointData(std::shared_ptr<WaypointDataMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getOriginID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getOriginID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitPerceptionData(std::shared_ptr<PerceptionDataMessage> message)
	{
		if (ConstructRepository->ConstructExists(message->getOriginID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getOriginID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlRequest(std::shared_ptr<ControlRequestMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitPrimaryControlRequest(std::shared_ptr<PrimaryControlRequestMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlTransferRequest(std::shared_ptr<ControlTransferRequestMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlResponse(std::shared_ptr<ControlResponseMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitPrimaryControlResponse(std::shared_ptr<PrimaryControlResponseMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlTransferResponse(std::shared_ptr<ControlTransferResponseMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitSetCurrentController(std::shared_ptr<SetCurrentControllerMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlLost(std::shared_ptr<ControlLostMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlRegained(std::shared_ptr<ControlRegainedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlRelinquished(std::shared_ptr<ControlRelinquishedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitPrimaryControlRelinquished(std::shared_ptr<PrimaryControlRelinquishedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitControlReclamation(std::shared_ptr<ControlReclamationMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitSimulationTime(std::shared_ptr<SimulationTimeMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitControlInitiated(std::shared_ptr<ControlInitiatedMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitControlReleased(std::shared_ptr<ControlReleasedMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitTransferConstructID(std::shared_ptr<TransferConstructIDMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitDisplayMessages(std::shared_ptr<DisplayMessagesMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitStopConstruct(std::shared_ptr<StopConstructMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitRemoveConstruct(std::shared_ptr<RemoveConstructMessage> message)
	{
		// no-op. there noting we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitSetSimulationTime(std::shared_ptr<SetSimulationTimeMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitRequestSimulationTime(std::shared_ptr<RequestSimulationTimeMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitCreateEnvironmentRequest(std::shared_ptr<CreateEnvironmentRequestMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitCreateEnvironmentResponse(std::shared_ptr<CreateEnvironmentResponseMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitFinalizeScenarioRequest(std::shared_ptr<FinalizeScenarioRequestMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitFinalizeScenarioResponse(std::shared_ptr<FinalizeScenarioResponseMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitCreateConstructRequest(std::shared_ptr<CreateConstructRequestMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitCreateConstructResponse(std::shared_ptr<CreateConstructResponseMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitParameterizeConstructRequest(std::shared_ptr<ParameterizeConstructRequestMessage> message)
	{
		if (getConstructRepository()->ConstructExists(message->getReceiverID()))
		{
			message->AcceptVisitor(getConstructRepository()->GetConstructByID(message->getReceiverID()));
		}
		else
		{
			throw KeyNotFoundException(StringHelper::wstring_to_string(CONSTRUCT_NOT_FOUND_ERROR_MSG));
		}
	}

	void DefaultInternalMessageVisitor::VisitParameterizeConstructResponse(std::shared_ptr<ParameterizeConstructResponseMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}

	void DefaultInternalMessageVisitor::VisitScenarioStart(std::shared_ptr<ScenarioStartMessage> message)
	{
		//  no-op, there's nothing we can handle here, up to user to handle in their own message visitor
	}
}
