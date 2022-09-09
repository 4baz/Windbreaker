#include "../hooking.h"

namespace hbase
{
	rage::eThreadState hooks::gta_thread_kill(GtaThread* thread)
	{
		rage::eThreadState result = g_hooking->m_gta_thread_kill_hook.get_original<decltype(&gta_thread_kill)>()(thread);

		if (thread->m_script_hash == RAGE_JOAAT("freemode"))
			g_settings.protections.freemode_terminated = !(result == rage::eThreadState::running);

		return result;
	}
}