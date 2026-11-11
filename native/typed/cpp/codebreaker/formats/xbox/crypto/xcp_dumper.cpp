// codebreaker/formats/xbox/crypto/xcp_dumper.cpp -- C++ port of codebreaker.formats.xbox.crypto.xcp_dumper.
//
// (The Python imports ceil, BytesIO, Union, unpack_from, Popen, TemporaryDirectory, compressobj and a few
// other names; those it uses are the runtime's, the rest are left out. `compressobj` and `pack` are unused.)

#include "codebreaker/formats/xbox/crypto/xcp_dumper.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <map>
#include <vector>

#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::formats::xbox::crypto::xcp_dumper {

using StreamIO::Endian;
using StreamIO_ = StreamIO::StreamIO;
using xell_unpack::ZlibError;

// constants
const py::str CACHE_DIR("cache");
const py::str TMP_CAB_FILE("tmp.cab");

// ------------------------------------------------------------------------------------------- Path
namespace {

bool is_sep(char32_t c) { return c == U'\\' || c == U'/'; }

// the pieces of a Windows path: drive, root, and the names (no empty names, no ".")
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

Path join_parts(const PathParts& p) {
    std::u32string out = p.drive + p.root;
    for (size_t k = 0; k < p.names.size(); k++) {
        if (k) out += U'\\';
        out += p.names[k];
    }
    if (out.empty()) return py::str(".");
    return py::str(out);
}

// Path.stem
py::str path_stem(const Path& p) {
    PathParts parts = split_path(p);
    if (parts.names.empty()) return py::str("");
    const std::u32string& name = parts.names.back();
    size_t dot = name.rfind(U'.');
    if (dot != std::u32string::npos && dot > 0 && dot + 1 < name.size()) return py::str(name.substr(0, dot));
    return py::str(name);
}

// Path.parents[0]
Path path_parent(const Path& p) {
    PathParts parts = split_path(p);
    if (parts.names.empty()) throw py::IndexError("0");
    parts.names.pop_back();
    return join_parts(parts);
}

// p / name
Path path_join(const Path& p, const py::str& name) {
    PathParts parts = split_path(p);
    for (const std::u32string& n : split_path(name).names) parts.names.push_back(n);
    return join_parts(parts);
}

// Path.mkdir(parents=False, exist_ok=False): FileExistsError if the name is taken, FileNotFoundError if the
// parent is missing
void path_mkdir(const Path& path, bool exist_ok = false) {
    std::error_code ec;
    std::filesystem::path p = std::filesystem::u8path(path.utf8());
    if (std::filesystem::exists(p, ec)) {
        if (exist_ok && std::filesystem::is_directory(p, ec)) return;
        throw py::FileExistsError("[WinError 183] Cannot create a file when that file already exists: " + py::repr(path).utf8());
    }
    std::filesystem::create_directory(p, ec);
    if (ec) py::os::raise_os_(ec, path);
}

// Path.mkdir(parents=True, exist_ok=True)
void path_makedirs(const Path& path) { py::os::makedirs(path, 0777, true); }

// input(prompt): the prompt on stdout, then a line of stdin; EOFError at the end of the input
py::str input_(const py::str& prompt) {
    std::string out = prompt.utf8();
    std::fwrite(out.data(), 1, out.size(), stdout);
    std::fflush(stdout);
    std::string line;
    int c;
    bool any = false;
    while ((c = std::fgetc(stdin)) != EOF) {
        any = true;
        if (c == '\n') break;
        line.push_back(static_cast<char>(c));
    }
    if (!any) throw py::EOFError("EOF when reading a line");
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return py::str(line);
}

// Python's AttributeError for a call of what the Python's StreamIO does not have
[[noreturn]] void no_stream_attribute(const char* name) {
    throw py::AttributeError(std::string("'StreamIO' object has no attribute '") + name + "'");
}

// xcp.perform_function_at(offset, size, function)
py::bytes perform_function_at(StreamIO_&, int64_t, int64_t, XeCrypt::XeCryptRc4&) { no_stream_attribute("perform_function_at"); }
void set_label(StreamIO_&, const py::str&) { no_stream_attribute("set_label"); }
int64_t get_label(StreamIO_&, const py::str&) { no_stream_attribute("get_label"); }

// bytes.fromhex(s) as Python 3.14 reads it: blanks between pairs, ValueError (with the position) otherwise
py::bytes fromhex(const py::str& s) {
    const std::u32string& u = s.u32();
    auto digit = [](char32_t c) -> int {
        if (c >= U'0' && c <= U'9') return static_cast<int>(c - U'0');
        if (c >= U'a' && c <= U'f') return static_cast<int>(c - U'a' + 10);
        if (c >= U'A' && c <= U'F') return static_cast<int>(c - U'A' + 10);
        return 99;
    };
    auto space = [](char32_t c) { return c == U' ' || c == U'\t' || c == U'\n' || c == U'\v' || c == U'\f' || c == U'\r'; };
    std::string out;
    size_t i = 0;
    auto bad = [&](size_t at) {
        return py::ValueError("non-hexadecimal number found in fromhex() arg at position " + std::to_string(at));
    };
    while (i < u.size()) {
        if (space(u[i])) {
            while (i < u.size() && space(u[i])) i++;
            if (i >= u.size()) break;
        }
        int top = digit(u[i]);
        if (top >= 16) throw bad(i);
        i++;
        if (i >= u.size()) throw py::ValueError("fromhex() arg must contain an even number of hexadecimal digits");
        int bot = digit(u[i]);
        if (bot >= 16) throw bad(i);
        i++;
        out.push_back(static_cast<char>(top * 16 + bot));
    }
    return py::bytes(out);
}

// the key of HMAC(key, SHA1()), which cryptography takes as a buffer: None and str are a TypeError
using Key = std::variant<std::monostate, py::str, py::bytes>;   // args.key: None, as typed, as converted
py::bytes hmac_sha(const Key& key, const py::bytes& data) {
    if (std::holds_alternative<py::bytes>(key)) return XeCrypt::XeCryptHmacSha(std::get<py::bytes>(key), py::list<py::bytes>{data});
    std::string msg = "argument 'key': Cannot convert \"<class '";
    msg += std::holds_alternative<py::str>(key) ? "str" : "NoneType";
    msg += "'>\" instance to a buffer.";
    if (std::holds_alternative<py::str>(key)) msg += "\nDid you mean to pass a bytestring instead?";
    throw py::TypeError(msg);
}

}  // namespace

