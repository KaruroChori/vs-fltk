## Missing for release 0.1.1

### Scripts

- [x] turn off (for now) app linking capabilities
- [ ] Make all paths being used by the c and js pipeline portable.

### Setup github actions

- [ ] ? To run tests and save its reports on some backend
- [ ] ? To run benchmarks and save its reports on some backend (variable system load will destroy performance metrics?)
- [ ] ? To run the full flatpak-builder. For referencehttps://github.com/flatpak/flatpak-github-actions

# Current

All these notes should be progressively removed from here and recorded as github issues.

### XSD -> RNG?

XSD as an XML schema format is not that good. RelaxNG better captures the flexible nature of vs files, so I would be better off considering it as I did for `vs.templ` already.

### Loading XML issues

```
[INFO]     : Loading component file://./two-buttons.xml @ [/app/fl:window/TwoButtons]
[INFO]     : Requested loading of file `file://./two-buttons.xml`
[PANIC]    : Oh no! Loading failed `file://./two-buttons.xml` for 2
[INFO]     : Loading failed, file cannot be opened file://./two-buttons.xml @ [/app/fl:window/TwoButtons]
```

At times it fails, at times it does not. Investigate and check against syntax in specs.

```
    <import src="this://two-buttons.xml" as="TwoButtons" />
```

works but

```
    <import src="file://./two-buttons.xml" as="TwoButtons" />
```

does not somehow. But just `./two-buttons.xml` does? Check why!

### Infrastructural

- [x] Add namespaces for vs elements and fltk elements
- [x] Expose some sort of inverse for apply_prop, where selected fields can be read from a widget.
- [ ] (?) Add special `inherit` value to props to remove the current one and let it be computed based on its parents? I have to decide if we want this one.
- [ ] Serialize the current UI structure in some format to be consumed by other services (AI assistants, speech dispatcher etc)
- [x] Add pipelines for compilation & testing on github/gitea
- [ ] Basic LSP for the vs syntax. Implementation @ https://github.com/KaruroChori/vs-fltk-lsp
- [x] Make some dependencies and their related features toggleable at build-time (tcc, quickjs, lua, wamr).
- [ ] Decide what to do about swift.
- [x] I probably want https://github.com/fmtlib/fmt
- [ ] Signal/event propagation mechanisms
- [x] Add special logging to generate output used by more complex tests. They must also be exposed to scripts.  
       These logs are only recorded if VS_DEBUG_FILE is set to a file name, and will result in a key,value dictionary which is later checked against reference as part of the test.
- [x] Namespace support
  - [x] In the root of app or components search for `xmlns` attributes, and define local namespaces based on `vs`, `vs.templ` & `vs.fltk`.
  - [x] Codegen for reserved elements & attributes in the vs namespace. Like for `vs.fltk`.
- [ ] Evaluate and check if possible to replace sqlite with https://github.com/tursodatabase/libsql as it looks like the better (it supports alteration of foreign keys for example)
- [ ] Installation!
  - [x] Rewrite TS stuff in meson directly
  - [x] Update internal paths to work in both build and installed mode.
  - [ ] Man page
  - [ ] ~~deb (mounted on opt, as most dependencies are needed recent and potentially forked)~~
  - [x] flatpak
    - [x] Swift6 is added, but meson cannot find swiftc. Why?
    - [x] Unable to append args to meson setup
- [ ] Escape vs_debug (tabs).
- [ ] Add TESTING flag in scripts to determine if some content should be run or not.
- [ ] Add contextual information in app and component (keys, policies & paths). Remove them from ui_tree as they can be accessed via root.
  - [ ] Once that is done, properly implement the path type for props, to resolve correctly.

### Scripting

- [ ] Expose path and the fetcher to embedded script at some degree.
- [x] Make errors in C scripts less useless
  - [x] Add `#line` to C library to properly count lines for error location
- [ ] Support for compact scripts
  - [x] C script
  - [ ] JS script

### Components

- [x] Enable codegen
- [x] Simplify codegen for all computed/props types which are not `raw` by writing more or the boilerplate.
- [x] and port the current code over the new codegen architecture.
- [ ] Viewport element.
- [ ] Fix on.failure and on.loading to match specs in `use` element.
- [x] Add properties on containers to set their type/mode whatever.
- [x] Split align property
- [ ] Split font property
- [x] Add debug component

### Paths

