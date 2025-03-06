/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IMUSIC_LIBRARY_H
#define DMI_INTERFACES_IMUSIC_LIBRARY_H
#define SPDLOG_WCHAR_FILENAMES

#include <typeinfo>
#include <typeindex>
#include "../Visitors/MUSICEndpoint.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <chrono>
#include <string> 


namespace MUSICLibrary::Interfaces
{
	class IMUSICLibrary
	{
	public:
		virtual std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> getSimulationID() const = 0;
		virtual unsigned int getExerciseID() const = 0;
		virtual std::shared_ptr<IMessageFilter> getMessageFilter() const = 0;
		virtual std::shared_ptr<IInternalMessageVisitor> getInternalVisitor() const = 0;
		virtual std::shared_ptr<IMUSICMessageVisitor> getExternalVisitor() const = 0;
		virtual void SubscribeToMUSIC() = 0;
		virtual bool IsSubscribed() = 0;
		virtual void UnsubscribeToMUSIC() = 0;
		virtual std::shared_ptr<MUSICLibrary::Visitors::MUSICEndpoint> GetEndpoint(std::type_index type) = 0;
		// Pass in the endpoint object you'd like to initialize. You can just use its default constructor and this method will handle getting it set up.
		virtual std::shared_ptr<IMUSICLibrary> InitializeEndpoint(std::shared_ptr<MUSICLibrary::Visitors::MUSICEndpoint> endpoint) = 0;
		virtual std::shared_ptr<IConstruct> InitializeLocalConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo) = 0;
		virtual std::shared_ptr<IMUSICLibrary> RemoveMUSICEndpoint(std::type_index endpointType) = 0;
		virtual void Transmit(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message) = 0;

	private:
		// Use the time to generate a unique log name to help stop file confliction errors with VBS.
		int uniqueLogSuffix = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

		std::shared_ptr<spdlog::logger> logger = spdlog::rotating_logger_mt("MUSICLibrary Logger", L"C:\\ProgramData\\DiscoveryMachine\\logs\\music" + std::to_wstring(uniqueLogSuffix) + L".log", 1048576 * 5, 3, true);
	};
}

#endif // !DMI_INTERFACES_IMUSIC_LIBRARY_H