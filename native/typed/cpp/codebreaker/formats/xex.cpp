// codebreaker/formats/xex.cpp -- typed C++ of codebreaker.formats.xex (see xex.hpp).
//
// The header reader is native/formats/xex/questions.cc over native/xex's one parser (xex_file.cpp): linked, and
// called with typed results.  What that file shaped as pickle (the listing of the header) is built here as a
// `Header`.

#include "codebreaker/formats/xex.hpp"

#include <algorithm>
#include <exception>
#include <memory>

#include "py_json.hpp"
#include "py_subprocess.hpp"

#include "../../../../formats/core/bytes.h"
#include "../../../../formats/core/files.h"
#include "../../../../formats/core/text.h"
#include "../../../../xex/src/xex_file.h"

namespace codebreaker::formats::xex {

namespace {

namespace cb = ::cbf;

constexpr uint32_t kExecutionInfo = 0x00040006;

// the exception an answer carried: native._exception(kind, message)
[[noreturn]] void raise_py(const cb::PyError& e) {
    const std::string& k = e.kind;
    const std::string& m = e.message;
    if (k == "struct.error") throw py::StructError(m);
    if (k == "KeyError") throw py::KeyError(py::repr(py::str(m)));
    if (k == "ValueError") throw py::ValueError(m);
    if (k == "IndexError") throw py::IndexError(m);
    if (k == "TypeError") throw py::TypeError(m);
    if (k == "MemoryError") throw py::MemoryError(m);
    if (k == "OverflowError") throw py::OverflowError(m);
    if (k == "OSError") throw py::OSError(m);
    throw py::RuntimeError(m);
}

// a library's name and version as the listing's line and xex.py's pattern give them back:
// "  %3d) %-14s v<version>" read as (\S+)\s+v(\S+); a blank name leaves the line unread
bool listed(const std::string& name, std::string& first_word) {
    size_t a = name.find_first_not_of(" \t");
    if (a == std::string::npos) return false;
    size_t b = name.find_first_of(" \t", a);
    if (b != std::string::npos && name.find_first_not_of(" \t", b) != std::string::npos) return false;
    first_word = name.substr(a, b == std::string::npos ? std::string::npos : b - a);
    return true;
}

std::string version_text(const ::xex::Version& v) { return v.str().substr(1); }   // without its "v"

// struct.unpack(">I", b): the slice must be exactly four bytes
int64_t unpack_be32(const py::bytes& b) {
    if (b.size() != 4) throw py::StructError("unpack requires a buffer of 4 bytes");
    return (b.byte_(0) << 24) | (b.byte_(1) << 16) | (b.byte_(2) << 8) | b.byte_(3);
}

void pack_be(std::string& buf, size_t at, uint64_t v, int width) {
    for (int k = 0; k < width; ++k) buf[at + size_t(k)] = char((v >> (8 * (width - 1 - k))) & 0xFF);
}

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

// the entries optional_headers gives, then the failure the reader met
class OptionalHeaders : public py::generator<HeaderEntry> {
  public:
    std::vector<HeaderEntry> entries;
    std::optional<cb::PyError> error;
    size_t pos = 0;
    std::optional<HeaderEntry> next() override {
        if (pos < entries.size()) return entries[pos++];
        if (error) {
            cb::PyError e = *error;
            error.reset();
            raise_py(e);
        }
        return std::nullopt;
    }
};

}  // namespace

const py::bytes MAGIC = py::bytes("XEX2");
const int64_t FILE_FORMAT_INFO = 0x000003FF;

bool is_xex(const py::str& path) {
    std::string head;
    return cb::read_part(path.utf8(), 0, 4, head) && head == "XEX2";
}

py::iter<HeaderEntry> optional_headers(const py::bytes& data) {
    auto g = std::make_shared<OptionalHeaders>();
    const cb::Bytes d(data.raw());
    try {
        const uint32_t count = d.be32(0x14);
        for (uint32_t i = 0; i < count; ++i) {
            const int64_t at = 0x18 + int64_t(i) * 8;
            const uint32_t key = d.be32(at), value = d.be32(at + 4);
            g->entries.emplace_back(at, int64_t(key), int64_t(value));
        }
    } catch (const cb::PyError& e) {
        g->error = e;
    }
    return py::iter<HeaderEntry>(g);
}

std::tuple<int64_t, int64_t> ids(const py::bytes& data) {
    try {
        const cb::Bytes d(data.raw());
        if (d.slice_is(0, 4, "XEX2", 4)) {
            const uint32_t count = d.be32(0x14);
            for (uint32_t i = 0; i < count; ++i) {
                const int64_t at = 0x18 + int64_t(i) * 8;
                const uint32_t key = d.be32(at), value = d.be32(at + 4);
                if (key == kExecutionInfo) {
                    const int64_t media_id = d.be32(value);               // (in this order: the first read that fails is the one reported)
                    const int64_t title_id = d.be32(int64_t(value) + 0x0C);
                    return std::make_tuple(media_id, title_id);
                }
            }
        }
        return std::make_tuple(int64_t(0), int64_t(0));
    } catch (const cb::PyError& e) {
        raise_py(e);
    }
}

py::str program() {
    py::str exe = py::os::path::join(py::settings::get_str(py::str("xex_bin")), py::str("xex.exe"));
    if (!py::os::path::isfile(exe)) {
        throw py::RuntimeError("the xex program is not built: cmake -S native/xex -B native/xex/build "
                               "&& cmake --build native/xex/build --config Release --target xex");
    }
    return exe;
}

py::str _sha(const py::str& path) {
    py::hashlib::Hash h = py::hashlib::sha1();
    py::file f = py::open_rb(path);
    while (true) {                                          // (blocks of 1 MiB)
        py::bytes block = f.read(1 << 20);
        if (block.empty()) break;
        h.update(block);
    }
    f.close();
    return h.hexdigest();
}

Header header(const py::str& path_s) {
    const std::string path = path_s.utf8();
    std::string file;
    if (!cb::read_file(path, file)) throw py::ValueError(path + ": error: cannot read file '" + path + "'");
    std::optional<::xex::XexFile> parsed;
    try {
        parsed = ::xex::XexFile::parse(std::vector<uint8_t>(file.begin(), file.end()));
    } catch (const std::exception& e) {
        throw py::ValueError(path + ": " + e.what());
    }
    const ::xex::XexFile& x = *parsed;
    Header h;
    std::string word;
    std::string name = x.original_pe_name() ? std::string(x.original_pe_name()->c_str()) : "";
    const char* ws = " \t\r\n\v\f";
    size_t a = name.find_first_not_of(ws);                  // the listing's "Original PE Name:\s+(\S+)"
    if (x.original_pe_name() && a != std::string::npos) {
        size_t b = name.find_first_of(ws, a);
        h.name = py::str(name.substr(a, b == std::string::npos ? std::string::npos : b - a));
    }
    if (auto b = x.image_base()) h.base = int64_t(*b);
    if (auto e = x.entry_point()) h.entry = int64_t(*e);
    for (auto& l : x.import_libraries())
        if (listed(l.name, word)) h.imports.append(py::str(cb::latin1(cb::Bytes(word))));   // (the listing's single-byte text)
    for (auto& l : x.static_libraries())
        if (listed(l.name, word)) h.libraries.append(std::make_tuple(py::str(cb::latin1(cb::Bytes(word))), py::str(version_text(l.version))));
    return h;
}

py::str basefile(const py::str& path) {
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower();
    py::str out = state({py::str("unpacked"), py::str("xex"), stem + py::str("-") + _sha(path).slice(0, 12) + py::str(".exe")});
    if (!py::os::path::isfile(out)) {
        py::str tmp = out + py::str(".part");
        py::subprocess::CompletedProcess r = py::subprocess::run(py::list<py::str>{program(), py::str("extract"), path, tmp});
        if (r.returncode != 0 || !py::os::path::isfile(tmp)) {
            py::str text = (r.stderr_ && !r.stderr_->empty()) ? *r.stderr_ : (r.stdout_ ? *r.stdout_ : py::str());
            throw py::ValueError(path.utf8() + ": no basefile (" + text.strip().utf8() + ")");
        }
        Header h = header(path);
        // json.dump({...}, f, indent=1) to a text file (a new line is written as the platform's)
        const std::string nl =
#ifdef _WIN32
            "\r\n";
#else
            "\n";
#endif
        auto q = [](const py::str& s) {
            std::string o;
            py::json::put(o, s);
            return o;
        };
        auto num = [](const std::optional<int64_t>& v) { return v ? std::to_string(*v) : std::string("null"); };
        std::string j = "{" + nl;
        j += " \"xex\": " + q(py::os::path::abspath(path)) + "," + nl;
        j += " \"name\": " + (h.name ? q(*h.name) : std::string("null")) + "," + nl;
        j += " \"base\": " + num(h.base) + "," + nl;
        j += " \"entry\": " + num(h.entry) + "," + nl;
        j += " \"imports\": ";
        if (h.imports.empty()) {
            j += "[]";
        } else {
            j += "[" + nl;
            for (int64_t i = 0; i < h.imports.size(); ++i) j += "  " + q(h.imports[i]) + (i + 1 < h.imports.size() ? "," : "") + nl;
            j += " ]";
        }
        j += "," + nl + " \"libraries\": ";
        if (h.libraries.empty()) {
            j += "[]";
        } else {
            j += "[" + nl;
            for (int64_t i = 0; i < h.libraries.size(); ++i) {
                j += "  [" + nl + "   " + q(std::get<0>(h.libraries[i])) + "," + nl + "   " + q(std::get<1>(h.libraries[i])) + nl + "  ]";
                j += (i + 1 < h.libraries.size() ? "," : "") + nl;
            }
            j += " ]";
        }
        j += nl + "}";
        {
            py::file f = py::open(out + py::str(".json"), py::str("wb"));
            f.write(py::bytes(j));
            f.close();
        }
        py::os::replace(tmp, out);
    }
    return out;
}

py::str basic(const py::str& path, const py::str& out) {
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read(-1);
        f.close();
    }
    if (data.slice(0, 4) != MAGIC) throw py::ValueError(path.utf8() + ": not an XEX2 module");
    int64_t pe_offset = unpack_be32(data.slice(8, 12));
    std::optional<int64_t> info;
    for (const HeaderEntry& e : optional_headers(data)) {
        if (std::get<1>(e) == FILE_FORMAT_INFO) info = std::get<2>(e);
    }
    if (!info) throw py::ValueError(path.utf8() + ": no file format info header");
    int64_t info_size = unpack_be32(data.slice(*info, *info + 4));
    if (info_size < 16) throw py::ValueError(path.utf8() + ": file format info too small for one block");
    py::bytes image;
    {
        py::file f = py::open_rb(basefile(path));
        image = f.read(-1);
        f.close();
    }
    std::string header = data.slice(0, pe_offset).raw();
    // one block, all of the image as data, no zero run (the block count comes from info_size)
    if (int64_t(header.size()) - *info < 16)
        throw py::StructError("pack_into requires a buffer of at least " + std::to_string(*info + 16) + " bytes for packing 16 bytes at offset " +
                              std::to_string(*info) + " (actual buffer size is " + std::to_string(header.size()) + ")");
    if (uint64_t(image.size()) > 0xFFFFFFFFull) throw py::StructError("'I' format requires 0 <= number <= 4294967295");
    pack_be(header, size_t(*info), 16, 4);
    pack_be(header, size_t(*info) + 4, 0, 2);
    pack_be(header, size_t(*info) + 6, 1, 2);
    pack_be(header, size_t(*info) + 8, uint64_t(image.size()), 4);
    pack_be(header, size_t(*info) + 12, 0, 4);
    {
        py::file f = py::open(out, py::str("wb"));
        f.write(py::bytes(header) + image);
        f.close();
    }
    return out;
}

