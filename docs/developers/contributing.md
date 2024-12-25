## Structure of the repo

- **src** where most of the source for **vs.app** and the **vs.fltk** library are located.
- **include** like before for the header files. Directories mostly mirrors what is in `src`.
- **schemas** high level specs, main source of information for documentation and automatic code-gen.
- **commons** extra public files part of every **vs** distribution (some of them are auto-generated).
- **docs** & **examples**: this documentation & and all examples. As part of the build process, they are shipped as part of the commons.
- **bindings** bindings for all languages supported in embedded `script`.
- **test** & **benchmark**: test suite & benchmarks for **vs** & the **vs.fltk** library.
- **scripts** utility scripts (mostly in TS/JS) to handle code generation, the build process and some workflows.
- **experiments** playground where new ideas or semi-standalone prototypes are tested.
- **metadata** information for distribution (icons, manifest files etc.).
- **dist** most of these files are automatically generated and not tracked, the rest of them are needed to distribute the final website with docs.
- **docker** anything related to the docker images used to build `vs`.

## Language & format guidelines

## Variables of environment

They are frequently used for both _benchmarks_ and _tests_.  
They can also be useful for the developer while testing new functionality, so they have been all covered in [here](./env-vars.md) for reference.

## Logging

### Debug logging

**vs** has some features to simplify debugging, mostly to support automatic tests and benchmarks, but they might be useful in other scenarios as well.
`vs::globals::debug` is responsible for that, and it is exposed in several ways:

- within embedded scripts
- via a special xml `debug` tag
- in the `vs.fltk` C interface as well
  While using it you can define a key and value. The current timestamp at nanoseconds resolution is also automatically recorded.  
  Records are saved to a file with name `VS_DEBUG_FILE` if set, otherwise no output will be emitted. Older content is destroyed.  
  The file format is just a simple CSV with horizontal tabs as separator of fields and newlines for rows. The order is _key_, _value_ & _timestamp_.

`vs::globals::debug` should not be confused with the ordinary logging functions which are also exposed in similar ways, but which are generally contextual and they mostly output to `stdout`.

## Testing & Benchmarking

- Static build tests for `vs` to ensure it can be compiled on all and for all supported platforms.
- Static tests/benchmarks on `vs.fltk` functionalities provided by the library, like path resolution, mixin logic etc.
- Runtime tests/benchmarks on `vs`in headless mode with automatic actions, with optional memory profiling.
- Runtime tests on `vs` with a headless x11 server to obtain screenshot to highlight possible visual regressions.

### Testing features of the vs-fltk library

### Memory profile of vs

### Benchmarking features of the vs-fltk library

### Runtime tests in vs

Supported actions are described in [here](./auto-ui-actions.md).

## Programming guidelines

### About exceptions

Exceptions are fully allowed in the CLI at `/src/app`.  
However, they are strongly discouraged anywhere else in the library code, and it is possible they will be fully disabled via `-fno-exceptions` at some point. The main reason is that the UI should be fault-tolerant and provide as much functionality as possible even if parts of it are broken, like malformed XML files, some scripts failing compilation, missing resources and so on. Exceptions, in this sense, are internally handled and semantically supported by providing subtrees to be rendered in case of failure events.  
Exceptions should only be used in those cases when the application **must** stop, either because the error is not recoverable or because it would leave the rest of the application in an inconsistent state.

### Memory allocations

As for memory allocations, spawning small dynamic objects is also discouraged. If possible, stack allocations are a better alternative. Arrays with variable length on stack are totally fine to be used in place of local objects allocated on heap.  
`std::string` is also highly discouraged, make sure `std::string_view` is used instead whenever possible.

### About log levels

Internally these are the log levels supported and their relative semantics:

- **INFO** to present informational messages, often as a result of user requests
- **OK** to notify that a certain operation completed successfully.
- **WARNING** to notify that a certain operation was not able to fully succeed, something should be fixed, but everything is fine.
- **CONTINUE** to notify that a certain operation was skipped by design.
- **PANIC** to notify that a certain operation failed in a way which cannot be recovered (but the application can still run)
- **LOG** to introduce a log without any further connotation.

For example, loading a remote resource:

- If the file was already cached, a `continue` message will be notified
- If no proper answer has been received from the remote host, a `panic` message is generated, since that content cannot be recovered
- If everything loaded fine, an `ok` message can be used
- If the component is using features which are not allowed by policies, `warning` messages can be generated.

Log levels are:

- **NORMAL** to be emitted when running the program normally
- **SILENT** to be emitted when running the program even in silent mode
- **VERBOSE** to be emitted when running the program in verbose mode only
- **DEBUG** to be emitted always when running a debug build or in debugging mode.

Each message must specify their semantic type and log level (normal by default). Usually:

- `panic` messages have a `silent` log level
- `ok` and `continue` messages are usually `verbose`
- `info`, `warning` and `log` are usually `normal`

## Documentation

Documentation is written in markdown. Files within `docs` will be shipped with `vs` and are expected to read by the internal markdown component.  
As such:

- They must be written in [CommonMark](https://commonmark.org/) (with some possible extensions like the block headers for notices or warnings).
- They shall not include HTML within them. XML will be parsed as normal `vs` code.

Files which are written in markdown, but are outside the `docs`, can follow a more relaxed format and target github/gitlab extensions for visualization.
