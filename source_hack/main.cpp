#define WIN32_LEAN_AND_MEAN
#define VS_EXTRA_LEAN
#include <Windows.h>

#pragma comment(lib, "lua53.lib")

extern DWORD __stdcall source_hack();

BOOL APIENTRY DllMain(HMODULE hMod, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		source_hack();
	}

	return TRUE;
}