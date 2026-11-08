// codebreaker/formats/xbox/crypto/build_lib.cpp -- C++ port of codebreaker.formats.xbox.crypto.build_lib.
//
// (The Python imports io.BytesIO, enum, subprocess and struct names; what it uses of them is below. The
// `BinLike` TypeVar and the `from .XeCrypt import *` / `from .lzx import *` / `from .StreamIO import *`
// star imports are the includes of the header.)

#include "codebreaker/formats/xbox/crypto/build_lib.hpp"

#include <algorithm>
#include <filesystem>
#include <type_traits>
#include <vector>

#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::formats::xbox::crypto::build_lib {

using StreamIO::Endian;
using StreamIO_ = StreamIO::StreamIO;
namespace lzx = codebreaker::formats::xbox::crypto::lzx;

const py::str& BIN_DIR() {
    static const py::str value = py::settings::get_str(py::str("xbox360_crypto_bin"));   // Free60 xenon-as / xenon-objcopy dir
    return value;
}
const py::str INCLUDE_DIR("Patches/include");

// ------------------------------------------------------------------------------------------- bytes
namespace {

void put16(std::string& out, int64_t v) {
    out.push_back(static_cast<char>((v >> 8) & 0xFF));
    out.push_back(static_cast<char>(v & 0xFF));
}
void put32(std::string& out, int64_t v) {
    put16(out, (v >> 16) & 0xFFFF);
    put16(out, v & 0xFFFF);
}
int64_t get16(const std::string& s, size_t at) {
    return (static_cast<int64_t>(static_cast<uint8_t>(s[at])) << 8) | static_cast<uint8_t>(s[at + 1]);
}
int64_t get32(const std::string& s, size_t at) { return (get16(s, at) << 16) | get16(s, at + 2); }

[[noreturn]] void buffer_too_small(int64_t have, int64_t need) {
    throw py::ValueError("Buffer size too small (" + std::to_string(have) + " instead of at least " + std::to_string(need) + " bytes)");
}

// struct.unpack_from(fmt, data, offset) with a buffer that is too short
[[noreturn]] void unpack_from_short(int64_t offset, int64_t size, int64_t have) {
    throw py::StructError("unpack_from requires a buffer of at least " + std::to_string(offset + size) + " bytes for unpacking " +
                          std::to_string(size) + " bytes at offset " + std::to_string(offset) + " (actual buffer size is " +
                          std::to_string(have) + ")");
}

// int.from_bytes(data, "big", signed=False) for at most 8 bytes
int64_t from_bytes_big(const py::bytes& b) {
    int64_t v = 0;
    for (char c : b.raw()) v = (v << 8) | static_cast<uint8_t>(c);
    return v;
}

}  // namespace

// ------------------------------------------------------------------------------------- structures
py::bytes FLASH_HEADER::to_bytes() const {
    std::string s;
    put16(s, magic);
    put16(s, build);
    put16(s, qfe);
    put16(s, flags);
    put32(s, entry);
    put32(s, size);
    s.append(reinterpret_cast<const char*>(copyright.data()), copyright.size());
    s.append(reinterpret_cast<const char*>(padding.data()), padding.size());
    put32(s, kv_length);
    put32(s, sys_upd_addr);
    put16(s, patch_slots);
    put16(s, kv_version);
    put32(s, kv_offset);
    put32(s, patch_slot_size);
    put32(s, smc_config_offset);
    put32(s, smc_length);
    put32(s, smc_offset);
    return py::bytes(s);
}

FLASH_HEADER FLASH_HEADER::from_buffer_copy(const py::bytes& data) {
    if (data.size() < SIZE) buffer_too_small(data.size(), SIZE);
    const std::string& s = data.raw();
    FLASH_HEADER h;
    h.magic = static_cast<WORD>(get16(s, 0));
    h.build = static_cast<WORD>(get16(s, 2));
    h.qfe = static_cast<WORD>(get16(s, 4));
    h.flags = static_cast<WORD>(get16(s, 6));
    h.entry = static_cast<DWORD>(get32(s, 8));
    h.size = static_cast<DWORD>(get32(s, 12));
    std::copy_n(s.begin() + 16, 0x40, reinterpret_cast<char*>(h.copyright.data()));
    std::copy_n(s.begin() + 0x50, 0x10, reinterpret_cast<char*>(h.padding.data()));
    h.kv_length = static_cast<DWORD>(get32(s, 0x60));
    h.sys_upd_addr = static_cast<DWORD>(get32(s, 0x64));
    h.patch_slots = static_cast<WORD>(get16(s, 0x68));
    h.kv_version = static_cast<WORD>(get16(s, 0x6A));
    h.kv_offset = static_cast<DWORD>(get32(s, 0x6C));
    h.patch_slot_size = static_cast<DWORD>(get32(s, 0x70));
    h.smc_config_offset = static_cast<DWORD>(get32(s, 0x74));
    h.smc_length = static_cast<DWORD>(get32(s, 0x78));
    h.smc_offset = static_cast<DWORD>(get32(s, 0x7C));
    return h;
}

