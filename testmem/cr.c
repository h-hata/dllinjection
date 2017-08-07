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


static void cr(DWORD pid,char *param){
	PTHREAD_START_ROUTINE remote_start;
	HANDLE hp=NULL,ht=NULL;
	int cch,cb;
	if(sscanf(param,"%p",&remote_start)!=1){
		return;
	}
	printf("Trying.. %p",remote_start);
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
		ht=CreateRemoteThread(hp,NULL,0,remote_start,NULL,0,NULL);
		if(ht==NULL){
			DispError();
			printf("CreateRemoteThread Error\n");
			__leave;
		}
		printf("Wait for Remote Thread\n");
		WaitForSingleObject(ht,INFINITE);
		printf("Remote Thread initialization completed\n");
	}
	__finally{
		if(ht)CloseHandle(ht);
		if(hp)CloseHandle(hp);
	}
	printf("over\n");
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
	cr(id,argv[2]);
	return 0;
}
		
		
