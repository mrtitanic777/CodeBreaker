// source/pipeline_period.hpp -- the text passes `repair <32-bit module>` gives a period compiler (internal to source/pipeline_*.cpp; tested in
// pipeline_period_test.cpp).
//
//     const RepairSource given = repair_period_source(header, order, texts, imports, relocation_targets);
//     const std::string near = repair_needed_header(given.header, given.texts.at("f"));
#pragma once

#include <cstdint>
#include <map>
#include <set>
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

}  // namespace cb::source::pipeline
