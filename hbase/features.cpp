#include"features.h"
#include "script_mgr.h"
#include"settings.h"
#include"gta/enums.h"
#include"gta_util.hpp"
#include"offset.h"
#include"world_memory.h"
#include"util.h"
#include"Types.h"
#include "keyboard.h"
#include"data/vehicles.h"
#include"data/player.h"
#include"gta/blip.h"

namespace hbase
{
	rage::CBaseModelInfo* get_model_info_ptr(uint32_t hash) {
		std::int32_t index = 0xFF;
		return 	g_pointers->m_get_model_info(hash, &index);
	}

	Vehicle features::spawn_vehicle(Hash hash, Entity on_entity) {
		if (STREAMING::IS_MODEL_VALID(hash)) {
			STREAMING::REQUEST_MODEL(hash);
			while (!STREAMING::HAS_MODEL_LOADED(hash)) 			script::get_current()->yield();
			Vector3 l_coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(on_entity, 0.0f, 0.0f, 0.0f);
			int net_id = NETWORK::VEH_TO_NET(VEHICLE::CREATE_VEHICLE(hash, l_coords.x, l_coords.y, l_coords.z, ENTITY::GET_ENTITY_HEADING(on_entity), TRUE, FALSE,false));
			NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(net_id, TRUE);
			if (ENTITY::IS_ENTITY_VISIBLE_TO_SCRIPT(NETWORK::NET_TO_VEH(net_id))) {
				DECORATOR::DECOR_SET_INT(NETWORK::NET_TO_VEH(net_id), "MPBitset", (1 << 10));
				return NETWORK::NET_TO_VEH(net_id);
			}
		}
		return NULL;
	}

