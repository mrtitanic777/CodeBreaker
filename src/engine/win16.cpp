#include "engine/win16.hpp"

#include <algorithm>
#include <any>
#include <chrono>
#include <format>
#include <mutex>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::engine::win16 {

using knowledge::Value;

namespace {

constexpr std::string_view kWin16Code = "engine.win16:1";   // the identity of the code these steps are

// ---- documents --------------------------------------------------------------------------------------------------------------

// A fact as a document: a text kept as text (the store keeps one so) is a document of text.
Value w16_doc(const facts::Value& fact, std::string_view what) {
    if (const auto* doc = std::get_if<Value>(&fact)) return *doc;
    if (const auto* text = std::get_if<std::string>(&fact)) return Value(*text);
    throw FormatError("engine fact", 0, std::format("{} is not a document", what));
}

const Value& w16_member(const Value& doc, std::string_view name) {
    const Value* v = doc.get(name);
    if (v == nullptr) throw FormatError("engine fact", 0, std::format("a document lacks '{}'", name));
    return *v;
}

std::string w16_text(const Value& v) {
    const std::string* t = v.text();
    if (t == nullptr) throw FormatError("engine fact", 0, "a document member is not text");
    return *t;
}

std::int64_t w16_int(const Value& v) {
    const auto i = v.integer();
    if (!i) throw FormatError("engine fact", 0, "a document member is not a number");
    return *i;
}

const Value::List& w16_list(const Value& v) {
    const Value::List* l = v.list();
    if (l == nullptr) throw FormatError("engine fact", 0, "a document member is not a list");
    return *l;
}

Value w16_texts(const std::vector<std::string>& items) {
    Value::List out;
    for (const std::string& s : items) out.emplace_back(s);
    return Value(std::move(out));
}

std::vector<std::string> w16_text_list(const Value& v) {
    std::vector<std::string> out;
    for (const Value& x : w16_list(v)) out.push_back(w16_text(x));
    return out;
}

facts::Value w16_fact(Value v) { return facts::Value(std::move(v)); }
Value w16_nullable(const std::optional<std::string>& s) { return s ? Value(*s) : Value(); }

std::string w16_hash(const facts::Value& v) { return facts::hash_of(facts::encode(v)); }

std::filesystem::path w16_binary(const facts::Value& binary) { return fs::from_text(w16_text(w16_member(w16_doc(binary, "the binary"), "path"))); }

bool w16_is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

// The words the segment table's kinds are spelled with are the loader's; the engine keeps them as they come.
Value w16_segment_value(const Segment& s) {
    return Value(Value::Map{{"idx", Value(s.idx)}, {"kind", Value(s.kind)}, {"file_off", Value(s.file_off)}, {"length", Value(s.length)},
                            {"flags", Value(s.flags)}, {"minalloc", Value(s.minalloc)}});
}

// What each function is, as ne.functions keeps it.
struct W16Function {
    std::int64_t va = 0;
    std::string name;
    std::optional<std::int64_t> seg, off;
    bool near = false;
    std::optional<std::string> stem;
};

std::map<std::string, W16Function> w16_functions_of(const Value& doc) {
    std::map<std::string, W16Function> out;
    if (doc.map() == nullptr) throw FormatError("engine fact", 0, "ne.functions is not a map");
    for (const auto& [key, f] : *doc.map()) {
        W16Function fn;
        fn.va = w16_int(w16_member(f, "va"));
        fn.name = w16_text(w16_member(f, "name"));
        if (const Value* s = f.get("seg"); s && s->integer()) fn.seg = *s->integer();
        if (const Value* o = f.get("off"); o && o->integer()) fn.off = *o->integer();
        fn.near = w16_member(f, "near").boolean().value_or(false);
        if (const Value* st = f.get("stem"); st && st->text()) fn.stem = *st->text();
        out.emplace(key, std::move(fn));
    }
    return out;
}

Module16 w16_module(Context& ctx) { return module_of(w16_doc(ctx.need("ne.module"), "ne.module")); }

// The folder a segment's functions are compiled in, its header beside them.
std::filesystem::path w16_folder(Context& ctx, build::functions::FunctionBuilder& builder, std::mutex& lock, std::int64_t seg, const std::string& header) {
    const std::filesystem::path d = ctx.work(std::filesystem::path("objects") / std::format("s{:02}", seg));
    const std::string want = build::functions::FunctionBuilder::header_digest(header);
    const std::lock_guard<std::mutex> guard(lock);
    if (builder.header_key(d) != want) builder.write_header(d, header);
    return d;
}

// The one step run of a gate batch is given: the build's folder, the gate's tables and each function's name.
struct W16GateHint {
    std::filesystem::path folder;
    Value tables;
    std::map<std::string, std::string> names;   // key -> function name
};

}  // namespace

