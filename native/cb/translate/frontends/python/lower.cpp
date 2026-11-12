// cb-unity: alone (file-local helpers named for this module)
// The way in: refusals, the L2 types of the Python model, the readers of the source text, FnLower, and lowering a program's
// functions with the refusal that a call of a refused function brings.
#include "cb/translate/frontends/python/lower.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

// ---- refusals ---------------------------------------------------------------------------------------------------------------

std::string_view refusal_class_word(RefusalClass cls) {
    switch (cls) {
        case RefusalClass::Untyped: return "untyped";
        case RefusalClass::Conflict: return "conflict";
        case RefusalClass::IntRange: return "int-range";
        case RefusalClass::Dynamic: return "dynamic";
        case RefusalClass::Stdlib: return "stdlib";
        case RefusalClass::Extern: return "extern";
        case RefusalClass::Syntax: return "syntax";
        case RefusalClass::Unresolved: return "unresolved";
        case RefusalClass::CallsRefused: return "calls-refused";
        case RefusalClass::FrontendError: return "frontend-error";
    }
    return "?";
}

std::optional<RefusalClass> refusal_class_from_word(std::string_view word) {
    for (const RefusalClass cls : {RefusalClass::Untyped, RefusalClass::Conflict, RefusalClass::IntRange, RefusalClass::Dynamic, RefusalClass::Stdlib,
                                   RefusalClass::Extern, RefusalClass::Syntax, RefusalClass::Unresolved, RefusalClass::CallsRefused,
                                   RefusalClass::FrontendError})
        if (refusal_class_word(cls) == word) return cls;
    return std::nullopt;
}

RefusalClass refusal_class_of(infer::IssueClass cls) { return *refusal_class_from_word(infer::issue_class_word(cls)); }

std::string Refusal::reason() const {
    return detail.empty() ? std::string(refusal_class_word(cls)) : std::format("{}:{}", refusal_class_word(cls), detail);
}

Refused::Refused(RefusalClass cls, std::string detail, std::uint32_t line)
    : Error(Refusal{cls, detail, line}.reason()), refusal_{cls, std::move(detail), line} {}

Refused::Refused(const infer::Issue& issue) : Refused(refusal_class_of(issue.cls), issue.detail, issue.line) {}

// ---- the L2 types of the Python model -----------------------------------------------------------------------------------------

const TypeP& i64_type() {
    static const TypeP t = ir::prim_type(ir::Prim::I64);
    return t;
}
const TypeP& bool_type() {
    static const TypeP t = ir::prim_type(ir::Prim::Bool);
    return t;
}
const TypeP& f64_type() {
    static const TypeP t = ir::prim_type(ir::Prim::F64);
    return t;
}
const TypeP& none_type() { return ir::void_type(); }
const TypeP& str_type() {
    static const TypeP t = ir::class_type("py.str");
    return t;
}
const TypeP& bytes_type() {
    static const TypeP t = ir::class_type("py.bytes");
    return t;
}
TypeP py_class(std::string_view name, std::vector<TypeP> args) { return ir::class_type(std::string(name), std::move(args)); }
TypeP optional_type(const TypeP& inner) { return py_class("py.optional", {inner}); }
const ir::ClassType* class_type_of(const TypeP& t) { return t ? std::get_if<ir::ClassType>(&t->body) : nullptr; }
bool names_class(const TypeP& t, std::string_view name) {
    const ir::ClassType* c = class_type_of(t);
    return c != nullptr && c->name == name;
}
bool is_opt(const TypeP& t) { return names_class(t, "py.optional"); }
bool is_ref_class(const TypeP& t) {
    const ir::ClassType* c = class_type_of(t);
    return c != nullptr && c->ref.value_or(false);
}
bool is_record_class(const TypeP& t) {
    const ir::ClassType* c = class_type_of(t);
    return c != nullptr && c->record.value_or(false);
}
std::string_view kind_of(const TypeP& t) { return t ? t->kind() : std::string_view(); }

