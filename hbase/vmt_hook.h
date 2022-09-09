#pragma once
#include "pch.h"

namespace hbase
{
	class VmtHook
	{
	public:
		explicit VmtHook(void* obj, std::size_t num_funcs);

		VmtHook(VmtHook&& that) = delete;
		VmtHook& operator=(VmtHook&& that) = delete;
		VmtHook(VmtHook const&) = delete;
		VmtHook& operator=(VmtHook const&) = delete;

		void hook(std::size_t index, void* func);
		void unhook(std::size_t index);

		template <typename T>
		T get_original(std::size_t index);

		void enable();
		void disable();
	private:
		void*** m_object;
		std::size_t m_numFuncs;

		void** m_originalTable;
		std::unique_ptr<void* []> m_newTable;
	};

	template<typename T>
	inline T VmtHook::get_original(std::size_t index)
	{
		return static_cast<T>(m_originalTable[index]);
	}
}