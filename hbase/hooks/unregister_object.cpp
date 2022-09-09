#include "../hooking.h"

namespace hbase
{
	void hooks::unregister_object(CNetworkObjectMgr* mgr, rage::netObject* netObject, int reason, bool force1, bool force2)
	{
		return g_hooking->m_unregister_object_hook.get_original<functions::unregister_object_t>()(mgr, netObject, reason, force1, force2);
	}
}