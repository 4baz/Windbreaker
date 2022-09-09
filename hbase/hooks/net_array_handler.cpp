#include"../hooking.h"
#include"../protection.h"

namespace hbase
{
	char hooks::net_array_handler(int64_t a1, CNetGamePlayer* player, rage::datBitBuffer* buffer, int size, int16_t id)
	{
		if (protection::is_block(player))
		{
			return true;
		}
		char v10; // r15
		int i; // eax
		int v12; // r12d
		__int64 v13; // rdx
		__int64 v14; // r9
		__int64 v15; // rax
		int v16; // eax
		unsigned int v18; // [rsp+20h] [rbp-10h] BYREF
		int v19; // [rsp+24h] [rbp-Ch]
		char v20; // [rsp+70h] [rbp+40h]
		char v21; // [rsp+88h] [rbp+58h] BYREF

		v10 = 1;
		v19 = buffer->m_unkBit;
		v20 = (*(__int64(__fastcall**)(__int64))(*(uint64_t*)a1 + 344i64))(a1);
		for (i = buffer->m_unkBit - v19; i < size; i = v16 - v19)
		{
			v12 = buffer->m_unkBit;
			(*(void(__fastcall**)(__int64, rage::datBitBuffer*, unsigned int*))(*(uint64_t*)a1 + 376i64))(a1, buffer, &v18);
			if (!(*(unsigned __int8(__fastcall**)(__int64, uint64_t))(*(uint64_t*)a1 + 384i64))(a1, v18))
			{
				return 1;
			}
			if (v20)
			{
				break;
			}
			else
			{
				v14 = 0;
				v21 = 0;
			}
			(*(void(__fastcall**)(__int64, rage::datBitBuffer*, uint64_t, __int64))(*(uint64_t*)a1 + 448i64))(a1, buffer, v18, 0);
			v14 = v21;
			if ((*(unsigned __int8(__fastcall**)(__int64, uint64_t, CNetGamePlayer*, __int64))(*(uint64_t*)a1 + 424i64))(
				a1,
				v18,
				player,
				v14))
			{
				if ((*(__int64(__fastcall**)(__int64))(*(uint64_t*)a1 + 304i64))(a1))
				{
					v15 = (*(__int64(__fastcall**)(__int64))(*(uint64_t*)a1 + 304i64))(a1);
					(*(void(__fastcall**)(__int64, uint64_t))(*(uint64_t*)v15 + 160i64))(v15, v18);
				}
				(*(void(__fastcall**)(__int64, uint64_t, CNetGamePlayer*))(*(uint64_t*)a1 + 472i64))(a1, v18, player);
				(*(void(__fastcall**)(__int64, uint64_t))(*(uint64_t*)a1 + 416i64))(a1, v18);
			}
			else
			{
				v10 = 0;
			}
			if (v12 == (unsigned int)buffer->m_unkBit)
			{
				if (player->is_valid())
				{
					LOG(INFO) << "崩溃保护 | NetArray崩溃 | 发起者:" << player->get_name();
					protection::set_block(player);
				}
				return true;
			}
			v16 = buffer->m_unkBit;
		}
		(*(void(__fastcall**)(__int64))(*(uint64_t*)a1 + 408i64))(a1);
		if (v10 && player->m_player_id != 0xFF)
			*(WORD*)(a1 + 2i64 * player->m_player_id + 108) = id;
		return v10;
	}
}