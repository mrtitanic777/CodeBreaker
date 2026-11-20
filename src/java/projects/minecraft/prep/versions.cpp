// java/projects/minecraft/prep/versions.cpp -- see versions.hpp.
// Ported from RetroMCP (org.mcphackers.mcp.tools.versions.*, tools.OS, MCPPaths), MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include "java/projects/minecraft/prep/versions.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/parallel.hpp"
#include "core/regex.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/projects/minecraft/prep/setup.hpp"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <sys/utsname.h>
#endif

namespace cb::java::minecraft::prep {

namespace stdfs = std::filesystem;

namespace {

// ---- org.json's accessors: the exceptions are std::runtime_error (a JSONException) ----

[[noreturn]] void json_fail(const std::string& what) { throw std::runtime_error("JSONException: " + what); }

const Json& require(const Json& obj, const char* key) {
    if (!obj.is_object() || !obj.contains(key)) json_fail(std::string("JSONObject[\"") + key + "\"] not found.");
    return obj[key];
}

std::string req_string(const Json& obj, const char* key) {   // getString
    const Json& v = require(obj, key);
    if (!v.is_string()) json_fail(std::string("JSONObject[\"") + key + "\"] is not a string.");
    return v.get<std::string>();
}

std::optional<std::string> opt_string(const Json& obj, const char* key) {   // optString(key, null)
    if (!obj.is_object() || !obj.contains(key)) return std::nullopt;
    const Json& v = obj[key];
    if (v.is_null()) return std::nullopt;
    if (v.is_string()) return v.get<std::string>();
    return v.dump();
}

std::string opt_string_empty(const Json& obj, const char* key) {   // optString(key): "" when there is none
    return opt_string(obj, key).value_or(std::string());
}

std::int64_t as_long(const Json& v, const char* key) {
    if (v.is_number()) return v.get<std::int64_t>();
    if (v.is_string()) {
        try {
            std::size_t used = 0;
            const std::string s = v.get<std::string>();
            const long long n = std::stoll(s, &used);
            if (used == s.size()) return n;
        } catch (const std::exception&) {
        }
    }
    json_fail(std::string("JSONObject[\"") + key + "\"] is not a long.");
}

std::int64_t req_long(const Json& obj, const char* key) { return as_long(require(obj, key), key); }   // getLong

std::int64_t opt_long(const Json& obj, const char* key) {   // optLong
    if (!obj.is_object() || !obj.contains(key)) return 0;
    try {
        return as_long(obj[key], key);
    } catch (const std::runtime_error&) {
        return 0;
    }
}

bool opt_bool(const Json& obj, const char* key) {   // optBoolean
    if (!obj.is_object() || !obj.contains(key)) return false;
    const Json& v = obj[key];
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_string()) return v.get<std::string>() == "true";
    return false;
}

const Json& req_object(const Json& obj, const char* key) {   // getJSONObject
    const Json& v = require(obj, key);
    if (!v.is_object()) json_fail(std::string("JSONObject[\"") + key + "\"] is not a JSONObject.");
    return v;
}

const Json& req_array(const Json& obj, const char* key) {   // getJSONArray
    const Json& v = require(obj, key);
    if (!v.is_array()) json_fail(std::string("JSONObject[\"") + key + "\"] is not a JSONArray.");
    return v;
}

const Json* opt_object(const Json& obj, const char* key) {   // optJSONObject
    if (!obj.is_object() || !obj.contains(key) || !obj[key].is_object()) return nullptr;
    return &obj[key];
}

const Json* opt_array(const Json& obj, const char* key) {   // optJSONArray
    if (!obj.is_object() || !obj.contains(key) || !obj[key].is_array()) return nullptr;
    return &obj[key];
}

// String.split(":"): trailing empty strings are dropped.
std::vector<std::string> split_colon(const std::string& s) {
    std::vector<std::string> parts;
    std::size_t from = 0;
    for (;;) {
        const std::size_t at = s.find(':', from);
        if (at == std::string::npos) {
            parts.push_back(s.substr(from));
            break;
        }
        parts.push_back(s.substr(from, at - from));
        from = at + 1;
    }
    while (!parts.empty() && parts.back().empty()) parts.pop_back();
    return parts;
}

std::string lib_path(const std::string& name, const std::optional<std::string>& classifier_suffix) {
    const std::vector<std::string> artifact = split_colon(name);
    if (artifact.size() < 3) throw std::runtime_error("ArrayIndexOutOfBoundsException: library name \"" + name + "\" has fewer than three parts");
    std::string group = artifact[0];
    std::replace(group.begin(), group.end(), '.', '/');
    return group + "/" + artifact[1] + "/" + artifact[2] + "/" + artifact[1] + "-" + artifact[2] +
           (classifier_suffix ? "-" + *classifier_suffix : std::string()) + ".jar";
}

Host detect_host() {
    Host h;
#ifdef _WIN32
    h.os = Os::Windows;
#if defined(_M_ARM64)
    h.arch = "aarch64";
#elif defined(_M_ARM)
    h.arch = "arm";
#elif defined(_M_IX86)
    h.arch = "x86";
#else
    h.arch = "amd64";
#endif
    // os.version is major.minor of the real version (RtlGetVersion: GetVersionEx lies to a program with no manifest)
    using RtlGetVersionFn = LONG(WINAPI*)(OSVERSIONINFOW*);
    h.version = "10.0";
    if (HMODULE ntdll = GetModuleHandleW(L"ntdll.dll")) {
        if (auto fn = reinterpret_cast<RtlGetVersionFn>(reinterpret_cast<void*>(GetProcAddress(ntdll, "RtlGetVersion")))) {
            OSVERSIONINFOW info{};
            info.dwOSVersionInfoSize = sizeof(info);
            if (fn(&info) == 0) h.version = std::to_string(info.dwMajorVersion) + "." + std::to_string(info.dwMinorVersion);
        }
    }
#else
    struct utsname u {};
    if (uname(&u) == 0) {
        const std::string sys = u.sysname;
        h.os = sys == "Darwin" ? Os::Osx : sys == "Linux" ? Os::Linux : Os::Unknown;
        const std::string m = u.machine;
        h.arch = m == "x86_64" ? "amd64" : m == "arm64" ? "aarch64" : m;
        h.version = u.release;
    }
#endif
    return h;
}

Host& host_storage() {
    static Host h = detect_host();
    return h;
}

}  // namespace

