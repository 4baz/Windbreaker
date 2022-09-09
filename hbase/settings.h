#pragma once
#include "pch.h"

namespace hbase
{
	class settings
	{
	public:
		struct protections
		{
			bool freemode_terminated{ false };
		};
		protections protections{};

		__int64 script_handler_net_component_mgr{};
		unsigned __int16 lastToken{};

		Hash lastSyncModelHashReceived{ 0 };
		Hash lastScriptEventHashReceived{ 0 };

		bool m_de_remove_crash{ false };
		bool m_spawn_crash_objects{ false };
		struct buffer_crash
		{
			bool m_buffer_crash{ false };
			int m_buffer_crash_count{ 0 };
		};
		bool f_waterloop{};
		bool f_fireloop{};

		buffer_crash buffer_crash{};

		bool spectating;

		nlohmann::json options;
		nlohmann::json default_options =
			R"({
			"god_mode": false,
			"invisible": false,
			"never_wanted": false,
			"super_jump": false,
			"unlimited_ammo": false,
			"rapid_fire": false,
			"explosive_ammo": false,
			"vehicle_invincible": false,
			"safety_belt": false,
			"infinite_nitrogen": false,
			"9_key_speed_up": false,
			"block_ad": false,
			"crash_ad": false,
			"rainbow_vehicle": false,
			"spwan_vehicle_max": false,
			"spwan_vehicle_in": false,
			"delete_last_vehicle": false,
			"bounty": false,
			"ceo_ban": false,
			"ceo_kick": false,
			"ceo_money": false,
			"clear_wanted_level": false,
			"fake_deposit": false,
			"force_mission": false,
			"gta_banner": false,
			"network_bail": false,
			"personal_vehicle_destroyed": false,
			"remote_off_radar": false,
			"rotate_cam": false,
			"send_to_cutscene": false,
			"send_to_island": false,
			"sound_spam": false,
			"spectate": false,
			"force_teleport": false,
			"transaction_error": false,
			"vehicle_kick": false,
			"vehicle_kick": false,
			"invalid_sync_type": false,
			"sync_model_mismatch": false,
			"model_crash": false,
			"freeze": false,
			"control": false,
			"join_rid": 0,
			"spoof_username": false,
			"net_array": false,
			"username": "",
			"spoof_ip": false,
			"ip": [0,0,0,0],
			"spoof_rockstar_id": false,
			"rockstar_id": 0,
			"disable_error_screen": false,
			"increment_stat_event": false,
			"sync_player_model": false,
			"spoof_host_token": false,
			"host_token": 0,
			"script_kick_and_crash": false
		})"_json;

		bool save()
		{
			std::ofstream file(settings_file, std::ios::out | std::ios::trunc);
			file << options.dump(4);
			file.close();
			return true;
		}

		bool load()
		{
			std::ifstream file(settings_file);

			if (!file.is_open())
			{
				write_default_config();
				file.open(settings_file);
			}

			file >> options;

			bool should_save = false;
			for (auto& e : default_options.items())
			{
				if (options.count(e.key()) == 0)
				{
					should_save = true;
					options[e.key()] = e.value();
				}
			}

			if (should_save)
			{
				LOG(INFO) << "Updating Settings.";
				save();
			}

			return true;
		}

	private:
		bool write_default_config()
		{
			std::ofstream file(settings_file, std::ios::out);
			file << default_options.dump(4);
			file.close();

			options.clear();
			options = default_options;

			return true;
		}

		std::string settings_file = "C:\\hbase\\settings.json";
	};

	inline settings g_settings;
}
