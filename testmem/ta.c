#include <windows.h>
#include <stdio.h>

void DispMessage(char *str)
{
	if(str!=NULL){
		printf("log:%s\n",str);
	}else{
		printf("log:\n");
	}
}
int filterException(int code, PEXCEPTION_POINTERS ex) {
	char msg[128];
	sprintf(msg,"Exception Filtering %X\n",code );
	DispMessage(msg);
	return EXCEPTION_EXECUTE_HANDLER;
}

main()
{
	char str[128];
	char d[16];
	int i;
	char *ptr;
	PTHREAD_START_ROUTINE p;
	PEXCEPTION_POINTERS info;
	printf("ptr=%p\n",DispMessage);
	printf("pid=%d\n",GetCurrentProcessId());
	printf(">");
	while(fgets(str,100,stdin)!=NULL){
		if(str[0]=='c'){
			i=sscanf(str,"%s %p",d,&p);
			if(i==2){
				printf("Start:%p\n",p);
				HANDLE ret=CreateThread(NULL,0,p,NULL,0,NULL);
				if(ret==NULL){
					printf("Error Create Thread\n");
				}
			}else{
				printf("Invalid\n");
			}
		}else{
			i=sscanf(str,"%p",&ptr);
			if(i==1){
				printf("%p\n",ptr);
				__try{
					printf("%s\n",ptr);
				}
				__except(filterException(
					GetExceptionCode(),GetExceptionInformation())){
					printf("%s\n","Exception");
				}
			}else{
				printf("Invalid\n");
			}
		}
		printf(">");
		fflush(stdout);
	}
}

