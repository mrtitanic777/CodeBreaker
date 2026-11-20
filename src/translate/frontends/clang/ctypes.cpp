#include "translate/frontends/clang/ctypes.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "core/error.hpp"
#include "core/text.hpp"

namespace cb::translate::frontends::clang::ctypes {

namespace {

using ir::TypeP;

[[noreturn]] void unreadable(const std::string& what) { throw ParseError("clang type", 0, 0, what); }

constexpr std::string_view kQualifiers[] = {"const", "volatile", "restrict", "__restrict", "__unaligned", "_Atomic",
                                    "struct", "union", "enum", "class", "typename", "__ptr64", "__ptr32", "__w64",
                                    // calling conventions: one convention on the x64 target
                                    "__cdecl", "__stdcall", "__fastcall", "__thiscall", "__vectorcall", "__clrcall"};
constexpr std::string_view kBuiltinWords[] = {"void", "char", "short", "int", "long", "float", "double", "signed",
                                      "unsigned", "_Bool", "bool", "wchar_t", "char8_t", "char16_t", "char32_t", "__int64",
                                      "__int32", "__int16", "__int8", "__int128", "_Float16", "__fp16"};

bool is_qualifier(std::string_view tok) { return std::ranges::find(kQualifiers, tok) != std::end(kQualifiers); }
bool is_builtin_word(std::string_view tok) { return std::ranges::find(kBuiltinWords, tok) != std::end(kBuiltinWords); }
bool is_pointer_op(std::string_view tok) { return tok == "*" || tok == "&" || tok == "&&"; }

bool is_ident_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '~'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }

// '(unnamed struct at C:\x\t.c:3:9)' -> '3_9': the line and column clang named it by ("0_0" when the text has none).
std::string anon_key_of(std::string_view note) {
    if (note.empty() || note.back() != ')') return "0_0";
    std::string_view body = note.substr(0, note.size() - 1);
    const auto number_at_end = [&body]() -> std::optional<std::string_view> {
        std::size_t start = body.size();
        while (start > 0 && is_digit(body[start - 1])) --start;
        if (start == body.size() || start == 0 || body[start - 1] != ':') return std::nullopt;
        const std::string_view digits = body.substr(start);
        body = body.substr(0, start - 1);
        return digits;
    };
    const auto column = number_at_end();
    if (!column) return "0_0";
    const auto line = number_at_end();
    if (!line) return "0_0";
    return std::format("{}_{}", *line, *column);
}

// Replaces each "<open>...)" (up to the first ')') by what `make` says of the whole note.
template <class Make>
std::string replace_notes(std::string s, std::string_view open, Make&& make) {
    std::size_t at = 0;
    while ((at = s.find(open, at)) != std::string::npos) {
        const std::size_t close = s.find(')', at + open.size());
        if (close == std::string::npos) break;
        const std::string note = s.substr(at, close + 1 - at);
        const std::string replacement = make(note);
        s.replace(at, note.size(), replacement);
        at += replacement.size();
    }
    return s;
}

// Drops `__attribute__((...))` (cdecl, aligned ...: not a type), whatever it nests.
std::string drop_attributes(const std::string& s) {
    constexpr std::string_view open = "__attribute__((";
    std::string out;
    std::size_t at = 0;
    while (true) {
        const std::size_t found = s.find(open, at);
        if (found == std::string::npos) break;
        out.append(s, at, found - at);
        std::size_t i = found + open.size();
        int depth = 2;
        while (i < s.size() && depth > 0) {
            if (s[i] == '(') ++depth;
            if (s[i] == ')') --depth;
            ++i;
        }
        if (depth > 0) {   // an unbalanced attribute is not one
            out.append(s, found, std::string::npos);
            return out;
        }
        out += ' ';
        at = i;
    }
    out.append(s, at, std::string::npos);
    return out;
}

// "unnamed struct", "anonymous union" ...: a note of the form `(<unnamed|anonymous> <tag> at ...)`.
std::string replace_tagged_anonymous(std::string s, std::string_view word) {
    for (const std::string_view tag : {"struct", "union", "enum", "class"}) {
        const std::string open = std::format("({} {} at ", word, tag);
        s = replace_notes(std::move(s), open, [](const std::string& note) { return "__unnamed_" + anon_key_of(note); });
    }
    return s;
}

}  // namespace