py::bytes BLDR_HEADER::to_bytes() const {
    std::string s;
    s.append(reinterpret_cast<const char*>(magic.data()), magic.size());
    put16(s, build);
    put16(s, qfe);
    put16(s, flags);
    put32(s, entry);
    put32(s, size);
    return py::bytes(s);
}

BLDR_HEADER BLDR_HEADER::from_buffer_copy(const py::bytes& data) {
    if (data.size() < SIZE) buffer_too_small(data.size(), SIZE);
    const std::string& s = data.raw();
    BLDR_HEADER h;
    std::copy_n(s.begin(), 2, reinterpret_cast<char*>(h.magic.data()));
    h.build = static_cast<WORD>(get16(s, 2));
    h.qfe = static_cast<WORD>(get16(s, 4));
    h.flags = static_cast<WORD>(get16(s, 6));
    h.entry = static_cast<DWORD>(get32(s, 8));
    h.size = static_cast<DWORD>(get32(s, 12));
    return h;
}

py::bytes SB_2BL_HEADER::to_bytes() const {
    std::string s = header.to_bytes().raw();
    s.append(reinterpret_cast<const char*>(nonce.data()), nonce.size());
    return py::bytes(s);
}

SB_2BL_HEADER SB_2BL_HEADER::from_buffer_copy(const py::bytes& data) {
    if (data.size() < SIZE) buffer_too_small(data.size(), SIZE);
    SB_2BL_HEADER h;
    h.header = BLDR_HEADER::from_buffer_copy(data);
    std::copy_n(data.raw().begin() + 0x10, 0x10, reinterpret_cast<char*>(h.nonce.data()));
    return h;
}

// --------------------------------------------------------------------------------------- BLHeader
BLHeader::BLHeader(bool include_nonce_) {
    reset();
    include_nonce = include_nonce_;
    // self.parse(data)
}

py::bytes BLHeader::__bytes__() const {
    // struct.pack(">2s 3H 2I", magic, build, qfe, flags, entry_point, size)
    std::string data;
    if (!magic) throw py::StructError("argument for 's' must be a bytes object");
    std::string m = magic->raw().substr(0, 2);
    m.resize(2, '\0');
    data += m;
    for (const std::optional<int64_t>* field : {&build, &qfe, &flags}) {
        if (!*field) throw py::StructError("required argument is not an integer");
        if (**field < 0 || **field > 0xFFFF) throw py::StructError("'H' format requires 0 <= number <= 65535");
        put16(data, **field);
    }
    for (const std::optional<int64_t>* field : {&entry_point, &size}) {
        if (!*field) throw py::StructError("required argument is not an integer");
        if (**field < 0 || **field > 0xFFFFFFFFLL) throw py::StructError("'I' format requires 0 <= number <= 4294967295");
        put32(data, **field);
    }
    if (include_nonce) {
        if (!nonce) throw py::TypeError("can't concat NoneType to bytes");
        data += nonce->raw();
    }
    return py::bytes(data);
}

BLHeaderDict BLHeader::__dict__() const {
    BLHeaderDict dct;
    dct.magic = magic;
    dct.build = build;
    dct.qfe = qfe;
    dct.flags = flags;
    dct.entry_point = entry_point;
    dct.size = size;
    if (include_nonce) {
        dct.has_nonce = true;
        dct.nonce = nonce;
    }
    return dct;
}

namespace {

// the attributes of a BLHeader that are neither fields nor None (Python 3.14): its methods, parse and the
// dunders every object has (getattr gives a callable, a class, a str, an int or a tuple)
const char* const OBJECT_ATTRIBUTES[] = {
    "__bytes__", "__class__", "__delattr__", "__dict__", "__dir__", "__eq__", "__firstlineno__", "__format__", "__ge__",
    "__getattribute__", "__getitem__", "__getstate__", "__gt__", "__hash__", "__init__", "__init_subclass__", "__le__", "__lt__",
    "__module__", "__ne__", "__new__", "__reduce__", "__reduce_ex__", "__repr__", "__setattr__", "__setitem__", "__sizeof__",
    "__static_attributes__", "__str__", "__subclasshook__", "parse", "reset"};

bool is_object_attribute(const std::string& name) {
    for (const char* n : OBJECT_ATTRIBUTES)
        if (name == n) return true;
    return false;
}

}  // namespace

