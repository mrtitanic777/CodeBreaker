#include "cb/exact/habits/oldcomp.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <optional>
#include <string_view>

namespace cb::exact::habits::oldcomp {

namespace {

// Positions are signed: a pattern looks behind the first differing byte (`i - 2`), and a jump target can fall
// before the start of the procedure.
using Pos = std::int64_t;

// A procedure's bytes with bounds-checked probing. A byte or a window that is not there never matches anything:
// `at` answers -1, `is` answers false.
class Code {
  public:
    explicit Code(ByteView bytes) : bytes_(bytes) {}
    Pos size() const { return static_cast<Pos>(bytes_.size()); }
    int at(Pos i) const { return i >= 0 && i < size() ? bytes_[static_cast<std::size_t>(i)] : -1; }
    // the bytes at i are exactly `pattern`
    bool is(Pos i, std::string_view pattern) const {
        if (i < 0 || i + static_cast<Pos>(pattern.size()) > size()) return false;
        return std::equal(pattern.begin(), pattern.end(), bytes_.begin() + i,
                          [](char p, std::uint8_t b) { return static_cast<std::uint8_t>(p) == b; });
    }
    // the same `n` bytes at i here and at j in `other`
    bool same_window(Pos i, const Code& other, Pos j, Pos n) const {
        if (i < 0 || j < 0 || i + n > size() || j + n > other.size()) return false;
        return std::equal(bytes_.begin() + i, bytes_.begin() + i + n, other.bytes_.begin() + j);
    }
    // a signed byte / a little-endian signed word at i, when they are there
    std::optional<Pos> s8(Pos i) const {
        const int b = at(i);
        if (b < 0) return std::nullopt;
        return b > 127 ? b - 256 : b;
    }
    std::optional<Pos> s16(Pos i) const {
        if (at(i) < 0 || at(i + 1) < 0) return std::nullopt;
        return static_cast<std::int16_t>(at(i) | (at(i + 1) << 8));
    }
    // the last start of `pattern` that lies wholly inside [from, to), or -1
    Pos rfind(std::string_view pattern, Pos from, Pos to) const {
        for (Pos p = std::min(to, size()) - static_cast<Pos>(pattern.size()); p >= std::max<Pos>(from, 0); --p) {
            if (is(p, pattern)) return p;
        }
        return -1;
    }
    // up to n bytes from i, as "xx xx xx" (the end of the procedure cuts it short)
    std::string hex(Pos i, Pos n) const {
        std::string out;
        for (Pos p = i; p < i + n && p < size(); ++p) {
            if (p < 0) continue;
            if (!out.empty()) out += ' ';
            out += std::format("{:02x}", at(p));
        }
        return out;
    }

