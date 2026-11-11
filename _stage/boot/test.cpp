// test.cpp -- the typed C++ codebreaker.source.vxd.boot on the cases of expected.py (cases.txt, inputs/); prints expected.txt's lines.
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>

#include "../_vxd_common/common.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/source/vxd/boot.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "py_settings.hpp"
#include "py_zlib.hpp"

namespace B = codebreaker::source::vxd::boot;
namespace dis = codebreaker::source::vxd::disassembly;
using namespace vh;

static string unhex(const string& h) { return h == "~" ? string() : from_hex(h).raw(); }
static string b2s(bool b) { return b ? "true" : "false"; }
static string strs(const py::list<py::str>& l) {
    vector<string> o;
    for (const py::str& s : l) o.push_back(jstr(s.utf8()));
    return jarr(o);
}
static string strs_v(const vector<string>& l) {
    vector<string> o;
    for (const string& s : l) o.push_back(jstr(s));
    return jarr(o);
}

static string norm_path(const py::str& p) {
    const py::str croot = codebreaker::base::corpus::root();
    return p.replace(croot, py::str("<C>")).replace(py::str("\\"), py::str("/")).utf8();
}

static string state_;

// the lines of a log without a screen's change (timing) and with the process number as N
static vector<string> lines_norm(const vector<string>& ls) {
    static const py::re::Pattern screen = py::re::compile(py::str("^\\s+\\d+s\\s+\\w+ screen \\d+x\\d+$"));
    static const py::re::Pattern pid = py::re::compile(py::str("QEMU process \\d+"));
    vector<string> out;
    for (const string& l : ls) {
        const py::str s(l);
        if (screen.match(s)) continue;
        string u = pid.sub(py::str("QEMU process N"), s).utf8();
        for (size_t p = u.find(state_); !state_.empty() && p != string::npos; p = u.find(state_, p + 7)) u.replace(p, state_.size(), "<STATE>");
        out.push_back(u);
    }
    return out;
}

static B::Image load(const string& nm) { return B::read_ppm(py::bytes(read_file("inputs/" + nm + ".ppm"))); }

static string rgbd(const B::Image& img) { return jarr({jint(img.width), jint(img.height), jstr(sha1(img.rgb))}); }

static string pix_j(const B::Pixel& p) { return jarr({jint(std::get<0>(p)), jint(std::get<1>(p)), jint(std::get<2>(p))}); }

// a PNG back to pixels (what PIL reads): IHDR, IDAT inflated, filter 0 rows
static B::Image png_decode(const py::bytes& png) {
    const string& s = png.raw();
    size_t pos = 8;
    B::Image img;
    string idat;
    while (pos + 8 <= s.size()) {
        const uint32_t len = (static_cast<unsigned char>(s[pos]) << 24) | (static_cast<unsigned char>(s[pos + 1]) << 16) | (static_cast<unsigned char>(s[pos + 2]) << 8) |
                             static_cast<unsigned char>(s[pos + 3]);
        const string type = s.substr(pos + 4, 4);
        const string data = s.substr(pos + 8, len);
        if (type == "IHDR") {
            img.width = (static_cast<unsigned char>(data[0]) << 24) | (static_cast<unsigned char>(data[1]) << 16) | (static_cast<unsigned char>(data[2]) << 8) | static_cast<unsigned char>(data[3]);
            img.height = (static_cast<unsigned char>(data[4]) << 24) | (static_cast<unsigned char>(data[5]) << 16) | (static_cast<unsigned char>(data[6]) << 8) | static_cast<unsigned char>(data[7]);
        } else if (type == "IDAT") {
            idat += data;
        }
        pos += 12 + len;
    }
    const string raw = py::zlib::decompress(py::bytes(idat)).raw();
    for (int64_t y = 0; y < img.height; ++y) img.rgb += raw.substr(static_cast<size_t>(y * (img.width * 3 + 1) + 1), static_cast<size_t>(img.width * 3));
    return img;
}

static string files_state(const string& tag) {
    const string log = py::os::path::join(py::settings::get_str(py::str("state")), py::str("vxdboot"), py::str("shots"), py::str(tag + ".bootlog.txt")).utf8();
    return jarr({std::filesystem::exists(log) ? jstr(sha1(read_file(log))) : string("null")});
}

