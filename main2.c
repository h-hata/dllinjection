#include <windows.h>
#include <stdio.h>
typedef int(*T)(int,int);
int main(int arc,char **argv)
{
	T f;
	HMODULE hm;
	hm=LoadLibrary("sub.dll");
	if(hm==NULL){
		printf("Load Error\n"); return 1;
	}
	f=(T)GetProcAddress(hm,"sub");
	printf("sub=%d\n",f(5,3));
}
