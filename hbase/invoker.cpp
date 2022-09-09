#include "pch.h"
#include "crossmap.h"
#include "invoker.h"
#include "logger.h"
#include "pointers.h"

extern "C" void	_call_asm(void* context, void* function, void* ret);
namespace hbase
{
	NativeCallContext::NativeCallContext()
	{
		m_return_value = &m_return_stack[0];
		m_args = &m_arg_stack[0];
	}

	void NativeInvoker::cacheHandlers()
	{
		for (const rage::scrNativeMapping& mapping : g_crossmap)
		{
			rage::scrNativeHandler handler = g_pointers->m_get_native_handler(
				g_pointers->m_native_registration_table, mapping.second);

			m_handlerCache.emplace(mapping.first, handler);
		}
	}

	void NativeInvoker::beginCall()
	{
		m_call_context.reset();
	}

	void NativeInvoker::endCall(rage::scrNativeHash hash)
	{
		if (auto it = m_handlerCache.find(hash); it != m_handlerCache.end())
		{
			rage::scrNativeHandler handler = it->second;

			_call_asm(&m_call_context, handler, g_pointers->m_native_return);
			//handler(&m_call_context);
			g_pointers->m_fix_vectors(&m_call_context);
		}
		else
		{
			[hash]() { LOG(WARNING) << "Failed to find " << HEX_TO_UPPER(hash) << " native's handler."; }();
		}
	}
}