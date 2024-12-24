project(
    'vs-fltk',
    ['c', 'cpp', 'swift'],
    #<inject-version>
    version: '0.1.2-alpha',
    #</inject-version>
    meson_version: '>= 1.1',
    default_options: ['c_std=gnu23', 'cpp_std=gnu++23'],
)
cmake = import('cmake')

cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')

vs_fltk_flags = ['-Wno-c23-extensions', '-Wno-vla-cxx-extension', '-Wno-unused-variable']
vs_fltk_deps = []

need_pugixml = false
need_fltk = false

# Full dependency list for vs_fltk

# Process options

vs_fltk_flags += '-DVS_COMMONS_DIR="' + join_paths(get_option('prefix'), get_option('datadir'), 'vs-fltk') + '/"'

if get_option('use_tcc')
    vs_fltk_flags += '-DVS_USE_TCC'
endif

if get_option('use_quickjs')
    vs_fltk_flags += '-DVS_USE_QJS'
endif

if get_option('use_wamr')
    vs_fltk_flags += '-DVS_USE_WAMR'
endif

if get_option('use_vm_riscv')
    vs_fltk_flags += '-DVS_USE_VS_RISCV'
endif

if get_option('use_lua_base') and get_option('use_lua_jit')
    error(
        'Only lua or luajit can be used. At least one must be disabled from the options',
    )
endif

if get_option('use_lua_base')
    vs_fltk_flags += '-DVS_USE_LUA'
endif

if get_option('use_lua_jit')
    vs_fltk_flags += '-DVS_USE_LUAJIT'
endif

if get_option('use_networking_curl') and get_option('use_networking_simple')
    error(
        'Only curl or simple can be used for networking. At least one must be disabled from the options',
    )
endif

if get_option('use_networking_curl')
    vs_fltk_flags += '-DVS_USE_NETWORKING_CURL'
endif

if get_option('use_networking_simple')
    vs_fltk_flags += '-DVS_USE_NETWORKING_SIMPLE'
endif

if get_option('use_system_pugixml')
    pugixml_dep = dependency('pugixml', version: '>=1.14', required: false)
    if pugixml_dep.found() == false
        need_pugixml = true
    endif
endif
if need_pugixml or get_option('use_system_pugixml') == false
    #Used by pugi to enable compact mode.
    #I might want to disable execptions too.
    pugixml_proj = subproject('pugixml', default_options: ['compact=true'])
    pugixml_dep = pugixml_proj.get_variable('pugixml_dep')
endif

if get_option('use_system_fltk')
    libfltk_dep = dependency(
        'FLTK',
        version: '>=1.4.0',
        modules: ['fltk::fltk', 'fltk::images', 'fltk::forms'],
        required: false,
    )
    if libfltk_dep.found() == false
        #For MacOS14 assuming a brew dependency because there is something broken in there.
        libfltk_dep = dependency('fltk', required: false)
        if libfltk_dep.found() == false
            need_fltk = true
        endif
    endif
endif
if need_fltk == true or get_option('use_system_fltk') == false
    opt_fltk = cmake.subproject_options()
    opt_fltk.add_cmake_defines(
        {
            'FLTK_BUILD_FLUID': false,
            'FLTK_BUILD_EXAMPLES': false,
            'FLTK_BUILD_TEST': false,
            'FLTK_BUILD_OPTIONS': false,
            'FLTK_BUILD_SHARED_LIBS': true,
            'CMAKE_POSITION_INDEPENDENT_CODE': true,
            'CMAKE_INSTALL_LIBDIR': get_option('libdir'),
        },
    )

    if get_option('force_x11_backend') == true
        opt_fltk.add_cmake_defines({'FLTK_BACKEND_X11': true})
    endif

    libfltk_proj = cmake.subproject('libfltk', options: opt_fltk)
    libfltk_dep = [
        libfltk_proj.dependency(['fltk-shared']),
        libfltk_proj.dependency(['fltk-images-shared']),
        libfltk_proj.dependency(['fltk-forms-shared']),
    ]
    #libfltk_gl_dep = libfltk_proj.dependency(['fltk-gl'])
endif

if get_option('use_wamr')
    opt_wamr = cmake.subproject_options()
    opt_wamr.add_cmake_defines(
        {
            'WAMR_BUILD_INTERP': 1,
            'WAMR_BUILD_PLATFORM': target_machine.system(),
            'CMAKE_INSTALL_LIBDIR': get_option('libdir'),
        },
    )
    wamr_proj = cmake.subproject('wamr', options: opt_wamr)
    wamr_dep = wamr_proj.dependency('wamr')

    vs_fltk_deps += wamr_dep

