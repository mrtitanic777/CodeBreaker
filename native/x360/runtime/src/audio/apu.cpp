// XMA decoder block of the APU (see apu.h).
//
// Context layout (big-endian words; from the title's XMA HAL at 0x82A07C20-0x82A08920, the XDK's xmadecoder.h /
// xmahardwareabstraction.h, cross-checked with Xenia/Nexia's XMA_CONTEXT_DATA where they agree):
//   w0  [0-11] input buffer 0 packets  [12-19] loop count (255 = infinite)  [20] input 0 valid  [21] input 1 valid
//       [22-26] output buffer size (256-byte blocks)  [27-31] output write offset (blocks)
//   w1  [0-11] input buffer 1 packets  [12-13] loop subframe end (inclusive)  [17-19] loop subframe skip
//       [20-23] subframes to decode per pass  [24-26] subframes to skip at the decode position
//       [27-28] sample rate (24000, 32000, 44100, 48000)  [29] stereo  [31] output buffer valid (clear = ring full)
//   w2  [0-25] input read offset (bits from the current buffer's start, packet headers included)
//       [26-30] error status  [31] error set
//   w3  [0-25] loop start (bits)  [26-30] parser error status  [31] parser error set
//   w4  [0-25] loop end (bits)  [26-30] packet metadata  [31] current input buffer
//   w5  input buffer 0 (physical)  w6 input buffer 1  w7 output buffer  w8 work buffer (decoder overlap state)
//   w9  [0-4] output read offset (blocks)
// Output: 16-bit big-endian PCM, 128-sample subframes; a stereo subframe is 2 blocks, samples interleaved L R.

#include "audio/apu.h"

#include "audio/mmio_trap.h"
#include "audio/wmapro.h"
#include "audio/xma_stream.h"
#include "kernel/kernel.h"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bitset>
#include <memory>
#include <mutex>
#include <set>

namespace audio
{
    namespace
    {
        constexpr uint32_t kMmioBase = 0x7FEA0000;
        constexpr uint32_t kMmioSize = 0x10000;
        constexpr uint32_t kRegContextArray = 0x600;
        constexpr uint32_t kRegNextContext = 0x607;
        constexpr uint32_t kRegKick = 0x650, kRegLock = 0x690, kRegClear = 0x6A0, kRegGroups = 10;

        constexpr uint32_t kContextCount = 320;
        constexpr uint32_t kContextSize = 64;
        constexpr uint32_t kPacketBytes = 2048;
        constexpr uint32_t kPacketBits = kPacketBytes * 8;
        constexpr uint32_t kHeaderBits = 32;
        constexpr uint32_t kBlockBytes = 256;
        constexpr uint32_t kSubframeSamples = 128;
        constexpr uint32_t kSubframesPerFrame = 4;
        constexpr uint32_t kNoFrame = 0x7FFF;
        constexpr int kSampleRates[4] = { 24000, 32000, 44100, 48000 };
        constexpr uint32_t kWorkMarker = 0x584D4157; // 'XMAW'

        // CB_XMA_DUMP=1: every decoded frame of context N appended to xma_context<N>.pcm (16-bit little-endian,
        // interleaved, all four subframes as decoded) and each input buffer the first time it is seen saved as
        // xma_input_<physical>.bin, with a log line per frame position.
        const bool g_dump = [] { const char* v = std::getenv("CB_XMA_DUMP"); return v && v[0] == '1'; }();
        // CB_XMA_TRACE=1: one log line per kick of a context (ring positions, subframes produced, input state), with
        // the render driver's tick (mix sample = tick * 256).
        const bool g_trace = [] { const char* v = std::getenv("CB_XMA_TRACE"); return v && v[0] == '1'; }();

        void LogOnce(const std::string& text)
        {
            static std::mutex mutex;
            static std::set<std::string> seen;
            std::lock_guard lock(mutex);
            if (seen.insert(text).second)
                Log("APU: {}", text);
        }

        guest::Memory& Mem() { return guest::g_memory; }
        uint8_t* Physical(uint32_t physical) { return Mem().Translate(guest::PhysicalToWindow(guest::kPhysicalWindow64k, physical)); }

        // A context's words, read once per pass and written back field by field.
        struct Words
        {
            uint32_t w[10];
            uint32_t Get(int word, int shift, int bits) const { return (w[word] >> shift) & ((1u << bits) - 1); }
            void Set(int word, int shift, int bits, uint32_t value)
            {
                const uint32_t mask = ((bits == 32 ? 0u : 1u << bits) - 1) << shift;
                w[word] = (w[word] & ~mask) | ((value << shift) & mask);
            }
        };

        struct FramePosition
        {
            uint32_t buffer = 0, offset = 0; // current-buffer index and bit offset of a frame start
        };

