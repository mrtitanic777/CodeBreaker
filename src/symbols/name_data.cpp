// symbols/name_data.cpp -- see name_data.hpp (the bodies are names_bodies.cpp, the debug symbols names_symbols.cpp).
#include "symbols/name_data.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/text.hpp"

namespace cb::symbols::names {

namespace {

// A name as `\W+` -> "_" makes it: each run of characters that are not letters, digits or underscore is one underscore.
std::string runs_to_underscore(std::string_view name) {
    std::string out;
    bool in_run = false;
    for (const char c : name) {
        const bool word = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
        if (word) {
            out += c;
            in_run = false;
        } else if (!in_run) {
            out += '_';
            in_run = true;
        }
    }
    return out;
}

}  // namespace

std::optional<Address> parse_address(std::string_view text_in) {
    const std::size_t colon = text_in.find(':');
    if (colon == std::string_view::npos) return text::parse_int<Address>(text_in, 16);
    const auto segment = text::parse_int<std::uint32_t>(text_in.substr(0, colon), 10);
    const auto offset = text::parse_int<std::uint32_t>(text_in.substr(colon + 1), 16);
    if (!segment || !offset) return std::nullopt;
    return static_cast<Address>(core16::address(*segment, *offset));
}

std::string function_key(base::Lane lane, Address address) {
    if (lane == base::Lane::Ne) {
        const core16::SegOff place = core16::where(static_cast<core16::Va>(address));
        return std::format("{}:{:04x}", place.seg, place.off);
    }
    return std::format("{:x}", address);
}

Named powerpc_names(const PowerPcNames& facts) {
    Named out;
    if (facts.entry && *facts.entry) out[*facts.entry] = {"entry", "entry point"};
    std::set<std::string> taken = {"entry"};
    std::vector<PowerPcNames::VirtualMethod> methods = facts.methods;
    std::stable_sort(methods.begin(), methods.end(), [](const auto& a, const auto& b) { return a.address < b.address; });
    for (const auto& m : methods) {
        const std::string name = std::format("{}_vf{}", runs_to_underscore(m.class_name), m.slot);
        if (out.contains(m.address) || taken.contains(name)) continue;
        taken.insert(name);
        out[m.address] = {name, std::format("run-time type information ({}, virtual function {})", m.class_name, m.slot)};
    }
    return out;
}

}  // namespace cb::symbols::names
