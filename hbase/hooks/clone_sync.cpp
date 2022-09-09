#include "../hooking.h"
#include"../gta/structs.h"
#include"../gta/net_object_mgr.h"
#include"../protection.h"

namespace hbase
{
	bool hooks::clone_sync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t _object_type, int32_t _object_id, rage::datBitBuffer* buffer, int32_t unk, int32_t timestamp)
	{
		//无效类型
		if (g_settings.options["invalid_sync_type"].get<bool>())
		{
			if (_object_type > rage::TRAIN)
			{
				protection::set_block(src);
				LOG(INFO) << "无效同步类型";
				return false;
			}
		}

		rage::netObject* netObject = g_pointers->m_get_network_object(mgr, _object_id, true);
		if (g_settings.options["sync_model_mismatch"].get<bool>())
		{
			if (netObject != nullptr && netObject->object_type != _object_type)
			{
				protection::set_block(src);
				LOG(INFO) << "同步模型更改";
				return false;
			}
		}

		//被隔离玩家
		if (protection::is_block(src))
		{
			return false;
		}

		return g_hooking->m_clone_sync_hook.get_original<functions::clone_sync_t>()(mgr, src, dst, _object_type, _object_id, buffer, unk, timestamp);
	}
}