// ---- the document of a module ---------------------------------------------------------------------------------------------

Value to_value(const Module16& m) {
    Value::List by_va, where;
    for (const auto& [va, name] : m.by_va) by_va.push_back(Value(Value::List{Value(va), Value(name)}));
    for (const auto& [va, p] : m.where) where.push_back(Value(Value::List{Value(va), Value(p.seg), Value(p.off)}));
    Value::Map key, texts, notes;
    for (const std::string& n : m.order) {   // (the order of the functions, then any others by name: a stable document)
        if (const auto k = m.key.find(n); k != m.key.end()) key.insert_or_assign(n, Value(k->second));
        if (const auto t = m.texts.find(n); t != m.texts.end()) texts.insert_or_assign(n, Value(t->second));
    }
    const std::map<std::string, std::string> rest_keys(m.key.begin(), m.key.end()), rest_texts(m.texts.begin(), m.texts.end());
    for (const auto& [n, k] : rest_keys) key.try_emplace(n, Value(k));
    for (const auto& [n, t] : rest_texts) texts.try_emplace(n, Value(t));
    for (const auto& [n, v] : m.notes) notes.insert_or_assign(n, v);
    return Value(Value::Map{{"header", Value(m.header)},
                            {"description", w16_texts(m.description)},
                            {"order", w16_texts(m.order)},
                            {"key", Value(std::move(key))},
                            {"all_keys", w16_texts(m.all_keys)},
                            {"texts", Value(std::move(texts))},
                            {"sig", Value(m.sig)},
                            {"by_va", Value(std::move(by_va))},
                            {"where", Value(std::move(where))},
                            {"near", w16_texts(std::vector<std::string>(m.near.begin(), m.near.end()))},
                            {"notes", Value(std::move(notes))},
                            {"wide", w16_texts(std::vector<std::string>(m.wide.begin(), m.wide.end()))}});
}

Module16 module_of(const Value& v) {
    Module16 m;
    m.header = w16_text(w16_member(v, "header"));
    m.description = w16_text_list(w16_member(v, "description"));
    m.order = w16_text_list(w16_member(v, "order"));
    m.all_keys = w16_text_list(w16_member(v, "all_keys"));
    m.sig = w16_text(w16_member(v, "sig"));
    const Value::Map* key = w16_member(v, "key").map();
    const Value::Map* texts = w16_member(v, "texts").map();
    const Value::Map* notes = w16_member(v, "notes").map();
    if (key == nullptr || texts == nullptr || notes == nullptr) throw FormatError("engine fact", 0, "a module document's key, texts or notes is not a map");
    for (const auto& [n, k] : *key) m.key.emplace(n, w16_text(k));
    for (const auto& [n, t] : *texts) m.texts.emplace(n, w16_text(t));
    for (const auto& [n, x] : *notes) m.notes.emplace(n, x);
    for (const Value& e : w16_list(w16_member(v, "by_va"))) m.by_va.emplace(w16_int(w16_list(e).at(0)), w16_text(w16_list(e).at(1)));
    for (const Value& e : w16_list(w16_member(v, "where")))
        m.where.emplace(w16_int(w16_list(e).at(0)), Place{w16_int(w16_list(e).at(1)), w16_int(w16_list(e).at(2))});
    for (const std::string& n : w16_text_list(w16_member(v, "near"))) m.near.insert(n);
    for (const std::string& n : w16_text_list(w16_member(v, "wide"))) m.wide.insert(n);
    return m;
}

std::vector<std::string> cflags(const std::vector<std::string>& platform_options) {
    std::vector<std::string> out;
    for (const std::string& f : platform_options)
        if (f != "-ms") out.push_back(f);   // (the module build compiles in the medium model)
    for (const char* extra : {"-mm", "-fpi87", "-os", "-nc=MCODE"}) out.emplace_back(extra);
    return out;
}

