// codebreaker/invert/grammars.cpp -- C++ from codebreaker.invert.grammars.
//
// grammars.py -- enumerative expression synthesis: candidate texts for a function's shape.
//
// The step that removes the human from the loop. Instead of a person reading a gap,
// analyzing the idiom, and hand-writing a rule, a GRAMMAR enumerates candidate C for the
// function's shape and the compiler-oracle (refine.check / mocklink.verify) proves each
// byte-exact. MSC 6.0's codegen is deterministic and its idiom vocabulary is finite, so a
// bounded search terminates on the exact source -- no analysis required. Whatever verifies
// is, by construction, a correct rule; it can be banked into templates.py automatically.
//
//     for grammar in grammars.GRAMMARS: for text in grammar(None): ...   (run.py compiles them)
//
// This is the "superoptimization" / "genetic search" layer of the roadmap. The offline
// precomputed [asm -> C] table is the same idea cached: run the grammars once over all
// holes, index by masked bytes, then decompilation is an O(1) lookup.

#include "codebreaker/invert/grammars.hpp"

namespace codebreaker::invert::grammars {

// ---- grammars: each yields candidate C strings for a function SHAPE -------------
// A grammar encodes the finite parameter space of one MSC-6 idiom class. The search is
// the cross-product of its holes; the oracle rejects all but the byte-exact instantiation.

// Pure forwarder: return SIB(args). Holes: convention, arg count/order.
py::list<py::str> g_thunk(const Ctx& ctx) {
    py::list<py::str> out;
    // (ctx or {}).get("nargs", 3)
    int64_t n = ctx ? ctx->get(py::str("nargs"), 3) : 3;
    for (const char* conv : {"pascal ", ""}) {
        py::str params;                                // "int a0, int a1, ..."
        for (int64_t i = 0; i < n; ++i) {
            params += py::str(i ? ", " : "") + py::str("int a") + py::str(std::to_string(i));
        }
        // range(n) and range(n - 1, -1, -1)
        for (int pass = 0; pass < 2; ++pass) {
            py::str fwd;
            for (int64_t k = 0; k < n; ++k) {
                int64_t i = pass == 0 ? k : n - 1 - k;
                fwd += py::str(k ? ", " : "") + py::str("a") + py::str(std::to_string(i));
            }
            out.append(py::str("extern int far SIB();\nint far ") + py::str(conv) + py::str("F(") + params +
                       py::str("){ return SIB(") + fwd + py::str("); }"));
        }
    }
    return out;
}

// Sibling fills a stack buffer, function reads it back and stores to *out.
// Holes: buffer size, read type/expr, return-flow, out-pointer width. This ONE grammar
// covers both seg11:0x7f4 (2-byte WORD) and seg11:0x830 (4-byte long) hand-fixes.
py::list<py::str> g_bufread(const Ctx& ctx) {
    (void)ctx;
    py::list<py::str> out;
    const char* word_reads[] = {"b[0]|(b[1]<<8)", "*(unsigned*)b"};
    // 2-byte WORD variant
    for (int bufsz : {2, 4}) {
        for (const char* rd : word_reads) {
            for (const char* flow : {"if", "early"}) {
                py::str body;
                py::str sz(std::to_string(bufsz));
                if (py::str(flow) == "if") {
                    body = py::str("unsigned char b[") + sz + py::str("]; if (SIB(p,(char far*)b,2)){ *out=") + py::str(rd) +
                           py::str("; return 0xffff; } return 0;");
                } else {
                    body = py::str("unsigned char b[") + sz + py::str("]; if (!SIB(p,(char far*)b,2)) return 0; *out=") + py::str(rd) +
                           py::str("; return 0xffff;");
                }
                out.append(py::str("extern int far SIB();\nint far F(int p, unsigned *out){ ") + body + py::str(" }"));
            }
        }
    }
    // 4-byte long variant: declare `long v`, then build it AFTER the buffer is filled
    py::str build("v=b[3]; v=(v<<8)+b[2]; v=(v<<8)+b[1]; v=(v<<8)+b[0];");
    for (int bufsz : {4, 6}) {
        for (const char* flow : {"if", "early"}) {
            py::str body;
            py::str sz(std::to_string(bufsz));
            if (py::str(flow) == "if") {
                body = py::str("unsigned char b[") + sz + py::str("]; long v; if (SIB(p,(char far*)b,4)){ ") + build +
                       py::str(" *out=v; return 0xffff; } return 0;");
            } else {
                body = py::str("unsigned char b[") + sz + py::str("]; long v; if (!SIB(p,(char far*)b,4)) return 0; ") + build +
                       py::str(" *out=v; return 0xffff;");
            }
            out.append(py::str("extern int far SIB();\nint far F(int p, long *out){ ") + body + py::str(" }"));
        }
    }
    return out;
}

const py::list<Grammar> GRAMMARS = {g_thunk, g_bufread};

}  // namespace codebreaker::invert::grammars
