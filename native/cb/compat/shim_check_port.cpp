// Compile-only check of the shims at the old header paths of port/*: the old API over cb::port. Nothing here runs; it is in
// cb_shim_check, with the old ports that still use them (port/oldlayout, port/__main__), so the shims stay honest until their
// last consumer is refactored.
#include "codebreaker/port/arith16.hpp"

namespace shim_check_port {

// Every entry point of the shims, named so the compiler checks its body.
inline void touch() {
    namespace a16 = codebreaker::port::arith16;
    (void)&a16::run;
    (void)&a16::pass_key;
    (void)&a16::_headers_key;
    (void)&a16::bare;
    (void)&a16::mark_files;
    (void)&a16::_files;
}

}  // namespace shim_check_port
