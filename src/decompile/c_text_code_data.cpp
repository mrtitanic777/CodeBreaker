// decompile/c_text_code_data.cpp -- data kept in a code segment, read through CS.
//
// KEYBOARD's GetKeyNameText looks a key up in tables beside its own code (9:0261, 9:02E9 ...) and reads them through CS:
// `MK_FP(unaff_CS, pcVar11)` with pcVar11 set to 0x261. The rebuilt code segment is laid out by the compiler -- functions rebuilt
// from C are other sizes, some are moved -- so CS:0261 is not the table any more, and in a function judged on its own it is whatever
// the throw-away build has there. The source such a module was written from named the table, in the code segment
// (`static char _based(_segname("_CODE")) ...`): so does this. The bytes a function's C reads through CS are the original's, in a
// structure in its own code segment, codebreaker_code_<n> (one member at each place the code starts reading from), and each number
// that is such a place is written as the member's offset.
#include <algorithm>
#include <format>
#include <nlohmann/json.hpp>

#include "decompile/data.hpp"
#include "core/error.hpp"
#include "core/io.hpp"
#include "core/json.hpp"
#include "core/text.hpp"
#include "decompile/c_text_internal.hpp"

namespace cb::decompile::c_text::internal {

namespace {

using Run = std::pair<std::int64_t, std::int64_t>;

bool cd_is_cs_return(const std::string& mn) { return mn == "ret" || mn == "retf" || mn == "iret" || mn == "iretd" || mn == "hlt"; }

// The branch target a jump, loop or call names as a number (`0x1c2`, `450`); nullopt for anything else.
std::optional<std::int64_t> cd_branch_target(std::string_view op_str) {
    const std::string_view s = text::trim(op_str);
    if (s.starts_with("0x") && s.size() > 2) return text::parse_int<std::int64_t>(s.substr(2), 16);
    if (!s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; })) return text::parse_int<std::int64_t>(s, 10);
    return std::nullopt;
}

// Which bytes of a code segment are its functions' instructions: each function followed from its start, every branch it can take.
// 1 at a byte of code. (A place only a jump table reaches is taken for data -- its bytes copied as data is all that comes of it.)
std::vector<std::uint8_t> cd_code_bytes(const Bytes& image, const std::vector<std::int64_t>& starts) {
    const describe::Code16 decoded{Bytes(image)};
    const std::int64_t n = static_cast<std::int64_t>(image.size());
    std::vector<std::uint8_t> code(image.size(), 0);
    std::set<std::int64_t> seen;
    std::vector<std::int64_t> todo;
    for (const std::int64_t s : starts)
        if (0 <= s && s < n) todo.push_back(s);
    while (!todo.empty()) {
        std::int64_t a = todo.back();
        todo.pop_back();
        while (0 <= a && a < n && !seen.contains(a)) {
            const describe::Insn* one = decoded.one(a);
            if (one == nullptr) break;
            seen.insert(a);
            for (std::int64_t k = a; k < std::min(a + one->size, n); ++k) code[static_cast<std::size_t>(k)] = 1;
            if (cd_is_cs_return(one->mn)) break;
            if (one->mn.starts_with("j") || one->mn.starts_with("loop") || one->mn == "call") {
                if (const auto target = cd_branch_target(one->op_str)) todo.push_back(*target);
            }
            if (one->mn == "jmp" || one->mn == "ljmp") break;
            a += one->size;
        }
    }
    return code;
}

// [(start, end)] of the bytes of a code segment that are not instructions
std::vector<Run> cd_data_runs(const std::vector<std::uint8_t>& code) {
    std::vector<Run> runs;
    std::size_t k = 0;
    while (k < code.size()) {
        if (code[k] != 0) {
            ++k;
            continue;
        }
        const std::size_t s = k;
        while (k < code.size() && code[k] == 0) ++k;
        runs.emplace_back(static_cast<std::int64_t>(s), static_cast<std::int64_t>(k));
    }
    return runs;
}

