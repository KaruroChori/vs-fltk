There are two main classes of tests for `vs`:
- Marginal tests are written in C/C++/Swift and link directly with `libvs`. They are meant to test specific aspects of the library in isolation.
- Full stack tests are based off a fully compiled `vs` and will run more or less as they would for the final user.  
  The main difference being potential tracking and memory profiling, as well as the execution of automatic actions.