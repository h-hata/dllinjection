# dllinjection
An example for online patch by using of DLL injection
====

## Description
The add.dll has a servere bug. It should add given two integers but it subtracts actually.
The main.exe uses the add.dll and displays the wrong result of additional calculation repeatedly.
However the main.exe is a mission critical process and you have to fix this bug without
restarting main.exe. Is it possible to do such a thing?  

This project should be built by the compiler of Visual Studio tools.

## Requirement
Visual Studio tools

## Notification
If you are on a 64bit cpu then invoke the VS201* x64 native tool command prompt.
The x86 native tool builds an 32 bit executable and you will get run time error
when  "CreateRemoteProcess" API is called. This occures because the Windows API DLL
is 64 bit neverthless your executable is 32 bit. I suggest you should build 
64 bit executables and DLLs when your Windows OS is for 64 bit.

## How to build
0)open Visual Studio tools command prompt
>cl
enter cl command, when the version of Microsoft(R) C/C++ Optimizing Compiler ought to be displayed.


1)build a buggy DLL

>cl add.c /LD

2)build a main.c which is a driver of add.dll

>cl main.c add.lib

3)build a bug fixed fixed DLL

>cl newadd.c /LD

4)build a DLL injector
>cl inj.c

Finally, you built two DLLs, add.dll and new add.dll and two executables, main.exe and inj.exe

## Usage

1) invoke wrong calculator
>main.exe  

When you invoke main.exe, a wrong result calculation is repeated on your screen.
This program won't terminate itself. You can enter Ctl+C to force to terminate.

2) Bug Fix

2-1) getting PID of main.exe
Open another command prompt and operate fixing via this window. 
>tasklist /FI "IMAGENAME eq main.exe"  

IMAGE NAME                     PID SESSION NAME        SESSION#       MEMORY  
========================= ======== ================ =========== ============  
main.exe                     860 Console                    1      1,608 K  

This says the PID of  maymain.exe is 860.  

2-2) inject bug fix patch to main.exe
> inj 860 newadd.dll  
Cross Injection id=860  
Wait for Remote Thread  
Remote Thread initialization completed  

When inj.exe successes to inject newadd.dll to main.exe,
the above message should be displayed.

195:15+3=12  
196:15+3=12  
197:15+3=12  
198:15+3=12  
199:15+3=12  
200:15+3=18 <===== Bug Fixed without re-starting main.exe  
201:15+3=18  
202:15+3=18  
203:15+3=18  
204:15+3=18  

You might notice the interval of increment counter of main.exe.
Sleep API is used in the loop of main.c. The newadd.dll overrides
add function and Sleep API. After injection of this newadd.dll,
when main.exe call Sleep API, the Sleep of the newadd.dll is invoked.
The newadd.dll divides the input parameter by 2 of Sleep method and call
the true Sleep of Win API and passes the half of the original parameter
as the duration parameter. This means you can investigate API calls of 
an application by inject DLL without alter the code of the application.
This is referred as to API hook.


## Licence

[MIT]

## Author

[h-hata](https://github.com/h-hata/)
