#include "cb/java/console/xdk_scan.hpp"

#include <algorithm>
#include <system_error>

#include "cb/core/fs.hpp"
#include "cb/java/console/tree.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::console::xdk_scan {

namespace stdfs = std::filesystem;

const std::vector<Pattern>& patterns() {
    using enum Kind;
    static const std::vector<Pattern> kPatterns = {
        {"XContentCreate", "XContent", Requirement, "save", Severity::Correctness, std::nullopt,
         "World saves must go through an STFS content package (not a raw file write)",
         "This SDK exports the XContent* content-package family -- the durable per-profile storage mechanism on "
         "the 360. CreateFile/WriteFile to a plain path is NOT committed to the device until the package is "
         "flushed (XContentFlush) after a save batch."},
        {"XShowDeviceSelectorUI", "XShowDeviceSelectorUI", Requirement, "save", Severity::Cert, std::nullopt,
         "Storage device must be selected before saving",
         "The SDK provides the device-selector UI; the chosen XCONTENTDEVICEID scopes XContentCreate. Cert "
         "requires picking a device and handling no-device / not-enough-space."},
        {"XNotifyGetNext", "XNotify", Requirement, "system", Severity::Cert, std::nullopt,
         "Pause/yield when the Guide overlay opens; handle sign-in & controller changes",
         "The SDK exposes the notification family (XNotifyGetNext). A title that keeps simulating/playing audio "
         "under the Guide overlay is a cert failure; also handle XN_SYS_SIGNINCHANGED and controller unplug."},
        {"XNetStartup", "XNet", Requirement, "network", Severity::Compat, std::nullopt,
         "Talking to a PC/Java server needs XNet security bypassed",
         "XNetStartup(XNET_STARTUP_BYPASS_SECURITY) + insecure setsockopt -- otherwise XNet encrypts all traffic "
         "and refuses non-Xbox peers, so a standard Minecraft (PC) server sees garbage."},
        {"XPhysicalAlloc", "XPhysical", Requirement, "video", Severity::Correctness, std::nullopt,
         "GPU resources should come from physical memory",
         "The SDK exposes XPhysicalAlloc/XPhysicalFree. The GPU reads physical (not paged) memory; allocating "
         "textures/vertex buffers from the general heap is the wrong path and feeds heap fragmentation."},

        {"XShowKeyboardUI", "XShowKeyboardUI", Equivalent, "input", std::nullopt, std::nullopt,
         "On-screen keyboard for text input",
         "XShowKeyboardUI (async) -- the OSK for server address / chat, since there is no PC keyboard."},

        {"XInputSetState", "XInputSetState", Opportunity, "input", std::nullopt, Effort::Small,
         "Controller rumble", "XInputSetState(XINPUT_VIBRATION) -- haptics on damage/explosions/mining."},
        {"XMPGetStatus", "XMP", Opportunity, "audio", std::nullopt, Effort::Medium, "Custom soundtracks",
         "The XMP family lets the player's own dashboard music play over the game."},
        {"XMemCompress", "XMemCompress", Opportunity, "save", std::nullopt, Effort::Medium, "LZX save compression",
         "XMemCompress/XMemDecompress (LZX) for our own saves -- smaller/faster than zlib. NOTE: MP server chunks "
         "stay zlib/puff; LZX is a different format."},
        {"XShowMessageBoxUI", "XShowMessageBoxUI", Opportunity, "system", std::nullopt, Effort::Small,
         "Native system dialogs", "XShowMessageBoxUI -- connect-failed/kicked prompts as the real 360 box."},
        {"XGetLanguage", "XGetLanguage", Opportunity, "i18n", std::nullopt, Effort::Small, "Console-language i18n",
         "XGetLanguage -> load the matching .lang instead of hardcoded en_US."},
        {"XGetVideoMode", "XGetVideoMode", Opportunity, "video", std::nullopt, Effort::Small,
         "Native display resolution", "XGetVideoMode -> size the render target to 480p/720p/1080p + widescreen."},
        {"XJSONCreateReader", "XJSON", Opportunity, "io", std::nullopt, Effort::Small, "Native JSON parser",
         "The XJSON family replaces the argo-JSON stub cluster (StringReader/J_*)."},
    };
    return kPatterns;
}

