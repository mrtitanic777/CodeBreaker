#include "formats/lynx.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <iterator>
#include <format>
#include <optional>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "formats/probe.hpp"

namespace cb::formats::lynx {

namespace {

constexpr std::string_view kReader = "lynx";
constexpr std::uint8_t kLynxMagic[4] = {'L', 'Y', 'N', 'X'};
constexpr std::uint8_t kBs93Magic[4] = {'B', 'S', '9', '3'};

// ---- the first stage's decryption ----------------------------------------------------------------------------------
//
// The boot ROM treats each 51-byte block as a 512-bit little-endian number c and computes c^3 mod N, with N the Lynx public modulus
// (published by the Handy and Felix emulators). The result is 51 bytes: 50 bytes of differences of the plain program (each plain byte
// is the running sum of the bytes before it) and the check byte $15 at the top. Over all blocks the running sum ends at 0 mod 256.

using Limbs = std::array<std::uint32_t, 17>;   // 512 bits and one more limb of room for a sum

constexpr std::string_view kModulusHex = "35b5a3942806d8a22695d771b23cfd561c4a19b6a3b02600365a306e3c4d63381bd41c136489364cf2ba2a58f4fee1fdac7e79";

Limbs modulus() {
    Limbs n{};
    std::size_t bit = 0;
    for (std::size_t i = kModulusHex.size(); i-- > 0;) {
        const char c = kModulusHex[i];
        const std::uint32_t digit = c <= '9' ? static_cast<std::uint32_t>(c - '0') : static_cast<std::uint32_t>(c - 'a' + 10);
        n[bit / 32] |= digit << (bit % 32);
        bit += 4;
    }
    return n;
}

bool less(const Limbs& a, const Limbs& b) {
    for (std::size_t i = a.size(); i-- > 0;) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return false;
}

void subtract(Limbs& a, const Limbs& b) {
    std::uint64_t borrow = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const std::uint64_t v = static_cast<std::uint64_t>(a[i]) - b[i] - borrow;
        a[i] = static_cast<std::uint32_t>(v);
        borrow = (v >> 63) & 1;
    }
}

void add(Limbs& a, const Limbs& b) {
    std::uint64_t carry = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const std::uint64_t v = static_cast<std::uint64_t>(a[i]) + b[i] + carry;
        a[i] = static_cast<std::uint32_t>(v);
        carry = v >> 32;
    }
}

void double_it(Limbs& a) {
    std::uint32_t carry = 0;
    for (std::uint32_t& limb : a) {
        const std::uint32_t next = limb >> 31;
        limb = (limb << 1) | carry;
        carry = next;
    }
}

// a * b mod n by doubling and adding (a, b < n).
Limbs mul_mod(const Limbs& a, const Limbs& b, const Limbs& n) {
    Limbs r{};
    for (std::size_t bit = 512; bit-- > 0;) {
        double_it(r);
        if (!less(r, n)) subtract(r, n);
        if ((b[bit / 32] >> (bit % 32)) & 1) {
            add(r, a);
            if (!less(r, n)) subtract(r, n);
        }
    }
    return r;
}

std::optional<Bytes> decrypt_block(ByteView block, int& accumulator) {
    static const Limbs n = modulus();
    Limbs c{};
    for (std::size_t i = 0; i < kEncryptedBlock; ++i) c[i / 4] |= static_cast<std::uint32_t>(block[i]) << (8 * (i % 4));
    if (!less(c, n)) return std::nullopt;
    const Limbs cube = mul_mod(mul_mod(c, c, n), c, n);
    Bytes out;
    for (std::size_t i = 0; i < kEncryptedBlock; ++i) out.push_back(static_cast<std::uint8_t>(cube[i / 4] >> (8 * (i % 4))));
    for (std::size_t i = kEncryptedBlock; i < 68; ++i) {
        if (static_cast<std::uint8_t>(cube[i / 4] >> (8 * (i % 4))) != 0) return std::nullopt;   // more than 51 bytes: not a block
    }
    if (out.back() != 0x15) return std::nullopt;
    out.pop_back();
    Bytes plain;
    for (const std::uint8_t d : out) {
        accumulator += d;
        plain.push_back(static_cast<std::uint8_t>(accumulator));
    }
    return plain;
}

std::string text_field(ByteView data, std::size_t at, std::size_t length) {
    std::string s;
    for (std::size_t i = 0; i < length && data[at + i] != 0; ++i) s.push_back(static_cast<char>(data[at + i]));
    return s;
}

}  // namespace

bool is_cartridge(ByteView data) { return data.size() >= kHeaderSize && std::memcmp(data.data(), kLynxMagic, 4) == 0; }

