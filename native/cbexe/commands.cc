// commands.cc -- codebreaker.exe's commands written in C++ (the codebreaker_native module; the command
// table in codebreaker/__main__.py names it). They are what other projects' tools ran CodeBreaker's
// Python for as a library -- the Dashboard's prepare_game.py, Windows95's place_map.py -- as commands of
// the program, so those tools run codebreaker.exe and nothing else:
//
//     codebreaker xbox-disc files  <image>                 every file: size, a tab, its path on the disc
//     codebreaker xbox-disc read   <image> <path> <out>    one file of the disc (exit 3: the disc has none)
//     codebreaker xbox-disc unpack <image> <folder>        every file, streamed in 8 MiB pieces; a file already
//                                                          there at its full size is kept (an interrupted unpack
//                                                          goes on); PROGRESS <bytes done> <bytes in all> lines
//     codebreaker xex-basic <file.xex> <out.xex>           the unencrypted, basic-compressed form XenonRecomp reads
//     codebreaker xenon-config <basic.xex> <out.toml> [--out-dir D] [--switch-tables F] [--file-path F]
//                                                          the XenonRecomp configuration of a module
//     codebreaker switch-tables <basic.xex> <out.toml>     its switch tables: switches.py's, with XenonAnalyse's
//                                                          for the bctr it leaves unresolved
//     codebreaker corpus-executables                       every corpus executable: its format, a tab, its path
//
// An image is a disc image (.iso) or a Games on Demand package's data folder. The work is the program's
// own (the disc reader is formats.dll's, the rest the translated modules'); these are its entry points.
#include "py_native.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using py::var;

