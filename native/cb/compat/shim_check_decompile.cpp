// Compile check of the shims at the old header paths for decompile/m68k, decompile/xenos_hlsl and decompile/vmx128: the
// surface the old callers use is instantiated here (the shims have no .cpp of their own). See CMakeLists.txt, cb_shim_check.
#include "codebreaker/decompile/m68k.hpp"
#include "codebreaker/decompile/vmx128.hpp"
#include "codebreaker/decompile/xenos_hlsl.hpp"

namespace codebreaker::shim_check {

py::str decompile_shims(const py::bytes& code, const decompile::xenos_hlsl::shader::Shader& shader,
                        const formats::powerpc_image::Image& image) {
    namespace m = decompile::m68k;
    py::str out;
    if (std::optional<m::Insn> insn = m::decode(code, 0, 0x1000)) {
        out += m::text(*insn, py::dict<int64_t, py::str>()) + insn->shape() + insn->repr();
        for (const m::Op& o : insn->operands) out += m::operand_text(o, py::dict<int64_t, py::str>()) + o.key();
    }
    out += py::str(m::assemble(py::str("RTS"), 0x1000).size() > 0 ? "x" : "y");
    out += py::str(std::to_string(m::_value(py::str("1+2"), py::dict<py::str, int64_t>(), 0)));
    out += decompile::xenos_hlsl::translate(shader, py::str("S")).text + decompile::xenos_hlsl::HELPERS + decompile::xenos_hlsl::_num(1.5);
    out += py::str(std::to_string(decompile::vmx128::coverage(image).size()));
    if (auto d = decompile::vmx128::decode(0x102320C3)) out += std::get<0>(*d);
    out += decompile::vmx128::sleigh();
    out += std::get<0>(decompile::vmx128::ROWS[0]) + std::get<0>(decompile::vmx128::X31[0]);
    return out;
}

}  // namespace codebreaker::shim_check
