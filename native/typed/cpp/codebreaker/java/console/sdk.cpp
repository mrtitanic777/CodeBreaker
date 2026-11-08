// codebreaker/java/console/sdk.cpp -- C++17 port of codebreaker.java.console.sdk (see the header).

#include "codebreaker/java/console/sdk.hpp"

namespace codebreaker::java::console::sdk {

namespace {
py::str S(const char* s) { return py::str(s); }
py::list<py::str> L(std::initializer_list<const char*> xs) {
    py::list<py::str> out;
    for (const char* x : xs) out.append(py::str(x));
    return out;
}
}  // namespace

// Per-platform: the environment variable(s) that point at the SDK root, the standard install paths to
// probe, the include subdir, and header files that must exist for the root to count as a real SDK.
const py::dict<py::str, SdkSpec> PLATFORM_SDKS = {
    {S("xbox360"),
     SdkSpec{S("Xbox 360 SDK (XDK)"),
             L({"XEDK"}),
             L({"C:\\Program Files (x86)\\Microsoft Xbox 360 SDK", "C:\\Program Files\\Microsoft Xbox 360 SDK", "C:\\XDK"}),
             py::list<py::str>{py::os::path::join(S("include"), S("xbox")), S("include")},
             L({"xtl.h"}),
             S("XEDK"),
             S("xbox360_sdk")}},
    {S("ps3"),
     SdkSpec{S("PlayStation 3 SDK (Cell)"),
             L({"SCE_PS3_ROOT", "CELL_SDK"}),
             L({"C:\\usr\\local\\cell", "C:\\ProgramData\\SCE\\PS3"}),
             py::list<py::str>{py::os::path::join(S("target"), S("ppu"), S("include")), S("include")},
             L({"cell", "sys"}),   // directories/headers present in the Cell SDK
             S("CELL_SDK"),
             std::nullopt}},
    {S("wii"),
     SdkSpec{S("Wii SDK (Revolution)"),
             L({"REVOLUTION_SDK_ROOT", "RVL_ROOT"}),
             L({"C:\\projects\\RVL", "C:\\RVL_SDK"}),
             L({"include"}),
             L({"revolution.h"}),
             S("REVOLUTION_SDK_ROOT"),
             std::nullopt}},
};

bool is_ungated(const std::optional<py::str>& platform) {
    if (!platform) return true;
    return *platform == S("none") || *platform == S("pc_windows") || *platform == S("pc") || *platform == S("windows");
}

SdkInfo::SdkInfo(const std::optional<py::str>& platform_, const py::str& name_, const py::str& root_,
                 const py::list<py::str>& include_dirs_, const py::str& source_)
    : platform(platform_), name(name_), root(root_), include_dirs(include_dirs_), source(source_) {}

py::str SdkInfo::repr() const {
    return py::percent("SdkInfo(%s @ %s via %s)", platform ? *platform : S("None"), root, source);
}

const py::str& SdkInfo::_all_header_text() {
    if (_symbol_cache) return *_symbol_cache;
    py::list<py::str> blob;
    for (const py::str& d : include_dirs) {
        if (!py::os::path::isdir(d)) continue;
        for (const auto& walked : py::os::walk(d)) {
            const py::str& dp = std::get<0>(walked);
            const py::list<py::str>& files = std::get<2>(walked);
            for (const py::str& f : files) {
                if (f.lower().endswith(std::make_tuple(S(".h"), S(".hpp"), S(".hxx")))) {
                    try {
                        blob.append(py::open(py::os::path::join(dp, f), S("r"), S("utf-8"), S("replace")).read_text());
                    } catch (const py::OSError&) {   // (IOError is OSError)
                    }
                }
            }
        }
    }
    _symbol_cache = py::join(S("\n"), blob);
    return *_symbol_cache;
}

bool SdkInfo::has_symbol(const py::str& symbol) {
    return py::re::search(py::percent("\\b%s\\b", py::re::escape(symbol)), _all_header_text()).has_value();
}

bool requires_sdk(const std::optional<py::str>& platform) {
    return !is_ungated(platform) && PLATFORM_SDKS.contains(*platform);
}

std::optional<py::list<py::str>> _valid_root(const SdkSpec& spec, const std::optional<py::str>& root) {
    if (!root || root->empty() || !py::os::path::isdir(*root)) return std::nullopt;
    py::list<py::str> inc_dirs;
    for (const py::str& s : spec.include_subdirs) {
        py::str d = py::os::path::join(*root, s);
        if (py::os::path::isdir(d)) inc_dirs.append(d);
    }
    if (inc_dirs.empty()) {
        // some SDKs point env straight at the include dir
        if (py::os::path::isdir(*root)) inc_dirs = py::list<py::str>{*root};
    }
    for (const py::str& probe : spec.probe_headers) {
        py::list<py::str> where = inc_dirs.copy();   // (inc_dirs + [root]: a new list)
        where.append(*root);
        for (const py::str& d : where) {
            if (py::os::path::exists(py::os::path::join(d, probe))) {
                return !inc_dirs.empty() ? inc_dirs : py::list<py::str>{*root};
            }
        }
    }
    return std::nullopt;
}

std::optional<SdkInfo> detect(const std::optional<py::str>& platform, const std::optional<py::str>& override_) {
    if (is_ungated(platform) || !PLATFORM_SDKS.contains(*platform)) {
        return SdkInfo(platform, S("no SDK required"), S(""), py::list<py::str>{}, S("ungated"));
    }
    const SdkSpec& spec = PLATFORM_SDKS.at(*platform);
    // 1. explicit override
    if (override_ && !override_->empty()) {
        auto inc = _valid_root(spec, override_);
        if (inc) return SdkInfo(platform, spec.name, *override_, *inc, S("override"));
        return std::nullopt;
    }
    // 1b. settings.json
    if (spec.setting) {
        py::str root = py::settings::get_str(*spec.setting);
        std::optional<py::list<py::str>> inc;
        if (!root.empty()) inc = _valid_root(spec, root);
        if (inc) return SdkInfo(platform, spec.name, root, *inc, py::percent("setting:%s", *spec.setting));
    }
    // 2. environment variables
    for (const py::str& ev : spec.env) {
        std::optional<py::str> root = py::os::environ_::get(ev);
        std::optional<py::list<py::str>> inc;
        if (root && !root->empty()) inc = _valid_root(spec, root);
        if (inc) return SdkInfo(platform, spec.name, *root, *inc, py::percent("env:%s", ev));
    }
    // 3. standard install paths
    for (const py::str& root : spec.default_paths) {
        auto inc = _valid_root(spec, root);
        if (inc) return SdkInfo(platform, spec.name, root, *inc, S("path"));
    }
    return std::nullopt;
}

SdkInfo require(const std::optional<py::str>& platform, const std::optional<py::str>& override_) {
    std::optional<SdkInfo> info = detect(platform, override_);
    if (info) return *info;
    const SdkSpec& spec = PLATFORM_SDKS.at(*platform);
    py::list<py::str> envl;
    for (const py::str& e : spec.env) envl.append(py::percent("%%%s%%", e));
    py::str envs = py::join(S(" or "), envl);
    throw SdkNotFound(py::percent(
        "No %s found -- cannot convert/emit for platform '%s'.\n"
        "  The converter never ships SDK headers or signatures; it pulls them from your installed SDK.\n"
        "  Provide it one of these ways:\n"
        "    * set the environment variable %s to the SDK root, or\n"
        "    * pass --sdk <path-to-sdk-root>, or\n"
        "    * install it to one of: %s\n"
        "  (Ungated targets still work: use --platform none for a portable conversion with no %s code.)",
        spec.name, *platform, envs, py::join(S(", "), spec.default_paths), spec.name));
}

py::str describe(const std::optional<py::str>& platform, const std::optional<py::str>& override_) {
    if (is_ungated(platform)) {
        return py::percent("platform '%s': no SDK required (ungated).", platform ? *platform : S("None"));
    }
    if (!PLATFORM_SDKS.contains(*platform)) {
        return py::percent("platform '%s': unknown.", *platform);
    }
    std::optional<SdkInfo> info = detect(platform, override_);
    if (!info) {
        const SdkSpec& spec = PLATFORM_SDKS.at(*platform);
        py::list<py::str> envl;
        for (const py::str& e : spec.env) envl.append(py::percent("%%%s%%", e));
        return py::percent("platform '%s': %s NOT found (checked %s + %s). Set %s or pass --sdk.",
                           *platform, spec.name, py::join(S(", "), envl), py::join(S(", "), spec.default_paths),
                           py::join(S(" / "), spec.env));
    }
    return py::percent("platform '%s': %s found at %s (via %s).", *platform, info->name, info->root, info->source);
}

}  // namespace codebreaker::java::console::sdk
