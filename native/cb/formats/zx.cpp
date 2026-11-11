#include "cb/formats/zx.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::zx {

namespace {

constexpr std::string_view kReader = "zx";
constexpr std::size_t kSnaHeader = 27;
constexpr std::size_t kRam48 = 0xC000;     // $4000-$FFFF
constexpr std::size_t kPage = 0x4000;
constexpr std::size_t kZ80Header = 30;
constexpr std::uint8_t kBasicUsrToken = 0xC0;
constexpr std::uint8_t kBasicNumberMarker = 0x0E;
constexpr std::size_t kTapeHeaderBlockSize = 19;

// Copy `src` into the memory image at `address`, as far as it fits below $10000.
void place(Bytes& mem, std::size_t address, ByteView src) {
    if (address >= mem.size()) return;
    const std::size_t n = std::min(src.size(), mem.size() - address);
    std::copy_n(src.begin(), n, mem.begin() + static_cast<std::ptrdiff_t>(address));
}

// .z80's run-length coding: ED ED n b is n copies of b. Stops once `limit` bytes are out (a run may overshoot it).
Bytes unrle(ByteView block, std::size_t limit) {
    Bytes out;
    std::size_t i = 0;
    while (i < block.size()) {
        if (block[i] == 0xED && i + 1 < block.size() && block[i + 1] == 0xED) {
            if (i + 3 >= block.size()) throw TruncatedError(kReader, i, 4, block.size() - i);
            out.insert(out.end(), block[i + 2], block[i + 3]);
            i += 4;
        } else {
            out.push_back(block[i]);
            i += 1;
        }
        if (limit != 0 && out.size() >= limit) break;
    }
    return out;
}

void add_entry(std::vector<Entry>& entries, std::uint16_t address, std::string name) {
    const bool taken = std::any_of(entries.begin(), entries.end(), [&](const Entry& e) { return e.address == address; });
    if (!taken) entries.push_back({address, std::move(name)});
}

// A snapshot's Program: the program block is RAM from $5B00; the entries are the PC and an IM 2 handler.
Program snapshot(Bytes memory, const Registers& regs, std::uint16_t pc) {
    Program p;
    p.kind = Kind::Snapshot;
    p.registers = regs;
    p.origin = kProgramStart;
    p.image.assign(memory.begin() + kProgramStart, memory.end());
    if (pc >= kProgramStart) add_entry(p.entries, pc, "start");
    if (regs.im == 2) {
        // IM 2: the vector table is at I*256, read at its last entry.
        const std::size_t at = (std::size_t{regs.i} << 8) | 0xFF;
        const std::uint32_t handler = memory[at] | (memory[(at + 1) & 0xFFFF] << 8);
        if (handler >= kProgramStart) add_entry(p.entries, static_cast<std::uint16_t>(handler), "interrupt");
    }
    p.memory = std::move(memory);
    return p;
}

Program parse_sna(ByteView data) {
    if (data.size() < kSnaHeader + kRam48) throw FormatError(kReader, 0, "too short for a .sna");
    io::ByteReader r(data, "zx.sna");
    Registers regs;
    ShadowRegisters s;
    regs.i = r.u8();
    s.l = r.u8();
    s.h = r.u8();
    s.e = r.u8();
    s.d = r.u8();
    s.c = r.u8();
    s.b = r.u8();
    s.f = r.u8();
    s.a = r.u8();
    regs.l = r.u8();
    regs.h = r.u8();
    regs.e = r.u8();
    regs.d = r.u8();
    regs.c = r.u8();
    regs.b = r.u8();
    regs.iy = r.u16();
    regs.ix = r.u16();
    r.skip(1);                  // IFF2
    regs.r = r.u8();
    regs.f = r.u8();
    regs.a = r.u8();
    regs.sp = r.u16();
    regs.im = r.u8();
    regs.shadow = s;

    Bytes mem(kMemorySize, 0);
    place(mem, 0x4000, data.subspan(kSnaHeader, kRam48));
    std::uint16_t pc;
    if (data.size() >= kSnaHeader + kRam48 + 4) {
        // 128K: the PC follows the 48K block.
        io::ByteReader tail(data, "zx.sna");
        tail.seek(kSnaHeader + kRam48);
        pc = tail.u16();
    } else {
        // 48K: RETN pops the PC off the stack.
        pc = static_cast<std::uint16_t>(mem[regs.sp] | (mem[(regs.sp + 1u) & 0xFFFFu] << 8));
        regs.sp = static_cast<std::uint16_t>(regs.sp + 2);
    }
    return snapshot(std::move(mem), regs, pc);
}

Program parse_z80(ByteView data) {
    io::ByteReader r(data, "zx.z80");
    Registers regs;
    regs.a = r.u8();
    regs.f = r.u8();
    regs.c = r.u8();
    regs.b = r.u8();
    regs.l = r.u8();
    regs.h = r.u8();
    std::uint16_t pc = r.u16();
    regs.sp = r.u16();
    regs.i = r.u8();
    const std::uint8_t r11 = r.u8();
    const std::uint8_t flags = r.u8();   // byte 12: bit 0 is R's bit 7, bit 5 says the memory is compressed
    regs.r = static_cast<std::uint8_t>((r11 & 0x7F) | ((flags & 1) << 7));
    regs.e = r.u8();
    regs.d = r.u8();
    r.seek(23);
    regs.iy = r.u16();
    regs.ix = r.u16();
    r.seek(29);
    regs.im = static_cast<std::uint8_t>(r.u8() & 3);

    Bytes mem(kMemorySize, 0);
    if (pc != 0) {
        // Version 1: 48K, compressed if bit 5 of byte 12.
        ByteView body = data.subspan(kZ80Header);
        Bytes ram;
        if (flags & 0x20) {
            static constexpr std::uint8_t kEnd[4] = {0x00, 0xED, 0xED, 0x00};
            const bool tail = body.size() >= 4 && std::equal(std::begin(kEnd), std::end(kEnd), body.end() - 4);
            ram = unrle(tail ? body.first(body.size() - 4) : body, kRam48);
        } else {
            ram.assign(body.begin(), body.end());
        }
        place(mem, 0x4000, ByteView(ram).first(std::min(ram.size(), kRam48)));
        return snapshot(std::move(mem), regs, pc);
    }

    // Versions 2 and 3: an extra header, then 16 KB pages, each with a length and a page number.
    r.seek(kZ80Header);
    const std::uint16_t extra = r.u16();
    pc = r.u16();
    const std::uint8_t hardware = r.u8();
    const bool v2 = extra == 23;
    const bool is128 = hardware >= (v2 ? 3 : 4) && hardware != 64;
    const unsigned paged = is128 ? (r.u8() & 7u) : 0;
    std::size_t at = 32 + std::size_t{extra};
    std::map<std::uint8_t, Bytes> banks;   // looked up by page number only; a page met again replaces the earlier one
    while (at + 3 <= data.size()) {
        io::ByteReader h(data, "zx.z80 page");
        h.seek(at);
        const std::uint16_t length = h.u16();
        const std::uint8_t page = h.u8();
        at += 3;
        const std::size_t take = length == 0xFFFF ? kPage : length;
        const std::size_t have = std::min(take, data.size() - at);   // a cut-off last page is what is there
        const ByteView raw = data.subspan(at, have);
        at += take;
        banks[page] = length == 0xFFFF ? Bytes(raw.begin(), raw.end()) : unrle(raw, kPage);
    }
    struct Slot {
        unsigned page;
        std::size_t address;
    };
    std::vector<Slot> where;
    if (is128) {
        // Pages 3-10 are RAM banks 0-7: bank 5 sits at $4000, bank 2 at $8000, the paged-in bank at $C000. When bank
        // 5 or 2 is the one paged in at $C000 the machine shows it in both places.
        where = {{3 + 5, 0x4000}, {3 + 2, 0x8000}, {3 + paged, 0xC000}};
    } else {
        where = {{8, 0x4000}, {4, 0x8000}, {5, 0xC000}};
    }
    for (const Slot& s : where) {
        const auto bank = banks.find(static_cast<std::uint8_t>(s.page));
        if (bank != banks.end()) place(mem, s.address, ByteView(bank->second).first(std::min(bank->second.size(), kPage)));
    }
    return snapshot(std::move(mem), regs, pc);
}

// The addresses BASIC passes to USR: the token $C0 followed by a number (its text, then $0E and the five-byte
// form, read as the small-integer form 00 sign lo hi 00).
std::vector<std::uint16_t> usr_calls(ByteView basic) {
    std::vector<std::uint16_t> out;
    std::size_t i = 0;
    while (i < basic.size()) {
        if (basic[i] == kBasicUsrToken) {
            const std::size_t stop = std::min(basic.size(), i + 16);
            const auto marker = std::find(basic.begin() + static_cast<std::ptrdiff_t>(i),
                                          basic.begin() + static_cast<std::ptrdiff_t>(stop), kBasicNumberMarker);
            const std::size_t j = static_cast<std::size_t>(marker - basic.begin());
            if (marker != basic.begin() + static_cast<std::ptrdiff_t>(stop) && j + 6 <= basic.size() && basic[j + 1] == 0) {
                out.push_back(static_cast<std::uint16_t>(basic[j + 3] | (basic[j + 4] << 8)));
                i = j + 6;
                continue;
            }
        }
        ++i;
    }
    return out;
}

Program parse_tap(ByteView data) {
    // Blocks: a length word, then that many bytes (a cut-off last block is what is there).
    std::vector<ByteView> blocks;
    std::size_t at = 0;
    while (at + 2 <= data.size()) {
        const std::size_t n = data[at] | (data[at + 1] << 8);
        const std::size_t have = std::min(n, data.size() - at - 2);
        blocks.push_back(data.subspan(at + 2, have));
        at += 2 + n;
    }
    ByteView header;             // the pending header block, empty if none
    std::optional<std::pair<std::uint16_t, ByteView>> code;   // load address and payload of the biggest code block
    Bytes basic;
    for (ByteView b : blocks) {
        if (b.empty()) continue;
        if (b[0] == 0x00 && b.size() == kTapeHeaderBlockSize) {
            header = b;
            continue;
        }
        if (b[0] == 0xFF && !header.empty()) {
            const std::uint8_t kind = header[1];
            const std::size_t length = header[12] | (header[13] << 8);
            const std::uint16_t param1 = static_cast<std::uint16_t>(header[14] | (header[15] << 8));
            const ByteView payload = b.subspan(1, std::min(length, b.size() - 1));
            if (kind == 0) {
                append(basic, payload);
            } else if (kind == 3 && (!code || payload.size() > code->second.size())) {
                code = {param1, payload};
            }
            header = {};
        }
    }
    if (!code) throw FormatError(kReader, 0, "no code block on the tape");
    const std::vector<std::uint16_t> usr = usr_calls(basic);
    const std::uint32_t origin = code->first;
    const ByteView payload = code->second;
    Program p;
    p.kind = Kind::Tape;
    p.origin = origin;
    p.memory.assign(kMemorySize, 0);
    const ByteView fitted = payload.first(std::min<std::size_t>(payload.size(), kMemorySize - origin));
    place(p.memory, origin, fitted);
    p.image.assign(fitted.begin(), fitted.end());
    for (std::size_t i = 0; i < usr.size(); ++i) {
        if (origin <= usr[i] && usr[i] < origin + payload.size()) {
            add_entry(p.entries, usr[i], i == 0 ? "start" : std::format("usr_{:04X}", usr[i]));
        }
    }
    // No USR found: the block's first byte, as most loaders call it.
    if (p.entries.empty()) p.entries.push_back({static_cast<std::uint16_t>(origin), "start"});
    return p;
}

}  // namespace