// `items` joined by commas, `per` to a line (an initialiser's elements)
std::string cd_rows(const std::vector<std::string>& items, std::size_t per) {
    if (items.size() <= per) return text::join(items, ", ");
    std::vector<std::string> rows;
    for (std::size_t k = 0; k < items.size(); k += per) {
        const std::vector<std::string> part(items.begin() + static_cast<std::ptrdiff_t>(k),
                                            items.begin() + static_cast<std::ptrdiff_t>(std::min(items.size(), k + per)));
        rows.push_back(text::join(part, ", "));
    }
    return "\n    " + text::join(rows, ",\n    ") + "\n  ";
}

// ---- what writing a function found of its reads through CS (the back end's report, in the library's tagged JSON) -------------

// A sequence as the library writes one: an array, or a tuple or set tagged as one.
const Json& cd_seq(const Json& v) {
    if (v.is_object()) {
        if (v.contains("$t")) return v.at("$t");
        if (v.contains("$s")) return v.at("$s");
    }
    return v;
}

// A dict with keys that are numbers, as the library writes one: {"$d": [[key, value], ...]}.
std::vector<std::pair<std::int64_t, std::int64_t>> cd_int_pairs(const Json& v) {
    std::vector<std::pair<std::int64_t, std::int64_t>> out;
    if (v.is_object() && v.contains("$d")) {
        for (const Json& kv : v.at("$d")) out.emplace_back(kv.at(0).get<std::int64_t>(), kv.at(1).get<std::int64_t>());
    } else if (v.is_array()) {   // (a list of pairs: the tuples of `each`)
        for (const Json& kv : v) out.emplace_back(cd_seq(kv).at(0).get<std::int64_t>(), cd_seq(kv).at(1).get<std::int64_t>());
    }
    return out;
}

struct CodeReads {
    std::int64_t reads = 0;
    std::vector<std::pair<std::int64_t, std::int64_t>> places;                          // {k: v}
    std::vector<std::pair<std::string, std::pair<std::int64_t, std::int64_t>>> events;  // [(kind, (k, v))]
    std::vector<std::string> unknown;
    std::vector<std::vector<std::pair<std::int64_t, std::int64_t>>> each;
    std::vector<std::string> unsafe;
};

CodeReads cd_parse_reads(const Json& report) {
    CodeReads r;
    r.reads = report.at("reads").get<std::int64_t>();
    r.places = cd_int_pairs(report.at("places"));
    for (const Json& e : cd_seq(report.at("events"))) {
        const Json& kind = cd_seq(e).at(0);
        const Json& place = cd_seq(cd_seq(e).at(1));
        r.events.emplace_back(kind.get<std::string>(), std::make_pair(place.at(0).get<std::int64_t>(), place.at(1).get<std::int64_t>()));
    }
    for (const Json& u : cd_seq(report.at("unknown"))) r.unknown.push_back(u.get<std::string>());
    for (const Json& g : cd_seq(report.at("each"))) r.each.push_back(cd_int_pairs(cd_seq(g)));
    for (const Json& u : cd_seq(report.at("unsafe"))) r.unsafe.push_back(u.is_string() ? u.get<std::string>() : u.dump());
    return r;
}

}  // namespace