        struct HostContext
        {
            std::unique_ptr<WmaProDecoder> decoder;
            int channels = 0, sampleRate = 0;
            // The frame being output: interleaved 16-bit samples, the next subframe and the subframe limit.
            std::array<int16_t, 2 * WmaProDecoder::kFrameSamples> pcm{};
            uint32_t nextSubframe = 0, subframeLimit = 0;
            bool pending = false;
            FramePosition pendingAt;       // where that frame starts (to detect a repositioning by the title)
            FramePosition after;           // where decoding continues once it is output
            bool afterSwitchesBuffer = false;
            bool loopJump = false;         // `after` is the loop start: skip loop-subframe-skip subframes there
            uint32_t skipNext = 0;         // subframes to drop from the next decoded frame (loop start)
        };

        std::mutex g_mutex;
        std::array<HostContext, kContextCount> g_host;
        std::bitset<kContextCount> g_allocated;
        std::bitset<kContextCount> g_enabled; // the kernel API's XMAEnableContext (apu.h XmaField)
        uint32_t g_arrayVirtual = 0, g_arrayPhysical = 0;

        uint32_t ContextAddress(uint32_t index) { return g_arrayVirtual + index * kContextSize; }

        // ---- Input buffers -------------------------------------------------------------------------------------------

        struct InputView
        {
            const Words& words;
            bool Valid(uint32_t b) const { return words.Get(0, 20 + int(b), 1) != 0; }
            uint32_t Current() const { return words.Get(4, 31, 1); }
            uint32_t Packets(uint32_t b) const { return b == 0 ? words.Get(0, 0, 12) : words.Get(1, 0, 12); }
            const uint8_t* Data(uint32_t b) const { return Physical(words.w[5 + b]); }
            uint32_t Header(uint32_t b, uint32_t packet) const
            {
                const uint8_t* p = Data(b) + size_t(packet) * kPacketBytes;
                return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
            }
            static uint32_t FrameCount(uint32_t header) { return header >> 26; }
            static uint32_t FirstFrame(uint32_t header) { return (header >> 11) & 0x7FFF; } // bits after the header
            static uint32_t Skip(uint32_t header) { return header & 0xFF; }
        };

        // The packet after (buffer, packet) in this stream: packet skip + 1 further, continuing in the other buffer.
        // The stream runs from the current buffer into the other one and never back: the current buffer is consumed
        // (its valid bit cleared) before the hardware reads past the other one.
        bool NextPacket(const InputView& in, uint32_t& buffer, uint32_t& packet, bool& crossed)
        {
            const uint32_t skip = InputView::Skip(in.Header(buffer, packet));
            uint32_t next = packet + skip + 1;
            crossed = false;
            if (next >= in.Packets(buffer))
            {
                if (buffer != in.Current())
                    return false; // past the end of the other buffer
                next -= in.Packets(buffer);
                buffer ^= 1;
                crossed = true;
                if (!in.Valid(buffer) || next >= in.Packets(buffer))
                    return false;
            }
            packet = next;
            return true;
        }

        // Copies `bits` stream bits starting at (buffer, offset) into `out` (MSB first), following packets. False when
        // the data continues into an input buffer that is not valid.
        bool GatherBits(const InputView& in, uint32_t buffer, uint32_t offset, uint32_t bits, std::vector<uint8_t>& out)
        {
            out.assign((bits + 7) / 8 + 8, 0);
            uint32_t packet = offset / kPacketBits;
            uint32_t bit = offset % kPacketBits;
            for (uint32_t done = 0; done < bits;)
            {
                if (bit >= kPacketBits)
                {
                    bool crossed;
                    if (!NextPacket(in, buffer, packet, crossed))
                        return false;
                    bit = kHeaderBits;
                }
                const uint8_t* p = in.Data(buffer) + size_t(packet) * kPacketBytes;
                const uint32_t take = std::min(bits - done, kPacketBits - bit);
                for (uint32_t i = 0; i < take; i++)
                {
                    const uint32_t src = bit + i, dst = done + i;
                    if ((p[src >> 3] >> (7 - (src & 7))) & 1)
                        out[dst >> 3] |= uint8_t(0x80 >> (dst & 7));
                }
                done += take;
                bit += take;
            }
            return true;
        }

        bool PeekLength(const InputView& in, uint32_t buffer, uint32_t offset, uint32_t& length)
        {
            std::vector<uint8_t> b;
            if (!GatherBits(in, buffer, offset, 15, b))
                return false;
            length = uint32_t(b[0]) << 7 | b[1] >> 1;
            return true;
        }

        // Stream position `bits` past (buffer, offset), skipping packet headers. False if it leaves the valid data.
        bool Advance(const InputView& in, uint32_t& buffer, uint32_t& offset, uint32_t bits)
        {
            uint32_t packet = offset / kPacketBits;
            uint32_t bit = offset % kPacketBits + bits;
            while (bit >= kPacketBits)
            {
                const uint32_t over = bit - kPacketBits;
                bool crossed;
                uint32_t b = buffer, p = packet;
                if (!NextPacket(in, b, p, crossed))
                {
                    // Position just past the end of the data: report it in the next buffer, packet 0.
                    if (over == 0 && crossed)
                    {
                        buffer ^= 1;
                        offset = kHeaderBits;
                        return true;
                    }
                    return false;
                }
                buffer = b;
                packet = p;
                bit = kHeaderBits + over;
            }
            offset = packet * kPacketBits + bit;
            return true;
        }

