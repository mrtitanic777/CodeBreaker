#include "formats/exepack.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "core/io.hpp"
#include "formats/dos.hpp"

namespace cb::formats::exepack {

namespace {

constexpr std::string_view kReader = "exepack";
constexpr std::uint16_t kSignature = 0x4252;   // "RB"
constexpr std::uint32_t kMaxRun = 0xFFFF;
constexpr std::uint16_t kUnpackerStack = 0x80;
constexpr std::size_t kParagraph = 16;
constexpr std::size_t kGroups = 16;   // one list of relocations for each 64 KB of the program

// The words at the head of the unpacker's segment.
struct Header {
    std::uint64_t at = 0;                  // where the segment begins in the module
    unsigned words = 0;
    std::uint16_t ip = 0, cs = 0;          // the program's entry
    std::uint16_t mem = 0;                 // EXEPACK's own field
    std::uint16_t own_size = 0;            // the segment's size in bytes
    std::uint16_t sp = 0, ss = 0;          // the program's stack
    std::uint16_t paragraphs = 0;          // the program's size in paragraphs
    std::uint16_t skip = 1;                // the later header's skip count (in paragraphs, counting the one the header is in)
};

// The header at `cs`:0 of the stored module whose entry is `ip` (0x10: eight words; later ones are longer and end the
// same way: the stack, the program's size, the skip count, "RB").
Header read_header(ByteView module, std::uint32_t cs, std::uint32_t ip) {
    Header h;
    h.at = std::uint64_t{cs} * kParagraph;
    const auto range = io::try_range(module, h.at, ip);
    if (!range) throw TruncatedError(kReader, h.at, ip, h.at < module.size() ? module.size() - h.at : 0);
    h.words = ip / 2;
    if (h.words == 0) throw FormatError(kReader, h.at, "not EXEPACK: the header is empty");
    io::ByteReader r(*range, kReader);
    std::vector<std::uint16_t> w(h.words);
    for (std::uint16_t& v : w) v = r.u16();
    if (w.back() != kSignature) throw FormatError(kReader, h.at, "not EXEPACK: no RB before the entry point");
    h.ip = w[0];
    h.cs = w[1];
    h.mem = w[2];
    h.own_size = w[3];
    if (ip == 0x10) {
        if (w.size() < 7) throw FormatError(kReader, h.at, "not EXEPACK: the header is too short");
        h.sp = w[4], h.ss = w[5], h.paragraphs = w[6], h.skip = 1;
    } else {
        if (w.size() < 5) throw FormatError(kReader, h.at, "not EXEPACK: the header is too short");
        const std::size_t first = w.size() - 5;   // the four words before the signature
        h.sp = w[first], h.ss = w[first + 1], h.paragraphs = w[first + 2], h.skip = w[first + 3];
    }
    return h;
}

// The stream, read backwards from `src` down to the program's first bytes: the padding, then commands, each a length word
// and a command byte (bit 0 marks the last) with a fill preceded by its byte and a copy by its bytes. `on_fill(dst, length, byte)`
// and `on_copy(src, dst, length)` see each one as it is met; the positions the stream and the program have come to are returned
// (equal when the program's first bytes are stored as they are). `buf` is read as it stands when each command is met.
struct Ends {
    std::int64_t src, dst;
};

template <class OnFill, class OnCopy>
Ends walk(ByteView buf, const Header& h, std::int64_t src, OnFill&& on_fill, OnCopy&& on_copy) {
    const auto byte_at = [&](std::int64_t i) -> std::uint8_t {
        if (i < 0 || static_cast<std::uint64_t>(i) >= buf.size()) throw FormatError(kReader, h.at, "EXEPACK data runs out");
        return buf[static_cast<std::size_t>(i)];
    };
    std::int64_t dst = static_cast<std::int64_t>(h.paragraphs) * kParagraph;
    while (src > 0 && byte_at(src - 1) == 0xFF) --src;
    for (;;) {
        if (src < 3) throw FormatError(kReader, h.at, "EXEPACK data runs out");
        const int command = byte_at(src - 1);
        const std::int64_t length = byte_at(src - 3) | (byte_at(src - 2) << 8);
        src -= 3;
        if ((command & 0xFE) == kFill) {   // `length` bytes of the byte before
            src -= 1;
            dst -= length;
            if (dst < 0 || src < 0) throw FormatError(kReader, h.at, "EXEPACK data runs out");
            on_fill(dst, length, byte_at(src));
        } else if ((command & 0xFE) == kCopy) {
            src -= length;
            dst -= length;
            if (src < 0 || dst < 0) throw FormatError(kReader, h.at, "EXEPACK data runs out");
            on_copy(src, dst, length);
        } else {
            throw FormatError(kReader, h.at, std::format("EXEPACK command {:#x}", command));
        }
        if (command & 1) break;
    }
    return {src, dst};
}

// The unpacker's message ends its code; where it is in the segment, or nullopt.
std::optional<std::size_t> message_in(ByteView segment) {
    const std::size_t at = as_text(segment).find(kMessage);
    if (at == std::string_view::npos) return std::nullopt;
    return at;
}

// The relocation table: sixteen lists, a count and that many offsets each. `lenient`: a file that ends between lists ends the table.
std::vector<std::uint32_t> read_table(ByteView data, std::uint64_t at, bool lenient) {
    io::ByteReader table(data, "exepack relocations");
    table.seek(at);
    std::vector<std::uint32_t> out;
    for (std::uint32_t group = 0; group < kGroups; ++group) {
        if (lenient && table.remaining() < 2) break;
        const std::uint32_t count = table.u16();
        for (std::uint32_t k = 0; k < count; ++k) out.push_back(group * 0x10000u + table.u16());
    }
    return out;
}

}  // namespace

dos::Unpack unpack(ByteView module, std::uint32_t cs, std::uint32_t ip) {
    const Header h = read_header(module, cs, ip);
    Bytes buf(module.begin(), module.begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(h.at, module.size())));
    buf.resize(std::max<std::uint64_t>(buf.size(), std::uint64_t{h.paragraphs} * kParagraph), 0);
    walk(
        buf, h, static_cast<std::int64_t>(h.at) - (static_cast<std::int64_t>(h.skip) - 1) * 16,
        [&](std::int64_t dst, std::int64_t length, std::uint8_t byte) { std::fill_n(buf.begin() + dst, length, byte); },
        [&](std::int64_t src, std::int64_t dst, std::int64_t length) {
            const std::uint64_t a = std::min<std::uint64_t>(static_cast<std::uint64_t>(src), buf.size());
            const std::uint64_t b = std::min<std::uint64_t>(static_cast<std::uint64_t>(src + length), buf.size());
            const Bytes piece(buf.begin() + static_cast<std::ptrdiff_t>(a), buf.begin() + static_cast<std::ptrdiff_t>(b));   // a copy: the ranges may overlap
            std::copy(piece.begin(), piece.end(), buf.begin() + dst);
        });
    // the relocation table follows the message the unpacker prints
    const ByteView stub_area = module.size() > h.at ? module.subspan(h.at, std::min<std::uint64_t>(h.own_size, module.size() - h.at)) : ByteView{};
    const auto found = message_in(stub_area);
    if (!found) throw FormatError(kReader, h.at, "EXEPACK relocation table not found");
    dos::Unpack r;
    r.relocations = read_table(module, h.at + *found + kMessage.size(), false);
    buf.resize(std::min<std::uint64_t>(buf.size(), std::uint64_t{h.paragraphs} * kParagraph));
    r.module = std::move(buf);
    r.cs = h.cs;
    r.ip = h.ip;
    r.ss = h.ss;
    r.sp = h.sp;
    return r;
}

