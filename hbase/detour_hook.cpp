#include "pch.h"
#include "detour_hook.h"
#include "logger.h"
#include "memory/handle.h"
#include<MinHook.h>

namespace hbase
{
	detour_hook::detour_hook(std::string name, void* target, void* detour) :
		m_name(std::move(name)),
		m_target(target),
		m_detour(detour)
	{
		fixHookAddress();

		if (MH_STATUS status = MH_CreateHook(m_target, m_detour, &m_original); status != MH_OK)
		{
			throw std::runtime_error(fmt::format("Failed to create hook '{}' at 0x{:X} (error: {})", m_name, reinterpret_cast<std::uintptr_t>(m_target), MH_StatusToString(status)));
		}
	}

	detour_hook::~detour_hook() noexcept
	{
		if (m_target)
		{
			MH_RemoveHook(m_target);
		}
	}

	void detour_hook::enable()
	{
		if (auto status = MH_EnableHook(m_target); status == MH_OK)
		{
			LOG(INFO) << "Hook " << m_name;
		}
		else
		{
			throw std::runtime_error(fmt::format("Failed to enable hook 0x{:X} ({})", reinterpret_cast<std::uintptr_t>(m_target), MH_StatusToString(status)));
		}
	}

	void detour_hook::disable()
	{
		if (auto status = MH_DisableHook(m_target); status == MH_OK)
		{
			LOG(INFO) << "UnHook " << m_name;
		}
		else
		{
			LOG(WARNING) << "Failed to disable hook '" << m_name << "'.";
		}
	}

	DWORD exp_handler(PEXCEPTION_POINTERS exp, std::string const& name)
	{
		return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION
			? EXCEPTION_EXECUTE_HANDLER
			: EXCEPTION_CONTINUE_SEARCH;
	}

	void detour_hook::fixHookAddress()
	{
		__try
		{
			auto ptr = memory::handle(m_target);
			while (ptr.as<std::uint8_t&>() == 0xE9)
			{
				ptr = ptr.add(1).rip();
			}

			m_target = ptr.as<void*>();
		}
		__except (exp_handler(GetExceptionInformation(), m_name))
		{
			[this]()
			{
				throw std::runtime_error(fmt::format("Failed to fix hook address for '{}'", m_name));
			}();
		}
	}
}