Path path_type(const py::str& s) { return join_parts(split_path(s)); }

bool extract_cab(const py::str& filename, const py::str& path) {
#ifdef _WIN32
    const char* name = "nt";
#else
    const char* name = "posix";
#endif
    py::list<py::str> args;
    if (std::string(name) == "nt") {
        args = py::list<py::str>{py::str("expand"), filename, py::str("-F:*"), path};
    } else if (std::string(name) == "posix") {
        args = py::list<py::str>{py::str("cabextract"), py::str("-d"), path, py::str("-F"), py::str("*"), filename};
    } else {
        // raise NotImplemented(...): NotImplemented is not callable
        throw py::TypeError("'NotImplementedType' object is not callable");
    }
    py::subprocess::RunOptions opts;
    opts.stdout_ = py::subprocess::Stream::Capture;
    opts.stderr_ = py::subprocess::Stream::Capture;
    opts.text = false;
    py::subprocess::CompletedProcess p = py::subprocess::run(args, opts);
    return p.returncode == 0;
}

py::bytes stream_decrypt_with_struct(StreamIO_& xcp, const py::bytes& key, int64_t struct_offset, int64_t data_offset, int64_t size) {
    // xcp.offset = struct_offset  (only an attribute in the Python: the read below is at the stream's position)
    (void)struct_offset;
    RC4_SHA_HEADER rc4_sha_struct = xcp.read_struct<RC4_SHA_HEADER>();
    py::bytes cksm(std::string(reinterpret_cast<const char*>(rc4_sha_struct.cksm), sizeof rc4_sha_struct.cksm));
    py::bytes confounder(std::string(reinterpret_cast<const char*>(rc4_sha_struct.confounder), sizeof rc4_sha_struct.confounder));
    XeCrypt::XeCryptRc4 cipher = XeCrypt::XeCryptRc4::new_(XeCrypt::XeCryptHmacSha(key, py::list<py::bytes>{cksm}));
    cipher.decrypt(confounder);

    py::bytes dec_data = perform_function_at(xcp, data_offset, size, cipher);

    return dec_data;
}

// ----------------------------------------------------------------------------------------- XCPFile
XCPFile::XCPFile(const py::str& path) {
    handle = py::open(path, py::str("rb"));
    handle->seek(0, 2);
    total_size = handle->tell();
    handle->seek(0);
}

XCPFile& XCPFile::__enter__() { return *this; }

void XCPFile::__exit__() {
    if (handle) handle->close();
}

Path XCPFile::create_file_struct(const py::str& out_path, const py::bytes& hdr) {
    py::str svod_name = hdr.slice(0x32C, 0x32C + (34 / 2)).hex().upper();
    py::str title_id = hdr.slice(0x360, 0x360 + 4).hex().upper();

    Path p = path_type(out_path);
    p = path_join(p, title_id);
    p = path_join(p, py::str("00007000"));
    // create content path
    path_makedirs(p);
    p = path_join(p, svod_name);

    // write header file
    {
        py::file f = py::open(p, py::str("wb"));
        f.write(hdr);
        f.close();
    }

    p = path_parent(p);
    p = path_join(p, svod_name + py::str(".data"));
    // create folder for data files
    path_mkdir(p, true);

    return p;
}

int64_t XCPFile::calc_buffer_len(StreamIO_& stream, const py::bytes& data) { return stream.tell() + data.size(); }
int64_t XCPFile::calc_buffer_len(py::file& stream, const py::bytes& data) { return stream.tell() + data.size(); }
int64_t XCPFile::calc_bytes_left(StreamIO_& stream, int64_t size) { return size - stream.tell(); }
int64_t XCPFile::calc_bytes_left(py::file& stream, int64_t size) { return size - stream.tell(); }

