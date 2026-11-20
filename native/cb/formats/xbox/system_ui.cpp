#include "cb/formats/xbox/system_ui.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/xbox/stfs.hpp"
#include "extract.h"    // formats/xex/src: the xex engine's resource dump

namespace cb::formats::xbox::system_ui {

namespace {

namespace stdfs = std::filesystem;

constexpr std::string_view kXuiMagic = "XUIZ";

std::string describe(const stdfs::path& path, std::string_view what) { return fs::to_text(path) + ": " + std::string(what); }

// The files under `root` by lower-case name, the first one met: a folder's own files before its subfolders.
void collect_online(const stdfs::path& folder, std::map<std::string, stdfs::path>& found) {
    std::vector<stdfs::path> folders;
    for (const auto& entry : stdfs::directory_iterator(folder)) {
        if (entry.is_directory()) folders.push_back(entry.path());
        else found.emplace(text::lower(fs::to_text(entry.path().filename())), entry.path());
    }
    for (const stdfs::path& sub : folders) collect_online(sub, found);
}

void write_asset(const stdfs::path& out_dir, const std::string& name, ByteView data, const Report& report) {
    fs::write_file(out_dir / name, data);
    report(std::format("{:<40} {:>8}", name, data.size()));
}

}  // namespace

std::map<std::string, Bytes> xui_packages(ByteView module) {
    const fs::ScopedTempDir work{"cb-sysui"};
    const stdfs::path resources = work.path() / "res";
    fs::make_dirs(resources);
    // (a module the engine cannot read has no resources: what it says about it is not wanted)
    try {
        ::xex::dump_resources(::xex::XexFile::parse(std::vector<std::uint8_t>(module.begin(), module.end())), fs::to_text(resources));
    } catch (const std::exception&) {
    }
    std::map<std::string, Bytes> found;
    for (const auto& entry : stdfs::directory_iterator(resources)) {
        if (!entry.is_regular_file()) continue;
        Bytes blob = fs::read_file(entry.path());
        if (blob.size() >= kXuiMagic.size() && std::equal(kXuiMagic.begin(), kXuiMagic.end(), blob.begin())) found.emplace(fs::to_text(entry.path().filename()), std::move(blob));
    }
    return found;
}

std::size_t extract(const Environment& environment, const stdfs::path& package, const stdfs::path& out_dir, const std::optional<stdfs::path>& online,
                    const Report& report) {
    fs::make_dirs(out_dir);
    const stfs::Package update = stfs::Package::parse(fs::read_file(package));
    std::map<std::string, std::string> files;   // the file's own name -> its path in the package
    for (const stfs::FileEntry& entry : update.files()) files[entry.path.substr(entry.path.rfind('/') == std::string::npos ? 0 : entry.path.rfind('/') + 1)] = entry.path;
    std::size_t written = 0;
    for (const std::string_view module : kModules) {
        const auto found = files.find(std::format("$flash_{}.xex", module));
        if (found == files.end()) throw FormatError("system_ui", 0, describe(package, std::format("no $flash_{}.xex (not a system update?)", module)));
        for (const auto& [resource, blob] : environment.read_xui(update.read_file(found->second))) {
            write_asset(out_dir, std::format("{}.{}.xzp", module, resource), blob, report);
            ++written;
        }
    }
    for (const Font& font : kFonts) {
        const auto found = files.find("$flash_" + std::string(font.source));
        if (found == files.end()) throw FormatError("system_ui", 0, describe(package, "no $flash_" + std::string(font.source)));
        write_asset(out_dir, std::string(font.target), update.read_file(found->second), report);
        ++written;
    }
    if (online) {
        std::map<std::string, stdfs::path> present;
        collect_online(*online, present);
        for (const std::string_view module : kOnlineModules) {
            const auto found = present.find(text::lower(module) + ".xex");
            if (found == present.end()) {
                report(std::format("{:<40} (not in {})", std::string(module) + ".xex", fs::to_text(*online)));
                continue;
            }
            for (const auto& [resource, blob] : environment.read_xui(fs::read_file(found->second))) {
                write_asset(out_dir, std::format("{}.{}.xzp", text::lower(module), resource), blob, report);
                ++written;
            }
        }
    }
    return written;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "system-ui";
    c.summary = "the Xbox 360 runtime's system UI assets from the owner's system update";
    c.area = "Xbox 360";
    c.params = {cli::Positional{"package", "the system update package (an STFS package of flash files)"},
                cli::Positional{"folder", "where to write the assets"},
                cli::Positional{"online", "the Guide's online modules as a console keeps them (optional)", false}};
    c.handler = [environment](const cli::Args& a) {
        const std::string target = a.required("folder");
        const std::optional<std::string> online = a.value("online");
        const std::size_t count = extract(environment, fs::from_text(a.required("package")), fs::from_text(target),
                                          online ? std::optional<stdfs::path>(fs::from_text(*online)) : std::nullopt, [](const std::string& line) { out("{}\n", line); });
        out("{} files -> {}\n", count, target);
        return kExitOk;
    };
    return c;
}

}  // namespace cb::formats::xbox::system_ui