CodeData code_segment_data(const std::vector<std::pair<std::string, std::optional<std::string>>>& reports,
                           const std::map<std::string, std::optional<SegOff>>& where, const describe::NeDescription& d,
                           const std::filesystem::path& module) {
    CodeData out;
    struct Found {
        std::string name;
        CodeReads rep;
        std::int64_t seg = 0;
    };
    std::vector<Found> found;
    for (const auto& [name, report] : reports) {
        const auto w = where.find(name);
        if (!report || w == where.end() || !w->second) continue;
        const Json parsed = json::parse(*report);
        if (parsed.is_null() || (parsed.is_object() && parsed.empty())) continue;
        found.push_back({name, cd_parse_reads(parsed), w->second->seg});
    }
    if (found.empty()) return out;
    const formats::ne::Module ne = formats::ne::Module::open_file(module);
    std::map<std::int64_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : ne.segments()) segs[s.index] = s;
    std::map<std::int64_t, std::vector<std::int64_t>> starts;
    for (const auto& [va, place] : d.where_map) starts[place.seg].push_back(place.off);
    std::map<std::int64_t, std::vector<Run>> runs;
    std::map<std::int64_t, std::vector<std::int64_t>> relocated;
    std::set<std::int64_t> found_segs;
    for (const Found& f : found) found_segs.insert(f.seg);
    static const Bytes kNoBytes;
    auto image_of = [&](std::int64_t seg) -> const Bytes& {
        const auto it = d.images.find(seg);
        return it == d.images.end() ? kNoBytes : it->second;
    };
    for (const std::int64_t sg : found_segs) {
        runs[sg] = cd_data_runs(cd_code_bytes(image_of(sg), starts[sg]));
        std::vector<std::int64_t> rel;
        const auto s = segs.find(sg);
        if (s != segs.end() && s->second.file_offset != 0) {
            const ByteView blob = io::checked_range(ne.data(), s->second.file_offset, s->second.length);
            for (const decompile::data::RelocationPlace& p : decompile::data::places(ne.relocations(s->second), blob)) rel.push_back(p.offset);
            std::sort(rel.begin(), rel.end());
        }
        relocated[sg] = std::move(rel);
    }
    // the run of data a place is in (bisect_right on the starts, then the one before)
    auto run_of = [&](std::int64_t sg, std::int64_t v) -> std::optional<Run> {
        const auto it = runs.find(sg);
        if (it == runs.end()) return std::nullopt;
        const std::vector<Run>& rs = it->second;
        const auto after = std::upper_bound(rs.begin(), rs.end(), Run(v, 0x10000));
        if (after == rs.begin()) return std::nullopt;
        const Run& r = *(after - 1);
        return (r.first <= v && v < r.second) ? std::optional<Run>(r) : std::nullopt;
    };
    struct Per {
        std::string name;
        std::vector<std::pair<std::int64_t, std::pair<std::int64_t, std::int64_t>>> places;   // k -> (k, v)
        std::vector<std::pair<std::int64_t, std::string>> kinds;                                // v -> kind (in the order the reads found them)
    };
    std::map<std::int64_t, std::vector<Per>> per;
    std::map<std::int64_t, std::vector<std::set<std::int64_t>>> each_read;
    std::map<std::int64_t, CodeSegmentReport> report;
    for (const Found& f : found) {
        const std::int64_t sg = f.seg;
        auto inside = [&](std::int64_t v) { return run_of(sg, v).has_value(); };
        Per p;
        p.name = f.name;
        for (const auto& [k, v] : f.rep.places)
            if (inside(v)) p.places.emplace_back(k, std::make_pair(k, v));
        for (const auto& [kind, place] : f.rep.events) {
            const std::int64_t v = place.second;
            if (!inside(v)) continue;
            const auto have = std::find_if(p.kinds.begin(), p.kinds.end(), [&](const auto& kk) { return kk.first == v; });
            if (have == p.kinds.end()) p.kinds.emplace_back(v, kind);
            else if (kind == "call") have->second = kind;
        }
        std::vector<std::set<std::int64_t>> each;
        for (const auto& got : f.rep.each) {
            std::set<std::int64_t> s;
            for (const auto& [k, v] : got)
                if (inside(v)) s.insert(v);
            each.push_back(std::move(s));
        }
        CodeSegmentReport& r = report[sg];
        r.read += f.rep.reads;
        if (!f.rep.unsafe.empty()) {   // (a table of places it cannot write as places)
            r.left.push_back(f.name + ": an offset it reads through CS is a " + f.rep.unsafe[0]);
            continue;
        }
        const bool no_places = p.places.empty();
        per[sg].push_back(std::move(p));
        for (auto& s : each) each_read[sg].push_back(std::move(s));
        if (no_places) {
            const std::set<std::string> unique(f.rep.unknown.begin(), f.rep.unknown.end());
            r.left.push_back(f.name + ": its offsets through CS come from " +
                             (unique.empty() ? std::string("nothing it sets to a place in the data") : text::join(unique, ", ")));
        }
    }
    for (auto& [sg, pers] : per) {
        const Bytes& image = image_of(sg);
        std::set<std::int64_t> bases;
        std::map<std::int64_t, std::string> kinds;
        for (const Per& p : pers) {
            for (const auto& [k, place] : p.places) bases.insert(place.second);
            for (const auto& [v, kind] : p.kinds)
                if (const auto it = kinds.find(v); it == kinds.end() || it->second != "call") kinds[v] = kind;
        }
        std::set<std::int64_t> calls;
        for (const auto& [v, kind] : kinds)
            if (kind == "call") calls.insert(v);
        if (!calls.empty()) {   // (its addresses would be the original's)
            std::vector<std::string> cs;
            for (const std::int64_t v : calls) cs.push_back(std::format("{:04x}", v));
            report[sg].left.push_back("tables of the code's own addresses, called: " + text::join(cs, ", "));
        }
        for (const std::int64_t c : calls) bases.erase(c);
        // what each table of places holds: words, each a place in the data, up to the first of those places that is after it (the
        // strings it points at, after the table)
        std::map<std::int64_t, std::vector<std::int64_t>> pointers;
        for (const std::int64_t b : bases) {
            const auto kind = kinds.find(b);
            if (kind == kinds.end() || kind->second != "pointers") continue;
            const Run run = *run_of(sg, b);
            std::int64_t limit = run.second;
            for (const std::int64_t x : bases)
                if (x > b) limit = std::min(limit, x);
            std::int64_t at = b;
            std::vector<std::int64_t> got;
            for (;;) {
                std::int64_t first_after = 0x10000;
                for (const std::int64_t t : got)
                    if (t > b) first_after = std::min(first_after, t);
                if (!(at + 2 <= limit && at < first_after)) break;
                const std::int64_t v = image[static_cast<std::size_t>(at)] | (image[static_cast<std::size_t>(at) + 1] << 8);
                if (!run_of(sg, v) || calls.contains(v)) break;
                got.push_back(v);
                at += 2;
            }
            if (!got.empty()) pointers[b] = std::move(got);
        }
        // the structure: each run of data a place is in, whole (a read may go back from the place: VGA's sub_5_027E reads from 40h
        // down), a member at each place
        std::map<Run, std::int64_t> spans;
        {
            std::set<std::int64_t> all = bases;
            for (const auto& [b, targets] : pointers) all.insert(targets.begin(), targets.end());
            for (const std::int64_t v : all) {
                const Run r = *run_of(sg, v);
                spans[r] = r.first;
            }
        }
        if (spans.empty()) continue;
        std::set<std::int64_t> cuts;
        for (;;) {
            cuts.clear();
            std::set<std::int64_t> targets;
            for (const auto& [b, ts] : pointers) targets.insert(ts.begin(), ts.end());
            for (const auto& [run, lo] : spans) {
                cuts.insert(lo);
                cuts.insert(run.second);
                for (const std::int64_t v : bases)
                    if (lo <= v && v < run.second) cuts.insert(v);
                for (const std::int64_t v : targets)
                    if (lo <= v && v < run.second) cuts.insert(v);
                for (const auto& [b, ts] : pointers)
                    if (lo <= b && b < run.second) cuts.insert(b + 2 * static_cast<std::int64_t>(ts.size()));
            }
            // (a table some place points into the middle of is bytes after all)
            std::vector<std::int64_t> torn;
            for (const auto& [b, ts] : pointers) {
                for (const std::int64_t c : cuts) {
                    if (b < c && c < b + 2 * static_cast<std::int64_t>(ts.size())) {
                        torn.push_back(b);
                        break;
                    }
                }
            }
            if (torn.empty()) break;
            for (const std::int64_t b : torn) pointers.erase(b);
        }
        std::map<std::int64_t, std::string> member_at;
        std::vector<Run> members;
        for (const auto& [run, lo] : spans) {
            std::vector<std::int64_t> edges;
            for (const std::int64_t c : cuts)
                if (lo <= c && c <= run.second) edges.push_back(c);
            for (std::size_t i = 0; i + 1 < edges.size(); ++i) {
                members.emplace_back(edges[i], edges[i + 1]);
                member_at[edges[i]] = std::format("at_{:04x}", edges[i]);
            }
        }
        const std::string struct_name = "codebreaker_code_" + std::to_string(sg);
        auto place = [&](std::int64_t v) {
            std::optional<std::int64_t> k;
            for (const Run& m : members)
                if (m.first <= v) k = std::max(k.value_or(m.first), m.first);
            CB_ASSERT(k.has_value(), "a place in the code segment's data has no member before it");
            return *k == v ? struct_name + "." + member_at[*k] : "&" + struct_name + "." + member_at[*k] + "[" + std::to_string(v - *k) + "]";
        };
        // (a place in a member the loader writes into -- a selector, a far pointer kept there -- is left as it was: the bytes copied
        // would be the made-up values the description gave them)
        std::set<std::int64_t> fixed;
        for (const Run& m : members) {
            for (const std::int64_t r : relocated[sg]) {
                if (m.first <= r && r < m.second) {
                    fixed.insert(m.first);
                    break;
                }
            }
        }
        if (!fixed.empty()) {
            std::vector<std::string> fs;
            for (const std::int64_t a : fixed) fs.push_back(std::format("{:04x}", a));
            report[sg].left.push_back("data the loader fixes up: " + text::join(fs, ", "));
        }
        std::set<std::int64_t> ok_place;
        for (const std::int64_t v : bases) {
            for (const Run& m : members) {
                if (m.first <= v && v < m.second && !fixed.contains(m.first)) {
                    ok_place.insert(v);
                    break;
                }
            }
        }
        std::vector<std::string> fields, inits;
        for (const auto& [a, b] : members) {
            if (const auto table = pointers.find(a); table != pointers.end()) {
                fields.push_back("  uint1 __based(__segname(\"_CODE\")) *" + member_at[a] + "[" + std::to_string(table->second.size()) + "];");
                std::vector<std::string> ps;
                for (const std::int64_t v : table->second) ps.push_back(place(v));
                inits.push_back("  {" + cd_rows(ps, 3) + "}");
            } else {
                fields.push_back("  uint1 " + member_at[a] + "[" + std::to_string(b - a) + "];");
                const ByteView chunk = ByteView(image).subspan(static_cast<std::size_t>(std::min<std::int64_t>(a, image.size())),
                                                               static_cast<std::size_t>(std::max<std::int64_t>(0, std::min<std::int64_t>(b, image.size()) - a)));
                if (const auto literal = string_literal(chunk)) {
                    inits.push_back("  " + *literal);
                } else {
                    std::vector<std::string> hx;
                    for (const std::uint8_t c : chunk) hx.push_back(std::format("0x{:02x}", c));
                    inits.push_back("  {" + cd_rows(hx, 16) + "}");
                }
            }
        }
        std::vector<std::string> wh;
        for (const auto& [run, lo] : spans) wh.push_back(std::format("{:04x}-{:04x}", lo, run.second));
        out.decls.push_back("/* the original's bytes of code segment " + std::to_string(sg) + " (" + text::join(wh, ", ") + ") that its C reads through CS */");
        out.decls.push_back("_Packed struct " + struct_name + " {\n" + text::join(fields, "\n") + "\n};");
        out.decls.push_back("extern _Packed struct " + struct_name + " __based(__segname(\"_CODE\")) " + struct_name + ";");
        out.defs[sg] = "_Packed struct " + struct_name + " __based(__segname(\"_CODE\")) " + struct_name + " = {\n" + text::join(inits, ",\n") + "\n};\n";
        for (const Per& p : pers) {
            Json written = Json::array();
            for (const auto& [k, place_of_k] : p.places) {
                const std::int64_t v = place_of_k.second;
                if (ok_place.contains(v)) written.push_back(Json::array({k, "(uint2)" + place(v)}));
            }
            if (written.empty()) continue;
            const std::string plan = Json{{"code_places", Json{{"$d", written}}}}.dump();
            const auto have = std::find_if(out.plan.begin(), out.plan.end(), [&](const auto& pl) { return pl.first == p.name; });
            if (have != out.plan.end()) have->second = plan;
            else out.plan.emplace_back(p.name, plan);
        }
        report[sg].placed.assign(ok_place.begin(), ok_place.end());
        std::int64_t named = 0;
        for (const auto& got : each_read[sg]) {
            const bool all_placed = !got.empty() && std::all_of(got.begin(), got.end(), [&](std::int64_t v) { return ok_place.contains(v); });
            if (all_placed) ++named;
        }
        report[sg].named = named;
    }
    out.report = std::move(report);
    return out;
}

