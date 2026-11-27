// app/settings_edit.cpp -- see settings_edit.hpp.
#include "app/settings_edit.hpp"

#include <algorithm>
#include <cctype>

#include <nlohmann/json.hpp>

#include "app/convert.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"

namespace cb::app::settings {

namespace stdfs = std::filesystem;

namespace {

struct StWords {
    const char* key;
    const char* label;
    const char* hint;
};

constexpr StWords kStWords[] = {
    {"openwatcom", "OpenWatcom folder", "The compiler that builds C source back into programs."},
    {"masm", "MASM assembler", "Microsoft's assembler, for source that needs it."},
    {"msc6", "Microsoft C 6", "The period compiler for 16-bit Windows code."},
    {"msvc152", "Visual C++ 1.52", "The period compiler for 16-bit Windows 95 code."},
    {"xenix_c", "Xenix C compiler", "A period compiler used to check old code."},
    {"period_compiler", "Period compiler", "Where the period compilers are kept."},
    {"host_cxx", "C++ compiler", "The compiler that builds translated C++ here."},
    {"msbuild", "MSBuild", "Builds Visual Studio projects."},
    {"cmake", "CMake", "Builds the C++ projects converted programs become."},
    {"visual_studio", "Visual Studio folder", "Where Visual Studio is installed."},
    {"java", "Java (JDK) folder", "The JDK used to run Java programs and the Java translator."},
    {"java_libs", "Java libraries", "Library files Java programs are built against."},
    {"java_overlays", "Java overlays", "Replacement files for the Java libraries."},
    {"ilspycmd", "ILSpy", "Optional: ILSpy itself, to compare CodeBreaker's C# with ILSpy's."},
    {"xbox360_sdk", "Xbox 360 SDK folder", "The development kit used to build Xbox 360 code."},
    {"xbox360_keys", "Xbox 360 keys", "Key files for reading protected Xbox 360 files."},
    {"xbox360_crypto_bin", "Xbox 360 crypto tool", "The helper program for Xbox 360 cryptography."},
    {"xna_overlays", "XNA overlays", "Replacement files for XNA games."},
    {"ppc_binutils", "PowerPC tools", "Assembler and linker for PowerPC code."},
    {"powerpc_tools", "PowerPC compiler", "The compiler for PowerPC code."},
    {"powerpc_lib", "PowerPC libraries", "Libraries for PowerPC code."},
    {"corpus", "Program collection", "Where the original programs to study are kept."},
    {"reference", "Reference files", "Extra files used to compare results."},
    {"target", "Target product", "Which product the collection belongs to."},
    {"win95", "Windows 95 folder", "Where the Windows 95 files and sources are."},
    {"win95_installed_hd", "Installed Windows 95 disk", "A disk image with Windows 95 already installed."},
    {"converted", "Converted source folder", "Where converted source goes."},
    {"state", "Working folder", "Where results and temporary files are kept."},
    {"build_cache", "Build cache", "Where finished build steps are remembered."},
    {"decompiler_bin", "Decompiler program", "The program that turns code into C."},
    {"tracer_bin", "Tracer program", "Runs code in the emulator to see what it does."},
    {"workers", "Jobs at once", "How many jobs run together (0 chooses for you)."},
    {"threads", "Threads", "How many threads each job may use."},
    {"low_load", "Keep the computer responsive", "Works more slowly so other programs stay quick."},
    {"min_free_gb", "Free disk space needed", "Stop when less than this many gigabytes are free."},
    {"cache_max_gb", "Cache size limit", "The most disk space the cache may use, in gigabytes."},
    {"struct_recovery", "Recover structures", "Which data structures to work out when decompiling."},
    {"design", "Design folder", "Where the Windows 95 design files are."},
    {"corpora", "Program collections", "The collection of each product."},
    {"service_idle_minutes", "Service idle time", "Minutes the background service waits before it stops."},
    {"work_folder", "Work folder", "The work folder commands use when none is given."},
    {"win95_boot", "Windows 95 boot disk", "The boot disk image for Windows 95."},
    {"win95_setup_iso", "Windows 95 Setup disc", "The Windows 95 Setup CD image."},
};

}  // namespace

std::string plain_label(const std::string& key) {
    for (const StWords& w : kStWords)
        if (key == w.key) return w.label;
    std::string out = key;
    for (char& c : out)
        if (c == '_') c = ' ';
    if (!out.empty()) out[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[0])));
    return out;
}