// ---- the text of the source ------------------------------------------------------------------------------------------------------

std::string clean_docstring(std::string_view raw) {
    // tabs to the next multiple of eight, as str.expandtabs does
    std::string text;
    std::size_t column = 0;
    for (const char c : raw) {
        if (c == '\t') {
            const std::size_t n = 8 - column % 8;
            text.append(n, ' ');
            column += n;
        } else if (c == '\n' || c == '\r') {
            text += c;
            column = 0;
        } else {
            text += c;
            if ((static_cast<unsigned char>(c) & 0xC0) != 0x80) ++column;   // one column per character
        }
    }
    std::vector<std::string> lines;
    for (const std::string_view line : text::lines(text)) lines.emplace_back(line);
    if (lines.empty()) lines.emplace_back();
    // the margin: the indentation every line after the first has, blank lines apart
    std::size_t margin = std::string::npos;
    for (std::size_t i = 1; i < lines.size(); ++i) {
        const std::size_t lead = lines[i].find_first_not_of(' ');
        if (lead != std::string::npos) margin = std::min(margin, lead);
    }
    const std::size_t first_lead = lines[0].find_first_not_of(' ');
    lines[0] = first_lead == std::string::npos ? std::string() : lines[0].substr(first_lead);
    if (margin != std::string::npos)
        for (std::size_t i = 1; i < lines.size(); ++i) lines[i] = lines[i].size() > margin ? lines[i].substr(margin) : std::string();
    while (!lines.empty() && lines.back().empty()) lines.pop_back();
    std::size_t first = 0;
    while (first < lines.size() && lines[first].empty()) ++first;
    std::string out;
    for (std::size_t i = first; i < lines.size(); ++i) {
        if (i > first) out += '\n';
        out += lines[i];
    }
    return out;
}

std::optional<std::string> source_segment(std::string_view source, const syntax::Pos& pos) {
    if (pos.lineno <= 0 || pos.end_lineno <= 0) return std::nullopt;
    std::vector<std::size_t> starts = {0};   // where each line starts (\n, \r and \r\n end a line)
    for (std::size_t i = 0; i < source.size(); ++i) {
        if (source[i] == '\r') {
            if (i + 1 < source.size() && source[i + 1] == '\n') ++i;
            starts.push_back(i + 1);
        } else if (source[i] == '\n') {
            starts.push_back(i + 1);
        }
    }
    const std::size_t first_line = static_cast<std::size_t>(pos.lineno - 1), last_line = static_cast<std::size_t>(pos.end_lineno - 1);
    if (last_line >= starts.size()) return std::nullopt;
    const std::size_t from = starts[first_line] + static_cast<std::size_t>(pos.col_offset);
    const std::size_t to = starts[last_line] + static_cast<std::size_t>(pos.end_col_offset);
    if (from > source.size() || to > source.size() || to < from) return std::nullopt;
    return std::string(source.substr(from, to - from));
}

std::string default_text(const syntax::ConstValue& value) {
    using K = syntax::ConstValue::Kind;
    const auto string_text = [](std::string_view s) { return nlohmann::json(std::string(s)).dump(-1, ' ', true); };   // json.dumps: ASCII only
    switch (value.kind) {
        case K::None: return "null";
        case K::True: return "true";
        case K::False: return "false";
        case K::Int: return value.fits_i64 ? std::to_string(value.i64) : value.text;
        case K::Float: return nlohmann::json(value.f).dump();
        case K::Str: return string_text(value.text);
        case K::Bytes: return "{\"bytes\": " + string_text(text::from_latin1(as_bytes(value.text))) + "}";
        default: return "null";
    }
}

// ---- one function, from outside --------------------------------------------------------------------------------------------------

FnLower::FnLower(Context& context, const Fn& function, infer::Eval& evaluation) : impl_(std::make_unique<Lowerer>(context, function, evaluation)) {}
FnLower::FnLower(Context& context, const Module& module, infer::Eval& evaluation) : impl_(std::make_unique<Lowerer>(context, module, evaluation)) {}
FnLower::~FnLower() = default;

