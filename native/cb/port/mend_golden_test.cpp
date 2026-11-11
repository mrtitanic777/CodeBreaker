// The port's passes, and the mending, against what the Python gave over real programs: the C of two old Microsoft
// tools (lib 2.20, and the first pass of the C 3.00 compiler's own source) as the port had modernised it.
//
// testdata/pre_ansi_p5/<target>.json holds the program's texts as the passes were given them (`out` the modernised
// files, `orig` the original text of the few files a pass changes), the port's state at that point (`port`: the
// functions, the files' own text, the library names ...), and for each function what the Python
// (codebreaker/port/pre_ansi.py) returned or did to the state: one case per file for each text pass, call_at on
// sampled lines, widen_member on structure members, union_member_for, visible_tags, the three passes over every file
// at once, stored_wide and passed_on, and `mend` over six complaints of each kind (and a second run of all of them
// twice, the loop's way). The files were made by state/work/cpp-refactor/pre_ansi_p5/dump.py from the live Python;
// the sampled complaints are the old harness's (state/work/typed-port/port.pre_ansi.p5/expected.py).
//
// What is compared is what the answer means: notes are compared as sets of places under each reason (the order a
// pass found them in is not meaning); regular-expression rules by their text; the order of rules that different
// variables made is not compared (the Python listed function pointers in the order it met them, the C++ by name).
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <format>
#include <map>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/csource.hpp"
#include "cb/port/mend.hpp"
#include "cb/port/old_c_passes.hpp"

namespace {

using cb::Json;
using namespace cb::port;

// A check whose failure says which case and what.
void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok)
        cb::testing::record_failure(__FILE__, __LINE__, what);
}

Sources sources_from(const Json& pairs) {
    Sources out;
    for (const Json& p : pairs)
        out[p.at(0).get<std::string>()] = p.at(1).get<std::string>();
    return out;
}

Function function_from(const Json& j) {
    Function f;
    f.name = j.at("name");
    f.ret = j.at("ret");
    f.is_static = j.at("static");
    f.returns_void = j.at("void");
    f.loose = j.at("loose");
    f.varargs = j.at("varargs");
    f.ret_set = j.at("ret_set");
    f.int_ret = j.at("int_ret");
    f.keep_int = j.at("keep_int");
    for (const Json& n : j.at("implicit"))
        f.implicit.insert(n.get<std::string>());
    f.body = j.at("body");
    if (!j.at("file").is_null())
        f.file = j.at("file").get<std::string>();
    for (const Json& p : j.at("params"))
        f.params.push_back({p.at(0), p.at(1)});
    return f;
}

bool same_function(const Function& a, const Function& b) {
    return a.name == b.name && a.ret == b.ret && a.is_static == b.is_static && a.returns_void == b.returns_void &&
           a.file == b.file && a.loose == b.loose && a.varargs == b.varargs && a.ret_set == b.ret_set &&
           a.int_ret == b.int_ret && a.keep_int == b.keep_int && a.implicit == b.implicit && a.body == b.body &&
           std::equal(a.params.begin(), a.params.end(), b.params.begin(), b.params.end(),
                      [](const Param& x, const Param& y) { return x.name == y.name && x.type == y.type; });
}

struct Program {
    Sources out;
    Sources orig;
    MendState state; // as the port had it
    Names ptr2, copied;
    std::vector<std::string> extra;
    Json cases, mend;
};

