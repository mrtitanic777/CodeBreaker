// questions.cc -- the Xbox 360 executable's header (codebreaker/formats/xex.py), read with
// CodeBreaker's one XEX parser (native/xex/reader.cmake: xex_file.h), not by running xex.exe
// and reading its listing back.
//
// header(path) gives what xex.py took from `xex.exe list`: the original PE name (as the listing
// shows it, so cut at the first blank, as xex.py's pattern read it), load address, entry point,
// the import libraries in block order and the static libraries with their versions.  The
// basefile itself (decrypting, decompressing) stays the xex program's: xex.basefile runs it.
#include <exception>

#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"
#include "xex_file.h"

namespace cbf::xexq {
namespace {

constexpr uint32_t kExecutionInfo = 0x00040006;

// a library's name and version as the listing's line and xex.py's pattern give them back:
// "  %3d) %-14s v<version>" read as (\S+)\s+v(\S+); a blank name leaves the line unread
bool listed(const std::string &name, std::string &first_word) {
    size_t a = name.find_first_not_of(" \t");
    if (a == std::string::npos) return false;
    size_t b = name.find_first_of(" \t", a);
    if (b != std::string::npos && name.find_first_not_of(" \t", b) != std::string::npos) return false;
    first_word = name.substr(a, b == std::string::npos ? std::string::npos : b - a);
    return true;
}

std::string version(const xex::Version &v) { return v.str().substr(1); }   // without its "v"

void is_xex(Call &c) {
    std::string head;
    c.out.boolean(read_part(c.str_arg(0), 0, 4, head) && head == "XEX2");
}

// optional_headers(data) -> (entries up to the first failure, (exception, message) or None)
void optional_headers(Call &c) {
    const Bytes d = c.data;
    Out &o = c.out;
    o.begin_tuple();
    o.begin_list();
    std::optional<PyError> err;
    try {
        const uint32_t count = d.be32(0x14);
        for (uint32_t i = 0; i < count; ++i) {
            const int64_t at = 0x18 + int64_t(i) * 8;
            const uint32_t key = d.be32(at), value = d.be32(at + 4);
            o.begin_tuple();
            o.integer(at);
            o.integer(key);
            o.integer(value);
            o.end_tuple();
        }
    } catch (const PyError &e) {
        err = e;
    }
    o.end_list();
    if (err) {
        o.begin_tuple();
        o.text(err->kind);
        o.text(err->message);
        o.end_tuple();
    } else {
        o.none();
    }
    o.end_tuple();
}

void ids(Call &c) {
    const Bytes d = c.data;
    if (d.slice_is(0, 4, "XEX2", 4)) {
        const uint32_t count = d.be32(0x14);
        for (uint32_t i = 0; i < count; ++i) {
            const int64_t at = 0x18 + int64_t(i) * 8;
            const uint32_t key = d.be32(at), value = d.be32(at + 4);
            if (key == kExecutionInfo) {
                c.out.begin_tuple();
                c.out.integer(d.be32(value));
                c.out.integer(d.be32(int64_t(value) + 0x0C));
                c.out.end_tuple();
                return;
            }
        }
    }
    c.out.begin_tuple();
    c.out.integer(0);
    c.out.integer(0);
    c.out.end_tuple();
}

void header(Call &c) {
    const std::string path = c.str_arg(0);
    std::string file;
    if (!read_file(path, file)) raise("ValueError", path + ": error: cannot read file '" + path + "'");
    std::optional<xex::XexFile> parsed;
    try {
        parsed = xex::XexFile::parse(std::vector<uint8_t>(file.begin(), file.end()));
    } catch (const std::exception &e) {
        raise("ValueError", path + ": " + e.what());
    }
    const xex::XexFile &x = *parsed;
    Out &o = c.out;
    o.begin_dict();
    o.key("name");
    std::string word;
    std::string name = x.original_pe_name() ? std::string(x.original_pe_name()->c_str()) : "";
    size_t a = name.find_first_not_of(" \t\r\n\v\f");          // the listing's "Original PE Name:\s+(\S+)"
    if (x.original_pe_name() && a != std::string::npos)
        o.text(name.substr(a, name.find_first_of(" \t\r\n\v\f", a) == std::string::npos ? std::string::npos
                                                                                      : name.find_first_of(" \t\r\n\v\f", a) - a));
    else o.none();
    o.key("base");
    if (auto a = x.image_base()) o.integer(*a);
    else o.none();
    o.key("entry");
    if (auto e = x.entry_point()) o.integer(*e);
    else o.none();
    o.key("imports");
    o.begin_list();
    for (auto &l : x.import_libraries())
        if (listed(l.name, word)) o.latin1(Bytes(word));   // (the listing's single-byte text: a name padded with 0xCD stays valid UTF-8)
    o.end_list();
    o.key("libraries");
    o.begin_list();
    for (auto &l : x.static_libraries())
        if (listed(l.name, word)) {
            o.begin_tuple();
            o.latin1(Bytes(word));
            o.text(version(l.version));
            o.end_tuple();
        }
    o.end_list();
    o.end_dict();
}

}  // namespace

CBF_QUESTIONS(xex) {
    add("xex.is_xex", is_xex);
    add("xex.optional_headers", optional_headers);
    add("xex.ids", ids);
    add("xex.header", header);
}

}  // namespace cbf::xexq
