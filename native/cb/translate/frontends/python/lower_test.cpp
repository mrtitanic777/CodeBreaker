// cb-unity: alone (test helpers named for this module)
// Tests of cb::translate::frontends::python::lower: small Python programs through inference and into L2, the tree printed compactly
// (one line a statement, an expression as a call of its kind) and compared with what the original module (lower.py) writes for the same
// source: the shapes were checked against lower.py's rules by hand, statement by statement (the expected texts are reviewed, not
// recorded). The library types only `len` and `range`, as infer_test's does.
//
// DIVERGES from the Python module, by design: the refusal is a struct (class, detail, line) and the class words are an enum; a function
// whose evaluation throws is refused as frontend-error with the error's kind and text; `@` is refused as syntax (Python raised a KeyError,
// reported as frontend-error); a record key that is not a constant is refused (the Python made a record with the key None); two sorted
// docstring lines are cleaned as inspect.cleandoc cleans them; the trees are made in the Context's arena.
#include "cb/translate/frontends/python/lower.hpp"

#include <format>
#include <functional>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/lower_internal.hpp"

namespace lw = cb::translate::frontends::python::lower;
namespace ir = cb::translate::ir;
namespace in = cb::translate::frontends::python::infer;
namespace pr = cb::translate::frontends::python::program;
namespace pt = cb::translate::frontends::python::pytypes;
namespace sx = ::pyfront;
using cb::translate::frontends::python::Wide;