  private:
    ByteView bytes_;
};

// A jump (or a short stretch of code) is a pair of places: one in each procedure.
struct Place {
    Pos want = 0;
    Pos got = 0;
};

bool is_jcc(int b) { return b >= 0x70 && b <= 0x7F; }

}  // namespace

Verdict equivalent(ByteView want_bytes, ByteView got_bytes, const std::set<std::size_t>& fixups) {
    const Code want(want_bytes);
    const Code got(got_bytes);
    const Pos n = want.size();
    const Pos m = got.size();
    Pos i = 0;
    Pos j = 0;
    std::vector<Place> amap{{0, 0}};   // the places where the two procedures are known to line up again
    std::vector<Place> jumps;          // where a jump of the original goes / where its counterpart does
    Verdict verdict;
    std::vector<std::string>& notes = verdict.found;
    // the pointer-kept-over-a-call habit: the frame temporary (a byte offset from bp), and the outer variable's offset
    std::optional<int> keep_temp;
    std::optional<int> keep_var;

    const auto stop = [&](std::string why) {
        verdict.found.clear();
        verdict.stopped = std::move(why);
        return verdict;
    };

    while (i < n && j < m) {
        if (want.at(i) == got.at(j) || fixups.contains(static_cast<std::size_t>(j))) {
            ++i;
            ++j;
            continue;
        }
        Pos a = i - 2;
        Pos b = j - 2;
        // FOR entry test
        if (a >= 0 && want.is(a, "\x8b\x56") && want.is(a + 3, "\x3b\x56") && want.at(a + 6) == 0x77 && want.at(a + 8) == 0x92 &&
            got.is(b, "\x8b\x56") && got.is(b + 3, "\x3b\x56") && got.at(b + 6) == 0x72 && got.is(b + 8, "\x8b\x46") &&
            want.at(a + 2) == got.at(b + 5) && got.at(b + 5) == got.at(b + 10) && want.at(a + 5) == got.at(b + 2)) {
            jumps.push_back({a + 8 + *want.s8(a + 7), b + 8 + *got.s8(b + 7)});
            notes.push_back(std::format("FOR entry test at +{:04x}", a));
            i = a + 9;
            j = b + 11;
            amap.push_back({i, j});
            continue;
        }
        // a >= b / a > b as a value
        if (a >= 0 && want.is(a, "\x8b\x56") && want.is(a + 3, "\x3b\x56") && want.is(a + 6, std::string_view("\xb9\x00\x00", 3)) &&
            got.is(b, "\x8b\x56") && got.is(b + 3, "\x3b\x56") && got.is(b + 6, std::string_view("\xb9\x00\x00", 3)) &&
            want.at(a + 2) == got.at(b + 5) && want.at(a + 5) == got.at(b + 2)) {
            const int x = want.at(a + 9);
            const int y = got.at(b + 9);
            if ((x == 0x72 && y == 0x77) || (x == 0x76 && y == 0x73) || (x == 0x77 && y == 0x72) || (x == 0x73 && y == 0x76)) {
                notes.push_back(std::format("comparison as a value at +{:04x}", a));
                i = a + 10;
                j = b + 10;
                continue;
            }
        }
        // the upper bound of a string parameter: the older compiler goes through the address of the pair
        if (want.is(i, "\x8d\x7e") && (want.at(i + 3) == 0xFF || want.at(i + 3) == 0x8B) &&
            (want.at(i + 4) == 0x75 || want.at(i + 4) == 0x55) && want.at(i + 5) == 2 && got.at(j) == want.at(i + 3) &&
            got.at(j + 1) == want.at(i + 4) + 1 && got.s8(j + 2) && *got.s8(j + 2) == *want.s8(i + 2) + 2) {
            notes.push_back(std::format("upper bound of a string parameter at +{:04x}", i));
            i += 6;
            j += 3;
            amap.push_back({i, j});
            continue;
        }
        // RETURN: the older compiler writes the procedure's exit in place, the kit's jumps to it
        {
            const Pos r0 = want.is(i, "\x8b\x46") ? 3 : 0;   // a function first loads its result: mov ax,[bp-n]
            const int ret = want.at(i + r0 + 4);
            if (want.is(i + r0, "\x8d\x66") && want.at(i + r0 + 3) == 0x5D && (ret == 0xCA || ret == 0xC2 || ret == 0xCB || ret == 0xC3) &&
                (got.at(j) == 0xE9 || got.at(j) == 0xEB)) {
                const Pos len = r0 + ((ret == 0xCA || ret == 0xC2) ? 7 : 5);
                const Pos jump_len = got.at(j) == 0xE9 ? 3 : 2;
                const std::optional<Pos> rel = jump_len == 3 ? got.s16(j + 1) : got.s8(j + 1);
                if (rel) {
                    const Pos target = j + jump_len + *rel;
                    if (want.same_window(i, got, target, len)) {
                        notes.push_back(std::format("RETURN at +{:04x}", i));
                        i += len;
                        j += jump_len;
                        amap.push_back({i, j});
                        continue;
                    }
                }
            }
        }
        // a conditional jump the kit compiler had to make long
        if (is_jcc(want.at(i)) && got.at(j) == (want.at(i) ^ 1) && got.at(j + 1) == 3 && got.at(j + 2) == 0xE9 && want.s8(i + 1) &&
            got.s16(j + 3)) {
            jumps.push_back({i + 2 + *want.s8(i + 1), j + 5 + *got.s16(j + 3)});
            notes.push_back(std::format("long jump at +{:04x}", i));
            i += 2;
            j += 5;
            amap.push_back({i, j});
            continue;
        }
        // a value still in DX: mov dx,[bp+a]  mov di,[m]  mov [di],dx  and then the older compiler goes on
        // with  add dx,[bp+d]  xchg dx,ax  where the kit's loads it again:  mov ax,[bp+a]  add ax,[bp+d]
        if (want.is(i, "\x03\x56") && want.at(i + 3) == 0x92 && got.is(j, "\x8b\x46") && got.is(j + 3, "\x03\x46") &&
            got.at(j + 5) == want.at(i + 2) && i >= 9 && want.is(i - 9, "\x8b\x56") && want.at(i - 7) == got.at(j + 2) &&
            want.is(i - 6, "\x8b\x3e") && want.is(i - 2, "\x89\x15")) {
            notes.push_back(std::format("value kept in DX at +{:04x}", i));
            i += 4;
            j += 6;
            amap.push_back({i, j});
            continue;
        }
        // a pointer kept over a call: the older compiler saves DI (an outer procedure's variable, loaded by
        // mov di,[bp]  mov di,[di+d]) in a temporary of its own before the call and takes it from there
        // afterwards; the kit's loads the variable again. One temporary more: the frame is two bytes larger.
        if (i == 4 && j == 4 && want.at(0) == 0xE8 && got.at(0) == 0xE8 && want.at(5) >= 0 && got.at(5) >= 0 &&
            (want.at(4) | want.at(5) << 8) == (got.at(4) | got.at(5) << 8) + 2) {
            keep_temp = (-(want.at(4) | want.at(5) << 8)) & 0xFF;
            i = j = 6;
            continue;
        }
        if (keep_temp && want.is(i, "\x89\x7e") && want.at(i + 2) == *keep_temp) {
            const Pos k = want.rfind(std::string_view("\x8b\x7e\x00\x8b\x7d", 5), std::max<Pos>(0, i - 24), i);
            if (k >= 0) {
                keep_var = want.at(k + 5);
                notes.push_back(std::format("pointer kept over a call at +{:04x}", i));
                i += 3;
                amap.push_back({i, j});
                continue;
            }
        }
        if (keep_var && a >= 0 && want.is(a, "\x8b\x7e") && want.at(a + 2) == *keep_temp &&
            got.is(b, std::string_view("\x8b\x7e\x00\x8b\x7d", 5)) && got.at(b + 5) == *keep_var) {
            i = a + 3;
            j = b + 6;
            amap.push_back({i, j});
            continue;
        }
        // ... and the other way round, where the older compiler's longer code put the target out of reach
        if (is_jcc(got.at(j)) && want.at(i) == (got.at(j) ^ 1) && want.at(i + 1) == 3 && want.at(i + 2) == 0xE9 && want.s16(i + 3) &&
            got.s8(j + 1)) {
            jumps.push_back({i + 5 + *want.s16(i + 3), j + 2 + *got.s8(j + 1)});
            notes.push_back(std::format("long jump at +{:04x}", i));
            i += 5;
            j += 2;
            amap.push_back({i, j});
            continue;
        }
        // last: the displacement of a short jump (opcode just before), of a near jump or call - the
        // patterns above come first, because a ModRM byte can look like a jump opcode
        if (i >= 1 && j >= 1 && want.at(i - 1) == got.at(j - 1) &&
            (is_jcc(want.at(i - 1)) || want.at(i - 1) == 0xEB || want.at(i - 1) == 0xE2 || want.at(i - 1) == 0xE3)) {
            jumps.push_back({i + 1 + *want.s8(i), j + 1 + *got.s8(j)});
            i += 1;
            j += 1;
            continue;
        }
        // (an opcode E8/E9 one byte back means the low byte of the displacement differs, two back the high byte)
        const auto near_op = [&](Pos back) {
            return i >= back && j >= back && want.at(i - back) == got.at(j - back) && (want.at(i - back) == 0xE8 || want.at(i - back) == 0xE9) &&
                   (back == 1 || want.at(i - 1) == got.at(j - 1));
        };
        const Pos k = near_op(1) ? 1 : (near_op(2) ? 2 : 0);
        if (k != 0) {
            a = i - k;
            b = j - k;
            if (want.s16(a + 1) && got.s16(b + 1)) {
                if (want.at(a) == 0xE9) {
                    jumps.push_back({a + 3 + *want.s16(a + 1), b + 3 + *got.s16(b + 1)});   // a call's target is compared by name in build
                }
                i = a + 3;
                j = b + 3;
                continue;
            }
        }
        return stop(std::format("no pattern at +{:04x} (original {}, compiled {})", i, want.hex(i, 6), got.hex(j, 6)));
    }
    if (i != n || j != m) return stop(std::format("lengths do not close: {} of {}, {} of {}", i, n, j, m));
    // where a place of the original is in the compiled procedure: the last known alignment at or before it
    const auto aligned = [&](Pos o) {
        const Place* last = &amap.front();
        for (const Place& p : amap) {
            if (p.want <= o) last = &p;
        }
        return last->got + (o - last->want);
    };
    for (const Place& jump : jumps) {
        if (0 <= jump.want && jump.want <= n && aligned(jump.want) != jump.got) {
            return stop(std::format("a jump to +{:04x} goes to +{:04x} in the compiled code, not +{:04x}", jump.want, jump.got,
                                    aligned(jump.want)));
        }
    }
    if (notes.empty()) return stop("differs only in jump displacements?");
    verdict.equivalent = true;
    return verdict;
}

}  // namespace cb::exact::habits::oldcomp
