#include "../gta/joaat.h"
#include "../hooking.h"
#include "../natives.h"
#include "../script_global.h"

namespace hbase
{
	void hooks::set_warning_message_with_header(const char* entryHeader, const char* entryLine1, int instructionalKey, const char* entryLine2, bool p4, Any p5, Any* showBackground, Any* p7, bool p8, Any p9)
	{
		if (g_settings.options["disable_error_screen"].get<bool>())
		{
			if (SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME() == RAGE_JOAAT("shop_controller") && strcmp(entryLine1, "CTALERT_F_2") == 0)
			{
				*script_global(4529830).as<int*>() = 0;
				return;
			}
		}

		return g_hooking->m_error_screen_hook.get_original<decltype(&hooks::set_warning_message_with_header)>()(entryHeader, entryLine1, instructionalKey, entryLine2, p4, p5, showBackground, p7, p8, p9);
	}
}