Model Model::for_triple(std::string_view triple) {
    const std::string t = text::lower(triple);
    const bool wide = t.starts_with("x86_64") || t.starts_with("aarch64");
    Model m;
    m.triple = std::string(triple);
    m.pointer = wide ? 8 : 4;
    if (t.find("windows") != std::string::npos || t.find("win32") != std::string::npos || t.find("mingw") != std::string::npos) {
        m.long_bytes = 4;
        m.wchar = 2;
    } else {
        m.long_bytes = wide ? 8 : 4;
        m.wchar = 4;
        m.long_double = 16;
    }
    return m;
}

ir::CompilerTarget Model::target(std::string_view language, std::string_view standard) const {
    ir::CompilerTarget t;
    t.triple = triple;
    t.pointer = pointer;
    t.long_bits = long_bytes;
    t.wchar = wchar;
    t.char_signed = char_signed;
    t.language = std::string(language);
    t.standard = std::string(standard);
    return t;
}

// ---- builtin types -------------------------------------------------------------------------------------------

ir::TypeP builtin_type(const std::vector<std::string>& words, const Model& model) {
    const auto has = [&words](std::string_view w) { return std::ranges::find(words, w) != words.end(); };
    const bool is_unsigned = has("unsigned");
    const bool is_signed = has("signed");
    const auto longs = std::ranges::count(words, "long");
    const auto int_of = [is_unsigned](std::uint32_t bytes) {
        switch (bytes) {
            case 1: return ir::prim_type(is_unsigned ? ir::Prim::U8 : ir::Prim::I8);
            case 2: return ir::prim_type(is_unsigned ? ir::Prim::U16 : ir::Prim::I16);
            case 4: return ir::prim_type(is_unsigned ? ir::Prim::U32 : ir::Prim::I32);
            default: return ir::prim_type(is_unsigned ? ir::Prim::U64 : ir::Prim::I64);
        }
    };
    if (has("void")) return ir::void_type();
    if (has("_Bool") || has("bool")) return ir::bool_type();
    if (has("float")) return ir::prim_type(ir::Prim::F32);
    if (has("double")) return ir::prim_type(longs == 0 || model.long_double == 8 ? ir::Prim::F64 : ir::Prim::F80);
    if (has("__int128")) throw UnsupportedError("clang type", 0, "__int128");
    if (has("_Float16") || has("__fp16")) throw UnsupportedError("clang type", 0, "half float");
    if (has("wchar_t")) {
        return ir::prim_type(model.wchar == 2 ? ir::Prim::U16 : ir::Prim::I32);
    }
    if (has("char8_t")) return ir::prim_type(ir::Prim::U8);
    if (has("char16_t")) return ir::prim_type(ir::Prim::U16);
    if (has("char32_t")) return ir::prim_type(ir::Prim::U32);
    if (has("char")) {
        if (is_unsigned) return ir::prim_type(ir::Prim::U8);
        if (is_signed) return ir::prim_type(ir::Prim::I8);
        return ir::prim_type(ir::Prim::Char, model.char_signed);
    }
    std::uint32_t bytes = 4;
    if (has("__int8")) bytes = 1;
    else if (has("short") || has("__int16")) bytes = 2;
    else if (longs >= 2 || has("__int64")) bytes = 8;
    else if (longs == 1) bytes = model.long_bytes;
    return int_of(bytes);
}

// ---- the parser ------------------------------------------------------------------------------------------------

std::string anon_key(std::int64_t line, std::int64_t column) { return std::format("{}_{}", line, column); }

std::vector<std::string> tokens(std::string_view spelling) {
    std::string s = drop_attributes(std::string(spelling));
    s = replace_tagged_anonymous(std::move(s), "unnamed");
    s = replace_tagged_anonymous(std::move(s), "anonymous");
    for (const std::string_view word : {"unnamed", "anonymous"}) {
        s = replace_notes(std::move(s), std::format("({} at ", word), [](const std::string& note) { return "struct __unnamed_" + anon_key_of(note); });
    }
    s = replace_notes(std::move(s), "(lambda at ", [](const std::string&) { return std::string("__lambda"); });

    std::vector<std::string> out;
    std::size_t i = 0;
    while (i < s.size()) {
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r' || s[i] == '\f' || s[i] == '\v')) ++i;
        if (i >= s.size()) break;
        const char c = s[i];
        std::size_t end = i;
        if (s.compare(i, 3, "...") == 0) {
            end = i + 3;
        } else if (s.compare(i, 2, "::") == 0 || s.compare(i, 2, "&&") == 0) {
            end = i + 2;
        } else if (is_ident_start(c)) {
            end = i + 1;
            while (end < s.size() && (is_ident_start(s[end]) ? s[end] != '~' : is_digit(s[end]))) ++end;
        } else if (is_digit(c)) {
            while (end < s.size() && is_digit(s[end])) ++end;
        } else if (std::string_view("*&()[],<>:").find(c) != std::string_view::npos) {
            end = i + 1;
        } else {
            unreadable(std::format("cannot read type '{}' at '{}'", s, s.substr(i)));
        }
        out.push_back(s.substr(i, end - i));
        i = end;
    }
    return out;
}

