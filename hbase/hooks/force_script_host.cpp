#include"../hooking.h"

namespace hbase
{
	char hooks::force_script_host(__int64 pCGameScriptHandlerNetComponenet, CNetGamePlayer* player, unsigned __int16 a3, char a4)
	{
		if (g_settings.script_handler_net_component_mgr == NULL)
		{
			g_settings.script_handler_net_component_mgr = pCGameScriptHandlerNetComponenet;
		}
		if (a3 > g_settings.lastToken)
			g_settings.lastToken = a3;

		return g_hooking->m_force_script_host_hook.get_original<functions::force_script_host_t>()(pCGameScriptHandlerNetComponenet, player, a3, a4);
	}
}