// ---- OS / host ----

Os parse_os(const std::string& name) {
    if (name == "linux") return Os::Linux;
    if (name == "windows") return Os::Windows;
    if (name == "osx") return Os::Osx;
    if (name == "unknown") return Os::Unknown;
    throw std::runtime_error("IllegalArgumentException: No enum constant org.mcphackers.mcp.tools.OS." + name);
}

const Host& host() { return host_storage(); }
void set_host(const Host& h) { host_storage() = h; }

// ---- MCPPaths ----

namespace mcp_paths {

stdfs::path get(const stdfs::path& working_dir, const std::string& path) { return working_dir / path; }

stdfs::path get(const stdfs::path& working_dir, const std::string& path, Side side) {
    auto replace_all = [](std::string s, const std::string& from, const std::string& to) {
        for (std::size_t at = s.find(from); at != std::string::npos; at = s.find(from, at + to.size())) s.replace(at, from.size(), to);
        return s;
    };
    std::string new_path = path;
    if (side == Side::Client) new_path = replace_all(path, "minecraft_%s", "minecraft");
    return get(working_dir, replace_all(new_path, "%s", side_name(side)));   // String.format(path, side.name)
}

}  // namespace mcp_paths

// ---- the version JSON ----

Artifact Artifact::from(const Json& obj, std::optional<std::string> name) {
    Artifact a;
    a.name = std::move(name);
    a.path = opt_string(obj, "path");
    a.sha1 = opt_string(obj, "sha1");
    a.url = opt_string(obj, "url");
    a.size = opt_long(obj, "size");
    return a;
}

Classifiers Classifiers::from(const Json& obj) {
    Classifiers c;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        const std::string key = it.key();
        const Json* o = opt_object(obj, key.c_str());
        c.artifacts[key] = o != nullptr ? std::optional<Artifact>(Artifact::from(*o, key)) : std::nullopt;
    }
    return c;
}

Downloads Downloads::from(const Json& obj) {
    Downloads d;
    if (const Json* a = opt_object(obj, "artifact")) d.artifact = Artifact::from(*a, std::nullopt);
    if (const Json* c = opt_object(obj, "classifiers")) d.classifiers = Classifiers::from(*c);
    return d;
}

Rule::OSInfo Rule::OSInfo::from(const Json& obj) {
    OSInfo info;
    info.name = parse_os(req_string(obj, "name"));
    info.arch = opt_string_empty(obj, "arch");
    info.version = opt_string_empty(obj, "version");
    return info;
}

