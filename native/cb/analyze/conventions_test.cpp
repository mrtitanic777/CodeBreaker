// Tests of the calling-convention pass: the facts it works out for the hand-assembled sample program (each read against the
// machine code), the facts of fifteen real modules of the Windows 95 corpus against what the pass answered when it was
// cbanalyze.dll's conventions.cc (every function's fact by a digest of its document, the counts, the report lines, the groups
// of identical functions and the bridge layer; testdata/<module>.conventions.json, made by testdata/gen_conventions.py from the
// live Python and DLL), the document a fact becomes, and the rows the fact table keeps of a report.
//
// DIVERGES from the engine it replaces:
//   * A function whose returns pop different amounts, equally often, takes the smaller (the engine took the first in the order of
//     a Python set of the amounts, which is an accident of hashing); none of the 15 modules has one.
//   * The names of the 16/32-bit bridge APIs, the callback names and the printf-like imports are matched by code, not by
//     std::regex; the patterns are the same.
#include "cb/analyze/conventions.hpp"

#include <format>

#include <nlohmann/json.hpp>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/golden_image.hpp"
#include "cb/analyze/test_image.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;
using cb::Json;
namespace cv = cb::analyze::conventions;

void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok) cb::testing::record_failure(__FILE__, __LINE__, what);
}

std::string document(const cv::Fact& f) { return facts::to_text(cv::to_value(f)); }

// The sample program, analysed.
struct Analysed {
    test::Sample sample;
    Image image{sample.spec()};
    call_graph::Graph graph;
    cv::Facts facts;
    Analysed() {
        image.discover();
        graph = call_graph::build(image);
        facts = cv::functions(image, graph);
    }
    const cv::Fact& at(Va va) const { return facts.at(va); }
};

}  // namespace

// ---- the sample program --------------------------------------------------------------------------------------------------

CB_TEST("analyze.conventions.an_argument_slot_the_code_reads_gives_the_argument_count", [] {
    Analysed a;
    const cv::Fact& c = a.at(a.sample.C);
    // C: mov eax,[esp+4] ... ret: it reads one argument slot, four bytes at +0; the caller cleans up (cdecl)
    CB_CHECK(c.convention == "cdecl");
    CB_CHECK(c.removes == 0);
    CB_CHECK(c.arg_slots == (std::map<std::int64_t, std::int64_t>{{0, 4}}));
    CB_CHECK(c.args == 1);
    CB_CHECK(c.arg_source == "frame");
    CB_CHECK(!c.frame);
    CB_CHECK(c.shape == cv::Shape::Leaf);
});

CB_TEST("analyze.conventions.callers_that_clean_up_give_the_argument_count_away", [] {
    Analysed a;
    // A pushes two arguments, calls B and adds 8 to esp: B takes two
    const cv::Fact& b = a.at(a.sample.B);
    CB_CHECK(b.convention == "cdecl");
    CB_CHECK(b.args == 2);
    CB_CHECK(b.arg_source == "callers");
    // B is `xor eax,eax; ret`: a function that returns the constant 0 (and nothing else uses its result): a flag
    CB_CHECK(b.shape == cv::Shape::Constant);
    CB_CHECK(b.constant == 0);
    CB_CHECK(b.result == cv::ResultKind::Flag);
    // H is `ret`: empty; I and J call it without cleaning up anything: no arguments, as the callers say
    const cv::Fact& h = a.at(a.sample.H);
    CB_CHECK(h.shape == cv::Shape::Empty);
    CB_CHECK(h.args == 0);
    CB_CHECK(h.arg_source == "callers");
});

CB_TEST("analyze.conventions.a_frame_a_tail_jump_and_a_function_that_never_returns", [] {
    Analysed a;
    const cv::Fact& entry = a.at(a.sample.A);
    CB_CHECK(entry.frame);                       // push ebp; mov ebp,esp
    CB_CHECK(entry.saved.empty());               // (ebp is the frame, not a saved register)
    CB_CHECK(entry.shape == cv::Shape::Calls);
    CB_CHECK(entry.result == cv::ResultKind::Value);   // it returns what B returned
    // D calls itself and then jumps to B: it has no return of its own, so no convention; the jump is a tail call to B
    const cv::Fact& d = a.at(a.sample.D);
    CB_CHECK(!d.convention.has_value());
    CB_CHECK(!d.removes.has_value());
    CB_CHECK(d.tail_calls == std::vector<std::string>{"401020"});   // (named by address: nothing named B)
});