        // Where the frame after the one at `frame` (length `length`) starts: the next frame start in its packet (the
        // packet header's frame count says how many start there), else the first frame of the next packet that has
        // one. `switches` is set when that lies in the other input buffer.
        bool NextFrame(const InputView& in, FramePosition frame, uint32_t length, FramePosition& next, bool& switches,
            bool& ended)
        {
            uint32_t buffer = frame.buffer, offset = frame.offset;
            ended = false;
            if (!Advance(in, buffer, offset, length))
            {
                ended = true; // the data ends inside the next packet chain; resume from the other buffer later
                next = { frame.buffer ^ 1, kHeaderBits };
                switches = true;
                return true;
            }
            uint32_t packet = offset / kPacketBits;
            const uint32_t inPacket = offset % kPacketBits;
            // Frames starting in this packet, walked from the first one.
            if (in.Valid(buffer) && packet < in.Packets(buffer))
            {
                const uint32_t header = in.Header(buffer, packet);
                uint32_t start = kHeaderBits + InputView::FirstFrame(header);
                for (uint32_t i = 0; i < InputView::FrameCount(header) && start < kPacketBits; i++)
                {
                    if (start == inPacket)
                    {
                        next = { buffer, offset };
                        switches = buffer != frame.buffer;
                        return true;
                    }
                    uint32_t len;
                    if (!PeekLength(in, buffer, packet * kPacketBits + start, len) || len == 0 || len == kNoFrame)
                        break;
                    start += len;
                }
            }
            // Next packet with a frame start.
            for (;;)
            {
                bool crossed;
                uint32_t b = buffer, p = packet;
                if (!NextPacket(in, b, p, crossed))
                {
                    ended = true;
                    next = { crossed ? buffer ^ 1 : buffer ^ 1, kHeaderBits };
                    switches = true;
                    return true;
                }
                buffer = b;
                packet = p;
                const uint32_t header = in.Header(buffer, packet);
                if (InputView::FrameCount(header) > 0 && InputView::FirstFrame(header) < kPacketBits - kHeaderBits)
                {
                    next = { buffer, packet * kPacketBits + kHeaderBits + InputView::FirstFrame(header) };
                    switches = buffer != frame.buffer;
                    return true;
                }
            }
        }

        // ---- Decoding ------------------------------------------------------------------------------------------------

        enum class DecodeResult { Frame, Idle, Waiting, Error };