std::string named(std::string_view text, std::int64_t seg, const std::map<std::int64_t, Callee>& by_va) {
    constexpr std::string_view kMark = "func_0x";
    std::string out;
    std::size_t at = 0;
    while (at < text.size()) {
        const std::size_t found = text.find(kMark, at);
        if (found == std::string_view::npos) break;
        out.append(text.substr(at, found - at));
        const std::size_t digits = found + kMark.size();
        bool ok = (found == 0 || !w16_is_word(text[found - 1])) && digits + 8 <= text.size();
        std::int64_t va = 0;
        for (std::size_t i = 0; ok && i < 8; ++i) {   // eight lower-case hex digits, then the end of the word
            const char c = text[digits + i];
            const bool hex = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
            if (!hex) ok = false;
            else va = va * 16 + (c <= '9' ? c - '0' : c - 'a' + 10);
        }
        if (ok && digits + 8 < text.size() && w16_is_word(text[digits + 8])) ok = false;
        const std::size_t end = ok ? digits + 8 : found + kMark.size();
        const auto target = ok ? by_va.find(va) : by_va.end();
        if (target != by_va.end() && target->second.seg && (!target->second.near || *target->second.seg == seg)) out += target->second.name;
        else out.append(text.substr(found, end - found));
        at = end;
    }
    out.append(text.substr(std::min(at, text.size())));
    return out;
}

// ---- the steps ------------------------------------------------------------------------------------------------------------------

struct Win16::Impl : std::enable_shared_from_this<Win16::Impl> {
    Steps& steps;
    Engine& engine;
    Config config;
    std::vector<std::string> flags;   // the module's compiler options
    std::mutex folder_mutex;          // a segment's folder gets its header once, whichever functions are compiled in it

    Impl(Steps& s, Engine& e, Config c) : steps(s), engine(e), config(std::move(c)), flags(cflags(config.options)) {}

    Step base(std::string kind, std::vector<std::string> reads, StepFn fn, std::string doc) const {
        Step s;
        s.kind = std::move(kind);
        s.reads = std::move(reads);
        s.platform = std::string(kPlatform);
        s.fn = std::move(fn);
        s.doc = std::move(doc);
        s.code = {std::string(kWin16Code)};
        return s;
    }

    std::string compiler_stamp() const { return config.compiler_stamp ? config.compiler_stamp() : std::string(); }
    std::string flags_text() const { return text::join(flags, " "); }

    // {va: (name, seg, near)} of what ne.functions says
    static std::map<std::int64_t, Callee> callees(const std::map<std::string, W16Function>& funcs) {
        std::map<std::int64_t, Callee> out;
        for (const auto& [key, f] : funcs) out[f.va] = Callee{f.name, f.seg, f.near};
        return out;
    }

