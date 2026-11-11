// codebreaker/build/period16.cpp -- typed C++ port of codebreaker.build.period16 (see period16.hpp).

#include "codebreaker/build/period16.hpp"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/dos_tools.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::build::period16 {

namespace fs = std::filesystem;
using py::str;

namespace {

// os.listdir(folder): the names in a folder (FileNotFoundError / NotADirectoryError as Python's).
std::vector<str> listdir_(const str& folder) {
    std::error_code ec;
    const fs::path p = fs::u8path(folder.utf8());
    if (!fs::exists(p, ec)) throw py::FileNotFoundError("[WinError 3] The system cannot find the path specified: '" + folder.utf8() + "'");
    if (!fs::is_directory(p, ec)) throw py::NotADirectoryError("[WinError 267] The directory name is invalid: '" + folder.utf8() + "'");
    std::vector<str> out;
    for (auto it = fs::directory_iterator(p, ec); !ec && it != fs::directory_iterator(); it.increment(ec)) {
        out.push_back(str(it->path().filename().u8string()));
    }
    return out;
}

// os.path.getsize(path)
int64_t getsize_(const str& path) {
    std::error_code ec;
    const auto n = fs::file_size(fs::u8path(path.utf8()), ec);
    if (ec) throw py::FileNotFoundError("[WinError 2] The system cannot find the file specified: '" + path.utf8() + "'");
    return static_cast<int64_t>(n);
}

py::bytes read_all_(const str& path) {
    py::file f = py::open_rb(path);
    return f.read();
}

bool ends_with_any_(const str& name, std::initializer_list<const char*> suffixes) {
    const str low = name.lower();
    for (const char* s : suffixes) {
        if (low.endswith(str(s))) return true;
    }
    return false;
}

// the Python's `x in list` for a str list
int64_t index_of_(const py::list<str>& l, const char* what) {
    for (int64_t i = 0; i < l.size(); i++) {
        if (l[i] == str(what)) return i;
    }
    return -1;
}

}  // namespace

void print_line(const str& line) {
    const std::string s = line.utf8() + "\n";
    std::fwrite(s.data(), 1, s.size(), stdout);
}

const str DOC(
    "period16.py -- a 16-bit Windows DLL built from its source with the tools of its time.\n"
    "\n"
    "    python -m codebreaker build16 <source folder> [--original MODULE] [--work DIR] [--stub DONOR]\n"
    "\n"
    "    from codebreaker.build import period16\n"
    "    dll = period16.build(folder)                  the built DLL's path, or None\n"
    "    same, total = period16.compare(dll, original)  segments identical to the original's\n"
    "    period16.normalize(dll, donor, out)            the original's DOS stub put in front\n"
    "\n"
    "The 16-bit modules of Windows 95 were compiled with Microsoft C 6.0a and linked with the\n"
    "Windows SDK's LINK 5.60 against the Windows 3.1 DLL runtime. Microsoft C 6 here compiles but\n"
    "has no linker or libraries, so the build uses both kits, in DOSBox-X:\n"
    "\n"
    "    each module   Microsoft C 6.0a   cl /c /AM /G2 /Gsw /Ox            (the code)\n"
    "    the DLL       Visual C++ 1.52c   link @<folder's .lnk> ; rc          (the NE file, its runtime)\n"
    "\n"
    "A source folder holds the modules' .c files, the headers, the module-definition (.def) file\n"
    "and the linker's response file (.lnk) that lists the objects and names the DLL. Objects with\n"
    "no .c (the DLL entry, LIBENTRY.OBJ) are taken from the Windows 3.1 DLL kit\n"
    "(toolchains/period/libs/win31-kit) or the DDK. That kit's lean runtime is the one the\n"
    "originals were linked with; Visual C++ 1.52c's own is a third larger.\n"
    "\n"
    "settings: dosbox_x, msc6 (Microsoft C 6.0a), msvc152 (Visual C++ 1.52c: LINK, RC, LIB).\n"
    "\n"
    "The comparison is per segment, byte for byte: LINK lays out the code exactly as the original\n"
    "was laid out when the source is the source. tests/fixtures/lzexpand is a reconstruction of\n"
    "LZEXPAND.DLL made this way. Its notes said eight of twelve segments came out identical; against\n"
    "the corpus's copies today it builds eleven segments, several the original's length, none\n"
    "identical (the old tool gives the same) -- a measure of what is left, not of the build.\n");

