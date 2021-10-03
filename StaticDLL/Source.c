#include <windows.h> 
#include <stdio.h>

extern int __cdecl myPuts(LPWSTR);   // a function from a DLL

int main(VOID)
{
    int Ret = 1;

    //Ret = myPuts(L"Message sent to the DLL function\n");

    #define MY_BUFSIZE 1024 // Buffer size for console window titles.
    char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
                                        // WindowTitle.
    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
    HWND hwnd = FindWindowA(NULL, pszOldWindowTitle);


    MEMORY_BASIC_INFORMATION memInfo;

    HANDLE hProcess = GetCurrentProcess();
    SYSTEM_INFO lpSysInfo;
    GetSystemInfo(&lpSysInfo);


    DWORD start = lpSysInfo.lpMinimumApplicationAddress;

    int lpBuffer = 0;
    

    DWORD ProcId; // ���������� �������� � ���� ID ��������
    GetWindowThreadProcessId(hwnd, &ProcId); // ��������� ID ��������
    HANDLE hChild = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcId); // handle ����� ��������� ��� ����� ��������� ���� � ���� (���� ��� ���������)

    const char* signature = "DLL.exe";      // ������������������ ������� ����� ������
    size_t len_signature = strlen(signature); // ����� ������������������.

    char dll[7] = { 'D', 'L','L','.','e','x', 'e' };

    while (start < lpSysInfo.lpMaximumApplicationAddress)
    {
        VirtualQueryEx(hProcess, start, &memInfo, sizeof(memInfo));
        DWORD err = GetLastError();

        //switch (memInfo.AllocationProtect) {
        //case PAGE_READONLY || PAGE_NOACCESS:
        //    printf("%s%d\n", "READONLY  or NOACCESS", memInfo.BaseAddress);
        //    break;
        //default:
        //    
        //   // ReadProcessMemory(hChild, memInfo.BaseAddress, &lpBuffer, memInfo.RegionSize, NULL);
        //    ReadProcessMemory(hChild, memInfo.BaseAddress, &lpBuffer, sizeof(lpBuffer), 0); // ������ ��� ������ �� ������
        //    if (lpBuffer != 0)
        //        printf("%d\n", lpBuffer);
        //    break;
        //}

        if (memInfo.AllocationProtect != (PAGE_READONLY || PAGE_NOACCESS))
        {
            if (memInfo.RegionSize != 0)
            {
                BYTE* lpData = (BYTE*)malloc(memInfo.RegionSize);//����������� ����� ��� ������� lpData, �������� � ������

                if (ReadProcessMemory(hChild, (LPCVOID)start, lpData, memInfo.RegionSize, 0))
                {
                    

                    BYTE* p = lpData;
                    while ((p = (BYTE*)memchr(p, signature[0], memInfo.RegionSize - len_signature + 1 - (p - lpData))) != 0)
                    {
                        if (0 == memcmp(p + 1, signature + 1, len_signature - 1))
                        {
                            // �������!!! � (p) ����� ������ ������, ��� ���������� ������������������.
                            // �������������� ����� � ���������� �����: Start + (p - lpData);
                            printf("%d  --  %d\n", lpData, start);

                            //WriteProcessMemory(hChild, start + (p - lpData), L"Helloee", sizeof("helloee"), 0);


                        }
                        ++p;
                    }

                }
                free(lpData);
            }
        }

        start += memInfo.RegionSize;
    }

    
    return Ret;
}