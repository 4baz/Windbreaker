#include "../hooking.h"
#include"../protection.h"

namespace hbase
{
	void hooks::player_leave(CNetworkObjectMgr* _this, CNetGamePlayer* net_player)
	{
		protection::delete_player(net_player);
		return g_hooking->m_player_has_left_hook.get_original<decltype(&hooks::player_leave)>()(_this, net_player);
	}
}