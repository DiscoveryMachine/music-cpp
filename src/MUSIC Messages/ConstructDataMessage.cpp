/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ConstructDataMessage.h"
#include "../Interfaces/IMUSICMessageVisitor.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;
using namespace MUSICLibrary::MUSIC_Messages;

namespace MUSICLibrary::MUSIC_Messages
{
	const std::wstring ConstructDataMessage::FORCE = L"force";
	const std::wstring ConstructDataMessage::ENTITY_TYPE = L"entityType";
	const std::wstring ConstructDataMessage::LOCATION = L"location";
	const std::wstring ConstructDataMessage::ORIENTATION = L"orientation";
	const std::wstring ConstructDataMessage::VELOCITY = L"velocity";
	const std::wstring ConstructDataMessage::DAMAGE_RECORD = L"damageRecord";
	const std::wstring ConstructDataMessage::DEAD_RECK = L"deadReck";
	const std::wstring ConstructDataMessage::HEADER = L"header";
	const std::wstring ConstructDataMessage::ORIGIN_ID = L"originID";
	const std::wstring ConstructDataMessage::GHOSTED_ID = L"ghostedID";
	const std::wstring ConstructDataMessage::PRIMARY_CONTROLLER_ID = L"primaryControllerID";
	const std::wstring ConstructDataMessage::CURRENT_CONTROLLER_ID = L"currentControllerID";
	const std::wstring ConstructDataMessage::CONSTRUCT_INFO_RECORD = L"constructInformationRecord";
	const std::wstring ConstructDataMessage::CONSTRUCT_RENDER = L"constructRender";
	const std::wstring ConstructDataMessage::CONSTRUCT_TYPE = L"constructType";
	const std::wstring ConstructDataMessage::CALLSIGN = L"callsign";
	const std::wstring ConstructDataMessage::CONSTRUCT_NAME = L"constructName";
	const std::wstring ConstructDataMessage::INTERACTION_RECORD = L"interactionRecord";
	const std::wstring ConstructDataMessage::PSI = L"psi";
	const std::wstring ConstructDataMessage::THETA = L"theta";
	const std::wstring ConstructDataMessage::PHI = L"phi";

	std::wstring ConstructDataMessage::getCallsign() const
	{
		return Callsign;
	}

	void ConstructDataMessage::setCallsign(const std::wstring &value)
	{
		Callsign = value;
	}

	std::wstring ConstructDataMessage::getName() const
	{
		return Name;
	}

	void ConstructDataMessage::setName(const std::wstring &value)
	{
		Name = value;
	}

	std::shared_ptr<EntityIDRecord> ConstructDataMessage::getGhostedID() const
	{
		return GhostedID;
	}

	void ConstructDataMessage::setGhostedID(std::shared_ptr<EntityIDRecord> value)
	{
		GhostedID = value;
	}

	std::shared_ptr<EntityIDRecord> ConstructDataMessage::getPrimaryControllerID() const
	{
		return PrimaryControllerID;
	}

	void ConstructDataMessage::setPrimaryControllerID(std::shared_ptr<EntityIDRecord> value)
	{
		PrimaryControllerID = value;
	}

	std::shared_ptr<EntityIDRecord> ConstructDataMessage::getCurrentControllerID() const
	{
		return CurrentControllerID;
	}

	void ConstructDataMessage::setCurrentControllerID(std::shared_ptr<EntityIDRecord> value)
	{
		CurrentControllerID = value;
	}

	std::optional<MUSICLibrary::MUSIC_Messages::ConstructRender> ConstructDataMessage::getConstructRender() const
	{
		return ConstructRender;
	}

	void ConstructDataMessage::setConstructRender(const std::optional<MUSICLibrary::MUSIC_Messages::ConstructRender> &value)
	{
		ConstructRender = value;
	}

	std::optional<MUSICLibrary::MUSIC_Messages::ConstructType> ConstructDataMessage::getConstructType() const
	{
		return ConstructType;
	}

	void ConstructDataMessage::setConstructType(const std::optional<MUSICLibrary::MUSIC_Messages::ConstructType> &value)
	{
		ConstructType = value;
	}

	std::shared_ptr<InteractionRecord> ConstructDataMessage::getInteractions() const
	{
		return Interactions;
	}

	void ConstructDataMessage::setInteractions(std::shared_ptr<InteractionRecord> value)
	{
		Interactions = value;
	}

