// cb/base/config.cpp -- see config.hpp.
#include "cb/base/config.hpp"


#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <format>
#include <fstream>
#include <mutex>
#include <set>
#include <sstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"

#include <nlohmann/json.hpp>
#include "cb/core/text.hpp"

namespace cb::config {

namespace {

namespace stdfs = std::filesystem;

// The defaults settings.json overrides (the same keys the terminal UI client reads).
struct Default {
    std::string_view key;
    std::string_view value;
};
constexpr std::array kDefaults{
    Default{"corpus", "../Windows95/design/corpus/files"},
    Default{"decompiler_bin", "native/decompiler/build/bin"},
    Default{"tracer_bin", "state/cxx/cb/bin"},
    Default{"formats_bin", "native/formats/build/bin"},
    Default{"engine_bin", "native/engine/build/bin"},
    Default{"dosexec_bin", "native/dosexec/build/bin"},
    Default{"symbols_bin", "native/symbols/build/bin"},
    Default{"invert_bin", "native/invert/build/bin"},
    Default{"analyze_bin", "native/analyze/build/bin"},
    Default{"ppc_binutils", "toolchains/binutils-ppc"},
    Default{"converted", "Converted"},
    Default{"nasm", "native/nasm/build/bin/nasm.exe"},
    Default{"retromcp", "native/retromcp/build/libs/retromcp.jar"},
    Default{"state", "state"},
    Default{"threads", ""},
    Default{"target", "win95"},
    Default{"struct_recovery", "all"},
};

// The settings whose value is a path (relative ones are taken against the checkout): the path keys, plus
// "reference" (a relative one is taken against the checkout, not the working folder).
constexpr std::array<std::string_view, 37> kPathSettings{
    "corpus",        "openwatcom",   "build_cache",  "decompiler_bin", "tracer_bin",
    "formats_bin",   "engine_bin",   "dosexec_bin",  "symbols_bin",    "invert_bin",       "analyze_bin",
    "state",         "powerpc_tools", "powerpc_lib", "period_compiler", "dosbox_x",        "msc6",
    "xenix_c",       "masm",         "nasm",         "msvc152",        "java",             "retromcp",
    "xbox360_sdk",   "host_cxx",     "msbuild",      "java_overlays",  "java_libs",        "visual_studio",
    "ppc_binutils",  "converted",    "xbox360_keys", "xbox360_crypto_bin", "ilspycmd",     "xna_overlays",
    "win95_installed_hd", "reference"};

std::string upper(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return out;
}

std::string as_text(const Value& v) {
    if (const auto* s = std::get_if<std::string>(&v)) return *s;
    if (const auto* i = std::get_if<std::int64_t>(&v)) return std::to_string(*i);
    if (const auto* d = std::get_if<double>(&v)) return std::format("{}", *d);
    if (const auto* b = std::get_if<bool>(&v)) return *b ? "true" : "false";
    return {};
}

bool has_checkout_marks(const stdfs::path& dir) {
    std::error_code ec;
    return stdfs::is_directory(dir / "data", ec) && stdfs::is_directory(dir / "native", ec);
}

std::optional<stdfs::path> checkout_at_or_above(stdfs::path dir) {
    std::error_code ec;
    dir = stdfs::weakly_canonical(dir, ec);
    for (;;) {
        if (has_checkout_marks(dir)) return dir;
        stdfs::path up = dir.parent_path();
        if (up.empty() || up == dir) return std::nullopt;
        dir = up;
    }
}

stdfs::path running_executable() { return fs::executable_path(); }

}  // namespace

bool Config::is_path_setting(std::string_view key) {
    return std::find(kPathSettings.begin(), kPathSettings.end(), key) != kPathSettings.end();
}

Config Config::load(const stdfs::path& checkout_root) {
    Config c;
    c.root_ = checkout_root;
    const stdfs::path file = checkout_root / "settings.json";
    std::error_code ec;
    if (!stdfs::exists(file, ec)) return c;   // no settings.json: the defaults and the environment
    Json doc;
    try {
        doc = json::load(file);
    } catch (const Error& e) {
        throw FormatError(fs::to_text(file), 0, e.what());
    }
    if (!doc.is_object()) throw FormatError(fs::to_text(file), 0, "not a JSON object");
    for (auto it = doc.begin(); it != doc.end(); ++it) {
        const Json& v = it.value();
        if (it.key() == "corpora") {
            if (v.is_object())
                for (auto ct = v.begin(); ct != v.end(); ++ct)
                    if (ct.value().is_string()) c.file_corpora_[ct.key()] = ct.value().get<std::string>();
            continue;
        }
        if (v.is_string()) c.file_[it.key()] = v.get<std::string>();
        else if (v.is_boolean()) c.file_[it.key()] = v.get<bool>();
        else if (v.is_number_integer()) c.file_[it.key()] = v.get<std::int64_t>();
        else if (v.is_number_float()) c.file_[it.key()] = v.get<double>();
        else if (v.is_object()) {   // shown by `settings`; read by whoever owns it (kits: exact/toolchain)
            std::string text;
            for (auto ot = v.begin(); ot != v.end(); ++ot)
                text += (text.empty() ? "" : ", ") + ot.key() + "=" + (ot.value().is_string() ? ot.value().get<std::string>() : ot.value().dump());
            c.file_objects_[it.key()] = text;
        }
        // (null and lists are not settings this reads: left unset)
    }
    return c;
}

Value Config::value(std::string_view key) const {
    if (auto env = fs::get_env("CB_" + upper(key)); env && !env->empty()) return *env;
    if (auto it = file_.find(key); it != file_.end()) return it->second;
    for (const Default& d : kDefaults)
        if (d.key == key) return std::string(d.value);
    return std::monostate{};
}

std::string Config::text(std::string_view key) const { return as_text(value(key)); }

std::optional<stdfs::path> Config::path(std::string_view key) const {
    const std::string t = text(key);
    if (t.empty()) return std::nullopt;
    stdfs::path p = fs::from_text(t);
    if (p.is_relative() && is_path_setting(key)) p = root_ / p;   // (only path settings are joined)
    return p.lexically_normal();
}

std::optional<std::int64_t> Config::integer(std::string_view key) const {
    const Value v = value(key);
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i;
    if (const auto* d = std::get_if<double>(&v)) return static_cast<std::int64_t>(*d);
    if (const auto* b = std::get_if<bool>(&v)) return *b ? 1 : 0;
    if (const auto* s = std::get_if<std::string>(&v)) return text::parse_int<std::int64_t>(text::trim(*s));
    return std::nullopt;
}

std::optional<double> Config::number(std::string_view key) const {
    const Value v = value(key);
    if (const auto* i = std::get_if<std::int64_t>(&v)) return static_cast<double>(*i);
    if (const auto* d = std::get_if<double>(&v)) return *d;
    if (const auto* b = std::get_if<bool>(&v)) return *b ? 1.0 : 0.0;
    if (const auto* s = std::get_if<std::string>(&v)) {
        const std::string t(text::trim(*s));
        if (t.empty()) return std::nullopt;
        char* end = nullptr;
        const double d = std::strtod(t.c_str(), &end);
        if (end == t.c_str() + t.size()) return d;
    }
    return std::nullopt;
}

bool Config::truthy(std::string_view key) const {
    const Value v = value(key);
    if (const auto* b = std::get_if<bool>(&v)) return *b;
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i != 0;
    if (const auto* d = std::get_if<double>(&v)) return *d != 0;
    if (const auto* s = std::get_if<std::string>(&v)) {
        const std::string t = text::lower(text::trim(*s));
        return !(t.empty() || t == "0" || t == "false" || t == "no" || t == "off" || t == "none");
    }
    return false;
}

std::vector<std::string> Config::keys() const {
    std::set<std::string> all;
    for (const auto& [key, v] : file_) all.insert(key);
    if (!file_corpora_.empty()) all.insert("corpora");
    for (const auto& [key, v] : file_objects_) all.insert(key);
    for (const Default& d : kDefaults) all.insert(std::string(d.key));
    for (std::string_view key : kPathSettings)
        if (key != "reference") all.insert(std::string(key));
    return {all.begin(), all.end()};
}

std::optional<std::string> Config::object_text(std::string_view key) const {
    const auto it = file_objects_.find(key);
    return it == file_objects_.end() ? std::nullopt : std::optional<std::string>(it->second);
}

std::string Config::corpora_text() const {
    std::string out;
    for (const auto& [name, folder] : file_corpora_) out += (out.empty() ? "" : ", ") + name + "=" + folder;
    return out;
}

stdfs::path Config::state() const { return path("state").value_or(root_ / "state"); }

stdfs::path Config::state(const stdfs::path& inside) const {
    const stdfs::path p = state() / inside;
    fs::make_dirs(p.parent_path());
    return p;
}

std::map<std::string, stdfs::path> Config::corpora() const {
    std::map<std::string, stdfs::path> out;
    for (const auto& [name, folder] : file_corpora_) {
        stdfs::path f = fs::from_text(folder);
        out[name] = (f.is_relative() ? root_ / f : f).lexically_normal();
    }
    const std::string target = text("target");
    if (!out.contains(target))
        if (auto c = path("corpus")) out[target] = *c;
    return out;
}

cb::base::Products Config::products() const {
    cb::base::Products p;
    p.checkout_root = root_;
    for (const auto& [name, folder] : file_corpora_) {
        stdfs::path f = fs::from_text(folder);
        p.listed[name] = (f.is_relative() ? root_ / f : f).lexically_normal();
    }
    p.target = text("target");
    p.target_folder = path("corpus").value_or(stdfs::path{});
    p.state = state();
    p.reference = path("reference");
    return p;
}

cb::parallel::WorkerPolicy Config::worker_policy() const {
    cb::parallel::WorkerPolicy w;
    w.workers = static_cast<int>(std::clamp<std::int64_t>(integer("workers").value_or(0), 0, 4096));
    w.low_load = truthy("low_load");
    return w;
}

cb::base::budget::Limits Config::budget_limits() const {
    cb::base::budget::Limits l;
    l.workers = worker_policy();
    l.min_free_gb = number("min_free_gb").value_or(6.0);
    l.slot_dir = state() / "slots";
    return l;
}

stdfs::path Config::main_checkout() const {
    const stdfs::path dot = root_ / ".git";
    std::error_code ec;
    if (!stdfs::is_regular_file(dot, ec)) return root_;
    std::ifstream f(dot, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    const std::string line(text::trim(ss.str()));
    constexpr std::string_view kGitdir = "gitdir:";
    if (!line.starts_with(kGitdir)) return root_;
    const stdfs::path where = fs::from_text(std::string(text::trim(std::string_view(line).substr(kGitdir.size())))).lexically_normal();
    const stdfs::path up = where.parent_path();   // <main>/.git/worktrees
    if (up.filename() == "worktrees" && up.parent_path().filename() == ".git") return up.parent_path().parent_path();
    return root_;
}

stdfs::path find_checkout() {
    if (auto env = fs::get_env("CB_CHECKOUT_ROOT"); env && !env->empty()) return fs::from_text(*env);
    if (const stdfs::path exe = running_executable(); !exe.empty())
        if (auto c = checkout_at_or_above(exe.parent_path())) return *c;
    std::error_code ec;
    const stdfs::path cwd = stdfs::current_path(ec);
    if (auto c = checkout_at_or_above(cwd)) return *c;
    return cwd;
}

const Config& current() {
    static std::once_flag once;
    static Config config;
    std::call_once(once, [] { config = Config::load(find_checkout()); });
    return config;
}

}  // namespace cb::config
