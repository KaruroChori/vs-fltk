Pipelines is the way different backends for scripting are supported in `vs`.

## C (native via TCC)

tinycc compiling for native, running C code.

## JS (quickjs)

quickjs runtime on native, running JS code.

## Lua (lua/luajit)

lua/luajit runtime on native, running Lua code.

## External via RPC on UDS

External interface with a different process via unix domain sockets and a remote procedure convention.

## Any (libriscv)

Several compilers can be distributed as RISCV binaries, so a wide range of languages is possible (in principle)

## Any (WAMR)

Several compilers can be distributed as wasm-wasi binaries, so a wide range of languages is possible (in principle).  
In practice the current landscape of WASM support and interoperability sucks... so nothing will happen there I guess.
