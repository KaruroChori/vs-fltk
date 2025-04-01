executable(
    'enums',
    ['./enums.cpp'],
    dependencies: [],
    override_options: ['c_std=c11', 'cpp_std=c++20'],
    install: false,
)

executable(
    'parallel-load',
    ['./parallel-load.cpp'],
    dependencies: [pugixml_dep],
    override_options: ['c_std=c11', 'cpp_std=c++20'],
    install: false,
)

executable(
    'xml-as-map',
    ['./xml-as-map.cpp'],
    dependencies: [pugixml_dep],
    override_options: ['c_std=c11', 'cpp_std=c++20'],
    install: false,
)

#executable(
#    'riscv',
#    ['./riscv.cpp'],
#    dependencies: [
#        pugixml_dep, 
#        riscv_dep, 
#        #libtcc_rv64_dep
#    ],
#    override_options: ['c_std=c23', 'cpp_std=c++20'],
#    install: false,
#)

executable(
    'parse-elf',
    ['./parse-elf.cpp'],
    dependencies: [],
    override_options: ['c_std=c23', 'cpp_std=c++20'],
    install: false,
)