std::optional<Original> read_original(ByteView file) {
    constexpr std::size_t kMzHeaderBytes = 0x20;       // the smallest file that can hold an MZ header and be packed
    constexpr std::size_t kPackedHeaderBytes = 18;     // the header: seven words, then (for the old form) padding
    const bool mz = file.size() >= 2 && ((file[0] == 'M' && file[1] == 'Z') || (file[0] == 'Z' && file[1] == 'M'));
    if (!mz || file.size() < kMzHeaderBytes) return std::nullopt;
    io::ByteReader mzr(file, kReader);
    mzr.seek(8);
    const std::uint32_t header_paragraphs = mzr.u16();
    mzr.seek(0x16);
    const std::uint32_t cs = mzr.u16();
    const std::uint64_t at = std::uint64_t{header_paragraphs} * 16 + std::uint64_t{cs} * 16;
    // (the packed file's CS:0 - EXEPACK's header; the signature sits at word 7 or, in the long form, word 8)
    const auto signed_at = [&](std::size_t offset) { return as_text(file.subspan(at + offset, 2)) == "RB"; };
    if (at + kPackedHeaderBytes > file.size() || (!signed_at(14) && !signed_at(16))) return std::nullopt;

    io::ByteReader r(file, kReader);
    r.seek(at);
    Original p;
    p.ip = r.u16();
    p.cs = r.u16();
    p.mem = r.u16();
    p.size = r.u16();
    p.sp = r.u16();
    p.ss = r.u16();
    p.dest = r.u16();
    const std::size_t found = as_text(file).find(kMessage, at);
    if (found == std::string_view::npos) return p;
    p.relocs = read_table(file, found + kMessage.size(), true);
    return p;
}