        DecodeResult DecodeNextFrame(uint32_t index, Words& w, HostContext& h)
        {
            const InputView in{ w };
            const uint32_t current = w.Get(4, 31, 1);
            if (!in.Valid(current))
            {
                if (in.Valid(current ^ 1))
                {
                    LogOnce("context starts in the other input buffer (current buffer not valid)");
                    w.Set(4, 31, 1, current ^ 1);
                    return DecodeNextFrame(index, w, h);
                }
                return DecodeResult::Idle;
            }
            FramePosition at{ current, w.Get(2, 0, 26) };
            const uint32_t packets = in.Packets(current);
            if (packets == 0 || at.offset >= packets * kPacketBits)
            {
                LogOnce(std::format("read offset {} outside the input buffer ({} packets)", at.offset, packets));
                return DecodeResult::Error;
            }
            // A position before the packet's first frame (in a header, or in the tail of a frame from the previous
            // packet) moves to that first frame.
            {
                const uint32_t packet = at.offset / kPacketBits;
                const uint32_t header = in.Header(current, packet);
                const uint32_t first = kHeaderBits + InputView::FirstFrame(header);
                if (at.offset % kPacketBits < first)
                {
                    if (InputView::FrameCount(header) == 0 || first >= kPacketBits)
                    {
                        FramePosition next;
                        bool switches, ended;
                        NextFrame(in, { current, packet * kPacketBits + kPacketBits - 1 }, 1, next, switches, ended);
                        if (ended || switches)
                        {
                            // No frame starts in the rest of this buffer: it is consumed.
                            w.Set(0, 20 + int(current), 1, 0);
                            w.Set(4, 31, 1, current ^ 1);
                            w.Set(2, 0, 26, ended ? kHeaderBits : next.offset);
                            return DecodeNextFrame(index, w, h);
                        }
                        at = next;
                    }
                    else
                        at.offset = packet * kPacketBits + first;
                }
            }

            uint32_t length;
            if (!PeekLength(in, at.buffer, at.offset, length))
                return DecodeResult::Waiting;
            if (length == kNoFrame || length < 16)
            {
                LogOnce(std::format("frame length field {:#x} at a frame position", length));
                return DecodeResult::Error;
            }
            std::vector<uint8_t> bits;
            if (!GatherBits(in, at.buffer, at.offset, length, bits))
            {
                LogOnce("a frame continues into an input buffer that is not valid yet (waiting)");
                return DecodeResult::Waiting;
            }

            // Decoder for the context's format.
            const int channels = w.Get(1, 29, 1) ? 2 : 1;
            const int rate = kSampleRates[w.Get(1, 27, 2)];
            if (!h.decoder || h.channels != channels || h.sampleRate != rate)
            {
                if (!h.decoder)
                    h.decoder = std::make_unique<WmaProDecoder>();
                h.decoder->Initialize(channels, rate);
                h.channels = channels;
                h.sampleRate = rate;
            }
            double pcm[WmaProDecoder::kMaxChannels][WmaProDecoder::kFrameSamples];
            WmaProFrameInfo info;
            if (!h.decoder->DecodeFrame(BitReader(bits.data(), 0, length), info, pcm))
            {
                Log("APU: context {}: frame at buffer {} bit {} does not decode: {}", index, at.buffer, at.offset,
                    h.decoder->error());
                w.Set(3, 26, 5, 4); // XMA_PLAYBACK_PARSE_ERROR_BITSTREAM_CORRUPTION
                w.Set(3, 31, 1, 1);
                return DecodeResult::Error;
            }
            for (int i = 0; i < WmaProDecoder::kFrameSamples; i++)
                for (int c = 0; c < channels; c++)
                    h.pcm[size_t(i) * channels + c] = XmaStreamDecoder::ToPcm16(pcm[c][i]);
            if (g_dump)
            {
                static std::set<uint32_t> savedInputs;
                const uint32_t input = w.w[5 + at.buffer];
                if (savedInputs.insert(input).second)
                {
                    if (FILE* f = std::fopen(std::format("xma_input_{:08X}.bin", input).c_str(), "wb"))
                    {
                        std::fwrite(in.Data(at.buffer), 1, size_t(in.Packets(at.buffer)) * kPacketBytes, f);
                        std::fclose(f);
                    }
                }
                if (FILE* f = std::fopen(std::format("xma_context{}.pcm", index).c_str(), "ab"))
                {
                    std::fwrite(h.pcm.data(), 2, size_t(WmaProDecoder::kFrameSamples) * channels, f);
                    std::fclose(f);
                }
                Log("XMA dump: context {} frame: buffer {} (input {:08X}, {} packets) bit {}, {} ch {} Hz, loop count {}",
                    index, at.buffer, input, in.Packets(at.buffer), at.offset, channels, rate, w.Get(0, 12, 8));
            }

            // Subframes to output: all four, fewer at a loop end, minus any skipped at the start.
            h.subframeLimit = kSubframesPerFrame;
            h.nextSubframe = 0;
            const uint32_t loopCount = w.Get(0, 12, 8);
            const bool loopEnd = loopCount > 0 && at.offset == w.Get(4, 0, 26) && at.buffer == current;
            if (loopEnd)
                h.subframeLimit = w.Get(1, 12, 2) + 1;
            uint32_t skip = h.skipNext;
            h.skipNext = 0;
            if (const uint32_t positionSkip = w.Get(1, 24, 3))
            {
                LogOnce("decode position subframe skip used");
                skip += positionSkip;
                w.Set(1, 24, 3, 0);
            }
            h.nextSubframe = std::min(skip, h.subframeLimit);

            h.pending = true;
            h.pendingAt = at;
            h.loopJump = false;
            if (loopEnd)
            {
                // Continue at the loop start, skipping its first subframes; one loop less (255 loops forever).
                h.after = { current, w.Get(3, 0, 26) };
                h.afterSwitchesBuffer = false;
                h.loopJump = true;
                if (loopCount != 255)
                    w.Set(0, 12, 8, loopCount - 1);
            }
            else
            {
                bool ended;
                NextFrame(in, at, length, h.after, h.afterSwitchesBuffer, ended);
            }
            w.Set(2, 0, 26, at.offset);
            w.Set(4, 31, 1, at.buffer);
            return DecodeResult::Frame;
        }

        // Moves the context past the frame just output.
        void FinishFrame(Words& w, HostContext& h)
        {
            h.pending = false;
            if (h.afterSwitchesBuffer)
            {
                // The current buffer is consumed: clear its valid bit and continue in the other one.
                const uint32_t current = w.Get(4, 31, 1);
                w.Set(0, 20 + int(current), 1, 0);
            }
            w.Set(4, 31, 1, h.after.buffer);
            w.Set(2, 0, 26, h.after.offset);
            if (h.loopJump)
                h.skipNext = w.Get(1, 17, 3);
        }

