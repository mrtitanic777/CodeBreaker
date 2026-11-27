// xna/xact.hpp -- the XACT audio an Xbox 360 game ships: wave banks (.xwb) as playable WAV files, sound banks (.xsb) as the cues they hold.
//
//     const xact::WaveBank bank = xact::read_wave_bank(bytes);       bank.name, bank.entries (codec, channels, rate), bank.stream(entry)
//     xact::decode_bank(file, out_dir, ffmpeg)                       every stream as stream_NNN.wav (PCM as it is, XMA decoded by ffmpeg)
//     const xact::SoundBank cues = xact::read_sound_bank(bytes);     cues.wave_banks, cues.cues {name: {waves [[bank, track]], variation, weights, loop, category}}
//
// A wave bank is XACT's container for the sounds a game plays (magic "WBND", byte-reversed "DNBW" on the Xbox 360, whose fields are big-endian;
// the version tells which). Each entry's format is one packed dword: codec (PCM, XMA, ADPCM, WMA), channels, rate, block alignment. A sound bank
// is XACT3's layout as MonoGame's SoundBank / XactSound / XactClip read the PC form, big-endian with the magic "KBDS" where the PC has "SDBK".
// A cue is either simple (one sound) or complex (a list of sounds chosen by variation); a sound plays one wave (simple) or clips of events, the
// play-wave events naming waves -- one, or a list played by variation. Each wave is a wave bank index and a track in that bank. `loop` is the play
// event's loop count (255: forever); `weights` the variation's weight per entry when it has them. Events other than play-wave are skipped by their
// sizes where MonoGame knows them; a sound with one it does not is listed without waves.
//
// XMA2 is not decoded here: ffmpeg does (the XMA2 codec is a WMA Pro derivative that ffmpeg maintains). A stream is wrapped in a RIFF-WAVE(XMA2)
// container natively and handed to it.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "core/ordered_map.hpp"
#include "formats/xwb.hpp"

namespace cb::xna::xact {

inline constexpr std::uint16_t kFormatXma2 = 0x0166;

// ---- wave banks ---------------------------------------------------------------------------------------------------------------

using Codec = formats::xwb::Codec;
using formats::xwb::codec_name;   // "PCM" "XMA" "ADPCM" "WMA"

struct WaveEntry {
    std::uint32_t index = 0;
    Codec codec = Codec::Pcm;
    std::uint32_t channels = 0, rate = 0, align = 0;
    std::uint32_t offset = 0, length = 0;   // where its stream is in the bank's wave data
};

struct WaveBank {
    Bytes data;
    bool big_endian = true;
    std::uint32_t version = 0;
    std::string name;                       // the bank's own name
    std::uint32_t wave_data_offset = 0;
    std::vector<WaveEntry> entries;
    ByteView stream(const WaveEntry& entry) const;
};

// FormatError for bytes that are not a wave bank (or whose byte order cannot be told).
WaveBank read_wave_bank(Bytes data);

// Raw PCM (an Xbox 360's is big-endian: 16-bit samples are swapped) as a RIFF/WAVE file.
Bytes pcm_to_wav(ByteView pcm, std::uint32_t channels, std::uint32_t rate, std::uint32_t bits = 16);

// A raw XMA2 stream in a RIFF-WAVE(XMA2) container.
Bytes wrap_xma2(ByteView data, std::uint32_t channels = 2, std::uint32_t rate = 44100, std::uint32_t block = 0x10000);

// A raw XMA2 stream (or an already wrapped one) decoded to PCM WAV by ffmpeg. ToolError when ffmpeg is missing or fails. -> the size written.
std::uint64_t decode_to_wav(ByteView xma_or_riff, const std::filesystem::path& out_wav, const std::optional<std::filesystem::path>& ffmpeg,
                            std::uint32_t channels = 2, std::uint32_t rate = 44100, std::uint32_t block = 0x10000);

// Every stream of the bank in `bank_file` as a playable file in `out_dir`: stream_NNN.wav (PCM as it is, XMA decoded), and for what cannot be
// decoded the raw stream (stream_NNN.xma / .adpcm / .wma). `say` gets a line for an XMA stream that failed. -> how many WAV files were written.
std::size_t decode_bank(const std::filesystem::path& bank_file, const std::filesystem::path& out_dir, const std::optional<std::filesystem::path>& ffmpeg,
                        const std::function<void(std::string_view)>& say = {});

// ---- sound banks ------------------------------------------------------------------------------------------------------------

struct Cue {
    std::vector<std::pair<std::int64_t, std::int64_t>> waves;   // [bank, track]
    std::int64_t variation = 0;
    std::optional<std::vector<std::int64_t>> weights;
    std::int64_t loop = 0;
    std::int64_t category = 0;
};

struct SoundBank {
    std::vector<std::string> wave_banks;
    OrderedMap<std::string, Cue> cues;   // by name, in the order the bank lists them; a name met twice keeps its place and the later cue
};

// FormatError for bytes that are not a sound bank.
SoundBank read_sound_bank(ByteView data);

// The bank as the JSON document the port reads (`{"wave_banks": [...], "cues": {name: {"waves", "variation", "weights", "loop", "category"}}}`).
std::string cues_json(const SoundBank& bank);

}  // namespace cb::xna::xact