Attribute BLHeader::__getitem__(const py::str& item_) const {
    std::string item = item_.lower().utf8();
    Attribute a;
    auto of_bytes = [&](const std::optional<py::bytes>& v) {
        if (v) a.value = *v;
    };
    auto of_int = [&](const std::optional<int64_t>& v) {
        if (v) a.value = *v;
    };
    if (item == "include_nonce") a.value = include_nonce;
    else if (item == "magic") of_bytes(magic);
    else if (item == "nonce") of_bytes(nonce);
    else if (item == "build") of_int(build);
    else if (item == "qfe") of_int(qfe);
    else if (item == "flags") of_int(flags);
    else if (item == "entry_point") of_int(entry_point);
    else if (item == "size") of_int(size);
    else if (item == "header_size") a.value = header_size();
    else if (item == "padded_size") a.value = padded_size();
    else if (item == "requires_padding") a.value = requires_padding();
    else if (is_object_attribute(item)) a.value = Attribute::Object{};
    return a;
}

namespace {

bool truthy(const FieldValue& v) {
    if (std::holds_alternative<py::bytes>(v)) return !std::get<py::bytes>(v).empty();
    if (std::holds_alternative<int64_t>(v)) return std::get<int64_t>(v) != 0;
    return std::get<bool>(v);
}

[[noreturn]] void wrong_type(const char* field, const char* want) {
    throw py::TypeError(std::string("BLHeader.") + field + " holds " + want);
}

void set_bytes_field(std::optional<py::bytes>& field, const char* name, const FieldValue& value) {
    if (!field) return;   // getattr(self, key, None) is None: nothing is set
    if (!std::holds_alternative<py::bytes>(value)) wrong_type(name, "bytes");
    field = std::get<py::bytes>(value);
}

void set_int_field(std::optional<int64_t>& field, const char* name, const FieldValue& value) {
    if (!field) return;
    if (std::holds_alternative<int64_t>(value)) field = std::get<int64_t>(value);
    else if (std::holds_alternative<bool>(value)) field = std::get<bool>(value) ? 1 : 0;
    else wrong_type(name, "an int");
}

}  // namespace

void BLHeader::__setitem__(const py::str& key_, const FieldValue& value) {
    std::string key = key_.lower().utf8();
    if (key == "include_nonce") include_nonce = truthy(value);
    else if (key == "magic") set_bytes_field(magic, "magic", value);
    else if (key == "nonce") set_bytes_field(nonce, "nonce", value);
    else if (key == "build") set_int_field(build, "build", value);
    else if (key == "qfe") set_int_field(qfe, "qfe", value);
    else if (key == "flags") set_int_field(flags, "flags", value);
    else if (key == "entry_point") set_int_field(entry_point, "entry_point", value);
    else if (key == "size") set_int_field(size, "size", value);
    else if (key == "header_size" || key == "padded_size" || key == "requires_padding") {
        // the properties have no setter (and getattr evaluates the property first)
        if (key == "padded_size") padded_size();
        if (key == "requires_padding") requires_padding();
        throw py::AttributeError("property '" + key + "' of 'BLHeader' object has no setter");
    }
    // (a method or dunder name: setattr would shadow it with an instance attribute -- not modelled)
}

int64_t BLHeader::header_size() const {
    if (include_nonce) return 0x20;
    return 0x10;
}

int64_t BLHeader::padded_size() const {
    if (!size) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
    return (*size + 0xF) & ~static_cast<int64_t>(0xF);
}

bool BLHeader::requires_padding() const { return padded_size() > 0; }

BLHeader BLHeader::parse(const BinLike& data, bool include_nonce) {
    BLHeader hdr(include_nonce);
    // (hdr.magic, hdr.build, hdr.qfe, hdr.flags, hdr.entry_point, hdr.size) = unpack_from(">2s 3H 2I", data, 0)
    if (data.size() < 16) unpack_from_short(0, 16, data.size());
    const std::string& s = data.raw();
    hdr.magic = py::bytes(s.substr(0, 2));
    hdr.build = get16(s, 2);
    hdr.qfe = get16(s, 4);
    hdr.flags = get16(s, 6);
    hdr.entry_point = get32(s, 8);
    hdr.size = get32(s, 12);
    if (hdr.include_nonce) {
        // (hdr.nonce,) = unpack_from("16s", data, 0x10)
        if (data.size() < 0x20) unpack_from_short(0x10, 16, data.size());
        hdr.nonce = py::bytes(s.substr(0x10, 16));
    }
    return hdr;
}

void BLHeader::reset() {
    include_nonce = true;
    magic = std::nullopt;
    build = std::nullopt;
    qfe = std::nullopt;
    flags = std::nullopt;
    size = std::nullopt;
    nonce = std::nullopt;
}

