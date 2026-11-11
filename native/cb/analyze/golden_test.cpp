// The analyses against what the engine they replace (cbanalyze.dll's image.cc, call_graph.cc, strings.cc and
// resources.cc) answered on real modules of the Windows 95 corpus: fifteen PE, NE and LE modules, small ones with
// strings, format calls, resource loads, jump tables and recursion in them.
//
// testdata/<module>.image is the module as the DLL was handed it (its spans, function starts, imports, relocations),
// <module>.names the names it was given after discovery, and <module>.answers.json what it answered, in the order
// asked: discovery (with a digest of every decoded function), the call graph, the strings table, uses, stack
// strings and formats, and the resource uses. Each test builds the Image from the first two files and runs the
// C++ analyses on it. The files were made by state/work/cpp-refactor/analyze/gen.py from the live Python and DLL.
//
// What is compared is what the answer means: lists the engine built in a hash order that no one chose (the order of
// a function's callees, the far addresses one instruction holds) are compared as sets, and the topological order of
// the call graph is checked to be one (callees before callers) rather than to be the DLL's.
#include <algorithm>
#include <format>
#include <map>
#include <memory>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/native.hpp"
#include "cb/analyze/resources.hpp"
#include "cb/analyze/strings.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;
using cb::Json;

constexpr const char* kModules[] = {"lz32.dll",   "mosaf.dll",   "password.cpl", "dpwsock.dll", "sccview.dll",
                                    "vswks.dll",  "iosclass.dll", "infrared.dll", "cheyprop.dll", "pa3dmxd.drv",
                                    "mcicda.drv", "pointer.exe", "drwatson.exe", "emm386.exe",   "bigmem.drv"};

// A check whose failure says which module and what.
void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok) cb::testing::record_failure(__FILE__, __LINE__, what);
}

// ---- reading the oracle's input --------------------------------------------------------------------------------

ImageSpec read_image(const std::string& module) {
    const auto blob = cb::fs::read_file(cb::testing::data_path("analyze/testdata/" + module + ".image"));
    const auto names = cb::fs::read_file(cb::testing::data_path("analyze/testdata/" + module + ".names"));
    cb::io::ByteReader r(blob, "image blob");
    ImageSpec spec;
    const char kind = static_cast<char>(r.u8());
    spec.kind = kind == 'p' ? ImageKind::Pe : kind == 'n' ? ImageKind::Ne : ImageKind::Le;
    spec.bits = r.u8();
    spec.flat = r.u8() != 0;
    spec.has_pointers = r.u8() != 0;
    const std::uint32_t n_spans = r.u32();
    for (std::uint32_t i = 0; i < n_spans; ++i) {
        Span s;
        s.va = r.i64();
        const std::uint32_t length = r.u32();
        s.executable = r.u8() != 0;
        s.writable = r.u8() != 0;
        s.initialised = r.i64();
        const std::uint32_t name_length = r.u32();
        s.name = std::string(cb::as_text(r.bytes(name_length)));
        s.data = r.bytes_copy(length);
        spec.spans.push_back(std::move(s));
    }
    const std::uint32_t n_functions = r.u32();
    for (std::uint32_t i = 0; i < n_functions; ++i) {
        const Va start = r.i64();
        const Va limit = r.i64();
        spec.functions.emplace_back(start, limit);
    }
    const std::uint32_t n_imports = r.u32();
    for (std::uint32_t i = 0; i < n_imports; ++i) spec.imports[r.i64()] = "";
    const std::uint32_t n_pointers = r.u32();
    for (std::uint32_t i = 0; i < n_pointers; ++i) {
        const Va site = r.i64();
        Pointer p;
        p.kind = static_cast<PointerKind>(r.u8());
        const bool has_target = r.u8() != 0;
        const Va target = r.i64();
        if (has_target) p.target = target;
        spec.pointers.emplace_back(site, p);
    }
    spec.data_segment = r.i64();
    const std::uint32_t n_roots = r.u32();
    for (std::uint32_t i = 0; i < n_roots; ++i) spec.roots.push_back(r.i64());
    // the names: the functions', then the imports'
    cb::io::ByteReader nr(names, "names blob");
    for (auto* table : {&spec.function_names, &spec.imports}) {
        const std::uint32_t n = nr.u32();
        for (std::uint32_t i = 0; i < n; ++i) {
            const Va va = nr.i64();
            const std::uint32_t length = nr.u32();
            (*table)[va] = std::string(cb::as_text(nr.bytes(length)));
        }
    }
    return spec;
}

