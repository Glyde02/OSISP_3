#include <windows.h> 
#include <stdio.h>

extern int __cdecl myPuts(LPWSTR);   // a function from a DLL

int main(VOID)
{
    int Ret = 1;

    //Ret = myPuts(L"Message sent to the DLL function\n");

    MEMORY_BASIC_INFORMATION memInfo;

    HANDLE hProcess = GetCurrentProcess();
    SYSTEM_INFO lpSysInfo;
    GetSystemInfo(&lpSysInfo);


    DWORD start = lpSysInfo.lpMinimumApplicationAddress;

    


    while (start < lpSysInfo.lpMaximumApplicationAddress)
    {
        VirtualQueryEx(hProcess, start, &memInfo, sizeof(memInfo));
        DWORD err = GetLastError();

        switch (memInfo.AllocationProtect) {
        case PAGE_READONLY:
            printf("%s%d\n", "READONLY  ", memInfo.BaseAddress);
            break;
        case PAGE_READWRITE:
            printf("%s%d\n", "READWRITE  ", memInfo.BaseAddress);
            break;
        case PAGE_NOACCESS:
            printf("%s%d\n", "NOACCESS  ", memInfo.BaseAddress);
            break;
        }
        start += memInfo.RegionSize;
    }




    if (memInfo.Protect == PAGE_READWRITE)
    {
        wsprintf("%s", L"READWRITE");
    }
    return Ret;
}