#include <windows.h>
#include <imagehlp.h>

#pragma comment(lib,"imagehlp.lib")
#define MODULE_NAME "main.exe"
__declspec(dllexport) int newproc(int a,int b){
	return a+b;
}
__declspec(dllexport) void newsleep(DWORD m){
	Sleep(m/2);
}
BOOL updateIAT(PIMAGE_IMPORT_DESCRIPTOR ipd,HMODULE hm, char *dllname, char *procname, PVOID newproc)
{
	PIMAGE_THUNK_DATA th,name;
	PIMAGE_IMPORT_BY_NAME ibn;
	DWORD old_status;
	for(;ipd->Name;ipd++){
		if(strcmp((PBYTE)hm+ipd->Name,dllname)!=0)
			continue;
		th=(PIMAGE_THUNK_DATA)((PBYTE)hm+ipd->FirstThunk);
		name=(PIMAGE_THUNK_DATA)((PBYTE)hm+ipd->OriginalFirstThunk);
		for(;th->u1.Function;name++,th++){
			if(name->u1.AddressOfData & 0x80000000){
				continue;
			}else{
				ibn=(PIMAGE_IMPORT_BY_NAME)((PBYTE)hm+name->u1.AddressOfData);
				if(strcmp((LPSTR)(ibn->Name),procname)==0){//Matched
					VirtualProtect(&th->u1.Function,sizeof(th->u1.Function),PAGE_WRITECOPY,&old_status);
					CopyMemory(&th->u1.Function,&newproc,sizeof(th->u1.Function));
					VirtualProtect(&th->u1.Function,sizeof(th->u1.Function),old_status,&old_status);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL WINAPI DllMain(HINSTANCE h,DWORD reason,PVOID pv){
	BOOL ret;
	PIMAGE_IMPORT_DESCRIPTOR ipd;
	HMODULE hm;
	ULONG s;
	char *str;
	if(reason==DLL_PROCESS_ATTACH){
		hm=GetModuleHandle(MODULE_NAME);
		if(hm==NULL){
			printf("SearchTargetModule Error\n");
			return FALSE;
		}
		ipd=(PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToDataEx(
			hm,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&s,NULL);
		if(ipd==NULL){
			printf("ImageDirectoryEntryToDataEx Error\n");
			return FALSE;
		}
		ret=updateIAT(ipd,hm,"add.dll","myadd",newproc);
		ret=updateIAT(ipd,hm,"KERNEL32.dll","Sleep",newsleep);
	}
	return TRUE;
}
