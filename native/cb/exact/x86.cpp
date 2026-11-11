#include "cb/exact/x86.hpp"

#include <algorithm>
#include <charconv>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::x86 {
namespace {

namespace cs = cb::disasm::capstone;

// The per-thread Capstone engine: 16-bit x86, default (Intel) syntax, no operand details.
class CapstoneDecoder final : public Decoder16 {
public:
    CapstoneDecoder() {
        const cs::Api& api = cs::api();
        cs::check(api.cs_open(CS_ARCH_X86, CS_MODE_16, &handle_), "cs_open(x86-16)");
        slot_ = api.cs_malloc(handle_);
    }
    ~CapstoneDecoder() override {
        const cs::Api& api = cs::api();
        if (slot_ != nullptr) api.cs_free(slot_, 1);
        if (handle_ != 0) api.cs_close(&handle_);
    }
    CapstoneDecoder(const CapstoneDecoder&) = delete;
    CapstoneDecoder& operator=(const CapstoneDecoder&) = delete;

    std::optional<Insn> first(ByteView code, std::uint32_t address) const override {
        const std::uint8_t* p = code.data();
        std::size_t left = code.size();
        std::uint64_t at = address;
        if (left == 0 || !cs::api().cs_disasm_iter(handle_, &p, &left, &at, slot_)) return std::nullopt;
        return make(*slot_);
    }

    std::vector<Insn> all(ByteView code, std::uint32_t address) const override {
        std::vector<Insn> out;
        const std::uint8_t* p = code.data();
        std::size_t left = code.size();
        std::uint64_t at = address;
        while (left != 0 && cs::api().cs_disasm_iter(handle_, &p, &left, &at, slot_)) out.push_back(make(*slot_));
        return out;
    }

private:
    static Insn make(const cs_insn& c) {
        Insn i;
        i.address = static_cast<std::uint32_t>(c.address);
        i.bytes.assign(c.bytes, c.bytes + c.size);
        i.mnemonic = c.mnemonic;
        i.op_str = c.op_str;
        return i;
    }
    csh handle_ = 0;
    cs_insn* slot_ = nullptr;
};

constexpr std::uint8_t kInt = 0xCD;
constexpr std::uint8_t kEmulatorEsc0 = 0x34, kEmulatorEsc7 = 0x3B;   // INT 34h-3Bh: ESC D8h-DFh
constexpr std::uint8_t kEmulatorSegEsc = 0x3C;                       // ESC with a segment override
constexpr std::uint8_t kEmulatorWait = 0x3D;                         // FWAIT
constexpr std::uint8_t kEsc0 = 0xD8;
constexpr std::size_t kMaxInsnBytes = 16;

ByteView slice(ByteView v, std::uint64_t from, std::uint64_t to) {
    const std::uint64_t n = v.size();
    from = std::min(from, n);
    to = std::min(to, n);
    return to > from ? v.subspan(from, to - from) : ByteView{};
}

Insn make_insn(std::uint32_t address, ByteView bytes, std::string mnemonic, std::string op_str) {
    return Insn{address, Bytes(bytes.begin(), bytes.end()), std::move(mnemonic), std::move(op_str)};
}

// What an 8087 interrupt stands for: the ESC opcode, then the image's bytes from `after` up to `until` (the
// instruction's modrm and operand), decoded as if they sat at `address`.
std::optional<Insn> decode_probe(const Decoder16& dec, std::uint8_t esc, ByteView img, std::uint64_t after,
                                 std::uint64_t until, std::uint32_t address) {
    Bytes probe{esc};
    append(probe, slice(img, after, until));
    return dec.first(probe, address);
}

bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

const char* segment_prefix(std::uint8_t sel) {
    switch (sel & 0xC0) {
        case 0x00: return "es";
        case 0x40: return "cs";
        case 0x80: return "ss";
        default: return "ds";
    }
}

// "+ 0x1234]" in an operand: the displacement of a `cs:[bx + 0x1234]` jump table.
std::optional<std::uint64_t> table_displacement(std::string_view op) {
    for (std::size_t from = 0; (from = op.find("+ 0x", from)) != std::string_view::npos; from += 4) {
        std::size_t digits = from + 4, end = digits;
        while (end < op.size() && is_lower_hex(op[end])) ++end;
        if (end == digits || end >= op.size() || op[end] != ']') continue;
        std::uint64_t v = 0;
        if (std::from_chars(op.data() + digits, op.data() + end, v, 16).ec != std::errc()) return std::nullopt;
        return v;
    }
    return std::nullopt;
}

}  // namespace

std::vector<Insn> Decoder16::all(ByteView code, std::uint32_t address) const {
    std::vector<Insn> out;
    std::size_t done = 0;
    while (done < code.size()) {
        std::optional<Insn> i = first(code.subspan(done), address + static_cast<std::uint32_t>(done));
        if (!i) break;
        done += i->size();
        out.push_back(std::move(*i));
    }
    return out;
}