namespace {

// range and len typed as the library of the original does; any other call is refused as stdlib:<name>.
class LowerTestLibrary final : public in::Library {
public:
    in::TR method(in::Eval& ev, const sx::Call* call, const pt::Type& recv, const std::string& attr, const in::Args& args, const in::Kw& kw,
                  const in::State& state) const override {
        return in::unmapped_library().method(ev, call, recv, attr, args, kw, state);
    }
    in::TR builtin(in::Eval& ev, const sx::Call* call, const std::string& name, const in::Args& args, const in::Kw& kw, const in::State& state) const override {
        using pt::Kind;
        if (name == "len" && args.size() == 1 && args[0]) {
            if (in::is_one_of(*args[0], {Kind::Str, Kind::List, Kind::Bottom, Kind::Top})) return in::TR(pt::size_type(), in::Res::of(in::ResKind::Builtin, "len"));
            return in::TR(ev.issue(call, in::IssueClass::Conflict, "len of a " + in::kind_text(*args[0])));
        }
        if (name == "range" && kw.empty() && !args.empty() && args.size() <= 3) {
            // (a parameter no caller has reached yet is Bottom)
            for (const auto& a : args)
                if (!a || !in::is_one_of(*a, {Kind::Int, Kind::Bool, Kind::Bottom})) return in::TR(ev.issue(call, in::IssueClass::Conflict, "range arguments"));
            for (const auto& a : args)
                if (a->kind == Kind::Bottom) return in::TR(pt::bottom());   // nothing known of it yet: nothing of its items
            const pt::Type lo = args.size() == 1 ? pt::const_int(Wide(0)) : *args[0];
            const pt::Type hi = args.size() == 1 ? *args[0] : *args[1];
            const std::optional<Wide> step = args.size() == 3 ? in::int_constant(call->args[2]) : std::optional<Wide>(Wide(1));
            if (!step || *step == Wide(0)) return in::TR(ev.issue(call, in::IssueClass::Syntax, "range with a computed step"));
            const pt::Type item = *step > Wide(0) ? pt::int_range(lo.lo, hi.hi ? pt::Bound(*hi.hi - Wide(1)) : pt::Bound()) : pt::int_range(hi.lo ? pt::Bound(*hi.lo + Wide(1)) : pt::Bound(), lo.hi);
            return in::TR(pt::iter_of(item, ev.range_count(call)), in::Res::of(in::ResKind::Builtin, "range"));
        }
        return in::unmapped_library().builtin(ev, call, name, args, kw, state);
    }
    in::TR std_call(in::Eval& ev, const sx::Call* call, const std::string& name, const in::Args& args, const in::Kw& kw, const in::State& state) const override {
        return in::unmapped_library().std_call(ev, call, name, args, kw, state);
    }
};

const LowerTestLibrary& library() {
    static const LowerTestLibrary l;
    return l;
}

// ---- the tree, compactly -----------------------------------------------------------------------------------------------------------

std::string show(const ir::Node* n);

std::string show_list(const std::vector<ir::Node*>& nodes, std::string_view separator = " ") {
    std::vector<std::string> parts;
    for (const ir::Node* x : nodes) parts.push_back(show(x));
    return cb::text::join(parts, separator);
}

std::string show_suite(const ir::Suite& suite) {
    if (const auto* one = std::get_if<ir::Node*>(&suite)) return *one != nullptr ? show(*one) : "-";
    return "{" + show_list(std::get<std::vector<ir::Node*>>(suite), "; ") + "}";
}

std::string lit_text(const ir::LitValue& v) {
    if (std::holds_alternative<std::monostate>(v)) return "None";
    if (const auto* b = std::get_if<bool>(&v)) return *b ? "True" : "False";
    if (const auto* i = std::get_if<std::int64_t>(&v)) return std::to_string(*i);
    if (const auto* d = std::get_if<double>(&v)) return std::format("{}", *d);
    if (const auto* s = std::get_if<std::string>(&v)) return "'" + *s + "'";
    return "?";
}

std::string show(const ir::Node* n) {
    if (n == nullptr) return "-";
    const auto type = [&] { return n->type ? ":" + ir::to_text(*n->type) : std::string(); };
    return ir::visit(*n, [&](const auto& k) -> std::string {
        using K = std::decay_t<decltype(k)>;
        if constexpr (std::is_same_v<K, ir::Block>) return "{" + show_list(k.stmts, "; ") + "}";
        else if constexpr (std::is_same_v<K, ir::Local>) return "var " + k.sym->name + ":" + ir::to_text(*k.type) + (k.init ? " = " + show(k.init) : "") + (k.mods.empty() ? "" : " [" + cb::text::join(k.mods, ",") + "]");
        else if constexpr (std::is_same_v<K, ir::ExprStmt>) return show(k.expr);
        else if constexpr (std::is_same_v<K, ir::If>) return "if (" + show(k.cond) + ") " + show_suite(k.then) + (ir::present(k.orelse) ? " else " + show_suite(k.orelse) : "");
        else if constexpr (std::is_same_v<K, ir::While>) return "while (" + show(k.cond) + ") " + show_suite(k.body);
        else if constexpr (std::is_same_v<K, ir::For>) return "for (" + show_list(k.init, ", ") + "; " + show(k.cond) + "; " + show_list(k.update, ", ") + ") " + show_suite(k.body);
        else if constexpr (std::is_same_v<K, ir::ForEach>) return "foreach (" + show(k.var) + " : " + show(k.iterable) + ") " + show(k.body);
        else if constexpr (std::is_same_v<K, ir::Return>) return k.expr ? "return " + show(k.expr) : "return";
        else if constexpr (std::is_same_v<K, ir::Throw>) return k.expr ? "throw " + show(k.expr) : "throw";
        else if constexpr (std::is_same_v<K, ir::Break>) return "break";
        else if constexpr (std::is_same_v<K, ir::Continue>) return "continue";
        else if constexpr (std::is_same_v<K, ir::Comment>) return "// " + k.text.value_or("");
        else if constexpr (std::is_same_v<K, ir::Try>) {
            std::string out = "try " + show(k.body);
            for (const ir::Node* c : k.catches) {
                const auto* catch_node = ir::get<ir::Catch>(c);
                std::vector<std::string> types;
                for (const ir::TypeP& t : catch_node->types) types.push_back(ir::to_text(*t));
                out += " catch (" + cb::text::join(types, "|") + ") " + show(catch_node->body);
            }
            return out;
        } else if constexpr (std::is_same_v<K, ir::Lit>) return lit_text(k.value) + type();
        else if constexpr (std::is_same_v<K, ir::Name>) return k.sym->name + type();
        else if constexpr (std::is_same_v<K, ir::This>) return "this" + type();
        else if constexpr (std::is_same_v<K, ir::TypeName>) return "type" + type();
        else if constexpr (std::is_same_v<K, ir::Select>) return show(k.obj) + "." + k.sym->name + type();
        else if constexpr (std::is_same_v<K, ir::Call>) {
            std::string out = (k.target ? show(k.target) + "." : std::string()) + (k.sym->owner == std::optional<std::string>("py") ? "py:" : "") + k.sym->name;
            if (!k.typeargs.empty()) {
                std::vector<std::string> targs;
                for (const ir::TypeArg& t : k.typeargs) {
                    if (const auto* s = std::get_if<std::string>(&t)) targs.push_back("'" + *s + "'");
                    else if (const auto* i = std::get_if<std::int64_t>(&t)) targs.push_back(std::to_string(*i));
                    else targs.push_back("type");
                }
                out += "<" + cb::text::join(targs, ",") + ">";
            }
            return out + "(" + show_list(k.args, ", ") + ")" + type();
        } else if constexpr (std::is_same_v<K, ir::New>) return "new " + k.sym->name + "(" + show_list(k.args, ", ") + ")" + type();
        else if constexpr (std::is_same_v<K, ir::Index>) return show(k.arr) + "[" + show_suite(k.index) + "]" + type();
        else if constexpr (std::is_same_v<K, ir::Unary>) return "(" + k.op + show(k.x) + ")" + type();
        else if constexpr (std::is_same_v<K, ir::Binary>) return "(" + show(k.left) + " " + k.op + " " + show(k.right) + ")" + type();
        else if constexpr (std::is_same_v<K, ir::Concat>) return "concat(" + show_list(k.parts, ", ") + ")";
        else if constexpr (std::is_same_v<K, ir::Assign>) return show(k.target) + " " + k.op.value_or("") + "= " + show(k.value);
        else if constexpr (std::is_same_v<K, ir::Cond>) return "(" + show(k.cond) + " ? " + show(k.then) + " : " + show(k.orelse) + ")";
        else if constexpr (std::is_same_v<K, ir::Cast>) return "cast(" + show(k.x) + ")" + type();
        else if constexpr (std::is_same_v<K, ir::Paren>) return "(" + show(k.x) + ")";
        else if constexpr (std::is_same_v<K, ir::Lambda>) return "lambda(" + show_list(k.params, ", ") + ") " + show(k.body);
        else if constexpr (std::is_same_v<K, ir::Param>) return k.sym->name + ":" + ir::to_text(*k.type) + (k.mods.empty() ? "" : " [" + cb::text::join(k.mods, ",") + "]");
        else if constexpr (std::is_same_v<K, ir::MethodDecl>) {
            return k.kind.value_or("") + " " + k.name.value_or("") + "(" + show_list(k.params, ", ") + ") -> " + ir::to_text(*k.ret) + " " + show_suite(k.body);
        } else return std::string(ir::kind_name(*n));
    });
}

// ---- running a program --------------------------------------------------------------------------------------------------------------

struct Run {
    pr::Program program;
    std::unique_ptr<lw::Context> context;
    cb::OrderedMap<std::string, lw::Lowered> lowered;
    explicit Run(const std::string& source, pr::IntPolicy policy = pr::IntPolicy::Strict) {
        program.int_policy = policy;
        program.add_module("app", source);
        program.index();
        in::Inference inference(library());
        program.solve(inference);
        context = std::make_unique<lw::Context>(program, library(), policy);
        std::vector<pr::Fn*> functions;
        for (auto& [name, f] : program.fns) functions.push_back(f.get());
        lowered = lw::lower_functions(*context, functions);
        lw::close_refusals(lowered);
    }
    // The function as text, or "refused <reason>".
    std::string text(const std::string& name) const {
        const lw::Lowered& l = *lowered.find("app." + name);
        return l.is_refused() ? "refused " + l.refused->reason() : show(l.decl);
    }
};

}  // namespace

