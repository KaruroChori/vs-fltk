vg = find_program('valgrind', required: false)
if vg.found()
    add_test_setup(
        'massif',
        exe_wrapper: [join_paths(meson.current_source_dir(), 'massif_wrap.sh')],
    )
endif

benchmark_cases = ['test']

foreach case : benchmark_cases
    exe = executable('test_' + case, './' + case + '.cpp', dependencies: [vs_fltk_dep])
    benchmark('benchmark_' + case, exe)
endforeach