namespace {

// A 32-bit program of the given functions, each at its address (a function ends where the next begins).
Image program(const test::Asm& code, std::initializer_list<Va> starts, Va end) {
    ImageSpec spec;
    spec.kind = ImageKind::Pe;
    spec.bits = 32;
    spec.flat = true;
    Span span;
    span.va = 0x401000;
    span.data = code.bytes();
    span.executable = true;
    span.initialised = static_cast<Va>(span.data.size());
    spec.spans = {span};
    std::vector<Va> all(starts);
    for (std::size_t i = 0; i < all.size(); ++i) spec.functions.emplace_back(all[i], i + 1 < all.size() ? all[i + 1] : end);
    spec.roots = {all.front()};
    return Image(std::move(spec));
}

}  // namespace

CB_TEST("analyze.conventions.a_function_whose_callers_remove_different_amounts_takes_a_variable_argument_list", [] {
    // V: push ebp; mov ebp,esp; lea eax,[ebp+0xC]; pop ebp; ret  -- takes the address of an argument slot
    // C1 calls it with two arguments (add esp,8), C2 with three (add esp,12)
    test::Asm a(0x401000);
    a.raw({0x55, 0x8B, 0xEC, 0x8D, 0x45, 0x0C, 0x5D, 0xC3}).pad_to(0x401010);
    a.raw({0x6A, 0x01, 0x6A, 0x02}).call(0x401000).raw({0x83, 0xC4, 0x08}).ret().pad_to(0x401020);
    a.raw({0x6A, 0x01, 0x6A, 0x02, 0x6A, 0x03}).call(0x401000).raw({0x83, 0xC4, 0x0C}).ret().pad_to(0x401030);
    Image image = program(a, {0x401000, 0x401010, 0x401020}, 0x401030);
    image.discover();
    const cv::Facts facts = cv::functions(image, call_graph::build(image));
    const cv::Fact& v = facts.at(0x401000);
    CB_CHECK(v.variadic);
    CB_CHECK(v.args == 2);   // at least the smaller of what the callers remove, in words
    CB_CHECK(v.arg_source == "callers (at least)");
    CB_CHECK(v.convention == "cdecl");
    CB_CHECK(v.frame);
    CB_CHECK(document(v).find("\"variadic\": true") != std::string::npos);
    // one caller alone is not enough to say so
    test::Asm b(0x401000);
    b.raw({0x55, 0x8B, 0xEC, 0x8D, 0x45, 0x0C, 0x5D, 0xC3}).pad_to(0x401010);
    b.raw({0x6A, 0x01, 0x6A, 0x02}).call(0x401000).raw({0x83, 0xC4, 0x08}).ret().pad_to(0x401020);
    Image once = program(b, {0x401000, 0x401010}, 0x401020);
    once.discover();
    CB_CHECK(!cv::functions(once, call_graph::build(once)).at(0x401000).variadic);
});

CB_TEST("analyze.conventions.a_float_instruction_on_an_argument_slot", [] {
    // F: push ebp; mov ebp,esp; fld qword [ebp+8]; pop ebp; ret -- reads an eight-byte argument and leaves it on the float stack
    test::Asm a(0x401000);
    a.raw({0x55, 0x8B, 0xEC, 0xDD, 0x45, 0x08, 0x5D, 0xC3}).pad_to(0x401010);
    Image image = program(a, {0x401000}, 0x401010);
    image.discover();
    const cv::Fact f = cv::functions(image, call_graph::build(image)).at(0x401000);
    CB_CHECK_EQ(f.float_instructions, std::int64_t{1});
    CB_CHECK((f.float_args == std::vector<std::int64_t>{0}));
    CB_CHECK(f.result == cv::ResultKind::Float);
    CB_CHECK((f.arg_slots == std::map<std::int64_t, std::int64_t>{{0, 8}}));
    CB_CHECK(f.args == 2);   // an eight-byte slot is two words
    CB_CHECK(f.arg_source == "frame");
    CB_CHECK(document(f).find("\"float_args\": [0]") != std::string::npos);
});

