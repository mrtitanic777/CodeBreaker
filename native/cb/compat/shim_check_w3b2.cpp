// Compile-only check of the shim at the old header path of decompile/describe: the old py:: surface (the Description record with its py
// containers, the registries, the decoder stand-in, every function) over cb/decompile/describe. The shim calls the old symbol tables and
// the old lanes, which cannot be linked into cb_tests, so nothing here runs; it is in cb_shim_check so the shim stays honest until its
// last consumer is refactored. (The shim is header-only: including it compiles every definition in it.)
#include "codebreaker/decompile/describe.hpp"

namespace shim_check_w3b2 {

inline void touch() {
    namespace d = codebreaker::decompile::describe;
    (void)&d::selector;
    (void)&d::address;
    (void)&d::where;
    (void)&d::le_address;
    (void)&d::_arch;
    (void)&d::decoder16;
    (void)&d::_one;
    (void)&d::set_capstone_dll;
    (void)&d::inside;
    (void)&d::function_names;
    (void)&d::decorated_names;
    (void)&d::import_names16;
    (void)&d::function_starts;
    (void)&d::describe32;
    (void)&d::functions;
    (void)&d::hidden_functions;
    (void)&d::prologues_after_ends;
    (void)&d::jump_by_return;
    (void)&d::return_jumps;
    (void)&d::returns;
    (void)&d::callers_say;
    (void)&d::forwards;
    (void)&d::popped_by_hand;
    (void)&d::args_read;
    (void)&d::windows_far_prologue;
    (void)&d::result_bytes;
    (void)&d::jump_tables;
    (void)&d::call_overrides;
    (void)&d::results;
    (void)&d::_caller_cleanup;
    (void)&d::callers_read_dx;
    (void)&d::register_helper;
    (void)&d::callers_read_ax;
    (void)&d::register_arguments;
    (void)&d::model_line;
    (void)&d::describe16;
    (void)&d::describe_le;
    (void)&d::describe_dos;
    (void)&d::describe_nes;
    d::_DxReturns* none = nullptr;
    (void)none;
}

}  // namespace shim_check_w3b2
