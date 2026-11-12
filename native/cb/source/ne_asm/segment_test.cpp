// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/ne_asm/segment. The goldens (testdata/segment.golden.txt, real_lzexpand.golden.txt) are the output of the
// Python module on the same inputs (testdata/gen_segment_golden.py); each was read through by hand. Three
// scenarios feed Seg a listing of their own (as objdump would print it; the listing is canned, objdump is not needed) over
// segments of the real module LZEXPAND.DLL: a window procedure as a compare chain, as a jump table, and a module whose relocation
// records are rewritten to reach the far and internal kinds of call. The real objdump is used where it is installed.
//
// DIVERGES from the Python module, by design: the generated C skeleton says "ne-asm segment --cskel" where the Python said
// "trace.py --cskel"; the prologue scan the Python described (`push bp; mov bp,sp` starts a function) never ran there, because the
// Python's test spelled the instruction with one space where objdump writes three, and stays off here (kScanPushBpPrologue; the
// golden is the Python's, so a start of that kind is not in it); the API surface of a trace is sorted by module and ordinal with a
// name ordering for an import by name (the Python raised a ValueError on one); a segment that does not exist is a UsageError (the
// Python indexed a list); a segment with no data in the file has no code (the Python took the start of the file for it).
#include "cb/source/ne_asm/segment.hpp"

#include <chrono>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace sg = cb::source::ne_asm::segment;
namespace fx = cb::source::ne_asm::fixups;
namespace ne = cb::formats::ne;
using cb::Bytes;

namespace {

std::filesystem::path testdata(std::string_view name) { return cb::testing::data_path("source/ne_asm/testdata/" + std::string(name)); }

Bytes lz_bytes() { return cb::fs::read_file(cb::testing::data_path("formats/testdata/lzexpand.dll")); }

bool has(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

sg::SharedOrdinals test_ordinals() { return std::make_shared<const sg::Ordinals>(sg::load_ordinals(testdata("ORDINALS.md"))); }

std::string trimmed(std::string s) {
    while (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

// The first two relocation records of segment 1 rewritten to point inside the module: record 0 at segment 2 offset 0x120 (a far
// reference), record 1 at segment 1 offset 0x90 (an internal one).
Bytes patched_lz() {
    Bytes data = lz_bytes();
    const ne::Module module = ne::Module::open(Bytes(data));
    const ne::Segment segment = module.segments().front();
    const std::size_t first = segment.file_offset + segment.length + 2;
    const std::pair<int, int> targets[] = {{2, 0x0120}, {1, 0x0090}};
    for (std::size_t k = 0; k < 2; ++k) {
        const std::size_t record = first + 8 * k;
        data[record + 1] = 0;   // an internal reference to a fixed segment
        data[record + 4] = static_cast<std::uint8_t>(targets[k].first), data[record + 5] = 0;
        data[record + 6] = static_cast<std::uint8_t>(targets[k].second), data[record + 7] = static_cast<std::uint8_t>(targets[k].second >> 8);
    }
    return data;
}

// A canned objdump: one line per (offset, assembly) row of a rows file, written the way objdump writes them.
struct Rows {
    std::vector<std::pair<std::uint32_t, std::string>> rows;
    fx::Objdump objdump() const {
        const auto copy = rows;
        return [copy](cb::ByteView, std::uint32_t) {
            std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
            for (const auto& [offset, text] : copy) out += std::format("{:4x}:\t00 00\t{}\r\n", offset, text);
            return out;
        };
    }
};

Rows rows_file(std::string_view name) {
    Rows out;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(testdata(name)), "\r\n", "\n"));   // (a checkout may have changed the line ends)
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        const std::size_t tab = line.find('\t');
        out.rows.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
    }
    return out;
}

// The golden file: "@@@ title" lines and the text under each.
std::map<std::string, std::string> golden(std::string_view name) {
    std::map<std::string, std::string> out;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(testdata(name)), "\r\n", "\n"));   // (a checkout may have changed the line ends)
    std::string title, body;
    bool open = false;
    const auto close = [&] {
        if (open) out[title] = trimmed(body);
    };
    for (std::string line; std::getline(in, line);) {
        if (line.starts_with("@@@ ")) {
            close();
            title = line.substr(4), body.clear(), open = true;
        } else {
            body += line + "\n";
        }
    }
    close();
    for (auto& [key, text] : out) text = cb::text::replace_all(text, "trace.py --cskel", "ne-asm segment --cskel");   // DIVERGES
    return out;
}

