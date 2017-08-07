#include <windows.h>
#include <stdio.h>

DispError(){
	LPVOID lpMsgBuf;
	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(),
			 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			 (LPTSTR) &lpMsgBuf, 0, NULL);
	printf("Error:%s\n",(const char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


static void inject(DWORD pid,char *fname){
	HANDLE hp=NULL;
	PWSTR remote_ptr=NULL;
	int cch,cb;
	PTHREAD_START_ROUTINE remote_start;
	__try{
		SetLastError(NO_ERROR);	
		hp=OpenProcess(
			PROCESS_CREATE_THREAD|//forCreateRemoteThread
			PROCESS_VM_OPERATION|//forVirtualAllocEx
			PROCESS_VM_WRITE,//WriteProcessMemory
			FALSE,pid);
		if(hp==NULL){
			printf("Cannot OpenProcess\n");
			__leave;
		}
		remote_ptr=(PWSTR)VirtualAllocEx(hp,NULL,strlen(fname)+1,
			MEM_COMMIT,PAGE_READWRITE);
		if(remote_ptr==NULL){
			printf("VirtualAllocEx Error\n");
			__leave;
		}
		if(!WriteProcessMemory(hp,remote_ptr,fname,strlen(fname)+1,NULL)){
			printf("WriteProcessMemory Error\n");
			__leave;
		}
		printf("Write Memory completed ptr=%p\n",remote_ptr);
	}
	__finally{
		if(hp)CloseHandle(hp);
	}
}
int main(int argc,char **argv){
	int id;
	if(argc!=3){
		printf("inj pid dllname\n");
		return 0;
	}
	if((id=atoi(argv[1]))==0){
		id=GetCurrentProcessId();
		printf("Self Injection id=%d\n",id);
	}else{
		printf("Cross Injection id=%d\n",id);
	}
	inject(id,argv[2]);
	return 0;
}
		
		
