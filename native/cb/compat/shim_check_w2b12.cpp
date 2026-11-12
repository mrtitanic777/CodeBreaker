// Compile-only check of the shims at the old header paths for source/ne_asm/datacode, source/vxd/masm and source/vxd/idiom (the old
// ports' VxDDisasm, rows, py::sets and the DOS lane they hand to the new toolchain). Nothing here runs; it is in cb_shim_check so the
// shims stay honest until their last consumers (the old link port, the VxD tools) are refactored.
#include "codebreaker/source/ne_asm/datacode.hpp"
#include "codebreaker/source/vxd/idiom.hpp"
#include "codebreaker/source/vxd/masm.hpp"

namespace shim_check {

inline void touch_w2b12() {
    namespace datacode = codebreaker::source::ne_asm::datacode;
    namespace masm = codebreaker::source::vxd::masm;
    namespace idiom = codebreaker::source::vxd::idiom;
    (void)&datacode::find_strings;
    (void)&datacode::garbage_spans;
    (void)&datacode::data_regions;
    (void)&datacode::main;
    (void)&masm::_db;
    (void)&masm::_masm_line;
    (void)&masm::object_body;
    (void)&masm::assemble_coff;
    (void)&masm::assemble_coff_batch;
    (void)&masm::coff_text_syms;
    (void)&masm::compare_object;
    (void)&masm::compare_module;
    (void)&masm::_stage_toolchain;
    (void)&idiom::device_incs;
    (void)&idiom::svc_macro;
    (void)&idiom::ddb_macro;
    (void)&idiom::service_table;
    (void)&idiom::control_dispatch;
    (void)&idiom::_ddb_size;
    (void)&idiom::emit_idiomatic;
    (void)&idiom::emit_idiomatic_tree;
    (void)&idiom::obj_seg;
}

}  // namespace shim_check