        void WriteSubframe(Words& w, const HostContext& h, uint32_t subframe, uint32_t& writeBlock, uint32_t blocks)
        {
            const uint32_t channels = uint32_t(h.channels);
            const int16_t* src = &h.pcm[size_t(subframe) * kSubframeSamples * channels];
            uint8_t* out = Physical(w.w[7]);
            for (uint32_t block = 0; block < channels; block++)
            {
                uint8_t* dst = out + size_t(writeBlock) * kBlockBytes;
                for (uint32_t i = 0; i < kBlockBytes / 2; i++)
                {
                    const uint16_t s = uint16_t(src[block * (kBlockBytes / 2) + i]);
                    dst[i * 2] = uint8_t(s >> 8);
                    dst[i * 2 + 1] = uint8_t(s);
                }
                writeBlock = (writeBlock + 1) % blocks;
            }
        }

        void Work(uint32_t index)
        {
            HostContext& h = g_host[index];
            const uint32_t address = ContextAddress(index);
            Words w;
            for (int i = 0; i < 10; i++)
                w.w[i] = Mem().Read32(address + i * 4);
            const Words before = w;

            // The work buffer holds the decoder's overlap state on the hardware; the title clears it to reset the
            // decoder (XMAHALFlushData). A marker there tells whether our state still belongs to it.
            const uint32_t work = w.w[8];
            if (work != 0)
            {
                uint8_t* marker = Physical(work);
                const uint32_t expected = kWorkMarker ^ index;
                uint32_t found;
                std::memcpy(&found, marker, 4);
                if (found != expected)
                {
                    if (h.decoder)
                        h.decoder->Reset();
                    h.pending = false;
                    h.skipNext = 0;
                    std::memcpy(marker, &expected, 4);
                }
            }
            // A pending frame belongs to the position it was decoded at; a repositioning by the title drops it.
            if (h.pending && (w.Get(4, 31, 1) != h.pendingAt.buffer || w.Get(2, 0, 26) != h.pendingAt.offset))
                h.pending = false;

            if (!w.Get(1, 31, 1))
                return; // output ring full
            const uint32_t blocks = w.Get(0, 22, 5);
            if (blocks == 0 || w.w[7] == 0)
            {
                LogOnce("kick of a context without an output buffer");
                return;
            }
            uint32_t writeBlock = w.Get(0, 27, 5);
            const uint32_t readBlock = w.Get(9, 0, 5);
            uint32_t freeBlocks = writeBlock == readBlock ? blocks : (readBlock + blocks - writeBlock) % blocks;
            uint32_t perPass = w.Get(1, 20, 4);
            if (perPass == 0)
            {
                LogOnce("subframes-to-decode of 0 (taken as 1)");
                perPass = 1;
            }

            bool stop = false;
            while (!stop)
            {
                const uint32_t channels = h.pending ? uint32_t(h.channels) : (w.Get(1, 29, 1) ? 2u : 1u);
                if (freeBlocks < perPass * channels)
                    break;
                uint32_t produced = 0;
                for (uint32_t s = 0; s < perPass && !stop; s++)
                {
                    while (!h.pending || h.nextSubframe >= h.subframeLimit)
                    {
                        if (h.pending)
                            FinishFrame(w, h);
                        const DecodeResult r = DecodeNextFrame(index, w, h);
                        if (r != DecodeResult::Frame)
                        {
                            if (r == DecodeResult::Error)
                                w.Set(2, 31, 1, w.Get(3, 31, 1));
                            stop = true;
                            break;
                        }
                    }
                    if (stop)
                        break;
                    WriteSubframe(w, h, h.nextSubframe++, writeBlock, blocks);
                    freeBlocks -= uint32_t(h.channels);
                    produced++;
                }
                if (produced == 0)
                    break;
            }
            if (h.pending && h.nextSubframe >= h.subframeLimit)
                FinishFrame(w, h);

            if (g_trace)
                Log("APU: tick {} kick {}: read {} write {}->{} of {} free {}->{} input {}{} cur {} offset {} stop {}",
                    RenderTicks(), index, readBlock, before.Get(0, 27, 5), writeBlock, blocks,
                    writeBlock == readBlock && before.Get(0, 27, 5) == writeBlock ? blocks : 0, freeBlocks,
                    w.Get(0, 20, 1), w.Get(0, 21, 1), w.Get(4, 31, 1), w.Get(2, 0, 26), stop ? 1 : 0);
            w.Set(0, 27, 5, writeBlock);
            if (freeBlocks == 0)
                w.Set(1, 31, 1, 0); // ring full
            for (int i = 0; i < 10; i++)
                if (w.w[i] != before.w[i])
                    Mem().Write32(address + i * 4, w.w[i]);
        }