// question -> answer
std::map<std::string, Json> read_answers(const std::string& module) {
    const Json doc = cb::json::load(cb::testing::data_path("analyze/testdata/" + module + ".answers.json"));
    std::map<std::string, Json> out;
    for (const Json& a : doc.at("answers")) out[a.at(0).get<std::string>()] = a.at(2);
    return out;
}

std::int64_t as_int(const Json& j) { return j.get<std::int64_t>(); }
std::optional<std::int64_t> as_optional(const Json& j) {
    return j.is_null() ? std::nullopt : std::optional<std::int64_t>(as_int(j));
}

// ---- a decoded function as text, the way gen.py wrote it ---------------------------------------------------------

std::string dash(const std::string& s) { return s.empty() ? "-" : s; }

std::string canonical(const Function& fn) {
    std::string out = std::format("F {} {} {}\n", fn.start, fn.limit, fn.end);
    for (const Instruction& ins : fn.insns) {
        std::string ops;
        for (const Operand& o : ins.operands) {
            if (!ops.empty()) ops += ';';
            if (o.kind == Operand::Kind::Register) ops += std::format("r:{}:{}", o.reg, o.size);
            else if (o.kind == Operand::Kind::Immediate) ops += std::format("i:{}:{}", o.value, o.size);
            else ops += std::format("m:{}|{}|{}|{}|{}|{}", dash(o.base), dash(o.index), o.scale, o.value, dash(o.seg), o.size);
        }
        auto regs = [](const std::set<std::string>& s) {
            std::string r;
            for (const std::string& x : s) r += (r.empty() ? "" : ",") + x;
            return r;
        };
        out += std::format("{} {} {}|{}|{}|{}|{}|{}|{}\n", ins.addr, ins.size, ins.mnemonic, ins.text, ops, regs(ins.reads),
                           regs(ins.writes), static_cast<int>(ins.flow), ins.target ? std::to_string(*ins.target) : "-");
    }
    for (const JumpTable& t : fn.tables) {
        std::string targets, bytes;
        for (Va v : t.targets) targets += (targets.empty() ? "" : ",") + std::to_string(v);
        if (t.index_bytes)
            for (std::uint8_t b : *t.index_bytes) bytes += (bytes.empty() ? "" : ",") + std::to_string(b);
        out += std::format("T {} {} {} {}|{}|{}|{}|{}\n", t.site, t.table, t.low, t.selector,
                           t.default_target ? std::to_string(*t.default_target) : "-", t.bounded ? 1 : 0, targets,
                           t.index_bytes ? bytes : "-");
    }
    return out;
}

// ---- the four comparisons ----------------------------------------------------------------------------------------

void check_discovery(const std::string& m, Image& image, const Json& answer) {
    std::vector<Va> expected_discovered;
    for (const Json& v : answer.at("discovered")) expected_discovered.push_back(as_int(v));
    const std::vector<Va> discovered = image.discover();
    expect(discovered == expected_discovered, std::format("{}: discovered starts differ ({} vs {})", m, discovered.size(), expected_discovered.size()));
    std::vector<std::pair<Va, Va>> expected_functions;
    for (const Json& f : answer.at("functions")) expected_functions.emplace_back(as_int(f.at(0)), as_int(f.at(1)));
    std::vector<std::pair<Va, Va>> functions;
    for (Va f : image.functions()) functions.emplace_back(f, image.function_limit(f));
    expect(functions == expected_functions, std::format("{}: function starts and limits differ", m));
    const Json& decoded = answer.at("decoded");
    expect(decoded.size() == image.functions().size(), std::format("{}: {} decoded functions, wanted {}", m, image.functions().size(), decoded.size()));
    for (const Json& d : decoded) {
        const Va start = as_int(d.at(0));
        const Function& fn = image.function(start);
        const bool same = fn.limit == as_int(d.at(1)) && fn.end == as_int(d.at(2)) &&
                          static_cast<std::int64_t>(fn.insns.size()) == as_int(d.at(3)) &&
                          static_cast<std::int64_t>(fn.tables.size()) == as_int(d.at(4)) &&
                          cb::crypto::crc32(cb::as_bytes(canonical(fn))) == static_cast<std::uint32_t>(as_int(d.at(5)));
        expect(same, std::format("{}: function {:x} decodes differently (limit/end/insns/tables/crc)", m, start));
    }
}

