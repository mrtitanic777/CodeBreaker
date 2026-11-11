#include "cb/build/exepack.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/build/fields.hpp"
#include "cb/core/io.hpp"

namespace cb::build::exepack {

namespace {

constexpr std::string_view kReader = "exepack";
constexpr std::uint16_t kSignature = 0x4252;   // "RB"
constexpr std::uint32_t kMaxRun = 0xFFFF;
constexpr std::uint16_t kUnpackerStack = 0x80;
constexpr std::size_t kParagraph = 16;

}  // namespace

Layout layout(ByteView module, std::uint16_t cs, std::uint16_t ip) {
    const std::uint64_t at = std::uint64_t{cs} * kParagraph;
    if (!(ip == 0x10 || ip == 0x12)) {
        throw FormatError(kReader, at, "not the EXEPACK this writes (a header of eight or nine words ending in RB)");
    }
    const auto header = io::try_range(module, at, ip);
    if (!header) throw TruncatedError(kReader, at, ip, at < module.size() ? module.size() - at : 0);
    io::ByteReader r(*header, kReader);
    std::array<std::uint16_t, 9> w{};
    const unsigned words = ip / 2;
    for (unsigned i = 0; i < words; ++i) w[i] = r.u16();
    if (w[words - 1] != kSignature || (ip == 0x12 && w[7] != 1)) {
        throw FormatError(kReader, at, "not the EXEPACK this writes (a header of eight or nine words ending in RB)");
    }
    const std::uint16_t own_size = w[3];
    const std::uint16_t paragraphs = w[6];

    // the stream is read backwards from the header: padding, then commands down to the program's first bytes
    std::int64_t src = static_cast<std::int64_t>(at);
    std::int64_t dst = static_cast<std::int64_t>(paragraphs) * kParagraph;
    auto byte_before = [&](std::int64_t pos) -> std::uint8_t {
        if (pos < 1 || static_cast<std::uint64_t>(pos) > module.size()) throw TruncatedError(kReader, 0, 1, 0);
        return module[static_cast<std::size_t>(pos - 1)];
    };
    std::vector<Fill> fills;
    while (byte_before(src) == 0xFF) --src;
    for (;;) {
        const std::uint8_t command = byte_before(src);
        if (src < 3) throw TruncatedError(kReader, 0, 3, static_cast<std::uint64_t>(src));
        const std::uint16_t length = u16_at(module, static_cast<std::uint64_t>(src - 3), kReader);
        src -= 3;
        if ((command & 0xFE) == kFill) {
            src -= 1;
            dst -= length;
            if (dst < 0) throw FormatError(kReader, static_cast<std::uint64_t>(src), "the stream holds more than the program");
            fills.push_back({static_cast<std::uint32_t>(dst), length, byte_before(src + 1)});
        } else if ((command & 0xFE) == kCopy) {
            src -= length;
            dst -= length;
            if (src < 0 || dst < 0) throw FormatError(kReader, 0, "a copy command runs past the start of the stream");
        } else {
            throw FormatError(kReader, static_cast<std::uint64_t>(src), std::format("EXEPACK command {:#x}", command));
        }
        if (command & 1) break;
    }
    if (src != dst) throw FormatError(kReader, static_cast<std::uint64_t>(src), "the stored program's first bytes are not stored as they are");

    // the unpacker's message ends its code; find it inside the segment
    const ByteView segment = io::checked_range(module, at, own_size);
    const std::string_view text = as_text(segment);
    const std::size_t message = text.find(kMessage);
    if (message == std::string_view::npos) throw FormatError(kReader, at, "EXEPACK message not found");

    Layout out;
    std::sort(fills.begin(), fills.end());
    out.fills = std::move(fills);
    const ByteView code = io::checked_range(segment, ip, message + kMessage.size() - ip);
    out.stub_code.assign(code.begin(), code.end());
    out.size = static_cast<std::uint32_t>(paragraphs) * kParagraph;
    out.words = words;
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

}  // namespace cb::build::exepack
