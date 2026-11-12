// cb/verify/authored/match16.cpp -- the 16-bit match driver (see the header).
#include "cb/verify/authored/match16.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::verify::authored::match16 {

double score(const Verdict& v) {
    if (v.match == ByteMatch::NoCompile) return 0.0;
    return v.match == ByteMatch::Identical ? 2.0 : 1.0 - static_cast<double>(v.differing) / static_cast<double>(std::max<std::size_t>(v.total, 1));
}

double score(std::span<const Verdict> verdicts) {
    double total = 0.0;
    for (const Verdict& v : verdicts) total += score(v);
    return total;
}

// ---- the original's source files ---------------------------------------------------------------------------------------------

std::vector<std::vector<std::string>> source_files(std::span<const Function> functions, const platforms::Decoder& x86_16) {
    const auto key = [](std::uint32_t segment, std::uint32_t offset) { return (static_cast<std::uint64_t>(segment) << 32) | offset; };
    std::map<std::uint64_t, const Function*> at;                  // (segment, offset) -> the function there (lookup only)
    std::vector<std::uint32_t> segments;                          // in the order a segment is first met
    std::map<std::uint32_t, std::vector<const Function*>> by_segment;
    for (const Function& f : functions) {
        at[key(f.segment, f.offset)] = &f;
        if (by_segment.find(f.segment) == by_segment.end()) segments.push_back(f.segment);
        by_segment[f.segment].push_back(&f);
    }
    std::vector<std::vector<std::string>> files;
    for (const std::uint32_t segment : segments) {
        std::vector<const Function*>& fns = by_segment[segment];
        std::stable_sort(fns.begin(), fns.end(), [](const Function* a, const Function* b) { return a->offset < b->offset; });
        std::map<const Function*, std::size_t> index;
        for (std::size_t i = 0; i < fns.size(); ++i) index[fns[i]] = i;
        std::vector<bool> joined(fns.size(), false);              // joined[i]: fns[i] and fns[i + 1] are one file
        for (const Function* fn : fns) {
            for (const platforms::Insn& insn : x86_16.disasm(fn->original.code, 0)) {
                // a direct near call: `push cs; call` of a far one, or a near callee
                if (insn.mnemonic != "call" || !insn.op_str.starts_with("0x") || insn.op_str.find(',') != std::string::npos) continue;
                const auto delta = text::parse_int<std::uint64_t>(std::string_view(insn.op_str).substr(2), 16);
                if (!delta) continue;
                const auto other = at.find(key(segment, static_cast<std::uint32_t>((fn->offset + *delta) & 0xFFFF)));
                if (other == at.end() || other->second == fn) continue;
                const auto [low, high] = std::minmax(index[fn], index[other->second]);
                for (std::size_t j = low; j < high; ++j) joined[j] = true;
            }
        }
        std::vector<std::string> current = {fns[0]->name};
        for (std::size_t j = 1; j < fns.size(); ++j) {
            if (joined[j - 1]) {
                current.push_back(fns[j]->name);
            } else {
                files.push_back(std::move(current));
                current = {fns[j]->name};
            }
        }
        files.push_back(std::move(current));
    }
    return files;
}

std::vector<Variant> make_variants(std::span<const std::string> stages) {
    std::vector<Variant> out;
    for (std::size_t k = 0; k < (std::size_t{1} << stages.size()); ++k) {
        Variant v;
        for (std::size_t b = 0; b < stages.size(); ++b) {
            if ((k >> b) & 1) v.stages.insert(stages[b]);
        }
        for (const std::string& stage : stages) {   // named in the stages' own order
            if (v.stages.contains(stage)) v.name += (v.name.empty() ? "" : "+") + stage;
        }
        if (v.name.empty()) v.name = "plain";
        out.push_back(std::move(v));
    }
    return out;
}