CB_TEST("translate.python.lower.refusals_have_words_and_reasons", [] {
    CB_CHECK_EQ(lw::refusal_class_word(lw::RefusalClass::IntRange), "int-range");
    CB_CHECK_EQ(lw::refusal_class_word(lw::RefusalClass::CallsRefused), "calls-refused");
    CB_CHECK(lw::refusal_class_from_word("frontend-error") == std::optional<lw::RefusalClass>(lw::RefusalClass::FrontendError));
    CB_CHECK(!lw::refusal_class_from_word("nonsense").has_value());
    for (const in::IssueClass c : {in::IssueClass::Untyped, in::IssueClass::Conflict, in::IssueClass::IntRange, in::IssueClass::Dynamic, in::IssueClass::Stdlib,
                                   in::IssueClass::Extern, in::IssueClass::Syntax, in::IssueClass::Unresolved})
        CB_CHECK_EQ(lw::refusal_class_word(lw::refusal_class_of(c)), in::issue_class_word(c));   // the same words as the inference's
    CB_CHECK_EQ((lw::Refusal{lw::RefusalClass::Conflict, "two kinds", 7}).reason(), "conflict:two kinds");
    CB_CHECK_EQ((lw::Refusal{lw::RefusalClass::Dynamic, "", 7}).reason(), "dynamic");
    const lw::Refused r(lw::RefusalClass::Syntax, "with", 12);
    CB_CHECK_EQ(r.refusal().line, 12U);
    CB_CHECK_EQ(std::string(r.what()), "syntax:with");
    const lw::Refused from_issue(in::Issue{in::IssueClass::Stdlib, "str.upper", 3});
    CB_CHECK_EQ(from_issue.refusal().reason(), "stdlib:str.upper");
});

