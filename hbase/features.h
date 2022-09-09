#pragma once

namespace hbase::features
{
	Vehicle spawn_vehicle(Hash hash, Entity on_entity);
	void spawn_crash_vehicle(Ped ped, char* vehicle, int id1, int id2);
	void runTick();

	void spectator(bool b, int player_id);
	void give_all_weapons();
	void give_all_mk2();
	void give_stun_gun();
	void give_digiscanner();
	void de_remove_ped_crash();
	void de_remove_veh_crash();
	void de_remove_veh_crash1();
	void script_func();

	void set_player_wanted_level(int i);

	extern float run_speed;
	void set_player_run_speed(float f);

	extern float swim_speed;
	void set_player_swim_speed(float f);

	void set_player_invisible(bool b);

	void chage_player_model(Hash hash);

	void teleport_to_objective();

	void tp_to_waypoint();

	void player_rapid_fire();

	void write_flame_flag(uint32_t Flag);

	void set_rain_bow_vehicle();

	void infinite_nitrogen();

	void teleport_to_coords(Entity e, Vector3 coords);

	void tp_to_vector3(Vector3 coords);

	void safety_belt();

	void key_9_speed_up();

	int spawn_vehicle(const char* model);
	int spawn_vehicle(Hash hash);

	void cage_player(Ped ped);
	void air_strike_player(Player player);
	void clone_player(Player player);
	void water_loop(bool enable, Player player);
	void fire_loop(bool enable, Player player);
	void ram_player();
	void copy_outfit(Player player);
}