Program load_program(const Json& j) {
    Program p;
    p.out = sources_from(j.at("out"));
    p.orig = sources_from(j.at("orig"));
    const Json& port = j.at("port");
    for (const Json& f : port.at("fns"))
        p.state.functions[f.at("name")] = function_from(f);
    for (const Json& n : port.at("libc"))
        p.state.libc.insert(n.get<std::string>());
    p.state.arg_order_left = port.at("arg_order") == "left";
    p.state.int16 = port.at("int16");
    p.state.files = sources_from(port.at("files"));
    for (const Json& a : port.at("asm_protos"))
        p.state.asm_protos[a.at(0)] = a.at(1).get<std::string>();
    for (const Json& a : port.at("renames"))
        p.state.renames[a.at(0)] = a.at(1).get<std::string>();
    for (const Json& o : port.at("line_origin")) {
        p.state.line_origin[o.at(0)][o.at(1).get<std::size_t>()] = {o.at(2), o.at(3)};
    }
    for (const Json& e : port.at("fptr")) {
        for (const Json& t : e.at(1))
            p.state.fptr[e.at(0)].insert(t.get<std::string>());
    }
    if (port.contains("fptr_mixed")) {
        for (const Json& n : port.at("fptr_mixed"))
            p.state.fptr_mixed.insert(n.get<std::string>());
        for (const Json& g : port.at("given"))
            p.state.given_params.insert({g.at(0), g.at(1).get<std::size_t>()});
    }
    if (port.contains("rules")) {   // what the port's earlier parts left in the Port
        MendState& st = p.state;
        for (const Json& n : port.at("fptr_done"))
            st.fptr_done.insert(n.get<std::string>());
        for (const Json& e : port.at("retyped"))
            st.retyped[{e.at(0).at("file"), e.at(0).at("function"), e.at(0).at("variable")}] = e.at(1).get<std::string>();
        for (const Json& e : port.at("local_retypes"))
            st.local_retypes[{e.at(0).at("file"), e.at(0).at("function"), e.at(0).at("variable")}] = e.at(1).get<std::string>();
        for (const Json& r : port.at("rules")) {
            DeclRule rule{r.at(0), r.at(1), std::nullopt};
            if (!r.at(2).is_null())
                rule.split = SplitDecl{r.at(2).at(0), r.at(2).at(1)};
            st.decl_rules.push_back(std::move(rule));
        }
        for (const Json& e : port.at("line_fixes"))
            st.line_fixes[{e.at(0).at(0), e.at(0).at(1), e.at(0).at(2).get<long>()}] = e.at(1).get<std::string>();
        for (const Json& e : port.at("need_headers")) {
            for (const Json& h : e.at(1))
                st.need_headers[e.at(0)].insert(h.get<std::string>());
        }
        for (const Json& e : port.at("param_types"))
            st.param_types[{e.at(0).at(0), e.at(0).at(1).get<std::size_t>()}] = e.at(1).get<std::string>();
        for (const Json& e : port.at("mended")) {
            for (const Json& place : e.at(1))
                st.mended[e.at(0).get<std::string>()].push_back(place.get<std::string>());
        }
    }
    for (const Json& n : j.at("ptr2"))
        p.ptr2.insert(n.get<std::string>());
    for (const Json& n : j.at("copied"))
        p.copied.insert(n.get<std::string>());
    for (const Json& n : j.at("extra"))
        p.extra.push_back(n.get<std::string>());
    p.cases = j.at("cases");
    p.mend = j.at("mend");
    return p;
}

// reasons and places as sets of places under each reason
using NoteSet = std::map<std::string, std::multiset<std::string>>;

NoteSet notes_of(const Notes& n) {
    NoteSet out;
    for (const auto& [why, places] : n) {
        if (!places.empty())
            out[why].insert(places.begin(), places.end());
    }
    return out;
}

NoteSet notes_of(const Json& j) {
    NoteSet out;
    for (const Json& e : j) {
        auto& places = out[e.at(0).get<std::string>()];
        for (const Json& p : e.at(1))
            places.insert(p.get<std::string>());
    }
    return out;
}

// the reasons whose places differ, with the places only one side has
std::string note_differences(const NoteSet& actual, const NoteSet& expected) {
    std::string out;
    std::set<std::string> reasons;
    for (const auto& e : actual)
        reasons.insert(e.first);
    for (const auto& e : expected)
        reasons.insert(e.first);
    for (const std::string& why : reasons) {
        const auto a = actual.find(why), e = expected.find(why);
        const std::multiset<std::string> none;
        const auto& have = a != actual.end() ? a->second : none;
        const auto& want = e != expected.end() ? e->second : none;
        if (have == want)
            continue;
        out += "\n    " + why + ":";
        for (const std::string& p : have) {
            if (want.count(p) < have.count(p))
                out += " +[" + p + "]";
        }
        for (const std::string& p : want) {
            if (have.count(p) < want.count(p))
                out += " -[" + p + "]";
        }
    }
    return out;
}

bool check_notes(const std::string& what, const NoteSet& actual, const NoteSet& expected) {
    if (actual == expected)
        return true;
    expect(false,
           what + ": notes differ (+ the C++ has more, - the Python has more)" + note_differences(actual, expected));
    return false;
}

