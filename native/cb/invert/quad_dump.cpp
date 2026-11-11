#include "cb/invert/quad_dump.hpp"

#include <algorithm>
#include <format>
#include <utility>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::invert::quad_dump {

namespace {

constexpr std::size_t kHeaderFields = 0x50;   // the last header field (the symbol count at 0x4e) ends here

struct OpcodeName {
    std::uint8_t opcode;
    std::string_view name;
};

// Sorted by opcode.
constexpr OpcodeName kOpcodes[] = {
    // unary
    {0x0a, "neg"}, {0x0b, "lnot"}, {0x0c, "compl"},
    // binary
    {0x10, "mul"}, {0x11, "div"}, {0x12, "mod"}, {0x13, "add"}, {0x14, "sub"}, {0x15, "shl"}, {0x16, "shr"},
    {0x17, "lt"},  {0x18, "le"},   {0x19, "gt"},  {0x1a, "ge"},  {0x1b, "eq"},  {0x1c, "ne"},  {0x1d, "and"},
    {0x1e, "xor"}, {0x1f, "or"},
    // statements and structure
    {0x20, "CALL"}, {0x23, "FUNC-BEGIN"}, {0x24, "RETURN"}, {0x2c, "CBRANCH"}, {0x2e, "LABEL"}, {0x2f, "LOOP"},
    {0x3f, "LABEL(named)"}, {0x40, "GOTO"},
};

bool is_pool_byte(std::uint8_t b) { return b == 0 || (b >= 32 && b < 127); }

}  // namespace

std::size_t namepool_start(ByteView file) {
    std::size_t i = file.size();
    while (i > 0 && is_pool_byte(file[i - 1])) --i;
    return i;
}

Scan scan(ByteView file) {
    Scan out;
    out.pool_offset = namepool_start(file);
    std::size_t o = kFirstRecord;
    while (o + 2 <= out.pool_offset) {
        const std::uint8_t op = file[o];
        const std::uint8_t length = file[o + 1];
        if (op == kTerminator) {
            out.terminator = Scan::Terminated{o, length};
            break;
        }
        if (o + 2 + length > file.size()) {   // the head promises more than the file holds
            out.overrun = o;
            break;
        }
        out.records.push_back({o, op, file.subspan(o + 2, length)});
        o += 2 + length;
    }
    out.end_offset = o;
    return out;
}

File parse(ByteView file) {
    if (file.size() < kHeaderFields) throw TruncatedError("quad file", 0, kHeaderFields, file.size());
    File out;
    out.size = file.size();
    io::ByteReader r(file);
    out.magic = r.u16();
    r.seek(4);
    for (auto& t : out.type_sizes) t = r.u16();
    r.seek(0x4e);
    out.symbol_count = r.u16();
    out.scan = scan(file);
    for (std::string_view name : text::split(as_text(file.subspan(out.scan.pool_offset)), '\0')) {
        if (!name.empty()) out.names.push_back(text::from_latin1(as_bytes(name)));
    }
    return out;
}

std::string_view opcode_name(std::uint8_t opcode) {
    const auto* end = std::end(kOpcodes);
    const auto* it = std::lower_bound(std::begin(kOpcodes), end, opcode,
                                      [](const OpcodeName& e, std::uint8_t op) { return e.opcode < op; });
    return (it != end && it->opcode == opcode) ? it->name : std::string_view{};
}

std::string opcode_label(std::uint8_t opcode) {
    const std::string_view name = opcode_name(opcode);
    return name.empty() ? std::format("op{:02x}", opcode) : std::string(name);
}

std::string format_payload(ByteView payload) {
    std::string out;
    for (const std::uint8_t b : payload) {
        if (!out.empty()) out += ' ';
        out += std::format("{:02x}", b);
    }
    return out;
}

std::string dump(const File& file, std::string_view label) {
    std::string out = std::format("== {} ({} bytes) ==\n", label, file.size);
    std::string sizes;
    for (const auto t : file.type_sizes) sizes += std::format("{}{}", sizes.empty() ? "" : ", ", t);
    out += std::format("0x00 magic={}  0x04 type-size-tbl=[{}]\n", file.magic, sizes);
    std::string names;
    for (const auto& n : file.names) names += std::format("{}\"{}\"", names.empty() ? "" : ", ", n);
    out += std::format("0x4e symbolcount={}  name-pool@0x{:x}=[{}]\n", file.symbol_count, file.scan.pool_offset, names);
    out += "\n-- quad records  [off] opcode(len) : payload --\n";
    for (const Record& rec : file.scan.records) {
        out += std::format("  0x{:03x}: {:<12}({:2d}) : {}\n", rec.offset, opcode_label(rec.opcode), rec.payload.size(),
                           format_payload(rec.payload));
    }
    if (file.scan.terminator) {
        const auto& t = *file.scan.terminator;
        out += std::format("  0x{:03x}: 0xff TERMINATOR (len={}); {} trailer bytes to name pool\n", t.offset, t.length,
                           file.scan.pool_offset - t.offset);
    } else if (file.scan.overrun) {
        out += std::format("  0x{:03x}: a record runs past the end of the file ({} bytes)\n", *file.scan.overrun, file.size);
    } else {
        out += std::format("  (reached 0x{:x}; name pool at 0x{:x})\n", file.scan.end_offset, file.scan.pool_offset);
    }
    return out;
}

std::string usage() {
    std::string out =
        "quad_dump - structural parser for Lattice C 2.12 quad files (<base>.q).\n"
        "\n"
        "Usage: quad_dump <file.q>\n"
        "\n"
        "Record framing (from LC2's reader @0x28c5 and confirmed by tiling every sample):\n"
        "  each quad record = [opcode:1][length:1][payload: `length` bytes]\n"
        "The reader reads 2 bytes (opcode,length) then `length` more, then parses up to 3\n"
        "operands from the payload. A function is a run of records starting at FUNC-BEGIN\n"
        "(0x23); the module ends at a 0xff terminator, after which come symbol trailer bytes\n"
        "and the NUL-terminated name pool.\n"
        "\n"
        "Opcode map (differentially verified via dynamic analysis; see\n"
        "re/notes/05-quad-format-and-harness.md, 07-quad-framing.md):\n  ";
    bool first = true;
    for (const OpcodeName& e : kOpcodes) {
        out += std::format("{}{:#04x}={}", first ? "" : ", ", e.opcode, e.name);
        first = false;
    }
    return out;
}

}  // namespace cb::invert::quad_dump
