// cb-unity: alone (file-local helpers named for this module)
#include "symbols/debug_build_estimate.hpp"

#include <algorithm>
#include <charconv>
#include <format>
#include <set>
#include <unordered_map>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::symbols::debug_build_estimate {

namespace segment = source::ne_asm::segment;
namespace ne = formats::ne;
using debug_build::FunctionKey;

namespace {

// ---- difflib.SequenceMatcher over sequences of dense element numbers ----------------------------------------------------------
// The longest-match search, the block merging and autojunk (the elements that make up more than one percent of a sequence of 200 or more
// are left out of the search and only bring a match back by its extension) are difflib's, so the same inputs pair the same elements.
class Matcher {
public:
    struct Block {
        std::size_t i, j, k;
        friend auto operator<=>(const Block&, const Block&) = default;
    };

    Matcher(const std::vector<std::uint32_t>& a, const std::vector<std::uint32_t>& b, std::uint32_t distinct, bool autojunk) : a_(a), b_(b) {
        by_element_.assign(distinct, {});
        for (std::size_t j = 0; j < b_.size(); ++j) by_element_[b_[j]].push_back(j);
        if (autojunk && b_.size() >= 200) {
            const std::size_t popular = b_.size() / 100 + 1;
            for (auto& places : by_element_)
                if (places.size() > popular) places.clear();
        }
    }

    Block longest(std::size_t alo, std::size_t ahi, std::size_t blo, std::size_t bhi) {
        std::size_t besti = alo, bestj = blo, bestsize = 0;
        std::vector<std::size_t> run(b_.size() + 1, 0), next(b_.size() + 1, 0);   // run[j + 1]: the match ending at b[j]
        std::vector<std::size_t> touched, touched_next;
        for (std::size_t i = alo; i < ahi; ++i) {
            for (const std::size_t j : by_element_[a_[i]]) {
                if (j < blo) continue;
                if (j >= bhi) break;
                const std::size_t k = run[j] + 1;
                next[j + 1] = k;
                touched_next.push_back(j + 1);
                if (k > bestsize) {
                    besti = i - k + 1;
                    bestj = j - k + 1;
                    bestsize = k;
                }
            }
            for (const std::size_t t : touched) run[t] = 0;
            std::swap(run, next);
            std::swap(touched, touched_next);
            touched_next.clear();
        }
        // grow the match over equal elements on either side: this is what brings an autojunked element back in
        while (besti > alo && bestj > blo && a_[besti - 1] == b_[bestj - 1]) {
            --besti;
            --bestj;
            ++bestsize;
        }
        while (besti + bestsize < ahi && bestj + bestsize < bhi && a_[besti + bestsize] == b_[bestj + bestsize]) ++bestsize;
        return {besti, bestj, bestsize};
    }

    std::vector<Block> blocks() {
        struct Range {
            std::size_t alo, ahi, blo, bhi;
        };
        std::vector<Range> queue = {{0, a_.size(), 0, b_.size()}};
        std::vector<Block> found;
        while (!queue.empty()) {
            const Range q = queue.back();
            queue.pop_back();
            const Block x = longest(q.alo, q.ahi, q.blo, q.bhi);
            if (x.k == 0) continue;
            found.push_back(x);
            if (q.alo < x.i && q.blo < x.j) queue.push_back({q.alo, x.i, q.blo, x.j});
            if (x.i + x.k < q.ahi && x.j + x.k < q.bhi) queue.push_back({x.i + x.k, q.ahi, x.j + x.k, q.bhi});
        }
        std::sort(found.begin(), found.end());
        std::vector<Block> merged;
        Block run{0, 0, 0};
        for (const Block& b : found) {
            if (run.i + run.k == b.i && run.j + run.k == b.j) {
                run.k += b.k;
            } else {
                if (run.k) merged.push_back(run);
                run = b;
            }
        }
        if (run.k) merged.push_back(run);
        return merged;   // (difflib adds a closing block of size 0 at the end of both; nothing here reads it)
    }