std::string how_name(call_graph::Access a) { return std::string(call_graph::name_of(a)); }

void check_call_graph(const std::string& m, Image& image, const call_graph::Graph& g, const Json& answer) {
    // calls, in order
    std::size_t listed = 0;
    for (const Json& row : answer.at("calls")) {
        const Va f = as_int(row.at(0));
        ++listed;
        const auto it = g.calls.find(f);
        if (it == g.calls.end()) {
            expect(false, std::format("{}: no calls entry for {:x}", m, f));
            continue;
        }
        std::vector<std::tuple<Va, std::string, std::optional<Va>>> want, got;
        for (const Json& c : row.at(1)) want.emplace_back(as_int(c.at(0)), c.at(1).get<std::string>(), as_optional(c.at(2)));
        for (const call_graph::Call& c : it->second) got.emplace_back(c.site, std::string(call_graph::name_of(c.kind)), c.destination);
        expect(want == got, std::format("{}: calls of {:x} differ", m, f));
    }
    expect(listed == g.calls.size(), std::format("{}: {} functions in calls, wanted {}", m, g.calls.size(), listed));
    // callers
    std::map<Va, std::vector<Va>> callers;
    for (const Json& row : answer.at("callers"))
        for (const Json& c : row.at(1)) callers[as_int(row.at(0))].push_back(as_int(c));
    expect(callers == g.callers, std::format("{}: callers differ", m));
    // users, by import name
    std::map<std::string, std::set<Va>> users;
    for (const Json& row : answer.at("users")) {
        const Va address = as_int(row.at(0));
        for (const Json& f : row.at(1)) users[image.import_name(address)].insert(as_int(f));
    }
    std::map<std::string, std::vector<Va>> want_users;
    for (const auto& [name, fs] : users) want_users[name] = {fs.begin(), fs.end()};
    expect(want_users == g.users, std::format("{}: import users differ", m));
    // references: the same set (their order within one 16-bit instruction is not meaningful)
    using Ref = std::tuple<Va, Va, Va, std::string>;
    std::multiset<Ref> want_refs, got_refs;
    for (const Json& r : answer.at("refs"))
        want_refs.emplace(as_int(r.at(0)), as_int(r.at(1)), as_int(r.at(2)), r.at(3).get<std::string>());
    for (const call_graph::Reference& r : g.refs) got_refs.emplace(r.function, r.instruction, r.address, how_name(r.how));
    expect(want_refs == got_refs, std::format("{}: references differ ({} vs {})", m, got_refs.size(), want_refs.size()));
    // where function addresses are taken
    using Take = std::tuple<bool, std::optional<Va>, Va>;
    std::map<Va, std::multiset<Take>> want_taken, got_taken;
    for (const Json& row : answer.at("taken"))
        for (const Json& t : row.at(1))
            want_taken[as_int(row.at(0))].emplace(t.at(0).get<std::string>() == "code", as_optional(t.at(1)), as_int(t.at(2)));
    for (const auto& [f, sites] : g.taken)
        for (const call_graph::Taken& t : sites) got_taken[f].emplace(t.in_code, t.function, t.at);
    expect(want_taken == got_taken, std::format("{}: address-taken sites differ", m));
    // reach, unreferenced
    std::set<Va> reachable, with_tables;
    for (const Json& v : answer.at("reachable")) reachable.insert(as_int(v));
    for (const Json& v : answer.at("reachable_with_tables")) with_tables.insert(as_int(v));
    expect(reachable == g.reachable, std::format("{}: reachable differs", m));
    expect(with_tables == g.reachable_with_tables, std::format("{}: reachable with tables differs", m));
    std::vector<Va> unreferenced;
    for (const Json& v : answer.at("unreferenced")) unreferenced.push_back(as_int(v));
    expect(unreferenced == g.unreferenced, std::format("{}: unreferenced differs", m));
    // recursive groups: the same groups
    std::set<std::vector<Va>> want_cycles, got_cycles(g.cycles.begin(), g.cycles.end());
    for (const Json& c : answer.at("cycles")) {
        std::vector<Va> group;
        for (const Json& v : c) group.push_back(as_int(v));
        std::sort(group.begin(), group.end());
        want_cycles.insert(group);
    }
    expect(want_cycles == got_cycles, std::format("{}: recursive groups differ", m));
    // the order: every function once, callees before callers except within a recursive group
    std::map<Va, std::size_t> position;
    for (std::size_t i = 0; i < g.order.size(); ++i) position[g.order[i]] = i;
    expect(g.order.size() == image.functions().size() && position.size() == g.order.size(), std::format("{}: order is not a permutation of the functions", m));
    std::map<Va, std::size_t> group_of;
    for (std::size_t i = 0; i < g.cycles.size(); ++i)
        for (Va f : g.cycles[i]) group_of[f] = i + 1;
    std::size_t out_of_order = 0;
    for (const auto& [f, calls] : g.calls)
        for (const call_graph::Call& c : calls) {
            if (!c.destination || (c.kind != call_graph::CallKind::Call && c.kind != call_graph::CallKind::Tail)) continue;
            const Va callee = *c.destination;
            if (callee == f || (group_of.contains(f) && group_of[f] != 0 && group_of[f] == group_of[callee])) continue;
            if (position.contains(callee) && position[callee] > position[f]) ++out_of_order;
        }
    expect(out_of_order == 0, std::format("{}: {} calls go to a function later in the order", m, out_of_order));
}

