/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_FACTORY_CONSTRUCT_FACTORY_H
#define DMI_CONSTRUCT_FACTORY_CONSTRUCT_FACTORY_H

#include <unordered_map>
#include "../../../Interfaces/IConstructFactory.h"


namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Factory
{
	class ConstructFactory : public MUSICLibrary::Interfaces::IConstructFactory
	{
	private:
		std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> RegisteredLocalConstructs;
		std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> RegisteredRemoteConstructs;
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> Transmitter;
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> Repository;

		std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> getRegisteredLocalConstructs() const;
		void setRegisteredLocalConstructs(const std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> &value);
		std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> getRegisteredRemoteConstructs() const;
		void setRegisteredRemoteConstructs(const std::unordered_map<std::wstring, std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo>> &value);
	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> getTransmitter() const;
		void setTransmitter(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> value);
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> getRepository() const;
		void setRepository(std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> value);

	private:
		static const std::wstring CREATE_ERROR_MSG_FORMAT;

		static const std::wstring INCOMPLETE_MESSAGE_FORMAT;

		static const std::wstring INVALID_ENTITY_ID_MESSAGE_FORMAT;

		static const std::wstring CONSTRUCT_DATA_PARAM;

		static const std::wstring CREATE_ERROR_MESSAGE_FORMAT;

		static const std::wstring CREATE_ERROR_MESSAGE_CAUSE1;

		static const std::wstring CREATE_ERROR_MESSAGE_CAUSE2;

		unsigned int nextEntityID = 0;

	public:
		ConstructFactory(std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> repository, std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter);

		/// <summary>
		/// Creates a new remote construct.
		/// Throws:
		///   ArgumentNullException when the construct's fully qualified name or origin ID are null.
		///   InvalidOperationException when a construct's fully qualified name is not registered with its type.
		/// </summary>
		/// <param name="constructData">The construct data message received from an endpoint.</param>
		/// <returns>A new construct of the type associated with the ConstructDataMessage's fully qualified name.</returns>
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> Create(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> constructData) override;

		/// <summary>
		/// Creates a new local construct from the minimum amount of information needed to create a new construct.
		/// </summary>
		/// <param name="fullyQualifiedName">The fully qualified name of the construct so we can find the create info in the list of
		/// registered local constructs..</param>
		/// /// <param name="callsign">The unique callsign to give to the specific instance of this construct when we create it from
		/// the create info we obtain from the list of registered local constructs.</param>
		/// <returns>A new construct instance of the type associated with the ConstructCreateInfo's fully qualified name.</returns>
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> Create(std::wstring fullyQualifiedName, std::wstring callsign) override;

		void RegisterLocalConstruct(std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo> createInfo) override;

		void RegisterRemoteConstruct(std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo> createInfo) override;

		bool IsConstructRegistered(const std::wstring &fullyQualifiedName) override;
	};
}

#endif // !DMI_CONSTRUCT_FACTORY_CONSTRUCT_FACTORY_H