    double ratio() {
        std::size_t matches = 0;
        for (const Block& b : blocks()) matches += b.k;
        const std::size_t length = a_.size() + b_.size();
        return length ? 2.0 * static_cast<double>(matches) / static_cast<double>(length) : 1.0;
    }

private:
    const std::vector<std::uint32_t>& a_;
    const std::vector<std::uint32_t>& b_;
    std::vector<std::vector<std::size_t>> by_element_;   // element -> where it is in b, ascending
};

// Numbers for texts, equal texts getting the same number (lookup only).
class Interner {
public:
    std::uint32_t of(const std::string& text) {
        const auto [it, added] = ids_.try_emplace(text, static_cast<std::uint32_t>(ids_.size()));
        (void)added;
        return it->second;
    }
    std::uint32_t distinct() const { return static_cast<std::uint32_t>(ids_.size()); }

private:
    std::unordered_map<std::string, std::uint32_t> ids_;
};

double mnemonic_ratio(Interner& ids, const std::vector<std::string>& a, const std::vector<std::string>& b) {
    std::vector<std::uint32_t> na, nb;
    for (const std::string& s : a) na.push_back(ids.of(s));
    for (const std::string& s : b) nb.push_back(ids.of(s));
    return Matcher(na, nb, ids.distinct(), /*autojunk=*/true).ratio();
}

// ---- what a function looks like ---------------------------------------------------------------------------------------------------
bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

// An instruction with its blanks collapsed and every `0x<hex>` number replaced by X: the skeleton, the same where only constants differ.
std::string skeleton_row(std::string_view instruction) {
    std::string collapsed;
    for (const std::string_view word : text::split_ws(instruction)) collapsed += (collapsed.empty() ? "" : " ") + std::string(word);
    std::string out;
    for (std::size_t i = 0; i < collapsed.size();) {
        std::size_t digits = 0;
        if (collapsed.compare(i, 2, "0x") == 0)
            while (i + 2 + digits < collapsed.size() && is_lower_hex(collapsed[i + 2 + digits])) ++digits;
        if (digits) {
            out += 'X';
            i += 2 + digits;
        } else {
            out += collapsed[i++];
        }
    }
    return out;
}

struct Look {
    std::vector<std::string> skeleton;     // one per instruction
    std::string skeleton_key;              // the skeleton as one text, for equality across functions
    debug_build::Signature strict;         // immediates kept
    std::vector<std::string> mnemonics;    // one per instruction
};

Look look_at(const segment::Seg& seg, std::uint32_t start) {
    Look look;
    for (const segment::Row& row : seg.function_rows(start).rows) {
        look.skeleton.push_back(skeleton_row(row.text));
        const auto words = text::split_ws(row.text);
        look.mnemonics.push_back(words.empty() ? "?" : std::string(words.front()));
    }
    for (const std::string& s : look.skeleton) look.skeleton_key += std::to_string(s.size()) + ":" + s + "|";   // length-prefixed: equal skeletons only
    look.strict = debug_build::signature(seg, start);
    return look;
}

// round(x, 2): the nearest two-decimal value, as the exact binary value decides.
double round_to_two_places(double x) {
    const std::string text = std::format("{:.2f}", x);
    double out = x;
    std::from_chars(text.data(), text.data() + text.size(), out);
    return out;
}

bool is_export_name(std::string_view name) { return !name.empty() && !text::lower(name).starts_with("sub_"); }

std::vector<std::uint32_t> code_segment_numbers(const ne::Module& m) {
    std::vector<std::uint32_t> out;
    std::uint32_t i = 0;
    for (const ne::Segment& s : m.segments()) {
        ++i;
        if (!s.is_data()) out.push_back(i);
    }
    return out;
}

}  // namespace

std::string_view tier_name(Tier tier) {
    switch (tier) {
        case Tier::Exact: return "EXACT";
        case Tier::High: return "HIGH";
        case Tier::Aligned: return "ALIGNED";
        case Tier::Probable: return "PROBABLE";
        case Tier::Guess: return "GUESS";
    }
    return "?";
}

std::optional<Tier> tier_from_name(std::string_view name) {
    for (const Tier tier : kTiers)
        if (tier_name(tier) == name) return tier;
    return std::nullopt;
}

bool is_confident(Tier tier) { return tier == Tier::Exact || tier == Tier::High || tier == Tier::Aligned; }

std::size_t Report::count(Tier tier) const {
    return static_cast<std::size_t>(std::count_if(tiers.begin(), tiers.end(), [&](const auto& kv) { return kv.second.tier == tier; }));
}