CB_TEST("analyze.conventions.a_function_that_removes_its_own_arguments_is_stdcall", [] {
    // S: ret 8 -- the function removes its eight bytes of arguments itself
    test::Asm a(0x401000);
    a.raw({0xC2, 0x08, 0x00}).pad_to(0x401010);
    Image image = program(a, {0x401000}, 0x401010);
    image.discover();
    const cv::Fact s = cv::functions(image, call_graph::build(image)).at(0x401000);
    CB_CHECK(s.convention == "stdcall");
    CB_CHECK(s.removes == 8);
    CB_CHECK(s.args == 2);
    CB_CHECK(s.arg_source == "return");
    CB_CHECK(s.shape == cv::Shape::Empty);
});

CB_TEST("analyze.conventions.a_register_read_before_it_is_written_is_passed_in", [] {
    // R: mov eax,ecx; ret -- reads ecx first: `this`; and one that reads ecx and edx is fastcall
    test::Asm a(0x401000);
    a.raw({0x8B, 0xC1, 0xC3}).pad_to(0x401010);                 // mov eax,ecx; ret
    a.raw({0x8B, 0xC1, 0x03, 0xC2, 0xC3}).pad_to(0x401020);     // mov eax,ecx; add eax,edx; ret
    a.raw({0x8B, 0xC3, 0xC2, 0x04, 0x00}).pad_to(0x401030);     // mov eax,ebx; ret 4
    Image image = program(a, {0x401000, 0x401010, 0x401020}, 0x401030);
    image.discover();
    const cv::Facts facts = cv::functions(image, call_graph::build(image));
    CB_CHECK(facts.at(0x401000).reg_args == std::vector<std::string>{"ecx"});
    CB_CHECK(facts.at(0x401000).convention == "thiscall");
    CB_CHECK((facts.at(0x401010).reg_args == std::vector<std::string>{"ecx", "edx"}));
    CB_CHECK(facts.at(0x401010).convention == "fastcall");
    CB_CHECK(facts.at(0x401020).reg_args == std::vector<std::string>{"ebx"});
    CB_CHECK(facts.at(0x401020).convention == "register");
});

CB_TEST("analyze.conventions.names_are_the_functions_or_their_address", [] {
    test::Sample sample;
    ImageSpec spec = sample.spec();
    spec.function_names[sample.B] = "the_zero";
    Image image(std::move(spec));
    image.discover();
    const cv::Facts facts = cv::functions(image, call_graph::build(image));
    CB_CHECK_EQ(facts.at(sample.B).name, std::string("the_zero"));
    CB_CHECK_EQ(facts.at(sample.A).name, std::string("401000"));
    // D tail-calls B, which has a name now
    CB_CHECK(facts.at(sample.D).tail_calls == std::vector<std::string>{"the_zero"});
});

CB_TEST("analyze.conventions.the_document_has_the_keys_the_analysis_has_always_written", [] {
    Analysed a;
    // B: every key a function has, `constant` because it returns one; no tail_calls, loops, string_ops, float, variadic, entry or procedure
    CB_CHECK_EQ(document(a.at(a.sample.B)),
                std::string("{\"addr\": 4198432, \"arg_source\": \"callers\", \"args\": 2, \"constant\": 0, \"convention\": \"cdecl\", "
                            "\"frame\": false, \"kind\": \"convention\", \"locals\": 0, \"name\": \"401020\", \"reg_args\": [], \"removes\": 0, "
                            "\"result\": \"flag\", \"result_evidence\": {\"callers_ignore\": 0, \"callers_use\": 0}, \"saved\": [], "
                            "\"shape\": \"constant\"}"));
    // D: a tail call, and nulls for what it does not say
    CB_CHECK_EQ(document(a.at(a.sample.D)),
                std::string("{\"addr\": 4198496, \"arg_source\": null, \"args\": null, \"convention\": null, \"frame\": false, "
                            "\"kind\": \"convention\", \"locals\": 0, \"name\": \"401060\", \"reg_args\": [], \"removes\": null, "
                            "\"result\": null, \"result_evidence\": {\"callers_ignore\": 0, \"callers_use\": 0}, \"saved\": [], "
                            "\"shape\": \"calls\", \"tail_calls\": [\"401020\"]}"));
});

