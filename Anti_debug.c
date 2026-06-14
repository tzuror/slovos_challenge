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





int __attribute__((section(".primary"))) hidden_ptrace()
{
    void *handle;
    long (*go)(enum __ptrace_request request, pid_t pid, void *addr, void *data);
    handle = dlopen ("libc.so.6", RTLD_NOW);
    if (!handle) {
        return -1;
    }
    char ptrace_str[] = __ENCRYPT64("ptrace");
    __DECRYPT64(ptrace_str);
    go = dlsym(handle, ptrace_str);
    if (go(PTRACE_TRACEME, 0, NULL, NULL) < 0)
    {
        return 1;
    }
    dlclose(handle);
    return 0;
}