std::map<std::int64_t, std::vector<std::pair<std::int64_t, std::int64_t>>> selector_sites(const std::filesystem::path& module) {
    const formats::ne::Module ne = formats::ne::Module::open_file(module);
    const std::vector<formats::ne::Entry> entries = ne.entries();
    std::map<std::int64_t, std::vector<std::pair<std::int64_t, std::int64_t>>> out;
    for (const formats::ne::Segment& s : ne.segments()) {
        if (s.file_offset == 0) continue;
        const ByteView raw = io::checked_range(ne.data(), s.file_offset, s.length);
        for (const formats::ne::Relocation& r : ne.relocations(s)) {
            const auto* internal_ref = std::get_if<formats::ne::InternalRef>(&r.target);
            if (internal_ref == nullptr || (r.source != 2 && r.source != 3)) continue;
            std::optional<std::int64_t> target = internal_ref->segment;
            if (target && *target == 0xFF) {   // a movable segment, through its entry
                std::optional<std::int64_t> seg;
                const std::int64_t e = internal_ref->entry;
                for (const formats::ne::Entry& entry : entries)
                    if (entry.ordinal == e && 0 < e) seg = entry.segment;
                target = seg;
            }
            if (!target || *target == 0) continue;
            std::int64_t at = r.offset;
            std::set<std::int64_t> seen;
            while (at < static_cast<std::int64_t>(raw.size()) - 1 && !seen.contains(at)) {
                seen.insert(at);
                // (the target of a far call or jump into the segment is not its selector as a value: with one in the function, a plain
                // `push 1000h` became segment 1's)
                const std::int64_t opcode = at - (r.source == 3 ? 1 : 3);
                if (!(opcode >= 0 && (raw[static_cast<std::size_t>(opcode)] == 0x9A || raw[static_cast<std::size_t>(opcode)] == 0xEA)))
                    out[s.index].emplace_back(at + (r.source == 3 ? 2 : 0), *target);
                if (r.additive()) break;
                const std::int64_t next = raw[static_cast<std::size_t>(at)] | (raw[static_cast<std::size_t>(at) + 1] << 8);
                if (next == 0xFFFF) break;
                at = next;
            }
        }
    }
    return out;
}

}  // namespace cb::decompile::c_text::internal
