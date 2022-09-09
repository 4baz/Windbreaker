#pragma once
#include "gta/fwddec.h"
#include "gta/natives.h"
#include"gta/player.h"

namespace hbase
{
	namespace functions
	{
		using run_script_threads_t = bool(*)(std::uint32_t ops_to_execute);
		using get_native_handler_t = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
		using fix_vectors_t = void(*)(rage::scrNativeCallContext*);

		using read_bitbuf_array_t = bool(*)(rage::datBitBuffer* buffer, PVOID read, int bits, int);
		using read_bitbuf_dword_t = bool(*)(rage::datBitBuffer* buffer, PVOID read, int bits);

		using scripted_game_event_t = bool(*)(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player);
		using received_event_t = bool(*)(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, uint16_t event_id, int event_index, int event_handled_bitset, int64_t bit_buffer_size, int64_t bit_buffer);
		using send_event_ack_t = void(*)(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, int event_index, int event_handled_bitset);

		using get_friend_rid_t = int64_t * (*)(int32_t friendIndex);
		using rid_joiner_t = BOOL(*)(int64_t* a1, int* hash);

		using increment_stat_event_t = bool(*)(uint64_t net_event_struct, int64_t sender, int64_t a3);

		using get_net_game_player_t = CNetGamePlayer * (*) (Player player);

		using force_script_host_t = char (*)(__int64 pCGameScriptHandlerNetComponenet, CNetGamePlayer* player, unsigned __int16 a3, char a4);

		using unregister_object_t = void (*)(CNetworkObjectMgr* mgr, rage::netObject* netObject, int reason, bool force1, bool force2);

		using clone_create_ack_t = void (*) (CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, uint16_t object_id, int32_t ack_code);
		using clone_sync_ack_t = void (*) (CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, rage::datBitBuffer* buffer, uint16_t object_id, int32_t ack_code);
		using clone_remove_ack_t = void (*) (CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, uint16_t object_id, int32_t ack_code);

		using buffer_int_t = void (*)(rage::datBitBuffer* buffer, uint32_t* value, int32_t length, int32_t unk);

		using pack_clone_create_t = void (*) (CNetworkObjectMgr* mgr, rage::netObject* netObject, CNetGamePlayer* src, rage::datBitBuffer* buffer);
		using send_clone_sync_t = void(*)(CNetworkObjectMgr* mgr, CNetGamePlayer* src, rage::netObject* netObject, __int64 a4, uint16_t* a5, bool a6);
		using pack_clone_remove_t = bool(*)(CNetworkObjectMgr* mgr, rage::netObject* netObject, CNetGamePlayer* src, bool a4);
		//char __fastcall pack_clone_remove(__int64 a1, __int64 a2, __int64 a3, char a4)

		using clone_create_t = bool (*) (CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t object_type, int32_t object_id, int32_t object_flag, rage::datBitBuffer* buffer, int32_t timestamp);
		using clone_sync_t = bool (*) (CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t object_type, int32_t object_id, rage::datBitBuffer* buffer, int32_t unk, int32_t timestamp);
		using clone_remove_t = void (*) (CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t object_id, int32_t unk);

		using sync_read_t = bool (*)(rage::netSyncTree* self, int32_t sync_type, int32_t sync_flag, rage::datBitBuffer* buffer, void* netLogStub);

		using can_apply_to_object_t = bool (*)(rage::netSyncTree* self, rage::netObject* obj);

		using get_owner_t = CNetGamePlayer * (*)(rage::netObject* netObject);

		using get_network_object_t = rage::netObject* (*) (CNetworkObjectMgr*, int16_t, bool);

		using get_sync_tree_from_type_t = rage::netSyncTree* (*) (CNetworkObjectMgr*, unsigned __int16);

		using get_network_object_for_player_t = rage::netObject* (*) (CNetworkObjectMgr*, int16_t, CNetGamePlayer*, bool);

		using node_read_t = bool(*)(void* node, int32_t sync_flag, void* mA0, rage::datBitBuffer* buffer, rage::netObject* object);

		using uWriteBitBufferQword = bool(*)(__int64 bit_buffer, __int64 value, int bits);
		using uWriteBitBufferDword = bool(*)(__int64 bit_buffer, int value, int bits);

		using get_model_info_t = rage::CBaseModelInfo* (*)(std::uint32_t hash, std::int32_t* index);

		using sync_player_model_t = char(*)(__int64 a1, __int64 a2);

		using net_array_handler_t = char(*)(int64_t this_, CNetGamePlayer* player, rage::datBitBuffer* buffer, int size, int16_t id);

		typedef __int64(__cdecl* fpGetChatData)(__int64 a1, __int64 a2, __int64 a3, const char* origText, BOOL a5);
		typedef __int64(__cdecl* fpGetChatPlayer)(__int64 a1, __int64 a2, __int64 a3);

		using trigger_script_event_t = void(*)(int event_group, int64_t* args, int arg_count, int player_bits);
		using remove_player_t = void (*)(rage::CNetworkPlayerMgr*, CNetGamePlayer* net_game_player);

		using sub_7FF7AA059CC4_t = __int64 (*)(rage::netObject*);
		typedef uint64_t(__cdecl* fpGetEntityFromScriptGuId)(int ped);
	}
}