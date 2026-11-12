// Tests of cb::translate::frontends::python::infer. The expectations are what the original module
// (codebreaker/translate/frontends/python/infer.py, with its methods module) answered for the same sources:
// testdata/infer_app.py and testdata/infer_issues.py run through `Program.solve` and the recording pass, the facts and
// issues written as sorted lines (testdata/*.golden.txt, reviewed). The sources use no call the library would have to
// type except `range` and `len`, which the test's library below types as methods.py does.
//
// DIVERGES: (1) the facts are compared as sets of lines: the Python's dicts are walked in the order facts were first
// set, which carries no meaning; (2) an iterator type prints as iter[<item>] (the Python printed `iter`); (3) a crash is
// reported as the error's message only.
#include "cb/translate/frontends/python/infer.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/infer_util.hpp"
#include "cb/translate/frontends/python/pyast.hpp"

namespace {

namespace in = cb::translate::frontends::python::infer;
namespace pr = cb::translate::frontends::python::program;
namespace pt = cb::translate::frontends::python::pytypes;
namespace pa = cb::translate::frontends::python::pyast;
namespace sx = ::pyfront;
using cb::translate::frontends::python::Wide;
using in::Kind;
using pt::Type;

// range and len, typed as the library of the original did; any other call is refused as stdlib:<name>.
class InferTestLibrary final : public in::Library {
public:
    in::TR method(in::Eval& ev, const sx::Call* call, const Type& recv, const std::string& attr, const in::Args& args,
                  const in::Kw& kw, const in::State& state) const override {
        return in::unmapped_library().method(ev, call, recv, attr, args, kw, state);
    }
    in::TR builtin(in::Eval& ev, const sx::Call* call, const std::string& name, const in::Args& args, const in::Kw& kw,
                   const in::State& state) const override {
        if (name == "len" && args.size() == 1 && args[0]) {
            if (in::is_one_of(*args[0], {Kind::Str, Kind::List, Kind::Bottom, Kind::Top})) {
                return in::TR(pt::size_type(), in::Res::of(in::ResKind::Builtin, "len"));
            }
            return in::TR(ev.issue(call, in::IssueClass::Conflict, "len of a " + in::kind_text(*args[0])));
        }
        if (name == "range" && kw.empty() && !args.empty() && args.size() <= 3) {
            for (const auto& a : args) {
                if (!a || !in::is_one_of(*a, {Kind::Int, Kind::Bool})) return in::TR(ev.issue(call, in::IssueClass::Conflict, "range arguments"));
            }
            const Type lo = args.size() == 1 ? pt::const_int(Wide(0)) : *args[0];
            const Type hi = args.size() == 1 ? *args[0] : *args[1];
            const std::optional<Wide> step = args.size() == 3 ? in::int_constant(call->args[2]) : std::optional<Wide>(Wide(1));
            if (!step || *step == Wide(0)) return in::TR(ev.issue(call, in::IssueClass::Syntax, "range with a computed step"));
            const Type item = *step > Wide(0) ? pt::int_range(lo.lo, hi.hi ? pt::Bound(*hi.hi - Wide(1)) : pt::Bound())
                                              : pt::int_range(hi.lo ? pt::Bound(*hi.lo + Wide(1)) : pt::Bound(), lo.hi);
            return in::TR(pt::iter_of(item, ev.range_count(call)), in::Res::of(in::ResKind::Builtin, "range"));
        }
        return in::unmapped_library().builtin(ev, call, name, args, kw, state);
    }
    in::TR std_call(in::Eval& ev, const sx::Call* call, const std::string& name, const in::Args& args, const in::Kw& kw,
                    const in::State& state) const override {
        return in::unmapped_library().std_call(ev, call, name, args, kw, state);
    }
};

const InferTestLibrary& test_library() {
    static const InferTestLibrary library;
    return library;
}

// (git may check the data out with CRLF line ends)
std::string inference_read(const std::string& relative) {
    std::string text = cb::fs::read_text(cb::testing::data_path(relative));
    std::erase(text, '\r');
    return text;
}

void inference_load(pr::Program& p, const std::string& source) {
    p.add_module("app", source);
    p.index();
}

// The facts and issues of a solved program, one per line, sorted.
std::string inference_dump(pr::Program& p, std::size_t rounds) {
    std::vector<std::string> lines = {std::format("rounds {}", rounds)};
    for (const auto& [fn_name, params] : p.params) {
        for (const auto& [name, type] : params) lines.push_back(std::format("param {} {} {}", fn_name, name, pt::describe(type)));
    }
    for (const auto& [fn_name, type] : p.returns) lines.push_back(std::format("return {} {}", fn_name, pt::describe(type)));
    for (const auto& [class_name, attrs] : p.attrs) {
        for (const auto& [name, type] : attrs) lines.push_back(std::format("attr {} {} {}", class_name, name, pt::describe(type)));
    }
    for (const auto& [module_name, globals] : p.globals) {
        for (const auto& [name, type] : globals) lines.push_back(std::format("global {} {} {}", module_name, name, pt::describe(type)));
    }
    for (const auto& [fn_name, f] : p.fns) {
        const std::vector<in::Issue> issues = p.evaluate([&] {
            in::Eval ev(p, f.get(), true, test_library());
            ev.run();
            return ev.issues;
        });
        for (const in::Issue& i : issues) lines.push_back(std::format("issue {} {} {}", fn_name, i.line, i.reason()));
    }
    lines.push_back(p.crashes.empty() ? "crashes {}" : "crashes " + p.crashes.entries().front().second);
    std::ranges::sort(lines);
    return cb::text::join(lines, "\n");
}

std::string inference_golden(const std::string& file) {
    return std::string(cb::text::trim(inference_read("translate/frontends/python/testdata/" + file)));
}

std::string inference_solve_and_dump(const std::string& source_file) {
    pr::Program p;
    inference_load(p, inference_read("translate/frontends/python/testdata/" + source_file));
    in::Inference inference(test_library());
    const std::size_t rounds = p.solve(inference);
    return inference_dump(p, rounds);
}

// The recording pass of the function `name` of `source` (solved first).
struct Recorded {
    std::unique_ptr<pr::Program> program = std::make_unique<pr::Program>();
    std::unique_ptr<in::Eval> eval;
};

Recorded inference_record(const std::string& source, const std::string& name) {
    Recorded r;
    inference_load(*r.program, source);
    in::Inference inference(test_library());
    r.program->solve(inference);
    pr::Fn& f = *r.program->fns.at("app." + name);
    r.program->evaluate([&] {
        r.eval = std::make_unique<in::Eval>(*r.program, &f, true, test_library());
        r.eval->run();
        return 0;
    });
    return r;
}

// The reasons of the recorded issues, joined by "; ".
std::string inference_reasons(const Recorded& r) {
    std::vector<std::string> out;
    for (const in::Issue& i : r.eval->issues) out.push_back(i.reason());
    return cb::text::join(out, "; ");
}

}  // namespace

