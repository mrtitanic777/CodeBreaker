// cb/source/ne_asm/reachability.cpp -- which bytes of a segment are reachable code (see reachability.hpp).
// cb-unity: alone
#include "cb/source/ne_asm/reachability.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <map>
#include <unordered_set>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::source::ne_asm::reachability {

namespace stdfs = std::filesystem;
using segment::Seg;

namespace {

// The mnemonics with a code offset as their operand.
bool is_branch_op(std::string_view op) {
    static constexpr std::array<std::string_view, 29> ops = {
        "jmp", "call", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge", "jb", "jbe", "ja", "jae", "jc", "jnc", "jo", "jno", "js", "jns", "jp",
        "jnp", "jpe", "jpo", "jcxz", "jecxz", "loop", "loope", "loopne"};
    static constexpr std::array<std::string_view, 2> more = {"loopz", "loopnz"};
    return std::find(ops.begin(), ops.end(), op) != ops.end() || std::find(more.begin(), more.end(), op) != more.end();
}

// The instructions after which control does not continue to the next byte.
bool no_fall_through(std::string_view op) {
    static constexpr std::array<std::string_view, 8> ops = {"jmp", "ret", "retf", "retn", "iret", "iretf", "iretd", "iretw"};
    return std::find(ops.begin(), ops.end(), op) != ops.end();
}

std::string normalised(std::string_view instruction) {
    std::string out;
    for (const std::string_view word : text::split_ws(instruction)) {
        if (!out.empty()) out += ' ';
        out += word;
    }
    return out;
}

}  // namespace

std::optional<Branch> direct_branch(std::string_view instruction) {
    const std::string s = normalised(instruction);
    const std::size_t space = s.find(' ');
    if (space == std::string::npos || s.find(' ', space + 1) != std::string::npos) return std::nullopt;
    const std::string_view op = std::string_view(s).substr(0, space), operand = std::string_view(s).substr(space + 1);
    if (op.empty() || !std::all_of(op.begin(), op.end(), [](char c) { return c >= 'a' && c <= 'z'; })) return std::nullopt;
    if (!operand.starts_with("0x") || operand.size() == 2) return std::nullopt;
    for (const char c : operand.substr(2))
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return std::nullopt;
    const auto target = text::parse_int<std::uint32_t>(operand.substr(2), 16);
    if (!target || !is_branch_op(op)) return std::nullopt;
    return Branch{std::string(op), *target};
}

std::optional<std::uint32_t> own_segment_target(std::string_view label, std::size_t index) {
    if (!label.starts_with("seg")) return std::nullopt;
    const std::size_t colon = label.find(":0x");
    if (colon == std::string_view::npos || colon == 3) return std::nullopt;
    const std::string_view number = label.substr(3, colon - 3), hex = label.substr(colon + 3);
    if (hex.empty() || !std::all_of(number.begin(), number.end(), [](char c) { return c >= '0' && c <= '9'; })) return std::nullopt;
    if (!std::all_of(hex.begin(), hex.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); })) return std::nullopt;
    if (text::parse_int<std::size_t>(number, 10) != index) return std::nullopt;
    return text::parse_int<std::uint32_t>(hex, 16);
}

std::set<std::uint32_t> reachable(const Seg& seg) {
    std::map<std::uint32_t, std::string> rows;
    for (const segment::Row& row : seg.rows()) rows[row.offset] = normalised(row.text);
    std::map<std::uint32_t, std::uint32_t> next;   // lookup only
    for (auto at = rows.begin(); at != rows.end(); ++at) {
        const auto following = std::next(at);
        if (following != rows.end()) next[at->first] = following->first;
    }
    std::set<std::uint32_t> seeds;
    for (const auto& [start, function] : seg.functions()) seeds.insert(start);
    for (const auto& [offset, name] : seg.export_names()) seeds.insert(offset);
    for (const auto& [site, target] : seg.fixups())
        if (const auto own = own_segment_target(target.label, seg.index())) seeds.insert(*own);

    std::set<std::uint32_t> reached;
    std::vector<std::uint32_t> work;
    for (const std::uint32_t s : seeds)
        if (rows.contains(s)) work.push_back(s);
    while (!work.empty()) {
        const std::uint32_t at = work.back();
        work.pop_back();
        if (reached.contains(at) || !rows.contains(at)) continue;
        reached.insert(at);
        const std::string& instruction = rows[at];
        const std::string_view op = std::string_view(instruction).substr(0, instruction.find(' '));
        if (const auto branch = direct_branch(instruction); branch && rows.contains(branch->target)) work.push_back(branch->target);
        if (!no_fall_through(op) && instruction.find("(bad)") == std::string::npos)   // fall-through
            if (const auto following = next.find(at); following != next.end()) work.push_back(following->second);
    }
    return reached;
}