CB_TEST("analyze.conventions.what_is_not_code_has_a_short_document", [] {
    cv::Fact f;
    f.addr = 0x401000;
    f.name = "401000";
    f.shape = cv::Shape::NotCode;
    CB_CHECK_EQ(document(f), std::string("{\"addr\": 4198400, \"arg_source\": null, \"args\": null, \"convention\": null, \"kind\": "
                                         "\"convention\", \"name\": \"401000\", \"shape\": \"not code\"}"));
});

CB_TEST("analyze.conventions.the_summary_is_what_dispatch_and_structures_read", [] {
    Analysed a;
    const ConventionFacts summary = cv::summary(a.facts);
    CB_CHECK_EQ(summary.size(), a.facts.size());
    CB_CHECK_EQ(summary.at(a.sample.B).convention, std::string("cdecl"));
    CB_CHECK(summary.at(a.sample.B).removes == 0);
    CB_CHECK(summary.at(a.sample.A).frame);
    CB_CHECK_EQ(summary.at(a.sample.D).convention, std::string());   // no return seen: no convention
    CB_CHECK(!summary.at(a.sample.D).removes.has_value());
});

CB_TEST("analyze.conventions.duplicates_are_functions_with_the_same_code", [] {
    Analysed a;
    // I and J push different strings: not the same code. Nothing in the sample repeats.
    CB_CHECK(cv::duplicates(a.image).empty());
    CB_CHECK(cv::duplicates(a.image, 1).empty());
    // the signature is a SHA-1, equal for the same code and different for another
    CB_CHECK_EQ(cv::signature(a.image, a.image.function(a.sample.B)).size(), std::size_t{40});
    CB_CHECK(cv::signature(a.image, a.image.function(a.sample.B)) != cv::signature(a.image, a.image.function(a.sample.H)));
    // two copies of `xor eax,eax; inc eax; inc eax; ret` and a shorter function
    ImageSpec spec;
    spec.kind = ImageKind::Pe;
    spec.bits = 32;
    spec.flat = true;
    Span code;
    code.va = 0x401000;
    code.data = {0x33, 0xC0, 0x40, 0x40, 0xC3, 0xCC, 0xCC, 0xCC, 0x33, 0xC0, 0x40, 0x40, 0xC3, 0xCC, 0xCC, 0xCC, 0x33, 0xC0, 0xC3, 0xCC};
    code.executable = true;
    code.initialised = static_cast<Va>(code.data.size());
    spec.spans = {code};
    spec.functions = {{0x401000, 0x401008}, {0x401008, 0x401010}, {0x401010, 0x401014}};
    spec.roots = {0x401000};
    Image image(std::move(spec));
    const auto groups = cv::duplicates(image, 3);
    CB_CHECK_EQ(groups.size(), std::size_t{1});
    CB_CHECK((groups[0] == std::vector<Va>{0x401000, 0x401008}));   // (the function of two instructions is below the least)
    CB_CHECK(cv::duplicates(image, 5).empty());                     // (the copies have four)
});

CB_TEST("analyze.conventions.the_bridge_layer_is_the_thunk_apis_a_module_imports", [] {
    test::Sample sample;
    Image plain(sample.spec());
    CB_CHECK(cv::thunk_layer(plain).empty());
    ImageSpec bridge = sample.spec();
    bridge.imports = {{sample.exit_slot, "KERNEL32!QT_Thunk"}, {sample.wsprintf_slot, "KERNEL32!ThunkConnect32"}, {sample.buffer, "USER32!wsprintfA"}};
    Image layer(std::move(bridge));
    const auto apis = cv::thunk_layer(layer);
    CB_CHECK((apis == std::vector<std::string>{"QT_THUNK", "THUNKCONNECT32"}));   // upper case, sorted; wsprintfA is not one
});

// ---- the report ------------------------------------------------------------------------------------------------------------------

