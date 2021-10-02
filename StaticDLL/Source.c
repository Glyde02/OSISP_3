#include <windows.h> 

extern int __cdecl myPuts(LPWSTR);   // a function from a DLL

int main(VOID)
{
    int Ret = 1;

    Ret = myPuts(L"Message sent to the DLL function\n");
    return Ret;
}