std::set<std::uint32_t> branch_targets(const Seg& seg) {
    std::set<std::uint32_t> targets;
    for (const auto& [start, function] : seg.functions()) targets.insert(start);
    for (const auto& [offset, name] : seg.export_names()) targets.insert(offset);
    for (const segment::Row& row : seg.rows())
        if (const auto branch = direct_branch(row.text)) targets.insert(branch->target);
    for (const auto& [site, target] : seg.fixups())
        if (const auto own = own_segment_target(target.label, seg.index())) targets.insert(*own);
    return targets;
}

std::vector<std::pair<std::uint32_t, std::uint32_t>> branches(const Seg& seg) {
    std::vector<std::pair<std::uint32_t, std::uint32_t>> found;
    for (const segment::Row& row : seg.rows())
        if (const auto branch = direct_branch(row.text)) found.emplace_back(row.offset, branch->target);
    return found;
}

std::vector<datacode::Region> safe_data_regions(const Seg& seg) {
    const auto all_branches = branches(seg);
    std::set<std::uint32_t> function_starts, exports, relocation_targets;
    for (const auto& [start, function] : seg.functions()) function_starts.insert(start);
    for (const auto& [offset, name] : seg.export_names()) exports.insert(offset);
    for (const auto& [site, target] : seg.fixups())
        if (const auto own = own_segment_target(target.label, seg.index())) relocation_targets.insert(*own);
    const auto inside = [](const std::set<std::uint32_t>& set, std::uint32_t s, std::uint32_t e) {
        const auto at = set.lower_bound(s);
        return at != set.end() && *at < e;
    };
    std::vector<datacode::Region> safe;
    for (const datacode::Region& region : datacode::data_regions(seg.code(), seg.rows())) {
        const std::uint32_t s = region.start, e = region.end;
        const bool entered = inside(function_starts, s, e) || inside(exports, s, e) || inside(relocation_targets, s, e) ||
                             std::any_of(all_branches.begin(), all_branches.end(), [&](const auto& b) {
                                 return b.second >= s && b.second < e && !(b.first >= s && b.first < e);
                             });
        if (!entered) safe.push_back(region);
    }
    return safe;
}

int main(const base::Corpus& corpus, std::span<const std::string> args, const fixups::Objdump& objdump) {
    const cli::Command command{
        "reachability",
        "which bytes of a segment are reachable code",
        {cli::Positional{"module", "a corpus name or a path"},
         cli::Option{"--seg", "", "the segment (from 1)", cli::Arg::Value, "N", "1"},
         cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const std::string name = a.required("module");
            const std::size_t index = a.integer<std::size_t>("--seg").value_or(1);
            const segment::SharedOrdinals ordinals = std::make_shared<const segment::Ordinals>(segment::load_ordinals(fs::from_text(a.required("--ords"))));
            const stdfs::path path = corpus.retail(name).value_or(fs::from_text(name));
            const formats::ne::Module module = fixups::open_module(path);
            const Seg seg(module, index, ordinals, {}, objdump);
            const std::set<std::uint32_t> reached = reachable(seg);
            const std::size_t total = seg.rows().size();
            const std::size_t unreached = total - reached.size();
            out("{} seg{}: {} rows, {} reachable code, {} unreachable ({:.1f}% -> data candidates)\n", name, index, total, reached.size(), unreached,
                100.0 * static_cast<double>(unreached) / static_cast<double>(std::max<std::size_t>(1, total)));
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("reachability", {command});
    std::vector<std::string> line = {"reachability"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::reachability