const Decoder16& capstone_decoder() {
    thread_local const CapstoneDecoder decoder;
    return decoder;
}

std::vector<Insn> disasm(ByteView img, std::uint32_t at, std::uint32_t end, bool emulator, const Decoder16& decoder) {
    std::vector<Insn> out;
    const std::uint32_t limit = static_cast<std::uint32_t>(std::min<std::uint64_t>(end, img.size()));
    std::uint32_t a = at;
    while (a < limit) {
        const ByteView pair = slice(img, a, a + 2);
        if (emulator && pair.size() == 2 && pair[0] == kInt) {
            const std::uint8_t n = pair[1];
            if (n >= kEmulatorEsc0 && n <= kEmulatorEsc7) {
                // INT 34h-3Bh stand for ESC D8h-DFh; the instruction's modrm follows the interrupt.
                if (auto x = decode_probe(decoder, static_cast<std::uint8_t>(kEsc0 + n - kEmulatorEsc0), img, a + 2, a + 16, a + 1)) {
                    std::string m = x->mnemonic.starts_with('f') ? x->mnemonic : "f" + x->mnemonic;
                    out.push_back(make_insn(a, slice(img, a, a + 1 + x->size()), std::move(m), x->op_str));
                    a += 1 + static_cast<std::uint32_t>(x->size());
                    continue;
                }
            } else if (n == kEmulatorWait) {
                out.push_back(make_insn(a, pair, "fwait", ""));
                a += 2;
                continue;
            } else if (n == kEmulatorSegEsc && a + 2 < img.size()) {
                // The byte after the interrupt: the segment (top two bits) and the ESC opcode's low three.
                const std::uint8_t sel = img[a + 2];
                if (auto x = decode_probe(decoder, static_cast<std::uint8_t>(kEsc0 | (sel & 7)), img, a + 3, a + 16, a + 2)) {
                    out.push_back(make_insn(a, slice(img, a, a + 2 + x->size()), x->mnemonic,
                                            std::string(segment_prefix(sel)) + ": " + x->op_str));
                    a += 2 + static_cast<std::uint32_t>(x->size());
                    continue;
                }
            }
        }
        std::optional<Insn> x = decoder.first(slice(img, a, std::min<std::uint64_t>(a + kMaxInsnBytes, limit)), a);
        if (!x) {
            out.push_back(make_insn(a, slice(img, a, a + 1), "db", std::format("0x{:02x}", img[a])));
            a += 1;
            continue;
        }
        a += static_cast<std::uint32_t>(x->size());
        out.push_back(std::move(*x));
    }
    return out;
}

std::map<std::uint32_t, std::uint32_t> tables(std::span<const Insn> insns, ByteView img, std::uint32_t lo,
                                              std::uint32_t hi, std::uint32_t base) {
    std::map<std::uint32_t, std::uint32_t> out;
    for (const Insn& i : insns) {
        if (i.mnemonic != "jmp" || i.op_str.find("cs:[") == std::string::npos) continue;
        const std::optional<std::uint64_t> disp = table_displacement(i.op_str);
        if (!disp) continue;
        const std::uint64_t t = base + *disp;
        std::uint64_t k = t;
        // each word of the table is a place in the function's own code
        while (lo <= k && k < hi && k + 2 <= hi && k + 2 <= img.size() &&
               lo <= base + static_cast<std::uint64_t>(img[k] | (img[k + 1] << 8)) &&
               base + static_cast<std::uint64_t>(img[k] | (img[k + 1] << 8)) < hi)
            k += 2;
        if (k > t) out[static_cast<std::uint32_t>(t)] = static_cast<std::uint32_t>(k - t);
    }
    return out;
}

std::vector<Insn> body(ByteView img, std::uint32_t lo, std::uint32_t hi, bool emulator, std::uint32_t base,
                       const Decoder16& decoder) {
    std::vector<Insn> first = disasm(img, lo, hi, emulator, decoder);
    const std::map<std::uint32_t, std::uint32_t> tb = tables(first, img, lo, hi, base);
    if (tb.empty()) return first;
    std::vector<Insn> out;
    std::uint32_t a = lo;
    auto step_to = [&](std::uint32_t t, std::uint32_t table_len) {
        if (t > a) {
            std::vector<Insn> part = disasm(img, a, t, emulator, decoder);
            out.insert(out.end(), std::make_move_iterator(part.begin()), std::make_move_iterator(part.end()));
        }
        a = std::max(a, t + table_len);
    };
    for (const auto& [t, len] : tb) step_to(t, len);   // (sorted: the tables are data, stepped over)
    step_to(hi, 0);
    return out;
}

}  // namespace cb::exact::x86