str _win(const str& p) { return p.replace(str("/"), str("\\")); }

std::optional<str> _kit() {
    const str d = py::os::path::join(py::settings::checkout_root(), str("toolchains"), str("period"), str("libs"), str("win31-kit"));
    if (py::os::path::exists(py::os::path::join(d, str("MDLLCEW.LIB")))) return d;
    return std::nullopt;
}

std::optional<str> _libentry() {
    const std::optional<str> kit = _kit();
    if (kit && py::os::path::exists(py::os::path::join(*kit, str("LIBENTRY.OBJ")))) return py::os::path::join(*kit, str("LIBENTRY.OBJ"));
    const str p = base::corpus::reference(py::list<str>{str("Microsoft Windows 95 DDK"), str("LIB"), str("LIBENTRY.OBJ")});
    if (py::os::path::exists(p)) return p;
    return std::nullopt;
}

str _response_file(const str& folder) {
    std::vector<std::string> found;
    for (const str& f : listdir_(folder)) {
        if (f.lower().endswith(str(".lnk"))) found.push_back(f.utf8());
    }
    std::sort(found.begin(), found.end());   // (UTF-8 order is code point order)
    if (found.empty()) throw py::RuntimeError("no linker response file (.lnk) in " + folder.utf8());
    return str(found.front());
}

std::optional<str> build(const str& folder, const std::optional<str>& work_in, const str& flags) {
    const std::optional<str> dbx = py::settings::get_opt_str(str("dosbox_x"));
    const std::optional<str> msc6 = py::settings::get_opt_str(str("msc6"));
    const std::optional<str> msvc = py::settings::get_opt_str(str("msvc152"));
    const std::pair<const char*, const std::optional<str>*> kits[] = {{"dosbox_x", &dbx}, {"msc6", &msc6}, {"msvc152", &msvc}};
    for (const auto& k : kits) {
        if (!*k.second || (*k.second)->empty() || !py::os::path::exists(**k.second)) {
            throw py::RuntimeError(std::string(k.first) + " is not set up (settings.json: " + k.first + ")");
        }
    }
    const str work = (work_in && !work_in->empty()) ? *work_in : py::tempfile::mkdtemp(std::nullopt, str("build16_"));
    py::os::makedirs(work, 0777, true);
    for (const str& fn : listdir_(folder)) {
        if (ends_with_any_(fn, {".c", ".h", ".lnk", ".def", ".obj"})) py::shutil::copy2(py::os::path::join(folder, fn), work);
    }
    const str stub = py::os::path::join(*msvc, str("BIN"), str("WINSTUB.EXE"));   // the .def's STUB
    if (py::os::path::exists(stub)) py::shutil::copy2(stub, work);
    const str lnk_name = _response_file(work);
    str lnk;
    {
        py::file f = py::open(py::os::path::join(work, lnk_name), str("r"), str("latin1"));
        lnk = f.read_text();
    }
    const py::list<str> objs = py::re::findall(R"(([A-Za-z_]\w*)\.obj)", lnk);
    const py::list<str> dlls = py::re::findall(R"((\w+\.(?:dll|exe|drv)))", lnk, py::re::IGNORECASE);
    const str out_name = dlls.size() ? dlls[0] : str("out.dll");
    bool libentry = false;
    for (const str& o : objs) {
        if (o.lower() == str("libentry")) libentry = true;
    }
    if (libentry && !py::os::path::exists(py::os::path::join(work, str("LIBENTRY.OBJ")))) {
        const std::optional<str> entry = _libentry();
        if (entry) py::shutil::copy2(*entry, py::os::path::join(work, str("LIBENTRY.OBJ")));
    }
    const std::optional<str> kit = _kit();
    if (kit && py::re::search(R"(mdllcew\.lib)", lnk, py::re::IGNORECASE)) {   // LINK looks in the current folder first
        for (const char* lib : {"MDLLCEW.LIB", "LIBW.LIB"}) {
            if (py::os::path::exists(py::os::path::join(*kit, str(lib)))) py::shutil::copy2(py::os::path::join(*kit, str(lib)), py::os::path::join(work, str(lib)));
        }
    }
    py::list<str> modules;
    for (const str& m : objs) {
        if (py::os::path::exists(py::os::path::join(work, m + str(".c")))) modules.append(m);
    }
    py::list<str> bat;
    for (const str& m : modules) bat.append(str("del ") + m + str(".obj"));
    bat.append(str("del ") + out_name);
    bat.append(str("del done.txt"));
    for (const str& m : modules) bat.append(str("cl /c ") + flags + str(" ") + m + str(".c >> CL.TXT"));
    bat.append(str("link @") + lnk_name + str(" > LINK.TXT"));
    bat.append(str("rc ") + out_name + str(" > RC.TXT"));
    // windows.h is in the SDK's (Visual C++ 1.52c's) INCLUDE, the C library's headers in Microsoft C 6's
    py::dict<str, str> cpu;
    cpu.set(str("cycles"), str("max"));
    py::dict<str, str> variables;
    variables.set(str("PATH"), str("C:\\BIN;E:\\BIN"));
    variables.set(str("INCLUDE"), str("C:\\INCLUDE;E:\\INCLUDE"));
    variables.set(str("LIB"), str("D:\\;E:\\LIB"));
    dos_tools::Mounts mounts;
    mounts.append(std::make_tuple(str("c"), _win(*msc6)));
    mounts.append(std::make_tuple(str("e"), _win(*msvc)));
    mounts.append(std::make_tuple(str("d"), _win(work)));
    dos_tools::run(mounts, bat, py::list<str>{out_name}, 600, py::dict<str, dos_tools::FileData>(), cpu, std::nullopt, variables, work);
    const str dll = py::os::path::join(work, out_name);
    if (py::os::path::exists(dll)) return dll;
    return std::nullopt;
}