CB_TEST("analyze.conventions.the_report_counts_and_describes", [] {
    Analysed a;
    const cv::Report r = cv::analyse(a.image, a.graph);
    CB_CHECK_EQ(r.counts.functions, std::int64_t{10});
    CB_CHECK_EQ(r.counts.convention_known, std::int64_t{9});          // all but D
    CB_CHECK_EQ(r.counts.caller_removes_arguments, std::int64_t{9});  // every return is a plain `ret`
    CB_CHECK_EQ(r.counts.callee_removes_arguments, std::int64_t{0});
    CB_CHECK_EQ(r.counts.argument_count_known, std::int64_t{3});      // B and H from their callers, C from its frame
    CB_CHECK_EQ(r.counts.argument_count_from_callers, std::int64_t{2});
    CB_CHECK_EQ(r.counts.argument_count_from_frame, std::int64_t{1});
    CB_CHECK_EQ(r.counts.result_known, std::int64_t{7});
    CB_CHECK_EQ(r.counts.empty_or_constant, std::int64_t{2});         // B and H
    CB_CHECK_EQ(r.counts.leaf, std::int64_t{2});
    CB_CHECK_EQ(r.counts.tail_calling, std::int64_t{1});
    CB_CHECK_EQ(r.counts.duplicate_groups, std::int64_t{0});
    CB_CHECK_EQ(r.text.size(), std::size_t{1});
    CB_CHECK_EQ(r.text[0], std::string("conventions: cdecl 9, unknown 1"));
});

CB_TEST("analyze.conventions.the_fact_table_rows", [] {
    Analysed a;
    const cv::Report r = cv::analyse(a.image, a.graph);
    const std::vector<facts::Row> rows = cv::fact_rows(r, ImageKind::Pe);
    CB_CHECK_EQ(rows.size(), std::size_t{10});   // one per function: no duplicates, no bridge
    CB_CHECK_EQ(rows[0].addr, std::string("401000"));
    CB_CHECK_EQ(rows[0].kind, std::string("convention"));
    CB_CHECK(facts::to_text(rows[1].value).find("\"name\": \"401020\"") != std::string::npos);
    // the report of a module with a bridge: a `thunk_layer` row with no address
    ImageSpec bridge = a.sample.spec();
    bridge.imports = {{a.sample.exit_slot, "KERNEL32!FT_Thunk"}};
    Image layer(std::move(bridge));
    layer.discover();
    const cv::Report b = cv::analyse(layer, call_graph::build(layer));
    const auto bridge_rows = cv::fact_rows(b, ImageKind::Pe);
    CB_CHECK_EQ(bridge_rows.back().kind, std::string("thunk_layer"));
    CB_CHECK_EQ(bridge_rows.back().addr, std::string());
    CB_CHECK_EQ(facts::to_text(bridge_rows.back().value), std::string("{\"addr\": null, \"imports\": [\"FT_THUNK\"], \"kind\": \"thunk_layer\"}"));
    CB_CHECK_EQ(b.text.back(), std::string("16/32-bit bridge imports: FT_THUNK"));
});

// ---- fifteen real modules ----------------------------------------------------------------------------------------------------------

namespace {

std::string counts_text(const cv::Counts& c) {
    return std::format("{} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}", c.functions, c.convention_known,
                       c.callee_removes_arguments, c.caller_removes_arguments, c.argument_count_known, c.argument_count_from_return,
                       c.argument_count_from_callers, c.argument_count_from_frame, c.register_arguments, c.this_in_ecx, c.result_known,
                       c.result_none, c.import_thunks, c.jump_thunks, c.tail_calling, c.empty_or_constant, c.leaf, c.with_loops,
                       c.inline_string_operations, c.floating_point, c.variadic, c.system_called_procedures, c.duplicate_groups,
                       c.duplicate_functions, c.bridge_imports);
}

// The counts the Python reported, in the order of cv::Counts.
std::string counts_text(const Json& j) {
    const char* keys[] = {"functions", "convention_known", "callee_removes_arguments", "caller_removes_arguments", "argument_count_known",
                          "argument_count_from_return", "argument_count_from_callers", "argument_count_from_frame", "register_arguments",
                          "this_in_ecx", "result_known", "result_none", "import_thunks", "jump_thunks", "tail_calling", "empty_or_constant",
                          "leaf", "with_loops", "inline_string_operations", "floating_point", "variadic", "system_called_procedures",
                          "duplicate_groups", "duplicate_functions", "bridge_imports"};
    std::string out;
    for (const char* k : keys) out += (out.empty() ? "" : " ") + std::to_string(j.at(k).get<std::int64_t>());
    return out;
}

}  // namespace