namespace {

// What an earlier run remembered of the choices: each segment's setting and the settings a function does better under.
struct Remembered {
    std::map<std::uint32_t, Setting> segments;
    std::map<std::string, Setting> functions;
};

Setting m16_setting(const Json& pair) { return {pair.at(0).get<std::string>(), pair.at(1).get<std::string>()}; }

Remembered m16_read(const std::filesystem::path& file) {
    Remembered out;
    Json saved = json::load(file);
    if (!saved.contains("segments")) saved = Json{{"segments", saved}};   // (an earlier layout: the segments alone)
    for (const auto& [seg, pair] : saved.at("segments").items()) {
        const auto n = text::parse_int<std::uint32_t>(seg);
        if (!n) throw ParseError(file.string(), 0, 0, "a segment number that is not a number: " + seg);
        out.segments[*n] = m16_setting(pair);
    }
    if (saved.contains("functions")) {
        for (const auto& [fn, pair] : saved.at("functions").items()) out.functions[fn] = m16_setting(pair);
    }
    return out;
}

void m16_write(const std::filesystem::path& file, const std::map<std::uint32_t, Setting>& chosen, const std::map<std::string, Setting>& each) {
    Json segments = Json::object();
    for (const auto& [seg, s] : chosen) segments[std::to_string(seg)] = Json::array({s.compiler, s.switches});
    Json functions = Json::object();
    for (const auto& [fn, s] : each) functions[fn] = Json::array({s.compiler, s.switches});   // (in name order)
    Json doc = Json::object();
    doc["segments"] = std::move(segments);
    doc["functions"] = std::move(functions);
    json::save(file, doc, json::Style::PythonCompatiblePretty);
}

// The first of the greatest: Python's max(items, key=...) over a list in its order.
template <class T, class Key>
const T& m16_first_greatest(const std::vector<T>& items, Key key) {
    const T* best = &items.front();
    for (const T& item : items) {
        if (key(item) > key(*best)) best = &item;
    }
    return *best;
}

}  // namespace

// ---- the run ------------------------------------------------------------------------------------------------------------------------