std::string hex(std::uint32_t v) { return std::format("{:x}", v); }

// What the generator's `facts` section printed: Python's lists of tuples.
std::string py_list(const std::vector<std::string>& items) { return "[" + cb::text::join(items, ", ") + "]"; }
std::string py_str(const std::string& s) { return "'" + s + "'"; }
const char* kind_name(sg::CallKind k) { return k == sg::CallKind::System ? "sys" : k == sg::CallKind::Internal ? "int" : "far"; }

std::string facts_of(const sg::Seg& seg, std::uint32_t start) {
    const auto dispatch = seg.dispatch(start);
    std::vector<std::string> pairs, args, calls;
    std::vector<std::uint32_t> handlers;
    for (const auto& d : dispatch) {
        pairs.push_back(std::format("({}, {})", d.message, d.handler));
        handlers.push_back(d.handler);
    }
    for (const auto a : seg.args_used(start)) args.push_back(std::to_string(a));
    for (const auto& c : seg.calls_in(start)) calls.push_back(std::format("({}, {})", py_str(kind_name(c.kind)), py_str(sg::describe_call(seg, c))));
    std::vector<std::uint32_t> sorted = handlers;
    std::sort(sorted.begin(), sorted.end());
    const auto thunk = seg.is_thunk(start);
    std::string out = "dispatch " + py_list(pairs) + "\nargs " + py_list(args) + "\nthunk " + (thunk ? std::to_string(*thunk) : "None") + "\ncalls " + py_list(calls);
    std::vector<sg::Dispatch> ordered = dispatch;
    std::sort(ordered.begin(), ordered.end(), [](const auto& a, const auto& b) { return std::tie(a.message, a.handler) < std::tie(b.message, b.handler); });
    for (const auto& d : ordered) {
        std::vector<std::string> shown;
        for (const std::string& s : seg.handler_calls(start, d.handler, sorted)) shown.push_back(py_str(s));
        out += "\nhandler " + hex(d.handler) + ": " + py_list(shown);
    }
    return out;
}

// Python's repr of what resolve_call gave.
std::string resolved_text(const std::optional<sg::Call>& call) {
    if (!call) return "(None, None)";
    switch (call->kind) {
        case sg::CallKind::System: return "('sys', (" + py_str(call->label) + ", " + (call->name ? py_str(*call->name) : "None") + "))";
        case sg::CallKind::Internal: return "('int', " + std::to_string(call->target) + ")";
        case sg::CallKind::Far: return "('far', " + py_str(call->label) + ")";
    }
    return {};
}

std::string join_lines(const std::vector<std::string>& lines) { return cb::text::join(lines, "\n"); }

}  // namespace

