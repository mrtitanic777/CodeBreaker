// cb/java/console/sdk.hpp -- platform SDK detection and gatekeeping.
//
// A console platform's SDK (Xbox 360 XDK, PS3 SDK, Wii RVL SDK) is proprietary. The converter must NOT ship or
// redistribute those headers/signatures, and must NOT emit platform-specific code for a target the user has no SDK
// for. So this module:
//
//   * DETECTS the SDK on the user's own machine (an explicit path, the configured one, an environment variable,
//     then the standard install paths).
//   * GATEKEEPS: converting/emitting for a console platform is refused unless its SDK is located. PC is ungated;
//     the platform "none" (portable only) is always allowed.
//   * VERIFIES symbols against the located SDK headers, so the code we emit calls only APIs that actually exist in
//     THAT installed SDK -- the truth comes from the user's SDK, not from a baked-in signature list. (The real
//     declarations come from the user's own <xtl.h> at compile time; the tool ships none of them.)
//
// require(platform) is the one call the pipeline uses to gate: an SdkInfo, or SdkNotFoundError with instructions.
// Nothing is read from the program's settings here: the caller passes the configured SDK folder in SdkHints.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/error.hpp"

namespace cb::java::console::sdk {

// No SDK was found for a platform that needs one; the message says how to provide it.
class SdkNotFoundError : public Error {
public:
    explicit SdkNotFoundError(const std::string& message) : Error(message) {}
    int exit_code() const override { return kExitNotFound; }
    CB_ERROR_BOILERPLATE(SdkNotFoundError)
};

// The platforms that have an SDK gate.
enum class Console { Xbox360, Ps3, Wii };

// The console a platform name ("xbox360", "ps3", "wii") means; empty for every other name, including the ungated
// ones (none, pc, pc_windows, windows).
std::optional<Console> console_of(std::string_view platform);

// The platform name of a console.
std::string_view platform_name(Console c);

// What identifies one console's SDK on disk.
struct SdkSpec {
    std::string name;                                        // "Xbox 360 SDK (XDK)"
    std::vector<std::string> env_vars;                       // environment variables that point at the SDK root
    std::vector<std::filesystem::path> default_paths;        // standard install locations to probe
    std::vector<std::filesystem::path> include_subdirs;      // where the headers are, below the root
    std::vector<std::string> probe_headers;                  // files or folders that must exist for a root to count
    std::string setting_key;                                 // the settings.json key naming the SDK ("" when none)
};

const SdkSpec& spec_of(Console c);

// Where the caller says to look before the environment and the standard paths.
struct SdkHints {
    // An explicit root (a command-line --sdk): when given it is the only place looked at.
    std::optional<std::filesystem::path> override_root;
    // The root the settings name for this console (SdkSpec::setting_key); empty when unset.
    std::optional<std::filesystem::path> configured_root;
};

// A located SDK. `has_symbol` reads the headers once, on first use, and keeps them; an SdkInfo is therefore used
// from one thread at a time.
class SdkInfo {
public:
    // How the SDK was found.
    enum class Source { Ungated, Override, Setting, Environment, DefaultPath };

    SdkInfo(std::string platform, std::string name, std::filesystem::path root,
            std::vector<std::filesystem::path> include_dirs, Source source, std::string source_detail = {});

    const std::string& platform() const { return platform_; }
    const std::string& name() const { return name_; }
    const std::filesystem::path& root() const { return root_; }
    // The include folders that exist, to search for symbols.
    const std::vector<std::filesystem::path>& include_dirs() const { return include_dirs_; }
    Source source() const { return source_; }
    // The source as reports print it: "ungated", "override", "setting:xbox360_sdk", "env:XEDK" or "path".
    std::string source_text() const;

    // True if `symbol` is declared (appears as a whole identifier) anywhere in the located SDK's headers (.h, .hpp,
    // .hxx). Lets the emitter verify an API exists in THIS install before emitting a call to it. False for an
    // empty symbol.
    bool has_symbol(std::string_view symbol);

private:
    const std::string& all_header_text();

    std::string platform_;
    std::string name_;
    std::filesystem::path root_;
    std::vector<std::filesystem::path> include_dirs_;
    Source source_;
    std::string source_detail_;      // the setting key or environment variable of a Setting/Environment source
    std::optional<std::string> header_text_;
};

// Does the platform need an SDK (is it a console with a gate)?
bool requires_sdk(std::string_view platform);

// The include folders of `root` if it looks like a real SDK for `console` (it has one of the probe headers in an
// include folder or in the root itself); nullopt otherwise.
std::optional<std::vector<std::filesystem::path>> valid_root(const SdkSpec& spec, const std::filesystem::path& root);

// Locate the SDK for `platform`. Order: the explicit override (and nothing else when one is given), the configured
// root, the environment variables, the default install paths. A platform that needs no SDK gets a trivial SdkInfo
// (source Ungated). nullopt when the SDK is not found.
std::optional<SdkInfo> detect(std::string_view platform, const SdkHints& hints = {});

// The SdkInfo for `platform`, or SdkNotFoundError with actionable guidance. This is the gate: no console SDK, no
// platform-specific conversion or emission.
SdkInfo require(std::string_view platform, const SdkHints& hints = {});

// A one-line account of the SDK's status for `platform` (for `codebreaker java sdk detect`).
std::string describe(std::string_view platform, const SdkHints& hints = {});

}  // namespace cb::java::console::sdk
