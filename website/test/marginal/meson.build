vg = find_program('valgrind', required: false)
if vg.found()
    add_test_setup(
        'valgrind',
        exe_wrapper: [
            join_paths(meson.current_source_dir(), '../scripts/valgrind_wrap.sh'),
        ],
    )
endif

to_pass = [
    'integration-tcc',
    'integration-qjs',
    'integration-lua',

    'resolver-paths',
    'resolver-fs',
    'resolver-networking',

    'static-xml',
]
to_fail = []

foreach case : to_pass
    exe = executable(
        'test_' + case,
        './' + case + '.cpp',
        dependencies: [vs_fltk_dep],
        cpp_args: '-DIS_TESTING=true',
    )
    test(
        'test_' + case,
        exe,
        env: [
            'VS_HEADLESS=true',
            'VS_DEBUG_FILE="./out/vs-test-' + case + '.log',
        ],
    )
endforeach

foreach case : to_fail
    exe = executable(
        'test_' + case,
        './' + case + '.cpp',
        dependencies: [vs_fltk_dep],
        cpp_args: '-DIS_TESTING=true',
    )
    test(
        'test_' + case,
        exe,
        should_fail: true,
        env: [
            'VS_HEADLESS=true',
            'VS_DEBUG_FILE="./out/vs-test-' + case + '.log',
        ],
    )
endforeach