// ------------------------------------------------------------------------------------------ paths
namespace {

bool is_sep(char32_t c) { return c == U'\\' || c == U'/'; }

// pathlib.Path on Windows, as a str: drive, root and names (no empty names, no ".")
struct PathParts {
    std::u32string drive;
    std::u32string root;
    std::vector<std::u32string> names;
};

PathParts split_path(const py::str& s) {
    std::u32string u = s.u32();
    auto dr = py::os::path::splitdrive_(u);
    PathParts p;
    p.drive = dr.first;
    for (char32_t& c : p.drive)
        if (c == U'/') c = U'\\';
    const std::u32string& rest = dr.second;
    size_t i = 0;
    if (!rest.empty() && is_sep(rest[0])) {
        p.root = U"\\";
        while (i < rest.size() && is_sep(rest[i])) i++;
    }
    while (i <= rest.size()) {
        size_t j = i;
        while (j < rest.size() && !is_sep(rest[j])) j++;
        std::u32string name = rest.substr(i, j - i);
        if (!name.empty() && name != U".") p.names.push_back(name);
        i = j + 1;
    }
    return p;
}

py::str join_parts(const PathParts& p) {
    std::u32string out = p.drive + p.root;
    for (size_t k = 0; k < p.names.size(); k++) {
        if (k) out += U'\\';
        out += p.names[k];
    }
    if (out.empty()) return py::str(".");
    return py::str(out);
}

py::str path_str(const py::str& s) { return join_parts(split_path(s)); }   // str(Path(s))

py::str path_join(const py::str& base, const py::str& name) {   // str(Path(base) / name)
    PathParts p = split_path(base);
    for (const std::u32string& n : split_path(name).names) p.names.push_back(n);
    return join_parts(p);
}

py::str path_parent(const py::str& s) {   // str(Path(s).parent)
    PathParts p = split_path(s);
    if (!p.names.empty()) p.names.pop_back();
    return join_parts(p);
}

// str(Path(s).absolute())
py::str path_absolute(const py::str& s) {
    PathParts p = split_path(s);
    if (!p.drive.empty() && !p.root.empty()) return join_parts(p);
    PathParts cwd = split_path(py::os::getcwd());
    if (!p.drive.empty()) return py::os::path::abspath(join_parts(p));   // "C:name": the cwd of that drive
    if (!p.root.empty()) {   // "\name": the cwd's drive
        p.drive = cwd.drive;
        return join_parts(p);
    }
    for (const std::u32string& n : p.names) cwd.names.push_back(n);
    return join_parts(cwd);
}

}  // namespace

// ------------------------------------------------------------------------------------------ files
std::optional<std::variant<py::bytes, py::str>> read_file_or_none(const py::str& filename, bool text) {
    if (filename == py::str("")) return std::nullopt;
    py::str p = path_str(filename);
    if (!py::os::path::isfile(p)) return std::nullopt;
    if (text) {
        py::file f = py::open(p, py::str("r"));
        py::str data = f.read_text();
        f.close();
        return std::variant<py::bytes, py::str>(data);
    }
    py::file f = py::open(p, py::str("rb"));
    py::bytes data = f.read();
    f.close();
    return std::variant<py::bytes, py::str>(data);
}

std::variant<py::bytes, py::str> read_file(const py::str& filename, bool text) {
    py::str p = path_str(filename);
    if (text) {
        py::file f = py::open(p, py::str("r"));
        py::str data = f.read_text();
        f.close();
        return data;
    }
    py::file f = py::open(p, py::str("rb"));
    py::bytes data = f.read();
    f.close();
    return data;
}

void write_file(const py::str& filename, const std::variant<py::str, py::bytes, py::bytearray>& data) {
    py::str p = path_str(filename);
    if (std::holds_alternative<py::str>(data)) {
        py::file f = py::open(p, py::str("w"));
        f.write(std::get<py::str>(data));
        f.close();
    } else {
        py::file f = py::open(p, py::str("wb"));
        if (std::holds_alternative<py::bytes>(data)) f.write(std::get<py::bytes>(data));
        else f.write(std::get<py::bytearray>(data));
        f.close();
    }
}

std::optional<std::variant<py::bytes, py::bytearray>> try_read_sources(const py::list<Source>& sources) {
    for (const Source& source : sources) {
        if (std::holds_alternative<py::str>(source)) {
            // (a Path is a str here: Path.is_file() and then str(path) read the same file as the str does)
            const py::str& path = std::get<py::str>(source);
            if (path.strip() == py::str("")) continue;
            std::optional<std::variant<py::bytes, py::str>> data = read_file_or_none(path);
            if (data) return std::variant<py::bytes, py::bytearray>(std::get<py::bytes>(*data));
        } else if (std::holds_alternative<py::bytes>(source)) {
            if (!(std::get<py::bytes>(source) == py::bytes(""))) return std::variant<py::bytes, py::bytearray>(std::get<py::bytes>(source));
        } else {
            if (!(std::get<py::bytearray>(source) == py::bytes(""))) return std::variant<py::bytes, py::bytearray>(std::get<py::bytearray>(source));
        }
    }
    return std::nullopt;
}

