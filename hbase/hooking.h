#pragma once
#include "pch.h"
#include "detour_hook.h"
#include "vmt_hook.h"
#include "gta/script_thread.h"
#include"gta/net_game_event.h"
#include"gta/player.h"

namespace hbase
{
	struct hooks
	{
		static bool runScriptThreads(std::uint32_t ops_to_execute);
		static void* convertThreadToFiber(void* param);

		static constexpr auto swapchainNumFuncs = 19;
		static constexpr auto swapchainPresentIndex = 8;
		static constexpr auto swapchainResizebuffersIndex = 13;
		static HRESULT swapchainPresent(IDXGISwapChain* this_, UINT sync_interval, UINT flags);
		static HRESULT swapchainResizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags);

		static LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static BOOL setCursorPos(int x, int y);

		static rage::eThreadState gta_thread_kill(GtaThread* thread);
		static rage::eThreadState gta_thread_tick(GtaThread* thread, unsigned int a2);

		static bool scripted_game_event(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player);
		static bool received_event(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, uint16_t event_id, int event_index, int event_handled_bitset, int64_t bit_buffer_size, int64_t bit_buffer);

		static int64_t* get_friend_rid(int32_t friendIndex);
		static BOOL rid_joiner(int64_t* a1, int* hash);

		static bool is_dlc_present(Hash dlc_hash);

		static bool increment_stat_event(CNetworkIncrementStatEvent* net_event_struct, CNetGamePlayer* sender, int64_t a3);

		static bool send_net_info_to_lobby(rage::netPlayerData* player, int64_t a2, int64_t a3, DWORD* a4);

		static void set_warning_message_with_header(const char* entryHeader, const char* entryLine1, int instructionalKey, const char* entryLine2, bool p4, Any p5, Any* showBackground, Any* p7, bool p8, Any p9);

		static char force_script_host(__int64 pCGameScriptHandlerNetComponenet, CNetGamePlayer* player, unsigned __int16 a3, char a4);

		static void player_join(CNetworkObjectMgr* _this, CNetGamePlayer* net_player);
		static void player_leave(CNetworkObjectMgr* _this, CNetGamePlayer* net_player);

		static bool can_apply_to_object(rage::netSyncTree* self, rage::netObject* obj);

		static bool clone_create(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t _object_type, int32_t _object_id, int32_t _object_flag, rage::datBitBuffer* buffer, int32_t timestamp);
		static bool clone_sync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t _object_type, int32_t _object_id, rage::datBitBuffer* buffer, int32_t unk, int32_t timestamp);
		static void clone_remove(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, int32_t _object_id, int32_t unk);
		static bool sync_read(rage::netSyncTree* netSyncTree, int32_t sync_type, int32_t _sync_flag, rage::datBitBuffer* buffer, void* netLogStub);

		static void clone_create_ack(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, uint16_t object_id, int32_t ack_code);
		static void clone_sync_ack(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, rage::datBitBuffer* buffer, uint16_t object_id, int32_t ack_code);
		static void clone_remove_ack(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, uint16_t object_id, int32_t ack_code);
		static void unregister_object(CNetworkObjectMgr* mgr, rage::netObject* netObject, int reason, bool force1, bool force2);

		static void pack_clone_create(CNetworkObjectMgr* mgr, rage::netObject* netObject, CNetGamePlayer* src, rage::datBitBuffer* buffer);
		static void send_clone_sync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, rage::netObject* netObject, __int64 a4, uint16_t* a5, bool a6);
		static bool pack_clone_remove(CNetworkObjectMgr* mgr, rage::netObject* netObject, CNetGamePlayer* src, bool a4);

		static bool write_bitbuffer_dword(__int64 bit_buffer, int value, int bits);

		static char sync_player_model(__int64 a1, __int64 a2);

		static __int64 __cdecl get_chat_data(__int64 a1, __int64 a2, __int64 a3, const char* origText, BOOL isTeam);

		static char net_array_handler(int64_t this_, CNetGamePlayer* player, rage::datBitBuffer* buffer, int size, int16_t id);

		static __int64 m_sub_7FF7AA059CC4(rage::netObject* netObject);
	};

	struct minhookKeepalive
	{
		minhookKeepalive();
		~minhookKeepalive();
	};

	class hooking
	{
		friend hooks;
	public:
		explicit hooking();
		~hooking();

		void enable();
		void disable();

	private:
		bool m_enabled{};
		minhookKeepalive m_minhookKeepalive;

		VmtHook m_swapchainHook;
		WNDPROC m_OgWndproc;
		detour_hook m_setCursorPosHook;

		detour_hook m_runScriptThreadsHook;
		detour_hook m_convertThreadToFiberHook;

		//detour_hook m_gta_thread_start_hook;
		detour_hook m_gta_thread_tick_hook;
		detour_hook m_gta_thread_kill_hook;

		detour_hook m_scripted_game_event_hook;

		detour_hook m_received_event_hook;

		detour_hook m_get_friend_rid_hook;
		detour_hook m_rid_joiner_hook;

		detour_hook m_is_dlc_present_hook;

		detour_hook m_increment_stat_hook;

		detour_hook m_send_net_info_to_lobby;

		detour_hook m_error_screen_hook;

		detour_hook m_force_script_host_hook;

		detour_hook m_player_has_joined_hook;
		detour_hook m_player_has_left_hook;

		detour_hook m_clone_create_hook;
		detour_hook m_clone_sync_hook;
		detour_hook m_clone_remove_hook;
		detour_hook m_sync_read_hook;
		detour_hook can_apply_to_object_hook;

		detour_hook m_unregister_object_hook;
		detour_hook m_clone_create_ack_hook;
		detour_hook m_clone_sync_ack_hook;
		detour_hook m_clone_remove_ack_hook;

		detour_hook m_pack_clone_create_hook;
		detour_hook m_send_clone_sync_hook;
		detour_hook m_pack_clone_remove_hook;

		detour_hook m_write_bitbuffer_dword_hook;

		detour_hook m_net_array_handle_hook;

		detour_hook m_sync_player_model_hook;

		detour_hook m_get_chat_data_hook;
		detour_hook m_sub_7FF7AA059CC4_hook;
	};

	inline hooking* g_hooking{};
}