bool Rule::OSInfo::equals_os(Os other) const {
    if (name != other) return false;
    // (name is never null in RetroMCP's OSInfo.from; arch and version are optString's, so never null either: "" is a value that equals nothing.)
    if (arch != host().arch) return false;
    try {
        const Regex pattern(version);
        if (!pattern.full_match(host().version)) return false;
    } catch (const Error&) {   // a pattern that does not compile: ignored, as RetroMCP's catch (Throwable) does
    }
    return true;
}

Rule Rule::from(const Json& obj) {
    if (!obj.is_object()) json_fail("a rule is not a JSONObject.");
    Rule r;
    const std::string action = req_string(obj, "action");
    if (action == "allow") r.action = Action::Allow;
    else if (action == "disallow") r.action = Action::Disallow;
    else throw std::runtime_error("IllegalArgumentException: No enum constant Rule.Action." + action);
    if (const Json* o = opt_object(obj, "os")) r.os = OSInfo::from(*o);
    if (const Json* f = opt_object(obj, "features")) {
        for (auto it = f->begin(); it != f->end(); ++it) {
            const std::string key = it.key();
            const Json& v = it.value();
            if (!v.is_boolean()) json_fail("JSONObject[\"" + key + "\"] is not a Boolean.");
            r.features.emplace_back(key, v.get<bool>());
        }
    }
    return r;
}

bool Rule::apply(const std::vector<Rule>& rules) { return apply(rules, {}); }

bool Rule::apply(const std::vector<Rule>& rules, const std::vector<std::string>& features) {
    if (rules.empty()) return true;
    Action last = Action::Disallow;
    for (const Rule& r : rules)
        if (const auto a = r.get_applied_action(features)) last = *a;
    return last == Action::Allow;
}

std::optional<Rule::Action> Rule::get_applied_action(const std::vector<std::string>& features_list) const {
    for (const auto& [key, wanted] : features) {
        const bool match = (std::find(features_list.begin(), features_list.end(), key) != features_list.end()) == wanted;
        if (!match) return Action::Disallow;
    }
    if (os && !os->equals_os(host().os)) return std::nullopt;
    return action;
}

AssetIndexMeta AssetIndexMeta::from(const Json& obj) {
    AssetIndexMeta m;
    m.id = req_string(obj, "id");
    m.sha1 = req_string(obj, "sha1");
    m.size = req_long(obj, "size");
    m.total_size = req_long(obj, "totalSize");
    m.url = req_string(obj, "url");
    return m;
}

std::string Asset::download_url() const { return url ? *url : "https://resources.download.minecraft.net/" + download_path(); }

AssetIndex AssetIndex::from(const Json& obj) {
    AssetIndex a;
    a.is_virtual = opt_bool(obj, "virtual");
    a.map_to_resources = opt_bool(obj, "map_to_resources");
    if (const Json* objects = opt_object(obj, "objects")) {
        for (auto it = objects->begin(); it != objects->end(); ++it) {
            const std::string key = it.key();
            const Json& o = req_object(*objects, key.c_str());
            Asset asset;
            asset.hash = req_string(o, "hash");
            asset.size = req_long(o, "size");
            asset.url = opt_string(o, "url");
            auto found = std::find_if(a.objects.begin(), a.objects.end(), [&](const auto& e) { return e.first == key; });
            if (found != a.objects.end()) found->second = asset;
            else a.objects.emplace_back(key, std::move(asset));
        }
    }
    return a;
}

DependDownload DependDownload::from(const Json& obj) {
    DependDownload d;
    d.name = req_string(obj, "name");
    d.url = opt_string(obj, "url");
    if (const Json* dl = opt_object(obj, "downloads")) d.downloads = Downloads::from(*dl);
    if (const Json* rules = opt_array(obj, "rules"))
        for (const Json& r : *rules) d.rules.push_back(Rule::from(r));
    return d;
}

const Artifact* DependDownload::get_artifact(const std::optional<std::string>& artifact_name) const {
    if (!artifact_name) return downloads && downloads->artifact ? &*downloads->artifact : nullptr;
    if (!downloads || !downloads->classifiers) return nullptr;
    const auto& map = downloads->classifiers->artifacts;
    const auto it = map.find(*artifact_name);
    return it != map.end() && it->second ? &*it->second : nullptr;
}

