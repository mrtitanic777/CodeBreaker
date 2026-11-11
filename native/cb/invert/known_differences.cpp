#include "cb/invert/known_differences.hpp"

#include <format>

namespace cb::invert {

namespace {

std::vector<DifferencePattern> seed_patterns() {
    return {
        {.id = "seg-indexed-in-cond",
         .match = R"(es:\[|ds:\[)",   // a raw <seg>:[..] leaked into C
         .cause = "segment-indexed memory used in a condition/expression, not resolved to a field",
         .fix = "_val() routes <seg>:[base+N] through _farmem() -> *(WORD far*)((char far*)p+N)",
         .construct = "_farmem in _val",
         .automatic = true,
         .example = "OLECLI seg3:0x2a00"},
        {.id = "far-param-as-int",
         .match = R"(ship\[les .*\] vs ours\[mov )",   // shipped les (far), ours mov (int)
         .cause = "a far-pointer param inferred as int (2-byte) -> wrong frame + param layout",
         .fix = "les [bp+off] marks off a far param (4 bytes); bound the count by retf N",
         .construct = "far-param inference (les + retf N)",
         .automatic = true,
         .example = "OLECLI seg11:0x31c"},
        {.id = "trailing-pad",
         .match = R"(len\+1$|trailing (90|00))",   // one extra alignment byte
         .cause = "compiler alignment nop after a function; not part of the semantic body",
         .fix = "strip trailing 0x90/0x00 from both sides before comparing (verify + _mask)",
         .construct = "verify_inverted padding-strip",
         .automatic = true,
         .example = "LZEXPAND Cleanup"},
        {.id = "cdecl-arg-order",
         .match = R"(push\[bp \+ N\].*vs.*push\[bp \+ N\]|arg.?order)",
         .cause = "cdecl call pushes args right-to-left; args must be REVERSED from push order",
         .fix = "detect 9a+add sp -> cdecl -> reverse s.args when building the call expr",
         .construct = "cdecl arg reversal",
         .automatic = true,
         .example = "OLECLI/seg10:0x23a"},
        {.id = "byte-store-mask",
         .match = R"(c606|byte store.*offset)",
         .cause = "a byte-global store (c6 06) DGROUP offset not masked -> never matches relink",
         .fix = "add 0x68/0xc6 to _mask's direct-[imm16] opcodes",
         .construct = "_mask opcode set",
         .automatic = true,
         .example = "LZEXPAND FreeBuffers"},
        {.id = "recipe-Os",
         .match = R"(len[-+]\d+.*frame|sub sp.*vs|83ec02)",
         .cause = "function compiled with a different optimization flag (e.g. /Os omits the dummy frame)",
         .fix = "verify against MULTIPLE recipes (/Ox and /Os); the flag prober pins the exact one",
         .construct = "multi-recipe verify / detect_recipe",
         .automatic = true,
         .example = "LZEXPAND return-0 (/Os)"},
        {.id = "addr-of-global-imm",
         .match = R"(push (imm|N).*global|68 N)",
         .cause = "a push imm16 that is a DGROUP offset is &global (near ptr), not a literal",
         .fix = "push imm16 whose value is also touched as [imm16] -> &gW_x; mask the 0x68 operand",
         .construct = "address-of-global push",
         .automatic = true,
         .example = "OLECLI/seg10:0x23a"},
    };
}

}  // namespace

KnownDifferences::KnownDifferences() : KnownDifferences(seed_patterns()) {}

KnownDifferences::KnownDifferences(std::vector<DifferencePattern> patterns) : patterns_(std::move(patterns)) {
    for (const DifferencePattern& p : patterns_) compiled_.emplace_back(p.match, RegexFlags::IgnoreCase);
}

std::vector<const DifferencePattern*> KnownDifferences::diagnose(std::string_view divergence) const {
    std::vector<const DifferencePattern*> hits;
    for (std::size_t i = 0; i < patterns_.size(); ++i) {
        if (compiled_[i].test(divergence)) hits.push_back(&patterns_[i]);
    }
    return hits;
}

const DifferencePattern& KnownDifferences::record(DifferencePattern pattern) {
    compiled_.emplace_back(pattern.match, RegexFlags::IgnoreCase);   // throws ParseError before anything is added
    patterns_.push_back(std::move(pattern));
    return patterns_.back();
}

std::string KnownDifferences::listing() const {
    std::string out = std::format("divergence KB: {} known patterns\n\n", patterns_.size());
    for (const DifferencePattern& p : patterns_) {
        out += std::format("  [{}] {}\n", p.automatic ? "auto" : "manual", p.id);
        out += std::format("      cause: {}\n", p.cause);
        out += std::format("      fix:   {}  ({})\n", p.fix, p.construct);
    }
    return out;
}

}  // namespace cb::invert
