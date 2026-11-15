#include "cb/exact/toolchain.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/base/cache_codec.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::toolchain {

namespace stdfs = std::filesystem;
using OJson = nlohmann::ordered_json;

namespace {

[[noreturn]] void toolchain_bad(const std::string& what) { throw FormatError("toolchain profile", 0, what); }

// A number, or text read as a number in base 0 ("0x749D", "256")
std::int64_t toolchain_number(const OJson& v, std::string_view where) {
    if (v.is_number_integer()) return v.get<std::int64_t>();
    if (v.is_string()) {
        std::string_view s = text::trim(v.get_ref<const std::string&>());
        int base = 10;
        if (s.starts_with("0x") || s.starts_with("0X")) {
            base = 16;
            s.remove_prefix(2);
        }
        if (const auto n = text::parse_int<std::int64_t>(s, base)) return *n;
    }
    toolchain_bad(std::format("{} is not a number", where));
}

Change toolchain_change(const OJson& v, std::string_view where) {
    if (v.is_number_integer()) return v.get<std::int64_t>();
    if (v.is_string()) return v.get<std::string>();
    toolchain_bad(std::format("{} is not a number or a name", where));
}

Bytes toolchain_hex(const OJson& v, std::string_view where) {
    if (!v.is_string()) toolchain_bad(std::format("{} is not hexadecimal text", where));
    return crypto::from_hex(v.get_ref<const std::string&>());
}

const OJson& toolchain_need(const OJson& object, std::string_view key, std::string_view where) {
    const auto it = object.find(std::string(key));
    if (it == object.end()) toolchain_bad(std::format("{} has no \"{}\"", where, key));
    return *it;
}

std::optional<std::string> toolchain_text(const OJson& object, std::string_view key) {
    const auto it = object.find(std::string(key));
    if (it == object.end() || !it->is_string()) return std::nullopt;
    return it->get<std::string>();
}

std::vector<std::string> toolchain_strings(const OJson& object, std::string_view key) {
    std::vector<std::string> out;
    const auto it = object.find(std::string(key));
    if (it != object.end() && it->is_array())
        for (const OJson& s : *it)
            if (s.is_string()) out.push_back(s.get<std::string>());
    return out;
}

// A pattern that matches `s` literally.
std::string toolchain_escape(std::string_view s) {
    std::string out;
    for (char c : s) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') out += '\\';
        out += c;
    }
    return out;
}

// the bytes with every CR LF a LF, then every LF a CR LF
Bytes toolchain_crlf(ByteView b) {
    Bytes out;
    out.reserve(b.size() + b.size() / 16);
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (b[i] == '\r' && i + 1 < b.size() && b[i + 1] == '\n') continue;
        if (b[i] == '\n') out.push_back('\r');
        out.push_back(b[i]);
    }
    return out;
}

Bytes toolchain_lf(ByteView b) {
    Bytes out;
    out.reserve(b.size());
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (b[i] == '\r' && i + 1 < b.size() && b[i + 1] == '\n') continue;
        out.push_back(b[i]);
    }
    return out;
}

std::string toolchain_upper_name(const stdfs::path& p) { return text::upper(fs::to_text(p)); }

void toolchain_remove(const stdfs::path& p) {
    std::error_code ec;   // (still open: left for the next stage's clean-up)
    stdfs::remove(p, ec);
}

// what the passes printed, as text
std::string toolchain_console(ByteView b) {
    std::string s = text::from_latin1(b);
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    return s;
}

}  // namespace

}  // namespace cb::exact::toolchain

// A compile as the cache keeps it.
namespace cb::base::cache {
template <>
struct Codec<cb::exact::toolchain::Compiled> {
    static constexpr std::uint32_t kSchema = 0x7101;
    static void write(io::ByteWriter& w, const cb::exact::toolchain::Compiled& c) {
        Codec<std::optional<Bytes>>::write(w, c.object);
        Codec<std::string>::write(w, c.console);
        Codec<std::optional<std::string>>::write(w, c.listing);
    }
    static cb::exact::toolchain::Compiled read(io::ByteReader& r) {
        cb::exact::toolchain::Compiled c;
        c.object = Codec<std::optional<Bytes>>::read(r);
        c.console = Codec<std::string>::read(r);
        c.listing = Codec<std::optional<std::string>>::read(r);
        return c;
    }
};
}  // namespace cb::base::cache

