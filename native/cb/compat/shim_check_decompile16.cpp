// Compile-only check of the shims at the old header paths of decompile/asm_blocks, frame16, shape16 and l2tree: they call old
// ports (translate/l2's type_json) that cannot be linked into cb_tests, so they are compiled here, in cb_shim_check, to stay
// honest until decompile/decompiler (their last consumer) is refactored. Nothing here runs.
#include "codebreaker/decompile/asm_blocks.hpp"
#include "codebreaker/decompile/frame16.hpp"
#include "codebreaker/decompile/l2tree.hpp"
#include "codebreaker/decompile/shape16.hpp"
#include "codebreaker/decompile/sm83.hpp"

namespace shim_check_decompile {

inline void touch() {
    namespace ab = codebreaker::decompile::asm_blocks;
    namespace f16 = codebreaker::decompile::frame16;
    namespace s16 = codebreaker::decompile::shape16;
    namespace l2 = codebreaker::decompile::l2tree;
    (void)&f16::lay_out;
    (void)&f16::_walk;
    (void)&s16::reshape;
    (void)&l2::to_json;
    (void)&l2::from_json;
    (void)static_cast<std::tuple<ab::Json, py::list<py::str>> (*)(const ab::Decoder&, const ab::Json&, const py::bytes&, int64_t, int64_t,
                                                                  const ab::Fixups&, const ab::NameAt&, std::optional<int64_t>,
                                                                  const std::tuple<int64_t, int64_t>&)>(&ab::place);
    (void)static_cast<std::tuple<ab::Json, py::list<py::str>> (*)(const ab::Decoder&, const ab::Json&, const py::bytearray&, int64_t,
                                                                  int64_t, const ab::Fixups&, const ab::NameAt&, std::optional<int64_t>,
                                                                  const std::tuple<int64_t, int64_t>&)>(&ab::place);
    (void)&ab::_unused;
    (void)&ab::_frame_names;
    (void)&ab::_named;
    (void)&ab::truthy;
    (void)&ab::is_str;
    (void)&ab::deepcopy;
}

}  // namespace shim_check_decompile
