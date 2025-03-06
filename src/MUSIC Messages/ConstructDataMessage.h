/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_CONSTRUCT_DATA_MESSAGE_H
#define DMI_MUSIC_MESSAGES_CONSTRUCT_DATA_MESSAGE_H

#include "MUSICMessage.h"
#include "../tangible_string_helper.h"
#include "Construct Data/ConstructRender.h"
#include "Construct Data/ConstructType.h"
#include "Construct Data/InteractionRecord.h"
#include "Construct Data/PhysicalRecord.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class ConstructDataMessage : public MUSICMessage
	{
	private:
		std::wstring Callsign;
		std::wstring Name;
		std::shared_ptr<EntityIDRecord> GhostedID;
		std::shared_ptr<EntityIDRecord> PrimaryControllerID;
		std::shared_ptr<EntityIDRecord> CurrentControllerID;
		std::optional<MUSICLibrary::MUSIC_Messages::ConstructRender> ConstructRender;
		std::optional<MUSICLibrary::MUSIC_Messages::ConstructType> ConstructType;
		std::shared_ptr<InteractionRecord> Interactions;
		std::shared_ptr<PhysicalRecord> Physical;

		static const std::wstring FORCE;
		static const std::wstring ENTITY_TYPE;
		static const std::wstring LOCATION;
		static const std::wstring ORIENTATION;
		static const std::wstring VELOCITY;
		static const std::wstring DAMAGE_RECORD;
		static const std::wstring DEAD_RECK;

		static const std::wstring HEADER;
		static const std::wstring ORIGIN_ID;
		static const std::wstring GHOSTED_ID;
		static const std::wstring PRIMARY_CONTROLLER_ID;
		static const std::wstring CURRENT_CONTROLLER_ID;
		static const std::wstring CONSTRUCT_INFO_RECORD;
		static const std::wstring CONSTRUCT_RENDER;
		static const std::wstring CONSTRUCT_TYPE;
		static const std::wstring CALLSIGN;
		static const std::wstring CONSTRUCT_NAME;
		static const std::wstring INTERACTION_RECORD;

		static const std::wstring PSI;
		static const std::wstring THETA;
		static const std::wstring PHI;


	public:
		std::wstring getCallsign() const;
		void setCallsign(const std::wstring &value);
		std::wstring getName() const;
		void setName(const std::wstring &value);

		std::shared_ptr<EntityIDRecord> getGhostedID() const;
		void setGhostedID(std::shared_ptr<EntityIDRecord> value);
		std::shared_ptr<EntityIDRecord> getPrimaryControllerID() const;
		void setPrimaryControllerID(std::shared_ptr<EntityIDRecord> value);
		std::shared_ptr<EntityIDRecord> getCurrentControllerID() const;
		void setCurrentControllerID(std::shared_ptr<EntityIDRecord> value);

		std::optional<MUSICLibrary::MUSIC_Messages::ConstructRender> getConstructRender() const;
		void setConstructRender(const std::optional<MUSICLibrary::MUSIC_Messages::ConstructRender> &value);
		std::optional<MUSICLibrary::MUSIC_Messages::ConstructType> getConstructType() const;
		void setConstructType(const std::optional<MUSICLibrary::MUSIC_Messages::ConstructType> &value);

		std::shared_ptr<InteractionRecord> getInteractions() const;
		void setInteractions(std::shared_ptr<InteractionRecord> value);
		std::shared_ptr<PhysicalRecord> getPhysical() const;
		void setPhysical(std::shared_ptr<PhysicalRecord> value);


		ConstructDataMessage();

		ConstructDataMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> constructID, const std::wstring &callsign, const std::wstring &name);

		ConstructDataMessage(std::shared_ptr<boost::json::object> obj);

		void Update(std::shared_ptr<ConstructDataMessage> message);

	private:
		std::shared_ptr<PhysicalRecord> GetPhysicalRecordFromJObjectIfFieldExists(std::shared_ptr<boost::json::object> obj);

	public:
		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ConstructDataMessage& obj) const;
		};

		bool operator == (const ConstructDataMessage &message2);

		bool operator != (const ConstructDataMessage &message2);

	protected:
		std::shared_ptr<ConstructDataMessage> shared_from_this()
		{
			return std::static_pointer_cast<ConstructDataMessage>(MUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_MESSAGES_CONSTRUCT_DATA_MESSAGE_H