py::bytes XCPFile::get_chunk(const py::bytes& data, int64_t offset, int64_t size) {
    if (size == 0) return data.slice_from(offset);
    return data.slice(offset, offset + size);
}

int64_t XCPFile::calc_data_file_num_from_offset(int64_t offset) {
    if (offset <= SVOD_INDEX_SIZE) return -1;   // index file
    offset -= SVOD_INDEX_SIZE;
    return static_cast<int64_t>(std::ceil(static_cast<double>(offset) / static_cast<double>(SVOD_DATA_SIZE)));
}

void XCPFile::extract(const py::str& out_path) {
    // variables
    int64_t dfn = 0;   // data file number
    py::bytes buf;
    py::bytes idx;
    int64_t data_written = 0;
    bool idx_done = false;

    // handles
    std::optional<Path> pd;
    std::optional<py::file> f;
    auto data_file_name = [&]() { return path_join(*pd, py::str("Data") + py::format(dfn, "04")); };
    {
        StreamIO_ bio;   // holds the index
        while (handle->tell() < total_size || buf.size() > 0) {
            Decompress dec;
            while (true) {
                if (buf.empty()) {   // no remainder buffer, so fetch more data
                    buf = handle->read(SVOD_BLOCK_SIZE);
                    if (buf.empty()) {   // eof
                        if (f) f->close();
                        py::print(py::str("Done!"));
                        break;
                    }
                    buf = dec.decompress(buf);
                    if (buf.empty()) {
                        handle->seek(-dec.unused_data.size(), 1);
                        buf = dec.flush();
                        dec = Decompress();
                    }
                }

                if (!idx_done) {   // index file
                    int64_t total = calc_buffer_len(bio, buf);
                    if (total < SVOD_INDEX_SIZE) {   // <, all of the buffer is consumed
                        bio.write(buf);
                        buf = py::bytes();
                    } else if (total > SVOD_INDEX_SIZE) {   // >, part of the buffer is consumed, and the file is complete
                        int64_t bl = calc_bytes_left(bio, SVOD_INDEX_SIZE);

                        bio.write(get_chunk(buf, 0, bl));

                        // carry remainder to data files
                        buf = get_chunk(buf, bl);

                        idx = bio.getvalue();
                        pd = create_file_struct(out_path, idx);
                        idx_done = true;

                        // open the new file
                        f = py::open(data_file_name(), py::str("wb"));
                    } else if (total == SVOD_INDEX_SIZE) {   // ==, all of the buffer is consumed, and the file is complete
                        bio.write(buf);
                        buf = py::bytes();

                        idx = bio.getvalue();
                        pd = create_file_struct(out_path, idx);
                        idx_done = true;

                        // open the new file
                        f = py::open(data_file_name(), py::str("wb"));
                    }
                    // done with header, start on data files
                    continue;
                }

                int64_t total = calc_buffer_len(*f, buf);
                if (total < SVOD_DATA_SIZE) {   // <, same as above
                    data_written += f->write(buf);
                    buf = py::bytes();
                } else if (total > SVOD_DATA_SIZE) {   // >, same as above
                    int64_t bl = calc_bytes_left(*f, SVOD_DATA_SIZE);

                    data_written += f->write(get_chunk(buf, 0, bl));

                    // carry remainder to next data file
                    buf = get_chunk(buf, bl);

                    // close the file
                    f->close();

                    dfn += 1;

                    // open the new file
                    f = py::open(data_file_name(), py::str("wb"));
                } else if (total == SVOD_DATA_SIZE) {   // ==, same as above
                    data_written += f->write(buf);
                    buf = py::bytes();

                    // close the file
                    f->close();

                    dfn += 1;

                    // open the new file
                    f = py::open(data_file_name(), py::str("wb"));
                }
            }
        }
    }

    // db_cnt = int.from_bytes(idx[0x392:0x392 + 3], "big")
    // db_offs = int.from_bytes(idx[0x395:0x395 + 3], "big")

    // (df_cnt, df_size) = unpack_from(">iq", idx, 0x39D)
    const int64_t at = 0x39D;
    if (idx.size() < at + 12) {
        throw py::StructError("unpack_from requires a buffer of at least " + std::to_string(at + 12) + " bytes for unpacking 12 bytes at offset " +
                              std::to_string(at) + " (actual buffer size is " + std::to_string(idx.size()) + ")");
    }
    uint32_t cnt_raw = 0;
    uint64_t size_raw = 0;
    for (int k = 0; k < 4; k++) cnt_raw = (cnt_raw << 8) | static_cast<uint32_t>(idx.byte_(at + k));
    for (int k = 0; k < 8; k++) size_raw = (size_raw << 8) | static_cast<uint64_t>(idx.byte_(at + 4 + k));
    int64_t df_cnt = static_cast<int32_t>(cnt_raw);
    int64_t df_size = static_cast<int64_t>(size_raw);
    if (!((dfn + 1) == df_cnt)) throw py::AssertionError("Not enough data files generated!");
    if (!(data_written == df_size)) throw py::AssertionError("Not enough data decompressed!");
}

