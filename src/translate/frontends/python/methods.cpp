#include "translate/frontends/python/methods.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>

#include "core/text.hpp"
#include "translate/frontends/python/infer_util.hpp"
#include "translate/frontends/python/pyast.hpp"
#include "translate/frontends/python/structfmt.hpp"

namespace cb::translate::frontends::python::methods {

namespace {

using infer::Args;
using infer::Eval;
using infer::IssueClass;
using infer::Keyword;
using infer::Kw;
using infer::Res;
using infer::ResKind;
using infer::State;
using infer::TR;
namespace P = pytypes;
namespace syntax = ::pyfront;
using P::Bound;
using P::Kind;
using P::Type;
using program::Container;
using program::SiteId;
using program::Table;

// ---- results ----------------------------------------------------------------------------------------------------------

TR refuse(Eval& ev, const syntax::Call* n, IssueClass cls, std::string detail) { return TR(ev.issue(n, cls, std::move(detail))); }
TR bmethod(const Type& t, std::string name) { return TR(t, Res::of(ResKind::BMethod, std::move(name))); }
TR builtin_res(const Type& t, std::string name) { return TR(t, Res::of(ResKind::Builtin, std::move(name))); }
TR std_res(const Type& t, std::string name, std::string format = {}) { return TR(t, Res::of(ResKind::Std, std::move(name), std::move(format))); }

Type new_list(Eval& ev, const syntax::Call* n, const Type& item) {
    const SiteId s = ev.p.new_site(n, Container::List, ev.where(n));
    ev.p.grow(Table::Elem, s, item);
    return infer::site_type(Kind::List, s);
}

// ---- what a call's arguments are --------------------------------------------------------------------------------------------

// A keyword's value node, nullptr when the call has no such keyword.
const syntax::Expr* keyword_value(const syntax::Call* n, std::string_view name) {
    for (const syntax::keyword* k : n->keywords)
        if (k->has_arg && k->arg == name)
            return k->value;
    return nullptr;
}

// Every keyword of the call is one of `names` (a `**spread` is none).
bool keywords_within(const Kw& kw, std::initializer_list<std::string_view> names) {
    return std::ranges::all_of(kw, [&](const Keyword& k) { return k.name && infer::word_in(*k.name, names); });
}

// Whether a kind is one of the words of `want` (`|` between them; a trailing `?` says the argument may be left out). A bool
// is an int; bytes take a bytearray; a bottom or top fits anything.
bool kind_fits(const std::optional<Type>& t, std::string_view want) {
    if (!t)
        return false;
    if (infer::is_one_of(*t, {Kind::Bottom, Kind::Top}))
        return true;
    const Kind k = t->kind == Kind::Bool ? Kind::Int : t->kind;
    if (want.ends_with('?'))
        want.remove_suffix(1);
    for (const std::string_view w : text::split(want, '|'))
        if (w == P::kind_word(k) || (w == "bytes" && k == Kind::Bytearray))
            return true;
    return false;
}

bool is_kind(const std::optional<Type>& t, std::initializer_list<Kind> kinds) { return t && infer::is_one_of(*t, kinds); }

std::string kind_of_arg(const std::optional<Type>& t) { return t ? infer::kind_text(*t) : "lambda"; }

// A lambda or a generator as an argument where the call does not take one: the refusal, else nothing.
std::optional<TR> lazy_argument(Eval& ev, const syntax::Call* n, const Args& args, const Kw& kw, bool allowed) {
    if (allowed)
        return std::nullopt;
    const bool lazy = std::ranges::any_of(args, [](const auto& a) { return !a; }) || std::ranges::any_of(kw, [](const Keyword& k) { return !k.type; });
    if (lazy)
        return refuse(ev, n, IssueClass::Dynamic, "a lambda or generator as an argument");
    return std::nullopt;
}

// A str as Python's repr writes it, for a message ("'rb'"); nullopt is None.
std::string repr_text(const std::optional<std::string>& s) {
    if (!s)
        return "None";
    std::string out = "'";
    for (const char c : *s) {
        if (c == '\\' || c == '\'')
            out += '\\';
        out += c;
    }
    return out + "'";
}

// The type of the items of a list or set type.
Type element_of(Eval& ev, const Type& t) { return ev.p.root(*t.site).elem; }

// The item type of what a call iterates: an argument typed in place, or a generator argument typed by gen_elem.
std::optional<Type> item_of_iterable(Eval& ev, const syntax::Call* n, const std::optional<Type>& arg, const State& state) {
    if (arg)
        return ev.iteration(n->args[0], *arg).first;
    if (!n->args.empty())
        if (const auto* g = syntax::as<syntax::GeneratorExp>(n->args[0]))
            return ev.gen_elem(g, state).first;
    return std::nullopt;
}

const Type& optional_arg(const Args& args, std::size_t i, const Type& fallback) { return i < args.size() && args[i] ? *args[i] : fallback; }

// ---- the simple signatures: name -> (argument kinds, result) ------------------------------------------------------------------------

struct Signature {
    std::vector<std::string> arguments;   // "str", "int?" (may be left out), "str|tuple" ...
    Type result;
};

const std::unordered_map<std::string, Signature>& simple_table() {
    static const std::unordered_map<std::string, Signature> table = [] {
        std::unordered_map<std::string, Signature> t;
        const Type s = P::str(), b = P::boolean(), by = P::bytes(), sz = P::size_type(), fd = P::find_type();
        const auto add = [&](std::initializer_list<const char*> names, std::vector<std::string> arguments, const Type& result) {
            for (const char* name : names)
                t.insert_or_assign(name, Signature{arguments, result});
        };
        add({"str.upper", "str.lower", "str.title", "str.capitalize"}, {}, s);
        add({"str.strip", "str.lstrip", "str.rstrip"}, {"str?"}, s);
        add({"str.startswith", "str.endswith"}, {"str|tuple"}, b);
        add({"str.find", "str.rfind"}, {"str", "int?", "int?"}, fd);
        add({"str.index", "str.rindex", "str.count"}, {"str"}, sz);
        add({"str.replace"}, {"str", "str"}, s);
        add({"str.isdigit", "str.isalpha", "str.isalnum", "str.isspace", "str.isupper", "str.islower", "str.isidentifier", "str.isprintable",
             "str.isascii"},
            {}, b);
        add({"str.ljust", "str.rjust", "str.center"}, {"int", "str?"}, s);
        add({"str.zfill"}, {"int"}, s);
        add({"str.removeprefix", "str.removesuffix"}, {"str"}, s);
        add({"str.encode"}, {"str?", "str?"}, by);
        add({"bytes.upper", "bytes.lower"}, {}, by);
        add({"bytes.strip", "bytes.lstrip", "bytes.rstrip"}, {"bytes?"}, by);
        add({"bytes.startswith", "bytes.endswith"}, {"bytes|tuple"}, b);
        add({"bytes.find", "bytes.rfind"}, {"bytes|int", "int?", "int?"}, fd);
        add({"bytes.index"}, {"bytes|int", "int?", "int?"}, sz);
        add({"bytes.count"}, {"bytes|int"}, sz);
        add({"bytes.replace"}, {"bytes", "bytes"}, by);
        add({"bytes.isalnum", "bytes.isalpha", "bytes.isdigit", "bytes.isspace", "bytes.isupper", "bytes.islower"}, {}, b);
        add({"bytes.hex"}, {}, s);
        add({"bytes.ljust", "bytes.rjust"}, {"int", "bytes?"}, by);
        add({"bytes.decode"}, {"str?", "str?"}, s);
        return t;
    }();
    return table;
}

constexpr std::string_view kEncodings[] = {"latin1", "latin-1", "iso-8859-1", "ascii", "utf-8", "utf8", "cp437", "cp1252"};
constexpr std::string_view kDecodeErrors[] = {"strict", "replace", "ignore"};

bool one_of(std::string_view s, std::span<const std::string_view> words) { return std::ranges::find(words, s) != words.end(); }

// The type of a call of a method of a builtin type whose signature is in the table: the signature's result, or the refusal.
Type simple_call(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args, const Kw& kw) {
    const Signature& sig = simple_table().at(name);
    const bool codec = name == "bytes.decode" || name == "str.encode";
    if (!kw.empty() && !(codec && keywords_within(kw, {"errors", "encoding"})))
        return ev.issue(n, IssueClass::Syntax, "keyword argument to " + name);
    if (args.size() > sig.arguments.size())
        return ev.issue(n, IssueClass::Conflict, "too many arguments to " + name);
    for (std::size_t i = 0; i < sig.arguments.size(); ++i) {
        if (i >= args.size()) {
            if (!sig.arguments[i].ends_with('?'))
                return ev.issue(n, IssueClass::Conflict, "missing argument to " + name);
            continue;
        }
        if (!kind_fits(args[i], sig.arguments[i]))
            return ev.issue(n, IssueClass::Conflict, std::format("{} of a {}", name, kind_of_arg(args[i])));
    }
    if (codec) {
        // the constant strings of the call: the positional arguments, then the keywords' values
        std::vector<std::optional<std::string>> consts;
        for (const syntax::Expr* a : n->args)
            consts.push_back(pyast::string_constant(a));
        for (const syntax::keyword* k : n->keywords)
            consts.push_back(pyast::string_constant(k->value));
        const bool has_encoding = !n->args.empty() || keyword_value(n, "encoding") != nullptr;
        const std::optional<std::string> encoding = has_encoding ? consts.front() : std::optional<std::string>("utf-8");
        if (!encoding || !one_of(text::lower(*encoding), kEncodings))
            return ev.issue(n, IssueClass::Stdlib, std::format("{}({})", name, repr_text(encoding)));
        const bool bad_errors = std::any_of(consts.begin() + 1, consts.end(), [](const auto& e) { return !e || !one_of(*e, kDecodeErrors); });
        if (bad_errors) {
            std::string shown;
            for (auto it = consts.begin() + 1; it != consts.end(); ++it)
                shown += (shown.empty() ? "" : ", ") + repr_text(*it);
            return ev.issue(n, IssueClass::Stdlib, std::format("{} errors=[{}]", name, shown));
        }
    }
    if ((name.ends_with(".startswith") || name.ends_with(".endswith")) && !args.empty() && args[0]->kind == Kind::Tuple) {
        const std::string receiver = name.substr(0, name.find('.'));
        for (const Type& a : args[0]->args)
            if (!kind_fits(a, receiver))
                return ev.issue(n, IssueClass::Conflict, std::format("{} of a tuple holding {}", name, infer::kind_text(a)));
    }
    return sig.result;
}

// ---- sorting, int.from_bytes ---------------------------------------------------------------------------------------------------

// sorted(...) and list.sort(...): the keywords are key and reverse; what is sorted by has to be an orderable kind.
TR sort_call(Eval& ev, const syntax::Call* n, const Type& item, const Kw& kw, const State& state, const std::string& name, const Type& result) {
    for (const Keyword& k : kw)
        if (!k.name || !infer::word_in(*k.name, {"key", "reverse"}))
            return refuse(ev, n, IssueClass::Syntax, std::format("{}({}=)", name, k.name.value_or("**")));
    Type by = item;
    if (const syntax::Expr* key = keyword_value(n, "key"))
        by = ev.lambda_type(key, {item}, state);
    if (!infer::is_one_of(by, {Kind::Int, Kind::Str, Kind::Float, Kind::Bytes, Kind::Tuple, Kind::Bottom, Kind::Top, Kind::Bool}))
        return refuse(ev, n, IssueClass::Conflict, "sorting by a " + infer::kind_text(by));
    return name == "sorted" ? builtin_res(result, "sorted") : bmethod(result, name);
}

TR int_from_bytes(Eval& ev, const syntax::Call* n, const Args& args, const Kw& kw) {
    if (args.size() != 1 && args.size() != 2)
        return refuse(ev, n, IssueClass::Conflict, "int.from_bytes arity");
    const std::optional<Type>& b = args[0];
    std::optional<std::string> order;
    if (n->args.size() > 1)
        order = pyast::string_constant(n->args[1]);
    else if (const syntax::Expr* given = keyword_value(n, "byteorder"))
        order = pyast::string_constant(given);
    else
        order = "big";
    if (order != "little" && order != "big")
        return refuse(ev, n, IssueClass::Dynamic, "int.from_bytes byte order");
    (void)kw;
    bool is_signed = false;
    if (const syntax::Expr* flag = keyword_value(n, "signed")) {
        const auto* c = syntax::as<syntax::Constant>(flag);
        if (!c || (c->value.kind != syntax::ConstValue::Kind::True && c->value.kind != syntax::ConstValue::Kind::False))
            return refuse(ev, n, IssueClass::Dynamic, "int.from_bytes signed=<computed>");
        is_signed = c->value.kind == syntax::ConstValue::Kind::True;
    }
    if (!is_kind(b, {Kind::Bytes, Kind::Bytearray, Kind::Bottom, Kind::Top}))
        return refuse(ev, n, IssueClass::Conflict, "int.from_bytes of a " + kind_of_arg(b));
    if (is_kind(b, {Kind::Bottom, Kind::Top}))
        return TR(*b);
    if (!b->hi)
        return std_res(P::int_range(is_signed ? Bound() : Bound(Wide(0)), Bound()), "int.from_bytes");
    const std::optional<std::uint64_t> bytes = (*b->hi).to_u64();
    if (!bytes || *bytes > (1u << 20))
        return std_res(P::int_range(is_signed ? Bound() : Bound(Wide(0)), Bound()), "int.from_bytes");
    const unsigned bits = static_cast<unsigned>(8 * *bytes);
    return std_res(is_signed && bits != 0 ? P::signed_int(bits) : P::unsigned_int(bits), "int.from_bytes");
}

// ---- the methods of the builtin types --------------------------------------------------------------------------------------------

TR list_method(Eval& ev, const syntax::Call* n, const Type& recv, const std::string& attr, const Args& args, const Kw& kw, const State& state) {
    const SiteId s = *recv.site;
    const Type e = element_of(ev, recv);
    if (attr == "append") {
        if (args.size() != 1 || !args[0])
            return refuse(ev, n, IssueClass::Conflict, "list.append arity");
        ev.p.grow(Table::Elem, s, ev.fit(n, *args[0], "list item"));
        return bmethod(P::none(), "list.append");
    }
    if (attr == "extend") {
        const std::optional<Type> item = item_of_iterable(ev, n, args.empty() ? std::nullopt : args[0], state);
        ev.p.grow(Table::Elem, s, item.value_or(P::bottom()));
        return bmethod(P::none(), "list.extend");
    }
    if (attr == "insert") {
        if (args.size() < 2 || !args[1])
            return refuse(ev, n, IssueClass::Conflict, "list.insert arity");
        ev.p.grow(Table::Elem, s, *args[1]);
        return bmethod(P::none(), "list.insert");
    }
    if (attr == "pop")
        return bmethod(e, "list.pop");
    if (attr == "index" || attr == "count")
        return bmethod(P::size_type(), "list." + attr);
    if (attr == "remove")
        return bmethod(P::none(), "list.remove");
    if (attr == "reverse" || attr == "clear")
        return bmethod(P::none(), "list." + attr);
    if (attr == "copy")
        return bmethod(new_list(ev, n, e), "list.copy");
    if (attr == "sort")
        return sort_call(ev, n, e, kw, state, "list.sort", P::none());
    return refuse(ev, n, IssueClass::Stdlib, "list." + attr);
}

TR dict_method(Eval& ev, const syntax::Call* n, const Type& recv, const std::string& attr, const Args& args) {
    const SiteId s = ev.p.find(*recv.site);
    const std::optional<std::string> key = n->args.empty() ? std::nullopt : pyast::string_constant(n->args[0]);
    if (attr == "get") {
        const Type dflt = optional_arg(args, 1, P::none());
        if (key && !ev.p.site(s).dynamic) {
            const Type* field = ev.p.site(s).fields.find(*key);
            const Type joined = ev.p.join(field ? *field : P::bottom(), dflt);
            if (joined.kind == Kind::Top)
                return refuse(ev, n, IssueClass::Conflict, "dict.get default: " + joined.why.value_or(""));
            return bmethod(joined, "record.get");
        }
        if (!key)
            ev.p.make_dynamic(s, "get of a computed key");
        if (!args.empty() && args[0])
            ev.p.grow(Table::Keys, s, *args[0]);
        const Type joined = ev.p.join(ev.p.site(s).elem, dflt);
        if (joined.kind == Kind::Top)
            return refuse(ev, n, IssueClass::Conflict, "dict.get default: " + joined.why.value_or(""));
        return bmethod(joined, "dict.get");
    }
    ev.p.make_dynamic(s, "dict." + attr);
    if (attr == "setdefault") {
        if (args.size() != 2 || !args[0] || !args[1])
            return refuse(ev, n, IssueClass::Conflict, "dict.setdefault arity");
        ev.p.grow(Table::Keys, s, *args[0]);
        ev.p.grow(Table::Elem, s, *args[1]);
        return bmethod(ev.p.site(s).elem, "dict.setdefault");
    }
    if (attr == "items")
        return bmethod(P::iter_of(P::tuple_of({ev.p.site(s).keys, ev.p.site(s).elem}), P::mem()), "dict.items");
    if (attr == "keys")
        return bmethod(P::iter_of(ev.p.site(s).keys, P::mem()), "dict.keys");
    if (attr == "values")
        return bmethod(P::iter_of(ev.p.site(s).elem, P::mem()), "dict.values");
    if (attr == "pop") {
        if (!args.empty() && args[0])
            ev.p.grow(Table::Keys, s, *args[0]);
        const Type r = args.size() < 2 || !args[1] ? ev.p.site(s).elem : ev.p.join(ev.p.site(s).elem, *args[1]);
        return bmethod(r, "dict.pop");
    }
    if (attr == "update") {
        if (args.size() == 1 && args[0] && args[0]->kind == Kind::Dict) {
            const SiteId other = ev.p.find(*args[0]->site);
            ev.p.make_dynamic(other, "dict.update");
            const Type keys = ev.p.site(other).keys, elems = ev.p.site(other).elem;
            ev.p.grow(Table::Keys, s, keys);
            ev.p.grow(Table::Elem, s, elems);
            return bmethod(P::none(), "dict.update");
        }
        return refuse(ev, n, IssueClass::Syntax, "dict.update arguments");
    }
    if (attr == "copy") {
        const SiteId copy = ev.p.new_site(n, Container::Dict, ev.where(n));
        ev.p.make_dynamic(copy, "dict.copy");
        const Type keys = ev.p.site(s).keys, elems = ev.p.site(s).elem;
        ev.p.grow(Table::Keys, copy, keys);
        ev.p.grow(Table::Elem, copy, elems);
        return bmethod(infer::site_type(Kind::Dict, copy), "dict.copy");
    }
    if (attr == "clear")
        return bmethod(P::none(), "dict.clear");
    return refuse(ev, n, IssueClass::Stdlib, "dict." + attr);
}

TR set_method(Eval& ev, const syntax::Call* n, const Type& recv, const std::string& attr, const Args& args, const State& state) {
    const SiteId s = *recv.site;
    if (attr == "add") {
        if (args.empty() || !args[0])
            return refuse(ev, n, IssueClass::Conflict, "set.add arity");
        ev.p.grow(Table::Elem, s, *args[0]);
        return bmethod(P::none(), "set.add");
    }
    if (attr == "discard" || attr == "remove")
        return bmethod(P::none(), "set." + attr);
    if (attr == "update") {
        const std::optional<Type> item = args.empty() || !args[0] ? std::nullopt : item_of_iterable(ev, n, args[0], state);
        ev.p.grow(Table::Elem, s, item.value_or(P::bottom()));
        return bmethod(P::none(), "set.update");
    }
    if (attr == "issubset" || attr == "issuperset" || attr == "isdisjoint")
        return bmethod(P::boolean(), "set." + attr);
    return refuse(ev, n, IssueClass::Stdlib, "set." + attr);
}

// str.join / bytes.join: the items of the argument have to be of the receiver's kind.
TR join_call(Eval& ev, const syntax::Call* n, const Args& args, const State& state, std::string_view receiver,
                            std::initializer_list<Kind> allowed, const Type& result) {
    const std::optional<Type> item = item_of_iterable(ev, n, args.empty() ? std::nullopt : args[0], state);
    if (!item)
        return refuse(ev, n, IssueClass::Conflict, std::format("{}.join of nothing", receiver));
    if (!infer::is_one_of(*item, allowed))
        return refuse(ev, n, IssueClass::Conflict, std::format("{}.join of {} items", receiver, infer::kind_text(*item)));
    return bmethod(result, std::format("{}.join", receiver));
}

TR str_method(Eval& ev, const syntax::Call* n, const std::string& attr, const Args& args, const Kw& kw, const State& state) {
    if (attr == "split") {
        if (args.size() > 2 || (!args.empty() && !kind_fits(args[0], "str|none")) || !kw.empty())
            return refuse(ev, n, IssueClass::Syntax, "str.split arguments");
        return bmethod(new_list(ev, n, P::str()), "str.split");
    }
    if (attr == "splitlines")
        return bmethod(new_list(ev, n, P::str()), "str.splitlines");
    if (attr == "join")
        return join_call(ev, n, args, state, "str", {Kind::Str, Kind::Bottom, Kind::Top}, P::str());
    if (attr == "partition" || attr == "rpartition")
        return bmethod(P::tuple_of({P::str(), P::str(), P::str()}), "str." + attr);
    if (attr == "format")
        return refuse(ev, n, IssueClass::Syntax, "str.format");
    return refuse(ev, n, IssueClass::Stdlib, "str." + attr);
}

TR bytes_method(Eval& ev, const syntax::Call* n, Kind k, const std::string& attr, const Args& args, const State& state) {
    if (attr == "join")
        return join_call(ev, n, args, state, "bytes", {Kind::Bytes, Kind::Bytearray, Kind::Bottom, Kind::Top}, Type{.kind = k});
    if (attr == "split")
        return bmethod(new_list(ev, n, Type{.kind = k}), "bytes.split");
    if (k == Kind::Bytearray && attr == "append") {
        if (args.empty() || !kind_fits(args[0], "int"))
            return refuse(ev, n, IssueClass::Conflict, "bytearray.append");
        return bmethod(P::none(), "bytearray.append");
    }
    if (k == Kind::Bytearray && attr == "extend") {
        if (args.empty() || !is_kind(args[0], {Kind::Bytes, Kind::Bytearray, Kind::Bottom, Kind::Top}))
            return refuse(ev, n, IssueClass::Syntax, "bytearray.extend of a " + (args.empty() ? std::string("?") : kind_of_arg(args[0])));
        return bmethod(P::none(), "bytearray.extend");
    }
    return refuse(ev, n, IssueClass::Stdlib, std::format("{}.{}", P::kind_word(k), attr));
}

}  // namespace

// ---- the library -----------------------------------------------------------------------------------------------------------------

namespace {

class StandardLibrary final : public infer::Library {
public:
    TR method(Eval& ev, const syntax::Call* n, const Type& recv, const std::string& attr, const Args& args, const Kw& kw,
              const State& state) const override;
    TR builtin(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args, const Kw& kw, const State& state) const override;
    TR std_call(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args, const Kw& kw, const State& state) const override;
};

TR StandardLibrary::method(Eval& ev, const syntax::Call* n, const Type& recv, const std::string& attr, const Args& args, const Kw& kw,
                           const State& state) const {
    if (const auto bad = lazy_argument(ev, n, args, kw, infer::word_in(attr, {"join", "extend", "update", "sort"})))
        return *bad;
    const Kind k = recv.kind == Kind::Bool ? Kind::Int : recv.kind;
    const std::string kind = std::string(P::kind_word(k));
    const bool as_bytes = k == Kind::Bytearray && simple_table().contains("bytes." + attr);
    const std::string name = (as_bytes ? "bytes" : kind) + "." + attr;
    if (simple_table().contains(name)) {
        Type t = simple_call(ev, n, name, args, kw);
        if (k == Kind::Bytearray && t.kind == Kind::Bytes && attr != "decode" && attr != "hex")
            t = Type{.kind = Kind::Bytearray};
        return bmethod(t, name);
    }
    switch (k) {
    case Kind::Str:
        if (infer::word_in(attr, {"split", "splitlines", "join", "partition", "rpartition", "format"}))
            return str_method(ev, n, attr, args, kw, state);
        break;
    case Kind::Bytes:
    case Kind::Bytearray:
        if (attr == "join" || attr == "split" || (k == Kind::Bytearray && (attr == "append" || attr == "extend")))
            return bytes_method(ev, n, k, attr, args, state);
        break;
    case Kind::List:
        return list_method(ev, n, recv, attr, args, kw, state);
    case Kind::Dict:
        return dict_method(ev, n, recv, attr, args);
    case Kind::Set:
        if (infer::word_in(attr, {"add", "discard", "remove", "update", "issubset", "issuperset", "isdisjoint"}))
            return set_method(ev, n, recv, attr, args, state);
        break;
    case Kind::Int:
        if (attr == "bit_length")
            return bmethod(P::int_range(Wide(0), recv.fits_i64() ? Bound(Wide(64)) : Bound()), "int.bit_length");
        if (attr == "to_bytes")
            return bmethod(P::bytes(), "int.to_bytes");
        break;
    case Kind::Fn:
        if (recv.name && recv.name->starts_with(infer::kBuiltinPrefix)) {
            const std::string full = recv.name->substr(infer::kBuiltinPrefix.size()) + "." + attr;
            if (full == "int.from_bytes")
                return int_from_bytes(ev, n, args, kw);
            if (full == "bytes.fromhex")
                return std_res(P::bytes(), "bytes.fromhex");
            return refuse(ev, n, IssueClass::Stdlib, full);
        }
        break;
    default:
        break;
    }
    return refuse(ev, n, IssueClass::Stdlib, kind + "." + attr);
}

// ---- the builtin functions --------------------------------------------------------------------------------------------------------

Type bool_as_int(const Type& t) { return t.kind == Kind::Bool ? P::int_range(Wide(0), Wide(1)) : t; }

// Removes the "set iteration order" issue this call's iteration of a set raised: what is sorted, or put in a set, has no order.
void forgive_set_order(Eval& ev, const syntax::Call* n) {
    std::erase_if(ev.issues, [&](const infer::Issue& i) { return i.detail == "set iteration order" && i.line == static_cast<std::uint32_t>(n->pos.lineno); });
}

TR range_call(Eval& ev, const syntax::Call* n, const Args& args, const Kw& kw) {
    if (!kw.empty() || args.empty() || args.size() > 3 ||
        !std::ranges::all_of(args, [](const auto& a) { return is_kind(a, {Kind::Int, Kind::Bottom, Kind::Top, Kind::Bool}); }))
        return refuse(ev, n, IssueClass::Conflict, "range arguments");
    for (const auto& a : args)
        if (a->kind == Kind::Bottom)
            return TR(P::bottom());
    for (const auto& a : args)
        if (a->kind == Kind::Top)
            return TR(*a);
    std::vector<Type> bounds;
    for (const auto& a : args)
        bounds.push_back(bool_as_int(*a));
    const Type lo = bounds.size() == 1 ? P::const_int(Wide(0)) : bounds[0];
    const Type hi = bounds.size() == 1 ? bounds[0] : bounds[1];
    const std::optional<Wide> step = bounds.size() == 3 ? infer::int_constant(n->args[2]) : std::optional<Wide>(Wide(1));
    if (!step || *step == Wide(0))
        return refuse(ev, n, IssueClass::Syntax, "range with a computed step");
    const Type item = *step > Wide(0) ? P::int_range(lo.lo, hi.hi ? Bound(*hi.hi - Wide(1)) : Bound())
                                      : P::int_range(hi.lo ? Bound(*hi.lo + Wide(1)) : Bound(), lo.hi);
    return builtin_res(P::iter_of(item, ev.range_count(n)), "range");
}

TR min_max_call(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args, const Kw& kw, const State& state) {
    for (const Keyword& k : kw)
        if (!k.name || !infer::word_in(*k.name, {"key", "default"}))
            return refuse(ev, n, IssueClass::Syntax, std::format("{}({}=)", name, k.name.value_or("**")));
    if (infer::find_keyword(kw, "key"))
        return refuse(ev, n, IssueClass::Syntax, name + " with a key");
    std::vector<std::optional<Type>> items;
    if (args.size() == 1) {
        std::optional<Type> e = item_of_iterable(ev, n, args[0], state);
        if (!e)
            return refuse(ev, n, IssueClass::Conflict, name + " arguments");
        if (const Keyword* dflt = infer::find_keyword(kw, "default"); dflt && dflt->type)
            e = ev.p.join(*e, *dflt->type);
        items = {e};
    } else {
        items = args;
    }
    Type r = P::bottom();
    for (const auto& t : items) {
        if (!t)
            return refuse(ev, n, IssueClass::Conflict, name + " arguments");
        r = ev.p.join(r, bool_as_int(*t));
    }
    if (r.kind == Kind::Top)
        return refuse(ev, n, IssueClass::Conflict, std::format("{} of mixed types: {}", name, r.why.value_or("")));
    if (!infer::is_one_of(r, {Kind::Int, Kind::Str, Kind::Float, Kind::Bytes, Kind::Tuple, Kind::Bottom}))
        return refuse(ev, n, IssueClass::Conflict, std::format("{} of {}", name, infer::kind_text(r)));
    if (r.kind == Kind::Int && items.size() > 1) {
        // the interval of the smallest (or largest) of several ints
        std::vector<Bound> los, his;
        for (const auto& t : items) {
            los.push_back(t->lo);
            his.push_back(t->hi);
        }
        const auto known = [](const std::vector<Bound>& v) {
            std::vector<Wide> out;
            for (const Bound& b : v)
                if (b)
                    out.push_back(*b);
            return out;
        };
        const auto any_unbounded = [](const std::vector<Bound>& v) { return std::ranges::any_of(v, [](const Bound& b) { return !b; }); };
        const std::vector<Wide> klo = known(los), khi = known(his);
        if (name == "min") {
            r = P::int_range(any_unbounded(los) ? Bound() : Bound(*std::ranges::min_element(klo)),
                             khi.empty() ? Bound() : Bound(*std::ranges::min_element(khi)));
        } else {
            r = P::int_range(klo.empty() ? Bound() : Bound(*std::ranges::max_element(klo)),
                             any_unbounded(his) ? Bound() : Bound(*std::ranges::max_element(khi)));
        }
    }
    return builtin_res(r, name + (args.size() == 1 ? "_iter" : ""));
}

TR sum_call(Eval& ev, const syntax::Call* n, const Args& args, const State& state) {
    Bound count;
    std::optional<Type> item;
    if (!args.empty() && !args[0] && !n->args.empty() && syntax::as<syntax::GeneratorExp>(n->args[0])) {
        item = ev.gen_elem(syntax::as<syntax::GeneratorExp>(n->args[0]), state).first;
        count = P::mem();
    } else if (!args.empty() && args[0]) {
        auto [e, passes] = ev.iteration(n->args[0], *args[0]);
        item = e;
        count = passes;
    } else {
        return refuse(ev, n, IssueClass::Conflict, "sum");
    }
    const Type e = bool_as_int(*item);
    if (e.kind == Kind::Int) {
        const Bound passes = count ? count : Bound(P::mem());
        Type r = P::multiply(e, P::int_range(Wide(0), passes));
        r = P::int_range(r.lo ? Bound(std::min(*r.lo, Wide(0))) : Bound(), r.hi ? Bound(std::max(*r.hi, Wide(0))) : Bound());
        if (args.size() > 1 && args[1])
            r = P::add(r, *args[1]);
        return builtin_res(r, "sum");
    }
    if (e.kind == Kind::Float)
        return builtin_res(P::floating(), "sum");
    if (infer::is_one_of(e, {Kind::Bottom, Kind::Top}))
        return TR(e);
    return refuse(ev, n, IssueClass::Conflict, "sum of " + infer::kind_text(e));
}

TR bytes_call(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args) {
    const Kind k = name == "bytes" ? Kind::Bytes : Kind::Bytearray;
    if (args.empty())
        return builtin_res(P::bytes_of(Wide(0), k), name);
    const std::optional<Type>& a0 = args[0];
    if (!a0)
        return refuse(ev, n, IssueClass::Conflict, name + " arguments");
    if (a0->kind == Kind::Int)
        return builtin_res(P::bytes_of(a0->hi, k), name + "_zeros");
    if (a0->kind == Kind::Bytes || a0->kind == Kind::Bytearray)
        return builtin_res(P::bytes_of(a0->hi, k), name + "_copy");
    if (a0->kind == Kind::List) {
        const Type e = element_of(ev, *a0);
        if (!infer::is_one_of(e, {Kind::Int, Kind::Bottom, Kind::Top}))
            return refuse(ev, n, IssueClass::Conflict, std::format("{} of a list of {}", name, infer::kind_text(e)));
        return builtin_res(Type{.kind = k}, name + "_list");
    }
    if (a0->kind == Kind::Str)
        return refuse(ev, n, IssueClass::Syntax, name + "(str, encoding)");
    if (a0->kind == Kind::Iter)
        return builtin_res(Type{.kind = k}, name + "_iter");
    if (infer::is_one_of(*a0, {Kind::Bottom, Kind::Top}))
        return TR(*a0);
    return refuse(ev, n, IssueClass::Conflict, std::format("{} of {}", name, infer::kind_text(*a0)));
}

TR dict_call(Eval& ev, const syntax::Call* n, const Args& args, const Kw& kw, const State& state) {
    const SiteId s = ev.p.new_site(n, Container::Dict, ev.where(n));
    std::vector<std::string> keys;
    for (const syntax::keyword* k : n->keywords) {
        if (!k->has_arg) {
            ev.spread(s, ev.ev(k->value, state), n);
        } else {
            const Keyword* given = infer::find_keyword(kw, k->arg);
            ev.p.grow_field(s, k->arg, ev.fit(k->value, given && given->type ? *given->type : P::bottom(), "field " + k->arg));
            keys.push_back(k->arg);
        }
    }
    if (!args.empty()) {
        if (!args[0] || args[0]->kind != Kind::Dict)
            return refuse(ev, n, IssueClass::Syntax, "dict() of a " + kind_of_arg(args[0]));
        ev.spread(s, *args[0], n);
    }
    if (ev.live()) {
        std::vector<std::string> all = keys;
        if (const auto& spread_keys = ev.p.site(s).spread_keys)
            all.insert(all.end(), spread_keys->begin(), spread_keys->end());
        ev.p.literal_keys[n] = all;
        if (ev.builder != n)
            ev.p.created(s, program::StringSet(all.begin(), all.end()));
    }
    return builtin_res(infer::site_type(Kind::Dict, s), "dict");
}

// isinstance whose answer the types decide is that answer; one that depends on the run-time class is refused.
TR isinstance_call(Eval& ev, const syntax::Call* n, const std::optional<Type>& t) {
    static const std::unordered_map<std::string_view, std::vector<Kind>> kTypes = {
        {"int", {Kind::Int, Kind::Bool}}, {"str", {Kind::Str}},     {"bytes", {Kind::Bytes}}, {"bytearray", {Kind::Bytearray}},
        {"bool", {Kind::Bool}},           {"float", {Kind::Float}}, {"list", {Kind::List}},   {"dict", {Kind::Dict}},
        {"tuple", {Kind::Tuple}},         {"set", {Kind::Set}}};
    if (!t || n->args.size() != 2)
        return refuse(ev, n, IssueClass::Conflict, "isinstance arguments");
    if (infer::is_one_of(*t, {Kind::Bottom, Kind::Top}))
        return TR(P::boolean());
    const syntax::Expr* spec = n->args[1];
    std::vector<const syntax::Expr*> names;
    if (const auto* tuple = syntax::as<syntax::Tuple>(spec))
        names.assign(tuple->elts.begin(), tuple->elts.end());
    else
        names.push_back(spec);
    const auto of_kind = [&](const std::string& id, Kind k) {
        const auto it = kTypes.find(id);
        return it != kTypes.end() && std::ranges::find(it->second, k) != it->second.end();
    };
    bool answer = false;
    for (const syntax::Expr* x : names) {
        const auto* name = syntax::as<syntax::Name>(x);
        if (name && kTypes.contains(name->id)) {
            if (of_kind(name->id, t->kind))
                answer = true;
            else if (t->kind == Kind::Opt && of_kind(name->id, t->args[0].kind))
                return refuse(ev, n, IssueClass::Dynamic, "isinstance of an optional value (use `is not None`)");
        } else if (name && t->kind == Kind::Inst) {
            const std::optional<std::string> cls = ev.lookup_class(x);
            if (!cls)
                return refuse(ev, n, IssueClass::Dynamic, "isinstance " + pyast::describe_expr(x));
            const std::vector<std::string> mro_of_value = ev.p.mro(*t->name);
            if (std::ranges::find(mro_of_value, *cls) != mro_of_value.end())
                answer = true;
            else if (const std::vector<std::string> mro_of_class = ev.p.mro(*cls); std::ranges::find(mro_of_class, *t->name) != mro_of_class.end())
                return refuse(ev, n, IssueClass::Dynamic, "isinstance of a subclass (run-time class)");
        } else if (name) {
            if (!ev.lookup_class(x))
                return refuse(ev, n, IssueClass::Dynamic, "isinstance " + pyast::describe_expr(x));
        } else {
            return refuse(ev, n, IssueClass::Dynamic, "isinstance " + pyast::describe_expr(x));
        }
    }
    Res r = Res::of(ResKind::Isinstance);
    r.flag = answer;
    return TR(P::boolean(), r);
}

TR StandardLibrary::builtin(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args, const Kw& kw, const State& state) const {
    if (infer::dynamic_builtins().contains(name))
        return refuse(ev, n, IssueClass::Dynamic, name);
    static const std::set<std::string> kLazyOk = {"sorted", "min", "max", "sum", "any", "all", "list", "set", "enumerate"};
    const bool takes_key = name == "sorted" || name == "min" || name == "max";
    if (const auto bad = lazy_argument(ev, n, args, takes_key ? Kw{} : kw, kLazyOk.contains(name)))
        return *bad;
    const std::optional<Type> a0 = args.empty() ? std::nullopt : args[0];

    if (name == "len") {
        if (args.size() != 1 || !a0)
            return refuse(ev, n, IssueClass::Conflict, "len arity");
        if (a0->kind == Kind::Dict)
            ev.p.make_dynamic(*a0->site, "len");
        if (a0->kind == Kind::Tuple)
            return builtin_res(P::const_int(Wide(static_cast<std::int64_t>(a0->args.size()))), "len");
        if (a0->kind == Kind::Bytes || a0->kind == Kind::Bytearray)
            return builtin_res(P::int_range(Wide(0), a0->hi ? a0->hi : Bound(P::mem())), "len");
        if (infer::is_one_of(*a0, {Kind::Str, Kind::List, Kind::Dict, Kind::Set, Kind::Bottom, Kind::Top}))
            return builtin_res(P::size_type(), "len");
        return refuse(ev, n, IssueClass::Conflict, "len of a " + infer::kind_text(*a0));
    }
    if (name == "range")
        return range_call(ev, n, args, kw);
    if (name == "enumerate") {
        if (!a0)
            return refuse(ev, n, IssueClass::Conflict, "enumerate arguments");
        const auto [item, count] = ev.iteration(n->args[0], *a0);
        const Keyword* start_kw = infer::find_keyword(kw, "start");
        const Type start = args.size() > 1 && args[1] ? *args[1] : start_kw && start_kw->type ? *start_kw->type : P::const_int(Wide(0));
        if (start.kind != Kind::Int)
            return refuse(ev, n, IssueClass::Conflict, "enumerate start");
        const Type index = P::add(start, P::int_range(Wide(0), (count ? *count : P::mem()) - Wide(1)));
        return builtin_res(P::iter_of(P::tuple_of({index, item}), count), "enumerate");
    }
    if (name == "zip") {
        if (!kw.empty() || std::ranges::any_of(args, [](const auto& a) { return !a; }))
            return refuse(ev, n, IssueClass::Syntax, "zip arguments");
        std::vector<Type> items;
        for (std::size_t i = 0; i < args.size(); ++i)
            items.push_back(ev.iteration(n->args[i], *args[i]).first);
        return builtin_res(P::iter_of(P::tuple_of(items), P::mem()), "zip");
    }
    if (name == "reversed") {
        if (!a0)
            return refuse(ev, n, IssueClass::Conflict, "reversed arguments");
        if (a0->kind == Kind::Dict)
            return refuse(ev, n, IssueClass::Syntax, "reversed of a dict");
        const auto [item, count] = ev.iteration(n->args[0], *a0);
        return builtin_res(P::iter_of(item, count), "reversed");
    }
    if (name == "sorted") {
        std::optional<Type> item;
        if (a0) {
            item = ev.iteration(n->args[0], *a0).first;
            if (a0->kind == Kind::Set)
                forgive_set_order(ev, n);
        } else if (!n->args.empty() && syntax::as<syntax::GeneratorExp>(n->args[0])) {
            item = ev.gen_elem(syntax::as<syntax::GeneratorExp>(n->args[0]), state).first;
        } else {
            return refuse(ev, n, IssueClass::Conflict, "sorted arguments");
        }
        return sort_call(ev, n, *item, kw, state, "sorted", new_list(ev, n, *item));
    }
    if (name == "min" || name == "max")
        return min_max_call(ev, n, name, args, kw, state);
    if (name == "abs") {
        if (!a0)
            return refuse(ev, n, IssueClass::Conflict, "abs");
        if (a0->kind == Kind::Int) {
            if (a0->lo && *a0->lo >= Wide(0))
                return builtin_res(*a0, "abs");
            const Bound most = a0->lo && a0->hi ? Bound(std::max(a0->lo->abs(), a0->hi->abs())) : Bound();
            return builtin_res(P::int_range(Wide(0), most), "abs");
        }
        if (a0->kind == Kind::Float)
            return builtin_res(P::floating(), "abs");
        if (infer::is_one_of(*a0, {Kind::Bottom, Kind::Top}))
            return TR(*a0);
        return refuse(ev, n, IssueClass::Conflict, "abs of " + infer::kind_text(*a0));
    }
    if (name == "sum")
        return sum_call(ev, n, args, state);
    if (name == "any" || name == "all") {
        if (!a0 && !n->args.empty() && syntax::as<syntax::GeneratorExp>(n->args[0]))
            (void)ev.gen_elem(syntax::as<syntax::GeneratorExp>(n->args[0]), state);
        else if (a0)
            (void)ev.iteration(n->args[0], *a0);
        return builtin_res(P::boolean(), name);
    }
    if (name == "int") {
        if (!kw.empty())
            return refuse(ev, n, IssueClass::Syntax, "int keywords");
        if (args.empty())
            return builtin_res(P::const_int(Wide(0)), "int");
        if (!a0)
            return refuse(ev, n, IssueClass::Conflict, "int() of lambda");
        if (a0->kind == Kind::Int || a0->kind == Kind::Bool)
            return builtin_res(a0->kind == Kind::Int ? *a0 : P::int_range(Wide(0), Wide(1)), "int");
        if (a0->kind == Kind::Str) {
            if (args.size() == 2 && !infer::int_constant(n->args[1]))
                return refuse(ev, n, IssueClass::Dynamic, "int() with a computed base");
            return builtin_res(P::int_range(), "int_str");
        }
        if (a0->kind == Kind::Float)
            return builtin_res(P::int_range(), "int_float");
        if (infer::is_one_of(*a0, {Kind::Bottom, Kind::Top}))
            return TR(*a0);
        return refuse(ev, n, IssueClass::Conflict, "int() of " + infer::kind_text(*a0));
    }
    if (name == "float") {
        if (a0 && a0->kind == Kind::Int)
            ev.exact_float(n, *a0);
        return builtin_res(P::floating(), "float");
    }
    if (name == "str") {
        if (a0)
            ev.check_str(n, *a0, false);
        return builtin_res(P::str(), "str");
    }
    if (name == "repr") {
        if (a0)
            ev.check_str(n, *a0, true);
        return builtin_res(P::str(), "repr");
    }
    if (name == "bool")
        return builtin_res(P::boolean(), "bool");
    if (name == "bytes" || name == "bytearray")
        return bytes_call(ev, n, name, args);
    if (name == "list") {
        const SiteId s = ev.p.new_site(n, Container::List, ev.where(n));
        if (!args.empty()) {
            const std::optional<Type> item = item_of_iterable(ev, n, a0, state);
            ev.p.grow(Table::Elem, s, item.value_or(P::bottom()));
        }
        return builtin_res(infer::site_type(Kind::List, s), "list");
    }
    if (name == "set") {
        const SiteId s = ev.p.new_site(n, Container::Set, ev.where(n));
        if (!args.empty()) {
            const std::optional<Type> item = item_of_iterable(ev, n, a0, state);
            if (a0 && a0->kind == Kind::Set)
                forgive_set_order(ev, n);
            ev.p.grow(Table::Elem, s, item.value_or(P::bottom()));
        }
        return builtin_res(infer::site_type(Kind::Set, s), "set");
    }
    if (name == "frozenset")
        return refuse(ev, n, IssueClass::Syntax, "frozenset");
    if (name == "tuple") {
        if (a0 && a0->kind == Kind::Tuple)
            return builtin_res(*a0, "tuple");
        return refuse(ev, n, IssueClass::Conflict, "tuple() of a sequence (its length is not in the type)");
    }
    if (name == "dict")
        return dict_call(ev, n, args, kw, state);
    if (name == "print") {
        if (!keywords_within(kw, {"end", "sep", "file", "flush"}))
            return refuse(ev, n, IssueClass::Syntax, "print keywords");
        if (infer::find_keyword(kw, "file"))
            return refuse(ev, n, IssueClass::Stdlib, "print(file=)");
        for (std::size_t i = 0; i < n->args.size() && i < args.size(); ++i)
            if (args[i])
                ev.check_str(n->args[i], *args[i], false);
        return builtin_res(P::none(), "print");
    }
    if (name == "open") {
        const syntax::Expr* mode_node = n->args.size() > 1 ? n->args[1] : keyword_value(n, "mode");
        const std::optional<std::string> mode = mode_node ? pyast::string_constant(mode_node) : std::nullopt;
        if (mode == "rb")
            return builtin_res(infer::named_type(Kind::Extern, "file"), "open_rb");
        return refuse(ev, n, IssueClass::Stdlib, std::format("open({})", repr_text(mode)));
    }
    if (name == "ord")
        return builtin_res(P::int_range(Wide(0), Wide(0x10FFFF)), "ord");
    if (name == "chr") {
        if (a0 && !infer::is_one_of(*a0, {Kind::Int, Kind::Bottom, Kind::Top}))
            return refuse(ev, n, IssueClass::Conflict, "chr of " + infer::kind_text(*a0));
        return builtin_res(P::str(), "chr");
    }
    if (name == "hex" || name == "oct" || name == "bin")
        return builtin_res(P::str(), name);
    if (name == "divmod") {
        if (args.size() == 2 && std::ranges::all_of(args, [](const auto& a) { return is_kind(a, {Kind::Int}); }))
            return builtin_res(P::tuple_of({P::floor_divide(*args[0], *args[1]), P::modulo(*args[0], *args[1])}), "divmod");
        return refuse(ev, n, IssueClass::Conflict, "divmod arguments");
    }
    if (name == "pow") {
        if (args.size() == 3 && std::ranges::all_of(args, [](const auto& a) { return is_kind(a, {Kind::Int}); }) && args[2]->positive())
            return builtin_res(P::int_range(Wide(0), args[2]->hi ? Bound(*args[2]->hi - Wide(1)) : Bound()), "pow3");
        if (args.size() == 2 && args[0] && args[1])
            return builtin_res(ev.binop(n, syntax::OpKind::Pow, *args[0], *args[1], n->args[0], n->args[1]), "pow");
        return refuse(ev, n, IssueClass::Conflict, "pow arguments");
    }
    if (name == "round")
        return args.size() == 1 ? builtin_res(P::int_range(), "round") : builtin_res(P::floating(), "round2");
    if (name == "format") {
        const std::optional<std::string> spec = n->args.size() > 1 ? pyast::string_constant(n->args[1]) : std::optional<std::string>("");
        if (!spec)
            return refuse(ev, n, IssueClass::Dynamic, "format with a computed spec");
        if (a0)
            ev.check_format(n, *a0, *spec);
        return builtin_res(P::str(), "format");
    }
    if (name == "isinstance")
        return isinstance_call(ev, n, a0);
    if (name == "super") {
        if (!args.empty())
            return refuse(ev, n, IssueClass::Syntax, "super(arguments)");
        return builtin_res(infer::named_type(Kind::Extern, "super"), "super");
    }
    if (name == "map" || name == "filter" || name == "object" || name == "staticmethod" || name == "classmethod" || name == "property")
        return refuse(ev, n, IssueClass::Dynamic, name);
    return refuse(ev, n, IssueClass::Stdlib, "builtins." + name);
}

// ---- the standard library ---------------------------------------------------------------------------------------------------------------

struct PathCall {
    std::size_t arity;   // 0: one or more strings (os.path.join)
    Type result;
};

const std::unordered_map<std::string, PathCall>& path_table() {
    static const std::unordered_map<std::string, PathCall> table = {
        {"os.path.join", {0, P::str()}},
        {"os.path.basename", {1, P::str()}},
        {"os.path.dirname", {1, P::str()}},
        {"os.path.splitext", {1, P::tuple_of({P::str(), P::str()})}},
        {"os.path.exists", {1, P::boolean()}},
        {"os.path.isfile", {1, P::boolean()}},
        {"os.path.isdir", {1, P::boolean()}},
        {"os.path.getsize", {1, P::size_type()}},
        {"os.path.split", {1, P::tuple_of({P::str(), P::str()})}},
        {"os.path.normcase", {1, P::str()}}};
    return table;
}

// struct.calcsize / unpack / unpack_from / pack / pack_into / error, for a constant format.
TR struct_call(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args, const Kw& kw) {
    const std::string fn = name.substr(name.find('.') + 1);
    if (!kw.empty())
        return refuse(ev, n, IssueClass::Syntax, name + " keywords");
    if (fn == "error")
        return TR(infer::named_type(Kind::Inst, "exc:struct.error"), Res::of(ResKind::Exc, "struct.error"));
    const std::optional<std::string> format = n->args.empty() ? std::nullopt : pyast::string_constant(n->args[0]);
    if (!format)
        return refuse(ev, n, IssueClass::Dynamic, name + " with a computed format");
    const auto items = structfmt::parse(*format);
    if (!items)
        return refuse(ev, n, IssueClass::Stdlib, std::format("{}({})", name, repr_text(*format)));
    const std::vector<structfmt::Item> values = structfmt::values(*items);
    if (fn == "calcsize")
        return std_res(P::const_int(Wide::from_u64(structfmt::size(*items))), name);
    if (fn == "unpack" || fn == "unpack_from") {
        if (fn == "unpack" ? args.size() != 2 : (args.size() != 2 && args.size() != 3))
            return refuse(ev, n, IssueClass::Conflict, name + " arity");
        if (!is_kind(args[1], {Kind::Bytes, Kind::Bytearray, Kind::Bottom, Kind::Top}))
            return refuse(ev, n, IssueClass::Conflict, std::format("{} of a {}", name, kind_of_arg(args[1])));
        if (args.size() == 3 && args[2] && !infer::is_one_of(*args[2], {Kind::Int, Kind::Bottom, Kind::Top}))
            return refuse(ev, n, IssueClass::Conflict, name + " offset");
        std::vector<Type> types;
        for (const structfmt::Item& v : values) {
            switch (v.kind) {
            case structfmt::Kind::Int:
                types.push_back(P::int_range(Wide(v.lo), Wide::from_u64(v.hi)));
                break;
            case structfmt::Kind::Bytes:
                types.push_back(P::bytes_of(Wide::from_u64(v.size)));
                break;
            case structfmt::Kind::Bool:
                types.push_back(P::boolean());
                break;
            default:
                types.push_back(P::floating());
                break;
            }
        }
        return std_res(P::tuple_of(types), name, *format);
    }
    if (fn == "pack" || fn == "pack_into") {
        const std::size_t first = fn == "pack" ? 1 : 3;
        const std::span<const std::optional<Type>> given = std::span(args).subspan(std::min(first, args.size()));
        if (given.size() != values.size())
            return refuse(ev, n, IssueClass::Conflict, std::format("{}: {} values for {}", name, given.size(), repr_text(*format)));
        if (fn == "pack_into" && !is_kind(args[1], {Kind::Bytearray, Kind::Bottom, Kind::Top}))
            return refuse(ev, n, IssueClass::Conflict, "pack_into a " + kind_of_arg(args[1]));
        for (std::size_t i = 0; i < given.size(); ++i) {
            if (!given[i])
                return refuse(ev, n, IssueClass::Conflict, name + " value");
            std::vector<Kind> want;
            switch (values[i].kind) {
            case structfmt::Kind::Int:
                want = {Kind::Int, Kind::Bool, Kind::Bottom, Kind::Top};
                break;
            case structfmt::Kind::Bytes:
                want = {Kind::Bytes, Kind::Bytearray, Kind::Bottom, Kind::Top};
                break;
            case structfmt::Kind::Bool:
                want = {Kind::Bool, Kind::Int, Kind::Bottom, Kind::Top};
                break;
            default:
                want = {Kind::Float, Kind::Int, Kind::Bottom, Kind::Top};
                break;
            }
            if (std::ranges::find(want, given[i]->kind) == want.end())
                return refuse(ev, n, IssueClass::Conflict, std::format("{} of a {} for {}", name, infer::kind_text(*given[i]), values[i].code));
        }
        return std_res(fn == "pack" ? P::bytes_of(Wide::from_u64(structfmt::size(*items))) : P::none(), name, *format);
    }
    return refuse(ev, n, IssueClass::Stdlib, name);
}

TR StandardLibrary::std_call(Eval& ev, const syntax::Call* n, const std::string& name, const Args& args, const Kw& kw, const State&) const {
    if (const auto bad = lazy_argument(ev, n, args, kw, false))
        return *bad;
    if (name.starts_with("struct."))
        return struct_call(ev, n, name, args, kw);
    if (const auto it = path_table().find(name); it != path_table().end()) {
        if (!kw.empty())
            return refuse(ev, n, IssueClass::Syntax, name + " keywords");
        const bool all_str = std::ranges::all_of(args, [](const auto& a) { return is_kind(a, {Kind::Str, Kind::Bottom, Kind::Top}); });
        if (!all_str || (it->second.arity == 0 ? args.empty() : args.size() != it->second.arity))
            return refuse(ev, n, IssueClass::Conflict, name + " arguments");
        return std_res(it->second.result, name);
    }
    if (name == "os.listdir")
        return std_res(new_list(ev, n, P::str()), name);
    if (name == "binascii.crc32" || name == "zlib.crc32") {
        if (args.empty() || !is_kind(args[0], {Kind::Bytes, Kind::Bytearray, Kind::Bottom, Kind::Top}))
            return refuse(ev, n, IssueClass::Conflict, name + " arguments");
        if (args.size() > 1 && (!args[1] || !args[1]->fits_i64()))
            return refuse(ev, n, IssueClass::IntRange, name + " start value");
        return std_res(P::unsigned_int(32), name);
    }
    if (name == "bisect.bisect_left" || name == "bisect.bisect_right" || name == "bisect.bisect") {
        if (args.size() != 2 || !kw.empty() || !args[0] || args[0]->kind != Kind::List)
            return refuse(ev, n, IssueClass::Syntax, name + " arguments");
        return std_res(P::size_type(), name);
    }
    if (name == "math.sqrt")
        return std_res(P::floating(), name);
    if (name == "math.floor" || name == "math.ceil")
        return std_res(P::int_range(), name);
    const std::string top = name.substr(0, name.find('.'));
    return refuse(ev, n, IssueClass::Stdlib, top != "re" ? name : "re." + name.substr(name.rfind('.') + 1));
}

}  // namespace

const infer::Library& library() {
    static const StandardLibrary instance;
    return instance;
}

}  // namespace cb::translate::frontends::python::methods
