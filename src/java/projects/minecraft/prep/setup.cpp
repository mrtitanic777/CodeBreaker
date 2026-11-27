// java/projects/minecraft/prep/setup.cpp -- see setup.hpp.
// Ported from RetroMCP (org.mcphackers.mcp.tasks.TaskSetup, TaskCleanup.cleanup; tools.FileUtil.extract), MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include "java/projects/minecraft/prep/setup.hpp"

#include <algorithm>
#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "formats/zip.hpp"
#include "java/projects/minecraft/prep/versions.hpp"

namespace cb::java::minecraft::prep {

namespace stdfs = std::filesystem;

bool cleanup(const stdfs::path& workspace) {
    bool deleted = false;
    std::vector<stdfs::path> files_to_delete;
    for (const Side side : kAllSides) {
        files_to_delete.push_back(mcp_paths::get(workspace, mcp_paths::JAR_ORIGINAL, side));
        files_to_delete.push_back(mcp_paths::get(workspace, mcp_paths::PROJECT, side));
        files_to_delete.push_back(mcp_paths::get(workspace, mcp_paths::PATCHES, side));
        files_to_delete.push_back(mcp_paths::get(workspace, mcp_paths::BUILD_ZIP, side));
        files_to_delete.push_back(mcp_paths::get(workspace, mcp_paths::BUILD_JAR, side));
    }
    files_to_delete.push_back(mcp_paths::get(workspace, mcp_paths::CONF));
    files_to_delete.push_back(mcp_paths::get(workspace, mcp_paths::NATIVES));

    for (const stdfs::path& path : files_to_delete) {
        if (stdfs::exists(path)) {
            deleted = true;
            fs::remove_tree(path);   // FileUtil.delete: a file, or a folder with everything in it
        }
    }
    const stdfs::path jars = mcp_paths::get(workspace, mcp_paths::JARS);
    if (stdfs::exists(jars) && stdfs::is_empty(jars)) {
        deleted = true;
        stdfs::remove(jars);
    }
    return deleted;
}

namespace {

void extract_entries(const formats::ZipArchive& zip, const stdfs::path& dest, const std::optional<std::string>& extension) {
    stdfs::create_directories(dest);
    const stdfs::path root = dest.lexically_normal();
    for (const formats::ZipArchive::Entry& entry : zip.entries()) {
        if (extension && !(entry.name.size() >= extension->size() && entry.name.compare(entry.name.size() - extension->size(), extension->size(), *extension) == 0))
            continue;
        const stdfs::path to = (dest / fs::from_text(entry.name)).lexically_normal();
        const stdfs::path inside = to.lexically_relative(root);
        if (inside.empty() || *inside.begin() == "..")
            throw FormatError("extract_zip", 0, "the entry \"" + entry.name + "\" would be written outside " + fs::to_text(dest));
        if (entry.is_directory()) {
            stdfs::create_directories(to);
        } else {
            std::error_code ec;
            stdfs::remove(to, ec);
            fs::write_file(to, zip.read(entry));   // (the parent folders are made; the entry's CRC is checked)
        }
    }
}

}  // namespace

void extract_zip(const stdfs::path& zip, const stdfs::path& dest, const std::optional<std::string>& extension) {
    extract_entries(formats::ZipArchive::open(zip), dest, extension);
}

void extract_zip_bytes(ByteView zip, const stdfs::path& dest) {
    extract_entries(formats::ZipArchive::from_bytes(Bytes(zip.begin(), zip.end()), "conf zip"), dest, std::nullopt);
}

SetupResult setup(const std::string& version_id, const stdfs::path& workspace, const SetupOptions& options) {
    auto progress = [&](const std::string& stage, int percent) {
        if (options.progress) options.progress(stage, percent);
    };
    SetupResult made;
    made.workspace = workspace;
    made.jars_dir = mcp_paths::get(workspace, mcp_paths::JARS);
    made.libraries_dir = mcp_paths::get(workspace, mcp_paths::LIB);
    made.natives_dir = mcp_paths::get(workspace, mcp_paths::NATIVES);
    made.conf_dir = mcp_paths::get(workspace, mcp_paths::CONF);
    made.version_json = mcp_paths::get(workspace, mcp_paths::VERSION);

    cleanup(workspace);
    stdfs::create_directories(made.jars_dir);
    stdfs::create_directories(made.libraries_dir);
    stdfs::create_directories(made.natives_dir);

    progress("setup", 1);
    const VersionParser parser(options.versions_url);
    std::string chosen = version_id;
    const VersionData* chosen_data = nullptr;
    // Keep asking until the chosen version is one of the versions
    for (;;) {
        chosen_data = parser.get_version(chosen);
        if (chosen_data != nullptr) break;
        std::optional<std::string> another;
        if (options.ask_version) another = options.ask_version(chosen);
        if (!another)
            throw UsageError("no version \"" + chosen + "\" in " + options.versions_url + (parser.failure_cause ? " (the list could not be read: " + *parser.failure_cause + ")" : std::string()));
        chosen = *another;
    }
    made.version_data = *chosen_data;
    const VersionData& data = made.version_data;

    std::string version_text;
    if (http::is_http_url(data.url)) {
        const Bytes body = http::get(data.url, options.http);
        version_text = std::string(as_text(body));
    } else {
        version_text = fs::read_text(mcp_paths::get(workspace, data.url));
    }
    const Json version_json = json::parse(version_text);
    made.version = Version::from(version_json);
    stdfs::create_directories(made.conf_dir);

    if (data.resources) {
        progress("download " + *data.resources, 2);
        if (http::is_http_url(*data.resources)) {
            extract_zip_bytes(http::get(*data.resources, options.http), made.conf_dir);
        } else {
            const stdfs::path p = fs::from_text(*data.resources);
            if (stdfs::exists(p)) extract_zip(p, made.conf_dir);
        }
    }

    DownloadData dl(workspace, options.side, made.version, options.http);
    std::int64_t libs_size = 0;
    dl.perform_download([&](const DownloadSpec& done, std::int64_t total_size) {
        libs_size += done.size;
        const int percent = total_size > 0 ? static_cast<int>(static_cast<double>(libs_size) / static_cast<double>(total_size) * 97.0) : 0;
        progress("download " + done.url, 3 + percent);
    });
    made.messages = dl.messages;

    made.native_archives = DownloadData::get_natives(made.libraries_dir, made.version);
    for (const stdfs::path& archive : made.native_archives) extract_zip(archive, made.natives_dir);
    made.libraries = DownloadData::get_libraries(made.libraries_dir, made.version);

    fs::write_text(made.version_json, org_json_text(version_json, 1));

    const stdfs::path client = mcp_paths::get(workspace, mcp_paths::JAR_ORIGINAL, Side::Client);
    const stdfs::path server = mcp_paths::get(workspace, mcp_paths::JAR_ORIGINAL, Side::Server);
    if (stdfs::exists(client)) made.client_jar = client;
    if (stdfs::exists(server)) made.server_jar = server;
    return made;
}

}  // namespace cb::java::minecraft::prep
