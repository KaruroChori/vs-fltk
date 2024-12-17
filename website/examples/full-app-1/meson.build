project(
    'app',
    ['cpp'],
    default_options: ['cpp_std=c++23'],
)

cmake = import('cmake')

shared_library(
    'app',
    [
        './src/main.cpp',
    ],
    #dependencies: vs_fltk_deps,
    include_directories: ['include'],
    override_options: ['cpp_std=gnu++23'],
)