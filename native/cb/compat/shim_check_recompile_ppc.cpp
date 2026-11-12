// Compile-only check of the shims at the old header paths of recompile/powerpc and recompile/config: the old API over
// cb::recompile. Nothing here runs; it is in cb_shim_check, so the shims stay honest until their last consumer (recompile/project,
// recompile/x360project) is refactored.
#include "codebreaker/recompile/config.hpp"
#include "codebreaker/recompile/powerpc.hpp"
#include "codebreaker/recompile/x360project.hpp"

namespace shim_check_recompile_ppc {

inline void touch() {
    namespace pc = codebreaker::recompile::powerpc;
    namespace cf = codebreaker::recompile::config;
    (void)&pc::emit_insn;
    (void)&pc::recompile_function;
    (void)&pc::runtime_header;
    (void)&pc::_emit_forward_decls;
    (void)&pc::recompile_image;
    (void)&pc::compiler;
    (void)&pc::kernel_includes;
    (void)&pc::check_compiles;
    (void)&pc::main;
    (void)&pc::set_root;
    const pc::Agg agg;
    (void)agg.parts;
    (void)&cf::helpers;
    (void)&cf::jump_buffers;
    (void)&cf::_start_before;
    (void)&cf::_end_of_run;
    (void)&cf::read_switch_tables;
    (void)&cf::write_switch_tables;
    (void)&cf::extra_functions;
    (void)&cf::make;
    (void)cf::REGISTER_SETTINGS;
    (void)&codebreaker::recompile::x360project::scaffold;   // (uses config's shim)
}

}  // namespace shim_check_recompile_ppc
