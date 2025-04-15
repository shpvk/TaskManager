#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <psapi.h>

int main()
{
    HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(h_snapshot == INVALID_HANDLE_VALUE)
    {
        printf("Snapshot value error\n");
        return 1;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    

    if(Process32First(h_snapshot, &pe32))
    {
        do
        {
            if (strcmp(pe32.szExeFile, "System") == 0 || strcmp(pe32.szExeFile, "csrss.exe") == 0)
            {
                continue;
            }

            HANDLE h_memory_info = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
            if(h_memory_info == NULL)
            {
                printf("Failed to open process for memory info\n");
                continue;
            }
            PROCESS_MEMORY_COUNTERS pmc;
            if(!GetProcessMemoryInfo(h_memory_info, &pmc, sizeof(pmc)))
            {
                printf("Failed to get memory information.\n");   
                continue;    
            }
            printf("Process: %s | PID: %lu | Process takes: %zu bytes\n", pe32.szExeFile, pe32.th32ProcessID, pmc.WorkingSetSize);
            CloseHandle(h_memory_info);
        } while (Process32Next(h_snapshot, &pe32));
    }

    unsigned int pid;
    printf("Enter the PID of the process you want to delete: ");
    printf("\n");
    scanf("%u", &pid);

    HANDLE h_terminate_process = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if(h_terminate_process == NULL)
    {
        printf("You do not have access to end this process\n");
        return 1;
    }

    TerminateProcess(h_terminate_process, 0);

    CloseHandle(h_snapshot);
    CloseHandle(h_terminate_process);

    getchar();
    return 0;
}