std::optional<std::string> DependDownload::get_artifact_url(const std::optional<std::string>& artifact_name) const {
    const Artifact* artifact = get_artifact(artifact_name);
    if (artifact == nullptr) return std::nullopt;
    if (artifact->url) return *artifact->url;
    std::string url_base = "https://libraries.minecraft.net/";
    if (url) {
        if (url->empty()) throw std::runtime_error("StringIndexOutOfBoundsException: a library's url is empty");
        url_base = url->back() == '/' ? *url : *url + "/";
    }
    return url_base + lib_path(name, artifact->name);
}

std::optional<std::string> DependDownload::get_artifact_path(const std::optional<std::string>& artifact_name) const {
    const Artifact* artifact = get_artifact(artifact_name);
    if (artifact == nullptr) return std::nullopt;
    if (artifact->path) return *artifact->path;
    return lib_path(name, artifact->name);
}

std::optional<DownloadSpec> DependDownload::get_download(const std::optional<std::string>& artifact_name) const {
    const Artifact* artifact = get_artifact(artifact_name);
    if (artifact == nullptr) return std::nullopt;
    DownloadSpec spec;
    spec.url = get_artifact_url(artifact_name).value_or(std::string());
    spec.path = get_artifact_path(artifact_name);
    spec.size = artifact->size;
    spec.hash = artifact->sha1;
    spec.verify = true;
    return spec;
}

std::optional<std::string> DependDownload::get_natives() const {
    const Host& h = host();
    auto has = [&](const char* n) { return get_artifact(std::string(n)) != nullptr; };
    switch (h.os) {
        case Os::Windows:
            if (has("natives-windows")) return "natives-windows";
            break;
        case Os::Linux:
            if (h.is_arm32() && has("natives-linux-arm32")) return "natives-linux-arm32";
            if (h.is_arm64() && has("natives-linux-arm64")) return "natives-linux-arm64";
            if (has("natives-linux")) return "natives-linux";
            break;
        case Os::Osx:
            if (h.is_m_series() && has("natives-osx-arm64")) return "natives-osx-arm64";
            if (has("natives-osx")) return "natives-osx";
            if (has("natives-macos")) return "natives-macos";
            break;
        default:
            break;
    }
    return std::nullopt;
}

Version Version::from(const Json& obj) {
    Version v;
    v.asset_index = AssetIndexMeta::from(req_object(obj, "assetIndex"));
    v.assets = req_string(obj, "assets");
    v.downloads = Classifiers::from(req_object(obj, "downloads"));
    v.id = req_string(obj, "id");
    v.time = req_string(obj, "time");
    v.release_time = req_string(obj, "releaseTime");
    v.type = req_string(obj, "type");
    for (const Json& o : req_array(obj, "libraries"))
        if (o.is_object()) v.libraries.push_back(DependDownload::from(o));
    v.main_class = req_string(obj, "mainClass");
    if (const Json* args = opt_object(obj, "arguments")) {
        Arguments a;
        auto read = [](const Json& list, std::vector<ArgumentItem>& out) {
            for (const Json& item : list) {
                ArgumentItem it;
                if (item.is_object()) {
                    Argument arg;
                    for (const Json& r : req_array(item, "rules")) arg.rules.push_back(Rule::from(r));
                    arg.value_json = require(item, "value").dump();
                    it.argument = std::move(arg);
                } else {
                    it.plain_json = item.dump();
                }
                out.push_back(std::move(it));
            }
        };
        read(req_array(*args, "game"), a.game);
        read(req_array(*args, "jvm"), a.jvm);
        v.arguments = std::move(a);
    }
    v.minecraft_arguments = opt_string(obj, "minecraftArguments");
    return v;
}

// ---- VersionParser ----

VersionData VersionData::from(const Json& obj) {
    VersionData d;
    d.id = req_string(obj, "id");
    d.time = req_string(obj, "time");
    d.release_time = req_string(obj, "releaseTime");
    d.type = req_string(obj, "type");
    d.url = req_string(obj, "url");
    d.resources = opt_string(obj, "resources");
    return d;
}

