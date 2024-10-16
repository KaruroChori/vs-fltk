## Building requirements
You will need a proper Linux environment, with a modern C++ toolchain installed.  
In addition to that, this repo makes use of:
- [meson](https://mesonbuild.com/) as its main build system
- [bun](https://bun.sh/) as the ts/js runtime to support some codegen and the more complex releasing pipelines.  
   I hate bash, and this is what replaces it.
- [libcurl] unless you are trying to compile a release without it.

At the moment only Linux is supported, probably on any of the major CPU architecture. Possibly some more UNIX-like systems.  
This is just temporary limitation, as all dependencies are portable, but my own code is not.

## Building process
Run the following npm scripts:
```bash
bun run codegen             #Initial codegen from schemas
bun run meson-configure     #Set up the meson builddir
bun run vs                  #Compile and run the main application with the demo xml.
```

To perform tests:
```bash
bun run meson-test
```

To run my dev demo:
```bash
bun run vs
```

`meson-install` is not implemented yet. `meson-release` works, but it is only meant for automatic pipelines.

### Patches
If not using a precompiled version of sqlite on your system, the `meson.build` of its amalgamate might need patching:
```
  override_options: ['c_std=gnu23'],
```
at the end of the library generation. The issue is tracked [here](https://github.com/mesonbuild/wrapdb/issues/1747)

## A word on codegen
A significant portion of code in this repository is generated automatically and does not ship with your `git clone`.  
Make sure you run `bun run codegen` before you attempt any further step with meson. I will probably integrate it in the main build file itself at some point.  

The main source for automatic code generation is located in `/schemas`. The json component definitions are compiled down into C++ classes, typescript type definitions, XSD schemas and XML data used in the embedded editor of `vs`.  
Any component shipped with `vs` (not those externally distributed in `/components`) must have a json schema definition, even if their class definition is not automatically generated. 

## Structure of the repo
- **src** where most of the source for `vs` and the `vs-fltk` library are located.
- **include** as before for the header files.
- **test** test suite for `vs` & `vs-fltk` library.
- **experiments** playground where new ideas or semi-standalone prototypes are tested.
- **components** native components to ship alongside `vs` but not embedded in `vs`.
- **docs** this documentation.
- **bindings** bindings for all languages supported in `script`.
- **scripts** utility scripts for the building process or distribution.
- **schemas** high level specs, source of information for documentation and automatic codegen.

