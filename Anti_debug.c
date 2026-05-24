//
// Created by TLP on 5/18/2026.
//

#include "Anti_debug.h"
#include  <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/ptrace.h>

#include <dlfcn.h>
#include  "crypt.h"


int simple_ptrace_check_debugger()
{
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        printf("being traced, ptrace\n");
        return 1;
    }else
    {
        printf("not being traced, ptrace\n");
    }
    return 0;
}
int check_tracer_pid()
{
    FILE *fp = fopen("/proc/self/status", "r");
    if (fp == NULL) {
        return -1; // Error opening the file
    }

    char line[256];
    int tracer_pid = 0;

}

int is_tracer_pid()
{
    FILE *fp = fopen("/proc/self/status", "r");
    if (fp == NULL) {
        return -1; // Error opening the file
    }

    char line[256];
    int tracer_pid = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "TracerPid: %d", &tracer_pid) == 1) {
            if (tracer_pid != 0) {
                fclose(fp);
                return tracer_pid;
            }
        }
    }

    fclose(fp);
    return tracer_pid;
}

int GetParentProcessName(char *buffer, size_t buffer_size) {
    FILE *fp;
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", getppid());

    fp = fopen(path, "r");
    if (fp == NULL) {
        return -1;
    }
    while (fgets(buffer, buffer_size, fp)) {
        if (sscanf(buffer, "Name: %s", buffer) == 1) {
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return -1;
}

int find_breakpoint(void* start, void* end) {
    unsigned char* ptr = (unsigned char*)start;
    while (ptr < (unsigned char*)end) {
        if (*ptr == 0xCC) {
            return 1;
        }
        ptr++;
    }
    return 0;
}



int hidden_ptrace()
{
    void *handle;
    long (*go)(enum __ptrace_request request, pid_t pid, void *addr, void *data);
    handle = dlopen ("libc.so.6", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error opening library: %s\n", dlerror());
        return -1;
    }
    char ptrace_str[] = __ENCRYPT64("ptrace");
    __DECRYPT64(ptrace_str);
    go = dlsym(handle, ptrace_str);
    if (go(PTRACE_TRACEME, 0, NULL, NULL) < 0)
    {
        printf("being traced, hidden ptrace\n");
        return 1;
    }
    dlclose(handle);
    return 0;
}