CB_TEST("translate.infer.app_facts", [] {
    CB_CHECK_EQ(inference_solve_and_dump("infer_app.py"), inference_golden("infer_app.golden.txt"));
});

CB_TEST("translate.infer.issues", [] {
    CB_CHECK_EQ(inference_solve_and_dump("infer_issues.py"), inference_golden("infer_issues.golden.txt"));
});

CB_TEST("translate.infer.recorded_types_and_resolutions", [] {
    const Recorded r = inference_record(R"PY(
class Box:
    def __init__(self, v: int):
        self.v = v

def use(n: int):
    b = Box(n)
    c = b.v
    return c
)PY", "use");
    // the argument n is annotated int, which is the whole int64 range: the constructor call is the one refusal
    CB_CHECK_EQ(inference_reasons(r), std::string("int-range:argument v in [None, None]"));
    // every call, attribute and name was typed; the constructor call resolved to Ctor, the field read to Field
    bool saw_ctor = false, saw_field = false;
    for (const pa::AnyNode& an : pa::walk(pa::AnyNode(static_cast<const sx::Stmt*>(r.program->fns.at("app.use")->node)))) {
        const sx::Expr* e = an.expr();
        if (e == nullptr) continue;
        const auto res = r.eval->res.find(e);
        if (res == r.eval->res.end()) continue;
        if (res->second.kind == in::ResKind::Ctor) {
            saw_ctor = true;
            CB_CHECK_EQ(*res->second.name, std::string("app.Box"));
            CB_CHECK(r.eval->types.at(e).kind == Kind::Inst);
        }
        if (res->second.kind == in::ResKind::Field) {
            saw_field = true;
            CB_CHECK_EQ(*res->second.name, std::string("app.Box"));
            CB_CHECK_EQ(res->second.member, std::string("v"));
        }
    }
    CB_CHECK(saw_ctor && saw_field);
    CB_CHECK_EQ(pt::describe(r.program->returns.at("app.use")), std::string("int[-9223372036854775808..9223372036854775807]"));
});

CB_TEST("translate.infer.narrowing", [] {
    // `x is None` narrows; a comparison bounds the int that was compared; a successful index bounds the index
    const Recorded r = inference_record(R"PY(
def f(x: int, data: bytes, p: int):
    if x < 10:
        small = x
    else:
        small = 0
    return small + data[p]

def g(x: int, data: bytes, p: int):
    if x < 10 and x > 2:
        small = x
    else:
        small = 0
    return small + data[p]
)PY", "f");
    // f: x is bounded above by the test but not below, so the sum leaves int64; g: both bounds hold
    CB_CHECK_EQ(inference_reasons(r), std::string("int-range:return value in [None, 264]"));
    CB_CHECK_EQ(pt::describe(r.program->returns.at("app.f")), std::string("int[-9223372036854775808..264]"));
    CB_CHECK_EQ(pt::describe(r.program->returns.at("app.g")), std::string("int[0..264]"));
});

