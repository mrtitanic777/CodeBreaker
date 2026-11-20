// java/projects/minecraft/prep/versions.hpp -- Minecraft's versions as RetroMCP's setup reads them: the list of versions (VersionParser), the version
// JSON of one (Version, its libraries and their rules for this operating system, the artifacts, the asset index), and what to download for it
// (DownloadData: the client and server jars, the libraries and their natives, the assets).
//
//     prep::VersionParser parser;                                  // https://mcphackers.org/versionsV3/versions.json
//     const prep::VersionData* v = parser.get_version("1.5.2");    // id, type, release time, the version JSON's URL, the conf zip's URL
//     prep::Version version = prep::Version::from(json);           // the version JSON
//     prep::DownloadData dl(paths, Side::Any, version);            // queues; fetches the asset index
//     dl.perform_download(listener);                               // fetches what is missing or does not match its hash
//     for (auto& native : prep::DownloadData::get_natives(lib_dir, version)) ...
//
// Ported from RetroMCP (org.mcphackers.mcp.tools.versions: VersionParser, DownloadData, IDownload; ...versions.json: Artifact, AssetIndex,
// AssetIndexMeta, Classifiers, DependDownload, Downloads, Rule, Version, VersionMetadata; tools.OS; MCPPaths), MIT License, (c) MCPHackers.
// A Java class keeps its name; the downloads use CodeBreaker's WinHTTP client (core/http.hpp, User-Agent "CodeBreaker").
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/http.hpp"
#include "core/json_fwd.hpp"

namespace cb::java::minecraft::prep {

// ---- tools.OS and the host the rules are judged for ----

enum class Os { Linux, Windows, Osx, Unknown };
// OS.valueOf: "linux", "windows", "osx", "unknown"; anything else is a std::runtime_error (IllegalArgumentException).
Os parse_os(const std::string& name);

// What java's os.name / os.arch / os.version say of this machine (Windows: "amd64" / "10.0" on x64 Windows 10 and 11).
struct Host {
    Os os = Os::Unknown;
    std::string arch;       // os.arch: "amd64", "x86", "aarch64", "arm" ...
    std::string version;    // os.version: "10.0"
    bool is_arm32() const { return arch == "arm"; }
    bool is_arm64() const { return arch == "aarch64"; }
    bool is_m_series() const { return os == Os::Osx && is_arm64(); }
};
const Host& host();                 // this machine
void set_host(const Host& host);    // judge for another one (tests)

// ---- Side (tasks.Task.Side) and MCPPaths ----

enum class Side { Any, Client, Server, Merged };
inline const char* side_name(Side s) {
    switch (s) {
        case Side::Any: return "any";
        case Side::Client: return "client";
        case Side::Server: return "server";
        case Side::Merged: return "merged";
    }
    return "any";
}
inline bool includes_client(Side s) { return s == Side::Any || s == Side::Merged || s == Side::Client; }
inline bool includes_server(Side s) { return s == Side::Any || s == Side::Merged || s == Side::Server; }
inline constexpr Side kAllSides[] = {Side::Client, Side::Server, Side::Merged};   // Side.ALL

// MCPPaths: the workspace layout (every path relative to the workspace folder).
namespace mcp_paths {
inline constexpr const char* JARS = "jars/";
inline constexpr const char* LIB = "libraries/";
inline constexpr const char* CONF = "conf/";
inline constexpr const char* BUILD = "build/";
inline constexpr const char* PROJECT = "minecraft_%s/";
inline constexpr const char* JAR_ORIGINAL = "jars/minecraft_%s.jar";
inline constexpr const char* NATIVES = "libraries/natives";
inline constexpr const char* BUILD_ZIP = "build/minecraft_%s.zip";
inline constexpr const char* BUILD_JAR = "build/minecraft_%s.jar";
inline constexpr const char* SOURCE_UNPATCHED = "minecraft_%s/src_original";
inline constexpr const char* SOURCE = "minecraft_%s/src";
inline constexpr const char* BIN = "minecraft_%s/bin";
inline constexpr const char* MD5_DIR = "minecraft_%s/md5";
inline constexpr const char* MD5 = "minecraft_%s/md5/original.md5";
inline constexpr const char* MD5_RO = "minecraft_%s/md5/modified.md5";
inline constexpr const char* JARS_DIR = "minecraft_%s/jars";
inline constexpr const char* REMAPPED = "minecraft_%s/jars/deobfuscated.jar";
inline constexpr const char* REOBF_JAR = "minecraft_%s/jars/reobfuscated.jar";
inline constexpr const char* SOURCE_JAR = "minecraft_%s/jars/deobfuscated-source.jar";
inline constexpr const char* REOBF_SIDE = "minecraft_%s/reobf";
inline constexpr const char* GAMEDIR = "minecraft_%s/game/";
inline constexpr const char* MAPPINGS = "conf/mappings.tiny";
inline constexpr const char* EXC = "conf/exceptions.exc";
inline constexpr const char* ACCESS = "conf/%s.access";
inline constexpr const char* CONF_PATCHES = "conf/%s.patch";
inline constexpr const char* PATCHES = "patches/%s.patch";
inline constexpr const char* VERSION = "conf/version.json";
inline constexpr const char* UPDATE_JAR = "update.jar";

// MCPPaths.get(mcp, path): the workspace's folder joined with the path.
std::filesystem::path get(const std::filesystem::path& working_dir, const std::string& path);
// MCPPaths.get(mcp, path, side): "%s" is the side's name, and for the client "minecraft_%s" is just "minecraft" (minecraft/, jars/minecraft.jar).
std::filesystem::path get(const std::filesystem::path& working_dir, const std::string& path, Side side);
}  // namespace mcp_paths

// ---- the version JSON (tools.versions.json) ----

// IDownload: one file to fetch. `path` is relative to the folder it is queued under (none: the folder IS the file).
struct DownloadSpec {
    std::optional<std::string> path;
    std::string url;                    // empty: the JSON gave none
    std::int64_t size = 0;
    std::optional<std::string> hash;    // sha1 (assets: the hash that names them)
    bool verify = true;
};

struct Artifact {
    std::optional<std::string> path, name, sha1, url;
    std::int64_t size = 0;
    static Artifact from(const Json& obj, std::optional<std::string> name);
    DownloadSpec spec() const { return {path, url.value_or(std::string()), size, sha1, true}; }
};

struct Classifiers {
    std::map<std::string, std::optional<Artifact>> artifacts;   // a key whose value is not an object has no artifact
    static Classifiers from(const Json& obj);
};

struct Downloads {
    std::optional<Artifact> artifact;
    std::optional<Classifiers> classifiers;
    static Downloads from(const Json& obj);
};

struct Rule {
    enum class Action { Allow, Disallow };
    struct OSInfo {
        Os name = Os::Unknown;
        std::string arch;       // optString: "" when the JSON has none (and "" matches no os.arch)
        std::string version;    // a regular expression the whole os.version must match; "" matches nothing
        static OSInfo from(const Json& obj);
        bool equals_os(Os os) const;
    };
    Action action = Action::Allow;
    std::optional<OSInfo> os;
    std::vector<std::pair<std::string, bool>> features;

