// java/projects/minecraft/prep/setup.hpp -- RetroMCP's setup: prepare a workspace folder for a Minecraft version.
//
//     auto made = prep::setup("1.5.2", "work/mc/1.5.2");
//     // work/mc/1.5.2/jars/minecraft.jar, minecraft_server.jar; libraries/<maven layout>, libraries/natives/; conf/ (mappings.tiny,
//     // exceptions.exc, <side>.access, <side>.patch where the version has them, version.json); jars/assets/
//
// The steps are TaskSetup's: clear the earlier setup (TaskCleanup.cleanup), look the version up in the versions list, read its version JSON, unpack its
// conf zip into conf/, download the jars, the libraries for this operating system and the assets, unpack the natives, write conf/version.json.
//
// Ported from RetroMCP (org.mcphackers.mcp.tasks.TaskSetup, TaskCleanup.cleanup; tools.FileUtil.extract), MIT License, (c) MCPHackers.
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "java/projects/minecraft/prep/versions.hpp"

namespace cb::java::minecraft::prep {

struct SetupOptions {
    Side side = Side::Any;                                  // which jars to fetch (RetroMCP's "side"; ANY: both)
    std::string versions_url = VersionParser::DEFAULT_JSON;  // the list of versions: an http(s) URL or a file
    http::Options http;
    // Asked when the version is not in the list (TaskSetup asks the user until it is); returns another id, or nothing to give up.
    std::function<std::optional<std::string>(const std::string& unknown_id)> ask_version;
    // (stage text, percent 0..100): TaskSetup's setProgress.
    std::function<void(const std::string&, int)> progress;
};

struct SetupResult {
    Version version;                                        // the version JSON that was written to conf/version.json
    VersionData version_data;
    std::filesystem::path workspace;
    std::filesystem::path conf_dir, natives_dir, libraries_dir, jars_dir, version_json;
    std::optional<std::filesystem::path> client_jar, server_jar;   // as downloaded (a server given as a .zip stays jars/minecraft_server.zip: server_jar is empty)
    std::vector<std::filesystem::path> libraries;           // the libraries' jars, as RetroMCP's classpath
    std::vector<std::filesystem::path> native_archives;     // unpacked into natives_dir
    std::vector<std::string> messages;                      // what was skipped or went wrong without stopping the setup
};

// TaskCleanup.cleanup: removes an earlier setup from the workspace (each side's jar, project, patches and build; conf/; libraries/natives; the empty jars/).
// Returns whether anything was removed.
bool cleanup(const std::filesystem::path& workspace);

// FileUtil.extract: unpack a zip into a folder (each file replaced if it exists; the folder tree made). `extension` limits it to entries ending so
// (FileUtil.extractByExtension). FormatError for a damaged zip; a name that would leave the folder is a FormatError (RetroMCP unpacks it anyway).
void extract_zip(const std::filesystem::path& zip, const std::filesystem::path& dest, const std::optional<std::string>& extension = std::nullopt);
void extract_zip_bytes(ByteView zip, const std::filesystem::path& dest);

// TaskSetup. Throws cb::UsageError for an unknown version nobody names (ask_version), cb::IoError / http::HttpError for what could not be read or fetched.
SetupResult setup(const std::string& version_id, const std::filesystem::path& workspace, const SetupOptions& options = {});

}  // namespace cb::java::minecraft::prep
