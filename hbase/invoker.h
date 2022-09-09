#pragma once
#include "pch.h"
#include "gta/natives.h"

namespace hbase
{
	class NativeCallContext : public rage::scrNativeCallContext
	{
	public:
		NativeCallContext();
	private:
		std::uint64_t m_return_stack[10];
		std::uint64_t m_arg_stack[100];
	};

	class NativeInvoker
	{
	public:
		explicit NativeInvoker() = default;
		~NativeInvoker() = default;

		void cacheHandlers();

		void beginCall();
		void endCall(rage::scrNativeHash hash);

		template <typename T>
		void pushArg(T&& value)
		{
			m_call_context.push_arg(std::forward<T>(value));
		}

		template <typename T>
		T& getReturnValue()
		{
			return *m_call_context.get_return_value<T>();
		}
	private:
		NativeCallContext m_call_context;
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> m_handlerCache;
	};

	inline NativeInvoker g_nativeInvoker;
}