// ---- the text passes ----------------------------------------------------------------------------------------

Names names_of(const Json& list) {
    Names out;
    for (const Json& n : list)
        out.insert(n.get<std::string>());
    return out;
}

// `args` of a case: the names a pass was given besides the program's own (ptr2, copied, olds)
std::string run_text_pass(const std::string& op, const Program& p, const Json& args, const std::string& text,
                          Notes& notes, const std::string& file) {
    if (op == "block_heads")
        return block_heads(text, notes, file, args.contains("ptr2") ? names_of(args.at("ptr2")) : p.ptr2);
    if (op == "whole_word_stores")
        return whole_word_stores(text, notes, file);
    if (op == "file_internals")
        return file_internals(text, notes, file);
    if (op == "stdio_inits")
        return stdio_inits(text, notes, file);
    if (op == "word_copies")
        return word_copies(text, notes, file, args.contains("copied") ? names_of(args.at("copied")) : p.copied);
    return word_unions(text, notes, file, args.contains("olds") ? names_of(args.at("olds")) : Names{});
}

// What changed between two texts, as the dump records it: the lines that differ (same number of lines), or the whole
// text. Small, and a person can read it.
Json delta_of(const std::string& before, const std::string& after) {
    const std::vector<std::string> b = csource::split_lines(before), a = csource::split_lines(after);
    if (a.size() != b.size())
        return Json{{"text", after}};
    Json lines = Json::array();
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i])
            lines.push_back(Json::array({i, a[i]}));
    }
    return Json{{"lines", lines}};
}

// the first place two texts differ, as a few lines of each
std::string first_difference(const std::string& a, const std::string& b) {
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i])
        ++i;
    const std::size_t from = i > 60 ? i - 60 : 0;
    return std::format("at {}:\n    actual:   {}\n    expected: {}", i, a.substr(from, 140), b.substr(from, 140));
}

// ---- the state a mending leaves -----------------------------------------------------------------------------

using Rule =
    std::tuple<std::string, std::string, std::string, std::string>; // pattern, template, split name, split type

std::multiset<Rule> rules_of(const std::vector<DeclRule>& rules) {
    std::multiset<Rule> out;
    for (const DeclRule& r : rules) {
        out.emplace(r.pattern, r.replacement, r.split ? r.split->name : "", r.split ? r.split->type : "");
    }
    return out;
}

std::multiset<Rule> rules_of(const Json& j) {
    std::multiset<Rule> out;
    for (const Json& r : j) {
        const bool split = !r.at(2).is_null();
        out.emplace(r.at(0).get<std::string>(), r.at(1).get<std::string>(),
                    split ? r.at(2).at(0).get<std::string>() : "", split ? r.at(2).at(1).get<std::string>() : "");
    }
    return out;
}

VariableKey key_from(const Json& j) {
    return {j.at("file"), j.at("function"), j.at("variable")};
}