ir::Node* FnLower::lower() { return impl_->lower(); }
ir::Node* FnLower::value_of(const syntax::Expr* expression, const ir::TypeP& want) { return impl_->value_of(expression, want); }
bool FnLower::has_pending() const { return !impl_->pre.empty(); }
const std::set<std::pair<std::string, std::string>>& FnLower::globals_used() const { return impl_->globals_used; }
const std::set<std::string>& FnLower::calls() const { return impl_->calls; }
const std::set<std::string>& FnLower::classes_made() const { return impl_->classes_made; }

// ---- lowering a program's functions ----------------------------------------------------------------------------------------------

namespace {

// What a front end error says of itself, for the reason.
std::string failure_text(const std::exception& e) {
    const auto* known = dynamic_cast<const Error*>(&e);
    return std::format("{}: {}", known != nullptr ? std::string(known->kind()) : std::string("error"), e.what());
}

}  // namespace

OrderedMap<std::string, Lowered> lower_functions(Context& context, std::span<Fn* const> functions) {
    OrderedMap<std::string, Lowered> out;
    Program& program = context.program();
    for (Fn* f : functions) {
        Lowered& slot = out[f->qname];
        const std::uint32_t line = static_cast<std::uint32_t>(f->node->pos.lineno);
        // the isolation boundary: a function that cannot be run or lowered is that function's refusal, and the next goes on
        std::shared_ptr<infer::Eval> evaluation;
        try {
            evaluation = program.evaluate([&] {
                auto ev = std::make_shared<infer::Eval>(program, f, /*record=*/true, context.library());
                ev->run();
                return ev;
            });
        } catch (const std::exception& e) {
            slot.refused = Refusal{RefusalClass::FrontendError, failure_text(e), line};
            continue;
        }
        slot.lowering = std::make_shared<FnLower>(context, *f, *evaluation);
        slot.lowering->keep(evaluation);
        try {
            slot.decl = slot.lowering->lower();
        } catch (const Refused& r) {
            slot.refused = r.refusal();
        } catch (const std::exception& e) {   // a front end bug: refused, with the error
            slot.refused = Refusal{RefusalClass::FrontendError, failure_text(e), line};
        }
    }
    return out;
}

void close_refusals(OrderedMap<std::string, Lowered>& lowered) {
    // The part of a qualified name after its module and class are taken off ("pkg.mod.Class.method" -> "Class.method", "pkg.mod.f" -> "f").
    const auto after_module = [](const std::string& qname) {
        const std::size_t first = qname.find('.');
        const std::size_t second = first == std::string::npos ? first : qname.find('.', first + 1);
        return second != std::string::npos ? qname.substr(second + 1) : qname.substr(first == std::string::npos ? 0 : first + 1);
    };
    const auto last_part = [](const std::string& qname) { return qname.substr(qname.rfind('.') == std::string::npos ? 0 : qname.rfind('.') + 1); };
    for (bool changed = true; changed;) {
        changed = false;
        for (auto& [qname, slot] : lowered) {
            if (slot.is_refused() || slot.lowering == nullptr) continue;
            const std::uint32_t line = slot.decl != nullptr ? slot.decl->line : 0;
            for (const std::string& callee : slot.lowering->calls()) {   // (sorted: the first refused one is the reason)
                const Lowered* got = lowered.find(callee);
                if (got == nullptr) {
                    slot.refused = Refusal{RefusalClass::CallsRefused, last_part(callee) + " (not in the translation)", line};
                } else if (got->is_refused()) {
                    slot.refused = Refusal{RefusalClass::CallsRefused, std::format("{} ({})", after_module(callee), got->refused->reason().substr(0, 60)), line};
                } else {
                    continue;
                }
                slot.decl = nullptr;
                changed = true;
                break;
            }
        }
    }
}

}  // namespace cb::translate::frontends::python::lower
