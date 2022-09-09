#include"../hooking.h"
#include"../gta/net_object_mgr.h"

namespace hbase
{
	__int64 hooks::m_sub_7FF7AA059CC4(rage::netObject* netObject)
	{
		if (g_settings.m_spawn_crash_objects)
		{
			netObject->object_type = 14;
		}
		return g_hooking->m_sub_7FF7AA059CC4_hook.get_original<functions::sub_7FF7AA059CC4_t>()(netObject);
	}
}