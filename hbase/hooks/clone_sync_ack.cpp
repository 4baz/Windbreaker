#include "../hooking.h"

namespace hbase
{
	void hooks::clone_sync_ack(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, rage::datBitBuffer* buffer, uint16_t object_id, int32_t ack_code)
	{
		return g_hooking->m_clone_sync_ack_hook.get_original<functions::clone_sync_ack_t>()(mgr, src, dst, buffer, object_id, ack_code);
	}
}