std::map<std::uint32_t, Estimate> align_segment(std::uint32_t seg_number, const segment::Seg& retail, const segment::Seg& debug_seg, const SegmentNames& debug_names,
                                                const debug_build::Names& exact, bool predict_exports) {
    std::map<std::string, std::uint32_t> debug_by_name;   // the first place a name is at
    for (const auto& [offset, name] : debug_names) debug_by_name.try_emplace(name, offset);

    std::vector<std::uint32_t> retail_offsets, debug_offsets;   // ascending (the maps are)
    for (const auto& [start, func] : retail.functions()) retail_offsets.push_back(start);
    for (const auto& [offset, name] : debug_names)
        if (debug_seg.is_instruction(offset)) debug_offsets.push_back(offset);
    std::map<std::uint32_t, Look> retail_look, debug_look;
    for (const std::uint32_t off : retail_offsets) retail_look.emplace(off, look_at(retail, off));
    for (const std::uint32_t off : debug_offsets) debug_look.emplace(off, look_at(debug_seg, off));

    const auto is_target = [&](std::uint32_t off) { return predict_exports ? is_export_name(retail.name_at(off)) : !is_export_name(retail.name_at(off)); };

    // anchors: the exact matches whose names the debug build has, kept in an order both builds agree on
    std::vector<std::pair<std::uint32_t, std::uint32_t>> anchors;   // (retail offset, debug offset)
    for (const std::uint32_t roff : retail_offsets) {
        const auto name = exact.find({seg_number, roff});
        if (name == exact.end() || name->second.empty()) continue;
        const auto at = debug_by_name.find(name->second);
        if (at == debug_by_name.end()) continue;
        while (!anchors.empty() && anchors.back().second >= at->second) anchors.pop_back();
        anchors.emplace_back(roff, at->second);
    }

    std::map<std::uint32_t, Estimate> out;
    constexpr std::int64_t kFar = std::int64_t{1} << 30;
    std::vector<std::pair<std::int64_t, std::int64_t>> bounds = {{-1, -1}};
    for (const auto& [r, d] : anchors) bounds.emplace_back(r, d);
    bounds.emplace_back(kFar, kFar);
    Interner ids;   // shared across the segment: the skeletons and the instruction names are different texts, so they never meet
    for (std::size_t b = 0; b + 1 < bounds.size(); ++b) {
        const auto [ra, da] = bounds[b];
        const auto [rb, db] = bounds[b + 1];
        std::vector<std::uint32_t> retail_gap, debug_gap;
        for (const std::uint32_t x : retail_offsets)
            if (ra < static_cast<std::int64_t>(x) && static_cast<std::int64_t>(x) < rb) retail_gap.push_back(x);
        for (const std::uint32_t x : debug_offsets)
            if (da < static_cast<std::int64_t>(x) && static_cast<std::int64_t>(x) < db) debug_gap.push_back(x);

        // the same skeletons in the same order are the same functions: strictly so (Exact) or up to constants (High)
        std::set<std::uint32_t> used;
        std::vector<std::uint32_t> sa, sb;
        for (const std::uint32_t x : retail_gap) sa.push_back(ids.of(retail_look.at(x).skeleton_key));
        for (const std::uint32_t x : debug_gap) sb.push_back(ids.of(debug_look.at(x).skeleton_key));
        for (const Matcher::Block& block : Matcher(sa, sb, ids.distinct(), /*autojunk=*/false).blocks()) {
            for (std::size_t k = 0; k < block.k; ++k) {
                const std::uint32_t roff = retail_gap[block.i + k], doff = debug_gap[block.j + k];
                if (!is_target(roff)) continue;
                const Tier tier = retail_look.at(roff).strict == debug_look.at(doff).strict ? Tier::Exact : Tier::High;
                out[roff] = {debug_names.at(doff), tier, 1.0};
                used.insert(doff);
            }
        }
        std::vector<std::uint32_t> retail_rest, debug_rest;
        for (const std::uint32_t x : retail_gap)
            if (!out.contains(x)) retail_rest.push_back(x);
        for (const std::uint32_t x : debug_gap)
            if (!used.contains(x)) debug_rest.push_back(x);

        // as many left on each side: they stand in the same order
        if (!retail_rest.empty() && retail_rest.size() == debug_rest.size()) {
            for (std::size_t k = 0; k < retail_rest.size(); ++k) {
                const std::uint32_t roff = retail_rest[k], doff = debug_rest[k];
                if (!is_target(roff)) continue;
                const double score = mnemonic_ratio(ids, retail_look.at(roff).mnemonics, debug_look.at(doff).mnemonics);
                out[roff] = {debug_names.at(doff), Tier::Aligned, round_to_two_places(score)};
                used.insert(doff);
            }
            continue;
        }
        // otherwise the likeliest unused function of the gap, if it is alike enough
        for (const std::uint32_t roff : retail_gap) {
            if (out.contains(roff) || !is_target(roff)) continue;
            std::optional<std::uint32_t> best;
            double best_score = 0.0;
            for (const std::uint32_t doff : debug_gap) {
                if (used.contains(doff)) continue;
                const double score = mnemonic_ratio(ids, retail_look.at(roff).mnemonics, debug_look.at(doff).mnemonics);
                if (score > best_score) {
                    best = doff;
                    best_score = score;
                }
            }
            if (best && best_score >= kFuzzyMin) {
                out[roff] = {debug_names.at(*best), best_score >= kProbableMin ? Tier::Probable : Tier::Guess, round_to_two_places(best_score)};
                used.insert(*best);
            }
        }
    }
    return out;
}