bool is_bs93(ByteView data) {
    if (data.size() < kBs93Header || std::memcmp(data.data() + 6, kBs93Magic, 4) != 0) return false;
    const std::size_t length = (static_cast<std::size_t>(data[4]) << 8) | data[5];
    return length >= kBs93Header && length <= data.size();   // (some tools leave bytes after the block, none cut it short)
}

Bytes decrypt_blocks(ByteView encrypted, std::size_t count) {
    if (count == 0 || count > kMaxEncryptedBlocks || encrypted.size() < count * kEncryptedBlock) return {};
    Bytes plain;
    int accumulator = 0;
    for (std::size_t i = 0; i < count; ++i) {
        const std::optional<Bytes> part = decrypt_block(encrypted.subspan(i * kEncryptedBlock, kEncryptedBlock), accumulator);
        if (!part) return {};
        plain.insert(plain.end(), part->begin(), part->end());
    }
    if ((accumulator & 0xFF) != 0) return {};
    return plain;
}

void Program::set_loaded(std::vector<Segment> loaded, std::vector<std::uint32_t> entered) {
    std::sort(loaded.begin(), loaded.end(), [](const Segment& a, const Segment& b) { return a.offset < b.offset; });
    segments = std::move(loaded);
    entries = std::move(entered);
}

Bytes Program::memory() const {
    Bytes mem(0x10000, 0);
    if (kind == Kind::Cartridge) {
        std::copy(stage1.begin(), stage1.end(), mem.begin() + kStage1Address);
    }
    for (const Segment& s : segments) {
        for (std::uint32_t i = 0; i < s.size && s.address + i < mem.size() && s.offset + i < file.size(); ++i) mem[s.address + i] = file[s.offset + i];
    }
    return mem;
}

std::optional<std::uint32_t> Program::address_of(std::uint32_t offset) const {
    std::optional<std::uint32_t> found;
    std::size_t owner = 0;
    for (std::size_t i = 0; i < segments.size(); ++i) {
        const Segment& s = segments[i];
        if (offset >= s.offset && offset - s.offset < s.size) {
            found = s.address + (offset - s.offset);
            owner = i;
        }
    }
    if (!found) return std::nullopt;
    for (std::size_t j = owner + 1; j < segments.size(); ++j) {   // a later segment over the same address wins
        const Segment& t = segments[j];
        if (*found >= t.address && *found - t.address < t.size) return std::nullopt;
    }
    return found;
}

std::vector<std::uint32_t> Program::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    std::set<std::uint32_t> starts;
    for (const std::uint32_t e : entries) {
        if (e < kRamEnd) starts.insert(e);
    }
    return find_functions(mem, std::move(starts), decode, {.begin = 0x0100, .end = kRamEnd});
}

Program parse(ByteView data) {
    Program p;
    p.file.assign(data.begin(), data.end());
    if (is_cartridge(data)) {
        p.kind = Kind::Cartridge;
        io::ByteReader r(data, kReader);
        r.seek(4);
        p.bank0_block = r.u16();
        p.bank1_block = r.u16();
        p.version = r.u16();
        p.name = text_field(data, 10, 32);
        p.manufacturer = text_field(data, 42, 16);
        p.rotation = data[58];
        p.audin = data[59];
        p.eeprom = data[60];
        const ByteView cart = data.subspan(kHeaderSize);
        if (!cart.empty()) {
            const std::size_t count = 0x100u - cart[0];
            if (count >= 1 && count <= kMaxEncryptedBlocks && cart.size() >= 1 + count * kEncryptedBlock) {
                p.stage1 = decrypt_blocks(cart.subspan(1), count);
                if (!p.stage1.empty()) p.encrypted_blocks = static_cast<std::uint32_t>(count);
            }
        }
        return p;
    }
    if (is_bs93(data)) {
        p.kind = Kind::Bs93;
        p.load_address = static_cast<std::uint16_t>((data[2] << 8) | data[3]);
        p.block_length = static_cast<std::uint16_t>((data[4] << 8) | data[5]);
        const std::uint32_t body = p.block_length - kBs93Header;
        p.segments = {{kBs93Header, p.load_address, body}};
        p.entries = {p.load_address};
        return p;
    }
    throw FormatError(kReader, 0, "not a Lynx cartridge image (LYNX) or a BS93 object");
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    if (!head) return false;
    if (is_cartridge(*head)) return true;
    if (head->size() < kBs93Header || std::memcmp(head->data() + 6, kBs93Magic, 4) != 0) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    const std::size_t length = (static_cast<std::size_t>((*head)[4]) << 8) | (*head)[5];
    return size && length >= kBs93Header && length <= *size;
}

