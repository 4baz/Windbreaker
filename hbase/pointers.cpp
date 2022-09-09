#include "pch.h"
#include "logger.h"
#include "pointers.h"
#include "memory/all.h"
#include"gta/blip.h"

namespace hbase
{
	pointers::pointers()
	{
		memory::pattern_batch main_batch;
		main_batch.add("SC", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", [this](memory::handle ptr)
			{
				m_swapchain = ptr.add(3).rip().as<IDXGISwapChain**>();
			});
		main_batch.add("SH&RSH", "45 33 F6 8B E9 85 C9 B8", [this](memory::handle ptr)
			{
				m_script_threads = ptr.sub(4).rip().sub(8).as<rage::atArray<GtaThread*>*>();
				m_run_script_threads = ptr.sub(0x1F).as<functions::run_script_threads_t>();
			});
		main_batch.add("NRT&GNH", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", [this](memory::handle ptr)
			{
				m_native_registration_table = ptr.add(3).rip().as<rage::scrNativeRegistrationTable*>();
				m_get_native_handler = ptr.add(12).rip().as<functions::get_native_handler_t>();
			});
		main_batch.add("FV", "83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA", [this](memory::handle ptr)
			{
				m_fix_vectors = ptr.as<functions::fix_vectors_t>();
			});
		main_batch.add("PF", "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81", [this](memory::handle ptr)
			{
				m_ped_factory = ptr.add(3).rip().as<CPedFactory**>();
			});
		main_batch.add("GTS", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B FA 85 D2 75 2A 8B 15", [this](memory::handle ptr)
			{
				m_gta_thread_start = ptr.as<PVOID>();
			});
		main_batch.add("TT", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 8B FA 48 8B D9 74 05", [this](memory::handle ptr)
			{
				m_gta_thread_tick = ptr.as<PVOID>();
			});
		main_batch.add("TK", "48 89 5C 24 ? 57 48 83 EC 20 48 83 B9 ? ? ? ? ? 48 8B D9 74 14", [this](memory::handle ptr)
			{
				m_gta_thread_kill = ptr.as<PVOID>();
			});
		main_batch.add("MSB", "48 8B C8 FF 52 30 84 C0 74 05 48", [this](memory::handle ptr)
			{
				m_model_spawn_bypass = ptr.add(8).as<PVOID>();
			});
		main_batch.add("ISA", "40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE", [this](memory::handle ptr)
			{
				m_is_session_started = ptr.add(3).rip().as<bool*>();
			});
		main_batch.add("RBWD", "48 89 74 24 ? 57 48 83 EC 20 48 8B D9 33 C9 41 8B F0 8A", [this](memory::handle ptr)
			{
				m_read_bitbuf_dword = ptr.sub(5).as<functions::read_bitbuf_dword_t>();
			});
		main_batch.add("RBA", "48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C", [this](memory::handle ptr)
			{
				m_read_bitbuf_array = ptr.as<functions::read_bitbuf_array_t>();
			});
		main_batch.add("SGEH", "40 53 48 81 EC ? ? ? ? 44 8B 81 ? ? ? ? 4C 8B CA 41 8D 40 FF 3D ? ? ? ? 77 42", [this](memory::handle ptr)
			{
				m_scripted_game_event = ptr.as<functions::scripted_game_event_t>();
			});
		main_batch.add("REH", "66 41 83 F9 ? 0F 83 ? ? ? ?", [this](memory::handle ptr)
			{
				m_received_event = ptr.as<functions::received_event_t>();
			});
		main_batch.add("SEA", "48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 7A", [this](memory::handle ptr)
			{
				m_send_event_ack = ptr.sub(5).as<functions::send_event_ack_t>();
			});
		main_batch.add("MP", "48 8B 0D ? ? ? ? 48 03 D2 48 8B 0C D1", [this](memory::handle ptr)
			{
				int64_t* tmp = ptr.add(3).rip().as<int64_t*>();
				if (*tmp != NULL)
				{
					m_menu_ptr = (int64_t*)(*tmp + 0xA0);
				}
			});
		main_batch.add("GFI", "75 1C E8 ? ? ? ? 48 85", [this](memory::handle ptr)
			{
				m_get_friend_rid = ptr.sub(11).as<functions::get_friend_rid_t>();
			});
		main_batch.add("RJ", "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B D9", [this](memory::handle ptr)
			{
				m_rid_joiner = ptr.as<functions::rid_joiner_t>();
			});
		main_batch.add("ISE", "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 55 41 56 41 57 48 8B EC 48 83 EC 60 8B 79 30", [this](memory::handle ptr)
			{
				m_increment_stat_event = ptr.as<functions::increment_stat_event_t>();
			});
		main_batch.add("SNITL", "44 8B 6C 24 ? 45 8B C6 48 8D 4E 70 41 8B D5 45 2B C5 4C 8D 4C 24 ? 03 D5 44 2B C5 49 03 D4 E8 ? ? ? ? 84 C0 74 69", [this](memory::handle ptr)
			{
				m_send_net_info_to_lobby = ptr.sub(0x64).as<PVOID>();
			});
		main_batch.add("NRF", "FF E3", [this](memory::handle ptr)
			{
				m_native_return = ptr.add(0).as<PVOID>();
			});
		main_batch.add("ESH", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 60 4C 8B F2 48 8B 94 24 ? ? ? ? 33 DB", [this](memory::handle ptr)
			{
				m_error_screen = ptr.as<decltype(m_error_screen)>();
			});
		main_batch.add("SNITL", "44 8B 6C 24 ? 45 8B C6 48 8D 4E 70 41 8B D5 45 2B C5 4C 8D 4C 24 ? 03 D5 44 2B C5 49 03 D4 E8 ? ? ? ? 84 C0 74 69", [this](memory::handle ptr)
			{
				m_send_net_info_to_lobby = ptr.sub(0x64).as<decltype(m_send_net_info_to_lobby)>();
			});
		main_batch.add("RCOEP", "48 89 5C 24 ? 57 48 83 EC 20 8B D9 E8 ? ? ? ? ? ? ? ? 8B CB", [this](memory::handle ptr)
			{
				PVOID spectator_check = ptr.add(0x11).as<PVOID>();
				memset(spectator_check, 0x90, 0x4);
			});
		main_batch.add("IVF", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 33 FF 48 8B DA", [this](memory::handle ptr)
			{
				uint8_t* incompatible_version = ptr.add(0x165CEE5 - 0x165CB80).as<uint8_t*>();
				memset(incompatible_version, 0x90, 0x165CF03 - 0x165CEE5);
			});
		main_batch.add("SG", "48 8D 15 ? ? ? ? 4C 8B C0 E8 ? ? ? ? 48 85 FF 48 89 1D", [this](memory::handle ptr)
			{
				m_script_globals = ptr.add(3).rip().as<std::int64_t**>();
			});
		main_batch.add("IDP", "48 89 5C 24 ? 57 48 83 EC ? 81 F9", [this](memory::handle ptr)
			{
				m_is_dlc_present = ptr.as<decltype(m_is_dlc_present)>();
			});
		main_batch.add("GNGP", "48 83 EC ? 33 C0 38 05 ? ? ? ? 74 ? 83 F9", [this](memory::handle ptr)
			{
				m_get_net_game_player = ptr.as<functions::get_net_game_player_t>();
			});
		main_batch.add("BL", "4C 8D 05 ? ? ? ? 0F B7 C1", [this](memory::handle ptr)
			{
				m_blip_list = ptr.add(3).rip().as<rage::BlipList*>();
			});
		main_batch.add("FSH", "48 8D 05 ? ? ? ? 8B D3 48 8B", [this](memory::handle ptr)
			{
				PVOID location = ptr.add(3).rip().as<PVOID>();
				//		CNetPtrMgr::MigrateScriptHost = (fpMigrateScriptHost)(*(__int64*)((__int64)location + (31 * 8)));
				m_forceScriptHost = (functions::force_script_host_t)(*(__int64*)((__int64)location + (31 * 8)));
			});	// (rip(3))
		main_batch.add("PHJ", "48 8B CA 48 8B F2 FF 50 18 4C 8D 05", [this](memory::handle ptr)
			{
				m_player_has_joined = ptr.sub(0x26).as<PVOID>();
			});
		main_batch.add("PHL", "4C 8B F1 48 8B CA 48 8B EA FF 50 18 4C 8D 05", [this](memory::handle ptr)
			{
				m_player_has_left = ptr.sub(0x26).as<PVOID>();
			});
		main_batch.add("NSTCATO", "49 8B CE FF 50 70 84 C0 74 31 33 FF", [this](memory::handle ptr)
			{
				m_netSyncTree_CanApplyToObject = ptr.sub(0x2C).as<functions::can_apply_to_object_t>();
			});
		main_batch.add("SHM", "48 8B 0D ? ? ? ? 4C 8B CE E8 ? ? ? ? 48 85 C0 74 05 40 32 FF", [this](memory::handle ptr)
			{
				m_script_handler_mgr = ptr.add(3).rip().as<CGameScriptHandlerMgr**>();
			});
		main_batch.add("NR", "48 8B 03 48 8B D6 48 8B CB EB 06", [this](memory::handle ptr)
			{
				m_node_read = ptr.sub(0x48).as<functions::node_read_t>();
			});
		main_batch.add("GO", "8A 41 49 3C FF 74 17 3C 20 73 13 0F B6 C8", [this](memory::handle ptr)
			{
				m_get_owner = ptr.as<functions::get_owner_t>();
			});
		main_batch.add("GNO", "44 38 33 75 30 66 44", [this](memory::handle ptr)
			{
				m_get_network_object = ptr.sub(0x40).as<functions::get_network_object_t>();
			});
		main_batch.add("GSTFT", "0F B7 CA 83 F9 07", [this](memory::handle ptr)
			{
				m_get_sync_tree_from_type = ptr.as<functions::get_sync_tree_from_type_t>();
			});
		main_batch.add("GNO", "44 38 33 75 30 66 44", [this](memory::handle ptr)
			{
				m_get_network_object = ptr.sub(0x40).as<functions::get_network_object_t>();
			});
		main_batch.add("GNOFP", "41 80 78 ? FF 74 2D 41 0F B6 40", [this](memory::handle ptr)
			{
				m_get_network_object_for_player = ptr.as<functions::get_network_object_for_player_t>();
			});
		main_batch.add("NOM", "48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 33 FF 4C 8B F0", [this](memory::handle ptr)
			{
				_m_network_object_manager = ptr.add(3).rip().as<CNetworkObjectMgr**>();
			});
		main_batch.add("RI", "48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D ? ? ? ?", [this](memory::handle ptr)
			{
				uint8_t* replay_interface_pattern = ptr.as<uint8_t*>();
				uint8_t* replay_interface_address = replay_interface_pattern + *(uint32_t*)(replay_interface_pattern + 0x3) + 0x7;
				m_replay = *(rage::CReplayInterface**)replay_interface_address;
			});
		main_batch.add("CP", "48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 24 ? 57 48 83 ec ? 48 8b f9 48 8b ca 49 8b e9", [this](memory::handle ptr)
			{
				m_pack_clone_create = ptr.as<functions::pack_clone_create_t>();
			});
		main_batch.add("CC", "48 8b c4 66 44 89 48", [this](memory::handle ptr)
			{
				m_clone_create = ptr.as<functions::clone_create_t>();
			});
		main_batch.add("CS", "4C 8B FA 41 0F B7 D1", [this](memory::handle ptr)
			{
				m_clone_sync = ptr.sub(0x1D).as<functions::clone_sync_t>();
			});
		main_batch.add("CR", "48 8b c4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 54 41 56 41 57 48 83 ec ? 4c 8b f2 4d 8b e0 48 8d 50", [this](memory::handle ptr)
			{
				m_clone_remove = ptr.as<functions::clone_remove_t>();
			});
		main_batch.add("SR", "45 89 43 18 57 48 83 EC 30 48 83 79 10 00 49", [this](memory::handle ptr)
			{
				m_sync_read = ptr.sub(15).as<functions::sync_read_t>();
			});
		main_batch.add("UO", "48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 ec ? 45 8a f1 41 8b e8 48 8b fa", [this](memory::handle ptr)
			{
				m_unregister_object = ptr.as<functions::unregister_object_t>();
			});
		main_batch.add("CCA", "48 8b c4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 54 41 56 41 57 48 83 ec ? 4c 8b fa 49 8b d8", [this](memory::handle ptr)
			{
				m_clone_create_ack = ptr.as<functions::clone_create_ack_t>();
			});
		main_batch.add("CSA", "48 89 5c 24 ? 48 89 74 24 ? 48 89 7c 24 ? 41 54 41 56 41 57 48 83 ec ? 4c 8b e2", [this](memory::handle ptr)
			{
				m_clone_sync_ack = ptr.as<functions::clone_sync_ack_t>();
			});
		main_batch.add("CRA", "48 8b c4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 56 41 57 48 83 ec ? 4c 8b fa 49 8b d8", [this](memory::handle ptr)
			{
				m_clone_remove_ack = ptr.as<functions::clone_remove_ack_t>();
			});
		main_batch.add("SCS", "E8 ? ? ? ? 8B 44 24 5C 44 0F B7 4C 24 ?", [this](memory::handle ptr)
			{
				m_send_clone_sync = ptr.add(1).rip().as<functions::send_clone_sync_t>();
			});
		main_batch.add("SCS", "48 83 EC 30 41 80 78 ? ?", [this](memory::handle ptr)
			{
				m_pack_clone_remove = ptr.sub(16).as<functions::pack_clone_remove_t>();
			});

		main_batch.add("WBQ", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 41 8B F0 48 8B EA 48 8B D9 41 83 F8 20", [this](memory::handle ptr)
			{
				write_bit_buffer_qword = ptr.as<functions::uWriteBitBufferQword>();
			});
		main_batch.add("WBD", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 8B EA BF 01 ? ? ?", [this](memory::handle ptr)
			{
				write_bit_buffer_dword = ptr.as<functions::uWriteBitBufferDword>();
			});
		main_batch.add("GMI", "0F B7 05 ? ? ? ? 45 33 C9 4C 8B DA 66 85 C0 0F 84 ? ? ? ? 44 0F B7 C0 33 D2 8B C1 41 F7 F0 48 8B 05 ? ? ? ? 4C 8B 14 D0 EB 09 41 3B 0A 74 54", [this](memory::handle ptr)
			{
				m_get_model_info = ptr.as<functions::get_model_info_t>();
			});
		main_batch.add("NTH", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 30 48 8B 05 ? ? ? ? 48 8B D9", [this](memory::handle ptr)
			{
				m_net_array_handler = ptr.as<functions::net_array_handler_t>();
			});
		main_batch.add("SPM", "48 89 5C 24 ? 55 48 8B EC 48 83 EC 20 8B 45 10 8B 89 ? ? ? ? BB ? ? ? ? 0D ? ? ? ? 48 8D 55 20 C7 45 ? ? ? ? ? 23 C3 25 ? ? ? ? 89 45 10 E8 ? ? ? ? 0F B7 45 20 66 89 45 10 8B 45 10 0D ? ? ? ? 23 C3 32 DB 89 45 10 66 3D FF FF 75 0E E8 ? ? ? ? 4C 8D 05 ? ? ? ? EB 4F 48 8D 4D 10 E8 ? ? ? ? 66 89 45 20 8B 45 20 8B C8 33 4D 10 81 E1 ? ? ? ? 33 C1 0F BA F0 1D 8B C8 33 4D 10 81 E1 ? ? ? ? 33 C1 48 8D 4D 20 25 ? ? ? ? 89 45 20 E8 ? ? ? ? 3A C3 75 1F E8 ? ? ? ? 4C 8D 05 ? ? ? ? 4C 8B 08 48 8D 15 ? ? ? ? 48 8B C8 41 FF 51 30 EB 02 B3 01 8A C3 48 8B 5C 24 ? 48 83 C4 20 5D C3 90", [this](memory::handle ptr)
			{
				m_sync_player_model = ptr.as<functions::sync_player_model_t>();
			});
		main_batch.add("GCD", "4D 85 C9 0F 84 ? ? ? ? 48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 48 20 55 41 54 41 55 41 56 41 57 48 8D A8", [this](memory::handle ptr)
			{
				get_chat_data = ptr.as<decltype(get_chat_data)>();
			});

		main_batch.add("GCP", "48 8B D1 48 8B 0D ? ? ? ? 41 B0 01 E9", [this](memory::handle ptr)
			{
				get_chat_player = ptr.as<decltype(get_chat_player)>();
			});

		main_batch.add("TSE", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41 8B F9", [this](memory::handle ptr)
			{
				m_trigger_script_event = ptr.as<decltype(m_trigger_script_event)>();
			});
		main_batch.add("RP", "48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 E8 ? ? ? ? 84 C0 75 1F", [this](memory::handle ptr)
			{
				m_remove_player = ptr.as<functions::remove_player_t>();
			});

		main_batch.add("NPM", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF", [this](memory::handle ptr)
			{
				m_network_player_mgr = ptr.add(3).rip().as<rage::CNetworkPlayerMgr**>();
			});
		main_batch.add("NPM", "33 C0 48 89 41 70 48 ", [this](memory::handle ptr)
			{
				m_sub_7FF7AA059CC4 = ptr.as<functions::sub_7FF7AA059CC4_t>();
			});
		main_batch.run(memory::module(nullptr));
		m_hwnd = FindWindowW(L"grcWindow", nullptr);
		if (!m_hwnd)
			throw std::runtime_error("Failed to find the game's window.");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}
}