#include <windows.h>
__declspec(dllexport) int myadd(int a ,int b){
	return a-b;//Bug
}

BOOL WINAPI DllMain(HINSTANCE h,DWORD reasin,PVOID imp){
	return TRUE;
}
