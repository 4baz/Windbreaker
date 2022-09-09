#include "pch.h"
#include "vmt_hook.h"

namespace hbase
{
	VmtHook::VmtHook(void* obj, std::size_t num_funcs) :
		m_object(static_cast<void***>(obj)),
		m_numFuncs(num_funcs + 1),
		m_originalTable(*m_object),
		m_newTable(std::make_unique<void* []>(m_numFuncs))
	{
		std::copy_n(m_originalTable - 1, m_numFuncs, m_newTable.get());
	}

	void VmtHook::hook(std::size_t index, void* func)
	{
		m_newTable[index + 1] = func;
	}

	void VmtHook::unhook(std::size_t index)
	{
		m_newTable[index + 1] = m_originalTable[index];
	}

	void VmtHook::enable()
	{
		*m_object = m_newTable.get() + 1;
	}

	void VmtHook::disable()
	{
		*m_object = m_originalTable;
	}
}