#include "decompile/conventions.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <memory>
#include <mutex>

#include <nlohmann/json.hpp>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"

namespace cb::decompile::conventions {
namespace {

namespace stdfs = std::filesystem;
using std::string;
using std::string_view;

// ---- a small XML reader: elements, attributes, children; comments, <?..?> and CDATA are skipped (the specs' only text is p-code) ----

struct Element {
    string name;
    std::vector<std::pair<string, string>> attributes;
    std::vector<Element> children;

    string_view attribute(string_view key) const {
        for (const auto& [k, v] : attributes)
            if (k == key) return v;
        return {};
    }
    bool has(string_view key) const {
        return std::any_of(attributes.begin(), attributes.end(), [&](const auto& a) { return a.first == key; });
    }
    template <class F>
    void each_descendant(string_view tag, F&& f) const {   // depth first, in document order
        for (const Element& c : children) {
            if (c.name == tag) f(c);
            c.each_descendant(tag, f);
        }
    }
    const Element* child(string_view tag) const {
        for (const Element& c : children)
            if (c.name == tag) return &c;
        return nullptr;
    }
};

class Reader {
public:
    explicit Reader(string_view text) : t_(text) {}
    Element document() {
        Element root;
        root.name = "";
        while (skip_misc(), p_ < t_.size()) root.children.push_back(element());
        return root;
    }

private:
    string_view t_;
    std::size_t p_ = 0;