    void declare() {
        auto self = shared_from_this();

        steps.declare(base("ne.segments", {"binary"}, [self](Context& ctx) -> StepResult {
            Value::List rows;
            for (const Segment& s : self->config.stages.segments(w16_binary(ctx.need("binary")))) rows.push_back(w16_segment_value(s));
            return w16_fact(Value(std::move(rows)));
        }, "The segment table: number, kind, where in the file, length, flags (the loader's parts)."));

        {
            Step s = base("ne.module", {"binary"}, [self](Context& ctx) -> StepResult {
                const Module16 m = self->config.stages.prepare(w16_binary(ctx.need("binary")), ctx.work("decompiled"));
                return w16_fact(to_value(m));
            }, "The module decompiled and made compilable, as the lane prepares it (describe, the decompiler, c_text), with its description.");
            s.stamp = config.stages.module_stamp;
            steps.declare(std::move(s));
        }

        steps.declare(base("ne.description", {"ne.module"}, [](Context& ctx) -> StepResult {
            return w16_fact(w16_member(w16_doc(ctx.need("ne.module"), "ne.module"), "description"));
        }, "The description's lines (F a function, X an import, O a call's convention, bytes ...)."));

        steps.declare(base("ne.functions", {"ne.module"}, [](Context& ctx) -> StepResult {
            const Module16 m = w16_module(ctx);
            Value::Map out;
            std::map<std::int64_t, std::vector<std::pair<std::int64_t, std::string>>> by_segment;   // segment -> (offset, key)
            struct Row {
                std::string key;
                std::int64_t va = 0;
                std::string name;
                std::optional<Place> where;
                bool near = false;
            };
            std::vector<Row> rows;
            for (const auto& [va, name] : m.by_va) {
                const auto k = m.key.find(name);
                if (k == m.key.end()) throw FormatError("engine fact", 0, "the function '" + name + "' has no key");
                Row r{k->second, va, name, std::nullopt, m.near.contains(name)};
                if (const auto w = m.where.find(va); w != m.where.end()) {
                    r.where = w->second;
                    by_segment[w->second.seg].emplace_back(w->second.off, r.key);
                }
                rows.push_back(std::move(r));
            }
            std::map<std::string, std::string> stem;   // key -> "f0000": the order of the segment
            for (auto& [seg, offs] : by_segment) {
                std::sort(offs.begin(), offs.end());
                for (std::size_t i = 0; i < offs.size(); ++i) stem[offs[i].second] = std::format("f{:04}", i);
            }
            for (const Row& r : rows) {
                Value::Map f{{"va", Value(r.va)}, {"name", Value(r.name)}, {"seg", r.where ? Value(r.where->seg) : Value()},
                             {"off", r.where ? Value(r.where->off) : Value()}, {"near", Value(r.near)}};
                if (r.where) f.insert_or_assign("stem", Value(stem.at(r.key)));
                out.insert_or_assign(r.key, Value(std::move(f)));
            }
            return w16_fact(Value(std::move(out)));
        }, "Every function: {key: {va, name, seg, off, near, stem}} -- stem the name of its object in its segment's folder (f0000 ... in the order of the segment)."));

        steps.declare(base("ne.header", {"ne.module"}, [](Context& ctx) -> StepResult {
            return w16_fact(w16_member(w16_doc(ctx.need("ne.module"), "ne.module"), "header"));
        }, "The header of types and prototypes every function is compiled with."));

        steps.declare(base("ne.lines", {"ne.description"}, [](Context& ctx) -> StepResult {
            Value::Map out;
            const Value description = w16_doc(ctx.need("ne.description"), "ne.description");
            for (const Value& line : w16_list(description)) {
                const std::string line_text = w16_text(line);   // (the pieces are views of it)
                const std::vector<std::string_view> p = text::split(line_text, '\t');
                if ((p[0] == "F" || p[0] == "X") && p.size() > 2) out.insert_or_assign(std::string(p[2]), line);
            }
            return w16_fact(Value(std::move(out)));
        }, "{name: its line in the description} (F and X lines): how the gate answers a call to it."));

        steps.declare(base("function.c", {"ne.module", "ne.functions"}, [](Context& ctx) -> StepResult {
            const auto funcs = w16_functions_of(w16_doc(ctx.need("ne.functions"), "ne.functions"));
            const Value texts = w16_member(w16_doc(ctx.need("ne.module"), "ne.module"), "texts");
            const Value* t = texts.get(funcs.at(ctx.key()).name);
            return w16_fact(t ? *t : Value());
        }, "The function's C, as the decompiler and c_text made it (or as given by hand)."));

        steps.declare(base("function.notes", {"ne.module", "ne.functions"}, [](Context& ctx) -> StepResult {
            const auto funcs = w16_functions_of(w16_doc(ctx.need("ne.functions"), "ne.functions"));
            const Value notes = w16_member(w16_doc(ctx.need("ne.module"), "ne.module"), "notes");
            const Value* n = notes.get(funcs.at(ctx.key()).name);
            return w16_fact(n ? *n : Value());
        }, "What the decompiler said of the function besides its C (its pointers' sizes, the globals it touches, the structures it recovered), or null."));

        steps.declare(base("function.l2", {"function.notes"}, [](Context& ctx) -> StepResult {
            const Value notes = w16_doc(ctx.need("function.notes", ctx.key()), "function.notes");
            const Value* l2 = notes.get("l2");
            return w16_fact(l2 ? *l2 : Value());
        }, "The function's L2 document, the C of it is written from (or null: the decompiler made none)."));

        {
            Step s = base("function.source", {"function.c", "ne.functions"}, [self](Context& ctx) -> StepResult {
                const Value funcs_doc = w16_doc(ctx.need("ne.functions"), "ne.functions");
                const auto funcs = w16_functions_of(funcs_doc);
                const W16Function& f = funcs.at(ctx.key());
                const Value text = w16_doc(ctx.need("function.c", ctx.key()), "function.c");
                if (text.text() == nullptr || !f.seg) return w16_fact(Value());
                std::vector<std::string> flags = self->flags;
                flags.push_back(std::format("-nt=CODE{}", *f.seg));
                return w16_fact(Value(Value::Map{{"text", Value(named(*text.text(), *f.seg, callees(funcs)))},
                                                 {"seg", Value(*f.seg)},
                                                 {"stem", Value(f.stem.value_or(""))},
                                                 {"flags", w16_texts(flags)}}));
            }, "What the build compiles for the function: {text, seg, stem, flags}.");
            s.stamp = [self] { return self->flags_text(); };
            steps.declare(std::move(s));
        }

        {
            Step s = base("function.object", {"function.source", "ne.header"}, [self](Context& ctx) -> StepResult {
                const Value src = w16_doc(ctx.need("function.source", ctx.key()), "function.source");
                if (src.is_null()) return w16_fact(Value());
                const std::string header = w16_text(w16_doc(ctx.need("ne.header"), "ne.header"));
                const std::string text = w16_text(w16_member(src, "text"));
                const std::string stem = w16_text(w16_member(src, "stem"));
                const std::vector<std::string> flags = w16_text_list(w16_member(src, "flags"));
                const std::filesystem::path d = w16_folder(ctx, self->config.builder, self->folder_mutex, w16_int(w16_member(src, "seg")), header);
                const std::string hk = build::functions::FunctionBuilder::header_digest(header);
                auto compile = [&](const std::string& source) {
                    return self->config.builder.compile_kept(self->config.wcc, build::functions::adjust_flags(flags, source), d, stem, source, hk);
                };
                Value::Map out{{"flags", w16_texts(flags)}, {"text", Value(text)}};
                const build::functions::CompileOutcome made = compile(text);
                if (!made.why.empty()) {
                    out.insert_or_assign("error", Value(made.why));
                    const std::string stand_in = build::functions::stand_in(text);
                    out.insert_or_assign("stand_in_text", Value(stand_in));
                    out.insert_or_assign("stand_in", Value());
                    if (compile(stand_in).why.empty()) out.insert_or_assign("stand_in", Value(crypto::to_hex(fs::read_file(d / (stem + ".obj")))));
                } else {
                    out.insert_or_assign("obj", Value(crypto::to_hex(fs::read_file(d / (stem + ".obj")))));
                }
                return w16_fact(Value(std::move(out)));
            }, "The object file the compiler makes of the function: {flags, text, obj} -- or, when it is rejected, {error, stand_in_text, stand_in: the object of a stand-in with its signature (or null)}. Objects are bytes in hex. The compiler's work goes through the shared build cache, so a fresh state compiles nothing one compiled before.");
            s.stamp = [self] { return self->compiler_stamp(); };
            steps.declare(std::move(s));
        }

        {
            Step s = base("function.never_set", {"function.source", "ne.header"}, [self](Context& ctx) -> StepResult {
                const Value src = w16_doc(ctx.need("function.source", ctx.key()), "function.source");
                if (src.is_null()) return w16_fact(Value());
                const std::string header = w16_text(w16_doc(ctx.need("ne.header"), "ne.header"));
                const std::string text = w16_text(w16_member(src, "text"));
                const std::filesystem::path d = w16_folder(ctx, self->config.builder, self->folder_mutex, w16_int(w16_member(src, "seg")), header);
                std::string stem = "n" + ctx.key();   // (only its scratch file's name)
                std::replace(stem.begin(), stem.end(), ':', '_');
                const std::vector<std::string> names = self->config.builder.never_set_kept(
                    self->config.wcc, self->flags, d, stem, text, build::functions::FunctionBuilder::header_digest(header));
                return w16_fact(Value(Value::Map{{"flags", w16_texts(self->flags)}, {"text", Value(text)}, {"names", w16_texts(names)}}));
            }, "The variables the function reads and never sets (the compiler's flow check), with the options and text it was asked of: {flags, text, names}.");
            s.stamp = [self] { return self->compiler_stamp() + "|" + self->flags_text(); };
            steps.declare(std::move(s));
        }

        steps.declare(base("function.contract", {"function.c", "ne.functions", "ne.lines"}, [](Context& ctx) -> StepResult {
            const auto funcs = w16_functions_of(w16_doc(ctx.need("ne.functions"), "ne.functions"));
            const std::string name = funcs.at(ctx.key()).name;
            const Value lines = w16_doc(ctx.need("ne.lines"), "ne.lines");
            const Value c = w16_doc(ctx.need("function.c", ctx.key()), "function.c");
            // the names called: an identifier followed by an opening parenthesis (blanks allowed between), and the function itself
            std::set<std::string> called{name};
            if (const std::string* t = c.text()) {
                for (std::size_t i = 0; i < t->size();) {
                    if (!w16_is_word((*t)[i])) {
                        ++i;
                        continue;
                    }
                    std::size_t end = i;
                    while (end < t->size() && w16_is_word((*t)[end])) ++end;
                    const bool starts_a_name = !std::isdigit(static_cast<unsigned char>((*t)[i]));
                    std::size_t after = end;
                    while (after < t->size() && std::isspace(static_cast<unsigned char>((*t)[after]))) ++after;
                    if (starts_a_name && after < t->size() && (*t)[after] == '(') called.insert(t->substr(i, end - i));
                    i = end;
                }
            }
            std::vector<std::string> out;
            for (const std::string& n : called)
                if (const Value* l = lines.get(n)) out.push_back(w16_text(*l));
            std::sort(out.begin(), out.end());
            return w16_fact(w16_texts(out));
        }, "The description's lines for the function and each it calls -- the gate answers a call as the description says the callee does, so a callee described anew is a verdict to make again."));

        {
            Step s = base("function.gate", {"binary", "ne.header", "function.c", "function.contract"}, nullptr,
                          "The function's C run beside the original's: [\"equivalent\", \"\"] or [\"divergent\", how]. Made in a build (the hint: its folder and the gate's tables), so it is asked for by ne.image; one that could not be run is not kept.");
            s.batch_fn = [self](std::span<Context* const> ctxs) -> std::map<std::string, StepResult> {
                if (ctxs.empty()) return {};
                for (Context* c : ctxs) {
                    c->need("binary");
                    c->need("ne.header");
                    c->need("function.c", c->key());
                    c->need("function.contract", c->key());
                }
                const auto* hint = std::any_cast<W16GateHint>(&ctxs.front()->hint());
                if (hint == nullptr) throw UsageError("a gate verdict is made in a build of the module: ask for ne.image");
                const std::filesystem::path binary = w16_binary(ctxs.front()->need("binary"));
                std::map<std::string, std::string> by_name;   // name -> key
                std::vector<std::string> names;
                for (Context* c : ctxs) {
                    const std::string& n = hint->names.at(c->key());
                    by_name[n] = c->key();
                    names.push_back(n);
                }
                std::sort(names.begin(), names.end());
                std::map<std::string, StepResult> out;
                for (const GateAnswer& a : self->config.stages.gate(binary, hint->folder, names, hint->tables)) {
                    const auto k = by_name.find(a.name);
                    if (k == by_name.end()) continue;
                    auto verdict = [&](verify::Verdict v, const std::string& why) { return Value(Value::List{Value(std::string(verify::word(v))), Value(why)}); };
                    if (a.verdict == verify::Verdict::Equivalent) out[k->second] = w16_fact(verdict(verify::Verdict::Equivalent, ""));
                    else if (a.verdict == verify::Verdict::Divergent) out[k->second] = w16_fact(verdict(verify::Verdict::Divergent, a.why));
                    else out[k->second] = Unkept{w16_fact(verdict(verify::Verdict::Inconclusive, a.why))};   // (it could not be run: not kept)
                }
                for (Context* c : ctxs)
                    if (!out.contains(c->key()))
                        out[c->key()] = Unkept{w16_fact(Value(Value::List{Value(std::string(verify::word(verify::Verdict::Inconclusive))), Value("the checker gave no answer")}))};
                return out;
            };
            s.stamp = config.stages.gate_stamp;
            steps.declare(std::move(s));
        }

        {
            Step s = base("ne.image", {"binary", "ne.module", "ne.description", "ne.functions", "function.c", "function.object", "function.never_set", "function.gate"},
                          [self](Context& ctx) -> StepResult { return self->image(ctx); },
                          "The module linked: {name, report, rejected, gated, gate_set, served, image: its file in the build folder, image_sha256} -- module build's build of a 16-bit module, made of the facts above.");
            s.stamp = config.stages.image_stamp;
            steps.declare(std::move(s));
        }
    }

