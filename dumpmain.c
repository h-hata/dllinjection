#include <windows.h>
#include <imagehlp.h>
#pragma comment(lib,"imagehlp.lib")
extern __declspec(dllimport) int myadd(int,int);
static void dumpIAT(PIMAGE_IMPORT_DESCRIPTOR ipd,HMODULE hm)
{
	PIMAGE_THUNK_DATA th,name;
	PIMAGE_IMPORT_BY_NAME ibn;
	DWORD old_status;
	printf("dumpIAT\n");
	for(;ipd->Name;ipd++){
		printf("DLL:%s\n",(PBYTE)hm+ipd->Name);
		th=(PIMAGE_THUNK_DATA)((PBYTE)hm+ipd->FirstThunk);
		name=(PIMAGE_THUNK_DATA)((PBYTE)hm+ipd->OriginalFirstThunk);
		for(;th->u1.Function;name++,th++){
			printf("    AddressOfData:%08X ",name->u1.AddressOfData);
			ibn=(PIMAGE_IMPORT_BY_NAME)((PBYTE)hm+name->u1.AddressOfData);
			printf(" %s => %X",(LPSTR)(ibn->Name), th->u1.Function);
			printf("\n");
		}
	}
}

int main (int argc,char **argv){
	PIMAGE_IMPORT_DESCRIPTOR ipd;
	HMODULE hm;
	ULONG s;
	char *str;
	
	printf("myadd=%d\n",myadd(1,2));
	hm=GetModuleHandle("dumpmain.exe");
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
	dumpIAT(ipd,hm);
	return 0;
}