const std::map<std::uint32_t, std::string>& registers() {
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t;
        // Suzy: the sprite engine's registers (low and high byte of each 16-bit one) ...
        const char* const sprite[] = {"TMPADR",  "TILTACUM", "HOFF",    "VOFF",    "VIDBAS",  "COLLBAS", "VIDADR",  "COLLADR", "SCBNEXT",  "SPRDLINE", "HPOSSTRT", "VPOSSTRT",
                                      "SPRHSIZ", "SPRVSIZ",  "STRETCH", "TILT",    "SPRDOFF", "SPRVPOS", "COLLOFF", "VSIZACUM", "HSIZOFF", "VSIZOFF", "SCBADR",   "PROCADR"};
        for (std::uint32_t i = 0; i < std::size(sprite); ++i) {
            t[0xFC00 + 2 * i] = std::string(sprite[i]) + "L";
            t[0xFC01 + 2 * i] = std::string(sprite[i]) + "H";
        }
        // ... the math unit's ...
        t[0xFC52] = "MATHD"; t[0xFC53] = "MATHC"; t[0xFC54] = "MATHB"; t[0xFC55] = "MATHA"; t[0xFC56] = "MATHP"; t[0xFC57] = "MATHN";
        t[0xFC60] = "MATHH"; t[0xFC61] = "MATHG"; t[0xFC62] = "MATHF"; t[0xFC63] = "MATHE";
        t[0xFC6C] = "MATHM"; t[0xFC6D] = "MATHL"; t[0xFC6E] = "MATHK"; t[0xFC6F] = "MATHJ";
        // ... the control registers, the joystick and the cartridge.
        t[0xFC80] = "SPRCTL0"; t[0xFC81] = "SPRCTL1"; t[0xFC82] = "SPRCOLL"; t[0xFC83] = "SPRINIT";
        t[0xFC88] = "SUZYHREV"; t[0xFC89] = "SUZYSREV"; t[0xFC90] = "SUZYBUSEN"; t[0xFC91] = "SPRGO"; t[0xFC92] = "SPRSYS";
        t[0xFCB0] = "JOYSTICK"; t[0xFCB1] = "SWITCHES"; t[0xFCB2] = "RCART0"; t[0xFCB3] = "RCART1";
        t[0xFCC0] = "LEDS"; t[0xFCC2] = "PPORTSTAT"; t[0xFCC3] = "PPORTDATA"; t[0xFCC4] = "HOWIE";
        // Mikey: eight timers of four registers, four audio channels of eight ...
        for (std::uint32_t n = 0; n < 8; ++n) {
            t[0xFD00 + 4 * n] = std::format("TIM{}BKUP", n);
            t[0xFD01 + 4 * n] = std::format("TIM{}CTLA", n);
            t[0xFD02 + 4 * n] = std::format("TIM{}CNT", n);
            t[0xFD03 + 4 * n] = std::format("TIM{}CTLB", n);
        }
        const char* const audio[] = {"VOL", "SHFTFB", "OUTVAL", "L8SHFT", "TBACK", "CTL", "COUNT", "MISC"};
        for (std::uint32_t c = 0; c < 4; ++c) {
            for (std::uint32_t r = 0; r < std::size(audio); ++r) t[0xFD20 + 8 * c + r] = std::format("AUD{}{}", c, audio[r]);
        }
        // ... the attenuation, panning and stereo registers, interrupts, system control, the UART, the display and the palette.
        t[0xFD40] = "ATTEN_A"; t[0xFD41] = "ATTEN_B"; t[0xFD42] = "ATTEN_C"; t[0xFD43] = "ATTEN_D"; t[0xFD44] = "MPAN"; t[0xFD50] = "MSTEREO";
        t[0xFD80] = "INTRST"; t[0xFD81] = "INTSET"; t[0xFD84] = "MAGRDY0"; t[0xFD85] = "MAGRDY1"; t[0xFD86] = "AUDIN"; t[0xFD87] = "SYSCTL1";
        t[0xFD88] = "MIKEYHREV"; t[0xFD89] = "MIKEYSREV"; t[0xFD8A] = "IODIR"; t[0xFD8B] = "IODAT"; t[0xFD8C] = "SERCTL"; t[0xFD8D] = "SERDAT";
        t[0xFD90] = "SDONEACK"; t[0xFD91] = "CPUSLEEP"; t[0xFD92] = "DISPCTL"; t[0xFD93] = "PBKUP"; t[0xFD94] = "DISPADRL"; t[0xFD95] = "DISPADRH";
        t[0xFD9C] = "MTEST0"; t[0xFD9D] = "MTEST1"; t[0xFD9E] = "MTEST2";
        for (std::uint32_t i = 0; i < 16; ++i) {
            t[0xFDA0 + i] = std::format("GREEN{:X}", i);
            t[0xFDB0 + i] = std::format("BLUERED{:X}", i);
        }
        t[0xFFF9] = "MAPCTL";
        return t;
    }();
    return table;
}

}  // namespace cb::formats::lynx