    static Rule from(const Json& obj);
    // The rules of a library or argument: the last that applies decides; none listed: allowed.
    static bool apply(const std::vector<Rule>& rules);
    static bool apply(const std::vector<Rule>& rules, const std::vector<std::string>& features);
    std::optional<Action> get_applied_action(const std::vector<std::string>& features) const;
};

struct AssetIndexMeta {
    std::string id, sha1;
    std::int64_t size = 0, total_size = 0;
    std::string url;
    static AssetIndexMeta from(const Json& obj);
};

struct Asset {
    std::string hash;
    std::optional<std::string> url;
    std::int64_t size = 0;
    std::string download_url() const;       // the JSON's, else resources.download.minecraft.net/<hash[0:2]>/<hash>
    std::string download_path() const { return hash.substr(0, 2) + "/" + hash; }
    DownloadSpec spec() const { return {download_path(), download_url(), size, hash, true}; }
};

struct AssetIndex {
    std::vector<std::pair<std::string, Asset>> objects;   // (document order; RetroMCP's is java.util.HashMap's -- only the queue order differs)
    bool is_virtual = false, map_to_resources = false;
    static AssetIndex from(const Json& obj);
};

struct DependDownload {
    std::optional<Downloads> downloads;
    std::string name;
    std::optional<std::string> url;
    std::vector<Rule> rules;

