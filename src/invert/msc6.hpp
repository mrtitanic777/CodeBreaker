// invert/msc6.hpp -- the Microsoft C 6.0a model: a 16-bit function of the medium model's far frame, back to the C it was compiled from.
//
//     msc6::Context context;
//     context.imports = {{0, "GlobalFree"}};                 // the k-th far call's callee
//     context.params = std::vector<msc6::Param>{{"p", msc6::Kind::Far}};
//     const Result<msc6::Reading> r = msc6::invert(code, context);
//     if (r.ok()) use(r.value().text); else say(r.error().what());   // a Declined: the model does not cover the function; the reason is the message
//
// One recursive pass runs register-value tracking, control-flow recovery (if / else, do-while, short-circuit and materialised conditions) and
// the statement templates (far, near, cdecl and COM-vtable calls, far pointers, longs, globals, stack locals) over the whole function, and
// declines at the first instruction it does not model -- it never guesses. The text is what `/AM /G2 /Gsw /Ox` compiles back to the same bytes
// (outside the places the linker fills in: msc6_small::mask).
//
// The module's other readers of the same compiler are cb/invert/msc6_small.hpp (the leaf templates, the walker, the register variables and the
// counted loop) and cb/invert/msc6_segment.hpp (parameters from how a segment's functions are called, thunks, one-shape functions).
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "invert/msc6_small.hpp"

namespace cb::invert::msc6 {

using msc6_small::Declined;

// What a parameter is.
enum class Kind { Int, Far, Long };

struct Param {
    std::string name;
    Kind kind = Kind::Int;
};

// What the caller knows about the function beyond its bytes.
struct Context {
    std::map<std::uint32_t, std::string> imports;       // the k-th far call (in code order) -> the routine it calls
    msc6_small::Globals globals;                        // the data segment's variables by the offset of their (low) word
    std::optional<std::vector<Param>> params;           // the signature; absent: read from the body and the far return's count
    std::string return_type = "int";                    // "int", "long" or "void"
    std::map<std::uint32_t, std::string> near_functions;   // same-segment callees by their offset in the segment
    std::uint32_t function_base = 0;                    // the function's offset in its segment
};

struct Notes {
    std::size_t calls = 0;                                          // far calls read
    std::vector<std::string> register_variables;
    std::vector<std::pair<std::string, std::string>> globals;      // name -> "int", "long" or "byte"
    bool plain_return = false;                                      // the function pops nothing (a plain far return)
    bool has_near_call = false;
    std::vector<std::string> near_calls;                            // the same-segment routines named
};

struct Reading {
    std::string text;
    Notes notes;
};

Result<Reading> invert(ByteView code, const Context& context = {});

}  // namespace cb::invert::msc6