CB_TEST("translate.python.lower.text_readers", [] {
    CB_CHECK_EQ(lw::clean_docstring("  One line.  "), "One line.  ");
    CB_CHECK_EQ(lw::clean_docstring("\n    Title\n\n      indented\n    last\n    "), "Title\n\n  indented\nlast");
    CB_CHECK_EQ(lw::clean_docstring("first\n\tsecond"), "first\nsecond");   // a tab is eight columns, the margin is taken off the rest
    CB_CHECK_EQ(lw::clean_docstring(""), "");
    sx::Pos pos;
    pos.lineno = 2;
    pos.col_offset = 4;
    pos.end_lineno = 2;
    pos.end_col_offset = 8;
    CB_CHECK(lw::source_segment("x = 1\n    0xFF + 2\n", pos) == std::optional<std::string>("0xFF"));
    pos.end_lineno = 3;
    pos.end_col_offset = 1;
    CB_CHECK(lw::source_segment("a\r\n    b\r\nc", pos) == std::optional<std::string>("b\r\nc"));   // two lines, as written
    pos.end_lineno = 9;
    CB_CHECK(!lw::source_segment("a\nb\n", pos).has_value());
    sx::ConstValue v;
    v.kind = sx::ConstValue::Kind::Str;
    v.text = "caf\xc3\xa9\n";
    CB_CHECK_EQ(lw::default_text(v), "\"caf\\u00e9\\n\"");   // json.dumps: ASCII only
    v.kind = sx::ConstValue::Kind::Bytes;
    v.text = "\xe9z";
    CB_CHECK_EQ(lw::default_text(v), "{\"bytes\": \"\\u00e9z\"}");
    v.kind = sx::ConstValue::Kind::Float;
    v.f = 1.5;
    CB_CHECK_EQ(lw::default_text(v), "1.5");
    v.kind = sx::ConstValue::Kind::Int;
    v.fits_i64 = true;
    v.i64 = -7;
    CB_CHECK_EQ(lw::default_text(v), "-7");
    v.kind = sx::ConstValue::Kind::None;
    CB_CHECK_EQ(lw::default_text(v), "null");
    v.kind = sx::ConstValue::Kind::True;
    CB_CHECK_EQ(lw::default_text(v), "true");
});

// The program the lowering tests run: small functions that between them use most of what the front end lowers.
constexpr std::string_view kProgram = R"py(
LIMIT = 10

def clamp(v):
    if v < 0:
        return 0
    if v > 255:
        return 255
    return v

def while_loop(n):
    i = 0
    while i < n:
        i += 1
    return i

def flags(a, b):
    return a > 0 and b < 5 or not (a == b)

def between(x):
    return 1 < x < 10

def pick2(x):
    return x if x > 3 else 0

def fmt(n):
    return f"n={n}!"

def membership(x):
    return x in (1, 2, 3)

def opt(x):
    if x is None:
        return 0
    return x + 1

def use_opt():
    return opt(None) + opt(3)

def tuple_use():
    t = (1, 2)
    a, b = t
    return a + b

def swap_pair():
    a, b = 1, 2
    return a - b

def comp():
    r = [i * i for i in range(4)]
    return len(r)

def guarded(n):
    try:
        assert n > 0
        return n
    except ValueError:
        return 0

def thrower():
    raise ValueError("bad")

def header_size():
    h = {"magic": 1, "size": 2}
    return h["size"] + h["magic"]

def uses_global():
    return LIMIT + 1

def callers():
    return clamp(300) + while_loop(5) + flags(1, 2) + between(5) + pick2(7) + membership(2) + len(fmt(3)) + tuple_use() + swap_pair() + comp() + guarded(2) + header_size() + uses_global() + use_opt()

class Counter:
    def __init__(self, start):
        self.value = start

    def get(self):
        return self.value

def count_with():
    c = Counter(4)
    return c.get()
)py";