std::optional<Format> format_of(const std::filesystem::path& path) {
    const std::string ext = extension_of(path);
    if (ext == ".sna") return Format::Sna;
    if (ext == ".z80") return Format::Z80;
    if (ext == ".tap") return Format::Tap;
    return std::nullopt;
}

Program parse(ByteView data, Format format) {
    switch (format) {
        case Format::Sna: return parse_sna(data);
        case Format::Z80: return parse_z80(data);
        case Format::Tap: return parse_tap(data);
    }
    CB_ASSERT(false, "unknown ZX format");
    return {};
}

Result<Program> try_parse(ByteView data, Format format) {
    try {
        return parse(data, format);
    } catch (const FormatError& e) {   // isolation boundary: a probe's answer is "not this format"
        return Failure(e);
    }
}

Program read(const std::filesystem::path& path) {
    const std::optional<Format> format = format_of(path);
    if (!format) throw UnsupportedError(kReader, 0, "not a Spectrum file: " + path_text(path));
    const fs::MappedFile file(path);
    return parse(file.view(), *format);
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Format> format = format_of(path);
    if (!format) return false;
    try {
        const fs::MappedFile file(path);
        return try_parse(file.view(), *format).ok();
    } catch (const IoError&) {
        return false;
    }
}

}  // namespace cb::formats::zx