std::string VersionData::to_string() const {
    auto starts = [&](const char* p) { return id.rfind(p, 0) == 0; };
    std::string typ, ver;
    if (starts("rd") && type == "old_alpha") {
        typ = "Pre-Classic";
        ver = id;
    } else if (starts("c") && type == "old_alpha") {
        typ = "Classic";
        ver = id.substr(1);
    } else if (starts("inf-")) {
        typ = "Infdev";
        ver = id.substr(4);
    } else if (starts("in-")) {
        typ = "Indev";
        ver = id.substr(3);
    } else if (starts("a") && type == "old_alpha") {
        typ = "Alpha";
        ver = id.substr(1);
    } else if (starts("b")) {
        typ = "Beta";
        ver = id.substr(1);
    } else {
        typ = type.empty() ? type : std::string(1, static_cast<char>(std::toupper(static_cast<unsigned char>(type[0])))) + type.substr(1);
        ver = id;
    }
    return typ + " " + ver;
}

VersionParser::VersionParser(const std::string& url) { add_versions_from_url(url); }

void VersionParser::add_versions_from_url(const std::string& url) {
    Json doc;
    try {
        std::string text;
        if (http::is_http_url(url)) {
            const Bytes body = http::get(url);
            text = std::string(as_text(body));
        } else {
            text = fs::read_text(stdfs::path(url));
        }
        doc = json::parse(text);
        if (!doc.is_array()) json_fail("A JSONArray text must start with '['");
    } catch (const std::exception& e) {
        failure_cause = e.what();
        return;   // couldn't init json
    }
    for (const Json& j : doc) {
        if (!j.is_object()) continue;
        try {
            VersionData data = VersionData::from(j);
            auto found = std::find_if(versions_.begin(), versions_.end(), [&](const VersionData& v) { return v.id == data.id; });
            if (found != versions_.end()) *found = std::move(data);
            else versions_.push_back(std::move(data));
        } catch (const std::exception& e) {
            warnings.push_back(e.what());   // skips to the next version
        }
    }
}

const VersionData* VersionParser::get_version(const std::string& id) const {
    const auto it = std::find_if(versions_.begin(), versions_.end(), [&](const VersionData& v) { return v.id == id; });
    return it == versions_.end() ? nullptr : &*it;
}

std::vector<VersionData> VersionParser::get_unsorted_versions() const {
    decompiler::JavaHashMap<std::string, VersionData, decompiler::JavaStringHash> map;   // the HashMap RetroMCP keeps them in
    for (const VersionData& v : versions_) map.put(v.id, v);
    return map.values();
}

