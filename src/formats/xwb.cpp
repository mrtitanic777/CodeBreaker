#include "formats/xwb.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/io.hpp"
#include "core/text.hpp"

namespace cb::formats::xwb {

std::string_view codec_name(Codec codec) {
    switch (codec) {
        case Codec::Pcm: return "PCM";
        case Codec::Xma: return "XMA";
        case Codec::Adpcm: return "ADPCM";
        case Codec::Wma: return "WMA";
    }
    return "?";
}

MiniFormat decode_miniformat(std::uint32_t dword) {
    MiniFormat m;
    m.codec = static_cast<Codec>(dword & 0x3);
    m.channels = (dword >> 2) & 0x7;
    m.sample_rate = (dword >> 5) & 0x3FFFF;
    m.block_align = (dword >> 23) & 0xFF;
    m.bits = ((dword >> 31) & 1) != 0 ? 16 : 8;
    return m;
}

std::uint32_t pack_pcm_miniformat(std::uint32_t channels, std::uint32_t rate, std::uint32_t bits) {
    const std::uint32_t block = channels * (bits / 8);
    return ((channels & 0x7) << 2) | ((rate & 0x3FFFF) << 5) | ((block & 0xFF) << 23) | ((bits == 16 ? 1u : 0u) << 31);
}

Header read_header(ByteView data, std::string_view reader) {
    constexpr std::uint32_t kLastPlausibleVersion = 0x10000;
    if (data.size() < 8) throw TruncatedError(reader, 0, 8, data.size());
    const bool magic_ok = (data[0] == 'W' && data[1] == 'B' && data[2] == 'N' && data[3] == 'D') || (data[0] == 'D' && data[1] == 'N' && data[2] == 'B' && data[3] == 'W');
    if (!magic_ok) throw FormatError(reader, 0, "not an XWB");
    Header h;
    // the version is a small number in the right byte order
    const std::uint32_t big = (std::uint32_t{data[4]} << 24) | (std::uint32_t{data[5]} << 16) | (std::uint32_t{data[6]} << 8) | data[7];
    const std::uint32_t little = (std::uint32_t{data[7]} << 24) | (std::uint32_t{data[6]} << 16) | (std::uint32_t{data[5]} << 8) | data[4];
    if (big >= 1 && big <= kLastPlausibleVersion) {
        h.big_endian = true;
        h.version = big;
    } else if (little >= 1 && little <= kLastPlausibleVersion) {
        h.big_endian = false;
        h.version = little;
    } else {
        throw FormatError(reader, 4, "cannot tell the byte order");
    }
    io::ByteReader r(data, reader);
    const auto u32 = [&] { return h.big_endian ? r.u32_be() : r.u32(); };
    r.seek(8);
    if (h.version >= 42) r.skip(4);   // the header version
    h.segment_count = h.version >= 42 ? 5 : 4;
    for (std::uint32_t i = 0; i < h.segment_count; ++i) {
        h.segments[i].offset = u32();
        h.segments[i].length = u32();
    }
    h.wave_data_offset = h.segments[h.segment_count - 1].offset;
    // a position past the end is a damaged bank's: the read there is the truncation
    r.seek(std::min<std::uint64_t>(h.segments[0].offset, r.size()));
    h.flags = u32();
    h.entry_count = u32();
    const ByteView name = r.bytes(h.version >= 42 ? 64 : 16);   // ended by the first zero
    const auto name_end = std::find(name.begin(), name.end(), std::uint8_t{0});
    h.name = text::from_latin1(name.first(static_cast<std::size_t>(name_end - name.begin())));
    h.meta_element_size = u32();
    return h;
}

}  // namespace cb::formats::xwb