Parser::Parser(Model model, NameResolver names) : model_(std::move(model)), names_(std::move(names)) {}

ir::TypeP Parser::parse(const std::string& text) {
    if (const auto hit = cache_.find(text); hit != cache_.end()) return hit->second;
    Cursor saved = std::move(cur_);   // (a typedef's meaning is parsed mid-parse)
    cur_ = Cursor{tokens(text), 0};
    ir::TypeP t;
    try {
        t = type_();
        if (cur_.at != cur_.tokens.size()) unreadable(std::format("trailing '{}' in type '{}'", cur_.tokens[cur_.at], text));
    } catch (...) {
        cur_ = std::move(saved);
        throw;
    }
    cur_ = std::move(saved);
    cache_.emplace(text, t);
    return t;
}

const std::string* Parser::peek(std::size_t ahead) const {
    const std::size_t j = cur_.at + ahead;
    return j < cur_.tokens.size() ? &cur_.tokens[j] : nullptr;
}

std::string Parser::take() {
    if (cur_.at >= cur_.tokens.size()) unreadable("type ends too soon");
    return cur_.tokens[cur_.at++];
}

void Parser::expect(std::string_view want) {
    const std::string* tok = peek();
    if (tok == nullptr || *tok != want) unreadable(std::format("expected '{}', got '{}'", want, tok != nullptr ? *tok : "nothing"));
    ++cur_.at;
}

ir::TypeP Parser::type_() { return declarator(specifiers()); }

ir::TypeP Parser::specifiers() {
    std::vector<std::string> words;
    std::optional<std::string> tag;
    std::optional<std::pair<std::string, std::vector<ir::TypeP>>> named;
    while (const std::string* tok = peek()) {
        if (*tok == "struct" || *tok == "union" || *tok == "enum" || *tok == "class") {
            tag = *tok;
            ++cur_.at;
            continue;
        }
        if (is_qualifier(*tok)) {
            ++cur_.at;
            continue;
        }
        if (is_builtin_word(*tok) && !named && !tag) {
            words.push_back(*tok);
            ++cur_.at;
            continue;
        }
        if ((is_ident_start((*tok)[0]) || *tok == "::") && words.empty() && !named) {
            named = qualified_name();
            continue;
        }
        break;
    }
    if (!words.empty()) return builtin_type(words, model_);
    if (!named) unreadable("no type in the spelling");
    const auto& [name, args] = *named;
    const ir::TypeP got = names_(name, args, tag);
    if (tag == "enum") return got ? got : ir::i32_type();
    if (!got) unreadable("unknown type name '" + name + "'");
    return got;
}

std::pair<std::string, std::vector<ir::TypeP>> Parser::qualified_name() {
    std::vector<std::string> parts;
    std::vector<ir::TypeP> args;
    if (const std::string* tok = peek(); tok != nullptr && *tok == "::") ++cur_.at;
    while (true) {
        parts.push_back(take());
        if (const std::string* tok = peek(); tok != nullptr && *tok == "<") {
            ++cur_.at;
            args = template_args();
        }
        if (const std::string* tok = peek(); tok != nullptr && *tok == "::") {
            ++cur_.at;
            if (!parts.empty() && !args.empty()) {   // (an argument list in the middle: kept on its part)
                std::vector<std::string> spelled;
                for (const ir::TypeP& a : args) spelled.push_back(ir::to_text(*a));
                parts.back() += "<" + text::join(spelled, ", ") + ">";
                args.clear();
            }
            continue;
        }
        break;
    }
    return {text::join(parts, "::"), std::move(args)};
}

std::vector<ir::TypeP> Parser::template_args() {
    std::vector<ir::TypeP> args;
    while (true) {
        const std::string* tok = peek();
        if (tok == nullptr || *tok == ">") break;
        if (is_digit((*tok)[0]) || *tok == "true" || *tok == "false") {
            args.push_back(ir::class_type(take()));   // a non-type argument: kept as its spelling
        } else {
            args.push_back(type_());
        }
        if (const std::string* sep = peek(); sep != nullptr && *sep == ",") ++cur_.at;
    }
    expect(">");
    return args;
}

