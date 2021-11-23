

#include "pch.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;


HMODULE gModule;

const char* passwordFileName = "C:\\oqwidmjkdbasfsdfs.txt";


void sendKeyDown(unsigned char keyCode)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;

    input.ki.wVk = keyCode;
    input.ki.dwFlags = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void sendKeyUp(unsigned char keyCode)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;

    input.ki.wVk = keyCode;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    SendInput(1, &input, sizeof(INPUT));

}
void type(HWND hwnd, string message)
{
    for (int i = 0; i < message.length(); i++)
    {
        auto letter = message[i];
                
        SetForegroundWindow(hwnd);

        if (isupper(letter) == true)
            sendKeyDown(VK_SHIFT);

        sendKeyDown(VkKeyScan(letter));
        sendKeyUp(VkKeyScan(letter));

        if (isupper(letter) == true)
            sendKeyUp(VK_SHIFT);
    }
}

BOOL CALLBACK childCallback(HWND hwnd, LPARAM lParam)
{
    char className[MAX_PATH];

    GetClassName(hwnd, className, sizeof(className));

    if (strcmp(className, "Edit") == 0)
    {
        BOOL* pSuccess = (BOOL*)lParam;

        ifstream in;

        in.open(passwordFileName);

        if (in.is_open() == true)
        {
            string password;

            getline(in, password);

            in.close();

            remove(passwordFileName);

            password += "\r";

            type(hwnd, password.c_str());

            *pSuccess = true;
        }

        return false;
    }

    return TRUE;
}

DWORD mainThread(LPVOID lpParam)
{
    Sleep(1000);

    DWORD startTime = GetTickCount();

    while (GetTickCount() - startTime < 10000)
    {
        Sleep(10);

        HWND logon = FindWindow(NULL, "Windows Logon");

        if (logon == NULL)
            continue;

        BOOL success = false;

        EnumChildWindows(logon, childCallback, (LPARAM)&success);

        if (success == true)
            break;
    }

    FreeLibraryAndExitThread(gModule, 0);
    
    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {

        case DLL_PROCESS_ATTACH:
            gModule = hModule;
            CreateThread(0, 0, mainThread, 0, 0, 0);
            break;
    }

    return TRUE;
}