        void OnRegisterWrite(uint32_t address, uint32_t value)
        {
            const uint32_t reg = (address - kMmioBase) / 4;
            const uint32_t bits = __builtin_bswap32(value); // the HAL stores byte-reversed
            auto forEach = [&](uint32_t base, auto&& f) {
                for (uint32_t m = bits; m; m &= m - 1)
                {
                    const uint32_t index = (reg - base) * 32 + uint32_t(__builtin_ctz(m));
                    if (index < kContextCount)
                        f(index);
                }
            };
            std::lock_guard lock(g_mutex);
            if (reg >= kRegKick && reg < kRegKick + kRegGroups)
            {
                forEach(kRegKick, [](uint32_t i) { Work(i); });
                WriteMmio(address, 0);
            }
            else if (reg >= kRegLock && reg < kRegLock + kRegGroups)
                WriteMmio(address, 0); // decoding is finished at each kick, so a lock is granted at once
            else if (reg >= kRegClear && reg < kRegClear + kRegGroups)
            {
                forEach(kRegClear, [](uint32_t i) {
                    HostContext& h = g_host[i];
                    if (h.decoder)
                        h.decoder->Reset();
                    h.pending = false;
                    h.skipNext = 0;
                });
                WriteMmio(address, 0);
            }
            else if (reg != 0x601)
                LogOnce(std::format("write to APU register {:#x} = {:08X} (no effect)", reg, bits));
        }
    }

    void InitializeApu()
    {
        auto& mem = Mem();
        const uint32_t bytes = kContextCount * kContextSize;
        g_arrayPhysical = mem.physical.Allocate(bytes, 0x1000, 0, 0x1FFFFFFF, true);
        if (g_arrayPhysical == UINT32_MAX)
            Fatal("Unable to allocate the XMA context array");
        mem.CommitPhysical(g_arrayPhysical, (bytes + 0xFFF) & ~0xFFFu);
        g_arrayVirtual = guest::PhysicalToWindow(guest::kPhysicalWindow4k, g_arrayPhysical);
        std::memset(mem.Translate(g_arrayVirtual), 0, bytes);
        mem.HostCommit(kMmioBase, kMmioSize);
        mem.Write32(kMmioBase + kRegContextArray * 4, __builtin_bswap32(g_arrayPhysical));
        mem.Write32(kMmioBase + kRegNextContext * 4, __builtin_bswap32(1));
        // Kick, lock and clear registers live in the page at 0x7FEA1000.
        InstallMmioWriteTrap(kMmioBase + 0x1000, OnRegisterWrite);
    }

    int TestApuStream(const std::vector<uint8_t>& packets, int channels, int sampleRate, std::string& report)
    {
        // Streams `packets` through one context the way XAudio2 does: two ping-pong input buffers of 4 packets that
        // are refilled when the hardware clears their valid bit, a 16-block output ring drained after every kick. The
        // output must equal the plain stream decode exactly.
        auto& mem = Mem();
        constexpr uint32_t kChunkPackets = 4, kRingBlocks = 16;
        static uint32_t inputPhys[2] = {}, outputPhys = 0, workPhys = 0;
        if (!outputPhys)
        {
            for (auto& p : inputPhys)
            {
                p = mem.physical.Allocate(kChunkPackets * kPacketBytes, 0x1000, 0, 0x1FFFFFFF, true);
                mem.CommitPhysical(p, kChunkPackets * kPacketBytes);
            }
            outputPhys = mem.physical.Allocate(0x2000, 0x1000, 0, 0x1FFFFFFF, true);
            mem.CommitPhysical(outputPhys, 0x2000);
            workPhys = mem.physical.Allocate(0x1000, 0x1000, 0, 0x1FFFFFFF, true);
            mem.CommitPhysical(workPhys, 0x1000);
        }
        std::memset(Physical(workPhys), 0, 0x1000);
        const uint32_t context = AllocateXmaContext();
        const uint32_t index = (context - g_arrayVirtual) / kContextSize;
        const uint32_t totalPackets = uint32_t(packets.size() / kPacketBytes);
        uint32_t nextChunk = 0;
        Words w{};
        auto fill = [&](uint32_t buffer) {
            const uint32_t first = nextChunk * kChunkPackets;
            if (first >= totalPackets)
                return false;
            const uint32_t count = std::min(kChunkPackets, totalPackets - first);
            std::memcpy(Physical(inputPhys[buffer]), packets.data() + size_t(first) * kPacketBytes, size_t(count) * kPacketBytes);
            w.Set(buffer == 0 ? 0 : 1, 0, 12, count);
            w.Set(0, 20 + int(buffer), 1, 1);
            nextChunk++;
            return true;
        };
        const uint32_t rateClass = sampleRate <= 24000 ? 0 : sampleRate <= 32000 ? 1 : sampleRate <= 44100 ? 2 : 3;
        fill(0);
        fill(1);
        w.Set(0, 22, 5, kRingBlocks);
        w.Set(1, 20, 4, 4);
        w.Set(1, 27, 2, rateClass);
        w.Set(1, 29, 1, channels == 2);
        w.Set(1, 31, 1, 1);
        w.Set(2, 0, 26, kHeaderBits);
        w.w[5] = inputPhys[0];
        w.w[6] = inputPhys[1];
        w.w[7] = outputPhys;
        w.w[8] = workPhys;
        for (int i = 0; i < 10; i++)
            mem.Write32(context + i * 4, w.w[i]);

        std::vector<int16_t> output;
        for (int pass = 0; pass < 1000000 && output.size() < (packets.size() * 64 + 1000000); pass++)
        {
            {
                std::lock_guard lock(g_mutex);
                Work(index);
            }
            for (int i = 0; i < 10; i++)
                w.w[i] = mem.Read32(context + i * 4);
            // Drain the ring (read offset to the write offset; the ring has room again).
            uint32_t read = w.Get(9, 0, 5);
            const uint32_t write = w.Get(0, 27, 5);
            const bool full = !w.Get(1, 31, 1);
            uint32_t drained = 0;
            while (read != write || (full && drained == 0))
            {
                const uint8_t* block = Physical(outputPhys) + size_t(read) * kBlockBytes;
                for (uint32_t i = 0; i < kBlockBytes; i += 2)
                    output.push_back(int16_t(uint16_t(block[i]) << 8 | block[i + 1]));
                read = (read + 1) % kRingBlocks;
                drained++;
            }
            w.Set(9, 0, 5, read);
            w.Set(1, 31, 1, 1);
            bool refilled = false;
            for (uint32_t b = 0; b < 2; b++)
                if (!w.Get(0, 20 + int(b), 1))
                    refilled |= fill(b);
            for (int i = 0; i < 10; i++)
                mem.Write32(context + i * 4, w.w[i]);
            if (drained == 0 && !refilled)
                break;
        }
        ReleaseXmaContext(context);

        XmaStreamDecoder reference;
        reference.Initialize(channels, sampleRate);
        std::vector<int16_t> expected;
        std::string error;
        reference.DecodeAll(packets, expected, error);
        size_t equal = 0;
        const size_t n = std::min(expected.size(), output.size());
        for (size_t i = 0; i < n; i++)
            equal += expected[i] == output[i];
        const bool ok = output.size() == expected.size() && equal == n;
        report = std::format("context output {} samples, stream decode {}; {} of {} equal{}", output.size() / channels,
            expected.size() / channels, equal, n, ok ? "" : "  FAILED");
        return ok ? 0 : 1;
    }

