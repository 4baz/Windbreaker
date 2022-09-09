#include"../hooking.h"
#include"../settings.h"

namespace hbase
{
	bool joinRid{};
	int64_t* hooks::get_friend_rid(int32_t friendIndex)
	{
		auto rid = g_hooking->m_get_friend_rid_hook.get_original<functions::get_friend_rid_t>()(friendIndex);
		if (joinRid)
		{
			*rid = (int64_t)g_settings.options["join_rid"].get<int>();
		}
		return rid;
	}

	BOOL hooks::rid_joiner(int64_t* a1, int* hash)
	{
		joinRid = true;
		auto result = g_hooking->m_rid_joiner_hook.get_original<functions::rid_joiner_t>()(a1, hash);
		joinRid = false;
		return result;
	}
}