std::string plain_hint(const std::string& key) {
    for (const StWords& w : kStWords)
        if (key == w.key) return w.hint;
    return {};
}

bool is_everyday(const std::string& key) { return key == "converted" || key == "low_load"; }

namespace {

struct StNeed {
    const char* label;     // what can be converted with the tool
    const char* key;       // the setting that names the tool
    const char* needs;     // the note while it is missing
    bool folder;
};

constexpr StNeed kStNeeds[] = {
    {"Java programs", "java", "Needs a Java JDK", true},
    {"Xbox 360 games", "xbox360_sdk", "Needs the Xbox 360 SDK", true},
};

}  // namespace

std::vector<Capability> capabilities(const std::vector<Row>& rows) {
    std::vector<Capability> out;
    Capability ready{"Ready", "Windows and DOS programs \xC2\xB7 game consoles \xC2\xB7 .NET programs \xC2\xB7 translating source to another language", true, {}, true};
    std::vector<Capability> missing;
    for (const StNeed& need : kStNeeds) {
        bool found = false;
        for (const Row& r : rows)
            if (r.key == need.key) found = r.set && r.present;
        if (found) ready.note += std::string(" \xC2\xB7 ") + need.label;
        else missing.push_back({need.label, need.needs, false, need.key, need.folder});
    }
    out.push_back(std::move(ready));
    for (Capability& c : missing) out.push_back(std::move(c));
    return out;
}

std::vector<Row> rows(const config::Config& config) {
    std::vector<Row> out;
    for (const std::string& key : config.keys()) {
        Row r;
        r.key = key;
        r.label = plain_label(key);
        r.hint = plain_hint(key);
        r.value = config.text(key);
        r.set = !r.value.empty();
        if (config::Config::is_path_setting(key)) {
            r.kind = Kind::Path;
            const auto p = config.path(key);
            r.present = p.has_value() && [&] {
                std::error_code ec;
                return stdfs::exists(*p, ec);
            }();
        } else {
            const std::string v = text::lower(r.value);
            r.kind = (v == "true" || v == "false") ? Kind::Toggle : Kind::Text;
        }
        out.push_back(std::move(r));
    }
    return out;
}

std::string write(const config::Config& config, const std::string& key, const std::string& value, Kind kind) {
    try {
        const stdfs::path file = config.settings_file();
        Json doc = Json::object();
        std::error_code ec;
        if (stdfs::is_regular_file(file, ec)) {
            doc = json::load(file);
            if (!doc.is_object()) return "settings.json is not a JSON object";
        }
        if (kind == Kind::Toggle) doc[key] = text::lower(value) == "true";
        else doc[key] = value;
        json::save(file, doc);
        return {};
    } catch (const std::exception& e) {
        return e.what();
    }
}

// ---- Advanced -----------------------------------------------------------------------------------------------------------------------------

namespace tl = config::tools;

namespace {

std::string se_how(const ToolSource& s, const std::string& setting) {
    using S = config::Config::Source;
    switch (s.source) {
        case S::Environment: return "Set by CB_" + text::upper(setting);
        case S::SettingsFile: return "Set in settings.json";
        case S::WorkFolder: return "Set by the work folder";
        case S::Attached: return s.attached_how == "you" ? "Attached by you" : "Found automatically";
        case S::Host: return "Found automatically";
        case S::Default: return "The default";
        case S::None: break;
    }
    return {};
}

}  // namespace