CB_TEST("translate.python.lower.functions_are_written_as_l2", [] {
    // Each function as the original's rules write it (lower.py), read statement by statement: an `if`/`elif` chain stays a chain, a counted
    // loop is a `for` over a fresh counter, `a, b = t` declares its names by binding, `x in (1, 2)` is a run of comparisons, an optional
    // narrowed by a test is unwrapped where it is read, a comprehension is built into a temporary that is returned, an `assert` is an
    // `if (!cond) throw`, a record is declared by `record<keys>(values)` and a read module name is a Select of the module.
    Run r{std::string(kProgram)};
    static const std::pair<std::string_view, std::string_view> expected[] = {
    {"clamp", R"x(function clamp(v:i64) -> i64 {if ((v:i64 < 0:i64):bool) {return 0:i64}; if ((v:i64 > 255:i64):bool) {return 255:i64}; return v:i64})x"},
    {"while_loop", R"x(function while_loop(n:i64) -> i64 {var i:i64 = 0:i64; while ((i:i64 < n:i64):bool) {i:i64 += 1:i64}; return i:i64})x"},
    {"flags", R"x(function flags(a:i64, b:i64) -> bool {return ((((a:i64 > 0:i64):bool && (b:i64 < 5:i64):bool):bool) || (!((a:i64 == b:i64):bool)):bool):bool})x"},
    {"between", R"x(function between(x:i64) -> bool {return ((1:i64 < x:i64):bool && (x:i64 < 10:i64):bool):bool})x"},
    {"pick2", R"x(function pick2(x:i64) -> i64 {return ((x:i64 > 3:i64):bool ? x:i64 : 0:i64)})x"},
    {"fmt", R"x(function fmt(n:i64) -> py.str {return concat('n=':py.str, py:str(n:i64):py.str, '!':py.str)})x"},
    {"membership", R"x(function membership(x:i64) -> bool {return ((((x:i64 == 1:i64):bool || (x:i64 == 2:i64):bool):bool || (x:i64 == 3:i64):bool):bool)})x"},
    {"opt", R"x(function opt(x:py.optional<i64>) -> i64 {if (py:is_none(x:py.optional<i64>):bool) {return 0:i64}; return (py:unwrap(x:py.optional<i64>):i64 + 1:i64):i64})x"},
    {"use_opt", R"x(function use_opt() -> i64 {return (opt(None:py.optional<i64>):i64 + opt(3:i64):i64):i64})x"},
    {"tuple_use", R"x(function tuple_use() -> i64 {var t:py.tuple<i64, i64> = py:tuple(1:i64, 2:i64):py.tuple<i64, i64>; py:bind(a:i64, b:i64):py.tuple<i64, i64> = t:py.tuple<i64, i64>; return (a:i64 + b:i64):i64})x"},
    {"swap_pair", R"x(function swap_pair() -> i64 {py:bind(a:i64, b:i64):py.tuple<i64, i64> = py:tuple(1:i64, 2:i64):py.tuple<i64, i64>; return (a:i64 - b:i64):i64})x"},
    {"comp", R"x(function comp() -> i64 {var r:py.list<i64> = py:empty():py.list<i64>; for (var i:i64 = 0:i64; (i:i64 < 4:i64):bool; (++i:i64):i64) {r:py.list<i64>.py:list.append((i:i64 * i:i64):i64):void}; return py:builtins.len(r:py.list<i64>):i64})x"},
    {"guarded", R"x(function guarded(n:i64) -> i64 {try {if ((!((n:i64 > 0:i64):bool)):bool) {throw new AssertionError():py.AssertionError}; return n:i64} catch (py.ValueError) {return 0:i64}})x"},
    {"thrower", R"x(function thrower() -> void {throw new ValueError('bad':py.str):py.ValueError})x"},
    {"header_size", R"x(function header_size() -> i64 {var h:app.HeaderSizeRecord = py:record<'magic','size'>(1:i64, 2:i64):app.HeaderSizeRecord; return (h:app.HeaderSizeRecord.size:i64 + h:app.HeaderSizeRecord.magic:i64):i64})x"},
    {"uses_global", R"x(function uses_global() -> i64 {return (type:app.LIMIT:i64 + 1:i64):i64})x"},
    {"Counter.__init__", R"x(ctor __init__(start:i64) -> void {this:app.Counter.value:i64 = start:i64})x"},
    {"Counter.get", R"x(method get() -> i64 {return this:app.Counter.value:i64})x"},
    {"count_with", R"x(function count_with() -> i64 {var c:app.Counter = new Counter(4:i64):app.Counter; return c:app.Counter.get():i64})x"},
    };
    for (const auto& [name, text] : expected) CB_CHECK_EQ(r.text(std::string(name)), std::string(text));
    // the sum of many calls is not shown to fit: refused, and its lowering kept (it was stopped at its signature: nothing found yet)
    CB_CHECK(r.text("callers").starts_with("refused int-range:"));
    CB_CHECK(r.lowered.find("app.callers")->lowering != nullptr);
    // what lowering found it needs
    const lw::FnLower& made = *r.lowered.find("app.count_with")->lowering;
    CB_CHECK((made.calls() == std::set<std::string>{"app.Counter.__init__", "app.Counter.get"}));
    CB_CHECK((made.classes_made() == std::set<std::string>{"app.Counter"}));
    CB_CHECK((r.lowered.find("app.uses_global")->lowering->globals_used() == std::set<std::pair<std::string, std::string>>{{"app", "LIMIT"}}));
    CB_CHECK(r.lowered.find("app.clamp")->lowering->calls().empty());
});

