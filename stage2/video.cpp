asm (".code16gcc");
asm (".code16");

#include <cstdint>
#include "realmode.h"

vbe_controller_info* read_controller_info()
{
    vbe_controller_info* block = (vbe_controller_info*)(0x4000);
    uint16_t return_code;
    __asm__ __volatile__(
        "int $0x10"
        : "=a"(return_code)
        : "a"(0x4F00),
          "D"(0x4000)
    );

    if(return_code != 0x004F)
        return nullptr;

    return block;
}

vbe_mode_info_structure* read_mode_info(uint16_t mode)
{
    vbe_mode_info_structure* block = (vbe_mode_info_structure*)(0x4200);
    // INT 0x10, AX=0x4F01, CX=mode, ES:DI=256 byte buffer
    uint16_t return_code;
    __asm__ __volatile__(
        "int $0x10"
        : "=a"(return_code)
        : "a"(0x4F01),
          "c"(mode),
          "D"(0x4200)
    );

    if(return_code != 0x004F)
        return nullptr;

    return block;
}

bool set_mode(vbe_mode_info_structure* mode_struct, uint16_t mode)
{
    uint16_t return_code;
    __asm__ __volatile__(
        "int $0x10"
        : "=a"(return_code)
        : "a"(0x4F02),
          "b"(mode | 0x4000),
          "D"(mode_struct)
    );

    return return_code == 0x004F;
}