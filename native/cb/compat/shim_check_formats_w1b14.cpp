// Compile-only check of the shims at codebreaker/formats/pe_functions.hpp and codebreaker/formats/media.hpp: the parts that reach
// the old corpus, settings and xbox volumes (and so cannot be linked into cb_tests). Nothing here runs; it is in cb_shim_check,
// so the shims stay honest until their last consumer is refactored.
#include "codebreaker/formats/media.hpp"
#include "codebreaker/formats/pe_functions.hpp"

namespace shim_check_formats_w1b14 {

inline void touch() {
    namespace pf = codebreaker::formats::pe_functions;
    pf::set_capstone_dll(py::str(""));
    pf::PeCarve carve(py::str("lz32.dll"), nullptr);
    (void)carve.insn->at(0);
    (void)carve.insn->get(0);
    (void)carve.insn->items();
    (void)carve.insn->values();
    (void)carve.resolve_call(carve.insn->at(0).ins);
    (void)carve.calls_in(0);
    (void)carve.disasm_func(0);
    (void)carve.data_refs();
    (void)carve.xrefs_to(0);
    (void)carve.string_refs(py::dict<int64_t, py::str>());
    (void)carve._in_image(0);
    (void)carve._image_ranges();
    (void)carve._sec_of(0);
    (void)pf::carve(py::str("lz32.dll"));
    pf::ensure_target();
    (void)pf::disasm_whole32(py::bytes(), 0, nullptr);
    pf::PeSeg seg(py::str("lz32.dll"), std::nullopt, nullptr, nullptr);
    (void)seg.funcs;
    (void)pf::function_spans(py::str("lz32.dll"));

    namespace media = codebreaker::formats::media;
    (void)media::volumes(py::str("disk.img"));
    (void)media::put(py::str("disk.img"), py::dict<py::str, py::bytes>(), 0);
    (void)media::extract(py::str("disk.img"), py::str("out"));
    (void)media::_safe(py::str("a/b"));
    (void)media::fnmatch(py::str("A.TXT"), py::str("*.txt"));
    (void)media::main(py::list<py::str>());
    media::FatVolume fat(media::_Source(py::str("floppy.img"), 0));
    (void)fat.files();
    (void)fat._next(2);
    (void)fat._chain(2);
    media::IsoVolume iso(media::_Source(py::str("disc.iso"), 0));
    (void)iso.files();
    (void)iso.boot_image();
    media::BundleVolume bundle(py::str("WIN100.BIN"));
    (void)bundle.files();
}

}  // namespace shim_check_formats_w1b14