static string boot_err(const string& disk, const string& tag, double wait) {
    try {
        py::dict<py::str, py::bytes> files;
        files.set(py::str("X.TXT"), py::bytes("hi"));
        B::boot(py::str(disk), files, py::str(tag), wait, [](const py::str&) {});
    } catch (const py::Exception& e) {
        string t = e.type();
        string msg = t == "RuntimeError" ? string(e.what()) : string();
        const string cwd = py::os::getcwd().utf8();
        for (size_t p = msg.find(cwd); p != string::npos; p = msg.find(cwd, p + 3)) msg.replace(p, cwd.size(), "<H>");
        return jarr({jstr(t), jstr(msg)});
    }
    return "null";
}

static vector<string> collect_lines(const std::function<void(const B::Log&)>& f) {
    vector<string> lines;
    f([&](const py::str& s) { lines.push_back(s.utf8()); });
    return lines;
}

int main(int argc, char** argv) {
    _putenv_s("CB_CHECKOUT_ROOT", "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker");
    dis::set_capstone_dll(py::str(read_file(argc > 2 ? argv[2] : "capstone.txt")));
    state_ = py::settings::get_str(py::str("state")).utf8();
    return run_cases(argc, argv, [&](const vector<string>& f) -> string {
        const string& op = f[0];
        if (op == "consts") {
            vector<string> g, o;
            for (const B::Pixel& p : B::GREYS) g.push_back(pix_j(p));
            for (const auto& kv : B::BOOT_OPTIONS) o.push_back(jarr({jstr(std::get<0>(kv).utf8()), jstr(std::get<1>(kv).utf8())}));
            return jarr({jstr(B::DEFAULT_DISK.utf8()), jarr(g), jarr(o)});
        }
        if (op == "corpus_file") return jstr(norm_path(B::corpus_file(py::str(unhex(f[1])))));
        if (op == "guest_path") return jstr(B::guest_path(B::corpus_file(py::str(unhex(f[1])))).utf8());
        if (op == "guest_raw") {
            string t = unhex(f[1]);
            if (t.find(':') == string::npos) {
                for (char& c : t)
                    if (c == '/') c = '\\';
                t = codebreaker::base::corpus::root().utf8() + "\\" + t;
            }
            return jstr(B::guest_path(py::str(t)).utf8());
        }
        if (op == "build") {
            const py::bytes b = B::build(B::corpus_file(py::str(unhex(f[1]))));
            return jarr({jint(b.size()), jstr(sha1(b))});
        }
        if (op == "build_path") {
            const py::bytes b = B::build(py::str(f[1]));
            return jarr({jint(b.size()), jstr(sha1(b))});
        }
        if (op == "members") {
            const string file = f[1].compare(0, 3, "<C>") == 0 ? B::corpus_file(py::str("vmm32")).utf8() : f[1];
            return strs(B::_bundle_members(py::bytes(read_file(file))));
        }
        if (op == "bootopts") {
            const string text = unhex(f[1]);
            if (f[2] == "-") return jstr(B::boot_options(py::str(text)).utf8());
            std::optional<py::list<std::tuple<py::str, py::str>>> opts;
            py::list<std::tuple<py::str, py::str>> l;
            if (f[2] != "~")
                for (const string& kv : split(f[2], ',')) {
                    const size_t eq = kv.find('=');
                    l.append(std::make_tuple(py::str(kv.substr(0, eq)), py::str(kv.substr(eq + 1))));
                }
            opts = l;
            return jstr(B::boot_options(py::str(text), opts).utf8());
        }
        if (op == "plan") {
            py::list<py::str> ds;
            if (f[2] != "-")
                for (const string& d : split(f[2], ',')) ds.append(py::str(d));
            vector<string> lines;
            const auto res = B::plan(py::str(f[1]), ds, f[3] == "1", f[4] == "1", [&](const py::str& s) { lines.push_back(s.utf8()); });
            vector<string> files;
            for (const auto& kv : res.items()) files.push_back(jarr({jstr(std::get<0>(kv).utf8()), jint(std::get<1>(kv).size()), jstr(sha1(std::get<1>(kv)))}));
            return jarr({strs_v(lines), jarr(files)});
        }
        if (op == "classify") return jstr(B::classify(load(f[1])).utf8());
        if (op == "resize") return rgbd(B::resize_nearest(load(f[1]), std::stoll(f[2]), std::stoll(f[3])));
        if (op == "crop") return rgbd(B::crop(load(f[1]), std::stoll(f[2]), std::stoll(f[3]), std::stoll(f[4]), std::stoll(f[5])));
        if (op == "pixel") {
            const B::Image img = load(f[1]);
            const int64_t x = std::stoll(f[2]), y = std::stoll(f[3]);
            if (y < 0 || (y == img.height - 14 && img.height <= 14 && x == 0)) return "null";
            return pix_j(B::getpixel(img, x, y));
        }
        if (op == "png") return rgbd(png_decode(B::png_bytes(load(f[1]))));
        if (op == "teal_all") {
            vector<string> o;
            for (int64_t r : {0, 10, 87, 100})
                for (int64_t g : {100, 110, 111, 128, 168, 189, 190})
                    for (int64_t b : {100, 128, 140, 168, 180, 200}) o.push_back(b2s(B::_teal(B::Pixel(r, g, b))));
            return jarr(o);
        }
        if (op == "loaded") return jstr(B::loaded(py::bytes(read_file(f[1])).decode("latin1"), py::str(unhex(f[2]))).utf8());
        if (op == "bisect") {
            py::list<py::str> units;
            if (f[1] != "-")
                for (const string& u : split(f[1], ',')) units.append(py::str(u));
            const string rule = f[2];
            vector<string> trials, log;
            auto has = [](const py::list<py::str>& part, const char* u) {
                for (const py::str& p : part)
                    if (p == py::str(u)) return true;
                return false;
            };
            auto trial = [&](const py::list<py::str>& part, const py::str& name) -> py::str {
                trials.push_back(jarr({strs(part), jstr(name.utf8())}));
                const bool fail = has(part, rule.c_str()) || (rule == "ab" && has(part, "a") && has(part, "b")) || (rule == "bd" && has(part, "b") && has(part, "d")) ||
                                  (rule == "ae" && (has(part, "a") || has(part, "e")));
                return fail ? py::str("FAIL(x)") : py::str("PASS");
            };
            const auto res = B::bisect(units, trial, py::str("tg"), [&](const py::str& s) { log.push_back(s.utf8()); });
            return jarr({jarr(trials), strs(res), strs_v(log)});
        }
        if (op == "boot") {
            const string& mode = f[4];
            const double wait = std::stod(f[3]);
            if (mode == "err") return boot_err(f[1], f[2], wait);
            if (mode == "space") {
                _wputenv_s(L"CB_MIN_FREE_GB", L"100000000");
                string r;
                try {
                    r = boot_err(f[1], f[2], wait);
                } catch (...) {
                    _wputenv_s(L"CB_MIN_FREE_GB", L"");
                    throw;
                }
                _wputenv_s(L"CB_MIN_FREE_GB", L"");
                return r;
            }
            vector<string> lines;
            py::dict<py::str, py::bytes> files;
            files.set(py::str("X.TXT"), py::bytes("hi"));
            const B::BootResult r = B::boot(py::str(f[1]), files, py::str(f[2]), wait, [&](const py::str& s) { lines.push_back(s.utf8()); });
            return jarr({jstr(r.verdict.utf8()), jstr(r.bootlog.utf8()), jstr(py::os::path::basename(r.png).utf8()), files_state(f[2]), strs_v(lines_norm(lines))});
        }
        if (op == "run") {
            py::list<py::str> ds;
            if (f[2] != "-")
                for (const string& d : split(f[2], ',')) ds.append(py::str(d));
            vector<string> lines;
            const py::str v = B::run(py::str(f[1]), ds, false, f[3] == "1", py::str(f[4]), std::stod(f[5]), [&](const py::str& s) { lines.push_back(s.utf8()); });
            return jarr({jstr(v.utf8()), strs_v(lines_norm(lines))});
        }
        if (op == "main") {
            py::list<py::str> args;
            string tag;
            for (size_t i = 1; i < f.size(); ++i) {
                args.append(py::str(unhex(f[i])));
                if (i > 1 && unhex(f[i - 1]) == "--tag") tag = unhex(f[i]);
            }
            std::ostringstream out, err;
            int64_t status = 0;
            try {
                status = B::main(args, out, err);
            } catch (const py::Exception& e) {
                return jarr({jstr("ERR"), jstr(e.type())});
            }
            vector<string> lines;
            std::istringstream is(out.str());
            string ln;
            while (std::getline(is, ln)) lines.push_back(ln);
            return jarr({jint(status), strs_v(lines_norm(lines)), tag.empty() ? string("null") : files_state(tag)});
        }
        throw py::ValueError("bad case " + op);
    });
}