py::list<std::tuple<int64_t, py::bytes>> segments(const str& path) {
    const py::bytes d = read_all_(path);
    const int64_t lfa = py::unpack_from1<int64_t>("<I", d, 0x3c);
    const int64_t n = py::unpack_from1<int64_t>("<H", d, lfa + 0x1c);
    const int64_t table = py::unpack_from1<int64_t>("<H", d, lfa + 0x22) + lfa;
    int64_t align = py::unpack_from1<int64_t>("<H", d, lfa + 0x32);
    if (!align) align = 9;
    py::list<std::tuple<int64_t, py::bytes>> out;
    for (int64_t i = 0; i < n; i++) {
        const auto rec = py::unpack_from<int64_t, int64_t, int64_t, int64_t>("<HHHH", d, table + i * 8);
        const int64_t sector = std::get<0>(rec), length = std::get<1>(rec);
        out.append(std::make_tuple(length, d.slice(sector << align, (sector << align) + length)));
    }
    return out;
}

std::tuple<int64_t, int64_t> compare(const str& built, const str& original, const Log& log) {
    const auto b = segments(built);
    const auto o = segments(original);
    if (log) {
        log(py::percent(str("built: %d segments, %d bytes   original: %d segments, %d bytes"), b.size(), getsize_(built), o.size(),
                        getsize_(original)));
    }
    int64_t same = 0;
    const int64_t count = std::max<int64_t>(b.size(), o.size());
    for (int64_t i = 0; i < count; i++) {
        const std::optional<int64_t> bl = i < b.size() ? std::optional<int64_t>(std::get<0>(b[i])) : std::nullopt;
        const std::optional<int64_t> ol = i < o.size() ? std::optional<int64_t>(std::get<0>(o[i])) : std::nullopt;
        str verdict;
        if (i < b.size() && i < o.size() && std::get<1>(b[i]) == std::get<1>(o[i])) {
            verdict = str("identical");
            same += 1;
        } else {
            verdict = bl == ol ? str("same length, different") : str("different length");
        }
        if (log) {
            log(py::percent(str("  segment %-2d built %-5s original %-5s %s"), i + 1, bl ? py::to_str(*bl) : str("-"),
                            ol ? py::to_str(*ol) : str("-"), verdict));
        }
    }
    if (log) log(py::percent(str("%d of %d segments identical"), same, o.size()));
    return std::make_tuple(same, static_cast<int64_t>(o.size()));
}

