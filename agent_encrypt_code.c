//
// Created by TLP on 5/24/2026.
//
// makes the function inline to place it alot in the compiled code
static inline char __attribute__((section(".primary"), always_inline)) decrypt_byte(int const line, int const byte_value, int const seed)
{
    /*key = int(line, 16) + seed
    result = (key ^ value) & 0xFF*/
    int key = line + seed;
    int result = (key ^ byte_value) & 0xFF;
    return result;
}