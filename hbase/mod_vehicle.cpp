#include"pch.h"
#include"mod_vehicle.h"
#include"script.h"
#include"util.h"
#include "pointers.h"
#include"natives.h"
#include"ini.h"
#include "pugixml.hpp"
#include"Types.h"
#include"features.h"

struct modEntity
{
	Entity e;
	int type;
	Hash hash;
	Entity initialHandle;
	bool isAttached;
	Entity attachTo;
	Vector3 AttachmentArgs;
	Vector3 AttachmentArgsRotation;
	int BoneIndex;
};
namespace hbase
{
	std::vector<Vehicle>last_vehicle;
	std::vector<Object>last_object;
	std::vector<Ped>last_ped;
	void deleteLastEntity()
	{
		if (g_settings.options["delete_last_vehicle"]) {
			for (Vehicle vehicle : last_vehicle)
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, true, true);
				VEHICLE::DELETE_VEHICLE(&vehicle);
			}
			for (Object object : last_object)
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(object, true, true);
				OBJECT::DELETE_OBJECT(&object);
			}
			for (Ped ped : last_ped)
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ped, true, true);
				PED::DELETE_PED(&ped);
			}
			//g_gui.SendNotification("INI载具", StringToChar("已经删除" + std::to_string(last_vehicle.size() + last_object.size() + last_ped.size()) + "个模型"));
			last_vehicle.clear();
			last_object.clear();
			last_ped.clear();
		}
	}

	void modvehicle::create_xml_vehicle(const char* xml)
	{
		deleteLastEntity();
		pugi::xml_document doc;
		if (doc.load_file(xml).status != pugi::status_ok)
		{
			//g_gui.SendNotification("XML载具", "未找到指定XML载具");
		}
		auto& nodeVehicle = doc.child("Vehicle"); // Root
		Hash VehicleModel = nodeVehicle.child("ModelHash").text().as_uint();
		//加载模型
		if (STREAMING::IS_MODEL_VALID(VehicleModel))
		{
			STREAMING::REQUEST_MODEL(VehicleModel);
			while (!STREAMING::HAS_MODEL_LOADED(VehicleModel))	script::get_current()->yield();
		}
		else
		{
			//g_gui.SendNotification("XML载具", StringToChar("车辆:" + std::to_string(VehicleModel) + "模型无效"));
			return;
		}
		std::vector<modEntity> entityList;
		modEntity vehicle;
		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
		vehicle.e = features::spawn_vehicle(VehicleModel);
		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
		last_vehicle.push_back(vehicle.e);
		entityList.push_back(vehicle);
		entityList[0].initialHandle = nodeVehicle.child("InitialHandle").text().as_int();
		entityList[0].isAttached = false;
		//设置引力

		auto& VehicleProperties = nodeVehicle.child("VehicleProperties");

		//设置Livery
		VEHICLE::SET_VEHICLE_LIVERY(vehicle.e, VehicleProperties.child("Livery").text().as_int());

		//设置颜色
		auto& nodeVehicleColours = VehicleProperties.child("Colours");
		int mod1a = nodeVehicleColours.child("Mod1_a").text().as_int();
		int mod1b = nodeVehicleColours.child("Mod1_b").text().as_int();
		int mod1c = nodeVehicleColours.child("Mod1_c").text().as_int();
		VEHICLE::SET_VEHICLE_MOD_COLOR_1(vehicle.e, mod1a, mod1b, mod1c);

		int mod2a = nodeVehicleColours.child("Mod2_a").text().as_int();
		int mod2b = nodeVehicleColours.child("Mod2_b").text().as_int();
		VEHICLE::SET_VEHICLE_MOD_COLOR_2(vehicle.e, mod2a, mod2b);

		int Primary = nodeVehicleColours.child("Primary").text().as_int();
		int Secondary = nodeVehicleColours.child("Secondary").text().as_int();
		VEHICLE::SET_VEHICLE_COLOURS(vehicle.e, Primary, Secondary);

		int Pearl = nodeVehicleColours.child("Pearl").text().as_int();
		int Rim = nodeVehicleColours.child("Rim").text().as_int();
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle.e, Pearl, Rim);

		bool isPrimaryColourCustom = nodeVehicleColours.child("IsPrimaryColourCustom").text().as_bool();
		bool isSecondaryColourCustom = nodeVehicleColours.child("IsSecondaryColourCustom").text().as_bool();
		if (isPrimaryColourCustom)
		{
			RGB cust1;
			cust1.r = nodeVehicleColours.child("Cust1_R").text().as_int();
			cust1.g = nodeVehicleColours.child("Cust1_G").text().as_int();
			cust1.b = nodeVehicleColours.child("Cust1_B").text().as_int();
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle.e, cust1.r, cust1.g, cust1.b);
		}
		if (isSecondaryColourCustom)
		{
			RGB cust2;
			cust2.r = nodeVehicleColours.child("Cust2_R").text().as_int();
			cust2.g = nodeVehicleColours.child("Cust2_G").text().as_int();
			cust2.b = nodeVehicleColours.child("Cust2_B").text().as_int();
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle.e, cust2.r, cust2.g, cust2.b);
		}

		RGB tyreSmokeRgb;
		tyreSmokeRgb.r = nodeVehicleColours.child("tyreSmoke_R").text().as_int();
		tyreSmokeRgb.g = nodeVehicleColours.child("tyreSmoke_G").text().as_int();
		tyreSmokeRgb.b = nodeVehicleColours.child("tyreSmoke_B").text().as_int();
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle.e, tyreSmokeRgb.r, tyreSmokeRgb.g, tyreSmokeRgb.b);

		VEHICLE::_SET_VEHICLE_INTERIOR_COLOR(vehicle.e, nodeVehicleColours.child("LrInterior").text().as_int());
		VEHICLE::_SET_VEHICLE_DASHBOARD_COLOR(vehicle.e, nodeVehicleColours.child("LrDashboard").text().as_int());
		VEHICLE::_SET_VEHICLE_XENON_LIGHTS_COLOR(vehicle.e, nodeVehicleColours.child("LrXenonHeadlights").text().as_int());

		//透明度
		int opacityLevel = nodeVehicle.child("OpacityLevel").text().as_int();
		if (opacityLevel < 255) ENTITY::SET_ENTITY_ALPHA(vehicle.e, opacityLevel, 0);
		ENTITY::SET_ENTITY_VISIBLE(vehicle.e, nodeVehicle.child("IsVisible").text().as_bool(), false);
		//着火
		nodeVehicle.child("IsOnFire").text().as_bool() ? FIRE::START_ENTITY_FIRE(vehicle.e) : FIRE::STOP_ENTITY_FIRE(vehicle.e);
		//无敌
		ENTITY::SET_ENTITY_INVINCIBLE(vehicle.e, nodeVehicle.child("IsInvincible").text().as_bool());
		//附加
		auto& nodeAttachments = nodeVehicle.child("SpoonerAttachments");
		for (auto& nodeAttachment = nodeAttachments.first_child(); nodeAttachment; nodeAttachment = nodeAttachment.next_sibling())
		{
			modEntity e;
			Hash eModel = nodeAttachment.child("ModelHash").text().as_uint();

			int Type = nodeAttachment.child("Type").text().as_int();
			bool Dynamic = nodeAttachment.child("Dynamic").text().as_bool();
			e.initialHandle = nodeAttachment.child("InitialHandle").text().as_int();

			//坐标和旋转
			auto& nodeEntityPosRot = nodeAttachment.child("PositionRotation");
			Vector3 placingEpos;
			placingEpos.x = nodeEntityPosRot.child("X").text().as_float();
			placingEpos.y = nodeEntityPosRot.child("Y").text().as_float();
			placingEpos.z = nodeEntityPosRot.child("Z").text().as_float();
			Vector3 placingErot;
			placingErot.x = nodeEntityPosRot.child("Pitch").text().as_float();
			placingErot.y = nodeEntityPosRot.child("Roll").text().as_float();
			placingErot.z = nodeEntityPosRot.child("Yaw").text().as_float();

			//加载模型
			if (STREAMING::IS_MODEL_VALID(eModel))
			{
				STREAMING::REQUEST_MODEL(eModel);
				while (!STREAMING::HAS_MODEL_LOADED(eModel))	script::get_current()->yield();
			}
			else
			{
				//g_gui.SendNotification("XML载具", StringToChar(std::to_string(eModel) + "模型无效"));
				for (int i = 0; i < entityList.size(); i++)
				{
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(entityList[i].hash);
				}
				return;
			}

			//创建模型
			if (Type == 3)
			{
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
				e.e = OBJECT::CREATE_OBJECT(eModel, placingEpos.x, placingEpos.y, placingEpos.z, true, true, false);
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
			}
			else if (Type == 1)
			{
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
				e.e = PED::CREATE_PED(26, eModel, placingEpos.x, placingEpos.y, placingEpos.z, placingErot.z, true, true);
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
				last_ped.push_back(e.e);
			}
			else if (Type == 2)
			{
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
				e.e = VEHICLE::CREATE_VEHICLE(eModel, placingEpos.x, placingEpos.y, placingEpos.z, placingErot.z, 1, 1, false);
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
				auto& nodeVehicleStuff = nodeAttachments.child("VehicleProperties");

				//颜色
				VEHICLE::SET_VEHICLE_LIVERY(e.e, nodeVehicleStuff.child("Livery").text().as_int());

				auto& nodeVehicleColours = nodeVehicleStuff.child("Colours");
				int mod1a = nodeVehicleColours.child("Mod1_a").text().as_int();
				int mod1b = nodeVehicleColours.child("Mod1_b").text().as_int();
				int mod1c = nodeVehicleColours.child("Mod1_c").text().as_int();
				VEHICLE::SET_VEHICLE_MOD_COLOR_1(e.e, mod1a, mod1b, mod1c);
				int mod2a = nodeVehicleColours.child("Mod2_a").text().as_int();
				int mod2b = nodeVehicleColours.child("Mod2_b").text().as_int();
				VEHICLE::SET_VEHICLE_MOD_COLOR_2(e.e, mod2a, mod2b);

				int Primary = nodeVehicleColours.child("Primary").text().as_int();
				int Secondary = nodeVehicleColours.child("Secondary").text().as_int();
				VEHICLE::SET_VEHICLE_COLOURS(e.e, Primary, Secondary);

				int Pearl = nodeVehicleColours.child("Pearl").text().as_int();
				int Rim = nodeVehicleColours.child("Rim").text().as_int();
				VEHICLE::SET_VEHICLE_EXTRA_COLOURS(e.e, Pearl, Rim);
				bool isPrimaryColourCustom = nodeVehicleColours.child("IsPrimaryColourCustom").text().as_bool();
				bool isSecondaryColourCustom = nodeVehicleColours.child("IsSecondaryColourCustom").text().as_bool();
				if (isPrimaryColourCustom)
				{
					RGB cust1;
					cust1.r = nodeVehicleColours.child("Cust1_R").text().as_int();
					cust1.g = nodeVehicleColours.child("Cust1_G").text().as_int();
					cust1.b = nodeVehicleColours.child("Cust1_B").text().as_int();
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(e.e, cust1.r, cust1.g, cust1.b);
				}
				if (isSecondaryColourCustom)
				{
					RGB cust2;
					cust2.r = nodeVehicleColours.child("Cust2_R").text().as_int();
					cust2.g = nodeVehicleColours.child("Cust2_G").text().as_int();
					cust2.b = nodeVehicleColours.child("Cust2_B").text().as_int();
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(e.e, cust2.r, cust2.g, cust2.b);
				}
				RGB tyreSmokeRgb;
				tyreSmokeRgb.r = nodeVehicleColours.child("tyreSmoke_R").text().as_int();
				tyreSmokeRgb.g = nodeVehicleColours.child("tyreSmoke_G").text().as_int();
				tyreSmokeRgb.b = nodeVehicleColours.child("tyreSmoke_B").text().as_int();
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(e.e, tyreSmokeRgb.r, tyreSmokeRgb.g, tyreSmokeRgb.b);
			}
			last_object.push_back(e.e);

			ENTITY::FREEZE_ENTITY_POSITION(e.e, nodeAttachment.child("FrozenPos").text().as_bool(!Dynamic));

			ENTITY::SET_ENTITY_HAS_GRAVITY(e.e, nodeAttachments.child("HasGravity").text().as_bool(true));

			//着火
			nodeAttachment.child("IsOnFire").text().as_bool() ? FIRE::START_ENTITY_FIRE(e.e) : FIRE::STOP_ENTITY_FIRE(e.e);
			//无敌
			ENTITY::SET_ENTITY_INVINCIBLE(e.e, nodeAttachment.child("IsInvincible").text().as_bool());
			//碰撞
			ENTITY::SET_ENTITY_COLLISION(e.e, true, false);
			//仅被玩家损坏
			ENTITY::SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(e.e, nodeAttachment.child("IsOnlyDamagedByPlayer").text().as_bool());
			//可见
			ENTITY::SET_ENTITY_VISIBLE(e.e, nodeAttachment.child("IsVisible").text().as_bool(), false);

			//附加信息
			auto& nodeEntityAttachment = nodeAttachment.child("Attachment");
			e.isAttached = nodeEntityAttachment.attribute("isAttached").as_bool();
			if (e.isAttached)
			{
				std::string attachedToHandleStr = nodeEntityAttachment.child("AttachedTo").text().as_string();
				if (attachedToHandleStr == "PLAYER")
				{
					e.attachTo = PLAYER::PLAYER_PED_ID();
				}
				else if (attachedToHandleStr == "VEHICLE")
				{
					if (ENTITY::DOES_ENTITY_EXIST(vehicle.e)) e.attachTo = vehicle.e;
					else e.isAttached = false;
				}
				else
				{
					e.attachTo = nodeEntityAttachment.child("AttachedTo").text().as_int();
				}
				e.BoneIndex = nodeEntityAttachment.child("BoneIndex").text().as_int();

				e.AttachmentArgs.x = nodeEntityAttachment.child("X").text().as_float();
				e.AttachmentArgs.y = nodeEntityAttachment.child("Y").text().as_float();
				e.AttachmentArgs.z = nodeEntityAttachment.child("Z").text().as_float();

				e.AttachmentArgsRotation.x = nodeEntityAttachment.child("Pitch").text().as_float();
				e.AttachmentArgsRotation.y = nodeEntityAttachment.child("Roll").text().as_float();
				e.AttachmentArgsRotation.z = nodeEntityAttachment.child("Yaw").text().as_float();
			}
			entityList.push_back(e);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(eModel);
		}

		for (modEntity& me : entityList)
		{
			if (me.isAttached)
			{
				util::request_control_of_ent(me.e);
				bool bHasCollision = !ENTITY::GET_ENTITY_COLLISION_DISABLED(me.e);
				if (me.attachTo == PLAYER::PLAYER_PED_ID())
				{
					ENTITY::ATTACH_ENTITY_TO_ENTITY(me.e, PLAYER::PLAYER_PED_ID(), me.BoneIndex, me.AttachmentArgs.x, me.AttachmentArgs.y, me.AttachmentArgs.z, me.AttachmentArgsRotation.x, me.AttachmentArgsRotation.y, me.AttachmentArgsRotation.z, FALSE, FALSE, bHasCollision, FALSE, 2, TRUE);
				}
				else if (me.attachTo == PLAYER::PLAYER_PED_ID())
				{
				}
				else
				{
					for (modEntity& me2 : entityList)
					{
						if (me.attachTo == me2.initialHandle)
						{
							ENTITY::ATTACH_ENTITY_TO_ENTITY(me.e, me2.e, me.BoneIndex, me.AttachmentArgs.x, me.AttachmentArgs.y, me.AttachmentArgs.z, me.AttachmentArgsRotation.x, me.AttachmentArgsRotation.y, me.AttachmentArgsRotation.z, FALSE, FALSE, bHasCollision, FALSE, 2, TRUE);
						}
					}
				}
			}
		}
	}

	bool modvehicle::create_ini_vehicle(const char* vehiclename)
	{
		deleteLastEntity();
		int allObjectsCount = INI::GetInt(vehiclename, "AllObjects", "Count");
		int allVehicleCount = INI::GetInt(vehiclename, "AllVehicles", "Count");
		int AllPedCount = INI::GetInt(vehiclename, "AllPeds", "Count");

		Vector3 ourCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);

		std::vector<modEntity>entityList;

		//loadVehicle
		for (int i = 0; i < allVehicleCount; i++)
		{
			modEntity temp;
			temp.hash = atoll(INI::GetString(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Hash").c_str());
			temp.type = 0;
			if (STREAMING::IS_MODEL_VALID(temp.hash))
			{
				STREAMING::REQUEST_MODEL(temp.hash);
				while (!STREAMING::HAS_MODEL_LOADED(temp.hash)) {
					script::get_current()->yield();
				}
				entityList.push_back(temp);
			}
			else
			{
				//g_gui.SendNotification("INI载具", StringToChar("车辆:" + std::to_string(temp.hash) + "模型无效"));
				for (int i = 0; i < entityList.size(); i++)
				{
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(entityList[i].hash);
				}
				return false;
			}
		}

		//loadObject
		for (int i = 0; i < allObjectsCount; i++)
		{
			modEntity temp{};
			temp.hash = atoll(INI::GetString(vehiclename, ("Object" + std::to_string(i)).c_str(), "Hash").c_str());
			temp.type = 1;
			if (STREAMING::IS_MODEL_VALID(temp.hash))
			{
				STREAMING::REQUEST_MODEL(temp.hash);
				while (!STREAMING::HAS_MODEL_LOADED(temp.hash)) {
					script::get_current()->yield();
				}
				entityList.push_back(temp);
			}
			else
			{
				//g_gui.SendNotification("INI载具", StringToChar("物体:" + std::to_string(temp.hash) + "模型无效"));
				for (int i = 0; i < entityList.size(); i++)
				{
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(entityList[i].hash);
				}
				return false;
			}
		}
		//创建车辆
		for (int i = 0; i < allVehicleCount && entityList[i].type == 0; i++)
		{
			entityList[i].BoneIndex = atoi(INI::GetString(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Bone").c_str());
			entityList[i].initialHandle = INI::GetInt(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "SelfNumeration");
			entityList[i].attachTo = INI::GetInt(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "AttachNumeration");
			entityList[i].AttachmentArgs.x = INI::GetFloat(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "OffsetX");
			entityList[i].AttachmentArgs.y = INI::GetFloat(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "OffsetY");
			entityList[i].AttachmentArgs.z = INI::GetFloat(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "OffsetZ");
			entityList[i].AttachmentArgsRotation.x = INI::GetFloat(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Pitch");
			entityList[i].AttachmentArgsRotation.y = INI::GetFloat(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Roll");
			entityList[i].AttachmentArgsRotation.z = INI::GetFloat(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Yaw");
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			last_vehicle.push_back(entityList[i].e = VEHICLE::CREATE_VEHICLE(entityList[i].hash, ourCoords.x , ourCoords.y, ourCoords.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 0, 1));
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
			util::request_control_of_ent(entityList[i].e);
			ENTITY::SET_ENTITY_INVINCIBLE(entityList[i].e, INI::GetBool(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Invincible"));
			ENTITY::SET_ENTITY_ALPHA(entityList[i].e, INI::GetInt(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Alpha"), 1);
			ENTITY::FREEZE_ENTITY_POSITION(entityList[i].e, INI::GetBool(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Freeze"));
			ENTITY::SET_ENTITY_VISIBLE(entityList[i].e, INI::GetBool(vehiclename, ("Vehicle" + std::to_string(i)).c_str(), "Visible"), 1);
			DECORATOR::DECOR_SET_INT(entityList[i].e, "MPBitset", 0);
			ENTITY::SET_ENTITY_INVINCIBLE(entityList[i].e, 1);
			VEHICLE::SET_VEHICLE_COLOURS(entityList[i].e, atoi(INI::GetString(vehiclename, ("Vehicle" + std::to_string(i) + "VehicleColors").c_str(), "Primary").c_str()), atoi(INI::GetString(vehiclename, ("Vehicle" + std::to_string(i) + "VehicleColors").c_str(), "Secondary").c_str()));
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(entityList[i].e, atoi(INI::GetString(vehiclename, ("Vehicle" + std::to_string(i) + "NeonColor").c_str(), "R").c_str()), atoi(INI::GetString(vehiclename, ("Vehicle" + std::to_string(i) + "NeonColor").c_str(), "G").c_str()), atoi(INI::GetString(vehiclename, ("Vehicle" + std::to_string(i) + "NeonColor").c_str(), "B").c_str()));
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(entityList[i].hash);
		}

		//创建物体
		for (int i = 0; i < allObjectsCount && entityList[i].type == 1; i++)
		{
			entityList[i].BoneIndex = INI::GetInt(vehiclename, ("Object" + std::to_string(i)).c_str(), "Bone");
			entityList[i].initialHandle = atoi(INI::GetString(vehiclename, ("Object" + std::to_string(i)).c_str(), "SelfNumeration").c_str());
			entityList[i].attachTo = atoi(INI::GetString(vehiclename, ("Object" + std::to_string(i)).c_str(), "AttachNumeration").c_str());
			entityList[i].AttachmentArgs.x = INI::GetFloat(vehiclename, ("Object" + std::to_string(i)).c_str(), "OffsetX");
			entityList[i].AttachmentArgs.y = INI::GetFloat(vehiclename, ("Object" + std::to_string(i)).c_str(), "OffsetY");
			entityList[i].AttachmentArgs.z = INI::GetFloat(vehiclename, ("Object" + std::to_string(i)).c_str(), "OffsetZ");
			entityList[i].AttachmentArgsRotation.x = INI::GetFloat(vehiclename, ("Object" + std::to_string(i)).c_str(), "Pitch");
			entityList[i].AttachmentArgsRotation.y = INI::GetFloat(vehiclename, ("Object" + std::to_string(i)).c_str(), "Roll");
			entityList[i].AttachmentArgsRotation.z = INI::GetFloat(vehiclename, ("Object" + std::to_string(i)).c_str(), "Yaw");
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			last_object.push_back(entityList[i].e = OBJECT::CREATE_OBJECT(entityList[i].hash, ourCoords.x , ourCoords.y , ourCoords.z, 1, 1, INI::GetBool(vehiclename, ("Object" + std::to_string(i)).c_str(), "Dynamic")));
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
			util::request_control_of_ent(entityList[i].e);
			ENTITY::SET_ENTITY_INVINCIBLE(entityList[i].e, INI::GetBool(vehiclename, ("Object" + std::to_string(i)).c_str(), "Invincible"));
			ENTITY::SET_ENTITY_ALPHA(entityList[i].e, INI::GetInt(vehiclename, ("Object" + std::to_string(i)).c_str(), "Alpha"), 1);
			ENTITY::FREEZE_ENTITY_POSITION(entityList[i].e, INI::GetBool(vehiclename, ("Object" + std::to_string(i)).c_str(), "Freeze"));
			ENTITY::SET_ENTITY_VISIBLE(entityList[i].e, INI::GetBool(vehiclename, ("Object" + std::to_string(i)).c_str(), "Visible"), 1);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(entityList[i].hash);
		}

		//附加
		for (modEntity& me : entityList)
		{
			for (modEntity& me2 : entityList)
			{
				if (me.initialHandle == me2.attachTo)
				{
					ENTITY::ATTACH_ENTITY_TO_ENTITY(me2.e, me.e, me2.BoneIndex, me2.AttachmentArgs.x, me2.AttachmentArgs.y, me2.AttachmentArgs.z, me2.AttachmentArgsRotation.x, me2.AttachmentArgsRotation.y, me2.AttachmentArgsRotation.z, 0, 1, 1, 0, 2, 1);
				}
			}
		}
		return true;
	}
}