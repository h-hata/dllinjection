#include <windows.h>
#include <stdio.h>
extern __declspec(dllimport) int myadd(int,int);
int main(int argc,char **argv){
	int i;
	for(i=0;;){
		printf("%d:15+3=%d\n",i++,myadd(15,3));
		Sleep(2000);
	}
}
