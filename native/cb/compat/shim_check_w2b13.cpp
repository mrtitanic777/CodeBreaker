// Compile-only check of the shims at the old header paths for source/vxd/le_writer, source/vxd/link, symbols/debug_build and
// translate/judge_native_jvm (the old types they take and give: VxDDisasm, LEFile, py::dict, the old Seg, the old Verdict and the front end
// the host gives). Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumers are refactored.
#include "codebreaker/source/vxd/le_writer.hpp"
#include "codebreaker/source/vxd/link.hpp"
#include "codebreaker/symbols/debug_build.hpp"
#include "codebreaker/translate/judge_native_jvm.hpp"

namespace shim_check {

inline void touch_w2b13() {
    namespace le_writer = codebreaker::source::vxd::le_writer;
    namespace link = codebreaker::source::vxd::link;
    namespace debug_build = codebreaker::symbols::debug_build;
    namespace judge = codebreaker::translate::judge_native_jvm;
    (void)&le_writer::build_le;
    (void)&le_writer::build_w3;
    (void)&le_writer::_emit_fixups;
    (void)&le_writer::_oracle_pages;
    (void)&link::emit_source_and_def;
    (void)&link::byte_exact_force_raw;
    (void)&link::build_and_compare;
    (void)&link::build_functional;
    (void)&link::build_loadable_idiomatic;
    (void)&link::compare_objects;
    (void)&link::_source_code_into_container;
    (void)&link::_assemble_idiomatic;
    (void)&link::link_vxd_260;
    (void)&debug_build::norm_sig;
    (void)&debug_build::build_debug_index;
    (void)&debug_build::match_module;
    (void)&debug_build::load_override;
    (void)&debug_build::recover;
    (void)&judge::judge;
    (void)&judge::roundtrip;
    (void)&judge::translate;
    (void)&judge::native;
    (void)&judge::runtime_classes;
    (void)&judge::set_clang_frontend;
}

}  // namespace shim_check