    // ne.image
    StepResult image(Context& ctx) {
        const Stages& stages = config.stages;
        const std::filesystem::path binary = w16_binary(ctx.need("binary"));
        const Module16 m0 = w16_module(ctx);
        const auto funcs = w16_functions_of(w16_doc(ctx.need("ne.functions"), "ne.functions"));
        std::vector<std::string> keys;
        for (const auto& [k, f] : funcs)
            if (f.seg) keys.push_back(k);
        const auto texts = ctx.need_many("function.c", keys);
        const auto objects = ctx.need_many("function.object", keys);
        const auto never = ctx.need_many("function.never_set", keys);
        const std::filesystem::path folder = ctx.work("build");
        Module16 m = m0;
        std::map<std::string, std::string> key_of;   // name -> key
        for (const auto& [k, f] : funcs) key_of[f.name] = k;
        for (const std::string& k : keys) {
            const Value t = w16_doc(texts.at(k), "function.c");
            if (t.text() != nullptr) m.texts[funcs.at(k).name] = *t.text();
        }
        // what the engine already made, offered to the link (as the compiler's answers)
        const std::string hk = build::functions::FunctionBuilder::header_digest(m.header);
        std::map<std::string, build::functions::ServedObject> served_objects;
        std::map<std::string, std::vector<std::string>> served_never;
        for (const auto& [k, o] : objects) {
            const Value ov = w16_doc(o, "function.object");
            if (ov.is_null()) continue;
            const std::vector<std::string> obj_flags = w16_text_list(w16_member(ov, "flags"));
            const std::string text = w16_text(w16_member(ov, "text"));
            if (const Value* error = ov.get("error")) {
                served_objects[build::functions::FunctionBuilder::served_key(hk, obj_flags, text)] = {w16_text(*error), {}};
                const Value* stand_in = ov.get("stand_in");
                if (stand_in != nullptr && stand_in->text() != nullptr)
                    served_objects[build::functions::FunctionBuilder::served_key(hk, obj_flags, w16_text(w16_member(ov, "stand_in_text")))] = {"", crypto::from_hex(*stand_in->text())};
            } else {
                served_objects[build::functions::FunctionBuilder::served_key(hk, obj_flags, text)] = {"", crypto::from_hex(w16_text(w16_member(ov, "obj")))};
            }
        }
        for (const auto& [k, n] : never) {
            const Value nv = w16_doc(n, "function.never_set");
            if (nv.is_null()) continue;
            served_never[build::functions::FunctionBuilder::served_key(hk, w16_text_list(w16_member(nv, "flags")), w16_text(w16_member(nv, "text")))] = w16_text_list(w16_member(nv, "names"));
        }
        // the verdicts already known, of the functions the gate ran last time
        std::map<std::string, std::string> gated;
        if (const auto previous = ctx.previous()) {
            if (const auto* prev = std::get_if<Value>(&*previous))
                if (const Value* set = prev->get("gate_set"); set && set->list())
                    for (const Value& n : *set->list()) {
                        const std::string name = w16_text(n);
                        const auto k = key_of.find(name);
                        if (k == key_of.end()) continue;
                        const auto v = ctx.peek("function.gate", k->second);
                        if (!v) continue;
                        const Value::List& pair = w16_list(w16_doc(*v, "function.gate"));
                        if (verify::parse_verdict(w16_text(pair.at(0))) == verify::Verdict::Divergent) gated[name] = w16_text(pair.at(1));
                    }
        }
        bool gate_now = config.gate;
        std::vector<std::string> gate_set;
        std::int64_t served = 0, compiled = 0;
        const Value tables = stages.gate_tables ? stages.gate_tables(m) : Value();
        LinkOutcome b;
        std::optional<std::filesystem::path> image_file;
        std::string image_sha;
        while (true) {
            config.builder.serve(served_objects, served_never);
            const build::functions::ServedCounts before = config.builder.served_counts();
            try {
                b = stages.link(LinkRequest{m, binary, folder, gated, m.all_keys.size()});
            } catch (...) {
                config.builder.unserve();
                throw;
            }
            const build::functions::ServedCounts after = config.builder.served_counts();
            config.builder.unserve();
            served = after.served - before.served;
            compiled = after.compiled - before.compiled;
            if (!b.resident_overflow.empty()) {   // (as the module build: again -- and gated still, if this build was to be: the overflow is found before the gate runs)
                for (const auto& [n, why] : b.gated) gated[n] = why;
                for (const auto& [n, why] : b.resident_overflow) gated[n] = why;
                continue;
            }
            if (!b.image) break;
            const std::string name = text::lower(fs::to_text(binary.filename()));
            image_file = folder / name;
            std::filesystem::create_directories(folder);
            std::filesystem::copy_file(*b.image, *image_file, std::filesystem::copy_options::overwrite_existing);
            if (stages.whole_file_image) stages.whole_file_image(binary, *image_file);
            if (!gate_now) break;
            gate_now = false;
            std::set<std::string> c(b.placed_c);
            c.insert(b.moved_c.begin(), b.moved_c.end());
            std::vector<std::string> keys_of_c;
            W16GateHint hint{folder, tables, {}};
            for (const std::string& n : c)
                if (const auto k = key_of.find(n); k != key_of.end()) {
                    keys_of_c.push_back(k->second);
                    hint.names[k->second] = n;
                }
            const auto verdicts = ctx.need_many("function.gate", keys_of_c, std::any(hint));
            std::map<std::string, std::string> differ;
            std::vector<std::pair<std::string, std::string>> failed;
            for (const std::string& n : c) {
                const auto k = key_of.find(n);
                if (k == key_of.end()) continue;
                const Value::List& v = w16_list(w16_doc(verdicts.at(k->second), "function.gate"));
                const std::optional<verify::Verdict> verdict = verify::parse_verdict(w16_text(v.at(0)));
                const std::string why = w16_text(v.at(1));
                if (verdict == verify::Verdict::Divergent) differ[n] = why;
                if (verdict == verify::Verdict::Inconclusive && why.starts_with("the checker failed")) failed.emplace_back(n, why);
            }
            if (!failed.empty()) b.gate_error = std::format("{} functions ({})", failed.size(), failed.front().second.substr(0, 120));
            try {
                if (stages.contract)
                    for (const auto& [n, why] : stages.contract(binary, folder, *image_file, std::vector<std::string>(c.begin(), c.end())))
                        differ.try_emplace(n, "its calls are not the original's: " + why);
            } catch (const Error& e) {   // isolation boundary: a check that could not be made is a note of the build
                b.contract_error = std::format("{}: {}", e.kind(), e.what());
            }
            std::set<std::string> now(c);
            for (const auto& [n, why] : gated) now.insert(n);
            gate_set.assign(now.begin(), now.end());
            if (!differ.empty()) {
                for (const auto& [n, why] : differ) gated[n] = why;
                continue;
            }
            break;
        }
        Value::Map gated_out, rejected_out;
        std::map<std::string, std::string> all_gated = gated;   // (what the link left as the original's, besides what it was told to)
        for (const auto& [n, why] : b.gated) all_gated[n] = why;
        for (const auto& [n, why] : all_gated) gated_out.insert_or_assign(n, Value(why));
        for (const auto& [n, why] : b.rejected) rejected_out.insert_or_assign(n, Value(why));
        Value file, sha;
        if (b.image && image_file) {
            file = Value(fs::to_text(image_file->filename()));
            sha = Value(crypto::to_hex(crypto::sha256(fs::read_file(*image_file))));
        }
        const std::string report = stages.check ? stages.check(b) : std::string();
        return w16_fact(Value(Value::Map{{"name", Value(b.name)}, {"gated", Value(std::move(gated_out))}, {"gate_set", w16_texts(gate_set)},
                                         {"rejected", Value(std::move(rejected_out))},
                                         {"served", Value(Value::Map{{"served", Value(served)}, {"compiled", Value(compiled)}})},
                                         {"image", file}, {"image_sha256", sha}, {"report", Value(report)}}));
    }
};

