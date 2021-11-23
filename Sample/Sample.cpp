#include <iostream>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <TlHelp32.h>
#include <filesystem>
#include <direct.h>


using namespace std;


DWORD getProcesByName(char* pName);


const char* passwordFileName = "C:\\oqwidmjkdbasfsdfs.txt";
const char* password = "put_your_password_here";


int main()
{
    LockWorkStation();

    Sleep(5000);


    ofstream out;

    out.open(passwordFileName, ios::out);

    if (out.is_open() == false)
    {
        printf("Failed to find open the password text file");
        return 2;
    }

    out << password;
    out.close();

    DWORD processId = getProcesByName((char*)"logonui.exe");

    if (processId == 0)
    {
        printf("Failed to find the process");
        return 2;
    }

    HANDLE procHandle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, processId);
    
    if (procHandle == NULL)
    {
        printf("Failed to open the process");
        return 2;
    }

    FARPROC loadLibraryAddr = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    char dllName[MAX_PATH] = { 0 };

    _fullpath(dllName, "./Windows7Logon.dll", sizeof(dllName));

    const unsigned int FILENAME_LENGTH = strlen(dllName) + 1;

    LPVOID allocMemAddress = VirtualAllocEx(procHandle, NULL, FILENAME_LENGTH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    SIZE_T bytesWritten;
    WriteProcessMemory(procHandle, allocMemAddress, &dllName[0], FILENAME_LENGTH,  &bytesWritten);

    if (bytesWritten < FILENAME_LENGTH)
    {
        printf("Failed to write dll path to the process");
        return 2;
    }

    CreateRemoteThread(procHandle, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, allocMemAddress, 0, NULL);
}


DWORD getProcesByName(char* pName)
{
    DWORD pID = 0;
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pInfo;
    pInfo.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapShot, &pInfo))
    {
        while (Process32Next(snapShot, &pInfo))
        {
            if (_stricmp(pName, pInfo.szExeFile) == 0)
            {
                pID = pInfo.th32ProcessID;
                break;
            }
        }
    }

    CloseHandle(snapShot);
    return pID;
}
