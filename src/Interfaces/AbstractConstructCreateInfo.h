/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ICONSTRUCT_CREATE_INFO_H
#define DMI_INTERFACES_ICONSTRUCT_CREATE_INFO_H

#include <typeinfo>
#include "IConstructRepository.h"
#include "IMUSICTransmitter.h"


namespace MUSICLibrary::Interfaces
{
	/**
	* This class was translated from IConstructCreateInfo in the C# MUSIC Library. Name was changed from "I" to "Abstract" because
	* C++ doesn't allow us to use properties like C# does, so we had to make them members, and it was weird to have an interface
	* with members.
	*/
	class AbstractConstructCreateInfo : public std::enable_shared_from_this<AbstractConstructCreateInfo>
	{
	private:
		std::wstring qualifiedName;
		std::wstring callsign;
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> constructID;
		std::shared_ptr<IConstructRepository> repo;
		std::shared_ptr<IMUSICTransmitter> transmitter;

	public:
		// This clone method should be implemented in a subclass by something like "return new SubclassName(*this)".
		virtual AbstractConstructCreateInfo* clone() const { return nullptr; };
		virtual std::wstring getQualifiedName() const { return qualifiedName; };
		virtual void setQualifiedName(const std::wstring &value) { qualifiedName = value; };
		virtual std::wstring getCallsign() const { return callsign; };
		virtual void setCallsign(const std::wstring &value) { callsign = value; };

		virtual std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> create() { return nullptr; };
		virtual std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> create(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> constructData,
			std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter,
			std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> constructRepository) { return nullptr; };

		/// <summary>
		/// ConstructID is only set by the construct factory because
		/// the ID of the construct is unknown before its instantiation due to the requirement that
		/// ConstructID.EntityID must be unique.
		/// </summary>
		virtual std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> getConstructID() const { return constructID; };
		virtual void setConstructID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> value) { constructID = value; };
		virtual std::shared_ptr<IConstructRepository> getRepository() const { return repo; };
		virtual void setRepository(std::shared_ptr<IConstructRepository> value) { repo = value; };
		virtual std::shared_ptr<IMUSICTransmitter> getTransmitter() const { return transmitter; };
		virtual void setTransmitter(std::shared_ptr<IMUSICTransmitter> value) { transmitter = value; };

	protected:
		std::shared_ptr<AbstractConstructCreateInfo> shared_from_this() {
			return std::enable_shared_from_this<AbstractConstructCreateInfo>::shared_from_this();
		}
	};
}

#endif // !DMI_INTERFACES_ICONSTRUCT_CREATE_INFO_H