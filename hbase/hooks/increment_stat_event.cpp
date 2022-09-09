#include "../hooking.h"

namespace hbase
{
	bool hooks::increment_stat_event(CNetworkIncrementStatEvent* net_event, CNetGamePlayer* sender, int64_t a3)
	{
		if (g_settings.options["increment_stat_event"].get<bool>())
		{
			switch (net_event->m_stat)
			{
			case RAGE_JOAAT("MPPLY_GAME_EXPLOITS"):
			case RAGE_JOAAT("MPPLY_VC_HATE"):
			case RAGE_JOAAT("MPPLY_EXPLOITS"):
			case RAGE_JOAAT("MPPLY_TC_ANNOYINGME"):
			case RAGE_JOAAT("MPPLY_TC_HATE"):
				LOG(INFO) << fmt::format("拦截举报 来自: {}", sender->get_name());
				return true;
			}
		}

		return g_hooking->m_increment_stat_hook.get_original<decltype(&increment_stat_event)>()(net_event, sender, a3);
	}
}