Result run(const Inputs& in) {
    if (in.toolchain == nullptr) throw UsageError("match16: no toolchain");
    if (in.compilers.empty()) throw UsageError("Microsoft C is not set up (settings.json: dosbox_x, msc6, msvc152)");
    Toolchain& chain = *in.toolchain;
    const std::vector<Function>& todo = in.functions;
    const auto log = [&](const std::string& line) {
        if (in.log) in.log(line);
    };

    Result r;
    std::map<std::string, const Function*> by_name;
    for (const Function& f : todo) {
        by_name[f.name] = &f;
        r.segment[f.name] = f.segment;
    }
    r.files = source_files(todo, platforms::Decoder::open(platforms::Arch::X86, 16, false));
    std::map<std::string, std::size_t> file_of;
    for (std::size_t k = 0; k < r.files.size(); ++k) {
        for (const std::string& fn : r.files[k]) file_of[fn] = k;
    }
    std::vector<Variant> variants = in.variants;   // (the caller's; a searched text is kept apart below)
    const auto variant_named = [&](const std::string& name) -> Variant* {
        for (Variant& v : variants) {
            if (v.name == name) return &v;
        }
        return nullptr;
    };
    std::map<std::string, std::string> searched_texts;   // function -> a rewrite that came nearer than any way of saying it
    const auto text_of = [&](const std::string& fn, const std::string& variant) -> const std::string& {
        if (variant == "search") return searched_texts.at(fn);
        return variant_named(variant)->texts.at(fn);
    };

    // Each function judged, in the ways named, under one setting: the nearest of them kept (and which one it was).
    const auto judged = [&](const Setting& setting, const std::vector<std::string>& names, const std::vector<std::string>& ways) {
        std::map<std::string, Verdict> best;
        for (const std::string& way : ways) {
            Compilation request{setting.compiler, setting.switches, variant_named(way)->header, {}, &r.files};
            for (const std::string& fn : names) request.sources.emplace_back(fn, variant_named(way)->texts.at(fn));
            const std::map<std::string, std::optional<Masked>> built = chain.compile(request);
            for (const std::string& fn : names) {
                const auto it = built.find(fn);
                const std::optional<Masked>* code = it != built.end() && it->second ? &it->second : nullptr;
                const Verdict got = code != nullptr ? chain.distance(**code, by_name.at(fn)->original) : Verdict{ByteMatch::NoCompile, 0, 0};
                if (!best.contains(fn) || score(got) > score(best.at(fn))) {
                    best[fn] = got;
                    r.winner[fn] = way;
                    r.code[fn] = code != nullptr ? *code : std::nullopt;
                }
            }
        }
        return best;
    };

    std::set<std::uint32_t> segment_set;
    for (const Function& f : todo) segment_set.insert(f.segment);
    const std::vector<std::uint32_t> segs(segment_set.begin(), segment_set.end());
    const auto setting_text = [](const Setting& s) { return s.compiler + " " + s.switches; };

    Remembered known;
    if (!in.choices_file.empty() && std::filesystem::is_regular_file(in.choices_file) && !in.choose && !in.compiler_given) known = m16_read(in.choices_file);
    std::map<std::string, Setting> each = known.functions;
    const bool all_known = std::all_of(segs.begin(), segs.end(), [&](std::uint32_t s) { return known.segments.contains(s); });
    if (in.switches && !in.switches->empty()) {
        for (const std::uint32_t s : segs) r.chosen[s] = {in.compilers.front(), *in.switches};
    } else if (all_known) {
        r.chosen = known.segments;   // (found by an earlier run: --choose finds them again)
        log("  each segment's and function's compiler and switches as found before (" + in.choices_file.string() + ")");
    } else {
        // a sample of each segment's functions (every few of them, six at most), judged under every setting
        std::map<std::uint32_t, std::vector<std::string>> sample;
        std::vector<std::string> every;
        for (const std::uint32_t s : segs) {
            std::vector<std::string> v;
            for (const Function& f : todo) {
                if (f.segment == s) v.push_back(f.name);
            }
            const std::size_t step = std::max<std::size_t>(1, v.size() / 6);
            for (std::size_t i = 0; i < v.size() && sample[s].size() < 6; i += step) sample[s].push_back(v[i]);
            every.insert(every.end(), sample[s].begin(), sample[s].end());
        }
        struct Score {
            std::uint32_t segment;
            Setting setting;
            double score;
        };
        std::vector<Score> scores;   // in the order made
        std::vector<std::string> trial;
        for (const char* v : {"plain", "fold", "frame", "fold+frame"}) {
            if (variant_named(v) != nullptr) trial.push_back(v);
        }
        for (const std::string& compiler : in.compilers) {
            for (const std::string& switches : chain.flag_sets(compiler)) {
                const std::map<std::string, Verdict> got = judged({compiler, switches}, every, trial);
                for (const std::uint32_t s : segs) {
                    double total = 0.0;
                    for (const std::string& fn : sample[s]) total += score(got.at(fn));
                    scores.push_back({s, {compiler, switches}, total});
                }
            }
        }
        std::map<std::uint32_t, std::vector<Setting>> top;
        for (const std::uint32_t s : segs) {
            std::vector<Score> ranked;
            for (const Score& sc : scores) {
                if (sc.segment == s) ranked.push_back(sc);
            }
            std::stable_sort(ranked.begin(), ranked.end(), [](const Score& a, const Score& b) { return a.score > b.score; });
            r.chosen[s] = ranked.front().setting;
            for (std::size_t i = 0; i < std::min<std::size_t>(3, ranked.size()); ++i) top[s].push_back(ranked[i].setting);
            log(std::format("  segment {:<3} {:<8} {:<16} ({:.1f} of a sample of {})", s, ranked.front().setting.compiler, ranked.front().setting.switches,
                            ranked.front().score, sample[s].size()));
        }
        // A segment of the file can be several source files' code (the linker packs them), each built with its own switches: every
        // function is judged under its segment's three best, and keeps the one it comes nearest under (its segment's first where
        // none is nearer).
        each.clear();
        struct Tried {
            double score;
            bool first;
            Setting setting;
        };
        std::map<std::string, std::vector<Tried>> tried;
        std::set<Setting> tops;
        for (const auto& [s, settings] : top) tops.insert(settings.begin(), settings.end());
        for (const Setting& c : tops) {
            std::vector<std::string> names;
            for (const Function& f : todo) {
                const std::vector<Setting>& t = top[f.segment];
                if (std::find(t.begin(), t.end(), c) != t.end()) names.push_back(f.name);
            }
            for (const auto& [fn, v] : judged(c, names, {"plain"})) tried[fn].push_back({score(v), c == r.chosen.at(r.segment.at(fn)), c});
        }
        for (const std::vector<std::string>& file : r.files) {   // one file, one command line: the setting its functions come nearest under together
            std::vector<std::pair<Setting, double>> total;
            for (const std::string& fn : file) {
                const auto t = tried.find(fn);
                if (t == tried.end()) continue;
                for (const Tried& x : t->second) {
                    auto it = std::find_if(total.begin(), total.end(), [&](const auto& p) { return p.first == x.setting; });
                    if (it == total.end()) it = total.insert(total.end(), {x.setting, 0.0});
                    it->second += x.score + (x.first ? 1e-6 : 0.0);
                }
            }
            if (total.empty()) continue;
            const Setting best = m16_first_greatest(total, [](const auto& p) { return p.second; }).first;
            for (const std::string& fn : file) {
                if (best != r.chosen.at(r.segment.at(fn))) each[fn] = best;
            }
        }
        log(std::format("  {} functions nearer under another of their segment's switches than its first", each.size()));
        if (!in.compiler_given && !in.choices_file.empty()) m16_write(in.choices_file, r.chosen, each);
    }
    for (const Function& f : todo) r.setting[f.name] = each.contains(f.name) ? each.at(f.name) : r.chosen.at(f.segment);
    for (const std::vector<std::string>& file : r.files) {   // (one setting a file: the one most of its functions have, the first where they tie)
        std::vector<Setting> votes;
        for (const std::string& fn : file) votes.push_back(r.setting.at(fn));
        const Setting one = m16_first_greatest(votes, [&](const Setting& v) { return std::count(votes.begin(), votes.end(), v); });
        for (const std::string& fn : file) r.setting[fn] = one;
    }
    std::set<Setting> used;
    for (const auto& [fn, s] : r.setting) used.insert(s);
    std::vector<std::string> all_ways;
    for (const Variant& v : variants) all_ways.push_back(v.name);
    for (const Setting& c : used) {
        std::vector<std::string> names;
        for (const Function& f : todo) {
            if (r.setting.at(f.name) == c) names.push_back(f.name);
        }
        for (const auto& [fn, v] : judged(c, names, all_ways)) r.verdicts[fn] = v;
    }
    if (in.identify) {   // (the compiler's run-time library: linked, not written)
        r.library = in.identify(todo);
        for (const auto& [fn, name] : r.library) {
            const auto it = r.verdicts.find(fn);
            if (it == r.verdicts.end() || it->second.match != ByteMatch::Identical) {
                r.verdicts[fn] = {ByteMatch::Identified, 0, by_name.at(fn)->original.code.size()};
            }
        }
    }
    if (in.search_rounds > 0) {   // the last bytes of the near misses (verify/authored/search16)
        std::vector<search16::NearMiss> near;
        for (const auto& [fn, v] : r.verdicts) {
            if ((v.match == ByteMatch::SameSize || v.match == ByteMatch::Different) && static_cast<double>(v.differing) <= 0.4 * static_cast<double>(v.total)) {
                const Setting& s = r.setting.at(fn);
                near.push_back({fn, text_of(fn, r.winner.at(fn)), v, {s.compiler, s.switches, r.winner.at(fn)}});
            }
        }
        const search16::CompileGroup compile_group = [&](const search16::Group& g, const std::map<std::string, std::string>& sources) {
            std::set<std::size_t> in_files;   // (a rewrite is judged in its file, among the other functions' best texts; files in ascending order)
            for (const auto& [fn, text] : sources) in_files.insert(file_of.at(fn));
            Compilation request;
            request.compiler = g.compiler;
            request.switches = g.switches;
            request.files = &r.files;
            const Variant* named = variant_named(g.winner);
            request.header = named != nullptr ? named->header : in.header;
            for (const std::size_t k : in_files) {
                for (const std::string& fn : r.files[k]) {
                    if (!sources.contains(fn) && r.winner.contains(fn)) request.sources.emplace_back(fn, text_of(fn, r.winner.at(fn)));
                }
            }
            for (const auto& [fn, text] : sources) request.sources.emplace_back(fn, text);
            const std::map<std::string, std::optional<Masked>> built = chain.compile(request);
            std::map<std::string, search16::ByteVerdict> out;
            for (const auto& [fn, text] : sources) {
                const auto it = built.find(fn);
                out[fn] = it != built.end() && it->second ? chain.distance(*it->second, by_name.at(fn)->original) : Verdict{ByteMatch::NoCompile, 0, 0};
            }
            return out;
        };
        search16::Options options;
        options.rounds = in.search_rounds;
        options.log = in.log;
        for (const search16::Best& best : search16::search(near, compile_group, [](const Verdict& v) { return score(v); }, options)) {
            if (score(best.verdict) > score(r.verdicts.at(best.function))) {
                r.verdicts[best.function] = best.verdict;
                r.winner[best.function] = "search";
                searched_texts[best.function] = best.text;
            }
        }
    }
    for (const Function& f : todo) {
        if (r.winner.contains(f.name)) r.texts[f.name] = text_of(f.name, r.winner.at(f.name));
    }
    for (const auto& [fn, s] : r.setting) r.settings.insert(s);
    return r;
}

