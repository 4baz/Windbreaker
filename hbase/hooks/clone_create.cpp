#include "../hooking.h"
#include"../gta/net_object_mgr.h"
#include"../gta/structs.h"
#include"../protection.h"
namespace hbase
{
	bool hooks::clone_create(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t _object_type, int32_t _object_id, int32_t _object_flag, rage::datBitBuffer* buffer, int32_t timestamp)
	{
		//±»¸ôÀëÍæ¼Ò
		if (protection::is_block(src))
		{
			return false;
		}

		return g_hooking->m_clone_create_hook.get_original<functions::clone_create_t>()(mgr, src, dst, _object_type, _object_id, _object_flag, buffer, timestamp);
	}
}