// codebreaker/invert/known_differences.cpp -- C++ port of codebreaker.invert.known_differences (see the .hpp).

#include "codebreaker/invert/known_differences.hpp"

namespace codebreaker::invert::known_differences {

py::list<Entry> KB = {
    Entry{py::str("seg-indexed-in-cond"),
          py::str(R"re(es:\[|ds:\[)re"),   // a raw <seg>:[..] leaked into C
          py::str("segment-indexed memory used in a condition/expression, not resolved to a field"),
          py::str("_val() routes <seg>:[base+N] through _farmem() -> *(WORD far*)((char far*)p+N)"),
          py::str("_farmem in _val"), true, py::str("OLECLI seg3:0x2a00")},
    Entry{py::str("far-param-as-int"),
          py::str(R"re(ship\[les .*\] vs ours\[mov )re"),   // shipped les (far), ours mov (int)
          py::str("a far-pointer param inferred as int (2-byte) -> wrong frame + param layout"),
          py::str("les [bp+off] marks off a far param (4 bytes); bound the count by retf N"),
          py::str("far-param inference (les + retf N)"), true, py::str("OLECLI seg11:0x31c")},
    Entry{py::str("trailing-pad"),
          py::str(R"re(len\+1$|trailing (90|00))re"),   // one extra alignment byte
          py::str("compiler alignment nop after a function; not part of the semantic body"),
          py::str("strip trailing 0x90/0x00 from both sides before comparing (verify + _mask)"),
          py::str("verify_inverted padding-strip"), true, py::str("LZEXPAND Cleanup")},
    Entry{py::str("cdecl-arg-order"),
          py::str(R"re(push\[bp \+ N\].*vs.*push\[bp \+ N\]|arg.?order)re"),
          py::str("cdecl call pushes args right-to-left; args must be REVERSED from push order"),
          py::str("detect 9a+add sp -> cdecl -> reverse s.args when building the call expr"),
          py::str("cdecl arg reversal"), true, py::str("OLECLI/seg10:0x23a")},
    Entry{py::str("byte-store-mask"),
          py::str(R"re(c606|byte store.*offset)re"),
          py::str("a byte-global store (c6 06) DGROUP offset not masked -> never matches relink"),
          py::str("add 0x68/0xc6 to _mask's direct-[imm16] opcodes"),
          py::str("_mask opcode set"), true, py::str("LZEXPAND FreeBuffers")},
    Entry{py::str("recipe-Os"),
          py::str(R"re(len[-+]\d+.*frame|sub sp.*vs|83ec02)re"),
          py::str("function compiled with a different optimization flag (e.g. /Os omits the dummy frame)"),
          py::str("verify against MULTIPLE recipes (/Ox and /Os); the flag prober pins the exact one"),
          py::str("multi-recipe verify / detect_recipe"), true, py::str("LZEXPAND return-0 (/Os)")},
    Entry{py::str("addr-of-global-imm"),
          py::str(R"re(push (imm|N).*global|68 N)re"),
          py::str("a push imm16 that is a DGROUP offset is &global (near ptr), not a literal"),
          py::str("push imm16 whose value is also touched as [imm16] -> &gW_x; mask the 0x68 operand"),
          py::str("address-of-global push"), true, py::str("OLECLI/seg10:0x23a")},
};

py::list<Entry> diagnose(const std::optional<py::str>& divergence_text) {
    py::list<Entry> hits;
    py::str t = divergence_text.has_value() && !divergence_text->empty() ? *divergence_text : py::str("");   // divergence_text or ""
    for (const Entry& p : KB) {
        bool found = false;
        try {
            found = py::re::search(p.match, t, py::re::IGNORECASE).has_value();
        } catch (const py::Exception&) {
            throw;
        } catch (const std::runtime_error& e) {
            // pyre reports a pattern that does not compile as a plain runtime_error (its own PyError, "ValueError"),
            // where Python raises re.error: the only thing a search of a pattern can fail on is its compiling.
            throw py::PatternError(e.what());
        }
        if (found) {
            hits.append(p);
        }
    }
    return hits;
}

Entry record(const Entry& pattern) {
    KB.append(pattern);
    return pattern;
}

void main() {
    py::print(py::percent(py::str("divergence KB: %d known patterns\n"), py::len(KB)));
    for (const Entry& p : KB) {
        py::print(py::percent(py::str("  [%s] %s"), p.auto_ ? py::str("auto") : py::str("manual"), p.id));
        py::print(py::percent(py::str("      cause: %s"), p.cause));
        py::print(py::percent(py::str("      fix:   %s  (%s)"), p.fix, p.msc6dec_construct));
    }
}

}  // namespace codebreaker::invert::known_differences