namespace {

// DateTimeFormatter.ISO_DATE_TIME parsed to an Instant (seconds, nanos); nullopt when it does not parse or has no offset.
std::optional<std::pair<std::int64_t, std::int64_t>> parse_instant(const std::string& s) {
    std::size_t i = 0;
    // n digits at i (fixed width), or nullopt
    auto digits = [&](std::size_t n) -> std::optional<int> {
        if (i + n > s.size()) return std::nullopt;
        int v = 0;
        for (std::size_t k = 0; k < n; ++k) {
            if (!std::isdigit(static_cast<unsigned char>(s[i + k]))) return std::nullopt;
            v = v * 10 + (s[i + k] - '0');
        }
        i += n;
        return v;
    };
    auto expect = [&](char c) {
        if (i < s.size() && s[i] == c) {
            ++i;
            return true;
        }
        return false;
    };
    int sec = 0;
    const auto oy = digits(4);
    if (!oy || !expect('-')) return std::nullopt;
    const auto omo = digits(2);
    if (!omo || !expect('-')) return std::nullopt;
    const auto od = digits(2);
    if (!od || !expect('T')) return std::nullopt;
    const auto oh = digits(2);
    if (!oh || !expect(':')) return std::nullopt;
    const auto omi = digits(2);
    if (!omi) return std::nullopt;
    int y = *oy;
    const int mo = *omo, d = *od, h = *oh, mi = *omi;
    std::int64_t nanos = 0;
    if (expect(':')) {
        const auto os = digits(2);
        if (!os) return std::nullopt;
        sec = *os;
        if (expect('.')) {
            std::int64_t scale = 100000000;
            while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) {
                nanos += (s[i] - '0') * scale;
                scale /= 10;
                ++i;
            }
        }
    }
    std::int64_t offset = 0;
    if (expect('Z')) {
    } else if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
        const int sign = s[i] == '-' ? -1 : 1;
        ++i;
        const auto ooh = digits(2);
        if (!ooh || !expect(':')) return std::nullopt;
        const auto oom = digits(2);
        if (!oom) return std::nullopt;
        offset = (*ooh * 3600 + *oom * 60) * sign;
    } else {
        return std::nullopt;   // an Instant needs an offset
    }
    // days from civil (Howard Hinnant)
    y -= mo <= 2;
    const std::int64_t era = (y >= 0 ? y : y - 399) / 400;
    const std::int64_t yoe = y - era * 400;
    const std::int64_t doy = (153 * (mo + (mo > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const std::int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    const std::int64_t days = era * 146097 + doe - 719468;
    return std::pair<std::int64_t, std::int64_t>{days * 86400 + h * 3600 + mi * 60 + sec - offset, nanos};
}

}  // namespace

std::vector<VersionData> VersionParser::get_sorted_versions() const {
    std::vector<VersionData> sorted = get_unsorted_versions();
    // VersionSorter: newer first; a date that does not parse compares as "before".
    auto compare = [](const VersionData& a, const VersionData& b) {
        const auto i1 = parse_instant(a.release_time), i2 = parse_instant(b.release_time);
        if (!i1 || !i2) return -1;
        return *i2 < *i1 ? -1 : *i2 > *i1 ? 1 : 0;
    };
    std::stable_sort(sorted.begin(), sorted.end(), [&](const VersionData& a, const VersionData& b) { return compare(a, b) < 0; });
    return sorted;
}

// ---- DownloadData ----

std::string get_sha1(const stdfs::path& file) { return crypto::to_hex(crypto::sha1(fs::read_file(file))); }

DownloadData::DownloadData(const stdfs::path& working_dir, Side side, const Version& version, const http::Options& http)
    : DownloadData(side, mcp_paths::get(working_dir, mcp_paths::LIB), mcp_paths::get(working_dir, mcp_paths::JARS),
                   mcp_paths::get(working_dir, mcp_paths::JAR_ORIGINAL, Side::Client), mcp_paths::get(working_dir, mcp_paths::JAR_ORIGINAL, Side::Server),
                   version, http) {}

DownloadData::DownloadData(Side side, const stdfs::path& libraries, const stdfs::path& game_dir, const stdfs::path& client, const stdfs::path& server,
                           const Version& version, const http::Options& http)
    : game_dir_(game_dir), http_(http) {
    auto artifact_of = [&](const char* key) -> const Artifact* {
        const auto it = version.downloads.artifacts.find(key);
        return it != version.downloads.artifacts.end() && it->second ? &*it->second : nullptr;
    };
    if (includes_client(side)) {
        if (const Artifact* a = artifact_of("client")) queue_download(a->spec(), client);
    }
    const Artifact* server_artifact = artifact_of("server");
    if (includes_server(side) && server_artifact != nullptr) {
        if (!server_artifact->url) throw std::runtime_error("NullPointerException: the server download has no url");
        const bool zip = server_artifact->url->size() >= 4 && server_artifact->url->compare(server_artifact->url->size() - 4, 4, ".zip") == 0;
        stdfs::path server_out = server;
        if (zip) server_out = server.parent_path() / "minecraft_server.zip";
        queue_download(server_artifact->spec(), server_out);
        if (zip) {
            // (as RetroMCP: this runs before anything is downloaded, so it unpacks only a zip an earlier setup left)
            try {
                extract_zip(server_out, server_out.parent_path(), std::string(".jar"));
                if (stdfs::exists(server_out.parent_path() / "minecraft-server.jar")) {
                    if (stdfs::exists(server)) throw IoError(server, 0, "already exists");
                    stdfs::rename(server_out.parent_path() / "minecraft-server.jar", server);
                }
            } catch (const std::exception& e) {
                messages.push_back(std::string("the server zip: ") + e.what());
            }
        }
    }
    for (const DependDownload& dep : version.libraries) {
        if (Rule::apply(dep.rules)) {
            queue_download(dep.get_download(std::nullopt), libraries);
            queue_download(dep.get_download(dep.get_natives()), libraries);
            queue_download(dep.get_download(std::string("sources")), libraries);
        }
    }
    try {
        const stdfs::path asset_index = game_dir_ / ("assets/indexes/" + version.assets + ".json");
        stdfs::create_directories(asset_index.parent_path());
        std::string text;
        if (!stdfs::exists(asset_index) || version.asset_index.sha1 != get_sha1(asset_index)) {
            const Bytes body = http::get(version.asset_index.url, http_);
            text = std::string(as_text(body));
            fs::write_text(asset_index, text);
        } else {
            text = fs::read_text(asset_index);
        }
        set_assets(AssetIndex::from(json::parse(text)));
    } catch (const IoError& e) {
        messages.push_back(std::string("the asset index: ") + e.what());
    } catch (const http::HttpError& e) {
        messages.push_back(std::string("the asset index: ") + e.what());
    } catch (const stdfs::filesystem_error& e) {
        messages.push_back(std::string("the asset index: ") + e.what());
    }
}

std::vector<std::string> DownloadData::get_libraries(const Version& version) {
    std::vector<std::string> out;
    for (const DependDownload& dep : version.libraries)
        if (Rule::apply(dep.rules)) {
            const auto lib = dep.get_artifact_path(std::nullopt);
            if (lib) out.push_back(*lib);
            else throw std::runtime_error("a library without a main artifact");   // (RetroMCP adds a null)
        }
    return out;
}

std::vector<stdfs::path> DownloadData::get_libraries(const stdfs::path& lib_dir, const Version& version) {
    std::vector<stdfs::path> out;
    for (const DependDownload& dep : version.libraries)
        if (Rule::apply(dep.rules)) {
            const auto lib = dep.get_artifact_path(std::nullopt);
            if (!lib) continue;
            out.push_back(lib_dir / *lib);
        }
    return out;
}

std::vector<stdfs::path> DownloadData::get_natives(const stdfs::path& lib_dir, const Version& version) {
    std::vector<stdfs::path> out;
    for (const DependDownload& dep : version.libraries)
        if (Rule::apply(dep.rules)) {
            const auto natives = dep.get_natives();
            if (!natives) continue;
            const auto lib = dep.get_artifact_path(natives);
            if (!lib) continue;
            out.push_back(lib_dir / *lib);
        }
    return out;
}

void DownloadData::set_assets(const AssetIndex& assets) {
    if (assets_) return;
    assets_ = assets;
    const stdfs::path dir = game_dir_ / (assets.map_to_resources ? "resources/" : "assets/objects/");
    for (const auto& [key, asset] : assets.objects) queue_download(asset.spec(), dir);
}

void DownloadData::queue_download(const std::optional<DownloadSpec>& dl, const stdfs::path& base_dir) {
    if (!dl) return;
    total_size += dl->size;
    queue_.push_back({*dl, base_dir});
}

void DownloadData::perform_download(const Listener& listener) {
    // Hundreds of tiny files: fetched side by side (16 at a time, as RetroMCP), the notifications one at a time.
    const std::size_t threads = std::min<std::size_t>(16, std::max<std::size_t>(1, queue_.size()));
    parallel::ThreadPool pool(threads);
    std::mutex notify_lock, file_locks_lock;
    std::map<std::string, std::shared_ptr<std::mutex>> file_locks;
    parallel::parallel_for(pool, queue_.size(), [&](std::size_t i) {
        const Queued& q = queue_[i];
        const DownloadSpec& dl = q.download;
        // if the path is null then the folder is the location of the downloaded file
        const stdfs::path file = dl.path ? q.dir / *dl.path : q.dir;
        // a library is queued twice when it has no natives (its main artifact is also the "natives" one): one at a time per file, so the
        // second finds the first's file and its hash right (RetroMCP's two threads race on the file; here the second waits)
        std::shared_ptr<std::mutex> file_lock;
        {
            const std::lock_guard<std::mutex> guard(file_locks_lock);
            auto& slot = file_locks[file.string()];
            if (!slot) slot = std::make_shared<std::mutex>();
            file_lock = slot;
        }
        const std::lock_guard<std::mutex> one_at_a_time(*file_lock);
        if (!stdfs::exists(file) || (dl.verify && !(dl.hash ? *dl.hash == get_sha1(file) : throw std::runtime_error("NullPointerException: no hash for " + dl.url)))) {
            const stdfs::path parent = file.parent_path();
            if (!parent.empty()) stdfs::create_directories(parent);
            if (dl.url.empty()) throw std::runtime_error("NullPointerException: no url for " + file.string());
            http::get_to_file(dl.url, file, http_);
        }
        const std::lock_guard<std::mutex> lock(notify_lock);
        if (listener) listener(dl, total_size);
    });
}

// ---- org.json's writer ----

namespace {

void write_indent(std::string& out, int n) { out.append(static_cast<std::size_t>(n), ' '); }

// JSONObject.quote
void write_quoted(std::string& out, const std::string& s) {
    out += '"';
    bool prev_lt = false;
    for (std::size_t i = 0; i < s.size();) {
        const unsigned char c0 = static_cast<unsigned char>(s[i]);
        std::uint32_t cp = c0;
        std::size_t len = 1;
        if (c0 >= 0xF0 && i + 3 < s.size()) { cp = ((c0 & 7u) << 18) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 12) | ((static_cast<unsigned char>(s[i + 2]) & 0x3Fu) << 6) | (static_cast<unsigned char>(s[i + 3]) & 0x3Fu); len = 4; }
        else if (c0 >= 0xE0 && i + 2 < s.size()) { cp = ((c0 & 15u) << 12) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 6) | (static_cast<unsigned char>(s[i + 2]) & 0x3Fu); len = 3; }
        else if (c0 >= 0xC0 && i + 1 < s.size()) { cp = ((c0 & 31u) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu); len = 2; }
        const bool b_lt = prev_lt;
        prev_lt = cp == '<';
        switch (cp) {
            case '\\': out += "\\\\"; break;
            case '"': out += "\\\""; break;
            case '/':
                if (b_lt) out += '\\';
                out += '/';
                break;
            case '\b': out += "\\b"; break;
            case '\t': out += "\\t"; break;
            case '\n': out += "\\n"; break;
            case '\f': out += "\\f"; break;
            case '\r': out += "\\r"; break;
            default:
                if (cp < 0x20 || (cp >= 0x80 && cp < 0xA0) || (cp >= 0x2000 && cp < 0x2100)) {
                    static const char* hex = "0123456789abcdef";
                    out += "\\u";
                    for (int shift = 12; shift >= 0; shift -= 4) out += hex[(cp >> shift) & 15];
                } else {
                    out.append(s, i, len);
                }
        }
        i += len;
    }
    out += '"';
}