CB_TEST("analyze.conventions.golden_the_facts_of_fifteen_real_modules", [] {
    std::size_t functions = 0, procedures = 0, with_registers = 0, thiscalls = 0, entries = 0;
    for (const char* m : cb::analyze::test::kGoldenModules) {
        const std::string module = m;
        const Json want = cb::json::load(cb::testing::data_path("analyze/testdata/" + module + ".conventions.json"));
        Image image(cb::analyze::test::read_golden_image(module));
        image.discover();
        const call_graph::Graph graph = call_graph::build(image);
        const cv::Report report = cv::analyse(image, graph);
        const Json& digests = want.at("digests");
        expect(report.facts.size() == digests.size(), std::format("{}: {} facts, wanted {}", module, report.facts.size(), digests.size()));
        for (const Json& row : digests) {
            const Va at = row.at(0).get<std::int64_t>();
            const auto it = report.facts.find(at);
            if (it == report.facts.end()) {
                expect(false, std::format("{}: no fact for {:x}", module, at));
                continue;
            }
            const std::string text = document(it->second);
            const auto crc = cb::crypto::crc32(cb::as_bytes(text));
            expect(crc == row.at(1).get<std::uint32_t>(), std::format("{}: the fact of {:x} differs: {}", module, at, text));
            ++functions;
            procedures += it->second.procedure.has_value();
            with_registers += !it->second.reg_args.empty();
            thiscalls += it->second.convention == "thiscall";
            entries += !it->second.entry.empty();
        }
        expect(counts_text(report.counts) == counts_text(want.at("counts")),
               std::format("{}: counts {} wanted {}", module, counts_text(report.counts), counts_text(want.at("counts"))));
        std::vector<std::string> text(want.at("text").size());
        for (std::size_t i = 0; i < text.size(); ++i) text[i] = want.at("text").at(i).get<std::string>();
        expect(report.text == text, std::format("{}: report lines differ", module));
        // the rows that are not a function's: the groups of identical functions and the bridge layer
        const std::vector<facts::Row> rows = cv::fact_rows(report, image.kind());
        std::vector<const facts::Row*> other;
        for (const facts::Row& r : rows)
            if (r.kind != "convention") other.push_back(&r);
        expect(other.size() == want.at("rows").size(), std::format("{}: {} other rows, wanted {}", module, other.size(), want.at("rows").size()));
        for (std::size_t k = 0; k < other.size() && k < want.at("rows").size(); ++k) {
            const Json& w = want.at("rows").at(k);
            expect(w.at("kind").get<std::string>() == other[k]->kind, std::format("{}: row kind", module));
            if (other[k]->kind == "duplicates") {
                expect(w.at("addr").get<std::int64_t>() == other[k]->value.get("addr")->integer().value(), std::format("{}: duplicates addr", module));
                std::vector<std::string> names, got;
                for (const auto& n : w.at("functions")) names.push_back(n.get<std::string>());
                for (const auto& n : *other[k]->value.get("functions")->list()) got.push_back(*n.text());
                expect(names == got, std::format("{}: duplicates functions", module));
            }
        }
        // the documents the file keeps whole
        for (const Json& whole : want.at("full")) {
            const Va at = whole.at("addr").get<std::int64_t>();
            const auto it = report.facts.find(at);
            expect(it != report.facts.end() && document(it->second) == cb::json::dump(whole, cb::json::Style::PythonCompatible),
                   std::format("{}: the whole fact of {:x} differs", module, at));
        }
    }
    // what the modules between them exercise: the less common facts are in the comparison
    CB_CHECK(functions > 900);
    CB_CHECK(procedures > 0);
    CB_CHECK(with_registers > 0);
    CB_CHECK(thiscalls > 0);
    CB_CHECK(entries > 0);
});
