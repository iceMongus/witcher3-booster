/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#include "Hooking.h"

namespace hook
{

	uintptr_t baseAddress;

#ifndef _M_AMD64
	void inject_hook::inject()
	{
		inject_hook_frontend fe(this);
		m_assembly = std::make_shared<FunctionAssembly>(fe);

		put<uint8_t>(m_address, 0xE9);
		put<int>(m_address + 1, (uintptr_t)m_assembly->GetCode() - (uintptr_t)get_adjusted(m_address) - 5);
	}

	void inject_hook::injectCall()
	{
		inject_hook_frontend fe(this);
		m_assembly = std::make_shared<FunctionAssembly>(fe);

		put<uint8_t>(m_address, 0xE8);
		put<int>(m_address + 1, (uintptr_t)m_assembly->GetCode() - (uintptr_t)get_adjusted(m_address) - 5);
	}
#else
	void* AllocateFunctionStub(void* ptr, int type)
	{

		typedef void*(*AllocateType)(void*, int);
		static AllocateType func;

		if (func == nullptr)
		{
			HMODULE coreRuntime = GetModuleHandleW(L"CoreRT.dll");
			func = (AllocateType)GetProcAddress(coreRuntime, "AllocateFunctionStubImpl");
		}

		return func(ptr, type);

	}
#endif
}