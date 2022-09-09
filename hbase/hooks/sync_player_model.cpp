#include"../hooking.h"
#include"../protection.h"

namespace hbase
{
	char hooks::sync_player_model(__int64 a1, __int64 a2)
	{
		int player_id = (int)*(unsigned __int8*)(a2 + 0x49);
		int hash = *(DWORD*)(a1 + 0x410);
		CNetGamePlayer* player = g_pointers->m_get_net_game_player(player_id);
		if (player)
		{
			int64_t rid = player->get_net_data()->m_rockstar_id;
			if (protection::users[rid]->player_model == 0)
			{
				protection::users[rid]->player_model = hash;
			}
			else
			{
				if (protection::users[rid]->player_model != hash)
				{
					protection::set_block(player);
					LOG(INFO) << "sync_player_model";
					return false;
				}
			}
		}
		return g_hooking->m_sync_player_model_hook.get_original<functions::sync_player_model_t>()(a1, a2);
	}
}