	std::shared_ptr<PhysicalRecord> ConstructDataMessage::getPhysical() const
	{
		return Physical;
	}

	void ConstructDataMessage::setPhysical(std::shared_ptr<PhysicalRecord> value)
	{
		Physical = value;
	}

	ConstructDataMessage::ConstructDataMessage() : MUSICMessage()
	{
	}

	ConstructDataMessage::ConstructDataMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> constructID, const std::wstring &callsign, const std::wstring &name) : MUSICMessage(exerciseID, constructID)
	{
		setCallsign(callsign);
		setName(name);
		setInteractions(std::make_shared<InteractionRecord>());
	}

	ConstructDataMessage::ConstructDataMessage(std::shared_ptr<object> obj) : MUSICMessage((*obj)["header"].as_object(), (*obj)["originID"].as_object())
	{
		object objVal = *obj;
		// If there is a ghosted ID, extract it, put it into a shared pointer, and set as ghosted ID
		if (objVal[StringHelper::wstringToString(GHOSTED_ID)] != nullptr)
		{
			setGhostedID(extractEntityIDRecordFromJson(objVal, StringHelper::wstringToString(GHOSTED_ID)));
		}

		setPrimaryControllerID(extractEntityIDRecordFromJson(objVal, StringHelper::wstringToString(PRIMARY_CONTROLLER_ID)));
		setCurrentControllerID(extractEntityIDRecordFromJson(objVal, StringHelper::wstringToString(CURRENT_CONTROLLER_ID)));

		object constructInfoRecord = objVal[StringHelper::wstringToString(CONSTRUCT_INFO_RECORD)].as_object();
		setConstructRender(std::make_optional<MUSICLibrary::MUSIC_Messages::ConstructRender>(static_cast<MUSICLibrary::MUSIC_Messages::ConstructRender>(value_to<int>(constructInfoRecord[StringHelper::wstringToString(CONSTRUCT_RENDER)]))));
		setConstructType(std::make_optional<MUSICLibrary::MUSIC_Messages::ConstructType>(static_cast<MUSICLibrary::MUSIC_Messages::ConstructType>(value_to<int>(constructInfoRecord[StringHelper::wstringToString(CONSTRUCT_TYPE)]))));

		setPhysical(GetPhysicalRecordFromJObjectIfFieldExists(obj));

		setCallsign(StringHelper::stringToWString(objVal[StringHelper::wstringToString(CALLSIGN)].as_string()));
		setName(StringHelper::stringToWString(objVal[StringHelper::wstringToString(CONSTRUCT_NAME)].as_string()));

		if (getInteractions() == nullptr)
		{
			setInteractions(std::make_shared<InteractionRecord>());
		}
		if (objVal.contains(StringHelper::wstringToString(INTERACTION_RECORD)))
		{
			getInteractions()->AddAllInteraction(StringHelper::stringToWString(objVal[StringHelper::wstringToString(INTERACTION_RECORD)].as_string()));
		}
	}

	void ConstructDataMessage::Update(std::shared_ptr<ConstructDataMessage> message)
	{
		setCallsign(message->getCallsign() == L"" ? getCallsign() : message->getCallsign());
		setName(message->getName() == L"" ? getName() : message->getName());
		setGhostedID(message->getGhostedID() == nullptr ? getGhostedID() : message->getGhostedID());
		setPrimaryControllerID(message->getPrimaryControllerID() == nullptr ? getPrimaryControllerID() : message->getPrimaryControllerID());
		setCurrentControllerID(message->getCurrentControllerID() == nullptr ? getCurrentControllerID() : message->getCurrentControllerID());
		setConstructRender(!message->getConstructRender() ? getConstructRender() : message->getConstructRender());
		setConstructType(!message->getConstructType() ? getConstructType() : message->getConstructType());
		setInteractions(message->getInteractions() == nullptr ? getInteractions() : message->getInteractions());
		setPhysical(message->getPhysical() == nullptr ? getPhysical() : message->getPhysical());
	}

	std::shared_ptr<PhysicalRecord> ConstructDataMessage::GetPhysicalRecordFromJObjectIfFieldExists(std::shared_ptr<object> obj)
	{
		object objVal = *obj;

		PhysicalRecord tempPhysical = PhysicalRecord();

		std::shared_ptr<DamageRecord> damage;
		if (objVal.contains(StringHelper::wstringToString(DAMAGE_RECORD)))
		{
			std::shared_ptr<object> dmgRecordObj = std::make_shared<object>(objVal[StringHelper::wstringToString(DAMAGE_RECORD)].as_object());
			damage = std::make_shared<DamageRecord>(DamageRecord::DamageRecord(dmgRecordObj));
			tempPhysical.setDamage(damage);
		}

		std::shared_ptr<DeadReckoningParametersRecord> drpr;
		if (objVal.contains(StringHelper::wstringToString(DEAD_RECK)))
		{
			std::shared_ptr<object> DRParametersObj = std::make_shared<object>(objVal[StringHelper::wstringToString(DEAD_RECK)].as_object());
			drpr = std::make_shared<DeadReckoningParametersRecord>(DeadReckoningParametersRecord::DeadReckoningParametersRecord(DRParametersObj));
			tempPhysical.setDeadReckoningParameters(drpr);
		}

		std::shared_ptr<EntityTypeRecord> et;
		if (objVal.contains(StringHelper::wstringToString(ENTITY_TYPE)))
		{
			std::shared_ptr<object> entityTypeRecObj = std::make_shared<object>(objVal[StringHelper::wstringToString(ENTITY_TYPE)].as_object());
			et = std::make_shared<EntityTypeRecord> (EntityTypeRecord::EntityTypeRecord(entityTypeRecObj));
			tempPhysical.setEntityType(et);
		}

		std::optional<Force> force;
		if (objVal.contains(StringHelper::wstringToString(FORCE)))
		{	
			unsigned char forceObj = value_to<unsigned char>(objVal[StringHelper::wstringToString(FORCE)]);
			force = std::optional<Force>( static_cast<Force>(forceObj));
			tempPhysical.setForceIDField(force);
		}

		std::shared_ptr<MUSICVector3> linearVel;
		if (objVal.contains(StringHelper::wstringToString(VELOCITY)))
		{
			std::shared_ptr<object> linearObj = std::make_shared<object>(objVal[StringHelper::wstringToString(VELOCITY)].as_object());
			linearVel = std::make_shared<MUSICVector3>(MUSICVector3::MUSICVector3(linearObj));
			tempPhysical.setLinearVelocity(linearVel);
		}

		std::shared_ptr<MUSICVector3> location;
		if (objVal.contains(StringHelper::wstringToString(LOCATION)))
		{
			std::shared_ptr<object> locObj = std::make_shared<object>(objVal[StringHelper::wstringToString(LOCATION)].as_object());
			location = std::make_shared<MUSICVector3>(MUSICVector3::MUSICVector3(locObj));
			tempPhysical.setLocation(location);
		}

		std::shared_ptr<MUSICVector3> orientation;
		if (objVal.contains(StringHelper::wstringToString(ORIENTATION)))
		{
			object oriObj = objVal[StringHelper::wstringToString(ORIENTATION)].as_object();

			double psi = oriObj[StringHelper::wstringToString(PSI)].as_double();
			double theta = oriObj[StringHelper::wstringToString(THETA)].as_double();
			double phi = oriObj[StringHelper::wstringToString(PHI)].as_double();
			orientation = std::make_shared<MUSICVector3>(psi, theta, phi);
			tempPhysical.setOrientation(orientation);
		}

		return tempPhysical.Equals(std::make_shared<PhysicalRecord>()) ? nullptr : std::make_shared<PhysicalRecord>(tempPhysical);
	}

	void ConstructDataMessage::AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor)
	{
		visitor->VisitConstructData(shared_from_this());
	}

	std::any ConstructDataMessage::Clone()
	{
		//HashSet<string>
		std::shared_ptr<EntityIDRecord> clonedConstructID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::wstring clonedCallsign = std::wstring(getCallsign());
		std::wstring clonedName = std::wstring(getName());
		std::shared_ptr<ConstructDataMessage> clonedMessage = std::make_shared<ConstructDataMessage>(getMUSICHeader()->getExerciseID(), clonedConstructID, clonedCallsign, clonedName);

		// check whether following fields are null and copy them accordingly.
		clonedMessage->setGhostedID(getGhostedID() == nullptr ? nullptr : std::make_shared<EntityIDRecord>(getGhostedID()->getSiteID(), getGhostedID()->getAppID(), getGhostedID()->getEntityID()));
		clonedMessage->setPrimaryControllerID(getPrimaryControllerID() == nullptr ? nullptr : std::make_shared<EntityIDRecord>(getPrimaryControllerID()->getSiteID(), getPrimaryControllerID()->getAppID(), getPrimaryControllerID()->getEntityID()));
		clonedMessage->setCurrentControllerID(getCurrentControllerID() == nullptr ? nullptr : std::make_shared<EntityIDRecord>(getCurrentControllerID()->getSiteID(), getCurrentControllerID()->getAppID(), getCurrentControllerID()->getEntityID()));
		clonedMessage->setConstructRender(!getConstructRender() ? std::nullopt : getConstructRender());
		clonedMessage->setConstructType(!getConstructType() ? std::nullopt : getConstructType());
		clonedMessage->setInteractions(getInteractions() == nullptr ? nullptr : std::make_shared<InteractionRecord>(InteractionRecord((getInteractions()->getAvailableInteractions()))));
		clonedMessage->setPhysical(getPhysical() == nullptr ? nullptr : std::make_shared<PhysicalRecord>(static_cast<Force>((*getPhysical()->getForceIDField())), 
			getPhysical()->getEntityType(), 
			getPhysical()->getLocation(), 
			getPhysical()->getOrientation(), 
			getPhysical()->getLinearVelocity(), 
			getPhysical()->getDamage(), 
			getPhysical()->getDeadReckoningParameters() == nullptr ? nullptr : getPhysical()->getDeadReckoningParameters()));

		return clonedMessage;
	}

	std::shared_ptr<object> ConstructDataMessage::ToJsonObject()
	{
		object objVal = object();

		objVal[StringHelper::wstringToString(HEADER)] = *(getMUSICHeader()->ToJsonObject());
		objVal[StringHelper::wstringToString(ORIGIN_ID)] = *(getOriginID()->ToJsonObject());

		if ((getGhostedID()) != nullptr)
		{
			objVal[StringHelper::wstringToString(GHOSTED_ID)] = *(getGhostedID()->ToJsonObject());
		}

		if (getPrimaryControllerID() != nullptr)
		{
			objVal[StringHelper::wstringToString(PRIMARY_CONTROLLER_ID)] = *(getPrimaryControllerID()->ToJsonObject());
		}

		if (getCurrentControllerID() != nullptr)
		{
			objVal[StringHelper::wstringToString(CURRENT_CONTROLLER_ID)] = *(getCurrentControllerID()->ToJsonObject());
		}

		if (getConstructRender() || getConstructType())
		{
			std::shared_ptr<object> constructInformationRecord = std::make_shared<object>();
			object recObj = *constructInformationRecord;

			recObj[StringHelper::wstringToString(CONSTRUCT_RENDER)] = (int)*getConstructRender();
			recObj[StringHelper::wstringToString(CONSTRUCT_TYPE)] = (int)*getConstructType();
			objVal[StringHelper::wstringToString(CONSTRUCT_INFO_RECORD)] = recObj;
		}

		if (getPhysical() != nullptr)
		{
			if (getPhysical()->getForceIDField().has_value())
			{
				objVal[StringHelper::wstringToString(FORCE)] = (int)(*getPhysical()->getForceIDField());

			}
			if (getPhysical()->getEntityType() != nullptr)
			{
				objVal[StringHelper::wstringToString(ENTITY_TYPE)] = *(getPhysical()->getEntityType()->ToJsonObject());
			}
			if (getPhysical()->getLocation() != nullptr)
			{
				objVal[StringHelper::wstringToString(LOCATION)] = *(getPhysical()->getLocation()->ToJsonObject());
			}
			if (getPhysical()->getOrientation() != nullptr)
			{
				objVal[StringHelper::wstringToString(ORIENTATION)] = *(getPhysical()->getOrientation()->ToOrientationObject());
			}
			if (getPhysical()->getLinearVelocity() != nullptr)
			{
				objVal[StringHelper::wstringToString(VELOCITY)] = *(getPhysical()->getLinearVelocity()->ToJsonObject());
			}
			if (getPhysical()->getDeadReckoningParameters() != nullptr)
			{
				objVal[StringHelper::wstringToString(DEAD_RECK)] = *(getPhysical()->getDeadReckoningParameters()->ToJsonObject());
			}
			if (getPhysical()->getDamage() != nullptr)
			{
				objVal[StringHelper::wstringToString(DAMAGE_RECORD)] = *(getPhysical()->getDamage()->ToJsonObject());
			}
		}

		objVal[StringHelper::wstringToString(CALLSIGN)] = StringHelper::wstringToString(getCallsign());
		objVal[StringHelper::wstringToString(CONSTRUCT_NAME)] = StringHelper::wstringToString(getName());

		if (getInteractions() != nullptr)
		{
			objVal[StringHelper::wstringToString(INTERACTION_RECORD)] = StringHelper::wstringToString(getInteractions()->ToCsvString());
		}

		return std::make_shared<object>(objVal);
	}

	bool ConstructDataMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ConstructDataMessage>))
		{
			std::shared_ptr<ConstructDataMessage> other = std::any_cast<std::shared_ptr<ConstructDataMessage>>(obj);

			std::shared_ptr<MUSICMessage> parent = std::make_shared<MUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID());

			if (MUSICMessage::Equals(parent)) 
			{
				if (getCallsign() == other->getCallsign() &&
					getName() == other->getName() &&
					getConstructRender() == other->getConstructRender() &&
					getConstructType() == other->getConstructType() &&
					getInteractions()->Equals(other->getInteractions()))
				{
					if (getGhostedID() != nullptr && other->getGhostedID() != nullptr)
					{
						if (!getGhostedID()->Equals(other->getGhostedID())) 
						{
							return false;
						}
					}
					if ((getGhostedID() == nullptr || other->getGhostedID() == nullptr) && (getGhostedID() != nullptr || other->getGhostedID() != nullptr))
					{
						return false;
					}
					if (getPhysical() != nullptr && other->getPhysical() != nullptr)
					{
						if (!getPhysical()->Equals(other->getPhysical()))
						{
							return false;
						}
					}
					if ((getPhysical() == nullptr || other->getPhysical() == nullptr) && (getPhysical() != nullptr || other->getPhysical() != nullptr))
					{
						return false;
					}

					if (getPrimaryControllerID() != nullptr && other->getPrimaryControllerID() != nullptr)
					{
						if (!getPrimaryControllerID()->Equals(other->getPrimaryControllerID()))
						{
							return false;
						}
					}
					if ((getPrimaryControllerID() == nullptr || other->getPrimaryControllerID() == nullptr) && (getPrimaryControllerID() != nullptr || other->getPrimaryControllerID() != nullptr))
					{
						return false;
					}
					if (getCurrentControllerID() != nullptr && other->getCurrentControllerID() != nullptr)
					{
						if (!getCurrentControllerID()->Equals(other->getCurrentControllerID()))
						{
							return false;
						}
					}

					if ((getCurrentControllerID() == nullptr || other->getCurrentControllerID() == nullptr) && (getCurrentControllerID() != nullptr || other->getCurrentControllerID() != nullptr))
					{
						return false;
					}
				}
				else
				{
					return false;
				}
				return true;
			}
		}

		return false;
	}

	int ConstructDataMessage::GetHashCode() const
	{
		auto hashCode = -1980379703;
		hashCode = hashCode * -1521134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((!getCallsign().empty()) ? std::hash<std::wstring>{}(getCallsign()) : 0);
		hashCode = hashCode * -1521134295 + ((!getName().empty()) ? std::hash<std::wstring>{}(getName()) : 0);
		hashCode = hashCode * -1521134295 + ((getGhostedID() != NULL) ? getGhostedID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getPrimaryControllerID() != nullptr) ? getPrimaryControllerID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getCurrentControllerID() != nullptr) ? getCurrentControllerID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + (getConstructRender().has_value() ? std::hash<std::optional<MUSICLibrary::MUSIC_Messages::ConstructRender>>{}(getConstructRender()) : 0);
		hashCode = hashCode * -1521134295 + (getConstructType().has_value() ? std::hash<std::optional<MUSICLibrary::MUSIC_Messages::ConstructType>>{}(getConstructType()) : 0);
		hashCode = hashCode * -1521134295 + ((getInteractions() != nullptr) ? getInteractions()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getPhysical() != nullptr) ? getPhysical()->GetHashCode() : 0);
		return hashCode;
	}

	size_t ConstructDataMessage::HashFunction::operator()(const ConstructDataMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ConstructDataMessage::operator == (const ConstructDataMessage &message2)
	{
		return this->Equals(std::make_shared<ConstructDataMessage>(message2));
	}

	bool ConstructDataMessage::operator != (const ConstructDataMessage &message2)
	{
		return !(*this == message2);
	}
}
