#include "cb/exact/fixupp.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::fixupp {

namespace {

constexpr std::uint8_t kFixupBit = 0x80;        // a subrecord is a fix-up (else a thread definition)
constexpr std::uint8_t kFrameThreadBit = 0x40;  // a thread definition is of a frame (else of a target)
constexpr std::uint8_t kSegmentRelativeBit = 0x40;
constexpr std::uint8_t kFixupRecord = 0x9C;
constexpr std::uint8_t kFirstFrameMethodWithoutIndex = 4;   // frame threads of method 4 and above carry no index
constexpr std::uint8_t kIndexedMethods = 3;                 // frame methods 0-2 name their frame by an index
constexpr const char* kReader = "omf.fixupp";

std::uint8_t byte_at(ByteView r, std::size_t pos) {
    if (pos >= r.size()) throw TruncatedError(kReader, pos, 1, 0);
    return r[pos];
}

}  // namespace

const Threads::Def& Threads::get(bool frame, std::uint8_t number) const {
    const std::optional<Def>& d = defs_[frame ? 1 : 0][number & 3];
    if (!d) {
        throw FormatError(kReader, 0, std::format("a fix-up names {} thread {} that no record defined",
                                                  frame ? "frame" : "target", number & 3));
    }
    return *d;
}

Index read_index(ByteView r, std::size_t pos) {
    const std::uint8_t first = byte_at(r, pos);
    if (first < 0x80) return {first, pos + 1};
    return {static_cast<std::uint16_t>(((first & 0x7F) << 8) | byte_at(r, pos + 1)), pos + 2};
}

Bytes index_bytes(std::uint16_t value) {
    if (value > 0x7FFF) throw FormatError(kReader, 0, std::format("index {} does not fit an OMF index", value));
    if (value < 0x80) return {static_cast<std::uint8_t>(value)};
    return {static_cast<std::uint8_t>(0x80 | (value >> 8)), static_cast<std::uint8_t>(value & 0xFF)};
}

std::vector<Fixup> parse(ByteView r, Threads& threads) {
    std::vector<Fixup> out;
    std::size_t q = 0;
    while (q < r.size()) {
        const std::uint8_t c = r[q];
        if (!(c & kFixupBit)) {   // THREAD: bit 6 frame/target, bits 2-4 method, bits 0-1 thread number
            ++q;
            const bool frame = (c & kFrameThreadBit) != 0;
            Threads::Def def;
            def.method = (c >> 2) & 7;
            if (!(frame && def.method >= kFirstFrameMethodWithoutIndex)) {
                const Index i = read_index(r, q);
                def.index = i.value;
                q = i.next;
            }
            threads.set(frame, c & 3, def);
            continue;
        }
        Fixup f;
        f.segment_relative = (c & kSegmentRelativeBit) != 0;
        f.location = (c >> 2) & 0xF;
        f.offset = static_cast<std::uint16_t>(((c & 3) << 8) | byte_at(r, q + 1));
        const std::uint8_t fd = byte_at(r, q + 2);   // the fix data: frame method or thread, P, target method or thread
        q += 3;
        if (fd & 0x80) {
            const Threads::Def& d = threads.get(true, (fd >> 4) & 3);
            f.frame_method = d.method;
            f.frame_index = d.index;
        } else {
            f.frame_method = (fd >> 4) & 7;
            if (f.frame_method < kIndexedMethods) {
                const Index i = read_index(r, q);
                f.frame_index = i.value;
                q = i.next;
            }
        }
        if (fd & 8) {
            const Threads::Def& d = threads.get(false, fd & 3);
            if (!d.index) throw FormatError(kReader, q, "a target thread without an index");
            f.target_method = d.method;
            f.target_index = *d.index;
        } else {
            f.target_method = fd & 3;
            const Index i = read_index(r, q);
            f.target_index = i.value;
            q = i.next;
        }
        f.no_displacement = (fd & 4) != 0;
        if (!f.no_displacement) {
            f.displacement = static_cast<std::uint16_t>(byte_at(r, q) | (byte_at(r, q + 1) << 8));
            q += 2;
        }
        out.push_back(f);
    }
    return out;
}

Bytes encode(std::span<const Fixup> fixups, std::uint32_t base) {
    Bytes b;
    for (const Fixup& f : fixups) {
        CB_ASSERT(f.at >= base && f.at - base < 1024, "a fix-up lies outside the data record it is written for");
        const std::uint32_t o = f.at - base;
        if (f.frame_method > 7 || f.target_method > 3) {
            throw FormatError(kReader, f.at, "a thread's method does not fit the fix data byte of a fix-up");
        }
        b.push_back(static_cast<std::uint8_t>(kFixupBit | (f.segment_relative ? kSegmentRelativeBit : 0) |
                                              (f.location << 2) | (o >> 8)));
        b.push_back(static_cast<std::uint8_t>(o & 0xFF));
        b.push_back(static_cast<std::uint8_t>((f.frame_method << 4) | (f.no_displacement ? 4 : 0) | f.target_method));
        if (f.frame_method < kIndexedMethods) {
            if (!f.frame_index) throw FormatError(kReader, f.at, "a frame method that needs an index has none");
            append(b, index_bytes(*f.frame_index));
        }
        append(b, index_bytes(f.target_index));
        if (!f.no_displacement) {
            b.push_back(static_cast<std::uint8_t>(f.displacement & 0xFF));
            b.push_back(static_cast<std::uint8_t>(f.displacement >> 8));
        }
    }
    return formats::omf::record(kFixupRecord, b);
}

}  // namespace cb::exact::fixupp
