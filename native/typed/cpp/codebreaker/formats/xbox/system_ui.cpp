// codebreaker/formats/xbox/system_ui.cpp -- C++17 port of codebreaker.formats.xbox.system_ui (see system_ui.hpp).

#include "codebreaker/formats/xbox/system_ui.hpp"

#include "codebreaker/formats/xbox/stfs.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::formats::xbox::system_ui {

namespace stfs = codebreaker::formats::xbox::stfs;
namespace xex = codebreaker::formats::xex;

const py::str DOC = py::str(
    "system_ui.py -- the system UI's assets the Xbox 360 runtime draws the Guide and its popups from,\n"
    "taken from the owner's own console system update.\n"
    "\n"
    "    python -m codebreaker system-ui <system update package> <out folder> [<online modules folder>]\n"
    "\n"
    "A system update (su20076000_00000000, dashboard 2.0.17559) is an STFS package of the console's\n"
    "flash files; its system modules carry XUI packages (XUIZ) as XEX resources. Written: every XUI\n"
    "package of the modules the Guide uses, as <module>.<resource>.xzp, and the system fonts -- the\n"
    "folder native/x360's xui::Assets loads (system/xui/xui/assets.h).\n"
    "\n"
    "<online modules folder> (optional): the Guide's LIVE-delivered modules as a console keeps them in\n"
    "its SystemAux partition (online/<version>/Guide.Friends.xex ...; no system update carries them);\n"
    "their XUI packages are written too. Modules missing there are skipped.\n"
    "\n"
    "These are Microsoft's files: the output folder is the owner's and is never committed or distributed.\n");

const py::list<py::str> MODULES = {py::str("xam"),           py::str("huduiskin"), py::str("hud"),
                                   py::str("signin"),        py::str("vk"),        py::str("gamerprofile"),
                                   py::str("createprofile"), py::str("deviceselector")};
const py::list<py::str> ONLINE_MODULES = {py::str("Guide.Friends"), py::str("Guide.ChatAndMessenger"),
                                          py::str("Guide.Voicemail")};
const py::list<std::tuple<py::str, py::str>> FONTS = {
    std::make_tuple(py::str("SegoeXbox-Light.xtt"), py::str("SegoeXbox-Light.xtt")),
    std::make_tuple(py::str("xenonclatin.xttp"), py::str("xenonclatin.xtt")),
    std::make_tuple(py::str("xenonjklatin.xttp"), py::str("xenonjklatin.xtt"))};

namespace {

// removes the folder when the scope ends (the Python's try/finally with shutil.rmtree(ignore_errors=True))
struct Cleanup {
    py::str folder;
    ~Cleanup() {
        try {
            py::shutil::rmtree(folder, true);
        } catch (...) {
        }
    }
};

void write_file(const py::str& path, const py::bytes& data) {
    py::file f = py::open(path, py::str("wb"));
    f.write(data);
}

// sorted(d.items()): by resource name (the names are distinct)
py::list<std::tuple<py::str, py::bytes>> sorted_items(const py::dict<py::str, py::bytes>& d) {
    py::list<std::tuple<py::str, py::bytes>> items;
    for (const auto& kv : d.items()) {
        items.append(kv);
    }
    items.sort([](const std::tuple<py::str, py::bytes>& kv) { return std::get<0>(kv); });
    return items;
}

}  // namespace

py::dict<py::str, py::bytes> _xui_packages(const py::bytes& module_bytes) {
    py::str work = py::tempfile::mkdtemp(std::nullopt, py::str("cb_sysui_"));
    Cleanup cleanup{work};
    py::str path = py::os::path::join(work, py::str("module.xex"));
    write_file(path, module_bytes);
    py::str out = py::os::path::join(work, py::str("res"));
    py::os::makedirs(out);
    py::subprocess::RunOptions opts;   // capture_output=True, and the output is not read: bytes
    opts.text = false;
    py::subprocess::run(py::list<py::str>{xex::program(), py::str("resources"), path, out}, opts);
    py::dict<py::str, py::bytes> found;
    for (const py::str& name : py::os::listdir(out)) {
        py::bytes blob;
        {
            py::file f = py::open_rb(py::os::path::join(out, name));
            blob = f.read();
        }
        if (blob.slice(0, 4) == py::bytes("XUIZ")) {
            found.set(name, blob);
        }
    }
    return found;
}

