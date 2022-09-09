#include "../hooking.h"
#include"../script.h"
#include"../fiber_pool.h"
#include"../script_mgr.h"
namespace hbase
{
	bool IsGGJ(std::string message) {
		char* normal[] = { "群", "解锁", "辅助", "科技", "成人", "少妇", "淫", "少女" };
		for (int i = 0; i < 8; i++)
		{
			if (strstr(message.c_str(), normal[i]) == NULL) {
				continue;
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	__int64 hooks::get_chat_data(__int64 a1, __int64 a2, __int64 a3, const char* origText, BOOL isTeam)
	{
		std::string Message(origText);
		CNetGamePlayer* player = (CNetGamePlayer*)g_pointers->get_chat_player(a3, a2, a3);
		bool isggj = IsGGJ(Message);
		if (g_settings.options["crash_ad"].get<bool>())
		{
			if (isggj) {
				if (player->is_valid())
				{
					g_fiber_pool->queue_job([=]
						{
							int64_t tseargs[3] = { -1386010354, player->m_player_id, 1 };
							g_pointers->m_trigger_script_event(1, tseargs, 3, 1 << player->m_player_id);
							int64_t tseargs1[3] = { 962740265, player->m_player_id, 1 };
							g_pointers->m_trigger_script_event(1, tseargs1, 3, 1 << player->m_player_id);
							int64_t tseargs2[3] = { 603406648, player->m_player_id, 1 };
							g_pointers->m_trigger_script_event(1, tseargs2, 3, 1 << player->m_player_id);
							int64_t tseargs3[3] = { -1715193475, player->m_player_id, 1 };
							g_pointers->m_trigger_script_event(1, tseargs3, 3, 1 << player->m_player_id);
							int64_t tseargs4[3] = { 1258808115, player->m_player_id, 1 };
							g_pointers->m_trigger_script_event(1, tseargs4, 3, 1 << player->m_player_id);
							int64_t tseargs6[3] = { -1386010354, player->m_player_id, 1 };
							g_pointers->m_trigger_script_event(1, tseargs6, 3, 1 << player->m_player_id);
							int64_t tseargs23[10] = { -1386010354, player->m_player_id, 0,  1229879414 , 1742796505 , -1794944145 , -480124848 , 439776191 , -2062014584 , 2120444344 };
							g_pointers->m_trigger_script_event(1, tseargs23, 10, 1 << player->m_player_id);
							//features::notifyfmt(u8"~r~Yi检测到广告机~s~~n~玩家:~b~%s~s~~n~措施:~r~崩溃", player->get_name());
						});
				}
				LOG(INFO) << "崩溃广告机:" << player->get_name();
			}
		}
		if (g_settings.options["block_ad"].get<bool>())
		{
			if (isggj) {
				return 0;
			}
		}

		return g_hooking->m_get_chat_data_hook.get_original<decltype(&get_chat_data)>()(a1, a2, a3, origText, isTeam);
	}
}