#include"../hooking.h"
#include"../gta/net_object_mgr.h"
#include"../protection.h"

namespace hbase
{
	bool hooks::can_apply_to_object(rage::netSyncTree* self, rage::netObject* obj)
	{
		if (g_settings.options["model_crash"].get<bool>())
		{
			Hash modelHash = self->m_sync_tree_node->model(obj->object_type);
			if (obj->object_type != rage::PLAYER)
			{
				switch (modelHash)
				{
				case 0xE5A2D6C6:
				case 0xFCFCB68B:
				case 0xFD707EDE:
				case 0xD6BC7523:
				case 0x21B4080F:
				case 0x2A72BEAB:
				case 0x4BBA84D9:
				case 0x782A236D:
					CNetGamePlayer * player = g_pointers->m_get_net_game_player(obj->owner_id);
					if (player)
					{
						protection::set_block(player);
					}
					LOG(INFO) << "崩溃保护 | 模型崩溃";

					return false;
				}
			}
		}

		return g_hooking->can_apply_to_object_hook.get_original<functions::can_apply_to_object_t>()(self, obj);
	}
}