namespace cb::exact::toolchain {

// ---- Locations -----------------------------------------------------------------------------------------------------

Locations Locations::of_checkout(const stdfs::path& checkout_root) {
    Locations l;
    l.checkout_root = checkout_root;
    l.profiles = checkout_root / "data" / "exact" / "toolchains";
    l.state = checkout_root / "state";
    const stdfs::path settings = checkout_root / "settings.json";
    std::error_code ec;
    if (stdfs::is_regular_file(settings, ec)) {
        const Json j = json::load(settings);
        if (j.is_object()) {
            if (const auto s = j.find("state"); s != j.end() && s->is_string()) {
                const stdfs::path p = fs::from_text(s->get<std::string>());
                l.state = p.is_absolute() ? p : checkout_root / p;
            }
            if (const auto k = j.find("kits"); k != j.end() && k->is_object())
                for (const auto& [name, folder] : k->items())
                    if (folder.is_string()) l.kits[name] = fs::from_text(folder.get<std::string>());
        }
    }
    if (const auto tag = fs::get_env("CB_STAGE_TAG")) l.stage_tag = *tag;
    return l;
}

stdfs::path Locations::kit(std::string_view name) const {
    stdfs::path path;
    if (const auto env = fs::get_env("CB_KIT_" + text::upper(name)); env && !env->empty()) {
        path = fs::from_text(*env);
    } else if (const auto it = kits.find(std::string(name)); it != kits.end()) {
        path = it->second;
    }
    if (path.empty())
        throw UsageError(std::format("the kit '{}' is not set up (settings.json: \"kits\": {{\"{}\": folder}})", name, name));
    return (path.is_absolute() ? path : checkout_root / path).lexically_normal();
}

stdfs::path Locations::kit_path(const std::optional<std::string>& kit_name, std::string_view path, const stdfs::path& base) const {
    const std::size_t colon = path.find(':');
    if (colon != std::string_view::npos && colon > 1) return kit(path.substr(0, colon)) / fs::from_text(path.substr(colon + 1));
    if (kit_name) return kit(*kit_name) / fs::from_text(path);
    return (base / fs::from_text(path)).lexically_normal();
}

std::vector<std::string> Locations::profile_names() const {
    std::vector<std::string> out;
    std::error_code ec;
    for (stdfs::directory_iterator it(profiles, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file(ec) && it->path().extension() == ".json") out.push_back(fs::to_text(it->path().stem()));
    std::sort(out.begin(), out.end());
    return out;
}

// ---- the profile ---------------------------------------------------------------------------------------------------

Profile parse_profile(std::string_view text) {
    OJson j;
    nlohmann::json sorted;   // the profile with its keys in order, for the digest: what is the same profile has one digest
    try {
        j = OJson::parse(text);
        sorted = nlohmann::json::parse(text);
    } catch (const nlohmann::json::exception& e) {
        throw ParseError("toolchain profile", 0, 0, e.what());
    }
    if (!j.is_object()) toolchain_bad("a profile is a JSON object");
    Profile p;
    p.name = toolchain_need(j, "name", "the profile").get<std::string>();
    for (const auto& [key, value] : j.items())
        if (value.is_string()) p.strings[key] = value.get<std::string>();
    p.kit = toolchain_text(j, "kit");
    p.default_variant = toolchain_text(j, "default_variant");
    if (const auto e = toolchain_text(j, "emulator")) p.emulator = *e;
    if (const auto st = j.find("stage"); st != j.end() && st->is_object()) {
        if (const auto files = st->find("files"); files != st->end() && files->is_object())
            for (const auto& [dst, src] : files->items()) p.stage.files.insert_or_assign(dst, src.get<std::string>());
        p.stage.include = toolchain_text(*st, "include");
        p.stage.all = st->value("all", false);
    }
    if (const auto psp = j.find("psp"); psp != j.end()) {
        if (psp->is_null()) p.psp = std::nullopt;
        else p.psp = toolchain_number(*psp, "psp");
    }
    if (const auto top = j.find("top"); top != j.end() && !top->is_null()) {
        const std::int64_t n = toolchain_number(*top, "top");
        if (n != 0) p.top = n;
    }
    p.env = toolchain_strings(j, "env");
    if (const auto u = j.find("env_unless"); u != j.end() && u->is_object())
        for (const auto& [e, rx] : u->items()) p.env_unless.insert_or_assign(e, rx.get<std::string>());
    p.switches = toolchain_text(j, "switches");
    p.file_switches = toolchain_text(j, "file_switches");
    p.file_switches_comment = toolchain_text(j, "file_switches_comment");
    if (const auto d = j.find("derive"); d != j.end() && d->is_object())
        for (const auto& [k, rule] : d->items())
            p.derive.insert_or_assign(k, Derive{toolchain_need(rule, "regex", "a derive rule").get<std::string>(),
                                                toolchain_text(rule, "then").value_or(""), toolchain_text(rule, "else").value_or("")});
    if (const auto ps = j.find("passes"); ps != j.end() && ps->is_array()) {
        for (const OJson& pass : *ps) {
            Pass x;
            x.run = toolchain_need(pass, "run", "a pass").get<std::string>();
            x.args = toolchain_text(pass, "args").value_or("");
            x.env = toolchain_strings(pass, "env");
            x.listing = toolchain_text(pass, "listing");
            p.passes.push_back(std::move(x));
        }
    }
    p.output = toolchain_text(j, "output");
    if (const auto e = toolchain_text(j, "source_ext")) p.source_ext = *e;
    p.source_eof = j.value("source_eof", false);
    if (const auto pa = j.find("patches"); pa != j.end() && pa->is_array()) {
        for (const OJson& patch : *pa) {
            Patch x;
            x.change = toolchain_change(toolchain_need(patch, "change", "a patch"), "a patch's change");
            x.file = toolchain_need(patch, "file", "a patch").get<std::string>();
            x.at = toolchain_number(toolchain_need(patch, "at", "a patch"), "a patch's offset");
            x.was = toolchain_hex(toolchain_need(patch, "was", "a patch"), "a patch's was");
            x.now = toolchain_hex(toolchain_need(patch, "now", "a patch"), "a patch's now");
            x.in_memory = patch.value("in_memory", false);
            p.patches.push_back(std::move(x));
        }
    }
    if (const auto vs = j.find("variants"); vs != j.end() && vs->is_object()) {
        for (const auto& [name, v] : vs->items()) {
            Variant x;
            const OJson* changes = &v;
            if (v.is_object()) {   // ({"patches": [changes], "hooks": [hook names]})
                x.hooks = toolchain_strings(v, "hooks");
                const auto it = v.find("patches");
                changes = it == v.end() ? nullptr : &*it;
            }
            if (changes && changes->is_array())
                for (const OJson& c : *changes) x.patches.push_back(toolchain_change(c, "a variant's change"));
            p.variants.insert_or_assign(name, std::move(x));
        }
    }
    if (const auto hs = j.find("hooks"); hs != j.end() && hs->is_object()) {
        for (const auto& [name, h] : hs->items()) {
            Hook x;
            x.pass = toolchain_need(h, "pass", "a hook").get<std::string>();
            for (const OJson& a : toolchain_need(h, "at", "a hook")) x.at.push_back(toolchain_number(a, "a hook's address"));
            if (const auto s = h.find("set"); s != h.end() && s->is_object())
                for (const auto& [reg, val] : s->items()) x.set.insert_or_assign(reg, toolchain_number(val, "a hook's register value"));
            x.header = toolchain_text(h, "header");
            x.call = toolchain_text(h, "call");
            p.hooks.insert_or_assign(name, std::move(x));
        }
    }
    if (const auto r = j.find("rename_externs"); r != j.end() && r->is_object())
        for (const auto& [from, to] : r->items()) p.rename_externs.insert_or_assign(from, to.get<std::string>());
    p.try_switches = toolchain_strings(j, "try_switches");
    p.plugin = toolchain_text(j, "plugin");

    // a listing's switch changes no object: the passes' "listing" is not in the digest
    if (const auto ps = sorted.find("passes"); ps != sorted.end() && ps->is_array())
        for (nlohmann::json& pass : *ps)
            if (pass.is_object()) pass.erase("listing");
    p.digest = crypto::to_hex(crypto::sha1(to_bytes(sorted.dump())));
    return p;
}

Profile read_profile(const Locations& locations, std::string_view name) {
    const stdfs::path file = locations.profiles / (std::string(name) + ".json");
    std::error_code ec;
    if (!stdfs::is_regular_file(file, ec)) throw NotFoundError(file, 2, "no toolchain profile");
    return parse_profile(fs::read_text(file));
}

// ---- running a pass ------------------------------------------------------------------------------------------------

emu::dos::Result ExecutiveRunner::run(const PassRun& pass) const {
    if (!pass.instrument.empty())
        throw UnsupportedError("exact toolchain", 0, "the DOS executive has no code hooks: a pass the profile instruments cannot be run");
    emu::dos::RunSpec spec;
    spec.exe = pass.exe;
    spec.workdir = pass.workdir;
    spec.command_tail = pass.command_tail;
    spec.environment = pass.environment;
    if (pass.psp) spec.psp = static_cast<std::uint16_t>(*pass.psp);
    if (pass.top) spec.top = static_cast<std::uint16_t>(*pass.top);
    spec.fast = pass.fast;
    spec.files = pass.files;
    return executive_.run(spec);
}

// ---- format_named --------------------------------------------------------------------------------------------------

std::string format_named(std::string_view pattern, const std::map<std::string, std::string>& values) {
    std::string out;
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        const char c = pattern[i];
        if ((c == '{' || c == '}') && i + 1 < pattern.size() && pattern[i + 1] == c) {
            out += c;
            ++i;
        } else if (c == '{') {
            const std::size_t close = pattern.find('}', i);
            if (close == std::string_view::npos) throw FormatError("profile template", i, "a { without its }");
            const std::string name(pattern.substr(i + 1, close - i - 1));
            const auto it = values.find(name);
            if (it == values.end()) throw FormatError("profile template", i, "no value named '" + name + "'");
            out += it->second;
            i = close;
        } else if (c == '}') {
            throw FormatError("profile template", i, "a } without its {");
        } else {
            out += c;
        }
    }
    return out;
}

// ---- Toolchain -----------------------------------------------------------------------------------------------------

Toolchain::Toolchain(Profile profile, std::optional<std::string> variant, const Context& context)
    : profile_(std::move(profile)), context_(context) {
    variant_ = variant.value_or(profile_.default_variant.value_or(""));
    std::vector<Change> on;
    if (const Variant* v = profile_.variants.find(variant_)) {
        on = v->patches;
        variant_hooks_ = v->hooks;
    } else if (!variant_.empty()) {
        std::string have;
        for (const auto& [name, ignored] : profile_.variants) have += (have.empty() ? "" : ", ") + name;
        throw UsageError(std::format("{} has no variant '{}' (it has {})", profile_.name, variant_, have));
    }
    for (const Patch& p : profile_.patches)
        if (std::find(on.begin(), on.end(), p.change) != on.end()) patches_.push_back(p);
    key_ = crypto::to_hex(crypto::sha1(to_bytes(profile_.digest + '\0' + variant_))).substr(0, 12);

    for (const auto& [e, rx] : profile_.env_unless) unless_.emplace_back(e, std::make_shared<Regex>(rx));
    for (const auto& [name, rule] : profile_.derive) derive_.emplace_back(name, std::make_shared<Regex>(rule.regex));
    if (profile_.file_switches) {
        const std::string name = toolchain_escape(*profile_.file_switches);
        file_switches_regex_ = std::make_shared<Regex>(profile_.file_switches_comment == ";" ? "^; " + name + R"(: ([^\r\n]*))"
                                                                                           : R"(^/\* )" + name + R"(: ([^*]*?) \*/)");
    }
    for (const auto& [name, h] : profile_.hooks)   // a hook the variant sets has no header: no source answers it
        if (h.header) answer_regexes_.emplace_back(name, std::make_shared<Regex>(R"(/\* )" + toolchain_escape(*h.header) + R"(: ([0-9, ]+) \*/)"));
}

std::optional<std::string> Toolchain::profile(std::string_view key) const {
    const auto it = profile_.strings.find(std::string(key));
    if (it == profile_.strings.end()) return std::nullopt;
    return it->second;
}

stdfs::path Toolchain::stage() const {
    const std::lock_guard<std::mutex> lock(stage_mutex_);
    const stdfs::path dir = context_.locations->state / "exact" / "stage" /
                            (profile_.name + "-" + key_ + (context_.locations->stage_tag.empty() ? "" : "-" + context_.locations->stage_tag));
    const stdfs::path done = dir / ".staged";
    std::error_code ec;
    if (stdfs::exists(done, ec)) return dir;
    if (!profile_.kit) throw UsageError(profile_.name + " names no kit to stage its files from");
    const Locations& loc = *context_.locations;
    const stdfs::path kit = loc.kit(*profile_.kit);
    const auto where = [&](std::string_view src) {   // "otherkit:path" names a file of another kit
        const std::size_t colon = src.find(':');
        return colon != std::string_view::npos && colon > 1 ? loc.kit(src.substr(0, colon)) / fs::from_text(src.substr(colon + 1))
                                                             : kit / fs::from_text(src);
    };
    OrderedMap<std::string, std::string> files = profile_.stage.files;
    if (profile_.stage.all) {   // (every file of the kit, its folders flattened: a compiler that finds its headers beside itself)
        for (stdfs::recursive_directory_iterator it(kit, ec), end; !ec && it != end; it.increment(ec)) {
            if (!it->is_regular_file(ec)) continue;
            const std::string name = toolchain_upper_name(it->path().filename());
            if (!files.contains(name)) files.insert_or_assign(name, fs::to_text(it->path().lexically_relative(kit)));
        }
    }
    for (const auto& [dst, src] : files) {
        Bytes b = fs::read_file(where(src));
        const std::size_t header = b.size() > 9 && b[0] == 'M' && b[1] == 'Z' ? static_cast<std::size_t>(b[8] | (b[9] << 8)) * 16 : 0;
        for (const Patch& p : patches_) {
            if (p.file != dst || p.in_memory) continue;
            const std::size_t at = header + static_cast<std::size_t>(p.at);
            if (at + p.was.size() > b.size() || !std::equal(p.was.begin(), p.was.end(), b.begin() + static_cast<std::ptrdiff_t>(at)))
                throw FormatError("toolchain stage", at,
                                  std::format("{}: {} at {:x} is not what change {} expects - not this compiler's kit", profile_.name, dst, p.at,
                                              std::holds_alternative<std::int64_t>(p.change) ? std::to_string(std::get<std::int64_t>(p.change))
                                                                                              : std::get<std::string>(p.change)));
            std::copy(p.now.begin(), p.now.end(), b.begin() + static_cast<std::ptrdiff_t>(at));
            if (p.now.size() > p.was.size() && at + p.now.size() > b.size()) b.resize(at + p.now.size());
        }
        fs::make_dirs((dir / fs::from_text(dst)).parent_path());
        fs::write_file(dir / fs::from_text(dst), b);
    }
    if (profile_.stage.include) {
        const stdfs::path from = where(*profile_.stage.include);
        for (stdfs::recursive_directory_iterator it(from, ec), end; !ec && it != end; it.increment(ec)) {   // (with its folders: SYS\)
            if (!it->is_regular_file(ec)) continue;
            const stdfs::path rel = it->path().parent_path().lexically_relative(from);
            const stdfs::path to = dir / "INCLUDE" / fs::from_text(text::upper(fs::to_text(rel)));
            fs::make_dirs(to);
            stdfs::copy_file(it->path(), to / fs::from_text(toolchain_upper_name(it->path().filename())), stdfs::copy_options::overwrite_existing);
        }
    }
    fs::make_dirs(dir);
    fs::write_text(done, key_);
    return dir;
}

std::string Toolchain::switches_of(ByteView source, const std::optional<std::string>& switches) const {
    if (file_switches_regex_) {
        if (const auto m = file_switches_regex_->match(as_text(source))) return std::string(text::trim(m->group(1)));
    }
    return switches ? *switches : profile_.switches.value_or("");
}

std::map<std::string, std::vector<std::int64_t>> Toolchain::answers_of(ByteView source) const {
    std::map<std::string, std::vector<std::int64_t>> out;
    const std::string_view head = as_text(source).substr(0, 400);
    for (const auto& [name, rx] : answer_regexes_) {
        const auto m = rx->search(head);
        if (!m) continue;
        std::vector<std::int64_t> counts;
        const std::string listed = text::replace_all(m->group(1), ",", " ");   // (the views below are into it)
        for (std::string_view n : text::split_ws(listed))
            if (const auto v = text::parse_int<std::int64_t>(n, 10)) counts.push_back(*v);
        std::sort(counts.begin(), counts.end());
        out[name] = std::move(counts);
    }
    return out;
}

std::map<std::string, std::string> Toolchain::values(const std::string& switches, const std::string& tag) const {
    std::map<std::string, std::string> v = {{"tag", tag}, {"tag6", tag.substr(0, 6)}, {"switches", switches}};
    for (const auto& [name, rx] : derive_) {
        const Derive& rule = *profile_.derive.find(name);
        if (const auto m = rx->search(switches)) {
            std::string t = rule.then;
            for (std::size_t i = 1; i < m->group_count(); ++i)
                t = text::replace_all(t, "{" + std::to_string(i) + "}", m->has(i) ? std::string(m->group(i)) : std::string());
            v[name] = std::move(t);
        } else {
            v[name] = rule.otherwise;
        }
    }
    return v;
}

Bytes Toolchain::environment(const std::string& switches) const {
    Bytes out;
    const auto add = [&](const std::string& s) {
        const Bytes b = text::to_latin1(s);
        out.insert(out.end(), b.begin(), b.end());
        out.push_back(0);
    };
    for (const std::string& e : profile_.env) add(e);
    for (const auto& [e, rx] : unless_)
        if (!rx->search(switches)) add(e);
    return out;
}

Instrument Toolchain::instrument_for(const Pass& pass, const std::map<std::string, std::vector<std::int64_t>>& answers) const {
    Instrument inst;
    for (const auto& [name, h] : profile_.hooks) {
        const auto given = answers.find(name);
        if (h.pass != pass.run || given == answers.end() || given->second.empty()) continue;
        CountedHook c;
        c.at = h.at;
        c.counts.insert(given->second.begin(), given->second.end());
        c.registers = h.set;
        inst.counted.push_back(std::move(c));
    }
    for (const std::string& x : variant_hooks_) {   // a hook whose answer is code: where the pass goes on
        const Hook* h = profile_.hooks.find(x);
        if (!h || h->pass != pass.run || !h->call) continue;
        const auto fn = context_.hooks ? context_.hooks->find(*h->call) : std::map<std::string, HookFn>::const_iterator();
        if (!context_.hooks || fn == context_.hooks->end()) throw UsageError("no function is registered for the hook '" + *h->call + "'");
        inst.calls.push_back({h->at, fn->second});
    }
    for (const Patch& p : patches_)   // a packed pass: changed once it has unpacked itself
        if (p.file == pass.run && p.in_memory) inst.memory.push_back({p.at, p.was, p.now});
    return inst;
}

std::pair<std::uint32_t, Bytes> Toolchain::run_passes(const stdfs::path& work, const std::string& switches, const std::string& tag,
                                                      const std::map<std::string, std::vector<std::int64_t>>& answers, emu::dos::MemFS* files,
                                                      const std::optional<std::size_t>& listing_pass) const {
    if (profile_.emulator != "dos") throw UnsupportedError("exact toolchain", 0, "the emulator '" + profile_.emulator + "' is not available");
    const std::map<std::string, std::string> v = values(switches, tag);
    const Bytes env = environment(switches);
    Bytes console;
    for (std::size_t i = 0; i < profile_.passes.size(); ++i) {
        const Pass& pass = profile_.passes[i];
        PassRun run;
        run.instrument = instrument_for(pass, answers);
        run.exe = work / fs::from_text(pass.run);
        run.workdir = work;
        std::string args = pass.args;
        if (listing_pass && *listing_pass == i) args += " " + *pass.listing;
        run.command_tail = format_named(args, v);
        run.environment = env;
        for (const std::string& e : pass.env) {
            const Bytes b = text::to_latin1(format_named(e, v));
            run.environment.insert(run.environment.end(), b.begin(), b.end());
            run.environment.push_back(0);
        }
        run.psp = profile_.psp;
        run.top = profile_.top;
        run.fast = run.instrument.empty();   // (no hook of ours in the pass: the fast run)
        run.files = files;
        const emu::dos::Result r = context_.runner->run(run);
        console.insert(console.end(), r.console.begin(), r.console.end());
        if (r.exit && *r.exit != 0) {
            const std::string stopped = "(the pass that stopped: " + pass.run + ")\n";
            console.insert(console.end(), stopped.begin(), stopped.end());
            return {*r.exit, console};
        }
    }
    return {0, console};
}

std::string Toolchain::compile_key(ByteView source, const modules::CompileOptions& options) const {
    const Bytes src = toolchain_lf(source);
    std::string headers, includes;
    for (const auto& [name, data] : options.headers) headers += name + '\0' + std::string(as_text(data)) + '\0';
    for (const std::string& dir : options.include) {
        std::vector<stdfs::path> names;
        std::error_code ec;
        for (stdfs::directory_iterator it(fs::from_text(dir), ec), end; !ec && it != end; it.increment(ec))
            if (it->is_regular_file(ec)) names.push_back(it->path());
        std::sort(names.begin(), names.end());
        for (const stdfs::path& f : names) includes += fs::to_text(f.filename()) + '\0' + fs::read_text(f) + '\0';
    }
    const std::string sw = switches_of(src, options.switches);
    std::string answers;
    for (const auto& [name, counts] : answers_of(src)) {
        answers += name + ':';
        for (std::int64_t c : counts) answers += std::to_string(c) + ',';
        answers += ';';
    }
    return base::cache::key({src, headers, includes, sw, answers, key_, std::string(kCacheVersion)});
}

modules::CompileResult Toolchain::compile(ByteView source, const modules::CompileOptions& options) const {
    Compiled c = compile_full(source, options);
    return {std::move(c.object), std::move(c.console)};
}

Compiled Toolchain::compile_full(ByteView source, const modules::CompileOptions& options, bool want_listing, bool cache) const {
    if (!want_listing) return compile_uncached(source, options, false, cache);
    // the pass the profile's "listing" names writes its listing of the code
    std::optional<std::size_t> lp;
    for (std::size_t i = 0; i < profile_.passes.size(); ++i)
        if (profile_.passes[i].listing) {
            lp = i;
            break;
        }
    if (!lp) {
        Compiled c = compile_uncached(source, options, false, cache);
        c.listing = "";
        return c;
    }
    Bytes marked(source.begin(), source.end());
    const std::string note = "\n/* listing */";
    marked.insert(marked.end(), note.begin(), note.end());
    Compiled c = compile_uncached(marked, options, true, cache);
    if (!c.listing) c.listing = "";
    return c;
}

Compiled Toolchain::compile_uncached(ByteView source_in, const modules::CompileOptions& options, bool want_listing, bool cache) const {
    CB_ASSERT(context_.runner != nullptr, "a toolchain needs a runner to compile");
    const Bytes src = toolchain_lf(source_in);
    std::vector<std::pair<std::string, Bytes>> incs;
    for (const std::string& dir : options.include) {
        std::vector<stdfs::path> names;
        std::error_code ec;
        for (stdfs::directory_iterator it(fs::from_text(dir), ec), end; !ec && it != end; it.increment(ec))
            if (it->is_regular_file(ec)) names.push_back(it->path());
        std::sort(names.begin(), names.end());
        for (const stdfs::path& f : names) incs.emplace_back(fs::to_text(f.filename()), fs::read_file(f));
    }
    const std::string sw = switches_of(src, options.switches);
    const auto ans = answers_of(src);
    const std::string key = compile_key(src, options);
    const bool kept = cache && context_.cache != nullptr;
    if (kept) {
        if (auto got = context_.cache->value<Compiled>(kCacheKind, key)) return std::move(*got);
    }

    const stdfs::path w = stage();
    const std::uint64_t counter = context_.tags ? ++*context_.tags : ++local_tags_;
    const std::string tag = std::format("T{:05}", (static_cast<std::uint64_t>(fs::process_id()) * 7 + counter) % 100000);
    const stdfs::path ix = w / "INCX";
    Bytes body = toolchain_crlf([&] {
        Bytes no_eof;
        for (std::uint8_t b : src)
            if (b != 0x1A) no_eof.push_back(b);
        return no_eof;
    }());
    if (profile_.source_eof) body.push_back(0x1A);   // (a ^Z after the text: MASM 1.00 wants it)
    if (!profile_.output) throw FormatError("toolchain profile", 0, profile_.name + " has no \"output\"");
    const stdfs::path out = w / fs::from_text(format_named(*profile_.output, values(sw, tag)));
    const stdfs::path lst = w / (tag + ".LST");   // (a listing the pass was asked for)
    const std::string ext = profile_.source_ext;

    std::optional<std::size_t> listing_pass;
    if (want_listing)
        for (std::size_t i = 0; i < profile_.passes.size(); ++i)
            if (profile_.passes[i].listing) {
                listing_pass = i;
                break;
            }

    std::optional<Bytes> object;
    std::optional<std::string> listing;
    Bytes console;
    if (context_.memfs && profile_.emulator == "dos") {   // (the run's files in memory: nothing written, no other run's files in the way)
        emu::dos::MemFS files({ix});
        for (const auto& [name, data] : options.headers) files.put(w / fs::from_text(text::upper(name)), toolchain_crlf(data));
        for (const auto& [name, data] : incs) files.put(ix / fs::from_text(text::upper(name)), toolchain_crlf(data));
        files.put(w / (tag + ext), body);
        const auto [code, con] = run_passes(w, sw, tag, ans, &files, listing_pass);
        console = con;
        if (code == 0 && files.is_file(out)) object = files.read(out);
        if (files.is_file(lst)) {
            std::string l = text::from_latin1(files.read(lst));
            l.erase(std::remove(l.begin(), l.end(), '\r'), l.end());
            listing = std::move(l);
        }
    } else {
        for (const auto& [name, data] : options.headers) fs::write_file(w / fs::from_text(text::upper(name)), toolchain_crlf(data));
        fs::make_dirs(ix);
        std::error_code ec;
        for (stdfs::directory_iterator it(ix, ec), end; !ec && it != end; it.increment(ec)) toolchain_remove(it->path());
        for (const auto& [name, data] : incs) fs::write_file(ix / fs::from_text(text::upper(name)), toolchain_crlf(data));
        fs::write_file(w / (tag + ext), body);
        toolchain_remove(out);
        const auto [code, con] = run_passes(w, sw, tag, ans, nullptr, listing_pass);
        console = con;
        if (code == 0 && stdfs::exists(out, ec)) object = fs::read_file(out);
        if (stdfs::exists(lst, ec)) {
            std::string l = text::from_latin1(fs::read_file(lst));
            l.erase(std::remove(l.begin(), l.end(), '\r'), l.end());
            listing = std::move(l);
        }
        toolchain_remove(w / (tag + ext));
        toolchain_remove(out);
        toolchain_remove(lst);
        for (const auto& [name, data] : options.headers) toolchain_remove(w / fs::from_text(text::upper(name)));
        for (stdfs::directory_iterator it(w, ec), end; !ec && it != end; it.increment(ec)) {   // (the passes' own files a failed run leaves: T12345EX ..)
            const std::string f = fs::to_text(it->path().filename());
            if (f.starts_with(tag) && f.size() == tag.size() + 2) toolchain_remove(it->path());
        }
    }
    if (object && !profile_.rename_externs.empty()) {
        std::map<std::string, std::string> rename;
        for (const auto& [from, to] : profile_.rename_externs) rename[from] = to;
        object = formats::omf::rename_externs(*object, rename);
    }
    Compiled result{std::move(object), toolchain_console(console), std::move(listing)};
    if (kept) context_.cache->keep(kCacheKind, key, result);
    return result;
}

// ---- Toolchains ----------------------------------------------------------------------------------------------------

Toolchains::Toolchains(Locations locations, const PassRunner* runner, base::cache::Cache* cache)
    : locations_(std::move(locations)), runner_(runner), cache_(cache) {
    context_.locations = &locations_;
    context_.runner = runner_;
    context_.cache = cache_;
    context_.hooks = &hooks_;
    context_.tags = &tags_;
    const auto memfs = fs::get_env("CB_MEMFS");
    context_.memfs = !(memfs && *memfs == "0");
}

void Toolchains::register_hook(std::string name, HookFn fn) { hooks_[std::move(name)] = std::move(fn); }

void Toolchains::register_plugin(std::string name, Plugin make) { plugins_[std::move(name)] = std::move(make); }

std::shared_ptr<Toolchain> Toolchains::load(const std::string& name, const std::optional<std::string>& variant) {
    const std::lock_guard<std::mutex> lock(mutex_);
    const auto key = std::make_pair(name, variant.value_or(""));
    if (const auto it = loaded_.find(key); it != loaded_.end()) return it->second;
    Profile profile = read_profile(locations_, name);
    std::shared_ptr<Toolchain> made;
    if (profile.plugin) {   // (a toolchain whose running is code)
        const auto plugin = plugins_.find(*profile.plugin);
        if (plugin == plugins_.end())
            throw UnsupportedError("exact toolchain", 0, "the toolchain " + name + " is run by the plugin '" + *profile.plugin + "', which is not registered");
        made = plugin->second(profile, variant, context_);
    } else {
        made = std::make_shared<Toolchain>(std::move(profile), variant, context_);
    }
    loaded_[key] = made;
    return made;
}

}  // namespace cb::exact::toolchain
