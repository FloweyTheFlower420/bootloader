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
    '-fcf-protection=none',
    '-fno-threadsafe-statics',
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

boot = executable('boot.bin',
    'boot/boot.S',
    link_args: [
        '-T' + (meson.source_root() / 'boot/boot.lds'),
        '-static',
        '-nostdlib',
        '-m16'
    ],
    c_args: ['-m16', '-march=i386'],
    cpp_args: ['-m16', '-march=i386']
)

stage2_rmode = executable('stage2-realmode.bin',
    'stage2/realmode/realmode.cpp',
    'stage2/realmode/e820.cpp',
    'stage2/realmode/video.cpp',
    'stage2/realmode/fetch_far.S',
    link_args: [
        '-T' + (meson.source_root() / 'stage2/realmode/link.lds'),
        '-static',
        '-nostdlib',
        '-m16'
    ],
    c_args: ['-m16', '-march=i386'],
    cpp_args: ['-m16', '-march=i386']
)

bootloaderimg = custom_target(
    'bootloader',
    output : 'bootloader.img',
    input : [boot, stage2_rmode],
    command : [kernel_gen, '@BUILD_ROOT@', '@OUTPUT@', '@INPUT0@', '@INPUT1@']
)

bochs = find_program('bochs')
run = run_target(
    'run',
    depends : [bootloaderimg],
    command : [bochs]
)