    int TestApuLoop(const std::vector<uint8_t>& packets, int channels, int sampleRate, std::string& report)
    {
        // The whole stream in one input buffer, frames 10..20 looped twice: loop end at frame 20 (all four subframes),
        // loop start at frame 10 with 4 subframes skipped, so frame 10 only rebuilds the overlap for frame 11. The
        // output must be frames 0..20, 11..20, 11..20, 21.. of the plain stream decode, and the loop count must run
        // down to 0.
        auto& mem = Mem();
        XmaStreamDecoder reference;
        reference.Initialize(channels, sampleRate);
        std::vector<int16_t> plain;
        std::vector<uint32_t> starts;
        std::string error;
        reference.DecodeAll(packets, plain, error, nullptr, &starts);
        const size_t frameValues = size_t(WmaProDecoder::kFrameSamples) * channels;
        if (starts.size() < 30 || packets.size() / kPacketBytes > 0xFFF)
        {
            report = "too short for the loop test, skipped";
            return 0;
        }
        std::vector<int16_t> expected(plain.begin(), plain.begin() + 21 * frameValues);
        for (int loop = 0; loop < 2; loop++)
            expected.insert(expected.end(), plain.begin() + 11 * frameValues, plain.begin() + 21 * frameValues);
        expected.insert(expected.end(), plain.begin() + 21 * frameValues, plain.end());

        static uint32_t inputPhys = 0, outputPhys = 0, workPhys = 0;
        constexpr uint32_t kInputBytes = 0x40000, kRingBlocks = 24;
        if (!inputPhys)
        {
            inputPhys = mem.physical.Allocate(kInputBytes, 0x1000, 0, 0x1FFFFFFF, true);
            mem.CommitPhysical(inputPhys, kInputBytes);
            outputPhys = mem.physical.Allocate(0x2000, 0x1000, 0, 0x1FFFFFFF, true);
            mem.CommitPhysical(outputPhys, 0x2000);
            workPhys = mem.physical.Allocate(0x1000, 0x1000, 0, 0x1FFFFFFF, true);
            mem.CommitPhysical(workPhys, 0x1000);
        }
        if (packets.size() > kInputBytes)
        {
            report = "too long for the loop test, skipped";
            return 0;
        }
        std::memcpy(Physical(inputPhys), packets.data(), packets.size());
        std::memset(Physical(workPhys), 0, 0x1000);
        const uint32_t context = AllocateXmaContext();
        const uint32_t index = (context - g_arrayVirtual) / kContextSize;
        const uint32_t rateClass = sampleRate <= 24000 ? 0 : sampleRate <= 32000 ? 1 : sampleRate <= 44100 ? 2 : 3;
        Words w{};
        w.Set(0, 0, 12, uint32_t(packets.size() / kPacketBytes));
        w.Set(0, 12, 8, 2);
        w.Set(0, 20, 1, 1);
        w.Set(0, 22, 5, kRingBlocks);
        w.Set(1, 12, 2, 3);
        w.Set(1, 17, 3, 4);
        w.Set(1, 20, 4, 2);
        w.Set(1, 27, 2, rateClass);
        w.Set(1, 29, 1, channels == 2);
        w.Set(1, 31, 1, 1);
        w.Set(2, 0, 26, kHeaderBits);
        w.Set(3, 0, 26, starts[10]);
        w.Set(4, 0, 26, starts[20]);
        w.w[5] = inputPhys;
        w.w[7] = outputPhys;
        w.w[8] = workPhys;
        for (int i = 0; i < 10; i++)
            mem.Write32(context + i * 4, w.w[i]);

        std::vector<int16_t> output;
        for (int pass = 0; pass < 100000 && output.size() < expected.size() * 2; pass++)
        {
            {
                std::lock_guard lock(g_mutex);
                Work(index);
            }
            for (int i = 0; i < 10; i++)
                w.w[i] = mem.Read32(context + i * 4);
            uint32_t read = w.Get(9, 0, 5);
            const uint32_t write = w.Get(0, 27, 5);
            const bool full = !w.Get(1, 31, 1);
            uint32_t drained = 0;
            while (read != write || (full && drained == 0))
            {
                const uint8_t* block = Physical(outputPhys) + size_t(read) * kBlockBytes;
                for (uint32_t i = 0; i < kBlockBytes; i += 2)
                    output.push_back(int16_t(uint16_t(block[i]) << 8 | block[i + 1]));
                read = (read + 1) % kRingBlocks;
                drained++;
            }
            w.Set(9, 0, 5, read);
            w.Set(1, 31, 1, 1);
            for (int i = 0; i < 10; i++)
                mem.Write32(context + i * 4, w.w[i]);
            if (drained == 0)
                break;
        }
        const uint32_t loopsLeft = w.Get(0, 12, 8);
        ReleaseXmaContext(context);
        size_t equal = 0;
        const size_t n = std::min(expected.size(), output.size());
        for (size_t i = 0; i < n; i++)
            equal += expected[i] == output[i];
        const bool ok = output.size() == expected.size() && equal == n && loopsLeft == 0;
        report = std::format("looped output {} samples, expected {}; {} of {} equal; loop count left {}{}",
            output.size() / channels, expected.size() / channels, equal, n, loopsLeft, ok ? "" : "  FAILED");
        return ok ? 0 : 1;
    }