ir::TypeP Parser::declarator(ir::TypeP t) {
    std::vector<std::string> ops;
    while (const std::string* tok = peek()) {
        if (!is_pointer_op(*tok) && !is_qualifier(*tok)) break;
        std::string taken = take();
        if (is_pointer_op(taken)) ops.push_back(std::move(taken));
    }
    for (const std::string& op : ops) {
        ir::PtrType ptr;
        ptr.to = t;
        ptr.ref = op != "*";
        t = ir::make_type(ptr);
    }
    std::optional<std::vector<std::string>> inner;
    const std::string* open = peek();
    if (open != nullptr && *open == "(") {
        const std::string* first = peek(1);
        const std::string* second = peek(2);
        const bool nested = first != nullptr && (is_pointer_op(*first) || *first == "(" ||
                                                 (is_qualifier(*first) && second != nullptr && is_pointer_op(*second)));
        if (nested) {
            ++cur_.at;
            int depth = 1;
            const std::size_t start = cur_.at;
            while (depth > 0) {
                const std::string tok = take();
                if (tok == "(") ++depth;
                if (tok == ")") --depth;
            }
            inner.emplace(cur_.tokens.begin() + static_cast<std::ptrdiff_t>(start), cur_.tokens.begin() + static_cast<std::ptrdiff_t>(cur_.at - 1));
        }
    }
    t = suffixes(std::move(t));
    if (inner) {
        Cursor saved = std::move(cur_);
        cur_ = Cursor{std::move(*inner), 0};
        try {
            t = declarator(std::move(t));
        } catch (...) {
            cur_ = std::move(saved);
            throw;
        }
        cur_ = std::move(saved);
    }
    return t;
}

ir::TypeP Parser::suffixes(ir::TypeP t) {
    struct Suffix {
        bool is_array = false;
        std::optional<std::uint64_t> count;
        std::vector<ir::TypeP> params;
        bool variadic = false;
    };
    std::vector<Suffix> sufs;
    while (const std::string* tok = peek()) {
        if (*tok != "[" && *tok != "(") break;
        Suffix s;
        if (take() == "[") {
            s.is_array = true;
            if (const std::string* next = peek(); next != nullptr && *next != "]") {
                const std::string digits = take();
                s.count = text::parse_int<std::uint64_t>(digits);
                if (!s.count) unreadable("array size '" + digits + "'");
            }
            expect("]");
        } else {
            while (const std::string* next = peek()) {
                if (*next == ")") break;
                if (*next == "...") {
                    ++cur_.at;
                    s.variadic = true;
                } else {
                    s.params.push_back(type_());
                }
                if (const std::string* sep = peek(); sep != nullptr && *sep == ",") ++cur_.at;
            }
            expect(")");
            while (const std::string* q = peek()) {
                if (*q != "const" && *q != "volatile" && *q != "noexcept" && *q != "&" && *q != "&&" && *q != "__attribute__") break;
                ++cur_.at;
            }
            if (s.params.size() == 1 && s.params[0] == ir::void_type()) s.params.clear();
        }
        sufs.push_back(std::move(s));
    }
    for (auto it = sufs.rbegin(); it != sufs.rend(); ++it) {
        if (it->is_array) {
            t = ir::make_type(ir::ArrayType{.elem = t, .count = it->count});
        } else {
            t = fn_type(t, it->params, it->variadic);
        }
    }
    return t;
}

// ---- function types, decay, normal form ------------------------------------------------------------------------

ir::TypeP fn_type(const ir::TypeP& result, const std::vector<ir::TypeP>& params, bool variadic, const std::vector<std::string>& names) {
    ir::FnType fn;
    fn.conv = ir::CallingConvention::Cdecl;
    for (std::size_t i = 0; i < params.size(); ++i) {
        ir::FnParam p;
        p.type = params[i];
        if (i < names.size() && !names[i].empty()) p.name = names[i];
        fn.params.push_back(std::move(p));
    }
    fn.result.type = result;
    fn.variadic = variadic;
    return ir::make_type(std::move(fn));
}

ir::TypeP decay(const ir::TypeP& t) {
    if (!t) return t;
    if (std::holds_alternative<ir::FnType>(t->body)) return ir::make_type(ir::FnPtrType{.space = ir::AddressSpace::Flat, .fn = t});
    if (const auto* array = std::get_if<ir::ArrayType>(&t->body)) return ir::pointer_to(array->elem);
    return t;
}