endif

if get_option('use_vm_riscv')
    opt_riscv = cmake.subproject_options()
    opt_riscv.add_cmake_defines(
        {
            'CMAKE_POSITION_INDEPENDENT_CODE': true,
            'CMAKE_INSTALL_LIBDIR': get_option('libdir'),
        },
    )
    riscv_proj = cmake.subproject('libriscv', options: opt_riscv)
    riscv_dep = riscv_proj.dependency('riscv')

    vs_fltk_deps += riscv_dep

endif

if get_option('use_quickjs')
    opt_quickjs = cmake.subproject_options()
    opt_quickjs.add_cmake_defines(
        {
            'CMAKE_POSITION_INDEPENDENT_CODE': true,
            'CMAKE_INSTALL_LIBDIR': get_option('libdir'),
        },
    )

    quickjs_proj = cmake.subproject('quickjs', options: opt_quickjs)
    quickjs_dep = quickjs_proj.dependency('qjs')

    vs_fltk_deps += quickjs_dep

endif

if get_option('use_tcc')
    libtcc_proj = subproject('libtcc')
    libtcc_dep = libtcc_proj.get_variable('libtcc_dep')

    vs_fltk_deps += libtcc_dep
endif

#TODO: 'CMAKE_INSTALL_LIBDIR': get_option('libdir')
md4c_proj = cmake.subproject('md4c')
md4c_dep = md4c_proj.dependency('md4c')

opt_fmt = cmake.subproject_options()
opt_fmt.add_cmake_defines(
    {
        'CMAKE_POSITION_INDEPENDENT_CODE': true,
        'BUILD_SHARED_LIBS': false,
        'CMAKE_INSTALL_LIBDIR': get_option('libdir'),
    },
)
fmt_proj = cmake.subproject('fmt', options: opt_fmt)
fmt_dep = fmt_proj.dependency('fmt')

hashlib_proj = subproject('hashlib')
hashlib_dep = hashlib_proj.get_variable('hashlib_dep')

json_proj = subproject('nlohmann_json')
json_dep = json_proj.get_variable('nlohmann_json_dep')

#TODO: Make it a system dependency
vs_templ_proj = subproject('vs-templ')
vs_templ_dep = vs_templ_proj.get_variable('vs_templ_dep')

treesitter_proj = subproject('tree-sitter')
treesitter_dep = treesitter_proj.get_variable('tree_sitter_dep')

#uv_proj = subproject('libuv')
#uv_dep = uv_proj.get_variable('libuv_dep')

mio_proj = cmake.subproject('mio')
mio_dep = mio_proj.dependency('mio')

sqlitecpp_dep = dependency('sqlitecpp', required: false)
if not sqlitecpp_dep.found()
    sqlitecpp_dep = cxx.find_library('SQLiteCpp', required: false)
    if not sqlitecpp_dep.found()
        sqlitecpp_dep = dependency(
            'sqlitecpp',
            fallback: ['sqlitecpp', 'sqlitecpp_dep'],
            default_options: 'default_library=static',
        )
    endif
endif

sqlite_dep = dependency(
    'sqlite3',
    version: '>=3.0.0',
    fallback: ['sqlite3', 'sqlite3_dep'],
    default_options: 'default_library=static',
)

if get_option('use_networking_curl') and get_option('use_networking_simple')
    error('Only one networking solution can be selected between curl and simple')
endif

if get_option('use_networking_curl')
    curl = dependency('libcurl', required: true)
    vs_fltk_deps += curl
endif

if get_option('use_networking_simple')
    libhttp_proj = subproject('libhttp')
    libhttp_dep = libhttp_proj.get_variable('http_dep')

    vs_fltk_deps += libhttp_dep
endif

include_dirs = include_directories(['./include'])

# Prepare `vs`

vs_fltk_deps += [
    libfltk_dep,

    pugixml_dep,
    json_dep,
    vs_templ_dep,
    fmt_dep,
    mio_dep,
    #uv_dep,    The one provided with the system if present should be fine for our tasks
    hashlib_dep,

    sqlite_dep,
    sqlitecpp_dep,

    md4c_dep,
    treesitter_dep,
]