CB_TEST("source.ne_asm.segment.ordinals_file", [] {
    const sg::Ordinals names = sg::load_ordinals(testdata("ORDINALS.md"));
    const auto expected = golden("segment.golden.txt").at("ordinals");
    std::vector<std::string> shown;
    for (const auto& [key, name] : names) shown.push_back(key + "=" + name);
    std::sort(shown.begin(), shown.end());
    CB_CHECK_EQ(join_lines(shown), expected);
    CB_CHECK_EQ(names.size(), 13u);   // the header row, the separator and the line that is not a row give none
    // the rows the Python reads and the ones it does not (each line checked against it)
    const std::string odd =
        "| KERNEL.5 | `GetVersion` | x |\r\n|---|---|---|\r\n| KERNEL.007 | Get Version | note |\r\n| A.1 |   | x |\r\n| A.2 | `Quoted`name | x |\r\n"
        "| A.3 | `Tick` | x |\r\n| 12abc.4 | NoLeadingDigit | x |\r\n| B.9 | first | x |\r\n| B.9 | second | x |\r\n| B_1.10 |    padded name   | x |\r\n"
        "text | KERNEL.20 | InMiddle | y\r\n| KERNEL.30 | NoClosingBar\r\n| KERNEL.40 | a`b | x |\r\n|KERNEL.50|Tight|x|\r\n";
    const sg::Ordinals parsed = sg::parse_ordinals(odd);
    const std::map<std::string, std::string> sorted(parsed.begin(), parsed.end());
    const std::map<std::string, std::string> want = {{"A.1", ""},       {"A.3", "Tick"},          {"B.9", "second"},      {"B_1.10", "padded name"},
                                                     {"KERNEL.20", "InMiddle"}, {"KERNEL.5", "GetVersion"}, {"KERNEL.50", "Tight"}, {"KERNEL.7", "Get Version"}};
    CB_CHECK(sorted == want);
    CB_CHECK(sg::load_ordinals(testdata("no-such-ORDINALS.md")).empty());   // no file: no names
    CB_CHECK(sg::parse_ordinals("").empty());
});

CB_TEST("source.ne_asm.segment.name_tables", [] {
    CB_CHECK_EQ(std::string(sg::window_message_name(0x0F)), "WM_PAINT");
    CB_CHECK_EQ(std::string(sg::window_message_name(0x0110)), "WM_INITDIALOG");
    CB_CHECK_EQ(std::string(sg::window_message_name(0x0304)), "WM_UNDO");
    CB_CHECK(sg::window_message_name(0x04) == nullptr && sg::window_message_name(0x0305) == nullptr && sg::window_message_name(0) == nullptr);
    CB_CHECK_EQ(std::string(sg::dos_function_name(0x3D)), "OPEN");
    CB_CHECK_EQ(std::string(sg::dos_function_name(0x57)), "FILE_TIME");
    CB_CHECK(sg::dos_function_name(0x58) == nullptr && sg::dos_function_name(0x01) == nullptr);
});

CB_TEST("source.ne_asm.segment.scenarios_match_the_python_module", [] {
    const auto want = golden("segment.golden.txt");
    const sg::SharedOrdinals ordinals = test_ordinals();
    struct Scenario {
        std::string name, rows;
        std::size_t segment;
        bool patched;
    };
    const Scenario scenarios[] = {{"a", "scenario_a.rows", 1, false}, {"b", "scenario_b.rows", 2, false}, {"c", "scenario_c.rows", 1, true}};
    for (const Scenario& s : scenarios) {
        const ne::Module module = ne::Module::open(s.patched ? patched_lz() : lz_bytes());
        const Rows rows = rows_file(s.rows);
        const fx::Objdump objdump = rows.objdump();
        const sg::Seg seg(module, s.segment, ordinals, {}, objdump);
        const auto check = [&](const std::string& title, const std::string& got) {
            const auto found = want.find(s.name + " " + title);
            CB_CHECK(found != want.end());
            if (found != want.end()) CB_CHECK_EQ(trimmed(got), found->second);
        };

        std::vector<std::string> starts, functions, exports;
        for (const std::uint32_t start : seg.starts()) starts.push_back(hex(start));
        for (const auto& [start, f] : seg.functions()) functions.push_back(std::format("{:x}-{:x} {}", f.start, f.end, f.name));
        for (const auto& [offset, name] : seg.export_names()) exports.push_back(hex(offset) + " " + name);
        check("starts", cb::text::join(starts, " "));
        check("funcs", join_lines(functions));
        check("exports", join_lines(exports));
        for (const auto& [start, f] : seg.functions()) {
            check("function " + hex(start), sg::function_report(seg, start, true));
            check("facts " + hex(start), facts_of(seg, start));
        }
        std::vector<std::string> graph;
        for (const std::uint32_t start : sg::call_graph(seg, 0)) graph.push_back(hex(start));
        check("call graph 0", cb::text::join(graph, " "));
        check("cskel", sg::skeleton(module, ordinals, s.segment, 0, objdump));
        check("trace", sg::trace_report(module, ordinals, s.segment, 0, objdump));
        check("wndprocs", sg::wndprocs_report(module, ordinals, s.segment, objdump));
        std::vector<std::string> resolved, annotated;
        for (const auto& [offset, text] : rows.rows) {
            if (text.starts_with("call") || text.starts_with("jmp")) resolved.push_back(std::format("{:x} {} -> {}", offset, text, resolved_text(seg.resolve_call(offset, text))));
            annotated.push_back(sg::annotate_row(seg, sg::Row{offset, text}));
        }
        check("resolve", join_lines(resolved));
        check("annotate", join_lines(annotated));
        std::vector<std::string> words;
        for (const std::uint32_t at : {0u, 0x20u, 0x1c6u, 0x1c7u, 0x2000u}) {
            const auto w = seg.word_at(at);
            words.push_back(w ? std::to_string(*w) : "None");
        }
        check("word_at", cb::text::join(words, " "));
    }
});

