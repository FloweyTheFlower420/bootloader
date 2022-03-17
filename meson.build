project('bootloader', 'cpp')

conf_data = configuration_data()
conf_data.set('MALLOC_IMPL_PATH', '"lib/memory-allocator/linked-list-memory-allocator/alloc.h"')
conf_data.set('DISK_IMPL_PATH', '"bios/stage2/driver/disk/disk.h"')
conf_data.set('BIOS_DISK_PATIENCE', '3')

# Set data
configure_file(input : 'config.h.in',
    output : 'config.h',
    configuration : conf_data)

configuration_inc = include_directories('.')

kernel_gen = find_program('gen_binary.sh')

add_project_arguments(
    '-fno-rtti',
    '-fno-exceptions',
    '-Os',
    '-ffreestanding',
    '-fno-PIC',
    '-no-pie',
    '-fno-exceptions',
    '-Wno-ignored-attributes',
    '-Wall',
    '-fno-stack-protector',
    '-fcf-protection=none',
    '-fno-threadsafe-statics',
    '-I' + (meson.source_root() / 'lib/'),
    '-mno-sse',
    '-mno-sse2',
    '-mno-mmx',
    '-std=c++2a',
    '-mno-80387',
    language: 'cpp'
)

add_project_arguments(
    '-Os',
    '-ffreestanding',
    '-fno-PIC',
    language: 'c'
)

bios_stage1 = executable('boot.bin',
    'bios/boot/boot.S',
    link_args: [
        '-T' + (meson.source_root() / 'bios/boot/boot.lds'),
        '-static',
        '-nostdlib',
        '-m32'
    ],
    c_args: ['-m32', '-march=i386'],
    cpp_args: ['-m32', '-march=i386']
)

bios_stage2 = executable('bios-stage2.img',
    'bios/stage2/main.cpp',
    'bios/stage2/realmode/e820.cpp',
    'bios/stage2/realmode/realmode.cpp',
    'bios/stage2/realmode/video.cpp',
    'bios/stage2/utils.cpp',
    'bios/stage2/driver/disk/disk.cpp',
    'lib/debug.cpp',
    'lib/term.cpp',
    'lib/libc.cpp',
    'lib/memory-allocator/linked-list-memory-allocator/alloc.cpp',
    'lib/font.cpp',
    'lib/fs/fat.cpp',
    'bios/stage2/realmode/a20.S',
    link_args: [
        '-T' + (meson.source_root() / 'bios/stage2/link.lds'),
        '-static',
        '-nostdlib',
        '-lgcc',
        '-m32'
    ],
    c_args: ['-m32', '-march=i386'],
    cpp_args: ['-m32', '-march=i386', '-I' + (meson.source_root() / 'bios/stage2')]
)

bios_boot_img = custom_target(
    'bootloader',
    output : 'bootloader.img',
    input : [bios_stage1, bios_stage2],
    command : [kernel_gen, '@BUILD_ROOT@', '@OUTPUT@', '@INPUT0@', '@INPUT1@']
)

bochs = find_program('bochs')
run = run_target(
    'run',
    depends : [bios_boot_img],
    command : [bochs]
)