void check_strings(const std::string& m, Image& image, const call_graph::Graph& g, const std::map<std::string, Json>& answers,
                   strings::Table& table_out, strings::Formats& formats_out) {
    // the table, before the uses mark anything
    strings::Table table = strings::build_table(image, g);
    std::map<Va, std::tuple<std::string, std::string, bool>> want_table, got_table;
    for (const Json& r : answers.at("strings.table"))
        want_table[as_int(r.at(0))] = {r.at(1).get<std::string>(), r.at(2).get<std::string>(), r.at(3).get<bool>()};
    for (const auto& [va, e] : table) got_table[va] = {e.text, e.encoding == strings::Encoding::Wide ? "W" : "A", e.referenced};
    expect(want_table == got_table, std::format("{}: strings table differs ({} vs {})", m, got_table.size(), want_table.size()));

    const auto uses = strings::find_uses(image, g, table);
    std::vector<std::tuple<Va, Va, Va, std::string>> want_uses, got_uses;
    for (const Json& row : answers.at("strings.uses"))
        for (const Json& u : row.at(1)) want_uses.emplace_back(as_int(row.at(0)), as_int(u.at(0)), as_int(u.at(1)), u.at(2).get<std::string>());
    for (const strings::FunctionUses& f : uses)
        for (const strings::Use& u : f.uses) got_uses.emplace_back(f.function, u.site, u.address, u.text);
    expect(want_uses == got_uses, std::format("{}: string uses differ ({} vs {})", m, got_uses.size(), want_uses.size()));

    const auto stack = strings::stack_strings(image);
    std::vector<std::tuple<Va, std::string, std::string, bool>> want_stack, got_stack;
    for (const Json& r : answers.at("strings.stack"))
        want_stack.emplace_back(as_int(r.at(0)), r.at(1).get<std::string>(), r.at(2).get<std::string>(), r.at(3).get<bool>());
    for (const strings::StackString& s : stack) got_stack.emplace_back(s.function, s.where, s.text, s.wide);
    expect(want_stack == got_stack, std::format("{}: stack strings differ", m));

    const strings::Formats formats = strings::find_formats(image, g, table);
    const Json& fa = answers.at("strings.formats");
    using Row = std::tuple<Va, Va, std::string, std::string, bool, bool>;
    std::vector<Row> want_rows, got_rows;
    for (const Json& r : fa.at("rows"))
        want_rows.emplace_back(as_int(r.at(0)), as_int(r.at(1)), r.at(2).get<std::string>(), r.at(3).get<std::string>(), r.at(4).get<bool>(), r.at(5).get<bool>());
    for (const strings::FormatCall& c : formats.calls) got_rows.emplace_back(c.function, c.site, c.api, c.format, c.wide, c.lists_arguments);
    expect(want_rows == got_rows, std::format("{}: format calls differ ({} vs {})", m, got_rows.size(), want_rows.size()));
    expect(formats.sites == as_int(fa.at("sites")), std::format("{}: format call sites {} vs {}", m, formats.sites, as_int(fa.at("sites"))));
    std::map<Va, int> want_own;
    for (const Json& o : fa.at("own")) want_own[as_int(o.at(0))] = static_cast<int>(as_int(o.at(1)));
    expect(want_own == formats.own, std::format("{}: the module's own printf-like functions differ", m));
    table_out = std::move(table);
    formats_out = formats;
}

