#include "../hooking.h"
#include"../gta/net_object_mgr.h"

namespace hbase
{
	bool hooks::pack_clone_remove(CNetworkObjectMgr* mgr, rage::netObject* netObject, CNetGamePlayer* src, bool a4)
	{
		if (g_settings.m_de_remove_crash)
		{
			return false;
		}

		return g_hooking->m_pack_clone_remove_hook.get_original<functions::pack_clone_remove_t>()(mgr, netObject, src, a4);
	}
}