// ------------------------------------------------------------------------------------ programs
namespace {

struct ShellRun {
    int64_t returncode = 0;
    py::bytes stdout_;
};

// subprocess.run(args, shell=True, stdout=DEVNULL | PIPE) on Windows: the command line is
// `%COMSPEC% /c "<list2cmdline(args)>"`, hidden, with the parent's stdin and stderr
ShellRun run_shell(const py::list<py::str>& args, bool capture_stdout) {
    std::optional<py::str> comspec = py::os::getenv(py::str("COMSPEC"));
    py::str cmd = (comspec ? *comspec : py::str("cmd.exe")) + py::str(" /c \"") + py::subprocess::list2cmdline(args) + py::str("\"");
    std::wstring wcmd = py::subprocess::to_wide(cmd);

    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr;
    std::vector<HANDLE> to_close;
    auto fail = [&](::DWORD code) {
        if (out_r) CloseHandle(out_r);
        for (HANDLE h : to_close) CloseHandle(h);
        py::raise_winerror(code);
    };
    if (capture_stdout) {
        if (!CreatePipe(&out_r, &out_w, &sa, 0)) fail(GetLastError());
        to_close.push_back(out_w);
        SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    } else {
        out_w = CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
        if (out_w == INVALID_HANDLE_VALUE) out_w = nullptr;
        else to_close.push_back(out_w);
    }
    HANDLE in_h = py::subprocess::detail_::dup_inheritable(GetStdHandle(STD_INPUT_HANDLE));
    if (in_h) to_close.push_back(in_h);
    HANDLE err_h = py::subprocess::detail_::dup_inheritable(GetStdHandle(STD_ERROR_HANDLE));
    if (err_h) to_close.push_back(err_h);

    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdInput = in_h;
    si.hStdOutput = out_w;
    si.hStdError = err_h;
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(nullptr, wcmd.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, nullptr, nullptr, &si, &pi);
    ::DWORD err = GetLastError();
    if (!ok) fail(err);
    CloseHandle(pi.hThread);
    for (HANDLE h : to_close) CloseHandle(h);
    to_close.clear();

    ShellRun r;
    if (out_r) {
        std::string raw;
        char buf[8192];
        ::DWORD got = 0;
        while (ReadFile(out_r, buf, sizeof buf, &got, nullptr) && got > 0) raw.append(buf, got);
        CloseHandle(out_r);
        r.stdout_ = py::bytes(raw);
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    ::DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    r.returncode = static_cast<int64_t>(code);
    return r;
}

}  // namespace

void assemble_patch(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines) {
    py::list<py::str> args{path_join(path_str(BIN_DIR()), py::str("xenon-as.exe")), py::str("-be"), py::str("-many"), py::str("-mregnames"),
                           asm_filename, py::str("-o"), py::str("temp.elf")};
    args.extend(py::list<py::str>{py::str("-I"), path_absolute(path_parent(asm_filename))});
    args.extend(py::list<py::str>{py::str("-I"), path_absolute(INCLUDE_DIR)});
    for (const py::str& x : defines) args.extend(py::list<py::str>{py::str("--defsym"), x + py::str("=1")});
    ShellRun result = run_shell(args, false);
    if (!(result.returncode == 0)) throw py::AssertionError("Patch assembly failed with error code " + std::to_string(result.returncode));

    args = py::list<py::str>{path_join(path_str(BIN_DIR()), py::str("xenon-objcopy.exe")), py::str("temp.elf"), py::str("-O"), py::str("binary"),
                             bin_filename};
    result = run_shell(args, false);
    if (!(result.returncode == 0)) throw py::AssertionError("ELF conversion failed with error code " + std::to_string(result.returncode));

    py::os::unlink(py::str("temp.elf"));
}

namespace {
// assemble_patch(asm_filename, bin_filename, <flags>, *defines)
void assemble_with(const py::str& asm_filename, const py::str& bin_filename, std::initializer_list<const char*> flags,
                   const py::list<py::str>& defines) {
    py::list<py::str> all;
    for (const char* f : flags) all.append(py::str(f));
    all.extend(defines);
    assemble_patch(asm_filename, bin_filename, all);
}
}  // namespace

void assemble_devkit_patch(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines) {
    assemble_with(asm_filename, bin_filename, {"DEVKIT"}, defines);
}

void assemble_retail_patch(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines) {
    assemble_with(asm_filename, bin_filename, {"RETAIL"}, defines);
}

void assemble_rgl_flash(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines) {
    assemble_with(asm_filename, bin_filename, {"FLASH"}, defines);
}

void assemble_rgl_vfuses_flash(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines) {
    assemble_with(asm_filename, bin_filename, {"VFUSES", "FLASH"}, defines);
}

void assemble_rgl_hdd(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines) {
    assemble_with(asm_filename, bin_filename, {"HDD"}, defines);
}

void assemble_rgl_vfuses_hdd(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines) {
    assemble_with(asm_filename, bin_filename, {"VFUSES", "HDD"}, defines);
}

std::tuple<int64_t, py::str> run_command(const py::str& path, const py::list<py::str>& args) {
    py::str ep = path_str(path);   // executable path
    py::list<py::str> a{path_absolute(ep)};
    a.extend(args);
    ShellRun result = run_shell(a, true);
    return {result.returncode, result.stdout_.decode("utf-8", "ignore")};
}

// ------------------------------------------------------------------------------ in-place bootloader work
namespace {

// stream.read(size): an open file refuses a negative size but -1 (ValueError), an io.BytesIO reads on to the
// end for any negative size. (py::file takes every negative size for "to the end".)
template <class S> py::bytes read_(S& stream, int64_t size) {
    if constexpr (std::is_same<S, py::file>::value) {
        if (size < -1) throw py::ValueError("read length must be non-negative or -1");
    }
    return stream.read(size);
}

}  // namespace

template <class S> int64_t get_bldr_size_in_place(S& stream, int64_t offset) {
    stream.seek(offset);
    py::bytes magic = read_(stream, 2);
    if (!(magic == py::bytes("SB") || magic == py::bytes("SC") || magic == py::bytes("SD") || magic == py::bytes("SE"))) {
        throw py::AssertionError("Invalid bootloader magic!");
    }
    stream.seek(10, StreamIO::SEEK_CUR_);
    int64_t size = from_bytes_big(read_(stream, 4));
    size -= 0x20;
    size += calc_bldr_pad_size(size);
    return size;
}

template <class S> int64_t patch_in_place(S& stream, const BinLike& patches) {
    int64_t c = 0;
    int64_t loc = stream.tell();
    {
        StreamIO_ pio(patches, Endian::BIG);
        while (true) {
            int64_t addr = pio.read_uint32();
            if (addr == 0xFFFFFFFF) break;
            int64_t size = pio.read_uint32();
            size *= 4;
            py::bytes pdata = pio.read(size);
            stream.seek(addr);
            py::bytes tdata = read_(stream, size);
            if (pdata == tdata) continue;
            stream.seek(addr);
            stream.write(pdata);
            c += 1;
        }
    }
    stream.seek(loc);
    return c;
}

template <class S> void encrypt_bldr_in_place(const py::bytes& key, S& stream, int64_t offset) {
    int64_t loc = stream.tell();
    int64_t size = get_bldr_size_in_place(stream, offset);

    // skip nonce
    stream.seek(16, StreamIO::SEEK_CUR_);
    // read data
    py::bytes data = read_(stream, size);
    // encrypt data and padding
    py::bytearray enc = encrypt_bl(key, data, false);
    // write the data back
    stream.seek(offset + 0x20);
    stream.write(enc);
    stream.seek(loc);
}

template <class S> py::bytes calc_se_hash_in_place(S& stream, int64_t offset) {
    int64_t loc = stream.tell();
    int64_t size = get_bldr_size_in_place(stream, offset);

    // create hash
    py::bytes h_data;
    {
        StreamIO_ bio;
        stream.seek(offset);
        bio.write(read_(stream, 0x10));
        stream.seek(offset + 0x20);
        bio.write(read_(stream, size));
        h_data = bio.getvalue();
    }
    py::bytes h = XeCrypt::XeCryptRotSumSha(h_data);

    stream.seek(loc);
    return h;
}

template <class S> void sign_bldr_in_place(S& stream, int64_t offset, XeCrypt::XeCryptRsaKey& key) {
    int64_t loc = stream.tell();
    int64_t size = get_bldr_size_in_place(stream, offset);

    // create hash
    py::bytes h_data;
    {
        StreamIO_ bio;
        stream.seek(offset);
        bio.write(read_(stream, 0x10));
        stream.seek(offset + 0x120);
        bio.write(read_(stream, size - 0x100));
        h_data = bio.getvalue();
    }
    py::bytes h = XeCrypt::XeCryptRotSumSha(h_data);

    // create signature
    py::bytes sig = key.sig_create(h, XeCrypt::XECRYPT_SD_SALT);

    // write the signature
    stream.seek(offset + 0x20);
    stream.write(sig);

    stream.seek(loc);
}

// the streams the callers use: a memory stream and a file
template int64_t get_bldr_size_in_place<StreamIO_>(StreamIO_&, int64_t);
template int64_t get_bldr_size_in_place<py::file>(py::file&, int64_t);
template int64_t patch_in_place<StreamIO_>(StreamIO_&, const BinLike&);
template int64_t patch_in_place<py::file>(py::file&, const BinLike&);
template void encrypt_bldr_in_place<StreamIO_>(const py::bytes&, StreamIO_&, int64_t);
template void encrypt_bldr_in_place<py::file>(const py::bytes&, py::file&, int64_t);
template py::bytes calc_se_hash_in_place<StreamIO_>(StreamIO_&, int64_t);
template py::bytes calc_se_hash_in_place<py::file>(py::file&, int64_t);
template void sign_bldr_in_place<StreamIO_>(StreamIO_&, int64_t, XeCrypt::XeCryptRsaKey&);
template void sign_bldr_in_place<py::file>(py::file&, int64_t, XeCrypt::XeCryptRsaKey&);

// ------------------------------------------------------------------------------------- C functions
py::bytes decompress_se(const py::bytes& data_, bool skip_header) {
    py::bytes data = data_;
    if (skip_header) {
        data = data.slice_from(0x30);
    }
    // with LZXDecompression() as lzx: return lzx.decompress_continuous(data)
    lzx::LZXDecompression lzxd;
    lzxd.__enter__();
    py::bytes result;
    try {
        result = lzxd.decompress_continuous(data);
    } catch (...) {
        lzxd.__exit__();   // (its own failure replaces the error)
        throw;
    }
    lzxd.__exit__();
    return result;
}

py::bytes compress_se(const py::bytes& data_, bool include_header) {
    // with LZXCompression() as lzx:
    lzx::LZXCompression lzxc;
    lzxc.__enter__();
    py::bytes result;
    try {
        py::bytes data = lzxc.compress_continuous(data_);
        if (include_header) {
            py::bytearray hdr_data = py::bytearray(py::bytes::zeros(0x30) + data);
            std::string& raw = hdr_data.raw_mut();
            std::string be;
            put32(be, static_cast<int64_t>(raw.size()));   // pack_into(">I", data, 0xC, len(data))
            raw.replace(0xC, 4, be);
            be.clear();
            put32(be, 0x280000);   // pack_into(">I", data, 0x28, 0x280000)
            raw.replace(0x28, 4, be);
            result = py::bytes(raw);
        } else {
            result = data;
        }
    } catch (...) {
        lzxc.__exit__();
        throw;
    }
    lzxc.__exit__();
    return result;
}

namespace {

// the signing hash of a 4BL: RotSumSha(data[:0x10] + data[0x120:])
py::bytes sd_4bl_hash(const py::bytes& data) { return XeCrypt::XeCryptRotSumSha(data.slice_to(0x10) + data.slice_from(0x120)); }

py::bytearray sign_sd_4bl_impl(XeCrypt::XeCryptRsaKey& key, const py::bytes& salt, const py::bytearray& data) {
    py::bytes h = sd_4bl_hash(data.view());
    py::bytes sig = key.sig_create(h, salt);
    data.set_slice(0x20, 0x20 + sig.size(), sig);
    return data;
}

}  // namespace

py::bytearray sign_sd_4bl(XeCrypt::XeCryptRsaKey& key, const py::bytes& salt, const py::bytes& data) {
    return sign_sd_4bl_impl(key, salt, py::bytearray(data));
}

py::bytearray sign_sd_4bl(XeCrypt::XeCryptRsaKey& key, const py::bytes& salt, const py::bytearray& data) {
    return sign_sd_4bl_impl(key, salt, data);
}

py::bytearray sign_sd_4bl(const py::bytes& key, const py::bytes& salt, const py::bytes& data) {
    XeCrypt::XeCryptRsaKey k(key);
    return sign_sd_4bl_impl(k, salt, py::bytearray(data));
}

py::bytearray sign_sd_4bl(const py::bytes& key, const py::bytes& salt, const py::bytearray& data) {
    XeCrypt::XeCryptRsaKey k(key);
    return sign_sd_4bl_impl(k, salt, data);
}

bool verify_sd_4bl(XeCrypt::XeCryptRsaKey& key, const py::bytes& salt, const py::bytes& data) {
    py::bytes sig = data.slice(0x20, 0x20 + 256);
    py::bytes h = sd_4bl_hash(data);
    return key.sig_verify(sig, h, salt);
}

bool verify_sd_4bl(const py::bytes& key, const py::bytes& salt, const py::bytes& data) {
    XeCrypt::XeCryptRsaKey k(key);
    return verify_sd_4bl(k, salt, data);
}

py::bytearray encrypt_bl(const py::bytes& key, const BinLike& data, bool skip_header) {
    StreamIO_ bio(data);
    if (skip_header) {
        bio.seek(0x20);   // skip header and nonce
    }
    py::bytes bl_data_enc = XeCrypt::XeCryptRc4::new_(key).encrypt(bio.read());   // read all of the remaining data and encrypt it
    if (skip_header) {
        bio.seek(0x20);   // write the encrypted data back
    } else {
        bio.seek(0);
    }
    bio.write(bl_data_enc);
    return py::bytearray(bio.getvalue());
}

py::bytearray apply_jump_sd_4bl(const py::bytes& data, int64_t size) {
    (void)size;
    StreamIO_ sio(data, Endian::BIG);
    while (true) {
        if (sio.read_uint32() == 0x4C000024) {
            // sio.offset -= 4: the Python's StreamIO has no `offset`. What follows in the Python --
            //   dist = (size & ~0x80000000) - (sio.offset & ~0x80000000)
            //   ret = 0x48000000 | (dist & 0x3FFFFFC)
            //   sio.write_uint32(ret); break
            // -- is not reachable.
            throw py::AttributeError("'StreamIO' object has no attribute 'offset'");
        }
    }
}

py::bytearray apply_patches(const py::bytes& bl_data, const std::variant<py::str, py::bytes, py::bytearray>& patch_data) {
    StreamIO_ blsio(bl_data, Endian::BIG);
    StreamIO_ psio = std::holds_alternative<py::str>(patch_data)     ? StreamIO_(std::get<py::str>(patch_data), Endian::BIG)
                     : std::holds_alternative<py::bytes>(patch_data) ? StreamIO_(std::get<py::bytes>(patch_data), Endian::BIG)
                                                                     : StreamIO_(std::get<py::bytearray>(patch_data), Endian::BIG);
    while (true) {
        int64_t addr = psio.read_uint32();
        if (addr == 0xFFFFFFFF) break;
        int64_t size = psio.read_uint32();
        py::bytes patch = psio.read_ubytes(size * 4);
        blsio.write_ubytes_at(addr, patch);
    }
    return py::bytearray(blsio.getvalue());
}

int64_t calc_patch_size(const py::bytes& patch_data, int64_t size) {
    int64_t patch_size = 0;
    StreamIO_ psio(patch_data, Endian::BIG);
    while (true) {
        int64_t addr = psio.read_uint32();
        if (addr == 0xFFFFFFFF) break;
        int64_t ps = psio.read_uint32();
        psio.seek(ps * 4, StreamIO::SEEK_CUR_);
        if (addr >= size) {
            patch_size += (ps * 4);
        }
    }
    return patch_size;
}

int64_t calc_pad_size(int64_t size, int64_t bounds) {
    // (bounds - (size % bounds)) % bounds
    if (bounds == 0) throw py::ZeroDivisionError("division by zero");
    return py::mod(bounds - py::mod(size, bounds), bounds);
}

int64_t calc_bldr_pad_size(int64_t size) { return ((size + 0xF) & ~static_cast<int64_t>(0xF)) - size; }

py::list<py::str> __all__() {
    return py::list<py::str>{
        // enums
        py::str("BLMagic"),

        // classes
        py::str("BLHeader"),

        // structures
        py::str("FLASH_HEADER"), py::str("BLDR_HEADER"), py::str("SB_2BL_HEADER"), py::str("SC_3BL_HEADER"), py::str("SD_4BL_HEADER"),
        py::str("SE_5BL_HEADER"), py::str("HV_HEADER"),

        // functions
        py::str("read_file_or_none"), py::str("read_file"), py::str("write_file"), py::str("try_read_sources"), py::str("assemble_patch"),
        py::str("assemble_devkit_patch"), py::str("assemble_retail_patch"), py::str("assemble_rgl_flash"), py::str("assemble_rgl_vfuses_flash"),
        py::str("assemble_rgl_hdd"), py::str("assemble_rgl_vfuses_hdd"), py::str("run_command"),

        py::str("get_bldr_size_in_place"), py::str("patch_in_place"), py::str("encrypt_bldr_in_place"), py::str("calc_se_hash_in_place"),
        py::str("sign_bldr_in_place"),

        py::str("decompress_se"), py::str("compress_se"), py::str("sign_sd_4bl"), py::str("verify_sd_4bl"), py::str("encrypt_bl"),
        py::str("apply_jump_sd_4bl"), py::str("apply_patches"), py::str("calc_patch_size"), py::str("calc_pad_size"), py::str("calc_bldr_pad_size")};
}

}  // namespace codebreaker::formats::xbox::crypto::build_lib