// Compares what the Python's state_json recorded with the state after the C++ ran. `before` is the port as it was.
void check_state(const std::string& what, const Json& expected, const MendState& after, const Sources& out_after,
                 const Program& p) {
    // the functions that changed, and how
    std::set<std::string> changed_actual, changed_expected;
    for (const auto& [name, fn] : after.functions) {
        if (!same_function(fn, p.state.functions.at(name)))
            changed_actual.insert(name);
    }
    for (const Json& f : expected.at("fns")) {
        const std::string name = f.at("name");
        changed_expected.insert(name);
        const auto it = after.functions.find(name);
        expect(it != after.functions.end() && same_function(it->second, function_from(f)),
               std::format("{}: function {} differs (ret '{}' void {} loose {} keep_int {} int_ret {} ret_set {})",
                           what, name, it != after.functions.end() ? it->second.ret : "?",
                           it != after.functions.end() && it->second.returns_void,
                           it != after.functions.end() && it->second.loose,
                           it != after.functions.end() && it->second.keep_int,
                           it != after.functions.end() && it->second.int_ret,
                           it != after.functions.end() && it->second.ret_set));
    }
    expect(changed_actual == changed_expected, what + ": a different set of functions changed");

    std::map<VariableKey, std::string> retyped, local;
    for (const Json& e : expected.at("retyped"))
        retyped[key_from(e.at(0))] = e.at(1);
    for (const Json& e : expected.at("local_retypes"))
        local[key_from(e.at(0))] = e.at(1);
    expect(after.retyped == retyped, what + ": retyped differ");
    expect(after.local_retypes == local, what + ": local_retypes differ");
    expect(rules_of(after.decl_rules) == rules_of(expected.at("rules")), what + ": decl_rules differ");

    std::map<LineFixKey, std::string> line_fixes;
    for (const Json& e : expected.at("line_fixes")) {
        line_fixes[{e.at(0).at(0), e.at(0).at(1), e.at(0).at(2).get<long>()}] = e.at(1).get<std::string>();
    }
    expect(after.line_fixes == line_fixes, what + ": line_fixes differ");

    std::map<std::string, std::set<std::string>> fptr, need_headers;
    for (const Json& e : expected.at("fptr")) {
        for (const Json& t : e.at(1))
            fptr[e.at(0)].insert(t.get<std::string>());
    }
    for (const Json& e : expected.at("need_headers")) {
        for (const Json& t : e.at(1))
            need_headers[e.at(0)].insert(t.get<std::string>());
    }
    expect(after.fptr == fptr, what + ": fptr differ");
    expect(after.need_headers == need_headers, what + ": need_headers differ");
    std::set<std::string> mixed, done;
    for (const Json& n : expected.at("fptr_mixed"))
        mixed.insert(n.get<std::string>());
    for (const Json& n : expected.at("fptr_done"))
        done.insert(n.get<std::string>());
    expect(after.fptr_mixed == mixed && after.fptr_done == done, what + ": fptr_mixed or fptr_done differ");

    std::map<std::pair<std::string, std::size_t>, std::string> param_types;
    for (const Json& e : expected.at("param_types"))
        param_types[{e.at(0).at(0), e.at(0).at(1).get<std::size_t>()}] = e.at(1);
    std::set<std::pair<std::string, std::size_t>> given;
    for (const Json& e : expected.at("given"))
        given.insert({e.at(0), e.at(1).get<std::size_t>()});
    expect(after.param_types == param_types, what + ": param_types differ");
    expect(after.given_params == given, what + ": given_params differ");

    std::map<std::string, std::string> renames;
    for (const Json& e : expected.at("renames"))
        renames[e.at(0)] = e.at(1).get<std::string>();
    expect(after.renames == renames, what + ": renames differ");
    check_notes(what, notes_of(after.mended), notes_of(expected.at("mended")));

    std::map<std::string, std::string> asm_protos, asm_after;
    for (const Json& e : expected.at("asm_protos"))
        asm_protos[e.at(0)] = e.at(1).get<std::string>();
    for (const auto& [k, v] : after.asm_protos)
        asm_after[k] = v;
    expect(asm_after == asm_protos, what + ": asm_protos differ");

    // the texts: the port's own that changed, and the program's
    std::map<std::string, Json> files_expected, files_actual, out_expected, out_actual;
    for (const Json& e : expected.at("files"))
        files_expected[e.at(0)] = e.at(1);
    for (const auto& [k, v] : after.files) {
        if (v != p.state.files.at(k))
            files_actual[k] = delta_of(p.state.files.at(k), v);
    }
    expect(files_actual == files_expected, what + ": the port's files differ");
    for (const Json& e : expected.at("changed_out"))
        out_expected[e.at(0)] = e.at(1);
    for (const auto& [k, v] : out_after) {
        if (v != p.out.at(k))
            out_actual[k] = delta_of(p.out.at(k), v);
    }
    if (out_actual != out_expected) {
        std::string detail;
        for (const auto& [k, v] : out_actual) {
            if (!out_expected.contains(k))
                detail += " [unexpected change to " + k + "]";
            else if (out_expected.at(k) != v)
                detail +=
                    " [" + k + " " + v.dump().substr(0, 400) + " vs " + out_expected.at(k).dump().substr(0, 400) + "]";
        }
        for (const auto& [k, v] : out_expected) {
            if (!out_actual.contains(k))
                detail += " [missing change to " + k + "]";
        }
        expect(false, what + ": program texts differ" + detail);
    }
}