void check_resources(const std::string& m, Image& image, const call_graph::Graph& g, const strings::Table& table, const Json& answer) {
    const resources::Uses uses = resources::find_uses(image, g, table);
    auto id_of = [](const Json& j) -> resources::ResourceId {
        if (j.is_string()) return j.get<std::string>();
        return as_int(j);
    };
    using Load = std::tuple<Va, Va, std::string, std::optional<std::string>, resources::ResourceId>;
    std::vector<Load> want, got;
    for (const Json& r : answer.at("loads"))
        want.emplace_back(as_int(r.at(0)), as_int(r.at(1)), r.at(2).get<std::string>(),
                          r.at(3).is_null() ? std::nullopt : std::optional<std::string>(r.at(3).get<std::string>()), id_of(r.at(4)));
    for (const resources::Load& l : uses.loads) got.emplace_back(l.function, l.site, l.call, l.type, l.id);
    expect(want == got, std::format("{}: resource loads differ ({} vs {})", m, got.size(), want.size()));
    std::vector<std::tuple<Va, Va, std::string, std::int64_t>> want_controls, got_controls;
    for (const Json& r : answer.at("controls"))
        want_controls.emplace_back(as_int(r.at(0)), as_int(r.at(1)), r.at(2).get<std::string>(), as_int(r.at(3)));
    for (const resources::ControlUse& c : uses.controls) got_controls.emplace_back(c.function, c.site, c.call, c.control);
    expect(want_controls == got_controls, std::format("{}: dialog control uses differ ({} vs {})", m, got_controls.size(), want_controls.size()));
}

void run_module(const std::string& m) {
    Image image(read_image(m));
    const auto answers = read_answers(m);
    check_discovery(m, image, answers.at("image.discover"));
    // (the names the DLL had after discovery are in <module>.names and already in the spec)
    const call_graph::Graph graph = call_graph::build(image);
    check_call_graph(m, image, graph, answers.at("call_graph.build"));
    strings::Table table;
    strings::Formats formats;
    check_strings(m, image, graph, answers, table, formats);
    check_resources(m, image, graph, table, answers.at("resources.uses"));
    // every report builds without surprises on real input
    const call_graph::Report cg = call_graph::analyse(image, graph);
    expect(cg.counts.functions == static_cast<std::int64_t>(image.functions().size()), m + ": call graph report");
    const strings::Report sr = strings::analyse(image, graph);
    expect(sr.counts.strings == static_cast<std::int64_t>(answers.at("strings.table").size()), m + ": strings report");
}

// One test per module: analyze.golden.<module>
const bool kRegistered = [] {
    static std::vector<std::unique_ptr<cb::testing::Registrar>> registrars;
    for (const char* m : kModules)
        registrars.push_back(std::make_unique<cb::testing::Registrar>(std::format("analyze.golden.{}", m).c_str(),
                                                                      [name = std::string(m)] { run_module(name); }));
    return true;
}();

}  // namespace
