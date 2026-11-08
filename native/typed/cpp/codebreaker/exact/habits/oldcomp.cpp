// codebreaker/exact/habits/oldcomp.cpp -- C++ from codebreaker.exact.habits.oldcomp (see oldcomp.hpp).
//
// Every index and slice goes through py::bytes' operator[] and slice(), which behave as Python's do
// (a negative index counts from the end, a slice is clamped, an index past the end is an IndexError),
// and every condition keeps Python's order, so what is evaluated before a pattern is rejected -- and
// so where an IndexError can come from -- is the same.

#include "codebreaker/exact/habits/oldcomp.hpp"

#include <string>

namespace codebreaker::exact::habits::oldcomp {

namespace {

using namespace std::string_literals;

py::bytes lit(const std::string& s) { return py::bytes(s); }

int64_t s8(int64_t b) { return b > 127 ? b - 256 : b; }

// bytes.hex(" ")
py::str hex_sep(const py::bytes& b) {
    py::str o("");
    bool first = true;
    for (int64_t c : b) {
        o += (first ? py::str("") : py::str(" ")) + py::percent(py::str("%02x"), c);
        first = false;
    }
    return o;
}

}  // namespace

std::tuple<bool, py::list<py::str>> equivalent(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx) {
    int64_t i = 0;
    int64_t j = 0;
    int64_t n = py::len(want);
    int64_t m = py::len(got);
    py::list<std::tuple<int64_t, int64_t>> amap{std::make_tuple<int64_t, int64_t>(0, 0)};
    py::list<std::tuple<int64_t, int64_t>> jumps;
    py::list<py::str> notes;
    std::optional<int64_t> keep_t;   // keep["t"]
    std::optional<int64_t> keep_d;   // keep["d"]

    while (i < n && j < m) {
        if (want[i] == got[j] || fx.contains(j)) {
            i += 1;
            j += 1;
            continue;
        }
        int64_t a = i - 2;
        int64_t b = j - 2;
        // FOR entry test
        if (a >= 0 && want.slice(a, a + 2) == lit("\x8b\x56"s) && want.slice(a + 3, a + 5) == lit("\x3b\x56"s) && want[a + 6] == 0x77 &&
            want[a + 8] == 0x92 && got.slice(b, b + 2) == lit("\x8b\x56"s) && got.slice(b + 3, b + 5) == lit("\x3b\x56"s) && got[b + 6] == 0x72 &&
            got.slice(b + 8, b + 10) == lit("\x8b\x46"s) && want[a + 2] == got[b + 5] && got[b + 5] == got[b + 10] && want[a + 5] == got[b + 2]) {
            jumps.append(std::make_tuple(a + 8 + s8(want[a + 7]), b + 8 + s8(got[b + 7])));
            notes.append(py::percent(py::str("FOR entry test at +%04x"), a));
            i = a + 9;
            j = b + 11;
            amap.append(std::make_tuple(i, j));
            continue;
        }
        // a >= b / a > b as a value
        if (a >= 0 && want.slice(a, a + 2) == lit("\x8b\x56"s) && want.slice(a + 3, a + 5) == lit("\x3b\x56"s) &&
            want.slice(a + 6, a + 9) == lit("\xb9\x00\x00"s) && got.slice(b, b + 2) == lit("\x8b\x56"s) &&
            got.slice(b + 3, b + 5) == lit("\x3b\x56"s) && got.slice(b + 6, b + 9) == lit("\xb9\x00\x00"s) && want[a + 2] == got[b + 5] &&
            want[a + 5] == got[b + 2] &&
            [&] {
                int64_t x = want[a + 9];
                int64_t y = got[b + 9];
                return (x == 0x72 && y == 0x77) || (x == 0x76 && y == 0x73) || (x == 0x77 && y == 0x72) || (x == 0x73 && y == 0x76);
            }()) {
            notes.append(py::percent(py::str("comparison as a value at +%04x"), a));
            i = a + 10;
            j = b + 10;
            continue;
        }
        // the upper bound of a string parameter: the older compiler goes through the address of the pair
        if (want.slice(i, i + 2) == lit("\x8d\x7e"s) && (want[i + 3] == 0xFF || want[i + 3] == 0x8B) && (want[i + 4] == 0x75 || want[i + 4] == 0x55) &&
            want[i + 5] == 2 && got[j] == want[i + 3] && got[j + 1] == want[i + 4] + 1 && s8(got[j + 2]) == s8(want[i + 2]) + 2) {
            notes.append(py::percent(py::str("upper bound of a string parameter at +%04x"), i));
            i += 6;
            j += 3;
            amap.append(std::make_tuple(i, j));
            continue;
        }
        // RETURN: the older compiler writes the procedure's exit in place, the kit's jumps to it
        int64_t r0 = want.slice(i, i + 2) == lit("\x8b\x46"s) ? 3 : 0;   // a function first loads its result: mov ax,[bp-n]
        if (want.slice(i + r0, i + r0 + 2) == lit("\x8d\x66"s) && want[i + r0 + 3] == 0x5D &&
            (want[i + r0 + 4] == 0xCA || want[i + r0 + 4] == 0xC2 || want[i + r0 + 4] == 0xCB || want[i + r0 + 4] == 0xC3) &&
            (got[j] == 0xE9 || got[j] == 0xEB)) {
            int64_t ln = r0 + ((want[i + r0 + 4] == 0xCA || want[i + r0 + 4] == 0xC2) ? 7 : 5);
            int64_t jl = got[j] == 0xE9 ? 3 : 2;
            int64_t tg = j + jl + (jl == 3 ? py::unpack_from1<int64_t>("<h", got, j + 1) : s8(got[j + 1]));
            if (got.slice(tg, tg + ln) == want.slice(i, i + ln)) {
                notes.append(py::percent(py::str("RETURN at +%04x"), i));
                i += ln;
                j += jl;
                amap.append(std::make_tuple(i, j));
                continue;
            }
        }
        // a conditional jump the kit compiler had to make long
        if (0x70 <= want[i] && want[i] <= 0x7F && got[j] == (want[i] ^ 1) && got[j + 1] == 3 && got[j + 2] == 0xE9) {
            jumps.append(std::make_tuple(i + 2 + s8(want[i + 1]), j + 5 + py::unpack_from1<int64_t>("<h", got, j + 3)));
            notes.append(py::percent(py::str("long jump at +%04x"), i));
            i += 2;
            j += 5;
            amap.append(std::make_tuple(i, j));
            continue;
        }
        // a value still in DX: mov dx,[bp+a]  mov di,[m]  mov [di],dx  and then the older compiler goes on
        // with  add dx,[bp+d]  xchg dx,ax  where the kit's loads it again:  mov ax,[bp+a]  add ax,[bp+d]
        if (want.slice(i, i + 2) == lit("\x03\x56"s) && want[i + 3] == 0x92 && got.slice(j, j + 2) == lit("\x8b\x46"s) &&
            got.slice(j + 3, j + 5) == lit("\x03\x46"s) && got[j + 5] == want[i + 2] && i >= 9 && want.slice(i - 9, i - 7) == lit("\x8b\x56"s) &&
            want[i - 7] == got[j + 2] && want.slice(i - 6, i - 4) == lit("\x8b\x3e"s) && want.slice(i - 2, i) == lit("\x89\x15"s)) {
            notes.append(py::percent(py::str("value kept in DX at +%04x"), i));
            i += 4;
            j += 6;
            amap.append(std::make_tuple(i, j));
            continue;
        }
        // a pointer kept over a call: the older compiler saves DI (an outer procedure's variable, loaded by
        // mov di,[bp]  mov di,[di+d]) in a temporary of its own before the call and takes it from there
        // afterwards; the kit's loads the variable again. One temporary more: the frame is two bytes larger.
        if (i == 4 && want[0] == got[0] && got[0] == 0xE8 && (want[4] | want[5] << 8) == (got[4] | got[5] << 8) + 2) {
            keep_t = (-(want[4] | want[5] << 8)) & 0xFF;
            i = j = 6;
            continue;
        }
        if (keep_t && want.slice(i, i + 2) == lit("\x89\x7e"s) && want[i + 2] == *keep_t) {
            int64_t k = want.rfind(lit("\x8b\x7e\x00\x8b\x7d"s), py::max(int64_t{0}, i - 24), i);
            if (k >= 0) {
                keep_d = want[k + 5];
                notes.append(py::percent(py::str("pointer kept over a call at +%04x"), i));
                i += 3;
                amap.append(std::make_tuple(i, j));
                continue;
            }
        }
        if (keep_d && a >= 0 && want.slice(a, a + 2) == lit("\x8b\x7e"s) && want[a + 2] == *keep_t &&
            got.slice(b, b + 6) == lit("\x8b\x7e\x00\x8b\x7d"s) + lit(std::string(1, static_cast<char>(*keep_d)))) {
            i = a + 3;
            j = b + 6;
            amap.append(std::make_tuple(i, j));
            continue;
        }
        // ... and the other way round, where the older compiler's longer code put the target out of reach
        if (0x70 <= got[j] && got[j] <= 0x7F && want[i] == (got[j] ^ 1) && want[i + 1] == 3 && want[i + 2] == 0xE9) {
            jumps.append(std::make_tuple(i + 5 + py::unpack_from1<int64_t>("<h", want, i + 3), j + 2 + s8(got[j + 1])));
            notes.append(py::percent(py::str("long jump at +%04x"), i));
            i += 5;
            j += 2;
            amap.append(std::make_tuple(i, j));
            continue;
        }
        // last: the displacement of a short jump (opcode just before), of a near jump or call - the
        // patterns above come first, because a ModRM byte can look like a jump opcode
        if (i >= 1 && want[i - 1] == got[j - 1] && ((0x70 <= want[i - 1] && want[i - 1] <= 0x7F) || want[i - 1] == 0xEB || want[i - 1] == 0xE2 || want[i - 1] == 0xE3)) {
            jumps.append(std::make_tuple(i + 1 + s8(want[i]), j + 1 + s8(got[j])));
            i += 1;
            j += 1;
            continue;
        }
        int64_t k = (i >= 1 && want[i - 1] == got[j - 1] && (want[i - 1] == 0xE8 || want[i - 1] == 0xE9))
                        ? 1
                        : ((i >= 2 && want[i - 2] == got[j - 2] && (want[i - 2] == 0xE8 || want[i - 2] == 0xE9) && want[i - 1] == got[j - 1]) ? 2 : 0);
        if (k) {
            a = i - k;
            b = j - k;
            int64_t tw = a + 3 + py::unpack_from1<int64_t>("<h", want, a + 1);
            int64_t tg = b + 3 + py::unpack_from1<int64_t>("<h", got, b + 1);
            if (want[a] == 0xE9) {
                jumps.append(std::make_tuple(tw, tg));   // a call's target is compared by name in build.py
            }
            i = a + 3;
            j = b + 3;
            continue;
        }
        return {false, py::list<py::str>{py::percent(py::str("no pattern at +%04x (original %s, compiled %s)"), i, hex_sep(want.slice(i, i + 6)),
                                                       hex_sep(got.slice(j, j + 6)))}};
    }
    if (i != n || j != m) {
        return {false, py::list<py::str>{py::percent(py::str("lengths do not close: %d of %d, %d of %d"), i, n, j, m)}};
    }
    auto mp = [&](int64_t o) {
        std::tuple<int64_t, int64_t> last;
        bool any = false;
        for (const auto& p : amap) {
            if (std::get<0>(p) <= o) {
                last = p;
                any = true;
            }
        }
        if (!any) throw py::IndexError("list index out of range");
        return std::get<1>(last) + (o - std::get<0>(last));
    };
    for (const auto& tt : jumps) {
        int64_t tw = std::get<0>(tt);
        int64_t tg = std::get<1>(tt);
        if (0 <= tw && tw <= n && mp(tw) != tg) {
            return {false, py::list<py::str>{py::percent(py::str("a jump to +%04x goes to +%04x in the compiled code, not +%04x"), tw, tg, mp(tw))}};
        }
    }
    if (py::len(notes)) {
        return {true, notes};
    }
    return {false, py::list<py::str>{py::str("differs only in jump displacements?")}};
}

}  // namespace codebreaker::exact::habits::oldcomp