namespace {

void say(const std::string& line) {
    var out = py::import("sys").getattr("stdout");
    out.call("write", {py::str_from(line + "\n")});
    out.call("flush");
}

int usage(const std::string& text) {
    var err = py::import("sys").getattr("stderr");
    err.call("write", {py::str_from("usage: codebreaker " + text + "\n")});
    return 2;
}

fs::path path_of(const var& s) { return fs::u8path(py::str_utf8(s)); }

// a file written whole or not at all: <out>.part, then renamed
void write_whole(const fs::path& out, const std::string& data) {
    if (out.has_parent_path()) fs::create_directories(out.parent_path());
    fs::path part = out;
    part += ".part";
    {
        std::ofstream f(part, std::ios::binary);
        f.write(data.data(), (std::streamsize)data.size());
        if (!f) py::raise(py::T(py::t::OSError), "cannot write " + part.u8string());
    }
    fs::rename(part, out);
}

var open_disc(const var& image) { return py::import("codebreaker.formats.xbox.gdf").getattr("_open")({image}); }

int xbox_disc(const std::vector<var>& a) {
    if (a.size() < 2) return usage("xbox-disc files|read|unpack <image> ...");
    std::string op = py::str_utf8(a[0]);
    var disc = open_disc(a[1]);
    if (op == "files" && a.size() == 2) {
        for (var item : py::each(disc.call("files"))) {
            auto parts = py::unpack(item, 2);
            say(py::str_utf8(py::str(parts[1])) + "\t" + py::str_utf8(parts[0]));
        }
        return 0;
    }
    if (op == "read" && a.size() == 4) {
        var data;
        try {
            data = disc.call("read", {a[2]});
        } catch (py::error& e) {
            if (!e.matches(py::t::KeyError)) throw;
            fprintf(stderr, "%s has no %s\n", py::str_utf8(a[1]).c_str(), py::str_utf8(a[2]).c_str());
            return 3;
        }
        write_whole(path_of(a[3]), py::bytes_raw(data));
        return 0;
    }
    if (op == "unpack" && a.size() == 3) {
        fs::path target = path_of(a[2]);
        var entries = disc.call("_walk_dir", {disc.getattr("root_sector"), disc.getattr("root_size")});
        struct Entry { std::string path; bool dir; int64_t start, size; };
        std::vector<Entry> list;
        int64_t total = 0;
        for (var item : py::each(entries)) {
            auto p = py::unpack(item, 4);
            Entry e{py::str_utf8(p[0]), p[1].truthy(), p[2].as_int64(), p[3].as_int64()};
            if (!e.dir) total += e.size;
            list.push_back(e);
        }
        if (total == 0) total = 1;
        var dev = disc.getattr("dev");
        int64_t base = disc.getattr("base").as_int64(), done = 0, last = -1;
        const int64_t piece_max = 8 << 20;
        for (auto& e : list) {
            fs::path out = target / fs::u8path(e.path);
            if (e.dir) { fs::create_directories(out); continue; }
            fs::create_directories(out.parent_path());
            std::error_code ec;
            if (!(fs::is_regular_file(out, ec) && (int64_t)fs::file_size(out, ec) == e.size)) {
                fs::path part = out;
                part += ".part";
                {
                    std::ofstream f(part, std::ios::binary);
                    for (int64_t at = 0; at < e.size;) {
                        int64_t piece = std::min(piece_max, e.size - at);
                        var data = dev.call("read_at", {var(base + e.start * 0x800 + at), var(piece)});
                        const std::string& raw = py::bytes_raw(data);
                        f.write(raw.data(), (std::streamsize)raw.size());
                        at += piece;
                        int64_t pct = (done + at) * 100 / total;
                        if (pct != last) {
                            last = pct;
                            say("PROGRESS " + std::to_string(done + at) + " " + std::to_string(total));
                        }
                    }
                    if (!f) py::raise(py::T(py::t::OSError), "cannot write " + part.u8string());
                }
                fs::rename(part, out);
            }
            done += e.size;
        }
        say("PROGRESS " + std::to_string(total) + " " + std::to_string(total));
        return 0;
    }
    return usage("xbox-disc files <image> | read <image> <path> <out> | unpack <image> <folder>");
}

int xex_basic(const std::vector<var>& a) {
    if (a.size() != 2) return usage("xex-basic <file.xex> <out.xex>");
    fs::path out = path_of(a[1]);
    if (out.has_parent_path()) fs::create_directories(out.parent_path());
    py::import("codebreaker.formats.xex").getattr("basic")({a[0], a[1]});
    return 0;
}

// --name value options after the positional arguments
bool options(const std::vector<var>& a, size_t from, std::map<std::string, var>& opt) {
    for (size_t i = from; i < a.size(); i += 2) {
        std::string k = py::str_utf8(a[i]);
        if (k.rfind("--", 0) != 0 || i + 1 >= a.size() || !opt.count(k.substr(2))) return false;
        opt[k.substr(2)] = a[i + 1];
    }
    return true;
}

int xenon_config(const std::vector<var>& a) {
    std::map<std::string, var> opt = {{"out-dir", "ppc"_s}, {"switch-tables", "switch_tables.toml"_s}, {"file-path", py::None}};
    if (a.size() < 2 || !options(a, 2, opt))
        return usage("xenon-config <basic.xex> <out.toml> [--out-dir D] [--switch-tables F] [--file-path F]");
    var text = py::import("codebreaker.recompile.config").getattr("make")(
        {a[0], py::kw("out_dir", opt["out-dir"]), py::kw("switch_tables", opt["switch-tables"]), py::kw("file_path", opt["file-path"])});
    write_whole(path_of(a[1]), py::str_utf8(text));
    return 0;
}

int switch_tables(const std::vector<var>& a) {
    if (a.size() != 2) return usage("switch-tables <basic.xex> <out.toml>");
    fs::path out = path_of(a[1]);
    fs::path analysed = out;
    analysed.replace_extension(".analysed.toml");
    if (out.has_parent_path()) fs::create_directories(out.parent_path());
    // XenonAnalyse's tables for the bctr switches.py leaves unresolved (without them such a switch is an
    // indirect call into the middle of its function)
    var with_analysed = py::None;
    try {
        var xenon = py::import("codebreaker.recompile.xenon");
        xenon.getattr("build")({});
        py::import("subprocess").getattr("run")({py::list({xenon.getattr("ANALYSE"), a[0], py::str_from(analysed.u8string())}),
                                                 py::kw("capture_output", var(true))});
        std::ifstream f(analysed, std::ios::binary);
        if (f) with_analysed = py::str_from(std::string(std::istreambuf_iterator<char>(f), {}));
    } catch (py::error& e) {
        fprintf(stderr, "NOTE XenonAnalyse did not run (%s); switch tables from switches.py alone\n", e.what());
    }
    fs::path part = out;
    part += ".part";
    py::import("codebreaker.recompile.config").getattr("write_switch_tables")({py::str_from(part.u8string()), a[0], with_analysed});
    fs::rename(part, out);
    return 0;
}

int corpus_executables(const std::vector<var>& a) {
    if (!a.empty()) return usage("corpus-executables");
    var detect = py::import("codebreaker.formats.detect");
    std::vector<std::string> paths;
    for (var item : py::each(py::import("codebreaker.base.corpus").getattr("executables")({})))
        paths.push_back(py::str_utf8(py::unpack(item, 2)[0]));
    auto base_lower = [](const std::string& p) {
        std::string b = fs::u8path(p).filename().u8string();
        for (auto& c : b) c = (char)tolower((unsigned char)c);
        return b;
    };
    std::sort(paths.begin(), paths.end());
    paths.erase(std::unique(paths.begin(), paths.end()), paths.end());
    std::stable_sort(paths.begin(), paths.end(), [&](const std::string& x, const std::string& y) { return base_lower(x) < base_lower(y); });
    for (auto& p : paths) {
        std::string fmt = "?";
        try {
            fmt = py::str_utf8(py::str(detect.getattr("format_of")({py::str_from(p)})));
        } catch (py::error&) {
        }
        say(fmt + "\t" + p);
    }
    return 0;
}

void init(const var& m) {
    // main(argv): argv[0] is the command (the table's fixed argument), the rest its arguments
    py::mod_def(m, "main", [](const py::CallArgs& c) -> var {
        std::vector<var> argv = py::to_vector(c.pos.at(0));
        if (argv.empty()) return var((int64_t)usage("<command> ..."));
        std::string cmd = py::str_utf8(argv[0]);
        std::vector<var> a(argv.begin() + 1, argv.end());
        if (cmd == "xbox-disc") return var((int64_t)xbox_disc(a));
        if (cmd == "xex-basic") return var((int64_t)xex_basic(a));
        if (cmd == "xenon-config") return var((int64_t)xenon_config(a));
        if (cmd == "switch-tables") return var((int64_t)switch_tables(a));
        if (cmd == "corpus-executables") return var((int64_t)corpus_executables(a));
        return var((int64_t)usage(cmd + ": not a command of codebreaker_native"));
    });
}

const bool registered = (py::register_native_module("codebreaker_native", init), true);

}  // namespace
