// codebreaker/formats/xbox/system_ui.hpp -- C++17 port of codebreaker.formats.xbox.system_ui.
//
// system_ui.py -- the system UI's assets the Xbox 360 runtime draws the Guide and its popups from,
// taken from the owner's own console system update.
//
//     python -m codebreaker system-ui <system update package> <out folder> [<online modules folder>]
//
// A system update (su20076000_00000000, dashboard 2.0.17559) is an STFS package of the console's
// flash files; its system modules carry XUI packages (XUIZ) as XEX resources. Written: every XUI
// package of the modules the Guide uses, as <module>.<resource>.xzp, and the system fonts -- the
// folder native/x360's xui::Assets loads (system/xui/xui/assets.h).
//
// <online modules folder> (optional): the Guide's LIVE-delivered modules as a console keeps them in
// its SystemAux partition (online/<version>/Guide.Friends.xex ...; no system update carries them);
// their XUI packages are written too. Modules missing there are skipped.
//
// These are Microsoft's files: the output folder is the owner's and is never committed or distributed.
//
// Typing notes:
//   * `report=print` is a callable taking one str: std::function<void(const py::str&)>; extract() without
//     it (an empty Report) prints.
//   * The STFS package is the typed formats::xbox::stfs::STFS; the .xex program is the typed
//     formats::xex::program().
//   * FONTS is a dict in the Python ({package file: written name}); here the list of its (key, value)
//     pairs in the same order (it is only ever iterated).
//   * main() takes sys.argv[1:] and returns the exit status (the Python's `sys.exit(main(sys.argv[1:]))`).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::system_ui {

// the module's docstring (the usage text main() prints is its first two paragraphs)
extern const py::str DOC;

extern const py::list<py::str> MODULES;
extern const py::list<py::str> ONLINE_MODULES;
// system font file in the package -> the name it is written under (in the Python's order)
extern const py::list<std::tuple<py::str, py::str>> FONTS;

// {resource name: bytes} of the XUI packages a module's XEX carries.
py::dict<py::str, py::bytes> _xui_packages(const py::bytes& module_bytes);

using Report = std::function<void(const py::str&)>;

// Writes the XUI packages and fonts into out_dir; -> the number of files written.
int64_t extract(const py::str& package, const py::str& out_dir, const std::optional<py::str>& online = std::nullopt,
                const Report& report = Report());

// `python -m codebreaker system-ui ...`; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::system_ui