CB_TEST("source.ne_asm.segment.function_discovery_rules", [] {
    const ne::Module module = ne::Module::open(lz_bytes());
    const sg::SharedOrdinals ordinals = test_ordinals();
    const auto starts_of = [&](const std::string& listing, std::size_t segment = 1) {
        Rows rows;
        std::istringstream in(listing);
        for (std::string line; std::getline(in, line);) {
            const std::size_t tab = line.find('\t');
            rows.rows.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
        }
        const sg::Seg seg(module, segment, ordinals, {}, rows.objdump());
        return std::make_pair(std::vector<std::uint32_t>(seg.starts().begin(), seg.starts().end()), seg.functions());
    };
    // (1) the entry table: segment 1 holds three exported entries; the first is at 0
    CB_CHECK(starts_of("0\tnop\n4\tnop\n").first == std::vector<std::uint32_t>{0});
    // (3) a near call's target, only where an instruction starts
    CB_CHECK(starts_of("0\tcall   0x10\n3\tnop\n10\tnop\n").first == (std::vector<std::uint32_t>{0, 0x10}));
    CB_CHECK(starts_of("0\tcall   0x11\n3\tnop\n10\tnop\n").first == std::vector<std::uint32_t>{0});
    CB_CHECK(starts_of("0\tcall   0x10,0x1\n3\tnop\n10\tnop\n").first == std::vector<std::uint32_t>{0});   // trailing text: not a plain call
    // (5) `enter` starts a function
    CB_CHECK(starts_of("0\tnop\n8\tenter  0x4,0x0\n").first == (std::vector<std::uint32_t>{0, 8}));
    // (4) the prologue scan stays off: `push   bp; mov    bp,sp` is no start by itself
    CB_CHECK(starts_of("0\tnop\n8\tpush   bp\n9\tmov    bp,sp\n").first == std::vector<std::uint32_t>{0});
    // (6) a routine two short thunks tail-jump to; one thunk is not enough, a jump far from any start does not count
    CB_CHECK(starts_of("0\tmov    cl,0x1\n2\tjmp    0x40\n5\tnop\n6\tmov    cl,0x2\n8\tjmp    0x40\n40\tnop\n").first == (std::vector<std::uint32_t>{0, 0x40}));
    CB_CHECK(starts_of("0\tmov    cl,0x1\n2\tjmp    0x40\n40\tnop\n").first == std::vector<std::uint32_t>{0});
    CB_CHECK(starts_of("0\tnop\n20\tjmp    0x40\n22\tnop\n30\tjmp    0x40\n40\tnop\n").first == std::vector<std::uint32_t>{0});
    // a function ends where the next starts, the last at the segment's end
    const auto [starts, functions] = starts_of("0\tcall   0x10\n3\tnop\n10\tnop\n");
    CB_CHECK_EQ(functions.at(0).end, 0x10u);
    CB_CHECK_EQ(functions.at(0x10).end, 0x1c8u);
    CB_CHECK_EQ(functions.at(0x10).name, "sub_0010");
    // no rows at all: no functions, and the entry points that no instruction starts are dropped
    CB_CHECK(starts_of("").first.empty());
});