- [x] Extend support for more virtual paths (all but those based on sqlite which will be handled later on)

### Dynamic Library linking

- [ ] Add support of loading of so/dylib/dll modules
  - [x] Test linking
  - [x] On http/s & gemini (the cache is now generic)
  - [ ] Without libc?
  - [x] If nobuiltin is not usable, I need to add more of libtcc to meson targets.
- [x] Fix building process of tcc to generate the minimal libraries needed.

### Policies

- [x] Add support for env variables to fully disable native code, internet access and to run headless.  
       Once profiles are ready this will be obsoleted, but for now I need for testing and to let people use it "safely"
- [ ] Policies are so complex to write manually and very easy to mess them up.
      Maybe code generation starting from a schema might be a better option.
- [ ] Support basic embedded script policies.

### Caching

- [ ] Caching all done (no SQLITE, for that there is a different milestone)

  - [x] Source files
    - [x] From fs
    - [x] From network
  - [ ] Pugi XML documents
    - [x] Cache support
    - [ ] Save XML trees while parsing
  - [x] Scripts

- [ ] Caching all done (SQLITE part)
  - [ ] kv
  - [ ] res
  - [ ] secrets
  - [x] DB schemas

# Props, computed, getters and setters

I would like to implement an architectural change how these concepts are handled.  
Schemas will be defining objects for which `getters` and `setters` are defined.  
Some component might allow for arbitrary definitions (already the case for the dispatcher which could be recycled), but most are fixed.  
Props are just calls to setters for which expression evaluation from a string is available.  
They are just an artifact of XML components, native ones for the most part are only going to use `getters` and `setters` directly.

It is also possible to mark specific objects as `persistent`. Those will be recorded as secrets & recovered if so desired.
It is also possible to mark specific objects as `semantic`. Those will be reported in the serialization.

# Supported environments

## TCC

- [x] Add basic support for TCC
- [x] Provide extensive bindings for TCC
  - [x] Stage 0
  - [x] Stage 1 Full subset of libc
  - [x] Stage 2 Full interface for ui tree operations
  - [x] UI base support of module scripts
  - [x] UI base support of getters/setters.
- [ ] Add an alternative backend like Cello
- [ ] Add an alternative backend like Vala (?)

All done for now

## WASM/WASI

Why is WASM so frustrating to work with!?! No compiler can compile itself into a wasm/wasi binary.  
AssemblyScript is a mess that is barely able to generate itself with WASI, but still it requires binaryen distributed alongside.  
Because of a series of requirements, V8 is basically the only runtime capable of running that combo.  
An all LLVM versions for wasm/wasi I tried were failures (or half so).

I am starting to regret java bytecode already.

Notice: the wasmer registry seems to have a small collection of clang builds on it. It might be good enough to start experimenting.  
https://wasmer.io/taybenlor/runno-clang

- [x] Integrate an engine (WAMR)
- [ ] ~~Find a way to have llvm/clang running as wasm an build to wasm (no emscripten)~~ cannot do that
- [ ] ~~Find a way to integrate assemblyscript as a language and embeding the compiler~~ ahah. not working

## QuickJS

- [x] Add quickjs to the repo
- [x] Add basic support for quickjs embedded scripts.
- [x] UI base support of module scripts
- [ ] Support of getters and setters

## LUA

- [ ] Add Lua & LuaJIT to the repo
- [ ] Add baseline support for Lua embedded scripts.

# Documentation

- [x] Provide actual examples.

# Unclassified

## General

- [x] Refactor the ui_tree to better handle different classes of components
- [x] Prepare infrastructure for codegen (no impl yet)
- [x] Move all the tcc stuff in proper pipelines
- [x] Add more components to play with

- [ ] For speed of development, many operations are written recursively.
      The stack is pretty limited, so either there is tail recursion or stacks in heap must be used.
- [ ] Add caching & preloading to avoid making the process of building UI dog slow.
  - [x] Partial implementation to support modules
  - [ ] Partial implementation to load resources in memory within the session (they might be promoted once SQLITE gets integrated)

## References for FFI & Lua

- https://github.com/shajunxing/quickjs-ffi
- https://github.com/q66/cffi-lua
- https://github.com/LuaJIT/LuaJIT
- http://lua-users.org/wiki/SimpleLuaApiExample

Probably I will allow switching between Lua & LuaJIT with a compile flag.
