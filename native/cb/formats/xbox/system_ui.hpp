// cb/formats/xbox/system_ui.hpp -- the system UI's assets the Xbox 360 runtime draws the Guide and its popups from, taken from the owner's
// own console system update.
//
//     system-ui <system update package> <out folder> [<online modules folder>]
//
// A system update (su20076000_00000000, dashboard 2.0.17559) is an STFS package of the console's flash files; its system modules carry XUI
// packages (XUIZ) as XEX resources. Written: every XUI package of the modules the Guide uses, as <module>.<resource>.xzp, and the system
// fonts -- the folder native/x360's xui::Assets loads (system/xui/xui/assets.h).
//
// <online modules folder> (optional): the Guide's LIVE-delivered modules as a console keeps them in its SystemAux partition
// (online/<version>/Guide.Friends.xex ...; no system update carries them); their XUI packages are written too. Modules missing there are
// skipped.
//
// These are Microsoft's files: the output folder is the owner's and is never committed or distributed.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xex.hpp"

namespace cb::formats::xbox::system_ui {

// The modules of a system update whose XUI packages are taken, and the Guide's online ones.
inline constexpr std::string_view kModules[] = {"xam", "huduiskin", "hud", "signin", "vk", "gamerprofile", "createprofile", "deviceselector"};
inline constexpr std::string_view kOnlineModules[] = {"Guide.Friends", "Guide.ChatAndMessenger", "Guide.Voicemail"};

// A system font: its file in the update ($flash_<source>) and the name it is written under.
struct Font {
    std::string_view source, target;
};
inline constexpr Font kFonts[] = {{"SegoeXbox-Light.xtt", "SegoeXbox-Light.xtt"}, {"xenonclatin.xttp", "xenonclatin.xtt"}, {"xenonjklatin.xttp", "xenonjklatin.xtt"}};

// Each line `extract` reports ("<file name, 40 wide> <size, 8 wide>" or "<module>.xex (not in <folder>)").
using Report = std::function<void(const std::string&)>;

// {resource name: bytes} of the XUI packages (XUIZ) a module's XEX carries, read by the xex engine. The module's other resources are not
// wanted; a module the engine cannot read has none.
std::map<std::string, Bytes> xui_packages(ByteView module);

// Writes the assets of the update `package` into `out_dir` (made if need be) and reports each file; returns how many. FormatError when the
// file is not an STFS package, or lacks a module or a font (a system update has them all); the files written before are left.
struct Environment;
std::size_t extract(const Environment& environment, const std::filesystem::path& package, const std::filesystem::path& out_dir,
                    const std::optional<std::filesystem::path>& online, const Report& report);

// What reads a module's XUI packages: xui_packages (the xex engine); a test may hand its own.
using XuiReader = std::function<std::map<std::string, Bytes>(ByteView module)>;
struct Environment {
    XuiReader read_xui = xui_packages;
};

// `system-ui <package> <out folder> [<online folder>]`: prints each file written and "N files -> <out folder>".
cli::Command command(const Environment& environment);

}  // namespace cb::formats::xbox::system_ui
