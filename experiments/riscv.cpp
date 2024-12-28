#include <iostream>
#include <libriscv/machine.hpp>
#include <libtcc.h>
#include <pugixml.hpp>
#include <vector>
#include <span>

/*
- How to get a buffer with the elf and don't write it to a file
- Check flags needed to crosscompile
- Ideally no linux support in the riscv env, just freestanding with my syscalls.
*/

std::vector<uint8_t> cc(const char *code) {
  TCCState *ctx = tcc_new();
  tcc_set_output_type(ctx, TCC_OUTPUT_MEMORY);
  tcc_set_options(ctx,"-nostdlib -oformat=binary");
  //Hardcoded since this is just meant to be an example to prototype a minimum viable functionality
  tcc_add_sysinclude_path(ctx, "/archive/shared/projects/vs-fltk/build/commons/bindings/native/tcc/include/");
  tcc_add_include_path(ctx, "/archive/shared/projects/vs-fltk/experiments/riscv/");
  tcc_add_file(ctx,"/archive/shared/projects/vs-fltk/experiments/riscv/start.c");
  tcc_add_file(ctx,"/archive/shared/projects/vs-fltk/experiments/riscv/main.c");
  /*tcc_compile_string(ctx, R"(
        int main(){
            //printf("BANANANAAAA\n");
			int a  = 0;
			int b = 4;
			if(a>b)return 1;
            return 0;
        }
    )");*/
  // tcc_relocate(ctx);
  union {
    char str[];
    struct {
      char prefix = '\e';
      uint8_t* buffer = nullptr;
      size_t size = 0x0;
    }__attribute__((packed));
  } fakename;
  tcc_add_symbol(ctx, "ss", (void*)+[](){});
  tcc_output_file(ctx, fakename.str);
  tcc_delete(ctx);
  std::cout << "Size: " << fakename.size << "\n";

  return std::vector<uint8_t>(fakename.buffer,fakename.buffer+fakename.size);
}

int main(int argc, const char **argv) {
  cc(nullptr);

  // Load ELF binary from file
  const std::vector<uint8_t> binary /* = ... */;

  using namespace riscv;

  // Create a 64-bit machine (with default options, see: libriscv/common.hpp)
  Machine<RISCV64> machine{binary};

  // Add program arguments on the stack, and set a few basic
  // environment variables.
  machine.setup_linux({"myprogram", "1st argument!", "2nd argument!"},
                      {"LC_TYPE=C", "LC_ALL=C", "USER=groot"});

  // Add all the basic Linux system calls.
  // This includes `exit` and `exit_group` which we will override below.
  machine.setup_linux_syscalls();

  // Install our own `exit_group` system call handler (for all 64-bit machines).
  Machine<RISCV64>::install_syscall_handler(
      94, // exit_group
      [](Machine<RISCV64> &machine) {
        int code = machine.sysarg<int>(0);
        printf(">>> Program exited, exit code = %d\n", code);
        machine.stop();
      });

  // This function will run until the exit syscall has stopped the
  // machine, an exception happens which stops execution, or the
  // instruction counter reaches the given 5bn instruction limit:
  try {
    machine.simulate(5'000'000'000ull);
  } catch (const std::exception &e) {
    fprintf(stderr, ">>> Runtime exception: %s\n", e.what());
  }
}