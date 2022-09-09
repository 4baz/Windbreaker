#pragma once
#include"pointers.h"

namespace hbase
{
	namespace memory
	{
		inline uintptr_t get_multilayer_pointer(uintptr_t base_address, std::vector<DWORD> offsets)
		{
			uintptr_t ptr = *(uintptr_t*)(base_address);
			if (!ptr) {
				return NULL;
			}
			auto level = offsets.size();

			for (auto i = 0; i < level; i++)
			{
				if (i == level - 1)
				{
					ptr += offsets[i];
					if (!ptr) return NULL;
					return ptr;
				}
				else
				{
					ptr = *(uint64_t*)(ptr + offsets[i]);
					if (!ptr) return NULL;
				}
			}

			return ptr;
		}

		template <typename T>
		inline void set_value(std::vector<DWORD> offsets, T value) {
			uintptr_t Addr = get_multilayer_pointer((uintptr_t)g_pointers->m_ped_factory, offsets);
			if (Addr == NULL) {
				return;
			}

			*reinterpret_cast<T*>(Addr) = value;
		}

		template <typename T>
		inline T get_value(std::vector<DWORD> offsets) {
			uintptr_t Addr = get_multilayer_pointer((uintptr_t)g_pointers->m_ped_factory, offsets);
			if (Addr == NULL) {
				return NULL;
			}

			return *((T*)Addr);
		}

		template <typename T>
		inline void set_self_value(uintptr_t base_address, std::vector<DWORD> offsets, T value) {
			uintptr_t Addr = get_multilayer_pointer(base_address, offsets);
			if (Addr == NULL) {
				return;
			}

			*reinterpret_cast<T*>(Addr) = value;
		}

		template <typename T>
		inline T get_self_value(uintptr_t base_address, std::vector<DWORD> offsets) {
			uintptr_t Addr_ = get_multilayer_pointer(base_address, offsets);
			if (Addr_ == NULL) {
				return NULL;
			}

			return *((T*)Addr_);
		}
	}
}