// What lowering refuses, what it declares and how it names it: a second program.
constexpr std::string_view kMore = R"py(
def g(a, b=5, c="x"):
    """Adds.

    Two things."""
    return a + b

def use():
    return g(1) + g(1, 2)

def mul(a: int, b: int):
    return a * b

def neg(a: int):
    return -a

def shifts(a: int):
    return (a & 255) << 3

def fn_with_lambda():
    f = lambda x: x
    return 1

def nested():
    def inner():
        return 1
    return inner()

def with_stmt():
    with open("f") as fh:
        return 1

def gen():
    yield 1

class Shape:
    def __init__(self, w):
        self.w = w

    @staticmethod
    def unit():
        return 1

    @property
    def area(self):
        return self.w

    @classmethod
    def make(cls):
        return 1

def use_shape():
    s = Shape(3)
    return s.area + Shape.unit()

def div_mod(a, b):
    return a // b + a % b

def uses_div():
    return div_mod(7, 2) + div_mod(9, 3)

def two_loops():
    t = 0
    for i in range(3):
        for j in range(2):
            t += i * j
    return t

def loop_var_after():
    for k in range(3):
        pass
    return k

def while_cond_ok(n):
    c = 0
    while c < n and c < 10:
        c += 1
    return c

def call_while():
    return while_cond_ok(4)

def tup(x):
    return (x, x + 1)

def use_tup():
    a, b = tup(1)
    return a + b

def stepped():
    s = 0
    for i in range(10, 0, -2):
        s += i
    return s

def ifexp_none(x):
    y = None
    if x > 0:
        y = x
    return y

def use_ifexp():
    return ifexp_none(3)

def membership_dict():
    h = {"a": 1}
    if "a" in h:
        return 1
    return 0
)py";

