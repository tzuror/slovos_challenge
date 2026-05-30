#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <crypt.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include "junk1.c"
#include "crypt.h"
#include "anti_debug.c"
#include "agent_encrypt_code.c"
#include "anti_debug.h"



#define SLEEP_MS(ms) usleep(ms * 1000)
#define DIFF(a, b) ((char*)a - (char*)b)
#define MACRO_SECRET_SEED 0xA1
#define GET_NTH_COUPLE(a, n) ((a>>(8*n))&0xFF)

typedef struct {
    char agent_id[32];
    char token[64];
    unsigned int session_id;
} BeaconFrame;
int change_page_permissions_of_address(void *addr, int len);
void print_function_instructions(void *func_ptr, unsigned int func_len);
void beacon_home(BeaconFrame* frame);
void check_debugger(void);
int hidden_ptrace(void);
void decrypt_function(void *func_ptr);
void change_secret_seed(void);
void decrypt_all(void);
void decrypt_place_holder(void);
extern int __mySectionStart;
extern int __mySectionEnd;
extern int __mySection2Start;
extern int __mySection2End;
extern int __to_encrypt_start;
extern int __to_encrypt_end;
extern int __start_text;
extern int __end_text;



void __attribute__((section(".mySection"))) generate_c2_token(const char* agent_id, char* output_token) {
    const unsigned int SECRET_SEED = MACRO_SECRET_SEED;

    //printf("tokern %x\n", SECRET_SEED);
    size_t len = strlen(agent_id); // the len of agent_id 
    unsigned char raw_data[32] = {0}; // 

    for (size_t i = 0; i < len && i < 32; i++) {
        unsigned char key_byte = (SECRET_SEED >> ((i % 4) * 8)) & 0xFF;
        raw_data[i] = (agent_id[i] ^ key_byte) + i;
    }

    for (size_t i = 0; i < len && i < 32; i++) {
        //sprintf(output_token + (i * 2), "%02X", raw_data[i]);
    }
}

void __attribute__((section(".mySection2"))) place_holder(void) {
    //place holder
    return;
}
void __attribute__((constructor)) start()
{
    check_debugger();//check if being debugged before doing anything

    void* placehold_func  = place_holder;

    char  offset_str[] = __ENCRYPT64("0x1000");
    __DECRYPT64(offset_str);
    int offset1 = (int)strtol(offset_str, NULL, 0);
    //char * secret_func_ptr  = placehold_func + offset1;
    //char * secret_func_ptr = &__mySectionStart + offset1;
    char * secret_func_ptr = (char*)&__mySection2End + offset1;
    //printf("secret_func_ptr %p\n", secret_func_ptr);
    //printf("%p\n",generate_c2_token);

    change_page_permissions_of_address(secret_func_ptr, (size_t)&__mySectionEnd - (size_t)secret_func_ptr );
    change_page_permissions_of_address(&__to_encrypt_start, (size_t)&__to_encrypt_end - (size_t)&__to_encrypt_start);
    change_page_permissions_of_address(&__mySection2Start, (size_t)&__mySection2End - (size_t)&__mySection2Start);


    decrypt_function(secret_func_ptr);
    decrypt_all();
    decrypt_place_holder();


    change_secret_seed();

}
void decrypt_all()
{
    //printf("decrypting all\n");
    char * func_ptr_char = (char*)&__to_encrypt_start;
    //size_t exact_len = (size_t)&__mySectionEnd - (size_t)&__mySectionStart;
    size_t exact_len = (size_t)&__to_encrypt_end - (size_t)&__to_encrypt_start;
    //printf("exact_len %x\n", exact_len);
    //printf("exact_len %x\n", exact_len);
    for ( int i = 0; i < exact_len; i++)
    {
        //printf("decrypting byte %d\n", i);
        char current_byte = func_ptr_char[i];
        char decrypted_byte = decrypt_byte(i, func_ptr_char[i], 0x1337);
        func_ptr_char[i] = decrypted_byte;
        //printf("changed %x to %x\n", current_byte, decrypted_byte);
    }
}
void decrypt_place_holder()
{
    char * func_ptr_char = (char*)&__mySection2Start;
    //size_t exact_len = (size_t)&__mySectionEnd - (size_t)&__mySectionStart;
    size_t exact_len = (size_t)&__mySection2End - (size_t)&__mySection2Start;
    //printf("exact_len %x\n", exact_len);
    for ( int i = 0; i < exact_len; i++)
    {
        char current_byte = func_ptr_char[i];
        char decrypted_byte = decrypt_byte(i, func_ptr_char[i], 0x1337);
        func_ptr_char[i] = decrypted_byte;
        //printf("changed %x to %x\n", current_byte, decrypted_byte);
    }
}
void decrypt_function(void *func_ptr) {
    char * func_ptr_char = (char*)func_ptr;
    //size_t exact_len = (size_t)&__mySectionEnd - (size_t)&__mySectionStart;
    size_t exact_len = (size_t)&__mySectionEnd - (size_t)func_ptr;
    //printf("exact_len %x\n", exact_len);
    for ( int i = 0; i < exact_len; i++)
    {
        char current_byte = func_ptr_char[i];
        char decrypted_byte = decrypt_byte(i, func_ptr_char[i], 0x1337);
        func_ptr_char[i] = decrypted_byte;
        //printf("changed %x to %x\n", current_byte, decrypted_byte);
    }
}
#include "junk2.c"
void check_debugger(void)
{

    int hidden = hidden_ptrace();
    //int simple_patrace = simple_ptrace_check_debugger();
    int sum = hidden;
    if (sum)
    {
        //printf("FAILED\n");
        while (1)
        {

        }
        exit(1);
    }
    //int hiden_ptrace = hidden_ptrace();
}