namespace {

bool is_word(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }
bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

// How many leading characters of the identifier make it an XDK entry-point name: XamX, XX, D3DX, MmX, KeX, NtX,
// VdX (X an upper-case letter); 0 when it starts with none of them.
std::size_t prefix_length(std::string_view id) {
    auto starts = [&](std::string_view p) { return id.starts_with(p); };
    auto upper_at = [&](std::size_t i) { return i < id.size() && is_upper(id[i]); };
    if (starts("Xam") && upper_at(3)) return 4;
    if (starts("X") && upper_at(1)) return 2;
    if (starts("D3DX")) return 4;
    if ((starts("Mm") || starts("Ke") || starts("Nt") || starts("Vd")) && upper_at(2)) return 3;
    return 0;
}

}  // namespace

// A name counts when it is a whole identifier that starts with one of the entry-point prefixes, has at least one
// more character after the prefix, and is followed (after any white space) by an opening parenthesis.
std::vector<std::string_view> entry_points_in(std::string_view text) {
    std::vector<std::string_view> out;
    std::size_t i = 0;
    while (i < text.size()) {
        if (!is_word(text[i])) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < text.size() && is_word(text[j])) ++j;
        const std::string_view id = text.substr(i, j - i);
        const std::size_t prefix = prefix_length(id);
        if (prefix != 0 && id.size() > prefix) {
            std::size_t k = j;
            while (k < text.size() && is_space(text[k])) ++k;
            if (k < text.size() && text[k] == '(') out.push_back(id);
        }
        i = j;
    }
    return out;
}

std::map<std::string, std::string> scan_functions(const stdfs::path& include_dir) {
    std::map<std::string, std::string> funcs;
    for (const stdfs::path& file : tree::files_below(include_dir)) {
        if (file.extension() != ".h") continue;   // case-sensitive, as the SDK's headers are written
        const std::optional<std::string> text = textutil::try_read_text(file);
        if (!text) continue;   // an unreadable header contributes nothing
        const std::string name = fs::to_text(file.filename());
        for (std::string_view fn : entry_points_in(*text)) funcs.emplace(std::string(fn), name);   // first header wins
    }
    return funcs;
}

std::optional<stdfs::path> sdk_include_dir(const stdfs::path& sdk_root) {
    if (sdk_root.empty()) return std::nullopt;
    std::error_code ec;
    for (const stdfs::path& cand : {sdk_root / "include" / "xbox", sdk_root / "include", sdk_root})
        if (stdfs::is_directory(cand, ec)) return cand;
    return std::nullopt;
}

ScanResult detect(const std::optional<stdfs::path>& sdk_root) {
    ScanResult result;
    if (sdk_root) result.include = sdk_include_dir(*sdk_root);
    if (result.include) result.funcs = scan_functions(*result.include);
    for (const Pattern& p : patterns()) {
        Detection d;
        static_cast<Pattern&>(d) = p;
        d.present = result.funcs.contains(p.needs);
        // the family's functions, sorted (the map already is) -- every name that starts with the family prefix
        std::vector<std::string> family;
        for (auto it = result.funcs.lower_bound(p.family); it != result.funcs.end() && it->first.starts_with(p.family); ++it)
            family.push_back(it->first);
        d.func_count = family.size();
        if (family.size() > kMaxListed) family.resize(kMaxListed);
        d.functions = std::move(family);
        if (const auto at = result.funcs.find(p.needs); at != result.funcs.end()) d.header = at->second;
        (d.present ? result.found : result.missing).push_back(std::move(d));
    }
    return result;
}

}  // namespace cb::java::console::xdk_scan