CB_TEST("translate.python.lower.more_of_what_a_function_can_hold", [] {
    // Defaults are modifiers of the parameter and filled in at the call; a refusal of the inference is the function's refusal (a lambda, a
    // nested function, a generator, a standard library call the library does not type, a classmethod); a static method and a property are
    // methods; `//` and `%` of values shown to be non-negative and positive are C++'s own operators; a loop variable read after the loop is
    // declared before it and copied from the loop's own counter (named k_1: k is taken); a step of -2 is `i += -2`; `"a" in record` of a
    // key every creation sets is True.
    Run r{std::string(kMore)};
    static const std::pair<std::string_view, std::string_view> expected[] = {
    {"g", R"x(function g(a:i64, b:i64 [default=5], c:py.str [default="x"]) -> i64 {return (a:i64 + b:i64):i64})x"},
    {"use", R"x(function use() -> i64 {return (g(1:i64, 5:i64, 'x':py.str):i64 + g(1:i64, 2:i64, 'x':py.str):i64):i64})x"},
    {"fn_with_lambda", R"x(refused dynamic:first-class lambda)x"},
    {"nested", R"x(refused syntax:nested function)x"},
    {"with_stmt", R"x(refused stdlib:open)x"},
    {"gen", R"x(function gen() -> py.iter<i64> {YieldReturn})x"},
    {"Shape.__init__", R"x(ctor __init__(w:i64) -> void {this:app.Shape.w:i64 = w:i64})x"},
    {"Shape.unit", R"x(method unit() -> i64 {return 1:i64})x"},
    {"Shape.area", R"x(method area() -> i64 {return this:app.Shape.w:i64})x"},
    {"Shape.make", R"x(refused dynamic:decorator classmethod)x"},
    {"use_shape", R"x(function use_shape() -> i64 {var s:app.Shape = new Shape(3:i64):app.Shape; return (s:app.Shape.area():i64 + type:app.Shape.unit():i64):i64})x"},
    {"div_mod", R"x(function div_mod(a:i64, b:i64) -> i64 {return ((a:i64 / b:i64):i64 + (a:i64 % b:i64):i64):i64})x"},
    {"uses_div", R"x(function uses_div() -> i64 {return (div_mod(7:i64, 2:i64):i64 + div_mod(9:i64, 3:i64):i64):i64})x"},
    {"two_loops", R"x(function two_loops() -> i64 {var t:i64 = 0:i64; for (var i:i64 = 0:i64; (i:i64 < 3:i64):bool; (++i:i64):i64) {for (var j:i64 = 0:i64; (j:i64 < 2:i64):bool; (++j:i64):i64) {t:i64 += (i:i64 * j:i64):i64}}; return t:i64})x"},
    {"loop_var_after", R"x(function loop_var_after() -> i64 {var k:i64; for (var k_1:i64 = 0:i64; (k_1:i64 < 3:i64):bool; (++k_1:i64):i64) {k:i64 = k_1:i64}; return k:i64})x"},
    {"while_cond_ok", R"x(function while_cond_ok(n:i64) -> i64 {var c:i64 = 0:i64; while (((c:i64 < n:i64):bool && (c:i64 < 10:i64):bool):bool) {c:i64 += 1:i64}; return c:i64})x"},
    {"call_while", R"x(function call_while() -> i64 {return while_cond_ok(4:i64):i64})x"},
    {"tup", R"x(function tup(x:i64) -> py.tuple<i64, i64> {return py:tuple(x:i64, (x:i64 + 1:i64):i64):py.tuple<i64, i64>})x"},
    {"use_tup", R"x(function use_tup() -> i64 {py:bind(a:i64, b:i64):py.tuple<i64, i64> = tup(1:i64):py.tuple<i64, i64>; return (a:i64 + b:i64):i64})x"},
    {"stepped", R"x(function stepped() -> i64 {var s:i64 = 0:i64; for (var i:i64 = 10:i64; (i:i64 > 0:i64):bool; i:i64 += -2:i64) {s:i64 += i:i64}; return s:i64})x"},
    {"ifexp_none", R"x(function ifexp_none(x:i64) -> py.optional<i64> {var y:py.optional<i64> = None:py.optional<i64>; if ((x:i64 > 0:i64):bool) {y:py.optional<i64> = x:i64}; return y:py.optional<i64>})x"},
    {"use_ifexp", R"x(function use_ifexp() -> py.optional<i64> {return ifexp_none(3:i64):py.optional<i64>})x"},
    {"membership_dict", R"x(function membership_dict() -> i64 {var h:app.MembershipDictRecord = py:record<'a'>(1:i64):app.MembershipDictRecord; if (True:bool) {return 1:i64}; return 0:i64})x"},
    };
    for (const auto& [name, text] : expected) CB_CHECK_EQ(r.text(std::string(name)), std::string(text));
    // the docstring is the function's comment, cleaned
    const ir::MethodDecl* g = ir::get<ir::MethodDecl>(r.lowered.find("app.g")->decl);
    CB_CHECK(g != nullptr);
    CB_CHECK_EQ(g->comments.size(), 1U);
    CB_CHECK_EQ(g->comments[0], "Adds.\n\nTwo things.");
    // a static method says so, a property says so
    CB_CHECK(ir::get<ir::MethodDecl>(r.lowered.find("app.Shape.unit")->decl)->mods == std::vector<std::string>{"static"});
    CB_CHECK(ir::get<ir::MethodDecl>(r.lowered.find("app.Shape.area")->decl)->mods == std::vector<std::string>{"property"});
    // the owner of a method is its class; of a function, its module
    CB_CHECK_EQ(ir::get<ir::MethodDecl>(r.lowered.find("app.Shape.area")->decl)->sym->owner.value_or(""), "app.Shape");
    CB_CHECK_EQ(ir::get<ir::MethodDecl>(r.lowered.find("app.g")->decl)->sym->owner.value_or(""), "app");
    // the lowering of a function is kept with what it found
    CB_CHECK(r.lowered.find("app.use_shape")->lowering->calls().contains("app.Shape.area"));
    CB_CHECK(r.lowered.find("app.use_shape")->lowering->calls().contains("app.Shape.unit"));
});

CB_TEST("translate.python.lower.the_checked_policy_writes_checked_operations", [] {
    const std::string source = R"py(
def mul(a: int, b: int):
    return a * b

def neg(a: int):
    return -a
)py";
    // an integer not shown to fit int64 is refused ...
    Run strict(source);
    CB_CHECK(strict.text("mul").starts_with("refused int-range:"));
    CB_CHECK(strict.text("neg").starts_with("refused int-range:"));
    // ... unless overflow is checked: the operations that may leave int64 raise
    Run checked(source, pr::IntPolicy::Checked);
    CB_CHECK_EQ(checked.text("mul"), "function mul(a:i64, b:i64) -> i64 {return py:checked_mul(a:i64, b:i64):i64}");
    CB_CHECK_EQ(checked.text("neg"), "function neg(a:i64) -> i64 {return py:checked_neg(a:i64):i64}");
});