Layout layout(ByteView module, std::uint16_t cs, std::uint16_t ip) {
    if (!(ip == 0x10 || ip == 0x12)) {
        throw FormatError(kReader, std::uint64_t{cs} * kParagraph, "not the EXEPACK this writes (a header of eight or nine words ending in RB)");
    }
    const Header h = read_header(module, cs, ip);
    if (ip == 0x12 && h.skip != 1) throw FormatError(kReader, h.at, "not the EXEPACK this writes (a header of eight or nine words ending in RB)");

    // the stream is read backwards from the header: padding, then commands down to the program's first bytes
    std::vector<Fill> fills;
    const Ends ends = walk(
        module, h, static_cast<std::int64_t>(h.at),
        [&](std::int64_t dst, std::int64_t length, std::uint8_t byte) { fills.push_back({static_cast<std::uint32_t>(dst), static_cast<std::uint32_t>(length), byte}); },
        [](std::int64_t, std::int64_t, std::int64_t) {});
    if (ends.src != ends.dst) throw FormatError(kReader, static_cast<std::uint64_t>(ends.src), "the stored program's first bytes are not stored as they are");

    // the unpacker's message ends its code; find it inside the segment
    // (a stored module cut inside its relocation table still says how it was packed: the segment is read as far as the
    // module goes, and the message must lie in what is there)
    const ByteView segment = *io::try_range(module, h.at, std::min<std::uint64_t>(h.own_size, module.size() - h.at));
    const auto message = message_in(segment);
    if (!message) throw FormatError(kReader, h.at, "EXEPACK message not found");

    Layout out;
    std::sort(fills.begin(), fills.end());
    out.fills = std::move(fills);
    const ByteView code = io::checked_range(segment, ip, *message + kMessage.size() - ip);
    out.stub_code.assign(code.begin(), code.end());
    out.size = static_cast<std::uint32_t>(h.paragraphs) * kParagraph;
    out.words = h.words;
    return out;
}

