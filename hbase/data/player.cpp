#include"player.h"
#include"../natives.h"
#include"../script.h"
#include"../pointers.h"

namespace hbase
{
	void players::on_tick()
	{
		for (size_t i = 0; i < 32; i++)
		{
			player_class& player = m_players[i];
			Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			{
				player.is_onlie = true;
				player.ped_id = ped;
				player.is_me = i == PLAYER::PLAYER_ID();
				player.net_player = g_pointers->m_get_net_game_player(i);
				if (player.net_player)
				{
					player.is_host = player.net_player->is_host();
				}
				player.is_script_host = NETWORK::NETWORK_GET_HOST_OF_SCRIPT("freemode", -1, 0) == i;
				strcpy(player.player_name, PLAYER::GET_PLAYER_NAME(i));
			}
			else
			{
				strcpy(player.player_name, "ÀëÏß");
				player.is_onlie = false;
			}
			script::get_current()->yield();
		}
	}
	void player::player_script()
	{
		TRY_CLAUSE
		{
			while (true)
			{
				g_players.on_tick();
				script::get_current()->yield();
			}
		}EXCEPT_CLAUSE
	}
}