	void features::spawn_crash_vehicle(Ped ped, char* vehicle, int id1, int id2) {
		Hash hash = MISC::GET_HASH_KEY(vehicle);
		rage::CBaseModelInfo* model_info = get_model_info_ptr(hash);
		if (model_info != NULL) {
			model_info->m_handlingType  = id1;
			Vehicle vehicle = spawn_vehicle(hash, ped);
			if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
				ENTITY::SET_ENTITY_VISIBLE(vehicle, FALSE,false);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle,false);
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, 100.0f);
				script::get_current()->yield(1s);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, 0, 1);
				VEHICLE::DELETE_VEHICLE(&vehicle);
			}
			model_info->m_handlingType = id2;
		}
	}

	void features::runTick()
	{
		//self
		memory::set_value<bool>({ OFFSET_PLAYER, OFFSET_ENTITY_GOD }, g_settings.options["god_mode"].get<bool>());
		if (g_settings.options["never_wanted"].get<bool>()) set_player_wanted_level(0);
		set_player_run_speed(run_speed);
		set_player_swim_speed(swim_speed);
		set_player_invisible(g_settings.options["invisible"].get<bool>());
		if (g_settings.options["super_jump"].get<bool>())write_flame_flag((uint32_t)eFrameFlags::eFrameFlagSuperJump);
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), g_settings.options["unlimited_ammo"].get<bool>());
		if (g_settings.options["rapid_fire"].get<bool>())player_rapid_fire();
		if (g_settings.options["explosive_ammo"].get<bool>()) write_flame_flag((uint32_t)eFrameFlags::eFrameFlagExplosiveAmmo);

		memory::set_value<bool>({ OFFSET_PLAYER, OFFSET_PLAYER_VEHICLE,OFFSET_ENTITY_GOD }, g_settings.options["vehicle_invincible"].get<bool>());
		if (g_settings.options["rainbow_vehicle"].get<bool>())set_rain_bow_vehicle();

		if (g_settings.options["safety_belt"].get<bool>())safety_belt();

		if (g_settings.options["infinite_nitrogen"].get<bool>())infinite_nitrogen();
		if (g_settings.options["9_key_speed_up"].get<bool>())key_9_speed_up();

		spectator(g_settings.spectating, g_players.player_id);

		water_loop(g_settings.f_waterloop, g_players.player_id);
		fire_loop(g_settings.f_fireloop, g_players.player_id);
	}
	void features::spectator(bool b, int player_id)
	{
		Ped target = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player_id);
		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(b, target);
		HUD::SET_MINIMAP_IN_SPECTATOR_MODE(b, target);
	}
	void features::cage_player(Ped ped)
	{
		Vector3 remotePos = ENTITY::GET_ENTITY_COORDS(ped, 0);
		OBJECT::CREATE_OBJECT(959275690, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
	}
	void features::ram_player()
	{
		Hash model = MISC::GET_HASH_KEY("INSURGENT");

		float offset;
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			script::get_current()->yield();
		}

		Vector3 pCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id), 0.0, -10.0, 0.0);
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
		{
			Vector3 dim1, dim2;
			MISC::GET_MODEL_DIMENSIONS(model, &dim1, &dim2);

			offset = dim2.y * 1.6f;

			Vector3 dir = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id));
			float rot = (ENTITY::GET_ENTITY_ROTATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id), 0)).z;
			pCoords.x += (dir.x * offset);
			pCoords.y += (dir.y * offset);
			Vehicle veh = VEHICLE::CREATE_VEHICLE(model, pCoords.x, pCoords.y, pCoords.z, rot, true, false, false);

			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
			ENTITY::SET_ENTITY_VISIBLE(veh, true, 0);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 1000.0);
		}
	}
	void features::air_strike_player(Player player) {
		Vector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1);
		StrikeCoords.z += 15;
		Hash hash = MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET");
		WEAPON::REQUEST_WEAPON_ASSET(hash, 31, false);
		while (!WEAPON::HAS_WEAPON_ASSET_LOADED(hash))
		{
			script::get_current()->yield();
		}

		MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 35, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1, 1, 500);
		MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 30, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
		MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 55, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, 0xF8A3939F, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), true, true, 600);
	}
	void features::clone_player(Player player) {
		PED::CLONE_PED(player, 1, 1, 1);
	}
	void features::water_loop(bool enable, Player player) {
		if (enable) {
			Player waterloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
			Vector3 Pos = ENTITY::GET_ENTITY_COORDS(waterloopedplayer, true);
			FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z - 2, 13, 5, true, 0, 0, 0);
		}
	}
	void features::fire_loop(bool enable, Player player) {
		if (enable) {
			Player fireloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
			Vector3 Pos = ENTITY::GET_ENTITY_COORDS(fireloopedplayer, true);
			FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z - 2, 12, 5, true, 0, 0, 0);
		}
	}
	void features::copy_outfit(Player player)
	{
		for (int i = 0; i < 12; i++)
		{
			PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i), PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i), PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i));
			for (int i = 0; i < 2; i++) {
				script::get_current()->yield();
			}
		}
	}
	void features::give_all_weapons()
	{
		script::get_current()->yield();
		Hash Weapons[] = { 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, 0x476BF155, 0xAF3696A1, 0xB62D1F67 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			if (!WEAPON::HAS_PED_GOT_WEAPON(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id), Weapons[i], false))
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
		}
	}
	void features::give_all_mk2() {
		script::get_current()->yield();
		Hash Weapons[] = { 3219281620 ,2024373456 ,961495388 ,4208062921 ,3686625920 ,177293209 , };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id), Weapons[i], 9999, 1);
		}
	}
	void features::give_stun_gun() {
		int Weapons[] = { 0x3656C8C1,911657153 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id), Weapons[i], 9999, 1);
		}
	}
	void features::give_digiscanner() {
		Hash Weapons[] = { 0xFDBADCED,4256881901 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id), Weapons[i], 9999, 1);
		}
	}
	void features::de_remove_ped_crash()
	{
		g_settings.m_de_remove_crash = true;

		Hash hash = 1830688247;
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			script::get_current()->yield();
		}

		for (size_t i = 0; i < 3000; i++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id);
			if (!ENTITY::DOES_ENTITY_EXIST(ped))
			{
				i = 3000;
				break;
			}
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false);
			float heading = ENTITY::GET_ENTITY_HEADING(ped);

			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			Ped crash_ped = PED::CREATE_PED(26, hash, pos.x, pos.y, pos.z, heading, *g_pointers->m_is_session_started, true);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

			script::get_current()->yield(10ms);
			PED::DELETE_PED(&crash_ped);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		LOG(INFO) << "OK";
		g_settings.m_de_remove_crash = false;
	}
	void features::de_remove_veh_crash()
	{
		g_settings.m_de_remove_crash = true;

		Hash hash = 0xD6BC7523;
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			script::get_current()->yield();
		}

		for (size_t i = 0; i < 3000; i++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id);
			if (!ENTITY::DOES_ENTITY_EXIST(ped))
			{
				i = 3000;
				break;
			}
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false);
			float heading = ENTITY::GET_ENTITY_HEADING(ped);

			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			Vehicle crash_veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, heading, *g_pointers->m_is_session_started, true, false);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

			script::get_current()->yield();

			VEHICLE::DELETE_VEHICLE(&crash_veh);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		LOG(INFO) << "OK";
		g_settings.m_de_remove_crash = false;
	}
	void features::de_remove_veh_crash1()
	{
		g_settings.m_de_remove_crash = true;

		Hash hash = -42959138;
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			script::get_current()->yield();
		}

		for (size_t i = 0; i < 3000; i++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id);
			if (!ENTITY::DOES_ENTITY_EXIST(ped))
			{
				i = 3000;
				break;
			}
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false);
			float heading = ENTITY::GET_ENTITY_HEADING(ped);

			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			Vehicle crash_veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, heading, *g_pointers->m_is_session_started, true, false);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

			script::get_current()->yield();

			VEHICLE::DELETE_VEHICLE(&crash_veh);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		LOG(INFO) << "OK";
		g_settings.m_de_remove_crash = false;
	}
	void features::infinite_nitrogen()
	{
		memory::set_value<float>({ OFFSET_PLAYER,OFFSET_PLAYER_VEHICLE,OFFSET_VEHICLE_BOOST }, 5);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			if (KeyPressed(VK_SPACE))
			{
				memory::set_value<float>({ OFFSET_PLAYER,OFFSET_PLAYER_VEHICLE,OFFSET_VEHICLE_BOOST }, 0);
			}
		}
	}
	void features::teleport_to_coords(Entity e, Vector3 coords)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
	}
	void features::tp_to_vector3(Vector3 coords)
	{
		Entity e = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
		{
			e = PED::GET_VEHICLE_PED_IS_USING(e);
		}
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
	}
	void features::teleport_to_objective()
	{
		rage::Blip_t* blip = util::get_blip_objective();
		if (blip == nullptr)
		{
			//g_gui.SendNotification("传送", "没有找到任务点");
		}
		else
		{
			Entity e = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID());
			if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
			{
				e = PED::GET_VEHICLE_PED_IS_USING(e);
			}
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, blip->m_x, blip->m_y, blip->m_z + 0.5f, 0, 0, 1);
		}
	}
	void features::tp_to_waypoint()
	{
		Entity ent = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_IN_ANY_VEHICLE(ent, FALSE))
			ent = PED::GET_VEHICLE_PED_IS_IN(ent, FALSE);
		Vector3 coords = util::get_blip_marker();

		if (coords.x != 0 || coords.y != 0)
		{
			bool groundFound = false;
			static float groundCheckHeight[] =
			{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
			for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
				script::get_current()->yield(75ms);
				if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0, 0))
				{
					groundFound = true;
					coords.z += 3.0;
					break;
				}
			}
			// if ground not found then set Z in air and give player a parachute
			if (!groundFound)
			{
				coords.z = 1000.0;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
			}
			//do it
			teleport_to_coords(ent, coords);
			HUD::SET_WAYPOINT_OFF();
		}
	}
	void features::player_rapid_fire()
	{
		Player playerPed = PLAYER::PLAYER_PED_ID();
		if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) {
			PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_ID(), 1);
			Vector3 gameplayCam = CAM::GET_FINAL_RENDERED_CAM_COORD();
			Vector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			Vector3 gameplayCamDirection = util::rotation_to_direction(gameplayCamRot);
			Vector3 startCoords = util::add_vector(gameplayCam, (util::multiply_vector(gameplayCamDirection, 1.0f)));
			Vector3 endCoords = util::add_vector(startCoords, util::multiply_vector(gameplayCamDirection, 500.0f));
			Hash weaponhash;
			WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, 1);
			if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000)) {
				PED::SET_PED_SHOOTS_AT_COORD(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX()), endCoords.x, endCoords.y, endCoords.z, true);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, startCoords.z, endCoords.x, endCoords.y, endCoords.z, 50, 1, weaponhash, playerPed, 1, 1, 0xbf800000);
			}
		}
	}
	void features::set_player_wanted_level(int i)
	{
		gta_util::get_local_player_info()->m_wanted_level = i;
	}
	float features::run_speed = 1.0f;
	void features::set_player_run_speed(float f)
	{
		gta_util::get_local_player_info()->m_run_speed = f;
	}
	float features::swim_speed = 1.0f;
	void features::set_player_swim_speed(float f)
	{
		gta_util::get_local_player_info()->m_swim_speed = f;
	}
	void features::set_player_invisible(bool b)
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), !b, false);
		NETWORK::_NETWORK_SET_ENTITY_INVISIBLE_TO_NETWORK(PLAYER::PLAYER_PED_ID(), b);
	}
	void features::chage_player_model(Hash hash)
	{
		if (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_VALID(hash))
		{
			STREAMING::REQUEST_MODEL(hash);
			while (!STREAMING::HAS_MODEL_LOADED(hash)) script::get_current()->yield(0ms);
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), hash);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
			ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), ENTITY::GET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID()), 0);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		}
	}
	void features::write_flame_flag(uint32_t Flag)
	{
		uint32_t& flags = gta_util::get_local_player_info()->m_frame_flags;
		flags |= Flag;
	}
	void features::safety_belt()
	{
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), 0);
		memory::set_value<BYTE>({ OFFSET_PLAYER,0x145C }, -55);
	}
	void features::key_9_speed_up()
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
		if (KeyPressed(VK_NUMPAD9))
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
			if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
			{
				float speed = ENTITY::GET_ENTITY_SPEED(veh);
				speed += speed * (1.f / 100);
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
			}
		}
		if (KeyPressed(VK_NUMPAD3))
		{
			util::request_control_of_ent(veh);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0);
			memory::set_value<float>({ OFFSET_PLAYER,OFFSET_PLAYER_VEHICLE,OFFSET_VEHICLE_BOOST }, 0);
		}
	}
	void features::set_rain_bow_vehicle()
	{
		static Color primary{ 255, 0, 0 };

		Vehicle VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
		util::request_control_of_ent(VehID);
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));
		if (primary.r > 0 && primary.b == 0)
		{
			primary.r--;
			primary.g++;
		}
		if (primary.g > 0 && primary.r == 0)
		{
			primary.g--;
			primary.b++;
		}
		if (primary.b > 0 && primary.g == 0)
		{
			primary.r++;
			primary.b--;
		}
		VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(VehID, primary.r, primary.g, primary.b);
		VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(VehID, primary.r, primary.g, primary.b);

		VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(VehID, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 0, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 1, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 2, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 3, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 4, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 5, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 6, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(VehID, 7, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(VehID, primary.r, primary.g, primary.b);
	}
	Vehicle last_vehicle{};
	int features::spawn_vehicle(Hash hash)
	{
		if (g_settings.options["delete_last_vehicle"].get<bool>())
		{
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(last_vehicle, true, true);
			VEHICLE::DELETE_VEHICLE(&last_vehicle);
		}
		if (STREAMING::IS_MODEL_VALID(hash))
		{
			STREAMING::REQUEST_MODEL(hash);
			while (!STREAMING::HAS_MODEL_LOADED(hash))
			{
				script::get_current()->yield();
			}

			Ped ped = PLAYER::PLAYER_PED_ID();
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, true);
			float forward = 5.f;
			float heading = ENTITY::GET_ENTITY_HEADING(ped);
			float xVector = forward * sin(heading * 3.141592653589793f / 180.f) * -1.f;
			float yVector = forward * cos(heading * 3.141592653589793f / 180.f);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			Vehicle vehicle = VEHICLE::CREATE_VEHICLE(hash, pos.x + xVector, pos.y + yVector, pos.z, 0.f, TRUE, FALSE, FALSE);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
			last_vehicle = vehicle;

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

			if (g_settings.options["spwan_vehicle_max"])
			{
				VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
				for (int i = 0; i < 50; i++)
				{
					VEHICLE::SET_VEHICLE_MOD(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1, false);
				}
			}
			if (g_settings.options["spwan_vehicle_in"])
			{
				PED::SET_PED_INTO_VEHICLE(ped, vehicle, -1);
			}
			return vehicle;
		}
		return 0;
	}
	int features::spawn_vehicle(const char* model)
	{
		Hash hash = rage::joaat(model);
		spawn_vehicle(hash);
	}
	void scriptInit()
	{
		g_vehicles.initialize();
	}
	void features::script_func()
	{
		TRY_CLAUSE
		{
			scriptInit();
			while (true)
			{
				runTick();
				script::get_current()->yield();
			}
		}EXCEPT_CLAUSE
	}
}