std::vector<ToolCard> tool_cards(const std::vector<tl::Tool>& catalog, const std::map<std::string, ToolSource>& sources) {
    std::vector<ToolCard> out;
    for (const tl::Tool& t : catalog) {
        ToolCard c;
        c.id = t.id;
        c.name = t.name;
        c.role = t.role;
        c.description = t.description;
        c.needed_for = t.needed_for;
        c.download = t.download;
        c.developer = t.developer;
        c.folder = t.value.empty() || t.value.find('/') != std::string::npos;
        if (t.built_in) {
            c.state = ToolState::BuiltIn;
            out.push_back(std::move(c));
            continue;
        }
        const auto s = sources.find(t.id);
        if (s == sources.end() || s->second.value.empty()) {
            c.state = ToolState::Missing;
            out.push_back(std::move(c));
            continue;
        }
        c.location = s->second.value;
        c.how = se_how(s->second, t.setting);
        if (s->second.works) {
            c.state = *s->second.works ? ToolState::Ready : ToolState::NotWorking;
            c.version = s->second.version;
        } else {
            const tl::Check k = tl::check(t, tl::home_of(t, fs::from_text(c.location)));
            c.state = k.ok ? ToolState::Ready : ToolState::NotWorking;
            c.problem = k.problem;
            if (k.version) c.version = std::to_string(*k.version);
        }
        out.push_back(std::move(c));
    }
    return out;
}

std::vector<ToolCard> tool_cards(const config::Config& config) {
    const std::vector<tl::Tool> catalog = tl::catalog_of(config.checkout_root());
    const std::map<std::string, tl::Attached> attached = tl::read_attached(tl::attached_file(config.main_checkout()));
    std::map<std::string, ToolSource> sources;
    for (const tl::Tool& t : catalog) {
        if (t.setting.empty()) continue;
        ToolSource s;
        s.value = config.text(t.setting);
        s.source = config.source(t.setting);
        if (const auto a = attached.find(t.id); a != attached.end()) s.attached_how = a->second.how;
        sources[t.id] = std::move(s);
    }
    return tool_cards(catalog, sources);
}

const std::vector<ToolGroup>& tool_groups() {
    static const std::vector<ToolGroup> groups = {
        {"Compilers", {"compiler"}}, {"Assemblers", {"assembler"}}, {"Decompilers", {"decompiler"}}, {"Build tools", {"build"}}, {"SDKs", {"sdk"}}};
    return groups;
}

const std::vector<std::string>& performance_keys() {
    static const std::vector<std::string> keys = {"workers", "threads", "min_free_gb", "cache_max_gb", "struct_recovery"};
    return keys;
}

const std::vector<std::string>& folder_keys() {
    static const std::vector<std::string> keys = {"state", "build_cache"};
    return keys;
}

bool is_developer_setting(const std::string& key, const std::vector<tl::Tool>& catalog) {
    if (is_everyday(key)) return false;
    for (const std::vector<std::string>* keys : {&performance_keys(), &folder_keys()})
        if (std::find(keys->begin(), keys->end(), key) != keys->end()) return false;
    for (const tl::Tool& t : catalog)
        if (t.setting == key) return false;
    return true;
}

std::string attach(const config::Config& config, const std::string& id, const std::string& chosen) {
    try {
        const std::vector<tl::Tool> catalog = tl::catalog_of(config.checkout_root());
        const auto t = std::find_if(catalog.begin(), catalog.end(), [&](const tl::Tool& x) { return x.id == id; });
        if (t == catalog.end() || t->setting.empty()) return "CodeBreaker does not know the tool " + id;
        const stdfs::path picked = fs::from_text(chosen);
        std::error_code ec;
        const stdfs::path home = stdfs::is_directory(picked, ec) ? picked : tl::home_of(*t, picked);
        const tl::Check k = tl::check(*t, home);
        if (!k.ok) return "that is not " + t->name + ": " + k.problem;
        const std::string value = fs::to_text((t->value.empty() ? home : home / fs::from_text(t->value)).lexically_normal());
        const stdfs::path file = tl::attached_file(config.main_checkout());
        std::map<std::string, tl::Attached> attached = tl::read_attached(file);
        attached[id] = tl::Attached{value, "you", k.version ? std::to_string(*k.version) : std::string()};
        tl::write_attached(file, attached);
        const stdfs::path settings = config.settings_file();   // (settings.json comes first: the old value there would hide the new one)
        if (stdfs::is_regular_file(settings, ec)) {
            Json doc = json::load(settings);
            if (doc.is_object() && doc.contains(t->setting)) {
                doc.erase(t->setting);
                json::save(settings, doc);
            }
        }
        return {};
    } catch (const std::exception& e) {
        return e.what();
    }
}

