#include "cb/decompile/struct_text.hpp"

#include <algorithm>
#include <format>
#include <limits>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::struct_text {

namespace {

// How big (in bytes) each scalar type name is. Lookup only.
std::optional<std::uint32_t> scalar_size(std::string_view name) {
    static const std::unordered_map<std::string_view, std::uint32_t> kSizes = {
        {"int1", 1}, {"uint1", 1}, {"xunknown1", 1}, {"undefined1", 1}, {"undefined", 1}, {"bool", 1}, {"char", 1},
        {"int2", 2}, {"uint2", 2}, {"xunknown2", 2}, {"undefined2", 2},
        {"int4", 4}, {"uint4", 4}, {"xunknown4", 4}, {"undefined4", 4}, {"float4", 4}, {"int", 4},
        {"int8", 8}, {"uint8", 8}, {"xunknown8", 8}, {"undefined8", 8}, {"float8", 8}};
    auto it = kSizes.find(name);
    if (it == kSizes.end()) return std::nullopt;
    return it->second;
}

// A function whose heading names a register convention that passes `this` (or the first argument) in a register
// has that parameter first in its list, before the stack arguments the analysis counts from 0 -- as the heading
// is written, where the module's C leaves the convention's word out.
bool takes_register_argument(std::string_view heading) {
    return heading.find("__thiscall") != std::string_view::npos || heading.find("__fastcall") != std::string_view::npos;
}

// The sort key of a structure name: the number it ends in after the last '_' (0 when there is none or it is not
// all digits; saturating when it does not fit).
std::uint64_t structure_number(std::string_view name) {
    const std::size_t cut = name.rfind('_');
    if (cut == std::string_view::npos) return 0;
    std::string_view tail = name.substr(cut + 1);
    if (tail.empty() || !std::all_of(tail.begin(), tail.end(), [](char c) { return c >= '0' && c <= '9'; })) return 0;
    return text::parse_int<std::uint64_t>(tail).value_or(std::numeric_limits<std::uint64_t>::max());
}

}  // namespace

std::optional<FieldType> kind(std::string_view base, std::size_t stars) {
    if (stars > 1) return FieldType{std::string(base) + " " + std::string(stars - 1, '*'), 4};
    if (auto size = scalar_size(base)) return FieldType{std::string(base), *size};
    return std::nullopt;
}

Plan plan(std::string_view module_path, const std::set<std::string>& docs,
          const std::map<std::uint64_t, std::string>& by_va, const std::map<std::string, std::string>& declared,
          const Analysis& analysis) {
    std::optional<std::vector<Structure>> found = analysis.recover(module_path);
    if (!found) return {};

    // (function, parameter) -> the structure it points at; the first structure to claim a parameter keeps it
    std::map<std::pair<std::string, std::string>, std::string> owner;
    for (const Structure& s : *found) {
        for (const Root& root : s.roots) {
            if (root.kind == RootKind::Other) continue;
            auto named = by_va.find(root.va);
            if (named == by_va.end() || !docs.contains(named->second)) continue;
            const std::string& fn = named->second;
            auto head_it = declared.find(fn);
            const bool register_arg = head_it != declared.end() && takes_register_argument(head_it->second);
            std::vector<std::string> params = analysis.parameter_names(fn);
            if (root.kind == RootKind::Arg) {
                if (!root.index) continue;   // an argument root without its index names no parameter
                const std::int64_t at = *root.index + (register_arg ? 1 : 0);
                if (0 <= at && at < static_cast<std::int64_t>(params.size())) {
                    owner.try_emplace({fn, params[static_cast<std::size_t>(at)]}, s.name);
                }
            } else if (!params.empty() && register_arg) {
                // the object a member function is called on: its first parameter, in ECX
                owner.try_emplace({fn, params[0]}, s.name);
            }
        }
    }
    if (owner.empty()) return {};

    // what type each field is read as, over the whole module: structure -> offset -> type -> how many accesses
    std::map<std::string, std::map<std::int64_t, std::map<FieldType, std::size_t>>> votes;
    for (const auto& [key, sname] : owner) {
        for (const Access& a : analysis.struct_accesses(key.first, key.second)) {
            if (auto field = kind(a.base, a.stars)) ++votes[sname][a.offset][*field];
        }
    }
    std::map<std::string, Fields> fields;
    for (const auto& [sname, by_off] : votes) {
        std::int64_t at = 0;
        Fields chosen;
        for (const auto& [off, counts] : by_off) {
            // the type with the most accesses; ties go to the greater (type, size)
            const FieldType* best = nullptr;
            std::size_t best_count = 0;
            for (const auto& [type, count] : counts) {
                if (!best || count > best_count || (count == best_count && type > *best)) {
                    best = &type;
                    best_count = count;
                }
            }
            if (off < at) continue;   // inside the field before it
            chosen.emplace(off, *best);
            at = off + best->size;
        }
        if (chosen.size() >= 2) fields.emplace(sname, std::move(chosen));
    }

    Plan out;
    for (const auto& [key, sname] : owner) {
        auto it = fields.find(sname);
        if (it != fields.end()) out[key.first][key.second] = PlannedParam{sname, it->second};
    }
    return out;
}

std::string declarations(const Plan& plan, const std::set<std::string>& used) {
    std::map<std::string, const Fields*> fields;
    for (const auto& [fn, by_param] : plan) {
        for (const auto& [param, planned] : by_param) fields[planned.structure] = &planned.fields;
    }
    std::vector<std::string> order(used.begin(), used.end());   // by name; the stable sort keeps that among equals
    std::stable_sort(order.begin(), order.end(), [](const std::string& a, const std::string& b) {
        return structure_number(a) < structure_number(b);
    });
    std::string out;
    for (std::size_t i = 0; i < order.size(); ++i) {
        const std::string& sname = order[i];
        auto it = fields.find(sname);
        if (it == fields.end()) throw UsageError("declarations: the plan has no structure " + sname);
        if (i) out += '\n';
        out += std::format("typedef struct {} {{\n", sname);
        std::int64_t at = 0;
        for (const auto& [off, field] : *it->second) {
            if (off > at) out += std::format("  uint1 _gap_{:x}[{}];\n", at, off - at);
            const std::size_t star = static_cast<std::size_t>(std::count(field.type.begin(), field.type.end(), '*'));
            out += std::format("  {} {}f_{:x};\n", text::trim(field.type, "* "), std::string(star, '*'), off);
            at = off + field.size;
        }
        out += std::format("}} {};", sname);
    }
    return out + "\n";
}

}  // namespace cb::decompile::struct_text
