// Compile-only check of the shims at the old header paths of emulate/emulator and emulate/shadow: the old py:: surface (the Trace record with its
// py containers, the trace options, the process-wide governor and stubbing default, trace_function / recorded, ShadowPPC and the x86 shadow
// factories) over cb/emulate/emulator and cb/emulate/shadow. The shims call the old corpus, prototypes, knowledge and lifters, which cannot be
// linked into cb_tests, so nothing here runs; it is in cb_shim_check so they stay honest until their last consumer is refactored. (Both shims are
// header-only: including them compiles every definition in them.)
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/shadow.hpp"

namespace shim_check_w4b1 {

inline void touch() {
    namespace em = codebreaker::emulate::emulator;
    namespace sh = codebreaker::emulate::shadow;
    (void)&em::trace_function;
    (void)&em::recorded;
    (void)&em::_resolve_func;
    (void)&em::_builtin_argc;
    (void)&em::_uc_reg;
    em::Trace trace;
    em::TraceOptions options;
    options.stub_externals = true;
    (void)trace;
    (void)em::_PARENT;
    (void)em::_COMPARES;
    (void)em::_FLAGLESS;
    (void)&sh::mode;
    (void)&sh::real_mode;
    (void)&sh::Shadow16;
    (void)&sh::Shadow32;
    (void)&sh::ShadowLE;
    sh::ShadowPPC* none = nullptr;
    (void)none;
}

}  // namespace shim_check_w4b1
