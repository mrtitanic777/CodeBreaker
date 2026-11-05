// xex_edit.cc -- an Xbox 360 executable's privilege bits, read and changed where they stand:
// codebreaker/formats/xex_edit.py (the code edits and the command stay with the PowerPC
// assembler and the XEX writer in the shim).
#include <algorithm>
#include <vector>

#include "../core/build.h"
#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::xexedit {
namespace {

constexpr int64_t kKeys[2] = {0x00030000, 0x00030100};

struct Found {
    std::vector<int64_t> bits;               // sorted
    int64_t where[2] = {-1, -1};            // the entries' value offsets
};

Found privileges(Bytes data) {
    if (!data.slice_is(0, 4, "XEX2", 4)) raise("ValueError", "not an XEX2 module");
    Found f;
    const int64_t count = data.be32(0x14);
    for (int64_t i = 0; i < count; ++i) {
        const int64_t at = 0x18 + i * 8;
        const int64_t key = data.be32(at), value = data.be32(at + 4);
        for (int k = 0; k < 2; ++k)
            if (key == kKeys[k]) {
                f.where[k] = at + 4;
                for (int b = 0; b < 32; ++b)
                    if ((value >> b) & 1 && std::find(f.bits.begin(), f.bits.end(), 32 * k + b) == f.bits.end())
                        f.bits.push_back(32 * k + b);
            }
    }
    std::sort(f.bits.begin(), f.bits.end());
    return f;
}

int64_t floordiv(int64_t a, int64_t b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0))); }
int64_t floormod(int64_t a, int64_t b) { return a - floordiv(a, b) * b; }

// xex_edit.privileges(data) -> (sorted bits, {key: value offset})
void privileges_q(Call &c) {
    Found f = privileges(c.data);
    Out &o = c.out;
    o.begin_tuple();
    o.begin_list();
    for (int64_t b : f.bits) o.integer(b);
    o.end_list();
    o.begin_dict();
    for (int k = 0; k < 2; ++k)
        if (f.where[k] >= 0) {
            o.integer(kKeys[k]);
            o.integer(f.where[k]);
        }
    o.end_dict();
    o.end_tuple();
}

// xex_edit.set_privileges(data, [(bit, on)]) -> the new bytes
void set_privileges(Call &c) {
    std::string out = c.data.str();
    Found f = privileges(c.data);
    for (const Value &ch : c.arg(0).items) {
        int64_t bit = ch[0].as_int();
        bool on = ch[1].truthy();
        int64_t idx = floordiv(bit, 32);
        if (idx < -2 || idx > 1) raise("IndexError", "tuple index out of range");
        int k = idx < 0 ? static_cast<int>(idx + 2) : static_cast<int>(idx);
        if (f.where[k] < 0)
            raise("ValueError", "the header has no privilege entry for bit " + dec(bit) + " (key " + hex(kKeys[k], true) + ")");
        Bytes cur(out);
        uint32_t value = cur.be32(f.where[k]);
        uint32_t mask = uint32_t(1) << floormod(bit, 32);
        uint32_t nv = on ? (value | mask) : (value & ~mask);
        for (int j = 0; j < 4; ++j) out[f.where[k] + j] = char(nv >> (24 - 8 * j));
    }
    c.out.bytes(Bytes(out));
}

}  // namespace

CBF_QUESTIONS(xexedit) {
    add("xex_edit.privileges", privileges_q);
    add("xex_edit.set_privileges", set_privileges);
}

}  // namespace cbf::xexedit
