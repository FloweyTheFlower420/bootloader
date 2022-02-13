project('kernel', 'cpp')

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
        '-T' + (meson.source_root() / 'boot/boot.lds'),
        '-static',
        '-nostdlib',
        '-m32'
    ],
    c_args: ['-m32', '-march=i386'],
    cpp_args: ['-m32', '-march=i386']
)

bios_stage2 = executable('bios-stage2.h',
    'bios/stage2/driver/disk/disk.cpp',
    'bios/stage2/driver/fs/fat.cpp',
    'bios/stage2/main.cpp',
    'bios/stage2/realmode/e820.cpp',
    'bios/stage2/realmode/realmode_interrupt.cpp',
    'bios/stage2/realmode/video.cpp',
    'bios/stage2/utils.cpp',
    'lib/debug.cpp',
    'lib/font.cpp',
    'lib/term.cpp',
    'bios/stage2/realmode/a20.S',
    link_args: [
        '-T' + (meson.source_root() / 'stage2/link.lds'),
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