void write_value(std::string& out, const Json& v, int factor, int indent);

void write_object(std::string& out, const Json& obj, int factor, int indent) {
    // org.json keeps an object's entries in a java.util.HashMap
    decompiler::JavaHashMap<std::string, const Json*, decompiler::JavaStringHash> map;
    for (auto it = obj.begin(); it != obj.end(); ++it) map.put(it.key(), &it.value());
    const auto entries = map.entries();
    out += '{';
    if (entries.size() == 1) {
        write_quoted(out, entries[0].first);
        out += ':';
        if (factor > 0) out += ' ';
        write_value(out, *entries[0].second, factor, indent);
    } else if (!entries.empty()) {
        const int new_indent = indent + factor;
        bool need_comma = false;
        for (const auto& [key, value] : entries) {
            if (need_comma) out += ',';
            if (factor > 0) out += '\n';
            write_indent(out, new_indent);
            write_quoted(out, key);
            out += ':';
            if (factor > 0) out += ' ';
            write_value(out, *value, factor, new_indent);
            need_comma = true;
        }
        if (factor > 0) out += '\n';
        write_indent(out, indent);
    }
    out += '}';
}

void write_array(std::string& out, const Json& arr, int factor, int indent) {
    out += '[';
    if (arr.size() == 1) {
        write_value(out, arr[0], factor, indent);
    } else if (!arr.empty()) {
        const int new_indent = indent + factor;
        bool first = true;
        for (const Json& item : arr) {
            if (!first) out += ',';
            first = false;
            if (factor > 0) out += '\n';
            write_indent(out, new_indent);
            write_value(out, item, factor, new_indent);
        }
        if (factor > 0) out += '\n';
        write_indent(out, indent);
    }
    out += ']';
}

void write_value(std::string& out, const Json& v, int factor, int indent) {
    if (v.is_null()) out += "null";
    else if (v.is_boolean()) out += v.get<bool>() ? "true" : "false";
    else if (v.is_number_integer() || v.is_number_unsigned()) out += v.dump();
    else if (v.is_number_float()) {
        std::string t = v.dump();   // JSONObject.numberToString: a decimal loses its trailing zeros
        if (t.find('.') != std::string::npos && t.find('e') == std::string::npos && t.find('E') == std::string::npos) {
            while (!t.empty() && t.back() == '0') t.pop_back();
            if (!t.empty() && t.back() == '.') t.pop_back();
        }
        out += t;
    } else if (v.is_string()) write_quoted(out, v.get<std::string>());
    else if (v.is_object()) write_object(out, v, factor, indent);
    else if (v.is_array()) write_array(out, v, factor, indent);
}

}  // namespace

std::string org_json_text(const Json& json, int indent_factor) {
    std::string out;
    write_value(out, json, indent_factor, 0);
    return out;
}

}  // namespace cb::java::minecraft::prep