std::optional<symbols::native::Json> import_libraries(const py::str& image_path) {
    using symbols::native::Json;
    try {
        py::str text;
        {
            py::file f = py::open(image_path + py::str(".json"), py::str("rb"));
            py::bytes raw = f.read(-1);
            f.close();
            text = raw.decode("utf-8");
        }
        Json doc = symbols::native::loads(text);
        if (std::holds_alternative<Json::Object>(doc.value)) {                 // ["imports"]
            for (const auto& kv : std::get<Json::Object>(doc.value))
                if (kv.first == py::str("imports")) return kv.second;
            return std::nullopt;                                                // KeyError
        }
        if (std::holds_alternative<Json::Array>(doc.value)) throw py::TypeError("list indices must be integers or slices, not str");
        if (std::holds_alternative<py::str>(doc.value)) throw py::TypeError("string indices must be integers, not 'str'");
        if (std::holds_alternative<std::monostate>(doc.value)) throw py::TypeError("'NoneType' object is not subscriptable");
        if (std::holds_alternative<bool>(doc.value)) throw py::TypeError("'bool' object is not subscriptable");
        if (std::holds_alternative<int64_t>(doc.value)) throw py::TypeError("'int' object is not subscriptable");
        throw py::TypeError("'float' object is not subscriptable");
    } catch (const py::OSError&) {
        return std::nullopt;
    } catch (const py::ValueError&) {                                           // (JSONDecodeError and UnicodeDecodeError are ValueErrors)
        return std::nullopt;
    }
}

int64_t run(const py::list<py::str>& argv) {
    py::list<py::str> args{program()};
    args.extend(argv);
    py::subprocess::RunOptions o;
    o.stdout_ = py::subprocess::Stream::Inherit;
    o.stderr_ = py::subprocess::Stream::Inherit;
    return py::subprocess::run(args, o).returncode;
}

int64_t main(const py::list<py::str>& argv) { return run(argv); }

}  // namespace codebreaker::formats::xex
