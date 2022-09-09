#include "pch.h"
#include "logger.h"
#include "hooking.h"
#include "memory/module.h"
#include "pointers.h"
#include "MinHook.h"
#include "script.h"
#include"script_mgr.h"
#include"renderer.h"
#include"keyboard.h"
#include"ui/ui_manager.h"

namespace hbase
{
	hooking::hooking() :
		m_swapchainHook(*g_pointers->m_swapchain, hooks::swapchainNumFuncs),
		m_setCursorPosHook("SCP", memory::module("user32.dll").get_export("SetCursorPos").as<void*>(), &hooks::setCursorPos),
		m_runScriptThreadsHook("RST", g_pointers->m_run_script_threads, &hooks::runScriptThreads),
		m_convertThreadToFiberHook("CTTF", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>(), &hooks::convertThreadToFiber),
		m_gta_thread_tick_hook("GTT", g_pointers->m_gta_thread_tick, &hooks::gta_thread_tick),
		m_gta_thread_kill_hook("GTK", g_pointers->m_gta_thread_kill, &hooks::gta_thread_kill),
		m_scripted_game_event_hook("SGE", g_pointers->m_scripted_game_event, &hooks::scripted_game_event),
		m_received_event_hook("RE", g_pointers->m_received_event, &hooks::received_event),
		m_get_friend_rid_hook("GFR", *g_pointers->m_get_friend_rid, &hooks::get_friend_rid),
		m_rid_joiner_hook("RJ", *g_pointers->m_rid_joiner, &hooks::rid_joiner),
		m_is_dlc_present_hook("IDP", g_pointers->m_is_dlc_present, &hooks::is_dlc_present),
		m_error_screen_hook("ES", g_pointers->m_error_screen, &hooks::set_warning_message_with_header),
		m_send_net_info_to_lobby("SNITL", g_pointers->m_send_net_info_to_lobby, &hooks::send_net_info_to_lobby),
		m_increment_stat_hook("ISE", g_pointers->m_increment_stat_event, &hooks::increment_stat_event),
		m_force_script_host_hook("FSH", g_pointers->m_forceScriptHost, &hooks::force_script_host),
		m_player_has_joined_hook("PHJ", g_pointers->m_player_has_joined, &hooks::player_join),
		m_player_has_left_hook("PHL", g_pointers->m_player_has_left, &hooks::player_leave),
		can_apply_to_object_hook("NSTCATO", g_pointers->m_netSyncTree_CanApplyToObject, &hooks::can_apply_to_object),
		m_clone_create_hook("CC", g_pointers->m_clone_create, &hooks::clone_create),
		m_clone_sync_hook("CS", g_pointers->m_clone_sync, &hooks::clone_sync),
		m_clone_remove_hook("CR", g_pointers->m_clone_remove, &hooks::clone_remove),
		m_sync_read_hook("SR", g_pointers->m_sync_read, &hooks::sync_read),
		m_unregister_object_hook("UO", g_pointers->m_unregister_object, &hooks::unregister_object),
		m_clone_create_ack_hook("CCA", g_pointers->m_clone_create_ack, &hooks::clone_create_ack),
		m_clone_sync_ack_hook("CSA", g_pointers->m_clone_sync_ack, &hooks::clone_sync_ack),
		m_clone_remove_ack_hook("CRA", g_pointers->m_clone_remove_ack, &hooks::clone_remove_ack),
		m_pack_clone_create_hook("PCC", g_pointers->m_pack_clone_create, &hooks::pack_clone_create),
		m_send_clone_sync_hook("SCS", g_pointers->m_send_clone_sync, &hooks::send_clone_sync),
		m_pack_clone_remove_hook("PCR", g_pointers->m_pack_clone_remove, &hooks::pack_clone_remove),
		m_write_bitbuffer_dword_hook("WBD", g_pointers->write_bit_buffer_dword, &hooks::write_bitbuffer_dword),
		m_net_array_handle_hook("NAH", g_pointers->m_net_array_handler, &hooks::net_array_handler),
		m_sync_player_model_hook("SPM", g_pointers->m_sync_player_model, &hooks::sync_player_model),
		m_get_chat_data_hook("GCD", g_pointers->get_chat_data, &hooks::get_chat_data),
		m_sub_7FF7AA059CC4_hook("UNK1", g_pointers->m_sub_7FF7AA059CC4, &hooks::m_sub_7FF7AA059CC4)
	{
		m_swapchainHook.hook(hooks::swapchainPresentIndex, &hooks::swapchainPresent);
		m_swapchainHook.hook(hooks::swapchainResizebuffersIndex, &hooks::swapchainResizebuffers);

		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		m_swapchainHook.enable();

		g_keyBoard->keyboardHandlerRegister(OnKeyboardMessage);
		m_OgWndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));

		m_setCursorPosHook.enable();
		m_runScriptThreadsHook.enable();
		m_convertThreadToFiberHook.enable();

		m_gta_thread_tick_hook.enable();
		m_gta_thread_kill_hook.enable();

		m_scripted_game_event_hook.enable();
		m_received_event_hook.enable();

		m_get_friend_rid_hook.enable();
		m_rid_joiner_hook.enable();

		m_is_dlc_present_hook.enable();

		m_error_screen_hook.enable();
		m_send_net_info_to_lobby.enable();
		m_increment_stat_hook.enable();

		m_force_script_host_hook.enable();

		can_apply_to_object_hook.enable();

		m_player_has_joined_hook.enable();
		m_player_has_left_hook.enable();

		m_clone_create_hook.enable();
		m_clone_sync_hook.enable();
		m_clone_remove_hook.enable();
		m_pack_clone_create_hook.enable();

		m_unregister_object_hook.enable();
		m_clone_create_ack_hook.enable();
		m_clone_sync_ack_hook.enable();
		m_clone_remove_ack_hook.enable();

		m_send_clone_sync_hook.enable();
		m_pack_clone_remove_hook.enable();

		m_write_bitbuffer_dword_hook.enable();

		m_sync_player_model_hook.enable();

		m_net_array_handle_hook.enable();

		m_get_chat_data_hook.enable();

		m_sub_7FF7AA059CC4_hook.enable();

		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;
		m_swapchainHook.disable();
		m_convertThreadToFiberHook.disable();
		m_runScriptThreadsHook.disable();
		m_setCursorPosHook.disable();

		m_gta_thread_tick_hook.disable();
		m_gta_thread_kill_hook.disable();

		m_scripted_game_event_hook.disable();
		m_received_event_hook.disable();

		m_get_friend_rid_hook.disable();
		m_rid_joiner_hook.disable();

		m_is_dlc_present_hook.disable();

		m_error_screen_hook.disable();
		m_send_net_info_to_lobby.disable();
		m_increment_stat_hook.disable();

		m_force_script_host_hook.disable();

		can_apply_to_object_hook.disable();

		m_player_has_joined_hook.disable();
		m_player_has_left_hook.disable();

		m_clone_create_hook.disable();
		m_clone_sync_hook.disable();
		m_clone_remove_hook.disable();
		m_pack_clone_create_hook.disable();

		m_unregister_object_hook.disable();
		m_clone_create_ack_hook.disable();
		m_clone_sync_ack_hook.disable();
		m_clone_remove_ack_hook.disable();

		m_send_clone_sync_hook.disable();
		m_pack_clone_remove_hook.disable();

		m_write_bitbuffer_dword_hook.disable();

		m_sync_player_model_hook.disable();

		m_net_array_handle_hook.disable();

		m_get_chat_data_hook.disable();

		m_sub_7FF7AA059CC4_hook.disable();

		g_keyBoard->keyboardHandlerUnregister(OnKeyboardMessage);
		SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_OgWndproc));
	}

	bool hooks::runScriptThreads(std::uint32_t ops_to_execute)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_script_mgr.tick();
			}

			return g_hooking->m_runScriptThreadsHook.get_original<functions::run_script_threads_t>()(ops_to_execute);
		} EXCEPT_CLAUSE
			return false;
	}

	void* hooks::convertThreadToFiber(void* param)
	{
		TRY_CLAUSE
		{
			if (IsThreadAFiber())
			{
				return GetCurrentFiber();
			}

			return g_hooking->m_convertThreadToFiberHook.get_original<decltype(&convertThreadToFiber)>()(param);
		} EXCEPT_CLAUSE
			return nullptr;
	}

	HRESULT hooks::swapchainPresent(IDXGISwapChain* this_, UINT sync_interval, UINT flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->on_present();
			}

			return g_hooking->m_swapchainHook.get_original<decltype(&swapchainPresent)>(swapchainPresentIndex)(this_, sync_interval, flags);
		} EXCEPT_CLAUSE
			return NULL;
	}

	HRESULT hooks::swapchainResizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->pre_reset();

				auto result = g_hooking->m_swapchainHook.get_original<decltype(&swapchainResizebuffers)>(swapchainResizebuffersIndex)
					(this_, buffer_count, width, height, new_format, swapchain_flags);

				if (SUCCEEDED(result))
				{
					g_renderer->post_reset();
				}

				return result;
			}

			return g_hooking->m_swapchainHook.get_original<decltype(&swapchainResizebuffers)>(swapchainResizebuffersIndex)
				(this_, buffer_count, width, height, new_format, swapchain_flags);
		} EXCEPT_CLAUSE
			return NULL;
	}

	LRESULT hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->wnd_proc(hwnd, msg, wparam, lparam);
			}

			return CallWindowProcW(g_hooking->m_OgWndproc, hwnd, msg, wparam, lparam);
		} EXCEPT_CLAUSE
			return NULL;
	}

	BOOL hooks::setCursorPos(int x, int y)
	{
		TRY_CLAUSE
		{
			if (g_ui_mgr->m_opened)
				return true;

			return g_hooking->m_setCursorPosHook.get_original<decltype(&setCursorPos)>()(x, y);
		} EXCEPT_CLAUSE
			return FALSE;
	}

	minhookKeepalive::minhookKeepalive()
	{
		MH_Initialize();
	}

	minhookKeepalive::~minhookKeepalive()
	{
		MH_Uninitialize();
	}
}