Packed pack(ByteView image_in, std::span<const std::uint32_t> relocs, Far16 entry, Far16 stack,
            std::span<const Fill> fills_in, ByteView stub_code, unsigned words) {
    if (words != 8 && words != 9) throw UsageError(std::format("an EXEPACK header of {} words", words));
    Bytes image(image_in.begin(), image_in.end());
    image.resize((image.size() + kParagraph - 1) / kParagraph * kParagraph, 0);
    if (image.size() / kParagraph > 0xFFFF) throw FormatError(kReader, 0, "a program of more than 1 MB cannot be packed");

    std::vector<Fill> fills(fills_in.begin(), fills_in.end());
    std::sort(fills.begin(), fills.end());
    std::uint64_t covered = 0;
    for (const Fill& f : fills) {
        const bool run = f.length > 0 && f.length <= kMaxRun && std::uint64_t{f.offset} + f.length <= image.size() &&
                         std::all_of(image.begin() + f.offset, image.begin() + f.offset + f.length,
                                     [&](std::uint8_t b) { return b == f.byte; });
        if (!run) throw UsageError(std::format("a fill at {:#x} is not {} bytes of {:02X}", f.offset, f.length, f.byte));
        if (f.offset < covered) throw UsageError(std::format("the fill at {:#x} overlaps the one before it", f.offset));
        covered = std::uint64_t{f.offset} + f.length;
    }

    // The stream: the program's first bytes as they are, then a command per stretch up to the end, each a fill or
    // a copy (copies in pieces of at most 64 KB - 1). The first command written is the last read: bit 0 marks it.
    // (a program with no fill is all copy commands: the unpacker needs a command to know where the stream ends)
    const std::size_t first_fill = fills.empty() ? 0 : fills[0].offset;
    io::ByteWriter stream;
    stream.bytes(ByteView(image).first(first_fill));
    bool first_command = true;
    auto command_byte = [&](std::uint8_t kind) {
        const auto b = static_cast<std::uint8_t>(kind | (first_command ? 1 : 0));
        first_command = false;
        return b;
    };
    std::size_t at = first_fill;
    auto copy_until = [&](std::size_t end) {
        while (at < end) {
            const std::size_t k = std::min<std::size_t>(end - at, kMaxRun);
            stream.bytes(ByteView(image).subspan(at, k));
            stream.u16(static_cast<std::uint16_t>(k));
            stream.u8(command_byte(kCopy));
            at += k;
        }
    };
    for (const Fill& f : fills) {
        copy_until(f.offset);
        stream.u8(f.byte);
        stream.u16(static_cast<std::uint16_t>(f.length));
        stream.u8(command_byte(kFill));
        at += f.length;
    }
    copy_until(image.size());
    Bytes stored = stream.take();
    stored.resize((stored.size() + kParagraph - 1) / kParagraph * kParagraph, 0xFF);

    // the relocation table: sixteen lists, one per 64 KB window of the program
    std::array<std::vector<std::uint16_t>, 16> lists;
    for (const std::uint32_t r : relocs) {
        if ((r >> 16) >= lists.size()) throw FormatError(kReader, r, "a relocation past the first megabyte");
        lists[r >> 16].push_back(static_cast<std::uint16_t>(r & 0xFFFF));
    }
    io::ByteWriter table;
    for (const auto& list : lists) {
        if (list.size() > 0xFFFF) throw FormatError(kReader, 0, "more than 65535 relocations in a 64 KB window");
        table.u16(static_cast<std::uint16_t>(list.size()));
        for (const std::uint16_t v : list) table.u16(v);
    }

    const std::size_t own = 2 * words + stub_code.size() + table.size();
    if (own > 0xFFFF) throw FormatError(kReader, 0, "the unpacker segment is larger than 64 KB");
    const auto paragraphs = static_cast<std::uint16_t>(image.size() / kParagraph);
    io::ByteWriter head;
    head.u16(entry.off);   // the program's IP, CS, a scratch word, the segment's size, SP, SS, its size in paragraphs
    head.u16(entry.seg);
    head.u16(0);
    head.u16(static_cast<std::uint16_t>(own));
    head.u16(stack.off);
    head.u16(stack.seg);
    head.u16(paragraphs);
    if (words == 9) head.u16(1);
    head.u16(kSignature);

    Packed out;
    out.registers.cs = static_cast<std::uint16_t>(stored.size() / kParagraph);
    out.registers.ip = static_cast<std::uint16_t>(2 * words);
    const std::size_t ss = paragraphs + (own + kParagraph - 1) / kParagraph;
    if (ss > 0xFFFF) throw FormatError(kReader, 0, "the unpacker's stack segment does not fit a word");
    out.registers.ss = static_cast<std::uint16_t>(ss);
    out.registers.sp = kUnpackerStack;
    out.module = std::move(stored);
    append(out.module, head.data());
    append(out.module, stub_code);
    append(out.module, table.data());
    return out;
}

}  // namespace cb::formats::exepack