    uint32_t AllocateXmaContext()
    {
        std::lock_guard lock(g_mutex);
        for (uint32_t i = 0; i < kContextCount; i++)
            if (!g_allocated[i])
            {
                g_allocated[i] = true;
                g_host[i].pending = false;
                g_host[i].skipNext = 0;
                if (g_host[i].decoder)
                    g_host[i].decoder->Reset();
                return ContextAddress(i);
            }
        return 0;
    }

    void ReleaseXmaContext(uint32_t context)
    {
        std::lock_guard lock(g_mutex);
        if (context < g_arrayVirtual || context >= g_arrayVirtual + kContextCount * kContextSize)
            return;
        const uint32_t index = (context - g_arrayVirtual) / kContextSize;
        std::memset(Mem().Translate(context), 0, kContextSize);
        g_allocated[index] = false;
        g_enabled[index] = false;
        g_host[index].pending = false;
    }

    namespace
    {
        uint32_t IndexOf(uint32_t context)
        {
            if (context < g_arrayVirtual || context >= g_arrayVirtual + kContextCount * kContextSize ||
                (context - g_arrayVirtual) % kContextSize)
                Fatal("XMA: {:08X} is not a hardware context", context);
            return (context - g_arrayVirtual) / kContextSize;
        }
    }

    uint32_t XmaField(uint32_t context, int word, int shift, int bits)
    {
        std::lock_guard lock(g_mutex);
        IndexOf(context);
        Words w;
        w.w[word] = Mem().Read32(context + word * 4);
        return w.Get(word, shift, bits);
    }

    void SetXmaField(uint32_t context, int word, int shift, int bits, uint32_t value, bool kick)
    {
        std::lock_guard lock(g_mutex);
        const uint32_t index = IndexOf(context);
        Words w;
        w.w[word] = Mem().Read32(context + word * 4);
        w.Set(word, shift, bits, value);
        Mem().Write32(context + word * 4, w.w[word]);
        if (kick && g_enabled[index])
            Work(index);
    }

    void EnableXmaContext(uint32_t context)
    {
        std::lock_guard lock(g_mutex);
        const uint32_t index = IndexOf(context);
        g_enabled[index] = true;
        Work(index);
    }

    void DisableXmaContext(uint32_t context)
    {
        std::lock_guard lock(g_mutex);
        g_enabled[IndexOf(context)] = false;
    }
}
