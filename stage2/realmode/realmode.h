#ifndef __STAGE2_REALMODE_REALMODE_H__
#define __STAGE2_REALMODE_REALMODE_H__
#include <cstdint>

struct cpustate
{
    uint32_t a, b, c, d, S, D, flags;

    inline uint16_t ax() { return a & 0xFFFF; }
    inline uint16_t bx() { return b & 0xFFFF; }
    inline uint16_t cx() { return c & 0xFFFF; }
    inline uint16_t dx() { return d & 0xFFFF; }

    inline uint16_t al() { return a & 0x00FF; }
    inline uint16_t bl() { return b & 0x00FF; }
    inline uint16_t cl() { return c & 0x00FF; }
    inline uint16_t dl() { return d & 0x00FF; }

    inline uint16_t ah() { return a & 0xFF00 >> 8; }
    inline uint16_t bh() { return b & 0xFF00 >> 8; }
    inline uint16_t ch() { return c & 0xFF00 >> 8; }
    inline uint16_t dh() { return d & 0xFF00 >> 8; }

    inline bool carry() { return flags & 0x1; }
};

void protected_mode(); // enters protected mode

extern "C" {
    void a20(); // tries to enable the a20 line. Returns true if a20 is enabled
}

cpustate bios_interrupt_pmode(uint8_t, const cpustate& state); // calls a bios interrupt from protected mode
cpustate bios_interrupt_realmode(uint8_t, const cpustate& state); // calls a bios interrupt from real mode

#define BEGIN_16 __asm__ __volatile__(".code16\n.code16gcc")
#define BEGIN_32 __asm__ __volatile__(".code32")

#endif