Report estimate_module(const ne::Module& retail, const ne::Module& debug_module, std::string_view map_text, const segment::SharedOrdinals& ordinals,
                       const source::ne_asm::fixups::Objdump& objdump) {
    const auto debug_names = debug_build::parse_map(map_text);
    const debug_build::Names exact = debug_build::match_module(retail, debug_build::build_index(debug_module, map_text, ordinals, objdump), ordinals,
                                                              debug_build::kMinLength, objdump).names;
    Report report;
    for (const std::uint32_t i : code_segment_numbers(retail)) {
        const auto named = debug_names.find(i);
        if (named == debug_names.end()) continue;
        const segment::Seg retail_seg(retail, i, ordinals, {}, objdump), debug_seg(debug_module, i, ordinals, {}, objdump);
        for (const auto& [start, func] : retail_seg.functions()) {   // the validated anchors are locked in
            const auto name = exact.find({i, start});
            if (name != exact.end() && !name->second.empty()) report.tiers[{i, start}] = {name->second, Tier::Exact, 1.0};
        }
        for (const auto& [start, estimate] : align_segment(i, retail_seg, debug_seg, named->second, exact))
            report.tiers.try_emplace({i, start}, estimate);   // (an anchor stays what it is)
        for (const auto& [start, estimate] : align_segment(i, retail_seg, debug_seg, named->second, exact, true)) {
            Accuracy& slot = report.accuracy[static_cast<std::size_t>(estimate.tier)];
            ++slot.total;
            slot.correct += text::upper(estimate.name) == text::upper(retail_seg.name_at(start));
        }
    }
    return report;
}

debug_build::Names confident_names(const Estimates& tiers, const ne::Module& retail, const segment::SharedOrdinals& ordinals,
                                   const source::ne_asm::fixups::Objdump& objdump) {
    std::map<std::string, FunctionKey> export_at;   // UPPERCASE export name -> where
    for (const std::uint32_t i : code_segment_numbers(retail)) {
        const segment::Seg seg(retail, i, ordinals, {}, objdump);
        for (const auto& [offset, name] : seg.export_names()) export_at[text::upper(name)] = {i, offset};
    }
    debug_build::Names confident;
    for (const auto& [key, estimate] : tiers) {
        if (!is_confident(estimate.tier)) continue;
        const auto clash = export_at.find(text::upper(estimate.name));
        if (clash != export_at.end() && !(clash->second == key)) continue;   // never shadow a real export elsewhere
        confident[key] = estimate.name;
    }
    std::map<std::string, std::size_t> claims;
    for (const auto& [key, name] : confident) ++claims[name];
    std::erase_if(confident, [&](const auto& kv) { return claims.at(kv.second) > 1; });   // a name two functions claim is neither's
    return confident;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "debug-estimate",
        "estimate the internal names of KRNL386 from Microsoft's debug build of it, in measured tiers",
        {cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const segment::SharedOrdinals ordinals = std::make_shared<const segment::Ordinals>(segment::load_ordinals(fs::from_text(a.required("--ords"))));
            const std::filesystem::path retail_path = env.corpus.resolve("krnl386.exe");
            const std::filesystem::path debug_folder = env.corpus.reference({"WIN32SDK", "SCT", "DEBUG16", "I386"});
            const ne::Module retail = source::ne_asm::fixups::open_module(retail_path);
            const ne::Module debug_module = source::ne_asm::fixups::open_module(debug_folder / "KRNL386.EXE");
            const Report report = estimate_module(retail, debug_module, fs::read_text(debug_folder / "KRNL386.MAP"), ordinals, env.objdump);
            const debug_build::Names confident = confident_names(report.tiers, retail, ordinals, env.objdump);
            const std::filesystem::path out = env.state_folder / "idiomatic" / "krnl386" / "names.json";
            fs::make_dirs(out.parent_path());
            fs::write_text(out, debug_build::to_json(confident));
            for (const Tier tier : kTiers) {
                const Accuracy& acc = report.of(tier);
                const std::string accuracy =
                    acc.total ? std::format("{}/{}={:.0f}%", acc.correct, acc.total, 100.0 * static_cast<double>(acc.correct) / static_cast<double>(acc.total)) : "-";
                cb::out("  {:<9} {:>4}   export-accuracy {}\n", tier_name(tier), report.count(tier), accuracy);
            }
            cb::out("confident names.json (EXACT+HIGH+ALIGNED, guarded): {}\n", confident.size());
            return 0;
        },
        {}, {}, {}};
    const cli::Cli program("debug-estimate", {command});
    std::vector<std::string> line = {"debug-estimate"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::symbols::debug_build_estimate
