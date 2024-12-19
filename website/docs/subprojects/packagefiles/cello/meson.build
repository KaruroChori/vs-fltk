project('cello', 'c')

src = files(
'./src/Function.c',
'./src/GC.c',
'./src/Show.c',
'./src/Cmp.c',
'./src/Len.c',
'./src/Exception.c',
'./src/Table.c',
'./src/Array.c',
'./src/Resize.c',
'./src/Iter.c',
'./src/Alloc.c',
'./src/Tree.c',
'./src/Type.c',
'./src/File.c',
'./src/Get.c',
'./src/String.c',
'./src/Assign.c',
'./src/Doc.c',
'./src/Thread.c',
'./src/Concat.c',
'./src/Start.c',
'./src/Push.c',
'./src/Pointer.c',
'./src/List.c',
'./src/Tuple.c',
'./src/Num.c',
'./src/Hash.c',
)

include_dir = include_directories('include')

cello = library(
    'cello',
    src,
    include_directories: [include_dir],
    install: not meson.is_subproject(),
    override_options : ['c_std=gnu99', 'Wno-unused=true']
)

cello_dep = declare_dependency(
    include_directories: include_dir,
    link_with: cello,
)

pkg_mod = import('pkgconfig')

pkg_mod.generate(
    cello,
    name: 'cello',
    description: 'A C99 library that brings higher level programming to C.',
    libraries: cello,
    version: meson.project_version(),
)

