#include "video.h"
#include "realmode.h"
#include <config.h>
#include <libc.h>
#include MALLOC_IMPL_PATH

static vbe_controller_info* read_controller_info()
{
    auto out = bios_interrupt_pmode(0x10, {.a = 0x4F00, .D = (uint32_t)REALMODE_BUFFER});

    if (out.ax() != 0x004F)
        return nullptr;

    void* buffer = alloc::malloc(sizeof(vbe_controller_info));
    if (buffer)
        return (vbe_controller_info*)memcpy(buffer, REALMODE_BUFFER, sizeof(vbe_controller_info));
    return nullptr;
}

static vbe_mode_info* read_mode_info(uint16_t mode)
{
    auto out = bios_interrupt_pmode(0x10, {.a = 0x4F01, .c = mode, .D = (uint32_t)REALMODE_BUFFER});

    if (out.ax() != 0x004F)
        return nullptr;

    void* buffer = alloc::malloc(sizeof(vbe_mode_info));
    if (buffer)
        return (vbe_mode_info*)memcpy(buffer, REALMODE_BUFFER, sizeof(vbe_mode_info));
    return nullptr;
}

static bool set_mode(uint16_t mode, vbe_mode_info* info)
{
    memcpy(REALMODE_BUFFER, info, sizeof(vbe_mode_info));
    auto out = bios_interrupt_pmode(
        0x10, cpustate{.a = 0x4F02, .b = (uint16_t)(mode | (uint16_t)0x4000), .D = (uint32_t)REALMODE_BUFFER});
    return out.ax() == 0x004F;
}

bool set_video_mode(video_mode_predicate predicate)
{
    vbe_controller_info* controller = read_controller_info();
    uint16_t* modes = (uint16_t*)controller->video_mode_ptr.to_ptr();
    uint16_t id;

    if (controller == nullptr)
        return false;

    while ((id = *modes) != 0xffff)
    {
        vbe_mode_info* mode = read_mode_info(id);
        if ((bool)mode & predicate(mode))
        {
            bool ret = set_mode(id, mode);
            alloc::free(controller);
            return ret;
        }

        alloc::free(mode);
        modes++;
    }
    return false;
}