CB_TEST("source.ne_asm.segment.names_and_overrides", [] {
    const ne::Module module = ne::Module::open(lz_bytes());
    const sg::SharedOrdinals ordinals = test_ordinals();
    const Rows rows = rows_file("scenario_b.rows");
    // segment 2 exports LZINIT at 0 and GETEXPANDEDNAME at 0x1bc; an override renames the anonymous 0x80 and cannot rename an export
    const sg::Seg seg(module, 2, ordinals, {{0x80, "Worker"}, {0, "Ignored"}}, rows.objdump());
    CB_CHECK_EQ(seg.name_at(0), "LZINIT");
    CB_CHECK_EQ(seg.name_at(0x80), "Worker");
    CB_CHECK_EQ(seg.name_at(0x44), "sub_0044");
    CB_CHECK_EQ(seg.functions().at(0x80).name, "Worker");
    CB_CHECK_EQ(seg.export_names().at(0x1bc), "GETEXPANDEDNAME");
    // a call to an overridden function reads with the new name
    CB_CHECK_EQ(sg::annotate_row(seg, sg::Row{0x40, "call   0x80"}), "    0040:  call   0x80      ; -> Worker");
    // function_rows of a start the carve did not find: a span to the next start
    const sg::FunctionRows odd = seg.function_rows(0x44);
    CB_CHECK_EQ(odd.func.start, 0x44u);
    CB_CHECK_EQ(odd.func.end, 0x80u);
    CB_CHECK_EQ(odd.rows.size(), 0u);   // 0x44 is no instruction of the listing: no rows
});

CB_TEST("source.ne_asm.segment.errors", [] {
    const Bytes data = lz_bytes();
    const ne::Module module = ne::Module::open(Bytes(data));
    const sg::SharedOrdinals ordinals = test_ordinals();
    const fx::Objdump nothing = [](cb::ByteView, std::uint32_t) { return std::string(); };
    CB_CHECK_THROWS_AS(cb::UsageError, (void)sg::Seg(module, 0, ordinals, {}, nothing));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)sg::Seg(module, 13, ordinals, {}, nothing));
    // a segment that lies outside the file
    CB_CHECK_THROWS_AS(cb::FormatError, (void)sg::Seg(ne::Module::open(Bytes(data.begin(), data.begin() + 0x400)), 1, ordinals, {}, nothing));
    CB_CHECK_NOTHROW((void)sg::Seg(module, 12, ordinals, {}, nothing));   // the data segment can be read as a listing of nothing
    // no listing: the entry points are no function starts
    const sg::Seg bare(module, 1, ordinals, {}, nothing);
    CB_CHECK(bare.rows().empty() && bare.starts().empty() && bare.functions().empty());
    CB_CHECK(!bare.word_at(0x1c7) && bare.word_at(0x1c6));
    CB_CHECK(!bare.resolve_call(0x1000, "nop"));
    CB_CHECK_EQ(sg::find_entry(module, 3)->first, 2u);
    CB_CHECK(!sg::find_entry(module, 31) && !sg::find_entry(module, 0));
});