subdir('./src/components')

vs_fltk = shared_library(
    'vs-fltk',
    [
        './src/cache/memory-storage.cpp',
        './src/cache/kv-storage.cpp',
        './src/cache/res-storage.cpp',
        './src/cache/secrets.cpp',

        './src/utils/resources.cpp',
        './src/utils/paths.cpp',
        './src/utils/strings.cpp',
        './src/utils/env.cpp',
        './src/utils/policies.cpp',
        './src/utils/policies.internal.cpp',
        './src/utils/tcc-wrap.cpp',

        './src/singletons.cpp',
        './src/fetcher.cpp',
        './src/loader.cpp',

        src_components,
        autogen_components,

        './src/ui-frame.cpp',
        './src/ui.cpp',
        './src/ui-base.cpp',

        './src/xml2native.cpp',

        './src/ui-tree.cpp',
        './src/ui-tree.xml.cpp',
        './src/ui-tree.wasm.cpp',
        './src/ui-tree.cnative.cpp',
        './src/ui-tree.dylib.cpp',
        './src/ui-tree.riscv.cpp',

        './src/ui-fields.cpp',

        './src/pipelines/tcc-c.cpp',
        './src/pipelines/tcc-cello.cpp',
        './src/pipelines/quickjs-js.cpp',
        './src/pipelines/wasm-wat.cpp',
        './src/pipelines/lua-lua.cpp',

        './src/themes/default-cute.cpp',

        './src/cbindings/vs.cpp',
        './src/cbindings/components.autogen.cpp',

        './src/globals.cpp',
    ],
    dependencies: vs_fltk_deps,
    include_directories: include_dirs,
    override_options: ['c_std=gnu23', 'cpp_std=gnu++23'],
    cpp_args: vs_fltk_flags,
    c_args: vs_fltk_flags,
    install: true,
)

vs_fltk_dep = declare_dependency(
    include_directories: include_dirs,
    link_with: vs_fltk,
)

cello_project = subproject('cello')
cello_dep = cello_project.get_variable('cello_dep')

shared_library('vs-cello', dependencies: [cello_dep])

#TODO add also a vs_codegen to introduce that step within meson instead of keeping it separate.
#I need to run it not from the builddir but from the root.
#vs_codegen_cmd = find_program('./scripts/codegen/index.ts')
#vs_codegen = custom_target(command: 'vs-codegen', output: 'vs-codegen', command: [vs_codegen_cmd], depends:[])

vs_commons_cmd = find_program('./scripts/gen-commons.sh')
vs_commons = custom_target(
    'vs-commons',
    output: 'commons',
    command: [vs_commons_cmd],
    depends: [],
    build_always_stale: true,
)

executable(
    'vs',
    [
        './src/app/main.cpp',
        './src/app/settings.cpp',
        './src/app/updater.cpp',
    ],
    dependencies: [vs_fltk_dep],
    link_depends: [vs_commons],
    override_options: ['c_std=gnuc23', 'cpp_std=gnu++23'],
    install: true,
    #install_rpath: ''
)

executable(
    'swift_vs',
    [
        './src/swift-app/app.swift',
    ],
    dependencies: [vs_fltk_dep],
)

subdir(['./experiments/'])

subdir(['./metadata/'])

if get_option('tests')
    subdir(['./test/'])
    subdir(['./benchmark/'])
endif

# Install stuff

install_headers(
    [
        './include/cbindings/vs.h',
        './include/cbindings/components.autogen.h',
        './include/cbindings/module.modulemap',
    ],
    preserve_path: false,
    subdir: 'vs-fltk',
)

install_subdir(
    'commons',
    exclude_files: ['.gitignore'],
    install_dir: 'share/vs-fltk',
    strip_directory: true,
)
install_subdir('docs', install_dir: 'share/vs-fltk')
install_data(
    ['./LICENCE.md', './README.md', './RELEASE.md'],
    install_dir: 'share/vs-fltk/',
)

#install_man()

### MESON DEVENV PROFILE
#TODO: Add a custom profile to devenv to avoid "polluting" or making user's one invalid.
meson.add_devenv({'VS_LOG_LEVEL': 'debug'})

pconf = import('pkgconfig')
pconf.generate(
    vs_fltk,
    description: 'VS fltk library (C interface only)',
    url: 'https://github.com/KaruroChori/vs-fltk/',
    version: meson.project_version(),
)