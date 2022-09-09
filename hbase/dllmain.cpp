﻿#include "pch.h"
#include"logger.h"
#include "pointers.h"
#include "hooking.h"
#include "renderer.h"
#include "fiber_pool.h"
#include "script.h"
#include"script_mgr.h"
#include"features.h"
#include"keyboard.h"
#include"ui/ui_manager.h"
#include"settings.h"
#include"ui_settings.h"
#include"data/player.h"
#include <Psapi.h>

using namespace hbase;
LPVOID baseAddress;
size_t moduleSize;

LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	PVOID ExceptionAddress = ExceptionInfo->ExceptionRecord->ExceptionAddress;
	PVOID ExceptionOffset = (PVOID)((DWORD_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress - (DWORD_PTR)baseAddress);

	std::ofstream repfile(MENU_PATH"\\crash_info.txt");
	repfile << "Crash report" << std::endl << std::endl;
	repfile << "ExceptionAddress: " << ExceptionAddress << std::endl;
	repfile << "Rip: " << (PVOID)ExceptionInfo->ContextRecord->Rip << std::endl;
	repfile << "ExceptionAddress (offset): " << ExceptionOffset << std::endl;
	repfile << "Last script event hash received: " << (int)g_settings.lastScriptEventHashReceived << std::endl;
	repfile << "Last sync model hash received: " << (int)g_settings.lastSyncModelHashReceived << std::endl;
	repfile.close();

	return EXCEPTION_CONTINUE_SEARCH;
}



DWORD run(LPVOID)
{
	MODULEINFO module_info;
	if (K32GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &module_info, sizeof(MODULEINFO)) == 0)
	{
		return 0;
	}

	baseAddress = module_info.lpBaseOfDll;
	moduleSize = module_info.SizeOfImage;

	auto logger_instance = std::make_unique<logger>();
	LOG(INFO) << "innit Logger";
	LOG(INFO) << u8R"kek(
  █     █░ ▄▄▄▄    ██▀███  ▓█████ ▄▄▄       ██ ▄█▀▓█████  ██▀███   █     █░▄▄▄█████▓ ▄▄▄       ██ ▄█▀▓█████  ██▀███  
▓█░ █ ░█░▓█████▄ ▓██ ▒ ██▒▓█   ▀▒████▄     ██▄█▒ ▓█   ▀ ▓██ ▒ ██▒▓█░ █ ░█░▓  ██▒ ▓▒▒████▄     ██▄█▒ ▓█   ▀ ▓██ ▒ ██▒
▒█░ █ ░█ ▒██▒ ▄██▓██ ░▄█ ▒▒███  ▒██  ▀█▄  ▓███▄░ ▒███   ▓██ ░▄█ ▒▒█░ █ ░█ ▒ ▓██░ ▒░▒██  ▀█▄  ▓███▄░ ▒███   ▓██ ░▄█ ▒
░█░ █ ░█ ▒██░█▀  ▒██▀▀█▄  ▒▓█  ▄░██▄▄▄▄██ ▓██ █▄ ▒▓█  ▄ ▒██▀▀█▄  ░█░ █ ░█ ░ ▓██▓ ░ ░██▄▄▄▄██ ▓██ █▄ ▒▓█  ▄ ▒██▀▀█▄  
░░██▒██▓ ░▓█  ▀█▓░██▓ ▒██▒░▒████▒▓█   ▓██▒▒██▒ █▄░▒████▒░██▓ ▒██▒░░██▒██▓   ▒██▒ ░  ▓█   ▓██▒▒██▒ █▄░▒████▒░██▓ ▒██▒
░ ▓░▒ ▒  ░▒▓███▀▒░ ▒▓ ░▒▓░░░ ▒░ ░▒▒   ▓▒█░▒ ▒▒ ▓▒░░ ▒░ ░░ ▒▓ ░▒▓░░ ▓░▒ ▒    ▒ ░░    ▒▒   ▓▒█░▒ ▒▒ ▓▒░░ ▒░ ░░ ▒▓ ░▒▓░
  ▒ ░ ░  ▒░▒   ░   ░▒ ░ ▒░ ░ ░  ░ ▒   ▒▒ ░░ ░▒ ▒░ ░ ░  ░  ░▒ ░ ▒░  ▒ ░ ░      ░      ▒   ▒▒ ░░ ░▒ ▒░ ░ ░  ░  ░▒ ░ ▒░
  ░   ░   ░    ░   ░░   ░    ░    ░   ▒   ░ ░░ ░    ░     ░░   ░   ░   ░    ░        ░   ▒   ░ ░░ ░    ░     ░░   ░ 
    ░     ░         ░        ░  ░     ░  ░░  ░      ░  ░   ░         ░                   ░  ░░  ░      ░  ░   ░     
               ░                                                                                                    
                                               
)kek";
		try
		{
			g_ui_settings.load();
			auto pointers_instance = std::make_unique<pointers>();
			LOG(INFO) << "innit Pointers";

			auto renderer_instance = std::make_unique<renderer>();
			LOG(INFO) << "innit Renderer";

			auto ui_manager_instance = std::make_unique<ui_mgr>();
			LOG(INFO) << "innit Ui Manager";

			auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
			LOG(INFO) << "innit Fiber Pool";

			auto keyboard_instance = std::make_unique<keyboard>();
			LOG(INFO) << "innit KeyBoard";

			auto hooking_instance = std::make_unique<hooking>();
			LOG(INFO) << "innit Hooks";

			g_script_mgr.add_script(std::make_unique<script>(&features::script_func));
			g_script_mgr.add_script(std::make_unique<script>(&player::player_script));

			g_settings.load();

			g_hooking->enable();

			AddVectoredExceptionHandler(true, ExceptionHandler);

			while (g_running)
			{
				if (GetAsyncKeyState(VK_DELETE)) {
					g_running = false;
				}
				std::this_thread::sleep_for(3ms);
				std::this_thread::yield();
			}

			g_settings.save();
			g_ui_settings.save();

			g_hooking->disable();

			g_script_mgr.remove_all_scripts();

			hooking_instance.reset();
			LOG(INFO) << "reset Hooking";

			keyboard_instance.reset();
			LOG(INFO) << "reset keyboard";

			fiber_pool_instance.reset();
			LOG(INFO) << "reset fiber pool";

			ui_manager_instance.reset();
			LOG(INFO) << "reset ui management";

			renderer_instance.reset();
			LOG(INFO) << "reset Renderer instance";

			pointers_instance.reset();
			LOG(INFO) << "reset pointers";
		}
		catch (const std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
		}
	
	logger_instance.reset();
	LOG(INFO) << "reset g3log";

	CloseHandle(g_mainThread);
	FreeLibraryAndExitThread(g_hmodule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		g_hmodule = hModule;
		g_mainThread = CreateThread(nullptr, 0, run, nullptr, 0, &g_mainThreadId);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		g_settings.save();
	}

	return true;
}