CB_TEST("source.ne_asm.segment.reports_with_the_real_objdump", [] {
    // objdump of binutils is not in the repository: without it on the PATH the canned scenarios above are what runs
    try {
        if (!cb::process::run({.argv = {"objdump", "--version"}}).ok()) return;
    } catch (const cb::ToolError&) {
        return;
    }
    const auto want = golden("real_lzexpand.golden.txt");
    const ne::Module module = ne::Module::open(lz_bytes());
    const sg::SharedOrdinals ordinals = test_ordinals();
    CB_CHECK_EQ(trimmed(sg::map_report(module, ordinals)), want.at("map"));
    CB_CHECK_EQ(trimmed(sg::list_report(module, ordinals, 6)), want.at("list 6"));
    CB_CHECK_EQ(trimmed(sg::list_report(module, ordinals, 3)), want.at("list 3"));
    CB_CHECK_EQ(trimmed(sg::function_listing(module, ordinals, 6, 0xA4)), want.at("fn 6 a4"));
    CB_CHECK_EQ(trimmed(sg::trace_report(module, ordinals, 3, 0)), want.at("trace 3 0"));
    CB_CHECK_EQ(trimmed(sg::wndprocs_report(module, ordinals, std::nullopt)), want.at("wndprocs"));
    // disassemble: the rows of a short blob
    const Bytes code = {0x1e, 0x58, 0x90, 0x45, 0x55, 0x8b, 0xec};
    const auto rows = sg::disassemble(code, 0x10);
    CB_CHECK_EQ(rows.size(), 6u);
    CB_CHECK(rows[0].offset == 0x10 && rows[0].text == "push   ds");
    CB_CHECK(rows[4].offset == 0x14 && rows[4].text == "push   bp" && rows[5].text == "mov    bp,sp");
    CB_CHECK(sg::disassemble({}, 0).empty());
});

CB_TEST("source.ne_asm.segment.command", [] {
    const auto path = cb::testing::data_path("formats/testdata/lzexpand.dll").string();
    const Rows rows = rows_file("scenario_a.rows");
    const fx::Objdump objdump = rows.objdump();
    const std::string ords = testdata("ORDINALS.md").string();
    const auto run = [&](std::vector<std::string> args) { return sg::main(args, objdump); };
    {
        cb::testing::CapturedStd captured;
        CB_CHECK_EQ(run({path, "--list", "1", "--ords", ords}), cb::kExitOk);
        CB_CHECK_EQ(run({path, "--fn", "0x90", "1", "--ords", ords}), cb::kExitOk);
        CB_CHECK_EQ(run({path, "--trace", "0x0", "1", "--ords", ords}), cb::kExitOk);
        CB_CHECK_EQ(run({path, "--trace", "3", "--ords", ords}), cb::kExitOk);   // an ordinal: LZINIT, segment 2 offset 0
        CB_CHECK_EQ(run({path, "--cskel", "0", "1", "--ords", ords}), cb::kExitOk);
        CB_CHECK_EQ(run({path, "--wndprocs", "--ords", ords}), cb::kExitOk);
        CB_CHECK_EQ(run({path, "--map", "--ords", ords}), cb::kExitOk);
        CB_CHECK_EQ(run({path, "--trace", "99", "--ords", ords}), cb::kExitFailed);   // not in the entry table
        const std::string out = captured.out();
        CB_CHECK(has(out, "# 4 functions in seg1"));
        CB_CHECK(has(out, "==== sub_0090  seg1:0x0090"));
        CB_CHECK(has(out, "# call graph from seg1:0x0000"));
        CB_CHECK(has(out, "# call graph from seg2:0x0000"));
        CB_CHECK(has(out, "AUTO-SKELETON from ne-asm segment --cskel"));
        CB_CHECK(has(out, "Exports:"));
        CB_CHECK(has(out, "ordinal 99 not found in entry table"));
    }
    CB_CHECK_THROWS_AS(cb::UsageError, (void)run({path}));                                    // no mode
    CB_CHECK_THROWS_AS(cb::UsageError, (void)run({path, "--trace", "xyz"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)run({path, "--fn", "nope", "1"}));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)run({testdata("ORDINALS.md").string(), "--map"}));   // not a module
});

