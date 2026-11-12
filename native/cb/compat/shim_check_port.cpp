// Compile-only check of the shims at the old header paths of port/*: the old API over cb::port. Nothing here runs; it is in
// cb_shim_check, with the old ports that still use them (port/__main__), so the shims stay honest until their last consumer is
// refactored.
#include "codebreaker/port/arith16.hpp"
#include "codebreaker/port/asm16.hpp"
#include "codebreaker/port/oldlayout.hpp"
#include "codebreaker/port/verify.hpp"

namespace shim_check_port {

// Every entry point of the shims, named so the compiler checks its body.
inline void touch() {
    namespace a16 = codebreaker::port::arith16;
    namespace ver = codebreaker::port::verify;
    (void)&a16::run;
    namespace oldl = codebreaker::port::oldlayout;
    (void)&oldl::run;
    (void)&oldl::declare;
    (void)&oldl::null_reads;
    (void)&oldl::PTR_PARAMS;
    (void)&ver::run_cases;
    (void)&ver::files_of;
    (void)&ver::show;
    (void)&ver::fnmatch;
    (void)&ver::main;
    (void)&ver::services;
    // asm16: the old names the ports that have not been refactored use (port/__main__, asmdata, pre_ansi, sanitize)
    namespace as16 = codebreaker::port::asm16;
    (void)&as16::parse;
    (void)&as16::code_segments;
    (void)&as16::strip_comment;
    (void)&as16::split_ops;
    (void)&as16::number;
    (void)&as16::c_functions;
    (void)&as16::listing_offsets;
    (void)&as16::module;
    (void)&as16::CTypes::_layouts;
    (void)&as16::Port::run;
    (void)&as16::Port::emit;
    (void)&as16::Port::exit_of;
    const py::list<py::str> none;
    const as16::CTypes types(none, none);
    (void)types.field(py::str("struct s"), 0);
    (void)types.elem_sizes(py::str("g"));
    (void)types.member_type(py::str("struct s"), 0);
    (void)types.pointee(py::str("g"));
}

}  // namespace shim_check_port
