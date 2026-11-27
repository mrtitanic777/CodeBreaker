// formats/xwb.hpp -- the facts of an XACT wave bank (.xwb) that every reader of one needs: how its header says what byte order
// it is in, where its segments are and what it is called, and what the packed format dword of a wave means.
//
// A wave bank's magic is "WBND" (PC, little-endian) or "DNBW" (Xbox 360, big-endian); the version, a small number, tells which
// byte order the fields are in. Version 42 and later add a header version dword and a fifth segment, and name the bank in 64
// bytes where the older ones use 16.
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

#include "core/bytes.hpp"

namespace cb::formats::xwb {

// ---- a wave's format -----------------------------------------------------------------------------------------

enum class Codec : std::uint8_t { Pcm = 0, Xma = 1, Adpcm = 2, Wma = 3 };

// "PCM", "XMA", "ADPCM", "WMA".
std::string_view codec_name(Codec codec);

// What a wave's packed WAVEBANKMINIWAVEFORMAT dword says: tag[0:2] channels[2:5] rate[5:23] blockAlign[23:31] bits[31:32]
// (one bit: 8 or 16).
struct MiniFormat {
    Codec codec = Codec::Pcm;
    std::uint32_t channels = 0;
    std::uint32_t sample_rate = 0;
    std::uint32_t block_align = 0;
    std::uint32_t bits = 8;
};
MiniFormat decode_miniformat(std::uint32_t dword);

// The packed dword of PCM parameters (tag 0), the inverse of decode_miniformat for PCM.
std::uint32_t pack_pcm_miniformat(std::uint32_t channels, std::uint32_t rate, std::uint32_t bits);

// ---- the header ----------------------------------------------------------------------------------------------

struct Segment {
    std::uint32_t offset = 0;
    std::uint32_t length = 0;
};

struct Header {
    bool big_endian = true;
    std::uint32_t version = 0;
    std::uint32_t segment_count = 0;          // 4, or 5 from version 42: BANKDATA, ENTRYMETADATA, SEEKTABLES, (ENTRYNAMES,) ENTRYWAVEDATA
    std::array<Segment, 5> segments{};
    std::uint32_t flags = 0;
    std::uint32_t entry_count = 0;
    std::string name;                         // the bank's own name
    std::uint32_t meta_element_size = 0;      // the stride of the entry table (0: the 24-byte entry)
    std::uint32_t wave_data_offset = 0;       // where the wave data begins: the last segment's offset
};

// FormatError for bytes that are not a wave bank (or whose byte order cannot be told); TruncatedError when the header is cut short.
Header read_header(ByteView data, std::string_view reader = "xwb");

}  // namespace cb::formats::xwb
