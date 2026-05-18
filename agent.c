#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <crypt.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include "crypt.h"
#define SLEEP_MS(ms) usleep(ms * 1000)
#define DIFF(a, b) ((char*)a - (char*)b)
#define MACRO_SECRET_SEED 0xA1
#define GET_NTH_COUPLE(a, n) ((a>>(8*n))&0xFF)

typedef struct {
    char agent_id[32];
    char token[64];
    unsigned int session_id;
} BeaconFrame;
int change_page_permissions_of_address(void *addr);
void print_function_instructions(void *func_ptr, unsigned int func_len);
void beacon_home(BeaconFrame* frame);


/*int has_dbg()
{
    unsigned int v1; // [rsp+8h] [rbp-828h]
    int fd; // [rsp+Ch] [rbp-824h]
    ssize_t v3; // [rsp+10h] [rbp-820h]
    char *v4; // [rsp+18h] [rbp-818h]
    char buf[2056]; // [rsp+20h] [rbp-810h] BYREF
    //unsigned __int64 v6; // [rsp+828h] [rbp-8h]

    //v6 = __readfsqword(0x28u);
    v1 = 0;
    fd = open("/proc/self/status", 0);
    if ( fd == -1 )
        return 0;
    v3 = read(fd, buf, 0x800u);
    close(fd);
    if ( v3 > 0 )
    {
        buf[v3] = 0;
        v4 = strstr(buf, "TracerPid:");
        if ( v4 )
            return atoi(v4 + 10) != 0;
    }
    return v1;
}*/


void __attribute__((section(".mySection"))) generate_c2_token(const char* agent_id, char* output_token) {
    const unsigned int SECRET_SEED = MACRO_SECRET_SEED;

    printf("tokern %x\n", SECRET_SEED);
    size_t len = strlen(agent_id); // the len of agent_id 
    unsigned char raw_data[32] = {0}; // 

    for (size_t i = 0; i < len && i < 32; i++) {
        unsigned char key_byte = (SECRET_SEED >> ((i % 4) * 8)) & 0xFF;
        raw_data[i] = (agent_id[i] ^ key_byte) + i;
    }

    for (size_t i = 0; i < len && i < 32; i++) {
        sprintf(output_token + (i * 2), "%02X", raw_data[i]);
    }
}
void __attribute__((constructor)) change_secret_seed() {

    char loc_str[] = __ENCRYPT64("0x12345679") ;
    __DECRYPT64(loc_str);
    int loc = (int)strtol(loc_str, NULL, 0);


    char * func_ptr = (void*)loc;

    int func_len = 100;
    int offset = 0;
    for(unsigned char i=0; i<func_len; i++) {
        unsigned char *instruction = (unsigned char*)func_ptr+i;
        int is_true = 1;
        for (int index = 0; index<4 && is_true; index++)
        {
            if (*(instruction+index) != GET_NTH_COUPLE(MACRO_SECRET_SEED,index))
            {
                is_true = 0;
                break;
            }
        }
        if ( is_true)
        {
            offset = i;
            printf("offset_found %d\n", offset);
            break;
        }
        //printf("%p (%2u): %x\n", func_ptr+i, i, *instruction);
    }

    change_page_permissions_of_address(func_ptr);
    unsigned char *ins = (unsigned char*)func_ptr + offset;
    *ins = 0xde;
    *(ins+1) = 0xba;
    *(ins+2) = 0xe1;
    *(ins+3) = 0xac;
}

void apple(void) {
    unsigned int a = 1;
    unsigned int b = 2;
    printf("apple: %x\n", a);
}

int main(int argc, char* argv[]) {
    BeaconFrame current_frame;


    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        printf("don't trace me !!\n");
        printf("FAILED\n");
        return 1;
    }
    printf("loc %p\n", generate_c2_token);
    //extern  char __excutable_start;

    //printf("start: 0x%lx\n", (unsigned long)&__executable_start);
    //printf("generate_c2 : 0x%lx\n", (unsigned long)generate_c2_token);
    // void* token_addr = (void*)generate_c2_token;
    // void* main_addr = (void*)main;
    // int diff = DIFF((void*)generate_c2_token, (void*)main);

    //change_page_permissions_of_address(token_addr);
    //print_function_instructions(token_addr, (char *)change_secret_seed - (char *)generate_c2_token);
    //change_secret_seed();

    current_frame.session_id = rand() % 0xFFFF;


    char default_id[] = __ENCRYPT64("sercret_agent");
    __DECRYPT64(default_id);

    strncpy(current_frame.agent_id, (argc > 1) ? argv[1] : default_id, 31);

    while (1) {
        memset(current_frame.token, 0, sizeof(current_frame.token));

        generate_c2_token(current_frame.agent_id, current_frame.token);

        beacon_home(&current_frame);

        //SLEEP_MS(30000);
        SLEEP_MS(3000);  // sleep for 30 milliseconds for testing purposes
    }

    return 0;
}
int change_page_permissions_of_address(void *addr) {
    // Move the pointer to the page boundary
    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    if(mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        return -1;
    }

    return 0;
}

void beacon_home(BeaconFrame* frame) {
	// This function sends the beacon, but it is not implemented in this case
    printf("Beaconing home with Agent ID: %s, Session ID: %u, Token: %s\n",
           frame->agent_id, frame->session_id, frame->token);
    return;
}
void print_function_instructions(void *func_ptr, unsigned int func_len) {
    for(unsigned int i=0; i<func_len; i++) {
        unsigned char *instruction = (unsigned char*)func_ptr+i;
        printf("%p (%2u): %x\n", func_ptr+i, i, *instruction);
    }
}

