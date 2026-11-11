#include "cb/formats/code_scan.hpp"

namespace cb::formats {

std::vector<std::uint32_t> find_functions(ByteView mem, std::set<std::uint32_t> starts, const Decoder& decode,
                                          const CodeSpace& space) {
    auto in_code = [&](std::uint32_t a) { return a >= space.begin && a < space.end; };
    auto usable_target = [&](std::uint32_t t) { return in_code(t) && (!space.even_targets_only || t % 2 == 0); };

    std::vector<std::uint32_t> todo(starts.begin(), starts.end());
    std::set<std::uint32_t> seen;
    while (!todo.empty()) {
        std::uint32_t pc = todo.back();
        todo.pop_back();
        while (in_code(pc) && !seen.contains(pc)) {
            if (pc >= space.data_begin && pc < space.data_end) break;
            seen.insert(pc);
            std::optional<DecodedInsn> insn = decode(mem, pc);
            if (!insn || insn->size == 0) break;
            const bool follows = insn->flow == Flow::Branch || insn->flow == Flow::Jump || insn->flow == Flow::Call;
            if (insn->target && usable_target(*insn->target)) {
                if (insn->flow == Flow::Call) starts.insert(*insn->target);
                if (follows) todo.push_back(*insn->target);
            }
            if (insn->flow == Flow::Jump || insn->flow == Flow::Return || insn->flow == Flow::Indirect ||
                insn->flow == Flow::Halt) {
                break;
            }
            pc += insn->size;
        }
    }
    return {starts.begin(), starts.end()};
}

}  // namespace cb::formats
