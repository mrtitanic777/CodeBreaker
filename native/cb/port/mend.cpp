#include "cb/port/mend.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <memory>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/csource.hpp"

namespace cb::port {

using base::ctext::blank;
using csource::CallAt;
using csource::FunctionSpan;
using csource::name_regex;
using csource::SourceIndex;
using csource::Span;
using csource::TagSet;

namespace {

// ---- small helpers over lines -------------------------------------------------------------------------

bool is_word(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_';
}
bool has(std::string_view s, std::string_view part) {
    return s.find(part) != std::string_view::npos;
}

// s[a:b] with Python's clamping: the pieces of a line the passes cut by offsets the compiler gave.
std::string slice(std::string_view s, std::size_t a, std::size_t b) {
    a = std::min(a, s.size());
    b = std::min(std::max(a, b), s.size());
    return std::string(s.substr(a, b - a));
}
std::string head(std::string_view s, std::size_t n) {
    return slice(s, 0, n);
}
std::string from(std::string_view s, std::size_t n) {
    return slice(s, n, s.size());
}
std::string strip(std::string_view s) {
    return std::string(text::trim(s));
}
std::string join_names(const std::vector<std::string>& v) {
    return text::join(v, ", ");
}
bool truthy(const std::optional<std::string>& s) {
    return s.has_value() && !s->empty();
}

// the leading whitespace of s
std::string leading_space(std::string_view s) {
    return std::string(s.substr(0, s.size() - text::ltrim(s).size()));
}

void note(Notes& notes, std::string_view why, std::string where) {
    notes[std::string(why)].push_back(std::move(where));
}

// The positions of the assignment signs of a line (a `=` that is not part of ==, <=, +=, ...), nearest to `col` first.
std::vector<std::size_t> assignment_signs(std::string_view l, std::size_t col) {
    std::vector<std::size_t> out;
    for (std::size_t i = 0; i < l.size(); ++i) {
        if (l[i] != '=')
            continue;
        if (i > 0 && has("=!<>+-*/%&|^", std::string_view(&l[i - 1], 1)))
            continue;
        if (i + 1 < l.size() && l[i + 1] == '=')
            continue;
        out.push_back(i);
    }
    const auto distance = [col](std::size_t p) { return p > col ? p - col : col - p; };
    std::stable_sort(out.begin(), out.end(), [&](std::size_t a, std::size_t b) { return distance(a) < distance(b); });
    return out;
}

// Where the value that starts at `a` ends: at a top-level `;` or `,`, or at the bracket that closes what it is in.
std::size_t value_end(std::string_view l, std::size_t a, bool braces) {
    std::size_t b = a;
    int depth = 0;
    while (b < l.size() && !(depth == 0 && (l[b] == ';' || l[b] == ','))) {
        depth += (l[b] == '(' || l[b] == '[' || (braces && l[b] == '{')) -
                 (l[b] == ')' || l[b] == ']' || (braces && l[b] == '}'));
        if (depth < 0)
            break;
        ++b;
    }
    return b;
}

// the `find("=", from)` of the Python: the index of the first '=' at or after `from`, or npos
std::size_t find_eq(std::string_view l, std::size_t from) {
    return l.find('=', std::min(from, l.size()));
}

// ---- what the compilers say ---------------------------------------------------------------------------

enum class Kind {
    LibName,
    ArgType,
    FnPtr,
    IntPtr,
    Assign,
    RetPtr,
    RetInt,
    RetMix,
    CallPtr,
    Neg,
    Member,
    Void,
    PtrCast,
    Incomplete,
    UnionSet,
    UnionArg,
    SeqPt,
    IntCast,
    Arity,
    Other,
};

struct Complaint {
    Kind kind;
    std::vector<Regex> patterns;
};

// Complaints by kind, in the order they are tried: gcc's words, clang's, MSVC's.
const std::vector<Complaint>& complaints() {
    static const std::vector<Complaint> table = [] {
        std::vector<Complaint> t;
        const auto add = [&t](Kind kind, std::initializer_list<std::string_view> patterns) {
            Complaint c{kind, {}};
            for (std::string_view p : patterns)
                c.patterns.emplace_back(p);
            t.push_back(std::move(c));
        };
        add(Kind::LibName, {R"(conflicting types for '(?P<fn>\w+)')", R"('(?P<fn>\w+)': redefinition; different)"});
        add(Kind::ArgType,
            {R"(passing argument (?P<n>\d+) of '(?P<fn>\w+)' (?P<how>from incompatible pointer type|makes integer from pointer without a cast|makes pointer from integer without a cast))",
             R"(incompatible (?P<how>pointer types|pointer to integer conversion|integer to pointer conversion) passing '(?P<frm>[^']*)'(?: \(aka '[^']*'\))? to parameter of type '(?P<to>[^']*)')",
             R"('function': incompatible types - from '(?P<frm>[^']*)' to '(?P<to>[^']*)')",
             R"('function': '(?P<to>[^']*)' differs in levels of indirection from '(?P<frm>[^']*)')"});
        add(Kind::FnPtr,
            {R"((?:assignment to|initialization of) '(?P<to>[^']*\(\*\)\([^']*\))'(?: \{aka '[^']*'\})? from incompatible pointer type '(?P<frm>[^']*\(\*\)\([^']*\))')",
             R"(incompatible function pointer types assigning to '(?P<to>[^']*)'(?: \(aka '[^']*'\))? from '(?P<frm>[^']*)')",
             R"('=': incompatible types - from '(?P<frm>[^']*\*\)\([^']*\))' to '(?P<to>[^']*)')"});
        add(Kind::IntPtr,
            {R"(assignment to '(?P<to>[^']*)'(?: \{aka '[^']*'\})? from '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? makes integer from pointer)",
             R"(incompatible pointer to integer conversion assigning to '(?P<to>[^']*)'(?: \(aka '[^']*'\))? from '(?P<frm>[^']*\*)')",
             R"('=': '(?P<to>[^'*]*)' differs in levels of indirection from '(?P<frm>[^']*\*)')"});
        add(Kind::Assign,
            {R"((?:assignment to|initialization of) '(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? from incompatible pointer type '(?P<frm>[^']*)')",
             R"(incompatible pointer types (?:assigning to|initializing) '(?P<to>[^']*\*)'(?: \(aka '[^']*'\))? (?:from|with an expression of type) '(?P<frm>[^']*)')",
             R"('(?:=|initializing)': incompatible types - from '(?P<frm>[^']*)' to '(?P<to>[^']*\*)')"});
        add(Kind::RetPtr,
            {R"(returning '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? from a function with return type '(?P<to>[^']*)'(?: \{aka '[^']*'\})? makes integer from pointer)",
             R"(incompatible pointer to integer conversion returning '(?P<frm>[^']*\*)')"});
        add(Kind::RetInt,
            {R"(returning '(?P<frm>[^'*]*)'(?: \{aka '[^']*'\})? from a function with return type '(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? makes pointer from integer)",
             R"(incompatible integer to pointer conversion returning '(?P<frm>[^'*]*)')"});
        add(Kind::RetMix,
            {R"(returning '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? from a function with incompatible return type '(?P<to>[^']*\*)')",
             R"(incompatible pointer types returning '(?P<frm>[^']*\*)')"});
        add(Kind::CallPtr,
            {R"((?:assignment to|initialization of) '(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? from '(?P<frm>[^'*]*)'(?: \{aka '[^']*'\})? makes pointer from integer)",
             R"(incompatible integer to pointer conversion (?:assigning to|initializing) '(?P<to>[^']*\*)')"});
        add(Kind::Neg, {R"(comparison is always (?:false|true) due to limited range of data type)",
                        R"(result of comparison of constant -\d+ with expression of type '(?P<t>[^']*)'.* is always)",
                        R"('(?:==|!=)': signed/unsigned mismatch)"});
        add(Kind::Member,
            {R"(request for member '(?P<m>\w+)' in something not a structure or union)",
             R"('(?:struct|union) \w+' has no member named '(?P<m>\w+)')", R"(no member named '(?P<m>\w+)' in)",
             R"(member reference base type '[^']*' is not a structure or union)",
             R"(left of '->(?P<m>\w+)' must point to struct)", R"(invalid type argument of '->')",
             R"('(?P<m>\w+)' : is not a member of)"});
        add(Kind::Void, {R"(void value not ignored)", R"(requires expression of scalar type \('void' invalid\))",
                         R"(incompatible type 'void')", R"(illegal use of type 'void')", R"('void' illegal)",
                         R"(void expression)", R"(controlling expression has type 'void')"});
        add(Kind::PtrCast, {R"(cast from pointer to integer of different size)",
                            R"(cast to smaller integer type '[^']*' from '[^']*\*')"});
        add(Kind::Incomplete, {R"(type of formal parameter (?P<n>\d+) is incomplete)",
                               R"(argument type '(?P<to>(?:union|struct) \w+)' is incomplete)"});
        add(Kind::UnionSet,
            {R"(incompatible types when assigning to type '(?P<to>union \w+)' from type '(?P<frm>[^']*)')",
             R"(assigning to '(?P<to>union \w+)' from incompatible type '(?P<frm>[^']*)')"});
        add(Kind::UnionArg,
            {R"(incompatible type for argument (?P<n>\d+) of '(?P<fn>\w+)')",
             R"(passing '(?P<frm>[^']*)'(?: \(aka '[^']*'\))? to parameter of incompatible type '(?P<to>union \w+)')"});
        add(Kind::SeqPt, {R"(operation on '(?P<v>\w+)' may be undefined)",
                          R"(unsequenced modification and access to '(?P<v>\w+)')"});
        add(Kind::IntCast,
            {R"(cast to pointer from integer of different size)", R"(cast to '[^']*' from smaller integer type)",
             R"(conversion from '[^']*' to '[^']*' of greater size)"});
        add(Kind::Arity, {R"(too (?:many|few) arguments to function (?:'(?P<fn>\w+)'|call))",
                          R"(too (?:many|few) actual parameters)"});
        return t;
    }();
    return table;
}

// What a complaint is about, and the match that says so (its named parts are what g() reads).
struct Said {
    Kind kind;
    std::optional<RegexMatch> match;
};

std::optional<Said> said(std::string_view text) {
    for (const Complaint& c : complaints()) {
        for (const Regex& p : c.patterns) {
            if (auto m = p.search(text))
                return Said{c.kind, std::move(m)};
        }
    }
    return std::nullopt;
}

// a cast at the start of an expression: (struct s *), (char *), (int (*)(int))
const Regex& cast_prefix() {
    static const Regex re(
        R"(\(\s*(?:(?:struct|union)\s+\w+|\w+)(?:\s+\w+)*\s*\**\s*\)|\(\s*[\w\s]+\**\s*\(\s*\*\s*\)\s*\([^()]*\)\s*\))");
    return re;
}

// The type words of a type as the 8086's compilers meant them: long 32 bits, short 16, and with `int16` every int.
std::string narrow_type(std::string_view type, bool int16) {
    static const Regex pointer_cast_unsigned(R"(\(\s*unsigned(?:\s+int)?\s*(?=\*+\s*\)))");
    static const Regex pointer_cast_int(R"(\(\s*int\s*(?=\*+\s*\)))");
    static const Regex unsigned_long(R"(\bunsigned\s+long(?:\s+int)?\b|\blong\s+unsigned(?:\s+int)?\b)");
    static const Regex long_word(R"(\blong(?:\s+int)?\b)");
    static const Regex unsigned_short(R"(\bunsigned\s+short(?:\s+int)?\b)");
    static const Regex short_word(R"(\bshort(?:\s+int)?\b)");
    static const Regex unsigned_word(R"(\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b)");
    static const Regex int_word(R"((?<!unsigned )(?<!signed )\bint\b)");
    std::vector<csource::Edit> edits;
    const auto substitute = [&](const Regex& rx, std::string_view replacement) {
        rx.for_each_match(type, [&](const RegexMatch& m) {
            const bool overlaps = std::any_of(edits.begin(), edits.end(), [&](const csource::Edit& e) {
                return m.start() < e.end && e.start < m.end();
            });
            if (!overlaps)
                edits.push_back({m.start(), m.end(), std::string(replacement)});
            return true;
        });
    };
    substitute(pointer_cast_unsigned, "(uint16_t ");
    substitute(pointer_cast_int, "(int16_t ");
    substitute(unsigned_long, "uint32_t");
    substitute(long_word, "int32_t");
    substitute(unsigned_short, "uint16_t");
    substitute(short_word, "int16_t");
    if (int16) { // (every int as wide as it was: a store truncates as the 8086's did)
        substitute(unsigned_word, "uint16_t");
        substitute(int_word, "int16_t");
    }
    return csource::apply_edits(type, std::move(edits));
}

// ---- one round ----------------------------------------------------------------------------------------

// The complaint being mended: the line it is on (the text mended so far), where, and what the compiler said.
struct Ctx {
    const Diag& diag;
    const std::string& file;
    std::size_t ln = 0; // 1-based
    std::string& line;
    std::size_t col = 0; // 0-based
    Kind kind = Kind::Other;
    std::optional<RegexMatch> match;
};

// What is left of a complaint after its kind's handler: nothing (the Python's `continue`), or the tail that every
// complaint gets (a chain of assignments of 0, then the kinds that mend a line).
enum class Flow { Done, Tail };

class Mender {
public:
    Mender(MendState& st, Sources& out, const Rules& rules) : st_(st), out_(out), rules_(rules) {}
    bool run(const std::vector<Diag>& diags);

private:
    MendState& st_;
    Sources& out_;
    const Rules& rules_;
    bool changed_ = false;
    csource::StructMembers mem_;
    TagSet tags_;
    std::optional<std::string> all_code_; // every file's code as one text: once a round
    std::map<std::string, std::set<std::string, std::less<>>> visible_;
    std::map<std::string, std::unique_ptr<SourceIndex>> index_;

