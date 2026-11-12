// Compile-only check of the shims at the old header paths of source/ne_asm/{fixups,segment} and source/vxd/{coalesce,disassembly}: what
// needs the old corpus (the DDK's folders, the names the disassembly takes by default) cannot be linked into cb_tests, so it is compiled
// here and run, with the names handed to it, by compat/source_w1b23_shims_test.cpp. Nothing here runs.
#include "codebreaker/source/ne_asm/fixups.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"
#include "codebreaker/source/vxd/coalesce.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"

namespace shim_check {

// Every entry point of the shims, named so the compiler checks its body.
inline void touch_w1b23(const codebreaker::formats::ne::NEFile& ne, const codebreaker::formats::le::LEFile& v, const py::dict<py::str, py::str>& ordinals) {
    namespace fix = codebreaker::source::ne_asm::fixups;
    namespace seg = codebreaker::source::ne_asm::segment;
    namespace co = codebreaker::source::vxd::coalesce;
    namespace dis = codebreaker::source::vxd::disassembly;
    (void)fix::open_ne(py::str("x"));
    (void)fix::build_fixup_map(ne, ne.segments()[0], ne.module_refs());
    fix::disasm_segment(py::str("x"), 1, 0, std::nullopt);
    fix::disasm_entry(py::str("x"), 1);
    (void)fix::main(py::list<py::str>{py::str("x")});

    const seg::Seg s(ne, 1, ordinals);
    (void)s.name_at(0);
    (void)s.resolve_call(0, py::str("call 0x0"));
    (void)s.func_rows(0);
    (void)s.calls_in(0);
    (void)s.args_used(0);
    (void)s.word_at(0);
    (void)s.dispatch(0);
    (void)s.handler_calls(0, 0, py::list<int64_t>());
    (void)s.is_thunk(0);
    (void)seg::annotate_row(s, 0, py::str("nop"));
    seg::print_function(s, 0);
    (void)seg::trace_callgraph(s, 0);
    (void)seg::find_seg_of_entry(ne, 1);
    (void)seg::disasm_whole(py::bytes(std::string()), 0);
    seg::cmd_map(ne, ordinals);
    seg::cmd_list(ne, ordinals, 1);
    seg::cmd_fn(ne, ordinals, 1, 0);
    seg::cmd_cskel(ne, ordinals, 1, 0);
    seg::cmd_wndprocs(ne, ordinals);
    seg::cmd_trace(ne, ordinals, 1, 0);
    (void)seg::main(py::list<py::str>{py::str("x")});

    (void)co::coalesce(py::bytes(std::string()));
    (void)co::restore_entry_point(py::bytes(std::string()), v);
    (void)co::restore_metadata(py::bytes(std::string()), v);
    (void)co::restore_version_resource(py::bytes(std::string()), v);
    (void)co::finalize(py::bytes(std::string()), v);

    dis::VxDDisasm d(v);
    d.run();   // with the names of the old corpus: the DDK's folders
    (void)d.fsite(1, 0);
    const auto row = d.insns.items();
    (void)row;
    (void)dis::_render_ops(d, 1, dis::Row());
    (void)dis::_direct_target_row(dis::Row());
    (void)dis::_svc_name(1, 1, py::str("inline"));
    (void)dis::emit_asm(v);
    (void)dis::verify(v);
    (void)dis::disassemble_file(py::str("x"), std::nullopt);
}

}  // namespace shim_check
