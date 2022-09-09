#include "../hooking.h"
#include"../protection.h"
namespace hbase
{
	void hooks::player_join(CNetworkObjectMgr* _this, CNetGamePlayer* net_player)
	{
		protection::add_player(net_player);
		return g_hooking->m_player_has_joined_hook.get_original<decltype(&hooks::player_join)>()(_this, net_player);
	}
}