    // a compiler's tags as the types name them
    const TagSet& tags() const { return tags_; }

    const SourceIndex& index_of(const std::string& file);
    const std::string& all_code();
    const std::set<std::string, std::less<>>& visible(const std::string& file);
    const FunctionSpan* enclosing(const std::string& file, std::size_t ln);
    Function* function(std::string_view name);
    std::optional<std::string> g(const Ctx& c, std::string_view name) const;
    bool any_file_matches(const Regex& rx);

    Flow libname(Ctx& c);
    Flow argtype(Ctx& c);
    Flow fnptr(Ctx& c);
    void assign(Ctx& c);
    void retptr(Ctx& c);
    void retint_retmix(Ctx& c);
    void callptr(Ctx& c);
    void neg(Ctx& c);
    Flow intptr(Ctx& c);
    void ptrcast(Ctx& c);
    void incomplete(Ctx& c);
    void unionset(Ctx& c);
    void unionarg(Ctx& c);
    void seqpt(Ctx& c);
    void intcast(Ctx& c);
    void member(Ctx& c);
    void void_value(Ctx& c);
    void arity(Ctx& c);
    void retype_global(const std::string& var, std::string_view type);
    void mend_function_pointers();
};

const SourceIndex& Mender::index_of(const std::string& file) {
    static const std::string none;
    const std::string* text = out_.find(file);
    const std::string& t = text != nullptr ? *text : none;
    auto& slot = index_[file];
    if (!slot || slot->text() != t)
        slot = std::make_unique<SourceIndex>(t);
    return *slot;
}

const std::string& Mender::all_code() { // (every file's code, as one text: once a round)
    if (!all_code_) {
        std::string all;
        for (const auto& entry : out_) {
            if (entry.second.empty())
                continue;
            if (!all.empty())
                all += '\n';
            all += blank(entry.second);
        }
        all_code_ = std::move(all);
    }
    return *all_code_;
}

const std::set<std::string, std::less<>>& Mender::visible(const std::string& file) { // (once a file a round)
    auto it = visible_.find(file);
    if (it == visible_.end())
        it = visible_.emplace(file, visible_tags(out_, file)).first;
    return it->second;
}

const FunctionSpan* Mender::enclosing(const std::string& file, std::size_t ln) {
    return index_of(file).enclosing(ln);
}

Function* Mender::function(std::string_view name) {
    const auto it = st_.functions.find(std::string(name));
    return it == st_.functions.end() ? nullptr : &it->second;
}

// A named part of the complaint. A name of one C library's own (__p_sig_fn_t) is what it stands for, as the
// compiler says in an {aka '...'}.
std::optional<std::string> Mender::g(const Ctx& c, std::string_view name) const {
    static const Regex library_own(R"(\b__\w+)");
    if (!c.match)
        return std::nullopt;
    const auto v = c.match->named(name);
    if (!v)
        return std::nullopt;
    if (!v->empty() && library_own.test(*v)) {
        if (const auto aka = name_regex(R"('@@' \{aka '([^']*)'\})", *v).search(c.diag.text))
            return std::string(aka->group(1));
    }
    return std::string(*v);
}

bool Mender::any_file_matches(const Regex& rx) {
    for (const auto& entry : out_) {
        if (rx.test(index_of(entry.first).code()))
            return true;
    }
    return false;
}

Flow Mender::libname(Ctx& c) {
    const std::optional<std::string> fn = g(c, "fn");
    if (c.file == "proto.h" && fn && st_.functions.contains(*fn) && !st_.renames.contains(*fn)) {
        st_.renames[*fn] = *fn + "_"; // (proto.h agrees with the definitions: what it meets is the library)
        note(st_.mended, "function named as one of the C library of today: renamed with _ after",
             std::format("{} ({})", st_.functions.at(*fn).file.value_or("None"), *fn));
        changed_ = true;
    }
    return Flow::Tail;
}

// ---- argtype ----

Flow Mender::argtype(Ctx& c) {
    static const Regex integer_pointer_word(R"(\(?\s*\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\))");
    static const Regex expected_to(R"(expected '(?P<to>[^']*)'(?: \{aka '[^']*'\})? but argument is of type)");
    static const Regex expected_aka(R"(expected '[^']*' \{aka '([^']*)'\})");
    static const Regex library_own(R"(\b__\w+)");
    static const Regex variable_path(R"((?:[\w\]\[]+(?:->|\.))*(\w+))");
    static const Regex void_pointer(R"(\s*(?:const\s+)?void\s*\*)");
    static const Regex const_word(R"(\bconst\s+)");
    static const Regex but_argument(R"(but argument is of type '(?P<t>[^']*)')");
    static const Regex but_pointer_argument(R"(but argument is of type '(?P<t>[^']*\*)')");
    static const Regex double_star_end(R"(\*\s*\*\s*$)");
    static const Regex intptr_pointer(R"(intptr_t\s*\*)");
    static const Regex int_pointer(R"((?:u?int16_t|short|int|unsigned)\s*\*)");
    static const Regex address_of(R"(&\s*(\w+))");
    static const Regex int_declaration_start(R"(^(?:register\s+)?(?:u?int16_t|int|unsigned|short)\b)");
    static const Regex plain_argument(R"([\w.\->\[\]]+|\w+\([^()]*\)|&?[\w.\->\[\]]+)");
    static const Regex intptr_word(R"(\bintptr_t\b)");
    std::string& l = c.line;
    const Diag& d = c.diag;

    std::optional<CallAt> at = csource::call_at(l, c.col);
    if (const auto fname = g(c, "fn"); truthy(fname)) { // (gcc names the function and the argument)
        const auto spans = csource::call_args(l, *fname, true);
        const auto n = text::parse_int<long>(g(c, "n").value_or(""));
        if (spans && n && *n >= 1 && static_cast<std::size_t>(*n - 1) < spans->size()) {
            at = CallAt{*fname, static_cast<std::size_t>(*n - 1), (*spans)[static_cast<std::size_t>(*n - 1)]};
        }
    }
    Function* fn = at ? function(at->function) : nullptr;
    if (at && (has(d.text + g(c, "how").value_or(""), "pointer from integer") || has(d.text, "integer to pointer"))) {
        // (a word of 16 bits laid over two bytes, *(int *)&s->sa, given as a pointer: it holds a handle; the pointer)
        const std::string piece = slice(l, at->span.start, at->span.end);
        const std::string arg = strip(piece);
        if (integer_pointer_word.match(arg)) {
            const std::string lead = leading_space(piece);
            l = head(l, at->span.start) + lead + "dos_hp(" + arg + ")" + from(l, at->span.end);
            note(st_.mended,
                 "a word of two bytes given as a pointer (a pointer kept there in two bytes): the handle it holds, the "
                 "pointer",
                 std::format("{}:{}", c.file, c.ln));
            changed_ = true;
            return Flow::Done;
        }
    }
    // the first note that says what was expected (the match of its pattern, or none)
    std::optional<RegexMatch> want;
    for (const std::string& x : d.notes) {
        if (has(x, "expected '")) {
            want = expected_to.search(x);
            break;
        }
    }
    if (!fn && at && want) { // (a parameter of the C library's: what it takes)
        std::string t = const_word.replace_all(csource::ctype(want->named("to").value_or(""), tags()),
                                               ""); // (the variable is written too)
        if (library_own.test(t)) {                  // (the library's own name for it: what it is)
            for (const std::string& x : d.notes) {
                if (!has(x, "aka"))
                    continue;
                if (const auto aka = expected_aka.search(x))
                    t = csource::ctype(aka->group(1), tags());
                break;
            }
        }
        const std::string arg = strip(slice(l, at->span.start, at->span.end));
        std::optional<std::string> v;
        if (const auto m = variable_path.full_match(arg))
            v = std::string(m->group(1));
        if (v && (st_.functions.contains(*v) || st_.asm_protos.contains(*v) || has(t, "(*)")))
            v.reset(); // (a function, or a function's pointer: a cast)
        const FunctionSpan* fn_here = enclosing(c.file, c.ln);
        if (v) {
            const Regex pointer_declared = name_regex(R"(\*\s*@@\s*[;,)=\[])", *v);
            if (!any_file_matches(pointer_declared) || (fn_here && index_of(c.file).declares_local(*fn_here, *v))) {
                v.reset(); // (an int that held a pointer, there: a cast)
            }
        }
        if (v && has(t, "*") && !cast_prefix().match(arg) && !void_pointer.match(t)) {
            // (a variable or member the recovered source typed loosely - the bytes did not say: it is what the
            // library takes, so it is declared one)
            const std::string& name = *v;
            const VariableKey key{"", "", name};
            if (!st_.retyped.contains(key)) {
                st_.retyped[key] = t;
                st_.decl_rules.push_back(
                    {R"((?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:const\s+)?(?:u?int16_t|int|unsigned|char|void|u?int32_t)\s*\*\s*)" +
                         csource::regex_escape(name) + R"(\s*;)",
                     "$1" + t + (t.ends_with('*') ? "" : " ") + name + ";", std::nullopt});
                note(st_.mended,
                     "variable or member given to the C library as its type (the recovered source had it loosely "
                     "typed): declared so",
                     std::format("{}:{} ({}, {})", c.file, c.ln, name, t));
                changed_ = true;
            }
        } else if (!cast_prefix().match(arg)) {
            l = head(l, at->span.start) + (slice(l, at->span.start, at->span.start + 1) == " " ? " " : "") + "(" + t +
                ")" + arg + from(l, at->span.end);
            note(st_.mended, "argument of another type than the C library's parameter: a cast",
                 std::format("{}:{} ({})", c.file, c.ln, at->function));
            changed_ = true;
        }
        return Flow::Done;
    }
    if (!fn || at->index >= fn->params.size())
        return Flow::Done;
    const std::string n = fn->params[at->index].name;
    const std::string decl = fn->params[at->index].type;
    const std::size_t a = at->span.start, b = at->span.end;
    const std::string arg = strip(slice(l, a, b));
    if (arg.starts_with("(intptr_t)") || arg.ends_with("!= 0"))
        return Flow::Done; // (mended already)
    const std::string how = g(c, "how").value_or("");
    const bool derefs = name_regex(R"(\*\s*\+*\s*@@\b|\b@@\s*(?:\+\+|--)?\s*(?:\[|->))", n).test(fn->body);
    const auto set_param = [&](std::size_t k, std::string type) {
        fn->params[k] = {n, std::move(type)};
        st_.param_types[{fn->name, k}] = fn->params[k].type;
    };
    if (has(how, "incompatible pointer") && st_.given_params.contains({fn->name, at->index}) && !has(decl, "void *") &&
        !derefs) {
        // (callers give it pointers of more than one type, as the int it was took them all: any pointer)
        set_param(at->index, "void *" + n);
        note(st_.mended, "parameter the old definition left an int, given pointers of several types: void *",
             std::format("{}:{} ({}, {})", c.file, c.ln, fn->name, n));
        changed_ = true;
        return Flow::Done;
    }
    std::string given_t = g(c, "frm").value_or("");
    if (given_t.empty()) {
        for (const std::string& y : d.notes) {
            if (const auto m = but_argument.search(y)) {
                given_t = std::string(m->named("t").value());
                break;
            }
        }
    }
    const bool word_out = name_regex(R"((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s*\*\s*@@)", n)
                              .full_match(strip(decl))
                              .has_value();
    if (has(how, "incompatible pointer") && word_out &&
        (double_star_end.search(given_t) || intptr_pointer.full_match(strip(given_t)))) {
        // (an int * the old function stored a pointer through - a word was either: a place that holds either whole)
        set_param(at->index, "intptr_t *" + n);
        note(st_.mended, "int * parameter given the place of a pointer (it stores one there): intptr_t *",
             std::format("{}:{} ({}, {})", c.file, c.ln, fn->name, n));
        changed_ = true;
        return Flow::Done;
    }
    if (has(how, "incompatible pointer") && name_regex(R"(intptr_t\s*\*\s*@@)", n).full_match(strip(decl))) {
        if (double_star_end.search(given_t))
            return Flow::Done; // (a pointer's place: an intptr_t's, the same 8 bytes)
        const auto v_ = address_of.full_match(arg);
        const FunctionSpan* fn_at = enclosing(c.file, c.ln);
        if (v_ && fn_at && int_pointer.full_match(strip(given_t)) &&
            index_of(c.file).declares_local(*fn_at, v_->group(1))) {
            st_.local_retypes[{c.file, fn_at->name, std::string(v_->group(1))}] =
                "intptr_t"; // (the word it gives: whole)
            note(st_.mended, "a local's place given for one a pointer is stored in: the local intptr_t",
                 std::format("{}:{} ({}, {})", c.file, c.ln, fn_at->name, v_->group(1)));
            changed_ = true;
            return Flow::Done;
        }
    }
    std::string replacement, why;
    const std::optional<std::string> frm = g(c, "frm"), to = g(c, "to");
    if (has(how, "from pointer") || has(how, "pointer to integer") ||
        (truthy(frm) && has(*frm, "*") && !has(truthy(to) ? *to : "*", "*"))) {
        std::optional<RegexMatch> given;
        for (const std::string& x : d.notes) {
            if (has(x, "but argument is of type")) {
                given = but_pointer_argument.search(x);
                break;
            }
        }
        std::string as_ptr_pattern = R"(\*\s*@@\b|\b@@\s*(?:\[|->)|\b(?:@LIBC@)\s*\(\s*@@\s*[,)])";
        std::string libc;
        for (const std::string& name : st_.libc)
            libc += (libc.empty() ? "" : "|") + csource::regex_escape(name);
        // (with no library function named there is no such use to look for)
        if (libc.empty())
            as_ptr_pattern = R"(\*\s*@@\b|\b@@\s*(?:\[|->))";
        const bool as_ptr = name_regex(text::replace_all(as_ptr_pattern, "@LIBC@", libc), n)
                                .test(fn->body); // (used as a pointer there: deref, index, a library's)
        const bool is_int_param = name_regex(R"((?:register\s+)?(?:int|unsigned|short|int16_t|uint16_t)\s+@@)", n)
                                      .full_match(strip(decl))
                                      .has_value();
        if (given && !csource::used_as_number(fn->body, n) && (fn->implicit.contains(n) || (as_ptr && is_int_param)) &&
            !fn->loose) {
            // (a parameter the old definition never declared - an int by default: what the callers give it)
            const std::string t_ = csource::ctype(given->named("t").value_or(""), tags());
            set_param(at->index, t_ + (t_.ends_with('*') ? "" : " ") + n);
            st_.given_params.insert({fn->name, at->index});
            note(st_.mended, "parameter the old definition left undeclared (an int): the pointer its callers give",
                 std::format("{}:{} ({}, {}: {})", c.file, c.ln, fn->name, n, t_));
            changed_ = true;
            return Flow::Done;
        }
        if (csource::only_tested(fn->body, n)) {
            replacement = arg + " != 0";
            why = "pointer given for a parameter that is only tested: p != 0";
        } else if (intptr_word.search(decl)) {
            replacement = "(intptr_t)(" + arg + ")";
            why = "pointer given for a parameter that holds either: (intptr_t)";
        } else { // (an int that carried a pointer - 16 bits both, then: the parameter holds either whole now)
            std::string widened = decl; // (the type word of the declaration, as intptr_t)
            if (const auto m = int_declaration_start.search(decl)) {
                const std::string word(m->whole());
                const std::size_t last = word.find_last_of(" \t\r\n\v\f");
                widened =
                    (last == std::string::npos ? std::string("intptr_t") : word.substr(0, last + 1) + "intptr_t") +
                    from(decl, m->end());
            }
            set_param(at->index, std::move(widened));
            note(st_.mended, "integer parameter given a pointer (both were 16 bits): intptr_t, to hold either",
                 std::format("{}:{} ({}, {})", c.file, c.ln, fn->name, n));
            changed_ = true;
            return Flow::Done;
        }
    } else {
        std::string t = truthy(to) ? csource::ctype(*to, tags()) : csource::abstract_type(decl, n);
        t = narrow_type(t, st_.int16);
        if (text::replace_all(arg, " ", "").starts_with("(" + text::replace_all(t, " ", "") + ")")) {
            return Flow::Done; // (cast so already: the complaint is another's)
        }
        replacement = "(" + t + ")" + (plain_argument.full_match(arg) ? arg : "(" + arg + ")");
        why = "argument of another type than the parameter (as old C took it): a cast";
    }
    l = head(l, a) + (slice(l, a, a + 1) == " " ? " " : "") + replacement + from(l, b);
    note(st_.mended, why, std::format("{}:{} ({}, argument {})", c.file, c.ln, fn->name, at->index + 1));
    changed_ = true;
    return Flow::Tail;
}

// ---- fnptr: a function put where a pointer to functions of another prototype is ----

Flow Mender::fnptr(Ctx& c) {
    static const Regex assignment_of_name(R"(\s*(\w+)\s*=\s*(\w+)\s*;)");
    static const Regex word(R"(\w+)");
    static const Regex star_parens(R"(\(\s*\*\s*\))");
    std::string& l = c.line;
    const auto x = assignment_of_name.match(l);
    const std::string rest = from(l, c.col);
    const auto at = word.match(rest);
    Function* fn = at ? function(at->whole()) : nullptr;
    const std::string to = g(c, "to").value_or("");
    if (fn && fn->returns_void && !csource::ctype(to).starts_with("void")) {
        fn->returns_void = false; // (kept with functions that give an int: so does it)
        fn->keep_int = true;
        note(st_.mended, "function put where functions giving an int are (a table, a pointer): int",
             std::format("{}:{} ({})", c.file, c.ln, fn->name));
        changed_ = true;
    } else if (at && st_.functions.contains(std::string(at->whole())) && !x && !to.empty() &&
               has(c.diag.text, "initialization")) {
        const std::string t = csource::ctype(to, tags()); // (an element of a table of them: as the table's)
        l = head(l, c.col) + "(" + t + ")" + from(l, c.col);
        note(st_.mended, "function put in a table of functions of another prototype: a cast",
             std::format("{}:{} ({})", c.file, c.ln, at->whole()));
        changed_ = true;
    } else if (x && st_.functions.contains(std::string(x->group(2))) &&
               st_.fptr_mixed.contains(std::string(x->group(1))) && !to.empty()) {
        const std::string t =
            csource::ctype(to, tags()); // (given functions of several prototypes: each made the pointer's own type)
        const std::string function_name(x->group(2));
        l = head(l, x->start(2)) + "(" + t + ")" + from(l, x->start(2));
        note(st_.mended, "pointer given functions of several prototypes: each cast to the pointer's type",
             std::format("{}:{} ({})", c.file, c.ln, function_name));
        changed_ = true;
    } else if (x && st_.functions.contains(std::string(x->group(2)))) {
        std::string t = csource::ctype(g(c, "frm").value_or(""), tags());
        if (!has(t, "*)")) {
            const std::size_t paren = t.find('('); // (clang names the function's type, not its pointer's)
            if (paren != std::string::npos)
                t.replace(paren, 1, "(*)(");
        }
        t = star_parens.replace_all(t, "(*)");
        st_.fptr[std::string(x->group(1))].insert(t);
        changed_ = true;
    } else {
        c.kind = Kind::Assign; // (no function of the program given: a cast)
    }
    return Flow::Tail;
}

// ---- assign: a pointer given to a pointer of another type ----

void Mender::assign(Ctx& c) {
    static const Regex plain_value(R"([\w.\->\[\]]+|\w+\(.*\))");
    std::string& l = c.line;
    const auto signs = assignment_signs(l, c.col);
    if (signs.empty())
        return;
    const std::size_t a = signs[0] + 1;
    std::size_t b = value_end(l, a, true);
    while (b > a && l[b - 1] == ' ')
        --b; // (the value: not the space before its end)
    std::string rhs = strip(slice(l, a, b));
    const std::string t = csource::ctype(g(c, "to").value_or(""), tags());
    auto cast = cast_prefix().match(rhs);
    if (cast && cast->end() < rhs.size() &&
        text::replace_all(head(rhs, cast->end()), " ", "") != "(" + text::replace_all(t, " ", "") + ")") {
        rhs = strip(from(rhs, cast->end())); // (a cast of an earlier round, to another type: replaced)
        cast.reset();
    }
    if (!rhs.empty() && !cast) {
        l = head(l, a) + " (" + t + ")" + (plain_value.full_match(rhs) ? rhs : "(" + rhs + ")") + from(l, b);
        note(st_.mended, "pointer given to a pointer of another type (as old C took it): a cast",
             std::format("{}:{}", c.file, c.ln));
        changed_ = true;
    }
}

// ---- return types ----

void Mender::retptr(Ctx& c) { // (a function of old C's int that gives a pointer)
    static const Regex function_head(R"(^[\w\s*]*?\b(\w+)\s*\([^;]*\)\s*$)");
    const std::vector<std::string> lines = csource::split_lines(out_.at(c.file));
    for (std::size_t i = std::min(c.ln, lines.size()); i-- > 0;) { // (the lines above, from this one up)
        const auto dm = function_head.match(lines[i]);
        if (dm && st_.functions.contains(std::string(dm->group(1)))) {
            Function& fn = st_.functions.at(std::string(dm->group(1)));
            const std::string t = csource::ctype(g(c, "frm").value_or(""), tags());
            if (fn.ret.empty() || fn.ret == "int" || fn.ret == "int16_t") {
                fn.ret = t;
                fn.returns_void = false;
                fn.keep_int = true;
                note(st_.mended, "function of old C's int that gives a pointer: declared to give it",
                     std::format("{} ({}, {})", c.file, fn.name, t));
                changed_ = true;
            }
            break;
        }
    }
}

void Mender::retint_retmix(Ctx& c) { // (a function given a pointer type by its callers that gives an int - an offset;
                                     // or pointers of more than one type)
    const FunctionSpan* fn_at = enclosing(c.file, c.ln);
    Function* fn = fn_at != nullptr ? function(fn_at->name) : nullptr;
    if (fn != nullptr && c.kind == Kind::RetInt && !fn->int_ret) {
        fn->ret = "int";
        fn->returns_void = false;
        fn->keep_int = true;
        fn->int_ret = true;
        fn->ret_set = true;
        note(st_.mended,
             "function that gives an integer though its callers take it for a pointer (16 bits both then): int, the "
             "callers cast",
             std::format("{} ({})", c.file, fn->name));
        changed_ = true;
    } else if (fn != nullptr && c.kind == Kind::RetMix && fn->ret != "void *") {
        fn->ret = "void *";
        fn->returns_void = false;
        fn->keep_int = true;
        fn->ret_set = true;
        note(st_.mended, "function that gives pointers of several types (old C's int): void *",
             std::format("{} ({})", c.file, fn->name));
        changed_ = true;
    }
}

void Mender::callptr(Ctx& c) { // (a pointer from a function declared int)
    static const Regex call_after_sign(R"(\s*\(*\s*(\w+)\s*\()");
    std::string& l = c.line;
    const auto near = assignment_signs(l, c.col);
    std::optional<std::string> name; // (the call right after the = complained of)
    if (!near.empty()) {
        const std::string after = from(l, near[0] + 1);
        if (const auto cm = call_after_sign.match(after))
            name = std::string(cm->group(1));
    }
    const std::string t = csource::ctype(g(c, "to").value_or(""), tags());
    Function* fn = name ? function(*name) : nullptr;
    if (fn != nullptr && fn->int_ret) {
        const std::size_t a = near[0] + 1; // (it gives an integer: what the old pointer was)
        const std::size_t b = value_end(l, a, false);
        const std::string rhs = strip(slice(l, a, b));
        if (!rhs.empty() && !cast_prefix().match(rhs)) {
            l = head(l, a) + " (" + t + ")(intptr_t)(" + rhs + ")" + from(l, b);
            note(st_.mended, "integer given to a pointer (as old C took it): a cast",
                 std::format("{}:{}", c.file, c.ln));
            changed_ = true;
        }
    } else if (fn != nullptr && (fn->ret.empty() || fn->ret == "int" || fn->ret == "int16_t")) {
        fn->ret = t;
        fn->returns_void = false;
        fn->keep_int = true;
        note(st_.mended, "function of old C's int that gives a pointer: declared to give it",
             std::format("{}:{} ({}, {})", c.file, c.ln, *name, t));
        changed_ = true;
    } else if (!name || (fn == nullptr && !st_.asm_protos.contains(*name))) {
        if (!near.empty()) { // (no call: the value cast, as old C took it)
            const std::size_t a = near[0] + 1;
            const std::size_t b = value_end(l, a, false);
            const std::string rhs = strip(slice(l, a, b));
            if (!rhs.empty() && !cast_prefix().match(rhs)) {
                l = head(l, a) + " (" + t + ")(" + rhs + ")" + from(l, b);
                note(st_.mended, "integer given to a pointer (as old C took it): a cast",
                     std::format("{}:{}", c.file, c.ln));
                changed_ = true;
            }
        }
    } else if (name && st_.asm_protos.contains(*name) && !st_.asm_protos.at(*name).starts_with(t)) {
        st_.asm_protos[*name] = t + (t.ends_with('*') ? "" : " ") + *name + "();";
        note(st_.mended, "routine of the assembly language that gives a pointer: declared to give it",
             std::format("{}:{} ({}, {})", c.file, c.ln, *name, t));
        changed_ = true;
    }
}

// x == -1 of an unsigned: 16 bits, the 8086 compared it unsigned (0xFFFF); here the uint16_t is made an int first and
// the test fails - the constant given the operand's type keeps the meaning
void Mender::neg(Ctx& c) {
    static const Regex comparison(
        R"((?P<a>[\w\]\)]+)\s*(?P<op>[=!<>]=|[<>])\s*(?P<k>-\s*\d+)\b|(?P<k2>-\s*\d+)\s*(?P<op2>[=!<>]=|[<>])\s*(?P<b>[\w\[(]+))");
    static const Regex bracket_tail(R"(\[.*)");
    std::string& l = c.line;
    std::optional<std::string> t;
    if (const auto given = g(c, "t"); truthy(given))
        t = csource::ctype(*given);
    struct Found {
        std::size_t start;
        std::string operand, constant;
        std::size_t constant_start, constant_end;
    };
    std::optional<Found> best;
    const auto distance = [&](std::size_t p) { return p > c.col ? p - c.col : c.col - p; };
    comparison.for_each_match(l, [&](const RegexMatch& x) {
        if (!best || distance(x.start()) < distance(best->start)) {
            const bool first_form = x.has(3); // the `k` of the first alternative (its groups: a, op, k)
            Found f;
            f.start = x.start();
            f.operand = std::string(first_form ? x.named("a").value_or("") : x.named("b").value_or(""));
            const std::string_view k = first_form ? x.named("k").value_or("") : x.named("k2").value_or("");
            f.constant = std::string(k);
            f.constant_start = x.start(first_form ? *comparison.group_index("k") : *comparison.group_index("k2"));
            f.constant_end = x.end(first_form ? *comparison.group_index("k") : *comparison.group_index("k2"));
            best = std::move(f);
        }
        return true;
    });
    if (!best)
        return;
    const std::string name = std::string(text::trim(bracket_tail.replace_first(best->operand, ""), "()"));
    std::optional<RegexMatch> decl;
    if (const std::string* source = out_.find(c.file)) {
        decl = name_regex(R"(\b(u?int16_t|uint8_t|unsigned char)\b[^;]*\b@@\b)", name).search(*source);
        if (!t)
            t = decl ? std::string(decl->group(1)) : "uint16_t"; // (a byte's test was always false there too)
    } else if (!t) {
        t = "uint16_t";
    }
    if (*t == "uint16_t" || *t == "unsigned short") {
        l = head(l, best->constant_start) + "(uint16_t)" + text::replace_all(best->constant, " ", "") +
            from(l, best->constant_end);
        note(st_.mended,
             "negative constant compared with a 16-bit unsigned (unsigned there, an int here): (uint16_t)-1",
             std::format("{}:{}", c.file, c.ln));
        changed_ = true;
    }
}

// ---- intptr: a pointer's value given to an integer ----

void Mender::retype_global(const std::string& var, std::string_view type) {
    const std::string escaped = csource::regex_escape(var);
    const std::string spaced = std::string(type) + (type.ends_with('*') ? "" : " ");
    st_.decl_rules.push_back({R"((?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:u?int16_t|int|unsigned)\s+)" +
                                  escaped + R"((\s*=\s*0)?\s*;)",
                              "$1" + spaced + var + "$2;", std::nullopt});
    // (extern int a, v, b;)
    st_.decl_rules.push_back(
        {R"((?m)((?:^|;)\s*(?:extern\s+|static\s+|register\s+)?)(u?int16_t|int|unsigned)\s+((?:\w+\s*,\s*)*))" +
             escaped + R"(\s*((?:,\s*\w+\s*)*);)",
         "", SplitDecl{var, std::string(type)}});
}

Flow Mender::intptr(Ctx& c) {
    static const Regex word_over_bytes(R"(\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\)\s*&[^=]*$)");
    static const Regex leading_assignment(R"(\s*(\w+)\s*=)");
    static const Regex trailing_word(R"((?<![.>])\b(\w+)\s*$)");
    static const Regex member_before_sign(R"((?<![\w.>])(\w+)\s*(?:->|\.)\s*(\w+)\s*$)");
    static const Regex int_member_decl_tail(R"([;,=)\[])");
    std::string& l = c.line;
    const std::string& f = c.file;
    const Diag& d = c.diag;
    const std::size_t from_col = c.col > 0 ? c.col - 1 : 0;

    const std::size_t eq_ = find_eq(l, from_col);
    if (eq_ != std::string::npos && eq_ > 0 && slice(l, eq_ + 1, eq_ + 2) != "=" &&
        word_over_bytes.search(head(l, eq_))) {
        // (*(int *)&s->sa = str: a pointer kept in a word laid over two bytes - its handle there)
        const std::size_t end_ = l.find(';', eq_);
        const std::string rhs_ = end_ != std::string::npos ? strip(slice(l, eq_ + 1, end_)) : "";
        if (!rhs_.empty() && !rhs_.starts_with("dos_ph(")) {
            l = head(l, eq_ + 1) + " dos_ph(" + rhs_ + ")" + from(l, end_);
            note(st_.mended,
                 "a pointer kept in a word of two bytes laid over others (*(int *)&s->sa): its handle (dos_ph)",
                 std::format("{}:{}", f, c.ln));
            changed_ = true;
            return Flow::Done;
        }
    }
    std::optional<std::string> var;
    if (const auto m = leading_assignment.match(l))
        var = std::string(m->group(1));
    const std::string t = csource::ctype(g(c, "frm").value_or(""), tags());
    if (!var) { // (if ((fp = fopen(..)) == 0): the name before the =)
        const std::size_t eqat = find_eq(l, from_col);
        std::optional<std::string> vm;
        if (eqat != std::string::npos && eqat > 0) {
            const std::string before_sign = head(l, eqat);
            if (const auto m = trailing_word.search(before_sign))
                vm = std::string(m->group(1));
        }
        const FunctionSpan* fn_at = enclosing(f, c.ln);
        if (vm && fn_at != nullptr && index_of(f).declares_local(*fn_at, *vm)) {
            var = vm;
        } else if (vm && eqat > 0 && slice(l, eqat + 1, eqat + 2) != "=" &&
                   has(g(c, "to").value_or("") + d.text, "intptr_t") && rules_.on("mend_intptr_assign")) {
            // (if (!(g_0c58 = fopen(..))), g_0c58 made intptr_t already: the value there its intptr_t - to the ')' that
            // closes the assignment)
            int depth = 0;
            std::size_t k_ = eqat + 1;
            while (k_ < l.size()) {
                const char ch = l[k_];
                if (ch == '(' || ch == '[') {
                    ++depth;
                } else if (ch == ')' || ch == ']') {
                    if (depth == 0)
                        break;
                    --depth;
                } else if ((ch == ',' || ch == ';') && depth == 0) {
                    break;
                }
                ++k_;
            }
            const std::string rhs_ = strip(slice(l, eqat + 1, k_));
            if (!rhs_.empty() && !rhs_.starts_with("(intptr_t)")) {
                l = head(l, eqat + 1) + " (intptr_t)(" + rhs_ + ")" + from(l, k_);
                note(st_.mended, "a pointer's value given to an integer (both 16 bits then): (intptr_t)",
                     std::format("{}:{}", f, c.ln));
                changed_ = true;
            }
            return Flow::Done;
        }
    }
    if (!var) { // (a member given a pointer's value - a = p = 0: the number it was, the pointer's bits)
        const std::size_t eq = find_eq(l, from_col);
        std::optional<RegexMatch> mm;
        const std::string before_sign =
            eq != std::string::npos && eq > 0 ? head(l, eq) : std::string(); // (mm points into it)
        if (!before_sign.empty())
            mm = member_before_sign.search(before_sign);
        if (mm && slice(l, eq + 1, eq + 2) != "=") {
            // (x->node = n, int node: the member a pointer's width)
            std::set<std::string> owners;
            name_regex(R"(\bstruct\s+(\w+)\s*\*?\s*@@\s*[;,=)\[])", mm->group(1))
                .for_each_match(index_of(f).code(), [&](const RegexMatch& m) {
                    owners.emplace(m.group(1));
                    return true;
                });
            bool done_ = false;
            if (owners.size() == 1) {
                const std::string& tag = *owners.begin();
                const Regex int_member = name_regex(
                    R"(\bstruct\s+@1@\s*\{[^{}]*?\b((?:u?int16_t|int|short|unsigned))\s+@2@\s*;)", tag, mm->group(2));
                for (auto& [h_, ht] : st_.files) {
                    const auto sm = int_member.search(blank(ht));
                    if (!sm)
                        continue;
                    ht = head(ht, sm->start(1)) + "intptr_t" + from(ht, sm->end(1));
                    if (const std::string* current = out_.find(h_); current != nullptr && !current->empty()) {
                        if (const auto s2 = int_member.search(blank(*current))) {
                            out_[h_] = head(*current, s2->start(1)) + "intptr_t" + from(*current, s2->end(1));
                        }
                    }
                    note(st_.mended, "a member given a pointer (an int then, a pointer's width): intptr_t",
                         std::format("{} (struct {}: {})", h_, tag, mm->group(2)));
                    changed_ = done_ = true;
                }
            }
            if (done_)
                return Flow::Done;
        }
        const std::size_t end = eq != std::string::npos ? l.find(';', eq) : std::string::npos;
        const std::string rhs =
            eq != std::string::npos && end != std::string::npos && end > eq ? strip(slice(l, eq + 1, end)) : "";
        if (!rhs.empty() && !rhs.starts_with("(intptr_t)") && slice(l, eq + 1, eq + 2) != "=") {
            l = head(l, eq + 1) + " (intptr_t)(" + rhs + ")" + from(l, end);
            note(st_.mended, "a pointer's value given to an integer (both 16 bits then): (intptr_t)",
                 std::format("{}:{}", f, c.ln));
            changed_ = true;
        }
        return Flow::Done;
    }
    const std::string& variable = *var;
    const FunctionSpan* fn_at =
        enclosing(f, c.ln); // (the function the complaint is in, and whether the variable is one of its own)
    const bool local = fn_at != nullptr && index_of(f).declares_local(*fn_at, variable);
    const VariableKey key = local ? VariableKey{f, fn_at->name, variable} : VariableKey{"", "", variable};
    const bool wide = has(g(c, "to").value_or(""), "intptr_t") || has(d.text, "long long");
    if (st_.retyped.contains(key) && !wide)
        return Flow::Done;
    std::string scope_text;
    if (local) {
        const auto& code_lines = index_of(f).code_lines();
        for (std::size_t i = fn_at->first; i <= fn_at->last && i < code_lines.size(); ++i) {
            if (i != fn_at->first)
                scope_text += '\n';
            scope_text += code_lines[i];
        }
    }
    const std::string& scope = local ? scope_text : all_code();
    if (wide || csource::used_as_number(scope, variable)) { // (a number too: the pointer's bits given to it, a cast)
        const std::size_t eqat = find_eq(l, from_col);
        if (eqat != std::string::npos && eqat > 0) {
            const std::size_t a = eqat + 1;
            const std::size_t b = value_end(l, a, false);
            const std::string rhs = strip(slice(l, a, b));
            if (!rhs.empty() && !rhs.starts_with("(intptr_t)")) {
                l = head(l, a) + " (intptr_t)(" + rhs + ")" + from(l, b);
                note(st_.mended, "a pointer's value given to an integer (both 16 bits then): (intptr_t)",
                     std::format("{}:{}", f, c.ln));
                changed_ = true;
            }
        }
        return Flow::Done;
    }
    if (!local && !any_file_matches(name_regex(
                      R"((?m)^(?:extern\s+|static\s+)?(?:u?int16_t|int|unsigned)\s+(?:[^;(){}\n]*,\s*)?@@\s*[;,=\[])",
                      variable))) {
        return Flow::Done; // (no variable of the program's by that name: not a guess)
    }
    st_.retyped[key] = t; // (the variable holds a pointer: it is declared one)
    if (local) {          // (there alone: another function's may be a number)
        st_.local_retypes[{f, fn_at->name, variable}] = t;
    } else {
        retype_global(variable, t);
    }
    note(st_.mended, "integer variable that holds a pointer (the 8086's were both 16 bits): declared a pointer",
         std::format("{}:{} ({}, {})", f, c.ln, variable, t));
    changed_ = true;
    return Flow::Done;
}

// ---- the rest of the kinds ----

void Mender::ptrcast(Ctx& c) { // ((int)p: a pointer as a word, 16 bits both then - the whole of it now)
    static const Regex int_cast(R"(\(\s*(?:u?int16_t|int|short|unsigned|unsigned\s+int)\s*\))");
    std::string& l = c.line;
    std::size_t at = 0, length = 0;
    if (const auto x = int_cast.match(from(l, c.col))) {
        at = c.col;
        length = x->end() - x->start();
    } else {
        const std::string before = head(l, c.col + 1);
        std::optional<std::pair<std::size_t, std::size_t>> last;
        int_cast.for_each_match(before, [&](const RegexMatch& m) {
            last = {m.start(), m.end() - m.start()};
            return true;
        });
        if (!last)
            return;
        at = last->first;
        length = last->second;
    }
    l = head(l, at) + "(intptr_t)" + from(l, at + length);
    note(st_.mended, "a pointer cast to an int (both 16 bits then): to intptr_t, the whole of it",
         std::format("{}:{}", c.file, c.ln));
    changed_ = true;
}

void Mender::incomplete(Ctx& c) { // (a structure given by value, its header not here)
    static const Regex tag_in_type(R"(\b(?:struct|union)\s+\w+)");
    const std::string& l = c.line;
    std::string tag = g(c, "to").value_or("");
    if (tag.empty()) {
        const auto at = csource::call_at(l, c.col);
        const Function* fn = at ? function(at->function) : nullptr;
        if (fn != nullptr && at->index < fn->params.size()) {
            if (const auto x = tag_in_type.search(fn->params[at->index].type))
                tag = std::string(x->whole());
        }
    }
    if (tag.empty())
        return;
    std::string pattern = R"(\b)"; // (the tag as the compiler wrote it, its spaces any run of them)
    for (char ch : tag)
        pattern += ch == ' ' ? std::string(R"(\s+)") : csource::regex_escape(std::string(1, ch));
    const Regex defined(pattern + R"(\s*\{)");
    std::vector<std::string> headers;
    for (const auto& entry : out_) {
        if (entry.first.ends_with(".h") && defined.test(index_of(entry.first).code()))
            headers.push_back(entry.first);
    }
    const auto have = st_.need_headers.find(c.file);
    if (headers.size() == 1 && (have == st_.need_headers.end() || !have->second.contains(headers[0]))) {
        st_.need_headers[c.file].insert(headers[0]);
        note(st_.mended, "a structure given by value where its header was not included: included",
             std::format("{}:{} ({})", c.file, c.ln, headers[0]));
        changed_ = true;
    }
}

void Mender::unionset(Ctx& c) { // (u = a member's value: that member of it set)
    std::string& l = c.line;
    const std::string to = g(c, "to").value_or(""), frm = g(c, "frm").value_or("");
    const std::size_t eq = find_eq(l, c.col > 0 ? c.col - 1 : 0);
    if (eq == std::string::npos || slice(l, eq + 1, eq + 2) == "=")
        return;
    const std::size_t a = eq + 1;
    const std::size_t b = value_end(l, a, true);
    const std::string rhs = strip(slice(l, a, b));
    const auto member_name = union_member_for(out_, to, frm);
    if (rhs.empty() || !member_name || rhs.starts_with("(" + to + ")"))
        return;
    l = head(l, a) + " (" + to + "){ ." + *member_name + " = " + rhs + " }" + from(l, b);
    note(st_.mended, "a union given a member's value (old C copied the bytes): the union, that member set",
         std::format("{}:{} (.{})", c.file, c.ln, *member_name));
    changed_ = true;
}

void Mender::unionarg(Ctx& c) { // (a union given by value: old C pushed a member's value, its bytes)
    static const Regex expected_union(R"(expected '(union \w+)')");
    static const Regex argument_type(R"(argument is of type '([^']*)')");
    static const Regex zero(R"(0[lL]?)");
    std::string& l = c.line;
    const Diag& d = c.diag;
    std::string to = g(c, "to").value_or("");
    if (to.empty()) {
        for (const std::string& y : d.notes) {
            if (const auto x = expected_union.search(y)) {
                to = std::string(x->group(1));
                break;
            }
        }
    }
    std::string frm = g(c, "frm").value_or("");
    if (frm.empty()) {
        for (const std::string& y : d.notes) {
            if (const auto x = argument_type.search(y)) {
                frm = std::string(x->group(1));
                break;
            }
        }
    }
    if (to.empty() || frm.empty())
        return;
    std::optional<CallAt> at = csource::call_at(l, c.col);
    if (const auto fname = g(c, "fn"); truthy(fname)) {
        const auto spans = csource::call_args(l, *fname, true);
        const auto n = text::parse_int<long>(g(c, "n").value_or(""));
        if (spans && n && *n >= 1 && static_cast<std::size_t>(*n - 1) < spans->size()) {
            at = CallAt{*fname, static_cast<std::size_t>(*n - 1), (*spans)[static_cast<std::size_t>(*n - 1)]};
        }
    }
    if (!at)
        return;
    const std::size_t a = at->span.start, b = at->span.end;
    const std::string piece = slice(l, a, b);
    const std::string arg = strip(piece);
    if (arg.starts_with("(" + to + ")"))
        return;
    const auto member_name = union_member_for(out_, to, frm);
    const bool is_zero = zero.full_match(arg).has_value();
    if (!member_name && !is_zero)
        return;
    const std::string value = (is_zero && !member_name) || (is_zero && frm == "int")
                                  ? "(" + to + "){ 0 }"
                                  : "(" + to + "){ ." + *member_name + " = " + arg + " }";
    l = head(l, a) + leading_space(piece) + value + from(l, b);
    const std::string union_tag(text::split_ws(to).back());
    if (!visible(c.file).contains(union_tag)) { // (the union, complete where it is made)
        for (const auto& entry : out_) {
            if (entry.first.ends_with(".h") &&
                csource::find_tag_body(index_of(entry.first).code(), "union", union_tag) != std::string::npos) {
                st_.need_headers[c.file].insert(entry.first);
                break;
            }
        }
    }
    note(st_.mended,
         "a union given by value as one of its members' values (old C pushed the bytes): the union, that member set",
         std::format("{}:{} ({}, .{})", c.file, c.ln, at->function, member_name.value_or("None")));
    changed_ = true;
}

// f(op, (n -= 2) == 0, n): the old compiler's order of the arguments, written out
void Mender::seqpt(Ctx& c) {
    static const Regex statement_start(R"((\s*)(return\s+)?)");
    static const Regex control(R"(\s*(?:if|while|for|switch)\b)");
    std::string& l = c.line;
    const std::string v = g(c, "v").value_or("");
    const auto x = name_regex(R"(\(\s*(@@\s*(?:[-+*/%&|^]|<<|>>)=\s*[^()=,;]+|(?:\+\+|--)\s*@@)\s*\))", v).search(l);
    const auto st = statement_start.match(l);
    const std::string_view trimmed = text::rtrim(l);
    if (!x || !st_.arg_order_left || trimmed.ends_with('{') || trimmed.ends_with(')') || control.match(l) ||
        !trimmed.ends_with(';'))
        return;
    // (read before it: an order of its own)
    const std::string between = slice(l, st->end(), x->start());
    for (std::size_t at = between.find(v); at != std::string::npos; at = between.find(v, at + 1)) {
        if ((at == 0 || !is_word(between[at - 1])) &&
            (at + v.size() >= between.size() || !is_word(between[at + v.size()])))
            return;
    }
    const std::string effect(x->group(1));
    const std::string body = head(l, x->start()) + v + from(l, x->end());
    const std::string indent(st->group(1)), ret(st->has(2) ? st->group(2) : "");
    std::string rest(text::rtrim(from(body, st->end())));
    if (!rest.empty())
        rest.pop_back(); // (the ';')
    l = !ret.empty() ? std::format("{}{}({}, {});", indent, ret, effect, rest)
                     : std::format("{}{}, {};", indent, effect, rest);
    note(st_.mended,
         "an argument changes a variable another reads (their order unsaid in C): the old compiler's order, left to "
         "right, written out",
         std::format("{}:{} ({})", c.file, c.ln, v));
    changed_ = true;
}

void Mender::intcast(Ctx& c) { // (an int made a pointer: it held one - 16 bits both)
    static const Regex pointer_cast(
        R"(\(\s*(?:(?:struct|union)\s+\w+|\w+)[\w\s]*\*+\s*\)\s*(?:\(\s*(\w+)\s*\)|(\w+)))");
    static const Regex followed_by_member(R"(\s*(?:\(|->|\.|\[))");
    static const Regex call_word(R"(\b(\w+)\s*\()");
    static const Regex cast_argument(R"((?:\(\s*(?:(?:struct|union)\s+\w+|\w+)[\w\s]*\*+\s*\)\s*)?\(?\s*(\w+)\s*\)?)");
    static const Regex int_param_of(R"((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+(\w+))");
    static const Regex int_param_any(R"((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+\w+)");
    std::string& l = c.line;
    const std::string rest = from(l, c.col);
    const auto x = pointer_cast.match(rest);
    if (!x || followed_by_member.match(from(l, c.col + x->end())))
        return;
    const std::string var(x->has(1) ? x->group(1) : x->group(2));
    const FunctionSpan* fn_at = enclosing(c.file, c.ln);
    Function* fn = fn_at != nullptr ? function(fn_at->name) : nullptr;
    std::optional<std::size_t> k;
    if (fn != nullptr) {
        for (std::size_t i = 0; i < fn->params.size(); ++i) {
            if (fn->params[i].name == var) {
                k = i;
                break;
            }
        }
    }
    if (k) {
        const auto param_is_int = [&](std::size_t i, std::string_view name) {
            const std::string type = strip(fn->params[i].type);
            const auto m = int_param_of.full_match(type);
            return m && m->group(1) == name;
        };
        if (param_is_int(*k, var)) {
            fn->params[*k] = {var, "intptr_t " + var};
            st_.param_types[{fn->name, *k}] = fn->params[*k].type;
            note(st_.mended, "integer parameter made a pointer (it held one: both were 16 bits): intptr_t",
                 std::format("{}:{} ({}, {})", c.file, c.ln, fn->name, var));
            changed_ = true;
        }
        // (the others of the line: f(.., (char *)a) && f(.., b) - one complaint a line - in the same place of a call of
        // the same function)
        std::vector<std::pair<std::string, std::vector<std::string>>> calls;
        call_word.for_each_match(l, [&](const RegexMatch& cm) {
            const std::string name(cm.group(1));
            const auto spans = csource::call_args(std::string_view(l).substr(cm.start()), name);
            std::vector<std::string> args;
            if (spans) {
                for (const Span& s : *spans)
                    args.push_back(strip(slice(l, cm.start() + s.start, cm.start() + s.end)));
            }
            calls.emplace_back(name, std::move(args));
            return true;
        });
        std::set<std::pair<std::string, std::size_t>> places;
        for (const auto& [name, args] : calls) {
            for (std::size_t i = 0; i < args.size(); ++i) {
                const auto m = cast_argument.full_match(args[i]);
                if (m && m->group(1) == var)
                    places.emplace(name, i);
            }
        }
        std::set<std::string> others;
        for (const auto& [name, args] : calls) {
            for (std::size_t i = 0; i < args.size(); ++i) {
                const auto m = cast_argument.full_match(args[i]);
                if (m && places.contains({name, i}))
                    others.emplace(m->group(1));
            }
        }
        for (const std::string& y : others) {
            std::optional<std::size_t> k2;
            for (std::size_t i = 0; i < fn->params.size(); ++i) {
                if (fn->params[i].name == y && fn->params[i].name != var) {
                    k2 = i;
                    break;
                }
            }
            if (k2 && int_param_any.full_match(strip(fn->params[*k2].type))) {
                const std::string pn2 = fn->params[*k2].name;
                fn->params[*k2] = {pn2, "intptr_t " + pn2};
                st_.param_types[{fn->name, *k2}] = fn->params[*k2].type;
                note(st_.mended, "integer parameter made a pointer (it held one: both were 16 bits): intptr_t",
                     std::format("{}:{} ({}, {})", c.file, c.ln, fn->name, pn2));
                changed_ = true;
            }
        }
        return;
    }
    const bool local = fn_at != nullptr && index_of(c.file).declares_local(*fn_at, var);
    const VariableKey key = local ? VariableKey{c.file, fn_at->name, var} : VariableKey{"", "", var};
    if (st_.retyped.contains(key))
        return;
    if (local) {
        st_.retyped[key] = "intptr_t";
        st_.local_retypes[{c.file, fn_at->name, var}] = "intptr_t";
    } else if (any_file_matches(
                   name_regex(R"((?m)^(?:extern\s+|static\s+)?(?:u?int16_t|int|unsigned)\s+@@\s*;)", var))) {
        st_.retyped[key] = "intptr_t";
        st_.decl_rules.push_back({R"((?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:u?int16_t|int|unsigned)\s+)" +
                                      csource::regex_escape(var) + R"(\s*;)",
                                  "$1intptr_t " + var + ";", std::nullopt});
    } else {
        return;
    }
    note(st_.mended, "integer variable made a pointer (it held one: both were 16 bits): intptr_t",
         std::format("{}:{} ({})", c.file, c.ln, var));
    changed_ = true;
}

void Mender::member(Ctx& c) {
    static const Regex arrow(R"(->\s*(\w+))");
    static const Regex tag_definition(R"(\b(?:struct|union)\s+(\w+)\s*\{)");
    std::string& l = c.line;
    const std::string name = g(c, "m").value_or("");
    struct Arrow {
        std::size_t start;
        std::string member;
    };
    std::vector<Arrow> xs;
    arrow.for_each_match(l, [&](const RegexMatch& x) {
        if (name.empty() || x.group(1) == name)
            xs.push_back({x.start(), std::string(x.group(1))});
        return true;
    });
    const auto distance = [&](std::size_t p) { return p > c.col ? p - c.col : c.col - p; };
    std::stable_sort(xs.begin(), xs.end(),
                     [&](const Arrow& a, const Arrow& b) { return distance(a.start) < distance(b.start); });
    if (xs.empty())
        return;
    const auto& seen = visible(c.file); // (of the structures the file sees: the one that has it)
    const Arrow& x = xs.front();
    const std::vector<std::string>* cands = mem_.find(x.member);
    if (cands == nullptr)
        return;
    std::set<std::string> in_headers;
    for (const auto& entry : out_) {
        if (!entry.first.ends_with(".h"))
            continue;
        tag_definition.for_each_match(index_of(entry.first).code(), [&](const RegexMatch& m) {
            in_headers.emplace(m.group(1));
            return true;
        });
    }
    const auto tag_of = [](const std::string& s) { return std::string(text::split_ws(s).back()); };
    std::optional<std::string> s;
    for (const std::string& cand : *cands) {
        if (seen.contains(tag_of(cand))) {
            s = cand;
            break;
        }
    }
    if (!s) { // (a header's: it can be included)
        for (const std::string& cand : *cands) {
            if (in_headers.contains(tag_of(cand))) {
                s = cand;
                break;
            }
        }
    }
    if (!s)
        return;
    const std::size_t o = csource::object_before(l, x.start);
    const std::string obj = strip(slice(l, o, x.start));
    if (obj.starts_with("((" + *s + " *)"))
        return;
    l = head(l, o) + "((" + *s + " *)" + obj + ")" + from(l, x.start);
    note(st_.mended,
         "member of a structure used on another pointer (as old C allowed): a cast to the structure that has it",
         std::format("{}:{} ({})", c.file, c.ln, x.member));
    changed_ = true;
}

void Mender::void_value(Ctx& c) {
    static const Regex call_word(R"(\b(\w+)\s*\()");
    call_word.for_each_match(c.line, [&](const RegexMatch& x) {
        Function* fn = function(x.group(1));
        if (fn != nullptr && fn->returns_void) {
            fn->returns_void = false;
            fn->keep_int = true;
            note(st_.mended, "function whose value is used after all: int (it was what the register held)",
                 std::format("{}:{} ({})", c.file, c.ln, fn->name));
            changed_ = true;
        }
        return true;
    });
}

void Mender::arity(Ctx& c) {
    static const Regex call_word(R"(\b(\w+)\s*\()");
    const std::string& l = c.line;
    std::vector<std::string> names;
    if (const auto fname = g(c, "fn"); truthy(fname)) {
        names.push_back(*fname);
    } else {
        call_word.for_each_match(l, [&](const RegexMatch& x) {
            names.emplace_back(x.group(1));
            return true;
        });
    }
    for (const std::string& n : names) {
        Function* fn = function(n);
        if (fn == nullptr || fn->loose || fn->varargs)
            continue;
        const auto sp = csource::call_args(l, n);
        if (!sp)
            continue;
        std::size_t given = 0;
        for (const Span& s : *sp)
            given += !strip(slice(l, s.start, s.end)).empty();
        if (given != fn->params.size()) {
            fn->loose = true;
            note(st_.mended,
                 "function called with other numbers of arguments than it has: old-style definition kept (its calls "
                 "unchecked, as they were)",
                 std::format("{}:{} ({})", c.file, c.ln, n));
            changed_ = true;
        }
    }
}

// ---- the round ----------------------------------------------------------------------------------------

void Mender::mend_function_pointers() {
    for (const auto& [var, types] : st_.fptr) { // (a pointer to functions of one prototype: given it)
        if (types.size() > 1 && !st_.fptr_mixed.contains(var)) {
            st_.fptr_mixed.insert(var);
            changed_ = true;
        }
        if (types.size() == 1 && !st_.fptr_done.contains(var)) {
            std::string t = *types.begin();
            const std::size_t at = t.find("(*)");
            if (at != std::string::npos)
                t.replace(at, 3, "(*" + var + ")");
            st_.decl_rules.push_back({R"(\b(?:unsigned\s+|signed\s+)?\w+\s*\**\s*\(\s*\*\s*)" +
                                          csource::regex_escape(var) + R"(\s*\)\s*\(\s*\))",
                                      text::replace_all(t, "$", "$$"), std::nullopt});
            st_.fptr_done.insert(var);
            note(st_.mended, "pointer to functions declared with the prototype of those it is given",
                 std::format("{} ({})", var, *types.begin()));
            changed_ = true;
        }
    }
}

bool Mender::run(const std::vector<Diag>& diags) {
    static const Regex chain(R"((\s*)((?:[\w.\->\[\]]+\s*=\s*){2,})(0|\(\w[\w\s*]*\)\s*0)\s*;(.*))");
    changed_ = passed_on(st_, out_);
    if (stored_wide(st_, out_, rules_))
        changed_ = true;
    std::vector<std::string_view> texts;
    for (const auto& entry : out_)
        texts.push_back(entry.second);
    mem_ = csource::members(texts);
    for (const auto& [member_name, owners] : mem_) {
        for (const std::string& s : owners)
            tags_.emplace(text::split_ws(s).back());
    }

    // the complaints by line, in the order the lines are first met
    std::vector<std::pair<std::pair<std::string, std::size_t>, std::vector<const Diag*>>> by_line;
    std::map<std::pair<std::string, std::size_t>, std::size_t> slot;
    for (const Diag& d : diags) {
        const auto key = std::make_pair(d.file, d.line);
        const auto [it, fresh] = slot.emplace(key, by_line.size());
        if (fresh)
            by_line.push_back({key, {}});
        by_line[it->second].second.push_back(&d);
    }
    for (auto& [key, ds] : by_line) {
        const auto& [f, ln] = key;
        const std::string* source = out_.find(f);
        if (source == nullptr || ln == 0)
            continue;
        const std::vector<std::string> lines = csource::split_lines(*source);
        if (ln > lines.size())
            continue;
        std::string l = lines[ln - 1];
        const std::string orig = l;
        // (right to left: a mend moves what follows it)
        std::stable_sort(ds.begin(), ds.end(),
                         [](const Diag* a, const Diag* b) { return a->col.value_or(0) > b->col.value_or(0); });
        for (const Diag* dp : ds) {
            const Diag& d = *dp;
            Ctx c{d, f, ln, l, std::max<std::size_t>(d.col.value_or(1), 1) - 1, Kind::Other, std::nullopt};
            if (auto s = said(d.text)) {
                c.kind = s->kind;
                c.match = std::move(s->match);
            } else if (has(d.text, "incompatible")) {
                c.kind = Kind::Other; // (only the a = b = 0 below may mend it)
            } else {
                continue;
            }
            Flow flow = Flow::Tail;
            switch (c.kind) {
            case Kind::LibName:
                flow = libname(c);
                break;
            case Kind::ArgType:
                flow = argtype(c);
                break;
            case Kind::FnPtr:
                flow = fnptr(c);
                break;
            default:
                break;
            }
            if (flow == Flow::Done)
                continue;
            if (const auto m = chain.full_match(l);
                m && (c.kind == Kind::Assign || c.kind == Kind::IntPtr || c.kind == Kind::CallPtr ||
                      c.kind == Kind::FnPtr || has(d.text, "incompatible"))) {
                std::vector<std::string> mended_parts;
                for (std::string_view x : text::split(m->group(2), '=')) {
                    if (!text::trim(x).empty())
                        mended_parts.push_back(strip(x) + " = 0");
                }
                l = std::string(m->group(1)) + join_names(mended_parts) + ";" + std::string(m->group(4));
                note(st_.mended, "0 given to several of different types at once (a = b = 0): to each",
                     std::format("{}:{}", f, ln));
                changed_ = true;
                c.kind = Kind::Other;
            }
            switch (c.kind) {
            case Kind::Assign:
                assign(c);
                break;
            case Kind::RetPtr:
                retptr(c);
                break;
            case Kind::RetInt:
            case Kind::RetMix:
                retint_retmix(c);
                break;
            case Kind::CallPtr:
                callptr(c);
                break;
            case Kind::Neg:
                neg(c);
                break;
            case Kind::IntPtr:
                intptr(c);
                break;
            case Kind::PtrCast:
                ptrcast(c);
                break;
            case Kind::Incomplete:
                incomplete(c);
                break;
            case Kind::UnionSet:
                unionset(c);
                break;
            case Kind::UnionArg:
                unionarg(c);
                break;
            case Kind::SeqPt:
                seqpt(c);
                break;
            case Kind::IntCast:
                intcast(c);
                break;
            case Kind::Member:
                member(c);
                break;
            case Kind::Void:
                void_value(c);
                break;
            case Kind::Arity:
                arity(c);
                break;
            default:
                break;
            }
        }
        if (l != orig) { // (kept as a rule: the next round's text has the line)
            const auto by_file = st_.line_origin.find(f);
            const LineOrigin* origin = nullptr;
            if (by_file != st_.line_origin.end()) {
                if (const auto at = by_file->second.find(ln); at != by_file->second.end())
                    origin = &at->second;
            }
            st_.line_fixes[origin != nullptr ? LineFixKey{f, origin->text, origin->n} : LineFixKey{f, orig, -1}] = l;
        }
    }
    mend_function_pointers();
    return changed_;
}

} // namespace

bool mend(MendState& state, Sources& out, const std::vector<Diag>& diags, const Rules& rules) {
    return Mender(state, out, rules).run(diags);
}

} // namespace cb::port
