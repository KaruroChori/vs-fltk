Embedded scripts are small programs which can be integrated directly in the XML structure of a `vs` component or application.  
Several languages and related backends are available:

- C running on TinyCC
- JS running on QuickJS
- Lua running on Lua/LuaJIT
- Any language with a backend which can run on libriscv

WASM support was originally planned, but the lack of good tooling put it in the freezer for now.

When working with embedded scripts there are few things to consider:

- Different runtimes can be more or less safe. Please refer to the [safety policies](../users/safety-policies.md) for more context on this.
- The runtime exposed by this language might not be what you would expect. For example, C has no libc, only a subset of it at best, and functions which are specific to `vs`.
- While there is an attempt to provide feature parity between the different backends, this is not always possible. Differences (for now) are [tracked here](https://github.com/KaruroChori/vs-fltk/issues/54)
- There is no restriction on what you can do within embedded scripts, but it is clear there should be no complex computation. Filesystem and network access might be available with some heavy restrictions and depending on policies. Most applications should not assume they can.

## The `script` tag

### Modules vs Single User Scripts

### Compact scripts

### Exported symbols

### Imported symbols
