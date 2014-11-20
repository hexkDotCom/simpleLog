//
// 
// MODULE NAME:			dllmain.c	for user library simpleLogTst
//

#ifdef WIN32

#include <stdio.h>
#include <windows.h>
#include "kicommon.h"

BOOL WINAPI DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			createReaderLock( "simpleLogTst" );
			break;
 
		case DLL_THREAD_ATTACH:
			break;
 
		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			deleteReaderLock( "simpleLogTst" );
			break;
	}
	return TRUE;
}

#endif