// Where the time of a program's cases went, for the speed record (printed when CB_P5_TIMING is set).
struct Spent {
    std::map<std::string, double> seconds;
    template <class F> void timed(const std::string& what, F&& fn) {
        const auto t0 = std::chrono::steady_clock::now();
        fn();
        seconds[what] += std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    }
};

void run_cases(const Json& program, Spent& spent) {
    Program p = load_program(program);
    const std::string target = program.at("target");
    const Rules rules;
    std::size_t n = 0;
    for (const Json& c : p.cases) {
        ++n;
        const std::string op = c.at("op");
        const Json& want = c.at("result");
        const std::string what = std::format("{} #{} {}", target, n, op);
        if (op == "block_heads" || op == "whole_word_stores" || op == "file_internals" || op == "stdio_inits" ||
            op == "word_copies" || op == "word_unions") {
            const std::string file = c.at("file");
            const std::string& text = c.at("src") == "orig" ? p.orig.at(file) : p.out.at(file);
            Notes notes;
            std::string got;
            spent.timed("text passes", [&] {
                got = run_text_pass(op, p, c.contains("args") ? c.at("args") : Json::object(), text, notes, file);
            });
            const bool changed = got != text;
            expect(changed == want.at("changed").get<bool>(), what + " " + file + ": changed differs");
            if (changed && want.at("changed").get<bool>()) {
                expect(delta_of(text, got) == want.at("delta"),
                       what + " " + file + ": text differs: " + delta_of(text, got).dump().substr(0, 600));
            }
            check_notes(what + " " + file, notes_of(notes), notes_of(want.at("notes")));
        } else if (op == "call_at") {
            const std::string file = c.at("file");
            const std::vector<std::string> lines = csource::split_lines(p.out.at(file));
            std::optional<csource::CallAt> got;
            spent.timed("call_at",
                        [&] { got = csource::call_at(lines.at(c.at("line").get<std::size_t>() - 1), c.at("col")); });
            if (want.is_null()) {
                expect(!got, std::format("{} {}:{}:{}: a call where none", what, file, c.at("line").dump(),
                                         c.at("col").dump()));
            } else {
                expect(got && got->function == want.at(0) && got->index == want.at(1).get<std::size_t>() &&
                           got->span.start == want.at(2).at(0).get<std::size_t>() &&
                           got->span.end == want.at(2).at(1).get<std::size_t>(),
                       std::format("{} {}:{}:{}: call differs", what, file, c.at("line").dump(), c.at("col").dump()));
            }
        } else if (op == "widen_member") {
            const std::string file = c.at("file");
            const auto got =
                widen_member(p.out.at(file), c.at("tag").get<std::string>(), c.at("member").get<std::string>());
            if (want.is_null()) {
                expect(!got, what + ": widened what the Python did not");
            } else {
                expect(got && delta_of(p.out.at(file), *got) == want,
                       what + " " + file + " " + c.at("tag").dump() + " " + c.at("member").dump() + ": differs");
            }
        } else if (op == "union_member_for") {
            const auto got = union_member_for(p.out, c.at("union").get<std::string>(), c.at("type").get<std::string>());
            if (want.is_null()) {
                expect(!got, std::format("{} {} {}: gave {}", what, c.at("union").dump(), c.at("type").dump(),
                                         got.value_or("")));
            } else {
                expect(got && *got == want.get<std::string>(),
                       std::format("{} {} {}: differs", what, c.at("union").dump(), c.at("type").dump()));
            }
        } else if (op == "visible_tags") {
            const auto got = visible_tags(p.out, c.at("file").get<std::string>());
            std::set<std::string> expected;
            for (const Json& t : want)
                expected.insert(t.get<std::string>());
            expect(std::set<std::string>(got.begin(), got.end()) == expected,
                   what + " " + c.at("file").dump() + ": tags differ");
        } else if (op == "old_c_ways" || op == "numbered_places" || op == "agree_wide") {
            Sources o = p.out;
            Notes notes;
            spent.timed(op, [&] {
                if (op == "old_c_ways")
                    old_c_ways(o, notes);
                if (op == "numbered_places")
                    numbered_places(o, p.extra, notes);
                if (op == "agree_wide")
                    agree_wide(o, p.extra, notes, rules);
            });
            std::map<std::string, Json> actual, expected;
            for (const auto& [k, v] : o) {
                if (v != p.out.at(k))
                    actual[k] = delta_of(p.out.at(k), v);
            }
            for (const Json& e : want.at("changed"))
                expected[e.at(0)] = e.at(1);
            expect(actual == expected, what + ": the files changed differ");
            check_notes(what, notes_of(notes), notes_of(want.at("notes")));
        } else if (op == "stored_wide" || op == "passed_on") {
            MendState st = p.state;
            Sources o = p.out;
            bool changed = false;
            spent.timed(op, [&] { changed = op == "stored_wide" ? stored_wide(st, o, rules) : passed_on(st, o); });
            expect(changed == want.at("changed").get<bool>(), what + ": changed differs");
            check_state(what, want.at("state"), st, o, p);
        } else {
            expect(false, "unknown op " + op);
        }
    }

    for (const Json& m : p.mend) {
        const std::string name = m.at("name");
        std::vector<Diag> diags;
        for (const Json& d : m.at("diags")) {
            Diag diag;
            diag.file = d.at("file");
            diag.line = d.at("line");
            diag.col = d.at("col").get<std::size_t>();
            diag.text = d.at("text");
            if (d.contains("notes")) {
                for (const Json& x : d.at("notes"))
                    diag.notes.push_back(x.get<std::string>());
            }
            diags.push_back(std::move(diag));
        }
        MendState st = p.state;
        Sources o = p.out;
        if (name == "all_diagnostics") {
            const Json& rounds = m.at("rounds");
            for (std::size_t r = 0; r < rounds.size(); ++r) {
                bool changed = false;
                spent.timed("mend", [&] { changed = mend(st, o, diags, rules); });
                expect(changed == rounds[r].at("changed").get<bool>(),
                       std::format("{} mend all round {}: changed differs", target, r));
                if (!rounds[r].at("state").is_null())
                    check_state(std::format("{} mend all round {}", target, r), rounds[r].at("state"), st, o, p);
            }
        } else {
            bool changed = false;
            spent.timed("mend", [&] { changed = mend(st, o, diags, rules); });
            expect(changed == m.at("result").at("changed").get<bool>(),
                   std::format("{} mend {}: changed differs", target, name));
            check_state(std::format("{} mend {}", target, name), m.at("result").at("state"), st, o, p);
        }
    }
}

} // namespace

