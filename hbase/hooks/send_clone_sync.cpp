#include "../hooking.h"

namespace hbase
{
	void hooks::send_clone_sync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, rage::netObject* netObject, __int64 a4, uint16_t* a5, bool a6)
	{
		return g_hooking->m_send_clone_sync_hook.get_original<functions::send_clone_sync_t>()(mgr, src, netObject, a4, a5, a6);
	}
}