void __attribute__((section(".to_encrypt"))) change_secret_seed() {
    void* placehold_func  = place_holder;
    char  offset_str[] = __ENCRYPT64("0x1000");
    __DECRYPT64(offset_str);
    int offset1 = (int)strtol(offset_str, NULL, 0);
    //char * func_ptr  = placehold_func + offset1;
    char * func_ptr = (char*)&__mySection2End + offset1;
    //change_page_permissions_of_address(func_ptr);
    //printf("loc %p\n", &func_ptr);
    //decrypt_function(func_ptr);


    int func_len = 100;
    int offset = 0;
    unsigned char i = 0;
    forloop:
    //for(unsigned char i=0; i<func_len; i++) {
    unsigned char *instruction = (unsigned char*)func_ptr+i;
    int is_true = 1;
    for (int index = 0; index<4 && is_true; index++)
    {
        if (*(instruction+index) != GET_NTH_COUPLE(MACRO_SECRET_SEED,index))
        {
            //printf("%x\n", *(instruction+index));
            is_true = 0;
            break;
        }
    }
    if ( is_true)
    {
        offset = i;
        //printf("offset_found %d\n", offset);
        goto endloop;
    }
    i++;
    if (i>=func_len) {

        goto endloop;
    }
    goto forloop;
    endloop:
    unsigned char *ins = (unsigned char*)func_ptr + offset;
    /**ins = 0xde;
    *(ins+1) = 0xba;
    *(ins+2) = 0xe1;
    *(ins+3) = 0xac;*/
    *ins = 0x55;
    *(ins+1) = 0x00;
    *(ins+2) = 0x6b;
    *(ins+3) = 0xb1;
}



int __attribute__((section(".to_encrypt"))) main(int argc, char* argv[]) {
    BeaconFrame current_frame;

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
int change_page_permissions_of_address(void *addr, int len) {
    // Move the pointer to the page boundary
    int page_size = getpagesize();
    //printf("page_size %d\n", page_size);
    addr -= (unsigned long)addr % page_size;

    if(mprotect(addr, len*2, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        return -1;
    }

    return 0;
}


void beacon_home(BeaconFrame* frame) {
	// This function sends the beacon, but it is not implemented in this case
    //printf("Beaconing home with Agent ID: %s, Session ID: %u, Token: %s\n",
    //       frame->agent_id, frame->session_id, frame->token);
    return;
}
void print_function_instructions(void *func_ptr, unsigned int func_len) {
    for(unsigned int i=0; i<func_len; i++) {
        unsigned char *instruction = (unsigned char*)func_ptr+i;
        printf("%p (%2u): %x\n", func_ptr+i, i, *instruction);
    }
}
#include "junk3.c"

