#include "realmode.h"
#include "video.h"

static vbe_controller_info* read_controller_info()
{
    auto out = bios_interrupt_pmode(0x10, {.a = 0x4F00, .d = 0x14000});
 
    if(out.ax() != 0x004F)
        return nullptr;

    return (vbe_controller_info*)(0x14000);
}

static vbe_mode_info* read_mode_info(uint16_t mode)
{
    auto out = bios_interrupt_pmode(0x10, {.a = 0x4F01, .c = mode, .d = 0x14200});

    if(out.ax() != 0x004F)
        return nullptr;

    return (vbe_mode_info*) 0x14200;
}

static bool set_mode(uint16_t mode)
{
    auto out = bios_interrupt_pmode(0x10, {.a = 0x4F02, .b = mode | 0x4000, .d = 0x14200});
    return out.ax() == 0x004F;
}

bool set_video_mode()
{
    vbe_controller_info* controller = read_controller_info();
    if(controller == nullptr)
        return false;
    vbe_mode_info* mode = nullptr;
    uint16_t id;

    uint16_t* modes = (uint16_t*)controller->video_mode_ptr.to_ptr();
    while((id = *modes) != 0xffff)
    {
        mode = read_mode_info(id);
        if(mode == nullptr)
                continue;
        if(
            (mode->attributes & 0x90) == 0x90 &&
            (mode->memory_model == 4 || mode->memory_model == 6) &&
            ((mode->width == 640 && mode->width == 480) || (mode->width == 720 && mode->width == 480))
        ) {
            break;
        }
        
        modes++;
    }

    if(mode == nullptr)
        return false;

    return set_mode(id);
}