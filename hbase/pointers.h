#pragma once
#include "pch.h"
#include "gta/fwddec.h"
#include "function_types.h"
#include"gta/blip.h"
#include "gta/replay.h"
#include"gta/player.h"

namespace hbase
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		HWND m_hwnd{};

		IDXGISwapChain** m_swapchain{};

		rage::atArray<GtaThread*>* m_script_threads{};
		functions::run_script_threads_t m_run_script_threads{};

		rage::scrNativeRegistrationTable* m_native_registration_table{};
		functions::get_native_handler_t m_get_native_handler{};

		functions::fix_vectors_t m_fix_vectors{};

		CPedFactory** m_ped_factory{};

		bool* m_is_session_started{};

		PVOID m_model_spawn_bypass;

		PVOID m_gta_thread_start{};
		PVOID m_gta_thread_tick{};
		PVOID m_gta_thread_kill{};

		functions::read_bitbuf_array_t m_read_bitbuf_array{};
		functions::read_bitbuf_dword_t m_read_bitbuf_dword{};

		functions::scripted_game_event_t m_scripted_game_event{};
		functions::received_event_t m_received_event{};
		functions::send_event_ack_t m_send_event_ack{};

		int64_t* m_menu_ptr{};
		functions::get_friend_rid_t m_get_friend_rid{};
		functions::rid_joiner_t m_rid_joiner{};

		PVOID m_is_dlc_present;

		functions::increment_stat_event_t m_increment_stat_event{};

		PVOID m_error_screen{};

		PVOID m_native_return;

		PVOID m_send_net_info_to_lobby{};

		std::int64_t** m_script_globals{};

		functions::get_net_game_player_t m_get_net_game_player{};

		rage::BlipList* m_blip_list{};

		functions::force_script_host_t m_forceScriptHost{};

		PVOID m_player_has_joined{};
		PVOID m_player_has_left{};

		functions::can_apply_to_object_t m_netSyncTree_CanApplyToObject{};

		CGameScriptHandlerMgr** m_script_handler_mgr{};

		functions::node_read_t m_node_read{};
		functions::get_owner_t m_get_owner{};
		functions::get_network_object_for_player_t m_get_network_object_for_player{};

		functions::get_network_object_t m_get_network_object{};
		functions::get_sync_tree_from_type_t m_get_sync_tree_from_type{};

		CNetworkObjectMgr** _m_network_object_manager{};
		CNetworkObjectMgr* m_network_object_manager() {
			return *_m_network_object_manager;
		}

		rage::CReplayInterface* m_replay{};

		functions::clone_create_t m_clone_create{};
		functions::clone_sync_t m_clone_sync{};
		functions::clone_remove_t m_clone_remove{};

		functions::sync_read_t m_sync_read{};

		functions::unregister_object_t m_unregister_object{};
		functions::clone_create_ack_t m_clone_create_ack{};
		functions::clone_sync_ack_t m_clone_sync_ack{};
		functions::clone_remove_ack_t m_clone_remove_ack{};

		functions::send_clone_sync_t m_send_clone_sync{};
		functions::pack_clone_create_t m_pack_clone_create{};
		functions::pack_clone_remove_t m_pack_clone_remove{};

		functions::uWriteBitBufferQword write_bit_buffer_qword{};
		functions::uWriteBitBufferDword write_bit_buffer_dword{};

		functions::get_model_info_t m_get_model_info{};

		functions::net_array_handler_t m_net_array_handler{};

		functions::sync_player_model_t m_sync_player_model{};

		functions::fpGetChatPlayer get_chat_player;
		functions::fpGetChatData get_chat_data;

		functions::trigger_script_event_t m_trigger_script_event{};
		functions::remove_player_t m_remove_player{};

		functions::sub_7FF7AA059CC4_t m_sub_7FF7AA059CC4{};

		rage::CNetworkPlayerMgr** m_network_player_mgr{};
	};

	inline pointers* g_pointers{};
}