// ------------------------------------------------------------------------------------------ argparse
namespace {

// argparse for this one parser: the positional input, and the options -h/--help, -k/--key KEY, --ignore and
// --no-backup (the algorithm of Python 3.14's argparse.ArgumentParser._parse_known_args for these actions)

enum class Kind { Help, Key, Ignore, NoBackup, Input };

struct OptionString {
    const char* text;
    Kind kind;
};
// the parser's _option_string_actions, in the order they were added
const OptionString OPTION_STRINGS[] = {{"-h", Kind::Help}, {"--help", Kind::Help}, {"-k", Kind::Key}, {"--key", Kind::Key},
                                       {"--ignore", Kind::Ignore}, {"--no-backup", Kind::NoBackup}};

// argparse prints this for -h when it cannot ask the terminal its width (80 columns)
const char* const HELP_TEXT =
    "\n"
    "A script to decrypt, merge, and convert XCP files for the Xbox 360\n"
    "\n"
    "positional arguments:\n"
    "  input          The file to extract\n"
    "\n"
    "options:\n"
    "  -h, --help     show this help message and exit\n"
    "  -k, --key KEY  The key used to decrypt the XCP package if it's encrypted\n"
    "  --ignore       Ignore the file overwrite warning\n"
    "  --no-backup    Disable backups\n";

// argparse.ArgumentError, as str(err) reads: "argument -k/--key: expected one argument"
struct ArgumentError {
    py::str message;
};

const char* action_name(Kind kind) {
    switch (kind) {
        case Kind::Help: return "-h/--help";
        case Kind::Key: return "-k/--key";
        case Kind::Ignore: return "--ignore";
        case Kind::NoBackup: return "--no-backup";
        default: return "input";
    }
}

ArgumentError argument_error(std::optional<Kind> action, const py::str& message) {
    if (!action) return ArgumentError{message};
    return ArgumentError{py::str("argument ") + py::str(action_name(*action)) + py::str(": ") + message};
}

void write_to(std::FILE* stream, const py::str& text) {
    std::string u = text.utf8();
    std::fwrite(u.data(), 1, u.size(), stream);
    std::fflush(stream);
}

py::str usage(const py::str& prog) { return py::str("usage: ") + prog + py::str(" [-h] [-k KEY] [--ignore] [--no-backup] input\n"); }

// parser.error(message): the usage and the message on stderr, exit code 2
[[noreturn]] void fail(const py::str& prog, const py::str& message) {
    write_to(stderr, usage(prog));
    write_to(stderr, prog + py::str(": error: ") + message + py::str("\n"));
    throw py::SystemExit(2);
}

struct OptionTuple {
    std::optional<Kind> action;   // None: an option the parser does not have
    py::str option_string;
    std::optional<py::str> sep;
    std::optional<py::str> explicit_arg;
};

std::optional<Kind> kind_of(const py::str& option_string) {
    for (const OptionString& o : OPTION_STRINGS)
        if (option_string == py::str(o.text)) return o.kind;
    return std::nullopt;
}

// str.partition("="): (before, sep, after) with sep None when there is no "="
struct Partition {
    py::str before;
    std::optional<py::str> sep;
    py::str after;
};
Partition partition(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t eq = u.find(U'=');
    if (eq == std::u32string::npos) return {s, std::nullopt, py::str()};
    return {py::str(u.substr(0, eq)), py::str("="), py::str(u.substr(eq + 1))};
}

// ArgumentParser._get_option_tuples (allow_abbrev is on)
std::vector<OptionTuple> get_option_tuples(const py::str& option_string) {
    std::vector<OptionTuple> result;
    const std::u32string& u = option_string.u32();
    if (u[0] == U'-' && u[1] == U'-') {
        // option strings starting with two prefix characters are only split at the '='
        Partition p = partition(option_string);
        for (const OptionString& o : OPTION_STRINGS) {
            if (py::str(o.text).startswith(p.before)) {
                result.push_back({o.kind, py::str(o.text), p.sep, p.sep ? std::optional<py::str>(p.after) : std::nullopt});
            }
        }
    } else {
        // single character options can be concatenated with their arguments
        Partition p = partition(option_string);
        py::str short_option_prefix(u.substr(0, 2));
        py::str short_explicit_arg(u.substr(2));
        for (const OptionString& o : OPTION_STRINGS) {
            if (py::str(o.text) == short_option_prefix) {
                result.push_back({o.kind, py::str(o.text), py::str(""), short_explicit_arg});
            } else if (py::str(o.text).startswith(p.before)) {
                result.push_back({o.kind, py::str(o.text), p.sep, p.sep ? std::optional<py::str>(p.after) : std::nullopt});
            }
        }
    }
    return result;
}

// ArgumentParser._parse_optional: None (a positional) or the option tuples
std::optional<std::vector<OptionTuple>> parse_optional(const py::str& arg_string) {
    if (arg_string.empty()) return std::nullopt;
    const std::u32string& u = arg_string.u32();
    if (u[0] != U'-') return std::nullopt;
    if (auto kind = kind_of(arg_string)) return std::vector<OptionTuple>{{kind, arg_string, std::nullopt, std::nullopt}};
    if (u.size() == 1) return std::nullopt;
    Partition p = partition(arg_string);
    if (p.sep) {
        if (auto kind = kind_of(p.before)) return std::vector<OptionTuple>{{kind, p.before, p.sep, p.after}};
    }
    std::vector<OptionTuple> option_tuples = get_option_tuples(arg_string);
    if (!option_tuples.empty()) return option_tuples;
    // a negative number is a positional (the parser has no options that look like one)
    if (py::re::match(py::str("-\\.?\\d"), arg_string)) return std::nullopt;
    if (arg_string.contains(py::str(" "))) return std::nullopt;
    return std::vector<OptionTuple>{{std::nullopt, arg_string, std::nullopt, std::nullopt}};
}

// ArgumentParser._get_nargs_pattern: a flag takes none, an option's value is one 'A'; a positional's is -*A-*
const char* nargs_pattern(Kind kind) {
    switch (kind) {
        case Kind::Help:
        case Kind::Ignore:
        case Kind::NoBackup: return "([AO]{0})";
        case Kind::Key: return "([A])";
        default: return "(-*A-*)";
    }
}

// ArgumentParser._match_argument: how many pattern characters the option takes (ArgumentError if it takes none it needs)
int64_t match_argument(Kind action, const std::string& arg_strings_pattern) {
    auto m = py::re::match(py::str(nargs_pattern(action)), py::str(arg_strings_pattern));
    if (!m) throw argument_error(action, py::str("expected one argument"));
    return m->end(1) - m->start(1);
}

// ArgumentParser._match_arguments_partial: how many arg strings each leading positional takes
std::vector<int64_t> match_arguments_partial(const std::vector<Kind>& actions, const std::string& arg_strings_pattern) {
    for (int64_t i = static_cast<int64_t>(actions.size()); i > 0; i--) {
        std::string pattern;
        for (int64_t k = 0; k < i; k++) pattern += nargs_pattern(actions[static_cast<size_t>(k)]);
        auto m = py::re::match(py::str(pattern), py::str(arg_strings_pattern));
        if (m) {
            std::vector<int64_t> result;
            for (int64_t g = 1; g <= i; g++) result.push_back(m->end(g) - m->start(g));
            if (m->end(0) < static_cast<int64_t>(arg_strings_pattern.size()) && arg_strings_pattern[static_cast<size_t>(m->end(0))] == 'O') {
                while (!result.empty() && result.back() == 0) result.pop_back();
            }
            return result;
        }
    }
    return {};
}

struct Namespace {
    Path input;
    std::optional<py::str> key;
    bool ignore = false;
    bool no_backup = false;
};

// ArgumentParser._parse_known_args (intermixed off), then parse_args' check of the extras
Namespace parse_args(const py::list<py::str>& argv, const py::str& prog) {
    std::vector<py::str> arg_strings(argv.vec().begin(), argv.vec().end());
    Namespace ns;

    // find all option indices, and determine the arg_string_pattern: an 'O' for an option, an 'A' for an
    // argument, a '-' for the first '--'
    std::map<int64_t, std::vector<OptionTuple>> option_string_indices;
    std::string arg_strings_pattern;
    for (size_t i = 0; i < arg_strings.size(); i++) {
        if (arg_strings[i] == py::str("--")) {   // all args after -- are non-options
            arg_strings_pattern += '-';
            for (size_t k = i + 1; k < arg_strings.size(); k++) arg_strings_pattern += 'A';
            break;
        }
        std::optional<std::vector<OptionTuple>> option_tuples = parse_optional(arg_strings[i]);
        if (!option_tuples) {
            arg_strings_pattern += 'A';
        } else {
            option_string_indices[static_cast<int64_t>(i)] = *option_tuples;
            arg_strings_pattern += 'O';
        }
    }

    std::vector<Kind> seen_actions;
    std::vector<py::str> extras;
    std::vector<Kind> positionals{Kind::Input};

    auto take_action = [&](Kind action, const std::vector<py::str>& argument_strings) {
        seen_actions.push_back(action);
        switch (action) {
            case Kind::Help:
                write_to(stdout, usage(prog) + py::str(HELP_TEXT));
                throw py::SystemExit(0);
            case Kind::Key: ns.key = argument_strings[0]; break;
            case Kind::Ignore: ns.ignore = true; break;
            case Kind::NoBackup: ns.no_backup = true; break;
            case Kind::Input: ns.input = path_type(argument_strings[0]); break;
        }
    };

    // function to convert arg_strings into an optional action
    auto consume_optional = [&](int64_t start_index) -> int64_t {
        const std::vector<OptionTuple>& option_tuples = option_string_indices[start_index];
        // if multiple actions match, the option string was ambiguous
        if (option_tuples.size() > 1) {
            py::list<py::str> names;
            for (const OptionTuple& t : option_tuples) names.append(t.option_string);
            throw ArgumentError{py::str("ambiguous option: ") + arg_strings[static_cast<size_t>(start_index)] + py::str(" could match ") +
                                py::join(py::str(", "), names)};
        }
        std::optional<Kind> action = option_tuples[0].action;
        py::str option_string = option_tuples[0].option_string;
        std::optional<py::str> sep = option_tuples[0].sep;
        std::optional<py::str> explicit_arg = option_tuples[0].explicit_arg;

        // identify additional optionals in the same arg string (e.g. -xyz is the same as -x -y -z if no args are required)
        std::vector<std::pair<Kind, std::vector<py::str>>> action_tuples;
        int64_t stop = 0;
        for (;;) {
            // if we found no optional action, skip it
            if (!action) {
                extras.push_back(arg_strings[static_cast<size_t>(start_index)]);
                return start_index + 1;
            }
            // if there is an explicit argument, try to match the optional's string arguments to only this
            if (explicit_arg) {
                int64_t arg_count = match_argument(*action, "A");
                // if the action is a single-dash option and takes no arguments, try to parse more single-dash
                // options out of the tail of the option string
                if (arg_count == 0 && option_string.u32()[1] != U'-' && !explicit_arg->empty()) {
                    if ((sep && !sep->empty()) || explicit_arg->u32()[0] == U'-') {
                        throw argument_error(action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                    }
                    action_tuples.push_back({*action, {}});
                    std::u32string chosen;
                    chosen += option_string.u32()[0];
                    chosen += explicit_arg->u32()[0];
                    option_string = py::str(chosen);
                    if (auto next = kind_of(option_string)) {
                        action = next;
                        py::str rest(explicit_arg->u32().substr(1));
                        if (rest.empty()) {
                            sep = std::nullopt;
                            explicit_arg = std::nullopt;
                        } else if (rest.u32()[0] == U'=') {
                            sep = py::str("=");
                            explicit_arg = py::str(rest.u32().substr(1));
                        } else {
                            sep = py::str("");
                            explicit_arg = rest;
                        }
                    } else {
                        std::u32string tail;
                        tail += chosen[0];
                        tail += explicit_arg->u32();
                        extras.push_back(py::str(tail));
                        stop = start_index + 1;
                        break;
                    }
                } else if (arg_count == 1) {
                    // if the action expect exactly one argument, we've successfully matched the option; exit the loop
                    stop = start_index + 1;
                    action_tuples.push_back({*action, {*explicit_arg}});
                    break;
                } else {
                    // error if a double-dash option did not use the explicit argument
                    throw argument_error(action, py::str("ignored explicit argument ") + py::repr(*explicit_arg));
                }
            } else {
                // if there is no explicit argument, try to match the optional's string arguments with the following strings
                int64_t start = start_index + 1;
                int64_t arg_count = match_argument(*action, arg_strings_pattern.substr(static_cast<size_t>(start)));
                stop = start + arg_count;
                action_tuples.push_back({*action, std::vector<py::str>(arg_strings.begin() + start, arg_strings.begin() + stop)});
                break;
            }
        }
        for (const auto& t : action_tuples) take_action(t.first, t.second);
        return stop;
    };

    // function to convert arg_strings into positional actions
    auto consume_positionals = [&](int64_t start_index) -> int64_t {
        // match as many Positionals as possible
        std::vector<int64_t> arg_counts = match_arguments_partial(positionals, arg_strings_pattern.substr(static_cast<size_t>(start_index)));
        // slice off the appropriate arg strings for each Positional and add the Positional and its args to the list
        for (size_t k = 0; k < arg_counts.size(); k++) {
            int64_t arg_count = arg_counts[k];
            std::vector<py::str> args(arg_strings.begin() + start_index, arg_strings.begin() + start_index + arg_count);
            // strip out the first '--'
            size_t dash = arg_strings_pattern.find('-', static_cast<size_t>(start_index));
            if (dash != std::string::npos && static_cast<int64_t>(dash) < start_index + arg_count) {
                for (size_t a = 0; a < args.size(); a++) {
                    if (args[a] == py::str("--")) {
                        args.erase(args.begin() + static_cast<std::ptrdiff_t>(a));
                        break;
                    }
                }
            }
            start_index += arg_count;
            take_action(positionals[k], args);
        }
        // slice off the Positionals that we just parsed
        positionals.erase(positionals.begin(), positionals.begin() + static_cast<std::ptrdiff_t>(arg_counts.size()));
        return start_index;
    };

    try {
        // consume Positionals and Optionals alternately, until we have passed the last option string
        int64_t start_index = 0;
        int64_t max_option_string_index = option_string_indices.empty() ? -1 : option_string_indices.rbegin()->first;
        while (start_index <= max_option_string_index) {
            // consume any Positionals preceding the next option
            int64_t next_option_string_index = start_index;
            while (next_option_string_index <= max_option_string_index) {
                if (option_string_indices.count(next_option_string_index)) break;
                next_option_string_index++;
            }
            if (start_index != next_option_string_index) {
                int64_t positionals_end_index = consume_positionals(start_index);
                // only try to parse the next optional if we didn't consume the option string during the positionals parsing
                if (positionals_end_index > start_index) {
                    start_index = positionals_end_index;
                    continue;
                } else {
                    start_index = positionals_end_index;
                }
            }
            // if we consumed all the positionals we could and we're not at the index of an option string, there were extra arguments
            if (!option_string_indices.count(start_index)) {
                for (int64_t k = start_index; k < next_option_string_index; k++) extras.push_back(arg_strings[static_cast<size_t>(k)]);
                start_index = next_option_string_index;
            }
            // consume the next optional and any arguments for it
            start_index = consume_optional(start_index);
        }
        // consume any positionals following the last Optional
        int64_t stop_index = consume_positionals(start_index);
        // if we didn't consume all the argument strings, there were extras
        for (size_t k = static_cast<size_t>(stop_index); k < arg_strings.size(); k++) extras.push_back(arg_strings[k]);

        // make sure all required actions were present
        py::list<py::str> required_actions;
        for (Kind action : {Kind::Input}) {
            bool seen = false;
            for (Kind s : seen_actions) seen = seen || s == action;
            if (!seen) required_actions.append(py::str(action_name(action)));
        }
        if (!required_actions.empty()) {
            throw ArgumentError{py::str("the following arguments are required: ") + py::join(py::str(", "), required_actions)};
        }
    } catch (const ArgumentError& err) {
        fail(prog, err.message);
    }
    if (!extras.empty()) {
        py::list<py::str> rest;
        for (const py::str& e : extras) rest.append(e);
        fail(prog, py::str("unrecognized arguments: ") + py::join(py::str(" "), rest));
    }
    return ns;
}

}  // namespace

// ----------------------------------------------------------------------------------------------- main
int64_t main(const py::list<py::str>& argv, const py::str& prog) {
    // setup arguments / parse arguments
    Namespace args = parse_args(argv, prog);
    Key key;   // args.key: None, then the text typed, then (for an encrypted file) the bytes
    if (args.key) key = *args.key;

    // validate arguments
    if (!py::os::path::isfile(args.input)) throw py::AssertionError("The specified input file or directory doesn't exist");

    // create work directory
    py::tempfile::TemporaryDirectory work_dir_holder(py::str("_") + path_stem(args.input).upper() + py::str("_XCP"));
    Path work_dir = path_type(work_dir_holder.name);
    Path out_dir = path_parent(args.input);

    if (!args.ignore) {
        input_(py::str("This will OVERWRITE and DELETE the original XCP file, press \"ENTER\" if you want to continue..."));
    }

    if (!args.no_backup) {
        py::print(py::str("Backing up the input file..."));
        py::shutil::copyfile(args.input, path_join(out_dir, path_stem(args.input) + py::str(".bak")));
    } else {
        py::print(py::str("Skipping backup..."));
    }

    py::print(py::str("Extracting XCP file..."));
    {
        StreamIO_ xcp(args.input, Endian::LITTLE);
        py::print(py::str("Checking CAB header..."));
        py::bytes cab_hdr_data = xcp.read_bytes_at(0, sizeof(CAB_HEADER));
        CAB_HEADER cab_hdr_struct = from_buffer_copy<CAB_HEADER>(cab_hdr_data);
        bool encrypted = !(cab_hdr_struct.magic == 0x4D534346 || cab_hdr_struct.magic == 0x4643534D);
        py::print(py::str("XCP file is ") + py::str(encrypted ? "encrypted" : "not encrypted"));

        if (encrypted) {
            if (args.key && !args.key->empty() && py::len(*args.key) % 2 == 0) {
                key = fromhex(*args.key);
            } else {
                throw py::Exception("Decryption key not specified and the file is encrypted!");
            }
        }

        if (encrypted) {
            py::print(py::str("Decrypting CAB header..."));
            cab_hdr_data = stream_decrypt_with_struct(xcp, std::get<py::bytes>(key), 0x60, 0, 0x60);
            cab_hdr_struct = from_buffer_copy<CAB_HEADER>(cab_hdr_data);
            if (!(cab_hdr_struct.magic == 0x4D534346 || cab_hdr_struct.magic == 0x4643534D)) throw py::AssertionError("Invalid key specified");
            py::print(py::str("Key appears to be OK!"));
        }

        if (encrypted) {
            py::print(py::str("Decrypting folder data..."));
            int64_t folder_size = cab_hdr_struct.cnt_folders * (static_cast<int64_t>(sizeof(CAB_FOLDER)) + XENON_DATA_SIZE);
            stream_decrypt_with_struct(xcp, std::get<py::bytes>(key), 0x28, 0x180, folder_size);
        }

        py::print(py::str("Processing filenames..."));
        int64_t offset = 0x44;   // xcp.offset = 0x44
        RC4_SHA_HEADER rc4_sha_struct = xcp.read_struct<RC4_SHA_HEADER>();
        py::bytes cksm(std::string(reinterpret_cast<const char*>(rc4_sha_struct.cksm), sizeof rc4_sha_struct.cksm));
        py::bytes confounder(std::string(reinterpret_cast<const char*>(rc4_sha_struct.confounder), sizeof rc4_sha_struct.confounder));
        XeCrypt::XeCryptRc4 cipher = XeCrypt::XeCryptRc4::new_(hmac_sha(key, cksm));
        cipher.decrypt(confounder);
        // decrypt filenames
        offset = cab_hdr_struct.off_files;
        for (int64_t i = 0; i < cab_hdr_struct.cnt_files; i++) {
            // decrypt header if necessary
            if (encrypted) {
                perform_function_at(xcp, offset, sizeof(CAB_ENTRY), cipher);
            }

            offset += sizeof(CAB_ENTRY);

            set_label(xcp, py::str("filename") + py::to_str(i));

            // decrypt filename if necessary
            int64_t idx = 0;
            int64_t byt = 0;
            while (byt != 0 || idx == 0) {
                if (encrypted) {
                    byt = perform_function_at(xcp, offset, 1, cipher)[0];
                    offset += 1;
                } else {
                    byt = xcp.read_byte();
                }
                idx += 1;
            }
            // rename the files to extract them properly
            xcp.write_bytes_at(offset - 4, py::str(py::to_str(i)).zfill(3).encode("utf-8"));
        }

        std::vector<CAB_FOLDER> folders;
        if (encrypted) {
            py::print(py::str("Preprocessing folders..."));
            offset = 0x180;
            for (int64_t i = 0; i < cab_hdr_struct.cnt_folders; i++) {
                set_label(xcp, py::str("folder") + py::to_str(i));
                folders.push_back(xcp.read_struct<CAB_FOLDER>());
                xcp.seek(XENON_DATA_SIZE, StreamIO::SEEK_CUR_);
            }
        }

        if (encrypted) {
            py::print(py::str("Processing folders..."));
            for (int64_t i = 0; i < cab_hdr_struct.cnt_folders; i++) {
                const CAB_FOLDER& curr_folder = folders[static_cast<size_t>(i)];

                int64_t size;
                if ((i + 1) == cab_hdr_struct.cnt_folders) {
                    size = xcp.length() - curr_folder.off_cab_start;
                } else {
                    const CAB_FOLDER& next_folder = folders[static_cast<size_t>(i + 1)];
                    size = static_cast<int64_t>(next_folder.off_cab_start) - static_cast<int64_t>(curr_folder.off_cab_start);
                }

                stream_decrypt_with_struct(xcp, std::get<py::bytes>(key), get_label(xcp, py::str("folder") + py::to_str(i)) + sizeof(CAB_FOLDER),
                                           curr_folder.off_cab_start, size);
            }
        }
    }

    if (!py::os::path::isdir(path_join(work_dir, CACHE_DIR))) {
        py::print(py::str("Creating output directory..."));
        path_mkdir(path_join(work_dir, CACHE_DIR));
    }

    py::print(py::str("Renaming XCP file..."));
    if (py::os::path::isfile(path_join(work_dir, TMP_CAB_FILE))) {
        py::os::unlink(path_join(work_dir, TMP_CAB_FILE));
    }
    py::shutil::move(args.input, path_join(work_dir, TMP_CAB_FILE));

    py::print(py::str("Extracting CAB file..."));
    // extract the cabinet file and make sure it exited properly
    if (!extract_cab(path_join(work_dir, TMP_CAB_FILE), path_join(work_dir, CACHE_DIR))) throw py::AssertionError("CAB extraction failed!");

    py::print(py::str("Merging the extracted files..."));
    // merge the files together to make them into one file
    {
        StreamIO_ fw(path_join(work_dir, path_stem(args.input).upper()));
        py::list<py::str> names = py::os::listdir(path_join(work_dir, CACHE_DIR));
        for (const py::str& name : names) {
            py::file fr = py::open(path_join(path_join(work_dir, CACHE_DIR), name), py::str("rb"));
            // shutil.copyfileobj(fr, fw, BUFF_SIZE)
            for (;;) {
                py::bytes block = fr.read(BUFF_SIZE);
                if (block.empty()) break;
                fw.write(block);
            }
            fr.close();
        }
        py::print(py::str("Converting to a LIVE package..."));
        // really hacky way to make it a LIVE file
        fw.write_bytes_at(0, py::bytes("LIVE"));
    }

    py::print(py::str("Moving file..."));
    py::shutil::move(path_join(work_dir, path_stem(args.input).upper()), path_join(out_dir, path_stem(args.input).upper()));

    py::print(py::str("Done!"));

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::xcp_dumper
