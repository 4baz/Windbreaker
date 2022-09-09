#include"../hooking.h"

namespace hbase
{
	bool hooks::write_bitbuffer_dword(__int64 bit_buffer, int value, int bits)
	{
		if (g_settings.buffer_crash.m_buffer_crash)
		{
			g_settings.buffer_crash.m_buffer_crash_count++;
			if (g_settings.buffer_crash.m_buffer_crash_count >= 10000)
			{
				g_settings.buffer_crash.m_buffer_crash = false;
				g_settings.buffer_crash.m_buffer_crash_count = 0;
				LOG(INFO) << "»º³åÆ÷±ÀÀ£ÖÕÖ¹";
			}
			for (int i = 0; i < 5; i++)
			{
				g_pointers->write_bit_buffer_qword(bit_buffer, 0x7fff, rand() % 0xff);
				g_hooking->m_write_bitbuffer_dword_hook.get_original<functions::uWriteBitBufferDword>()(bit_buffer, value, bits);
			}

			return true;
		}

		return g_hooking->m_write_bitbuffer_dword_hook.get_original<functions::uWriteBitBufferDword>()(bit_buffer, value, bits);
	}
}