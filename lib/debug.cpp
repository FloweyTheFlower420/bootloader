#include "debug.h"

// stolen from limine 
// :P
// go use limine lol its the better bootloader
template <typename C>
void backtrace(size_t *base_ptr, backtrace_callback_t callback)
{
    if (base_ptr == NULL) 
    {
        // we should be in 32 bit mode until the end.
        // i want the bootloader to work on very old systems
        
        __asm__ __volatile__(
            "movl %%ebp, %0"
            : "=g"(base_ptr)
            :
            : "memory"
        );
    }

    while(true)
    {
        size_t old_bp = base_ptr[0];
        size_t ret_addr = base_ptr[1];
        if (!ret_addr)
            break;
        
        callback((size_t)ret_addr);
        base_ptr = (size_t*)old_bp;
    }
}
