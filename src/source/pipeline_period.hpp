// source/pipeline_period.hpp -- the text passes `repair <32-bit module>` gives a period compiler (internal to source/pipeline_*.cpp; tested in
// pipeline_period_test.cpp).
//
//     const RepairSource given = repair_period_source(header, order, texts, imports, relocation_targets);
//     const std::string near = repair_needed_header(given.header, given.texts.at("f"));
#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cb::source::pipeline {

struct RepairSource {
    std::string header;
    std::map<std::string, std::string> texts;
};

// The reconstruction's header and function texts as a period compiler is given them: an import is `__declspec(dllimport)`, a global the module's
// relocations name is a variable, an address taken as a number is the address of a variable (`codebreaker_at_XXXXXXXX`), and a function whose every
// `return` hands back "whatever was in EAX" returns nothing. `order` is the functions in the module's order; `texts` has each one's text.
RepairSource repair_period_source(const std::string& header, const std::vector<std::string>& order, const std::unordered_map<std::string, std::string>& texts,
                                  const std::set<std::string>& imports, const std::set<std::uint64_t>& targets);

// The header with only the declarations `text` uses (types and macros are all kept).
std::string repair_needed_header(const std::string& header, std::string_view text);

// The functions' spans {start: end}, each ended where the next function of either list begins (`starts`: the module's functions): a span that runs
// on past a start it did not know of holds the next function's bytes too.
std::map<std::uint64_t, std::uint64_t> match_clipped_spans(std::map<std::uint64_t, std::uint64_t> spans, const std::vector<std::uint64_t>& starts);

// The functions that take their first argument in ECX (a C++ method's object) and those that take two in ECX and EDX, declared for Microsoft's
// compiler. The reconstruction says it with OpenWatcom's `#pragma aux NAME ... parm routine [ecx] [edx] ...`, which Microsoft's compiler ignores:
// it passed the object on the stack, and every such function removed four bytes more than the original's `ret N`. Here both are `__fastcall`
// (ECX, then EDX, the rest on the stack, removed by the function). A method with arguments after its object takes an unused second one in EDX,
// so the rest stay where the original has them; it is declared with its list open (`int f();`), so its callers' calls still compile. The
// pragmas are left out. `texts` is changed in place.
std::string period_registers(const std::string& header, std::map<std::string, std::string>& texts);

// Does the function build a frame (`push ebp; mov ebp, esp` first)? A compiler of the time builds one only when told to keep the frame pointer
// (/Oy-) or when the function needs it; `match` compiles such a function with /Oy-.
bool builds_frame(std::span<const std::uint8_t> code);

}  // namespace cb::source::pipeline