ir::TypeP normalise(const ir::TypeP& t) {
    if (!t) return t;
    if (const auto* ptr = std::get_if<ir::PtrType>(&t->body)) {
        const ir::TypeP to = normalise(ptr->to);
        if (to && std::holds_alternative<ir::FnType>(to->body) && !ptr->ref) {
            return ir::make_type(ir::FnPtrType{.space = ir::AddressSpace::Flat, .fn = to});
        }
        ir::PtrType out = *ptr;
        out.to = to;
        return ir::make_type(std::move(out));
    }
    if (const auto* array = std::get_if<ir::ArrayType>(&t->body)) {
        ir::ArrayType out = *array;
        out.elem = normalise(array->elem);
        return ir::make_type(std::move(out));
    }
    if (const auto* fn = std::get_if<ir::FnType>(&t->body)) {
        ir::FnType out = *fn;
        for (ir::FnParam& p : out.params) p.type = normalise(p.type);
        out.result.type = normalise(out.result.type);
        return ir::make_type(std::move(out));
    }
    if (const auto* cls = std::get_if<ir::ClassType>(&t->body); cls != nullptr && !cls->args.empty()) {
        ir::ClassType out = *cls;
        for (ir::TypeP& a : out.args) a = normalise(a);
        return ir::make_type(std::move(out));
    }
    return t;
}

// ---- sizes -------------------------------------------------------------------------------------------------------

std::pair<std::uint64_t, std::uint64_t> Layouts::size_align(const ir::TypeP& t) const {
    if (!t) throw UnsupportedError("clang type", 0, "no size for an absent type");
    return std::visit(
        [&](const auto& body) -> std::pair<std::uint64_t, std::uint64_t> {
            using B = std::decay_t<decltype(body)>;
            if constexpr (std::is_same_v<B, ir::PrimType>) {
                std::uint64_t s = 0;
                switch (body.prim) {
                    case ir::Prim::Bool: case ir::Prim::I8: case ir::Prim::U8: case ir::Prim::Char: s = 1; break;
                    case ir::Prim::I16: case ir::Prim::U16: s = 2; break;
                    case ir::Prim::I32: case ir::Prim::U32: case ir::Prim::F32: s = 4; break;
                    case ir::Prim::I64: case ir::Prim::U64: case ir::Prim::F64: s = 8; break;
                    case ir::Prim::F80: s = 16; break;
                }
                return {s, s};
            } else if constexpr (std::is_same_v<B, ir::PtrType> || std::is_same_v<B, ir::FnPtrType>) {
                return {model_.pointer, model_.pointer};
            } else if constexpr (std::is_same_v<B, ir::ArrayType>) {
                const auto [s, a] = size_align(body.elem);
                return {s * body.count.value_or(0), a};
            } else if constexpr (std::is_same_v<B, ir::ClassType>) {
                const ir::TypeP record = record_(t);
                const auto* r = record ? std::get_if<ir::RecordType>(&record->body) : nullptr;
                if (r == nullptr) throw UnsupportedError("clang type", 0, "no layout for " + ir::to_text(*t));
                return {r->size, record_align(*r)};
            } else if constexpr (std::is_same_v<B, ir::RecordType>) {
                return {body.size, record_align(body)};
            } else if constexpr (std::is_same_v<B, ir::VoidType>) {
                return {1, 1};
            } else {
                throw UnsupportedError("clang type", 0, "no size for " + ir::to_text(*t));
            }
        },
        t->body);
}

std::uint64_t Layouts::record_align(const ir::RecordType& record) const {
    std::uint64_t widest = 1;
    for (const ir::RecordField& f : record.fields) widest = std::max(widest, align(f.type));
    return widest;
}

ir::TypeP Layouts::lay_out(const std::string& name, const std::vector<Field>& fields, bool is_union) const {
    std::uint64_t offset = 0, widest = 1;
    ir::RecordType record;
    record.name = name;
    for (const Field& f : fields) {
        const auto [s, a] = size_align(f.type);
        widest = std::max(widest, a);
        ir::RecordField placed;
        placed.name = f.name;
        placed.type = f.type;
        if (is_union) {
            placed.offset = 0;
            offset = std::max(offset, s);
        } else {
            offset = (offset + a - 1) / a * a;
            placed.offset = offset;
            offset += s;
        }
        record.fields.push_back(std::move(placed));
    }
    record.size = (offset + widest - 1) / widest * widest;
    record.role = is_union ? ir::RecordRole::Union : ir::RecordRole::Struct;
    record.packed = false;
    return ir::make_type(std::move(record));
}

}  // namespace cb::translate::frontends::clang::ctypes