// The Python module's own digest of 200 and more segments of the Windows 3.x and Setup corpus (the harness's `digest` cases:
// rows, fix-ups, exports, starts, every function with its arguments, calls, dispatch and handlers, a checksum of the annotated rows,
// word reads, spans of addresses that are no start, the call graph), compared field by field with this code's, over the real
// objdump. CB_NE_SEGMENT_HARNESS=<...>/state/work/typed-port/source.ne_asm.segment (cases.txt and expected.txt); skipped without it.
CB_TEST("source.ne_asm.segment.agrees_with_the_python_module_on_its_harness", [] {
    const auto folder = cb::fs::get_env("CB_NE_SEGMENT_HARNESS");
    if (!folder) return;
    try {
        if (!cb::process::run({.argv = {"objdump", "--version"}}).ok()) return;
    } catch (const cb::ToolError&) {
        return;
    }
    using Json = cb::Json;
    std::vector<std::vector<std::string>> cases;   // in the harness's order: path, segment, ordinals file, override flag (a name is used twice: two variants)
    {
        std::istringstream in(cb::fs::read_text(std::filesystem::path(*folder) / "cases.txt"));
        for (std::string line; std::getline(in, line);) {
            const auto f = cb::text::split(line, '\t');
            if (f.size() == 6 && f[1] == "digest") cases.push_back({std::string(f[2]), std::string(f[3]), std::string(f[4]), std::string(f[5])});
        }
    }
    std::istringstream in(cb::fs::read_text(std::filesystem::path(*folder) / "expected.txt"));
    const auto begin = std::chrono::steady_clock::now();
    std::size_t compared = 0, differ = 0;
    std::string first;
    std::size_t next_case = 0;
    for (std::string line; std::getline(in, line);) {
        const std::size_t eq = line.find(" = ");
        if (eq == std::string::npos || !line.starts_with("digest_")) continue;
        if (next_case >= cases.size()) break;
        const std::vector<std::string>& args = cases[next_case++];
        Json want;
        if (!cb::json::try_parse(std::string_view(line).substr(eq + 3), want) || !want.is_array()) continue;   // an error case
        const std::size_t segment_index = static_cast<std::size_t>(*cb::text::parse_int<int>(args[1]));
        const sg::SharedOrdinals ordinals = std::make_shared<const sg::Ordinals>(args[2] == "-" ? sg::Ordinals{} : sg::load_ordinals(std::filesystem::path(args[2])));
        const ne::Module module = fx::open_module(std::filesystem::path(args[0]));
        if (segment_index < 1 || segment_index > module.segments().size()) continue;   // the Python counted from the end, or raised IndexError (see DIVERGES)
        sg::NameOverrides overrides;
        if (args[3] == "1") {
            const sg::Seg basic(module, segment_index, ordinals);
            std::size_t k = 0;
            for (const std::uint32_t start : basic.starts()) {
                if (k++ % 7 == 0) overrides[start] = std::format("ovr_{:x}", start);
            }
            overrides[3] = "zz";
            overrides[1000000] = "far";
        }
        const sg::Seg seg(module, segment_index, ordinals, overrides);
        // the same digest as the Python module's
        Json mine = Json::array();
        mine.push_back(seg.rows().size());
        mine.push_back(seg.fixups().size());
        Json exports = Json::array(), starts = Json::array(), funcs = Json::array(), per_function = Json::array();
        for (const auto& [offset, name] : seg.export_names()) exports.push_back(Json::array({offset, name}));
        for (const std::uint32_t s : seg.starts()) starts.push_back(s);
        for (const auto& [start, f] : seg.functions()) {
            funcs.push_back(Json::array({start, f.end, f.name}));
            const sg::FunctionRows fr = seg.function_rows(start);
            const auto dispatch = seg.dispatch(start);
            std::vector<std::uint32_t> handlers;
            for (const auto& d : dispatch) handlers.push_back(d.handler);
            std::sort(handlers.begin(), handlers.end());
            Json args_json = Json::array(), calls = Json::array(), dispatch_json = Json::array(), handler_calls = Json::array();
            for (const auto a : seg.args_used(start)) args_json.push_back(a);
            for (const auto& c : seg.calls_in(start)) calls.push_back(Json::array({c.kind == sg::CallKind::System ? "sys" : c.kind == sg::CallKind::Internal ? "int" : "far", sg::describe_call(seg, c)}));
            for (const auto& d : dispatch) {
                dispatch_json.push_back(Json::array({d.message, d.handler}));
                handler_calls.push_back(seg.handler_calls(start, d.handler, handlers));
            }
            const auto thunk = seg.is_thunk(start);
            per_function.push_back(Json::array({start, fr.rows.size(), fr.rows.empty() ? Json() : Json(fr.rows.front().offset), args_json, calls, dispatch_json,
                                                thunk ? Json(*thunk) : Json(), handler_calls}));
        }
        std::vector<std::string> annotated_lines;
        for (const sg::Row& row : seg.rows()) annotated_lines.push_back(sg::annotate_row(seg, row));
        const std::string annotated = cb::text::join(annotated_lines, "\n");
        const std::size_t code_size = seg.code().size();
        Json words = Json::array(), odd = Json::array(), graph = Json::array(), ends = Json::array(), fixup_list = Json::array();
        for (const std::int64_t x : {std::int64_t{0}, std::int64_t(code_size) - 2, std::int64_t(code_size) - 1, std::int64_t{-1}, std::int64_t(code_size), std::int64_t{1}}) {
            const auto w = x >= 0 ? seg.word_at(static_cast<std::uint32_t>(x)) : std::nullopt;
            words.push_back(w ? Json(*w) : Json());
        }
        const std::vector<std::uint32_t> function_starts(seg.starts().begin(), seg.starts().end());
        std::vector<std::int64_t> addresses = {0, 1, std::int64_t(code_size), std::int64_t(code_size) + 5, -3, function_starts.empty() ? 7 : std::int64_t(std::next(seg.functions().begin(), seg.functions().size() / 2)->first) + 1};
        for (const std::int64_t a : addresses) {
            if (a < 0) {
                odd.push_back(want[8][odd.size()]);   // (a negative address: the Python's own answer is taken: nothing here reads one)
                continue;
            }
            const sg::FunctionRows fr = seg.function_rows(static_cast<std::uint32_t>(a));
            odd.push_back(Json::array({a, fr.func.start, fr.func.end, fr.func.name, fr.rows.size()}));
        }
        if (!seg.functions().empty()) {
            for (const std::uint32_t s : sg::call_graph(seg, seg.functions().begin()->first)) graph.push_back(s);
        }
        if (!seg.rows().empty()) ends = Json::array({seg.rows().front().text, seg.rows().back().text});
        std::size_t listed = 0;
        for (const auto& [offset, target] : seg.fixups()) {
            if (listed++ == 40) break;
            fixup_list.push_back(Json::array({offset, target.label}));
        }
        mine.push_back(exports), mine.push_back(starts), mine.push_back(funcs), mine.push_back(per_function);
        mine.push_back(cb::crypto::crc32(cb::as_bytes(annotated)));
        mine.push_back(words), mine.push_back(odd), mine.push_back(graph), mine.push_back(ends), mine.push_back(seg.index()), mine.push_back(fixup_list);
        // (the Python's order: rows, fixups, exports, starts, funcs, per-function, crc, words, odd, graph, ends, si, fixups; the tail is spliced so)
        Json ordered = Json::array();
        for (const std::size_t i : {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u}) ordered.push_back(mine[i]);
        ++compared;
        std::string field;
        for (std::size_t i = 0; i < ordered.size() && i < want.size(); ++i) {
            if (ordered[i] != want[i]) {
                field = std::to_string(i);
                break;
            }
        }
        if (!field.empty() || ordered.size() != want.size()) {
            ++differ;
            if (differ <= 6) first += std::format("\n  {} seg {} (override {}): field {} differs", args[0].substr(args[0].find_last_of("/\\") + 1), args[1], args[3], field);
        }
    }
    const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - begin).count();
    CB_CHECK(compared > 100);
    CB_CHECK_EQ(differ, 0u);
    cb::err("segment harness: {} segments, {} differ, {:.1f} s (objdump included){}\n", compared, differ, seconds, first);
});