// ---- the report -----------------------------------------------------------------------------------------------------------------------

std::vector<std::string> report_lines(std::string_view module_name, const Result& result) {
    std::vector<std::string> lines;
    std::map<ByteMatch, std::size_t> counts;
    for (const auto& [fn, v] : result.verdicts) ++counts[v.match];
    // the settings the functions were judged under (the segments' where none were), each once, in order
    std::set<Setting> used = result.settings;
    if (used.empty()) {
        for (const auto& [seg, setting] : result.chosen) used.insert(setting);
    }
    std::set<std::string> texts;
    for (const Setting& s : used) texts.insert(s.compiler + " " + s.switches);
    std::string settings;
    for (const std::string& t : texts) settings += (settings.empty() ? "" : "; ") + t;
    std::string by_verdict;
    for (const ByteMatch k : {ByteMatch::Identical, ByteMatch::Identified, ByteMatch::SameSize, ByteMatch::Different, ByteMatch::NoCompile}) {
        if (counts[k] != 0) by_verdict += (by_verdict.empty() ? "" : ", ") + std::string(label(k)) + " " + std::to_string(counts[k]);
    }
    lines.push_back(std::format("{}: {} functions compiled with Microsoft C ({}): {}", module_name, result.verdicts.size(), settings, by_verdict));
    std::map<std::string, std::size_t> won;   // by way of saying, in name order
    for (const auto& [fn, way] : result.winner) {
        const auto v = result.verdicts.find(fn);
        if (v != result.verdicts.end() && v->second.match == ByteMatch::Identical) ++won[way];
    }
    if (!won.empty()) {
        std::string parts;
        for (const auto& [way, n] : won) parts += (parts.empty() ? "" : ", ") + way + " " + std::to_string(n);
        lines.push_back("  identical by way of saying: " + parts);
    }
    struct Miss {
        double share;
        std::string name;
        Verdict v;
    };
    std::vector<Miss> near;
    for (const auto& [fn, v] : result.verdicts) {
        if (v.match == ByteMatch::SameSize || v.match == ByteMatch::Different) {
            near.push_back({static_cast<double>(v.differing) / static_cast<double>(std::max<std::size_t>(v.total, 1)), fn, v});
        }
    }
    std::sort(near.begin(), near.end(), [](const Miss& a, const Miss& b) { return std::tie(a.share, a.name) < std::tie(b.share, b.name); });
    for (std::size_t i = 0; i < std::min<std::size_t>(8, near.size()); ++i) {
        lines.push_back(std::format("  {:<28} {:<10} {} of {} bytes differ", near[i].name, label(near[i].v.match), near[i].v.differing, near[i].v.total));
    }
    return lines;
}

}  // namespace cb::verify::authored::match16