std::vector<std::string> attach_found(const config::Config& config) {
    std::vector<std::string> named;
    try {
        using S = config::Config::Source;
        const std::vector<tl::Tool> catalog = tl::catalog_of(config.checkout_root());
        const stdfs::path file = tl::attached_file(config.main_checkout());
        std::map<std::string, tl::Attached> attached = tl::read_attached(file);
        bool changed = false;
        for (auto it = attached.begin(); it != attached.end();) {   // a found tool that is not there any more is let go
            const auto t = std::find_if(catalog.begin(), catalog.end(), [&](const tl::Tool& x) { return x.id == it->first; });
            if (it->second.how == "found" && (t == catalog.end() || !tl::check(*t, tl::home_of(*t, fs::from_text(it->second.value))).ok)) {
                it = attached.erase(it);
                changed = true;
            } else {
                ++it;
            }
        }
        for (const tl::Detected& d : tl::scan(catalog)) {
            const auto t = std::find_if(catalog.begin(), catalog.end(), [&](const tl::Tool& x) { return x.id == d.id; });
            if (t == catalog.end() || t->setting.empty()) continue;
            const S source = config.source(t->setting);
            if (source == S::Environment || source == S::SettingsFile || source == S::WorkFolder) continue;
            const auto have = attached.find(d.id);
            if (have != attached.end() && (have->second.how == "you" || have->second.value == d.value)) continue;
            if (have == attached.end()) named.push_back(t->name);
            attached[d.id] = tl::Attached{d.value, "found", d.version ? std::to_string(*d.version) : std::string()};
            changed = true;
        }
        if (changed) tl::write_attached(file, attached);
    } catch (const std::exception&) {   // (not being able to save it is not an error: what is found is still used, and found again next time)
        named.clear();
    }
    return named;
}

namespace {

stdfs::path st_file() {
    return convert::settings_file();
}

}  // namespace

std::string theme_name(Theme theme) { return theme == Theme::Light ? "Light" : theme == Theme::Dark ? "Dark" : "System"; }

Theme remembered_theme() {
    try {
        const stdfs::path file = st_file();
        std::error_code ec;
        if (file.empty() || !stdfs::is_regular_file(file, ec)) return Theme::System;
        const Json doc = json::load(file);
        if (doc.is_object() && doc.contains("theme") && doc["theme"].is_string()) {
            const std::string t = text::lower(doc["theme"].get<std::string>());
            if (t == "light") return Theme::Light;
            if (t == "dark") return Theme::Dark;
        }
    } catch (const std::exception&) {
    }
    return Theme::System;
}

void remember_theme(Theme theme) {
    try {
        const stdfs::path file = st_file();
        if (file.empty()) return;
        Json doc = Json::object();
        std::error_code ec;
        if (stdfs::is_regular_file(file, ec)) {
            try {
                doc = json::load(file);
            } catch (const std::exception&) {
            }
            if (!doc.is_object()) doc = Json::object();
        }
        doc["theme"] = text::lower(theme_name(theme));
        fs::make_dirs(file.parent_path());
        json::save(file, doc);
    } catch (const std::exception&) {
    }
}

bool remembered_developer_tools() {
    try {
        const stdfs::path file = st_file();
        std::error_code ec;
        if (file.empty() || !stdfs::is_regular_file(file, ec)) return false;
        const Json doc = json::load(file);
        return doc.is_object() && doc.contains("developer_tools") && doc["developer_tools"].is_boolean() && doc["developer_tools"].get<bool>();
    } catch (const std::exception&) {
    }
    return false;
}

void remember_developer_tools(bool show) {
    try {
        const stdfs::path file = st_file();
        if (file.empty()) return;
        Json doc = Json::object();
        std::error_code ec;
        if (stdfs::is_regular_file(file, ec)) {
            try {
                doc = json::load(file);
            } catch (const std::exception&) {
            }
            if (!doc.is_object()) doc = Json::object();
        }
        doc["developer_tools"] = show;
        fs::make_dirs(file.parent_path());
        json::save(file, doc);
    } catch (const std::exception&) {
    }
}

}  // namespace cb::app::settings
