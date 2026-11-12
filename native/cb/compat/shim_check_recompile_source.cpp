// Compile-only check of the shims at the old header paths that call old ports (which cannot be linked into cb_tests): the old API over
// cb::recompile and cb::source. Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "codebreaker/recompile/x360project.hpp"
#include "codebreaker/source/assembly.hpp"
#include "codebreaker/source/ne_asm/segdiff.hpp"
#include "codebreaker/source/placement.hpp"

namespace shim_check {

// Every entry point of the shims, named so the compiler checks its body.
inline void touch() {
    namespace dis = codebreaker::recompile::disassemble;
    namespace x360 = codebreaker::recompile::x360project;
    namespace disc = codebreaker::recompile::discover;
    namespace asmb = codebreaker::source::assembly;
    namespace seg = codebreaker::source::ne_asm::segdiff;
    namespace place = codebreaker::source::placement;
    (void)&dis::classify;
    (void)&dis::data_spans;
    (void)&dis::listing;
    (void)&dis::image;
    (void)&dis::code_section;
    (void)&dis::main;
    (void)&disc::_image;
    (void)&disc::_plausible_start;
    (void)&disc::from_pointers;
    (void)&disc::from_constants;
    (void)&disc::discover;
    (void)&disc::main;
    (void)&x360::scaffold;
    (void)&x360::build;
    (void)&x360::check;
    (void)&asmb::assemble;
    (void)&asmb::link;
    (void)&asmb::probe;
    (void)&asmb::operands;
    (void)&asmb::data_lines;
    (void)&asmb::_last_segment_data;
    (void)&asmb::culprits<int64_t>;
    (void)&seg::compile_group;
    (void)&seg::shipped_ne_segment;
    (void)&seg::diff;
    (void)&seg::render;
    (void)&seg::main;
    (void)&place::resolve;
    (void)&place::folder;
    (void)&place::main;
}

}  // namespace shim_check
