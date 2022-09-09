#include "../hooking.h"

namespace hbase
{
	void hooks::clone_create_ack(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, uint16_t object_id, int32_t ack_code)
	{
		return g_hooking->m_clone_create_ack_hook.get_original<functions::clone_create_ack_t>()(mgr, src, dst, object_id, ack_code);
	}
}