int64_t extract(const py::str& package, const py::str& out_dir, const std::optional<py::str>& online, const Report& report_in) {
    Report report = report_in ? report_in : Report([](const py::str& line) { py::print(line); });
    py::os::makedirs(out_dir, 0777, true);
    py::bytes package_bytes;
    {
        py::file f = py::open_rb(package);
        package_bytes = f.read();
    }
    stfs::STFS pkg(package_bytes);
    py::dict<py::str, py::str> files;   // {name.rsplit("/", 1)[-1]: name for name in pkg.files}
    for (const py::str& name : pkg.files) {
        int64_t slash = name.rfind(py::str("/"));
        files.set(slash < 0 ? name : name.slice_from(slash + 1), name);
    }
    int64_t written = 0;
    for (const py::str& module : MODULES) {
        std::optional<py::str> name = files.get(py::percent("$flash_%s.xex", module));
        if (!name || name->empty()) {
            throw py::ValueError(py::percent("%s: no $flash_%s.xex (not a system update?)", package, module).utf8());
        }
        for (const auto& [res, blob] : sorted_items(_xui_packages(pkg.read_file(*name)))) {
            py::str target = py::os::path::join(out_dir, py::percent("%s.%s.xzp", module, res));
            write_file(target, blob);
            report(py::percent("%-40s %8d", py::os::path::basename(target), py::len(blob)));
            written += 1;
        }
    }
    for (const auto& [source, target_name] : FONTS) {
        std::optional<py::str> name = files.get(py::str("$flash_") + source);
        if (!name || name->empty()) {
            throw py::ValueError(py::percent("%s: no $flash_%s", package, source).utf8());
        }
        py::bytes data = pkg.read_file(*name);
        write_file(py::os::path::join(out_dir, target_name), data);
        report(py::percent("%-40s %8d", target_name, py::len(data)));
        written += 1;
    }
    if (online && !online->empty()) {
        py::dict<py::str, py::str> found;
        for (const auto& [root, dirs, names] : py::os::walk(*online)) {
            (void)dirs;
            for (const py::str& n : names) {
                found.setdefault(n.lower(), py::os::path::join(root, n));
            }
        }
        for (const py::str& module : ONLINE_MODULES) {
            std::optional<py::str> path = found.get(module.lower() + py::str(".xex"));
            if (!path || path->empty()) {
                report(py::percent("%-40s (not in %s)", module + py::str(".xex"), *online));
                continue;
            }
            py::bytes module_bytes;
            {
                py::file f = py::open_rb(*path);
                module_bytes = f.read();
            }
            py::dict<py::str, py::bytes> packages = _xui_packages(module_bytes);
            for (const auto& [res, blob] : sorted_items(packages)) {
                py::str target = py::os::path::join(out_dir, py::percent("%s.%s.xzp", module.lower(), res));
                write_file(target, blob);
                report(py::percent("%-40s %8d", py::os::path::basename(target), py::len(blob)));
                written += 1;
            }
        }
    }
    return written;
}

int64_t main(const py::list<py::str>& argv) {
    if (py::len(argv) < 2) {
        py::list<py::str> paragraphs = py::split(DOC, py::str("\n\n"));
        py::print(paragraphs[0] + py::str("\n\n") + paragraphs[1]);
        return 2;
    }
    int64_t n = extract(argv[0], argv[1], py::len(argv) > 2 ? std::optional<py::str>(argv[2]) : std::nullopt);
    py::print(py::percent("%d files -> %s", n, argv[1]));
    return 0;
}

}  // namespace codebreaker::formats::xbox::system_ui