    bool at(string_view s) const { return t_.substr(p_, s.size()) == s; }
    void skip_to(string_view end) {
        const std::size_t e = t_.find(end, p_);
        p_ = e == string_view::npos ? t_.size() : e + end.size();
    }
    void skip_space() {
        while (p_ < t_.size() && std::isspace(static_cast<unsigned char>(t_[p_]))) ++p_;
    }
    // whitespace, comments, declarations, and text between elements
    void skip_misc() {
        for (;;) {
            skip_space();
            if (at("<!--")) skip_to("-->");
            else if (at("<?")) skip_to("?>");
            else if (at("<!DOCTYPE")) skip_to(">");
            else return;
        }
    }
    static bool name_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == ':' || c == '.'; }
    string name() {
        const std::size_t s = p_;
        while (p_ < t_.size() && name_char(t_[p_])) ++p_;
        if (s == p_) throw ParseError("<xml>", 0, 0, "an XML name was expected");
        return string(t_.substr(s, p_ - s));
    }
    static string unescape(string v) {
        constexpr std::pair<string_view, char> table[] = {{"&lt;", '<'}, {"&gt;", '>'}, {"&amp;", '&'}, {"&quot;", '"'}, {"&apos;", '\''}};
        for (const auto& [from, to] : table)
            for (std::size_t i = v.find(from); i != string::npos; i = v.find(from, i + 1)) v.replace(i, from.size(), 1, to);
        return v;
    }
    Element element() {
        if (!at("<")) throw ParseError("<xml>", 0, 0, "an element was expected");
        ++p_;
        Element e;
        e.name = name();
        for (;;) {
            skip_space();
            if (at("/>")) { p_ += 2; return e; }
            if (at(">")) { ++p_; break; }
            string key = name();
            skip_space();
            if (!at("=")) throw ParseError("<xml>", 0, 0, "an attribute value was expected");
            ++p_;
            skip_space();
            const char quote = p_ < t_.size() ? t_[p_] : '\0';
            if (quote != '"' && quote != '\'') throw ParseError("<xml>", 0, 0, "a quoted attribute value was expected");
            const std::size_t close = t_.find(quote, p_ + 1);
            if (close == string_view::npos) throw ParseError("<xml>", 0, 0, "an unterminated attribute value");
            e.attributes.emplace_back(std::move(key), unescape(string(t_.substr(p_ + 1, close - p_ - 1))));
            p_ = close + 1;
        }
        for (;;) {   // content
            while (p_ < t_.size() && t_[p_] != '<') ++p_;   // text between elements is not kept
            if (p_ >= t_.size()) throw ParseError("<xml>", 0, 0, "an element is not closed: " + e.name);
            if (at("<!--")) skip_to("-->");
            else if (at("<![CDATA[")) skip_to("]]>");
            else if (at("<?")) skip_to("?>");
            else if (at("</")) {
                p_ += 2;
                const string close = name();
                skip_space();
                if (at(">")) ++p_;
                if (close != e.name) throw ParseError("<xml>", 0, 0, "</" + close + "> closes <" + e.name + ">");
                return e;
            } else e.children.push_back(element());
        }
    }
};

Element read_xml(const stdfs::path& file) {
    const string text = fs::read_text(file);
    return Reader(text).document();
}

// ---- cspec to models ----

unsigned number(string_view v, unsigned fallback) {
    unsigned out = fallback;
    if (!v.empty()) std::from_chars(v.data(), v.data() + v.size(), out);
    return out;
}

std::optional<int> signed_number(string_view v) {
    int out = 0;
    if (v.empty() || std::from_chars(v.data(), v.data() + v.size(), out).ec != std::errc()) return std::nullopt;
    return out;
}

Model read_model(const Element& proto, string_view spec, bool is_default) {
    Model m;
    m.name = string(proto.attribute("name"));
    m.spec = string(spec);
    m.is_default = is_default;
    m.stack_shift = signed_number(proto.attribute("stackshift")).value_or(0);
    m.extra_pop = signed_number(proto.attribute("extrapop"));   // "unknown" is not a number
    m.has_this = proto.attribute("hasthis") == "true" || m.name == "__thiscall";
    if (const Element* input = proto.child("input")) {
        input->each_descendant("pentry", [&](const Element& pentry) {
            const bool is_float = pentry.attribute("metatype") == "float";
            const unsigned min = number(pentry.attribute("minsize"), 1), max = number(pentry.attribute("maxsize"), 8);
            if (const Element* reg = pentry.child("register")) {
                m.reg_args.push_back({string(reg->attribute("name")), min, max, is_float});
            } else if (const Element* addr = pentry.child("addr")) {
                if (addr->attribute("space") == "stack" && !m.stack_args_offset) {
                    m.stack_args_offset = signed_number(addr->attribute("offset")).value_or(0);
                    m.stack_align = number(pentry.attribute("align"), 0);
                }
            }
        });
    }
    if (const Element* output = proto.child("output")) {
        output->each_descendant("pentry", [&](const Element& pentry) {
            ResultEntry r;
            r.min_size = number(pentry.attribute("minsize"), 1);
            r.max_size = number(pentry.attribute("maxsize"), 8);
            r.is_float = pentry.attribute("metatype") == "float";
            if (const Element* reg = pentry.child("register")) {
                r.reg = string(reg->attribute("name"));
            } else if (const Element* addr = pentry.child("addr"); addr && addr->attribute("space") == "join") {
                r.reg = string(addr->attribute("piece1")) + ":" + string(addr->attribute("piece2"));
            } else {
                return;
            }
            m.results.push_back(std::move(r));
        });
    }
    auto registers = [&](string_view tag, std::vector<string>& into) {
        if (const Element* list = proto.child(tag))
            for (const Element& c : list->children)
                if (c.name == "register") into.emplace_back(c.attribute("name"));
    };
    registers("unaffected", m.unaffected);
    registers("killedbycall", m.killed);
    // The Pascal models (callee pops everything; every x86-16 "__stdcall16*" and "__fastcall16*") push left to right.
    if (const auto parsed = parse(m.name); parsed && parsed->distance != Distance::Flat &&
                                           (parsed->convention == Convention::Stdcall || parsed->convention == Convention::Fastcall))
        m.stack_order = Order::LeftToRight;
    return m;
}

std::vector<Model> read_spec(const stdfs::path& file) {
    const Element document = read_xml(file);
    std::vector<Model> models;
    const string stem = fs::to_text(file.stem());
    for (const Element& spec : document.children) {
        if (spec.name != "compiler_spec") continue;
        for (const Element& c : spec.children) {
            if (c.name == "default_proto")
                for (const Element& p : c.children)
                    if (p.name == "prototype") models.push_back(read_model(p, stem, true));
            if (c.name == "prototype") models.push_back(read_model(c, stem, false));
        }
    }
    return models;
}

stdfs::path default_folder() {
    if (const std::optional<string> env = fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

}  // namespace

// ---- names ----

namespace {
constexpr struct { Convention convention; string_view base; bool has_distance; } kBases[] = {
    {Convention::Cdecl, "__cdecl", true},       {Convention::Stdcall, "__stdcall", true},
    {Convention::Fastcall, "__fastcall", true}, {Convention::Thiscall, "__thiscall", false},
    {Convention::Regcall, "__regcall", false},  {Convention::Interrupt, "__int16", false},
    {Convention::InterruptKeepDs, "__int16keepds", false},
};
}  // namespace

string name_of(Convention convention, Distance distance) {
    for (const auto& b : kBases)
        if (b.convention == convention) {
            string out(b.base);
            if (b.has_distance && distance != Distance::Flat) out += distance == Distance::Far ? "16far" : "16near";
            return out;
        }
    return {};
}

std::optional<Parsed> parse(string_view model) {
    for (const auto& b : kBases) {
        if (!model.starts_with(b.base)) continue;
        const string_view rest = model.substr(b.base.size());
        if (rest.empty()) return Parsed{b.convention, Distance::Flat};
        if (!b.has_distance) continue;
        if (rest == "16near") return Parsed{b.convention, Distance::Near};
        if (rest == "16far") return Parsed{b.convention, Distance::Far};
    }
    return std::nullopt;
}

bool mentions(string_view text, Convention convention) {
    for (const auto& b : kBases)
        if (b.convention == convention && b.has_distance) return text.find(string(b.base) + "16") != string_view::npos;
    return false;
}

bool mentions_16bit(string_view text) {
    for (const auto& b : kBases)
        if (b.has_distance && text.find(string(b.base) + "16") != string_view::npos) return true;
    return false;
}

translate::ir::CallingConvention to_l2(Parsed parsed) {
    using L2 = translate::ir::CallingConvention;
    switch (parsed.convention) {
        // In 16-bit code the callee-pops models are Pascal's; flat they are stdcall.
        case Convention::Stdcall: return parsed.distance == Distance::Flat ? L2::Stdcall : L2::Pascal;
        case Convention::Fastcall: return L2::Fastcall;
        case Convention::Thiscall: return L2::Thiscall;
        case Convention::Regcall: return L2::Register;
        case Convention::Interrupt:
        case Convention::InterruptKeepDs: return L2::Interrupt;
        case Convention::Cdecl: break;
    }
    return L2::Cdecl;
}

translate::ir::CallingConvention to_l2(string_view model) {
    const std::optional<Parsed> parsed = parse(model);
    return parsed ? to_l2(*parsed) : translate::ir::CallingConvention::Cdecl;
}

symbols::wine16::Convention to_export(Parsed parsed) {
    using E = symbols::wine16::Convention;
    switch (parsed.convention) {
        case Convention::Stdcall: return parsed.distance == Distance::Flat ? E::Stdcall : E::Pascal;
        case Convention::Fastcall: return E::Pascal;   // the callee pops, as Pascal does
        default: break;
    }
    return E::Cdecl;
}

string Model::result_register(unsigned bytes, bool is_float) const {
    for (const ResultEntry& r : results)
        if (r.is_float == is_float && bytes >= r.min_size && bytes <= r.max_size) return r.reg;
    return {};
}

// ---- the service ----

Service::Service(const stdfs::path& languages) : folder_(languages) {
    std::vector<stdfs::path> ldefs, cspecs;
    for (const stdfs::directory_entry& entry : stdfs::directory_iterator(folder_)) {
        const string ext = fs::to_text(entry.path().extension());
        if (ext == ".ldefs") ldefs.push_back(entry.path());
        else if (ext == ".cspec") cspecs.push_back(entry.path());
    }
    std::sort(ldefs.begin(), ldefs.end());
    std::sort(cspecs.begin(), cspecs.end());
    for (const stdfs::path& file : cspecs) specs_[fs::to_text(file.stem())].models = read_spec(file);
    for (const stdfs::path& file : ldefs) {
        const Element document = read_xml(file);
        document.each_descendant("language", [&](const Element& language) {
            Language& l = languages_[string(language.attribute("id"))];
            for (const Element& c : language.children)
                if (c.name == "compiler") {
                    Compiler compiler{string(c.attribute("name")), string(c.attribute("id")), string(c.attribute("spec"))};
                    l.compilers.push_back(std::move(compiler));
                }
        });
    }
    // The facts the specs lack: config/abi/*.json beside the checkout's other configuration (absent in a copy of the specs alone).
    const stdfs::path abi = config::current().checkout_root() / "config" / "abi";
    std::error_code ec;
    if (stdfs::is_directory(abi, ec)) {
        std::vector<stdfs::path> files;
        for (const stdfs::directory_entry& entry : stdfs::directory_iterator(abi))
            if (entry.path().extension() == ".json") files.push_back(entry.path());
        std::sort(files.begin(), files.end());
        for (const stdfs::path& file : files) {
            const Json document = json::load(file);
            if (!document.contains("models")) continue;
            for (const auto& [name, value] : document["models"].items()) {
                Extra& e = extras_[name];
                e.decoration = value.value("decoration", e.decoration);
                e.notes = value.value("notes", e.notes);
                if (value.contains("compiler_keyword"))
                    for (const auto& [compiler, keyword] : value["compiler_keyword"].items()) e.compiler_keyword[compiler] = keyword.get<string>();
            }
        }
    }
}

const Service& Service::get() {
    static std::once_flag once;
    static std::unique_ptr<Service> service;
    std::call_once(once, [] { service = std::make_unique<Service>(default_folder()); });
    return *service;
}

std::vector<string> Service::languages() const {
    std::vector<string> out;
    for (const auto& [id, language] : languages_) out.push_back(id);
    return out;
}

std::span<const Compiler> Service::compilers(string_view language) const {
    const auto it = languages_.find(language);
    return it == languages_.end() ? std::span<const Compiler>() : std::span<const Compiler>(it->second.compilers);
}

const Model* Service::model_of(string_view spec, string_view name) const {
    const stdfs::path stem = fs::from_text(string(spec)).stem();   // "x86win.cspec" and "x86win" are the same spec
    const auto it = specs_.find(fs::to_text(stem));
    if (it == specs_.end()) return nullptr;
    for (const Model& m : it->second.models)
        if (m.name == name) return &m;
    return nullptr;
}

std::span<const Model> Service::models_of(string_view spec) const {
    const stdfs::path stem = fs::from_text(string(spec)).stem();
    const auto it = specs_.find(fs::to_text(stem));
    return it == specs_.end() ? std::span<const Model>() : std::span<const Model>(it->second.models);
}

std::vector<string> Service::specs() const {
    std::vector<string> out;
    for (const auto& [stem, spec] : specs_) out.push_back(stem);
    return out;
}

const Model* Service::model(string_view language, string_view name, string_view compiler) const {
    for (const Compiler& c : compilers(language)) {
        if (!compiler.empty() && c.id != compiler) continue;
        if (const Model* m = model_of(c.spec, name)) return m;
    }
    return nullptr;
}

const Model* Service::default_model(string_view language, string_view compiler) const {
    for (const Compiler& c : compilers(language)) {
        if (!compiler.empty() && c.id != compiler) continue;
        for (const Model& m : models_of(c.spec))
            if (m.is_default) return &m;
    }
    return nullptr;
}

const Service::Extra* Service::extra(string_view model) const {
    const auto it = extras_.find(model);
    return it == extras_.end() ? nullptr : &it->second;
}

}  // namespace cb::decompile::conventions
