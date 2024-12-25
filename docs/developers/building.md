> [!IMPORTANT]  
> It was decided that distribution & building will be split.  
> Distribution is to be handled [here](https://github.com/lazy-eggplant/vs.autobuilds), while everything build-related stays.  
> Docker images are excluded, as they are meant for developers of `vs`, not distribution of the final runtime.

## Supported platforms

At the moment, only Linux is supported for building and running `vs` as its primary target.  
It is likely that all major CPU architectures are supported, and several POSIX systems can also be targetted.

This is just temporary limitation, as all dependencies being used by this project are highly portable, but my own code and the build system are probably not.

| **Platform**     | **Arch** | **Building** | **Running** |
| ---------------- | :------: | :----------: | :---------: |
| debian-trixie    |  amd64   |      ✅      |     ✅      |
| debian-trixie    |  arm64   |      ❓      |     ❓      |
| debian-trixie    |  risc64  |      ❓      |     ❓      |
| freedektop-24.08 |  amd64   |     ✅\*     |    ✅\*     |
| macos-13         |  amd64   |      ❓      |     ❓      |
| macos-14         |  arm64   |      ✅      |     ❓      |
| win64            |  amd64   |      ❌      |     ❓      |

In addition to this, the building environment is also distributed via docker images.

### Distribution

There are plans to distribute both **stable** and **nightly** releases of vs.  
Please, be mindful that nightly releases can be extremely broken, and for safety reasons they will only be offered as flatpaks.

Distribution formats which are supported:

- `flatpak` targetting the latest stable freedesktop environment

Distribution formats which are expected to be supported (at some point):

- `deb` for the latest stable and experimental of debian (multiarch)
- `aur` for arch linux and derivatives
- `brew` for macos

Installing directly via `meson` is very much not suggested if you don't want to break your system.

## Configuration flags

Configuration flags, like for any meson project, are in `meson.options`.  
In brief, you can disable most features of the runtime if you so wish.  
This can be good to meet specific safety targets by removing some available options, or to optimize it on embedded applications where the overhead of some parts might be excessive.

Builds distributed from this repository are based on the default options unless explicitly specified.

## Building requirements

You will need a proper Linux environment, with a modern C++ toolchain installed.  
Specifically, I suggest `clang-19` or higher, as this repo is using modern C23 features like `#embed` to make everyone's (my) life a bit easier.
In addition to that, this repo makes use of:

- [meson](https://mesonbuild.com/) as its main build system. Any recent-ish version will do (unless you want `zig` to simplify cross-compiling; for that, >= 1.60 is needed)
- [bun](https://bun.sh/) as the ts/js runtime to support all the code generation tasks and some of the more complex pipelines.  
   I hate bash, and this is what replaces it for any complex task.
- [swiftc](https://www.swift.org/documentation/swift-compiler/) barely used for now, but many of the native components shipped within `vs` will be written in swift (or so I am planning). Version 6 or higher is needed.

### FLTK

If your system provides a modern version of `fltk>=1.4`, that will be used by default.  
If not present, you are likely needing few more [dependencies](https://github.com/fltk/fltk/blob/master/README.Unix.txt) depending on your distribution.  
On debian-like systems the followings are needed:

- **libpng-dev**
- **libglu1-mesa-dev**
- **libpango1.0-dev**

There are also some more or less optional dependencies:

- **libcurl-dev**, unless you are trying to compile a custom version without network support, or http only which are supported.
- [gnuplot](http://www.gnuplot.info/) if you want to generate plots from testing & benchmarking reports.
- [flatpak-builder](https://docs.flatpak.org/en/latest/flatpak-builder.html) if you plan on packing and distributing a flatpak of `vs` (usually not needed, read more about it later in this document)

### Additional documentation

To generate the additional documentation for distribution and consumption:

- [mkdocs](https://www.mkdocs.org/) and several packages for themes and extensions.
  - `mkdocs`
  - `mkdocs-github-admonitions-plugin`
  - more to be documented
- [doxygen](https://www.doxygen.nl/) for documentation generated from the code itself.

### IDE integrations

There is an ongoing effort to better support `vs` with a proper [lsp](https://github.com/lazy-eggplant/vs.lsp).  
In the same repo there is also a vscode client extension.

`vs` is expected to leverage this LSP directly in its embedded editor once integrated.

## Docker

A docker image is available that automatically covers all the build dependencies. See
[docker/README.md](../docker/README.md) for further documentation.

## Building process

> [!NOTE]  
> This project uses meson, so there are no git submodules for the most part.
> One exception is made for `flatpak-builder` to be usable, but it is being deprecated.  
> If you plan on using it to generate new flatpak images, please ensure submodules are also cloned with the rest of the repository.

Start by installing all the `bun` dependencies needed:

```bash
bun install
```

Then run the following NPM scripts:

```bash
bun run codegen                     #Initial codegen from schemas
bun run meson-setup.release         #Set up the meson build directory.
bun run meson-build                 #Build everything!
```

You might want to use `meson-setup.clang-release` to use clang-19 if found on your system, and your default compiler or choice might not support some modern functions used in this project.

To perform tests and benchmarks:

```bash
bun run test
bun run benchmark
```

To run the dev demo where features under development are usually being tested:

```bash
bun run vs.example
```

## Installation

> [!WARNING]  
> Don't install it via `meson install ...` without a custom `DESTDIR` set, as shown by its wrapper in `package.json` script.  
> Some library names will most surely clash with those already installed on your system, and will be overridden.  
> Or future system updates might break `vs`.  
> Also consider https://github.com/KaruroChori/vs-fltk/issues/3 and its impact.  
> `vs.fltk` is using very recent versions which have not been rolled out yet in most distribution. Or even worse, custom forks.

`bun run meson-install` is now implemented & tested. By default, it installs everything in `./build/dist`.

### Flatpak

> [!IMPORTANT]  
> Flatpak in this repos is being deprecated. It will be fully relocated and removed from here before v0.2.1.

> [!NOTE]  
> You need a modern version of `flatpak-builder` to be installed.  
> Some EOL or LTS will not ship with one. Anything > 1.4 should be ok.  
> Also, make sure your runtime is not using a cmake version in $[3.31.0,3.31.2)$.  
> A regression later fixed prevents building cmake via meson. Tracked [here](https://github.com/KaruroChori/vs-fltk/issues/46).

Flatpak is not really the ideal approach to deliver `vs` due to the intrinsic cost of having a separate runtime already installed on each system.  
Also, its sandboxing & permission system might raise some issues (but also help to mitigate the current lack of safety features).  
Still, it is good to support it at these early stages, since many libraries & build dependencies are bleeding-edge, and other forms of distribution are not as convenient.

```sh
flatpak --user remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
```

if you never configured `--user` on your system.  
Then

```sh
bun run codegen           #If not run already
bun run flatpak-builder
```