CB_TEST("translate.python.lower.a_record_is_named_for_what_it_holds_and_who_makes_it", [] {
    Run r(R"py(
def read_header(n):
    h = {"magic": n, "size": 2}
    return h

def get_items(n):
    t = {"k": n}
    return t

def make_thing(n):
    a = {"x": n}
    b = {"y": n}
    return a
)py");
    // the verb the function starts with is taken off, the name made singular; a record that does not leave the function is the function's
    // name and "Record"; two of one name in a module are numbered
    std::set<std::string> names;
    for (const auto& [root, name] : r.context->records().names()) {
        names.insert(r.context->qualified_record(root));
        CB_CHECK_EQ(r.context->records().module(root), "app");
    }
    CB_CHECK((names == std::set<std::string>{"app.Header", "app.Item", "app.Thing", "app.ThingRecord"}));
    // the fields are in the order the record's own display writes them, and say which every creation sets
    for (const auto& [root, name] : r.context->records().names()) {
        if (name != "Header") continue;
        const auto fields = r.context->records().fields(root);
        CB_CHECK_EQ(fields.size(), 2U);
        CB_CHECK_EQ(fields[0].key, "magic");
        CB_CHECK_EQ(fields[1].key, "size");
        CB_CHECK(fields[0].required && fields[1].required);
        CB_CHECK(!r.context->records().is_ref(root));   // never changed after it is built
    }
});

CB_TEST("translate.python.lower.a_refusal_reaches_whoever_calls_the_refused", [] {
    Run r(R"py(
def bad(s):
    return s.upper()

def mid():
    return bad("x")

def top():
    return mid()

def fine():
    return 1
)py");
    CB_CHECK_EQ(r.text("bad"), "refused stdlib:str.upper");
    // (the inference already carries a refusal to the callers through the return value, with the callee's name as the detail)
    CB_CHECK_EQ(r.text("mid"), "refused calls-refused:bad");
    CB_CHECK_EQ(r.text("top"), "refused calls-refused:mid");
    CB_CHECK(r.text("fine").starts_with("function fine"));
    CB_CHECK(r.lowered.find("app.top")->decl == nullptr);   // (the tree of a refused function is not offered)
    CB_CHECK(r.lowered.find("app.top")->lowering != nullptr);
});

CB_TEST("translate.python.lower.a_call_of_a_function_left_out_refuses_the_caller", [] {
    pr::Program program;
    program.add_module("app", "def leaf():\n    return 1\n\ndef caller():\n    return leaf()\n\ndef outer():\n    return caller()\n");
    program.index();
    in::Inference inference(library());
    program.solve(inference);
    lw::Context context(program, library());
    std::vector<pr::Fn*> without_leaf = {program.fns.at("app.caller").get(), program.fns.at("app.outer").get()};
    auto lowered = lw::lower_functions(context, without_leaf);
    CB_CHECK(!lowered.find("app.caller")->is_refused());   // each lowers on its own ...
    CB_CHECK(!lowered.find("app.outer")->is_refused());
    lw::close_refusals(lowered);                           // ... until what they call is looked for
    CB_CHECK_EQ(lowered.find("app.caller")->refused->reason(), "calls-refused:leaf (not in the translation)");
    CB_CHECK_EQ(lowered.find("app.outer")->refused->reason(), "calls-refused:caller (calls-refused:leaf (not in the translation))");
    CB_CHECK(lowered.find("app.caller")->decl == nullptr);
    CB_CHECK(lowered.find("app.leaf") == nullptr);
});

CB_TEST("translate.python.lower.a_module_name_is_lowered_as_a_value", [] {
    // what the package translation does for a module-level name a function reads: lower its value as if inside a function of the module
    pr::Program program;
    program.add_module("app", "LIMIT = 10\nSQUARES = [i * i for i in range(3)]\n");
    program.index();
    in::Inference inference(library());
    program.solve(inference);
    lw::Context context(program, library());
    pr::Module& module = *program.modules.at("app");
    in::ModuleEval evaluation(program, &module, /*record=*/true, library());
    evaluation.run();
    lw::FnLower lowering(context, module, evaluation);
    ir::Node* limit = lowering.value_of(module.assigns.at("LIMIT").front(), lw::i64_type());
    CB_CHECK_EQ(show(limit), "10:i64");
    CB_CHECK(!lowering.has_pending());
    // a comprehension needs its loops before the statement it is in: built into a temporary
    ir::Node* squares = lowering.value_of(module.assigns.at("SQUARES").front(), lw::py_class("py.list", {lw::i64_type()}));
    CB_CHECK_EQ(show(squares), "items:py.list<i64>");
    CB_CHECK(lowering.has_pending());
});