Win16::Win16(Steps& steps, Engine& engine, Config config) : impl_(std::make_shared<Impl>(steps, engine, std::move(config))) {
    const Stages& st = impl_->config.stages;
    if (!st.segments || !st.prepare || !st.link) throw UsageError("the 16-bit steps need the segments, prepare and link stages");
    impl_->declare();
}

Win16::~Win16() = default;

Built16 Win16::build(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, const Run::Log& log) {
    const std::string name = text::lower(fs::to_text(module.filename()));
    const auto started = std::chrono::steady_clock::now();
    Engine& engine = impl_->engine;
    const auto db = engine.open(module);
    if (log) log(std::format("{}: on the engine ({})", name, fs::to_text(db->path())));
    facts::Value got = engine.get(db, "ne.image", "", log);
    const std::filesystem::path folder = facts::Databases::work_folder(*db, "build");
    if (const Value* file = w16_doc(got, "ne.image").get("image"); file && file->text() && !std::filesystem::is_regular_file(folder / *file->text())) {
        engine.forget(*db, "ne.image");   // the build folder was cleared since the fact was made: made again
        got = engine.get(db, "ne.image", "", log);
    }
    if (log) {
        std::vector<Run::Count> made = engine.last_run(*db);
        std::sort(made.begin(), made.end(), [](const Run::Count& a, const Run::Count& b) { return a.kind < b.kind; });
        std::string counts;
        for (const Run::Count& c : made) counts += std::format("{}{} {}/{}", counts.empty() ? "" : ", ", c.kind, c.kept, c.made);
        log(std::format("{}: {:.1f}s; facts kept/made: {}", name, std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count(), counts));
    }
    const Value& doc = w16_doc(got, "ne.image");
    Built16 out;
    out.name = w16_text(w16_member(doc, "name"));
    out.report = w16_text(w16_member(doc, "report"));
    for (const auto& [n, why] : *w16_member(doc, "rejected").map()) out.rejected[n] = w16_text(why);
    for (const auto& [n, why] : *w16_member(doc, "gated").map()) out.gated[n] = w16_text(why);
    out.gate_set = w16_text_list(w16_member(doc, "gate_set"));
    out.served = w16_int(w16_member(w16_member(doc, "served"), "served"));
    out.compiled = w16_int(w16_member(w16_member(doc, "served"), "compiled"));
    if (const std::string* file = w16_member(doc, "image").text()) {
        out.image = folder / *file;
        if (outdir) {
            std::filesystem::create_directories(*outdir);
            std::filesystem::copy_file(folder / *file, *outdir / *file, std::filesystem::copy_options::overwrite_existing);
            out.image = *outdir / *file;
        }
    }
    return out;
}

}  // namespace cb::engine::win16