    static DependDownload from(const Json& obj);
    std::optional<std::string> get_artifact_url(const std::optional<std::string>& artifact_name) const;
    std::optional<DownloadSpec> get_download(const std::optional<std::string>& artifact_name) const;
    std::optional<std::string> get_artifact_path(const std::optional<std::string>& artifact_name) const;
    // The classifier of this machine's natives ("natives-windows"), if the library has one.
    std::optional<std::string> get_natives() const;

private:
    const Artifact* get_artifact(const std::optional<std::string>& name) const;
};

struct Version {
    struct Argument {
        std::vector<Rule> rules;
        std::string value_json;     // the "value" (a string or a list), as JSON text
    };
    // One entry of "game" or "jvm": a plain value (its JSON text) or an argument with rules.
    struct ArgumentItem {
        std::optional<Argument> argument;
        std::string plain_json;
    };
    struct Arguments {
        std::vector<ArgumentItem> game, jvm;
    };

    AssetIndexMeta asset_index;
    std::string assets;
    Classifiers downloads;
    std::string id, time, release_time, type;
    std::vector<DependDownload> libraries;
    std::string main_class;
    std::optional<std::string> minecraft_arguments;
    std::optional<Arguments> arguments;

    // std::runtime_error (a JSONException) for a required key that is missing or the wrong kind.
    static Version from(const Json& obj);
};

// ---- VersionParser ----

struct VersionData {
    std::string id, type, time, release_time, url;
    std::optional<std::string> resources;   // the conf zip (mappings, exceptions, access, patches)
    static VersionData from(const Json& obj);
    std::string to_string() const;          // "Beta 1.7.3", "Infdev 20100618", "Classic 0.30-c-1900", "Release 1.5.2"
};

class VersionParser {
public:
    static constexpr const char* DEFAULT_JSON = "https://mcphackers.org/versionsV3/versions.json";

    // Reads the versions from the URL (an http(s) URL, or a file); a failure leaves the list empty and failure_cause set.
    explicit VersionParser(const std::string& url = DEFAULT_JSON);
    void add_versions_from_url(const std::string& url);

    // The version of that id/name, or null.
    const VersionData* get_version(const std::string& id) const;
    // In java.util.HashMap order.
    std::vector<VersionData> get_unsorted_versions() const;
    // Newest release time first (VersionSorter).
    std::vector<VersionData> get_sorted_versions() const;

    std::optional<std::string> failure_cause;   // the message of what stopped the list from loading
    std::vector<std::string> warnings;          // versions that were skipped (a required key missing)

private:
    std::vector<VersionData> versions_;   // in document order; a later one with the same id replaces the earlier
};

// ---- DownloadData ----

class DownloadData {
public:
    // Notified once per finished download (any thread, one at a time) with the download and the total size queued.
    using Listener = std::function<void(const DownloadSpec&, std::int64_t total_size)>;

    // The workspace's folders: libraries/, jars/ (the game dir: assets go under it), jars/minecraft.jar, jars/minecraft_server.jar.
    DownloadData(const std::filesystem::path& working_dir, Side side, const Version& version, const http::Options& http = {});
    DownloadData(Side side, const std::filesystem::path& libraries, const std::filesystem::path& game_dir, const std::filesystem::path& client,
                 const std::filesystem::path& server, const Version& version, const http::Options& http = {});

    static std::vector<std::string> get_libraries(const Version& version);
    static std::vector<std::filesystem::path> get_libraries(const std::filesystem::path& lib_dir, const Version& version);
    static std::vector<std::filesystem::path> get_natives(const std::filesystem::path& lib_dir, const Version& version);

    void set_assets(const AssetIndex& assets);
    void queue_download(const std::optional<DownloadSpec>& dl, const std::filesystem::path& base_dir);
    // Fetches (16 at a time) each queued file that is missing or whose sha1 is not the hash; the first failure is thrown once all have run.
    void perform_download(const Listener& listener);

    std::int64_t total_size = 0;
    // What the constructor could not do and carried on (a server zip not yet downloaded to unpack, an asset index that could not be fetched).
    std::vector<std::string> messages;

private:
    struct Queued {
        DownloadSpec download;
        std::filesystem::path dir;
    };
    std::filesystem::path game_dir_;
    std::vector<Queued> queue_;
    std::optional<AssetIndex> assets_;
    http::Options http_;
};

// Util.getSHA1: the file's sha1 as lowercase hex.
std::string get_sha1(const std::filesystem::path& file);

// JSONObject.write(writer, indent_factor, 0) of org.json: the text RetroMCP writes conf/version.json as (keys in java.util.HashMap order,
// an object or array of one entry on one line).
std::string org_json_text(const Json& json, int indent_factor = 1);

}  // namespace cb::java::minecraft::prep
