// Compile-only check of the shims at the old header paths of emulate/windows16, emulate/driver and ui/mcp_server: the old py:: surface (the 16-bit trace record
// and its options, the stand-ins, probe / record / recorded; the driver image, run record and runner) over cb/emulate/windows16 and
// cb/emulate/driver. The shims call the old knowledge base, symbols and corpus, which cannot be linked into cb_tests, so nothing here runs; it is in
// cb_shim_check so they stay honest until their last consumer is refactored. (Both shims are header-only: including them compiles every
// definition in them.)
#include "codebreaker/emulate/driver.hpp"
#include "codebreaker/emulate/windows16.hpp"
#include "codebreaker/ui/mcp_server.hpp"

namespace shim_check_w5b1 {

inline void touch() {
    namespace w16 = codebreaker::emulate::windows16;
    namespace dr = codebreaker::emulate::driver;
    (void)&w16::trace_function16;
    (void)&w16::probe;
    (void)&w16::record;
    (void)&w16::recorded;
    (void)&w16::relocated_data;
    (void)&w16::default_stub;
    (void)&w16::selector_base;
    (void)&w16::segment_base;
    (void)&w16::_desc;
    w16::Trace16 trace;
    w16::ProbeOptions probe;
    w16::Trace16Options options;
    w16::Context context;
    w16::Hit hit;
    probe.near_ = options.near_;
    (void)trace;
    (void)context;
    (void)hit;
    (void)&dr::load;
    (void)&dr::run;
    (void)&dr::record;
    (void)&dr::recorded;
    (void)&dr::answers_in;
    dr::Record rec;
    (void)rec;
    (void)&codebreaker::ui::mcp_server::main;
    (void)&codebreaker::ui::mcp_server::token;
}

}  // namespace shim_check_w5b1
