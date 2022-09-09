#include "../hooking.h"
#include"../gta_util.hpp"
namespace hbase
{
	bool hooks::send_net_info_to_lobby(rage::netPlayerData* player, int64_t a2, int64_t a3, DWORD* a4)
	{
		// check so we're 100% sure we modify data only for ourselves
		if (gta_util::get_local_player_info()->m_net_player_data.m_rockstar_id == player->m_rockstar_id)
		{
			if (g_settings.options["spoof_username"].get<bool>())
				memcpy(player->m_name, g_settings.options["username"].get<std::string>().c_str(), sizeof(player->m_name));

			if (g_settings.options["spoof_ip"].get<bool>())
			{
				player->m_external_ip.m_field1 = g_settings.options["ip"][0];
				player->m_external_ip.m_field2 = g_settings.options["ip"][1];
				player->m_external_ip.m_field3 = g_settings.options["ip"][2];
				player->m_external_ip.m_field4 = g_settings.options["ip"][3];
			}

			if (g_settings.options["spoof_rockstar_id"].get<bool>())
			{
				player->m_rockstar_id = g_settings.options["rockstar_id"].get<int>();
				//player->m_rockstar_id = g_settings.options["rockstar_id"].get<int>();
			}

			if (g_settings.options["spoof_host_token"].get<bool>())
			{
				player->m_host_token = g_settings.options["host_token"].get<int>();
			}
		}

		return g_hooking->m_send_net_info_to_lobby.get_original<decltype(&hooks::send_net_info_to_lobby)>()(player, a2, a3, a4);
	}
}