Json load_file(const std::string& name) {
    return cb::json::load(cb::testing::data_path("port/testdata/pre_ansi_p5/" + name));
}

void report(const char* name, const Spent& spent) {
    if (!cb::fs::get_env("CB_P5_TIMING"))
        return;
    double total = 0;
    for (const auto& [what, s] : spent.seconds) {
        std::fprintf(stderr, "  %s: %-22s %.3f s\n", name, what.c_str(), s);
        total += s;
    }
    std::fprintf(stderr, "  %s: %-22s %.3f s\n", name, "total", total);
}

CB_TEST("port.pre_ansi_p5.golden.lib-2.20", [] {
    Spent spent;
    run_cases(load_file("lib-2.20.json"), spent);
    report("lib-2.20", spent);
});
CB_TEST("port.pre_ansi_p5.golden.msc-3.00-p0", [] {
    Spent spent;
    run_cases(load_file("msc-3.00-p0.json"), spent);
    report("msc-3.00-p0", spent);
});
// small programs written to reach each branch of the passes and of the mending
// (state/work/cpp-refactor/pre_ansi_p5/units.py)
CB_TEST("port.pre_ansi_p5.golden.units", [] {
    const Json file = load_file("units.json");
    Spent spent;
    for (const Json& program : file.at("programs"))
        run_cases(program, spent);
    report("units", spent);
});

// Bigger programs, not checked in (the C 3.00 compiler's own source is 1.3 MB of golden data): any
// testdata/pre_ansi_p5/extra-*.json that dump.py has written is run too.
CB_TEST("port.pre_ansi_p5.golden.extra", [] {
    const std::filesystem::path dir = cb::testing::data_path("port/testdata/pre_ansi_p5");
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        const std::string name = entry.path().filename().string();
        if (!name.starts_with("extra-") || entry.path().extension() != ".json")
            continue;
        Spent spent;
        run_cases(load_file(name), spent);
        report(name.c_str(), spent);
    }
});