CB_TEST("translate.infer.loops_bound_counters", [] {
    // a counter stepped by a bounded amount in a loop of bounded passes stays bounded; one in `while True` is refused
    pr::Program p;
    inference_load(p, R"PY(
def bounded():
    k = 0
    for i in range(100):
        k += 3
    return k

def unbounded():
    k = 0
    while True:
        k += 1
    return k
)PY");
    in::Inference inference(test_library());
    p.solve(inference);
    CB_CHECK_EQ(pt::describe(p.returns.at("app.bounded")), std::string("int[0..300]"));
    CB_CHECK_EQ(pt::describe(p.returns.at("app.unbounded")), std::string("int[0..9223372036854775807]"));
});

CB_TEST("translate.infer.no_library_refuses_calls", [] {
    // with no library, a builtin call is refused with its name (the coverage report lists exactly what is missing)
    pr::Program p;
    inference_load(p, "def f(x: int):\n    return abs(x)\n");
    in::Inference inference;   // unmapped_library()
    p.solve(inference);
    const std::vector<in::Issue> issues = p.evaluate([&] {
        in::Eval ev(p, p.fns.at("app.f").get(), true);
        ev.run();
        return ev.issues;
    });
    CB_CHECK_EQ(issues.size(), std::size_t{1});
    CB_CHECK_EQ(issues[0].reason(), std::string("stdlib:abs"));
});

CB_TEST("translate.infer.module_level", [] {
    pr::Program p;
    inference_load(p, R"PY(
LIMIT = 100
class C:
    SIZE = 4
def f(x: int = 7):
    return x
)PY");
    in::Inference inference(test_library());
    p.solve(inference);
    CB_CHECK_EQ(pt::describe(p.globals.at("app").at("LIMIT")), std::string("int[100..100]"));
    CB_CHECK_EQ(pt::describe(p.class_attr_types.at({"app.C", "SIZE"})), std::string("int[4..4]"));
    CB_CHECK_EQ(pt::describe(p.default_types.at({"app.f", "x"})), std::string("int[7..7]"));
});

CB_TEST("translate.infer.helpers", [] {
    const auto top_inside = pt::tuple_of({pt::top("why")});
    CB_CHECK(in::has_top(top_inside));
    CB_CHECK(!in::has_top(pt::tuple_of({pt::str()})));
    CB_CHECK(in::is_one_of(pt::str(), {Kind::Int, Kind::Str}));
    CB_CHECK(!in::is_one_of(pt::str(), {Kind::Int}));
    CB_CHECK(in::builtin_functions().contains("len"));
    CB_CHECK(in::dynamic_builtins().contains("getattr"));
    CB_CHECK(!in::dynamic_builtins().contains("len"));
    // args[a:b] of a tuple's items, as Python slices
    const std::vector<Type> items = {pt::str(), pt::none(), pt::boolean()};
    CB_CHECK_EQ(in::tuple_slice(items, Wide(1), std::nullopt).size(), std::size_t{2});
    CB_CHECK_EQ(in::tuple_slice(items, Wide(-1), std::nullopt).size(), std::size_t{1});
    CB_CHECK_EQ(in::tuple_slice(items, std::nullopt, Wide(9)).size(), std::size_t{3});
    CB_CHECK_EQ(in::tuple_slice(items, Wide(-9), Wide(1)).size(), std::size_t{1});
    CB_CHECK_EQ(in::tuple_slice(items, Wide(2), Wide(1)).size(), std::size_t{0});
    const Type iterator = pt::iter_of(pt::str(), Wide(3));
    CB_CHECK(iterator.kind == Kind::Iter && iterator.args[0].kind == Kind::Str && iterator.lo == Wide(3));
    CB_CHECK_EQ(pt::describe(iterator), std::string("iter[str]"));
    CB_CHECK_EQ(in::issue_class_word(in::IssueClass::IntRange), std::string_view("int-range"));
    CB_CHECK_EQ((in::Issue{in::IssueClass::Dynamic, "x", 3}).reason(), std::string("dynamic:x"));
    CB_CHECK_EQ((in::Issue{in::IssueClass::Syntax, "", 3}).reason(), std::string("syntax"));
});

CB_TEST("translate.infer.state", [] {
    in::State s;
    s.set("a", pt::str());
    s.set("b", pt::none());
    s.set("a", pt::boolean());   // a rebinding keeps its place
    CB_CHECK_EQ(s.items().front().first, std::string("a"));
    CB_CHECK(s.at("a").kind == Kind::Bool);
    CB_CHECK(!s.contains("c"));
    in::State t = s;
    t.pop("b");
    CB_CHECK(!s.same_keys(t));
    CB_CHECK(s.same_keys(s));
    CB_CHECK_THROWS_AS(cb::InternalError, (void)s.at("c"));
});