py::bytes normalize(const str& built, const str& donor, const str& out) {
    const py::bytes file = read_all_(built);
    py::bytearray d(file);
    py::bytes stub = read_all_(donor);
    if (stub.slice(0, 2) == py::bytes("MZ")) {
        const int64_t lfanew = py::unpack_from1<int64_t>("<H", stub, 0x3c);
        if (0 < lfanew && lfanew <= stub.size()) stub = stub.slice(0, lfanew);
    }
    const int64_t ne = py::unpack_from1<int64_t>("<H", file, 0x3c);
    const int64_t new_ne = stub.size();
    if (new_ne % 16 || new_ne > ne) {
        throw py::ValueError(py::percent(str("the donor's stub (0x%x) cannot replace this one (0x%x)"), new_ne, ne).utf8());
    }
    const int64_t shift = ne - new_ne;
    int64_t align = py::unpack_from1<int64_t>("<H", file, ne + 0x32);
    if (!align) align = 9;
    if (shift % (int64_t(1) << align)) {
        throw py::ValueError(py::percent(str("the stubs differ by 0x%x, not a whole number of sectors"), shift).utf8());
    }
    const int64_t sectors = shift >> align;
    py::bytearray o = py::bytearray(stub) + file.slice(ne, file.size());
    py::pack_into("<H", o, 0x3c, new_ne);
    const int64_t n = py::unpack_from1<int64_t>("<H", o.view(), new_ne + 0x1c);
    const int64_t table = py::unpack_from1<int64_t>("<H", o.view(), new_ne + 0x22);
    for (int64_t i = 0; i < n; i++) {
        const int64_t p = new_ne + table + i * 8;
        const int64_t sector = py::unpack_from1<int64_t>("<H", o.view(), p);
        if (sector) py::pack_into("<H", o, p, sector - sectors);   // 0: no image in the file
    }
    const int64_t nonresident = py::unpack_from1<int64_t>("<I", o.view(), new_ne + 0x2c);
    if (nonresident) py::pack_into("<I", o, new_ne + 0x2c, nonresident - shift);
    const int64_t fast = py::unpack_from1<int64_t>("<H", o.view(), new_ne + 0x38);
    if (fast) py::pack_into("<H", o, new_ne + 0x38, fast - sectors);
    {
        py::file f = py::open(out, str("wb"));
        f.write(o);
    }
    return py::bytes(o.view());
}

int64_t main(const py::list<str>& argv) {
    if (argv.empty()) {
        print_line(DOC);
        return 2;
    }
    const str folder = argv[0];
    std::optional<str> work;
    const int64_t wi = index_of_(argv, "--work");
    if (wi >= 0) {
        if (wi + 1 >= argv.size()) throw py::IndexError("list index out of range");
        work = argv[wi + 1];
    }
    const std::optional<str> dll = build(folder, work);
    if (!dll) {
        print_line(str("no DLL was made (see CL.TXT and LINK.TXT in the work folder)"));
        return 1;
    }
    print_line(py::percent(str("built: %s"), *dll));
    std::optional<str> original;
    const int64_t oi = index_of_(argv, "--original");
    if (oi >= 0) {
        if (oi + 1 >= argv.size()) throw py::IndexError("list index out of range");
        original = argv[oi + 1];
    } else {
        original = py::os::path::basename(*dll);
    }
    original = py::os::path::isfile(*original) ? original : base::corpus::retail(*original);
    const int64_t si = index_of_(argv, "--stub");
    if (si >= 0) {
        if (si + 1 >= argv.size()) throw py::IndexError("list index out of range");
        const str donor0 = argv[si + 1];
        const std::optional<str> donor = py::os::path::isfile(donor0) ? std::optional<str>(donor0) : base::corpus::retail(donor0);
        if (!donor) throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
        normalize(*dll, *donor, *dll);
    }
    if (original && !original->empty()) compare(*dll, *original);
    return 0;
}

}  // namespace codebreaker::build::period16
