// codebreaker/port/asm16.cpp -- what is left of the old translation unit (docs/CPP_DESIGN.md 6.3): listing_offsets, which runs the
// program's own assembler under the old emulator wrapper. Everything else of port/asm16 is native/cb/port/asm16*.cpp, behind the
// shim in asm16.hpp.
//
// COMPAT(exact/emu/dos): this is the one place that names the old emulator; the new code (cb::port::asm16::listing_offsets) is
// given it as a callback. Deleted with exact/emu/dos's refactoring.
#include "codebreaker/port/asm16.hpp"

#include "codebreaker/exact/emu/dos.hpp"

namespace codebreaker::port::asm16 {

Listing listing_offsets(const py::str& path, const py::str& assembler, const py::str& args) {
    namespace dos = codebreaker::exact::emu::dos;
    const newp::AssemblerRunner runner = [](const std::filesystem::path& folder, std::string_view a) {
        dos::run(compat::to_py(::cb::fs::to_text(folder / "ASM.EXE")), compat::to_py(::cb::fs::to_text(folder)), compat::to_py(a),
                 py::bytes(std::string()), dos::DEFAULT_MAX_BLOCKS, false, py::bytes(std::string("PATH=C:\\\0", 9)), std::nullopt, std::nullopt, true);
    };
    return to_old(guarded([&] {
        return newp::listing_offsets(::cb::fs::from_text(compat::from_py(path)), ::cb::fs::from_text(compat::from_py(assembler)), runner,
                                     compat::from_py(args));
    }));
}

}  // namespace codebreaker::port::asm16
