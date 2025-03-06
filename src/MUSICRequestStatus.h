/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_LIBRARY_MUSIC_REQUEST_STATUS_H
#define DMI_MUSIC_LIBRARY_MUSIC_REQUEST_STATUS_H

#include <unordered_map>
#include <functional>
#include <optional>
#include <memory>
#include "MUSIC Messages/MUSIC Response Messages/RequestStatus.h"
#include "tangible_exception_helper.h"
#include"Interfaces/IToJSON.h"

namespace MUSICLibrary::Interfaces
{
	class IConstruct;
}

namespace MUSICLibrary
{
	typedef std::function<void(unsigned int, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>, unsigned int, std::shared_ptr<boost::json::object>)> MUSIC_REQ_HANDLER_FUNC;
	class MUSICRequestStatus
	{
	private:
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> Sender;
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> Receiver;
		unsigned int ExerciseID = 0;
		unsigned int RequestID = 0;
		std::optional<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus> LastStatus;

	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> getSender() const;
		void setSender(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> value);
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> getReceiver() const;
		void setReceiver(std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> value);
		unsigned int getExerciseID() const;
		void setExerciseID(unsigned int value);

		/// <summary>
		/// A unique ID per construct's request monitor for each request that construct creates.
		/// </summary>
		unsigned int getRequestID() const;
		void setRequestID(unsigned int value);

	private:
		std::optional<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus> getLastStatus() const;
		void setLastStatus(const std::optional<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus> &value);


		std::unordered_map<std::optional<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus>, MUSIC_REQ_HANDLER_FUNC> handlers;
		std::unordered_map<std::optional<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus>, std::shared_ptr<boost::json::object>> notifiedStatuses;

	public:
		MUSICRequestStatus(unsigned int exerciseID, unsigned int requestID, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> sender, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> receiver);

		void Notify(MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, std::shared_ptr<boost::json::object> optionalData = nullptr);

		void RegisterHandler(MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, MUSIC_REQ_HANDLER_FUNC handler);

	private:
		void ExecuteHandlerThenRemoveFromStatusSet(MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, std::shared_ptr<boost::json::object> optionalData = nullptr);

		bool LastStatusWasTerminal();
	};
}

#endif // !DMI_MUSIC_LIBRARY_MUSIC_REQUEST_STATUS_H