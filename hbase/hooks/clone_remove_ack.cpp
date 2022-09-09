#include "../hooking.h"

namespace hbase
{
	void hooks::clone_remove_ack(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, uint16_t object_id, int32_t ack_code)
	{
		return g_hooking->m_clone_remove_ack_hook.get_original<functions::clone_remove_ack_t>()(mgr, src, dst, object_id, ack_code);
	}
}