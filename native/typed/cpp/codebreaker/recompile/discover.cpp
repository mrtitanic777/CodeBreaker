// codebreaker/recompile/discover.cpp -- typed C++ port of codebreaker.recompile.discover (see discover.hpp).

#include "codebreaker/recompile/discover.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include "codebreaker/formats/xex.hpp"
#include "codebreaker/recompile/disassemble.hpp"

namespace codebreaker::recompile::discover {

namespace powerpc_image = codebreaker::formats::powerpc_image;
namespace xex = codebreaker::formats::xex;

const py::set<py::str>& _PROLOGUE() {
    static const py::set<py::str> s = {py::str("mflr"), py::str("stwu"), py::str("stw"), py::str("std"), py::str("mr"),
                                       py::str("li"), py::str("lis"), py::str("or"), py::str("addi"), py::str("stmw"),
                                       py::str("b"), py::str("clrlwi"), py::str("rlwinm"), py::str("cmpwi"), py::str("cmplwi"),
                                       py::str("lwz"), py::str("add")};
    return s;
}

const py::set<int64_t>& _XL_ENDERS() {
    static const py::set<int64_t> s = {16, 528, 18};   // bclr, bcctr, rfid
    return s;
}

namespace {

// struct.unpack_from(">I", code, offset)[0]
int64_t be32(const py::bytes& code, int64_t offset) {
    if (offset < 0 || offset + 4 > code.size()) throw py::Exception("unpack_from requires a buffer of at least 4 bytes");
    const std::string& s = code.raw();
    const unsigned char* p = reinterpret_cast<const unsigned char*>(s.data()) + offset;
    return (static_cast<int64_t>(p[0]) << 24) | (static_cast<int64_t>(p[1]) << 16) | (static_cast<int64_t>(p[2]) << 8) | p[3];
}

// the known functions as sorted (start, size) pairs, and whether `a` lies strictly inside one
struct Extents {
    std::vector<std::pair<int64_t, int64_t>> ext;
    explicit Extents(const py::dict<int64_t, int64_t>& known) {
        for (const auto& kv : known.items()) ext.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(ext.begin(), ext.end());
    }
    bool interior(int64_t a) const {
        // i = bisect_right(starts, a) - 1
        const auto it = std::upper_bound(ext.begin(), ext.end(), a, [](int64_t v, const std::pair<int64_t, int64_t>& e) { return v < e.first; });
        const int64_t i = static_cast<int64_t>(it - ext.begin()) - 1;
        return i >= 0 && ext[i].first < a && a < ext[i].first + ext[i].second;
    }
};

}  // namespace

py::ref<powerpc_image::Image> _image(const py::str& path) {
    return powerpc_image::read(xex::is_xex(path) ? xex::basefile(path) : path);
}

bool _plausible_start(const py::bytes& code, int64_t lo, int64_t a) {
    const auto first = disassemble::listing(code, lo, a - lo, 1);
    if (first.empty()) return false;
    const py::str& mnemonic = std::get<2>(first[0]);
    if (mnemonic == py::str(".long")) return false;
    if (_PROLOGUE().contains(mnemonic)) return true;
    if (a - lo >= WORD) {
        const int64_t w = be32(code, a - lo - WORD);
        return w == 0 || w == 0x4E800020 || w == 0x4E800420 || ((w >> 26) == 18 && !(w & 3));
    }
    return false;
}

py::set<int64_t> from_pointers(const powerpc_image::Image& img, const py::dict<int64_t, int64_t>& known) {
    const int64_t lo = std::get<0>(img.code), hi = std::get<1>(img.code);
    const Extents ext(known);
    const py::bytes code = img.bytes_at(lo, hi - lo);
    py::set<int64_t> out;
    for (const auto& s : img.sections) {
        const py::str& name = std::get<0>(s);
        const int64_t va = std::get<1>(s), size = std::get<2>(s), chars = std::get<3>(s);
        if ((chars & 0x20000000) || name == py::str(".text")) continue;
        const py::bytes data = img.bytes_at(va, size);
        for (int64_t o = 0; o < data.size() - 3; o += 4) {
            const int64_t a = be32(data, o);
            if ((a & 3) || !(lo <= a && a < hi) || known.contains(a) || ext.interior(a)) continue;
            if (_plausible_start(code, lo, a)) out.add(a);
        }
    }
    return out;
}

py::set<int64_t> from_constants(const powerpc_image::Image& img, const py::dict<int64_t, int64_t>& known) {
    const int64_t lo = std::get<0>(img.code), hi = std::get<1>(img.code);
    const Extents ext(known);
    const py::bytes code = img.bytes_at(lo, hi - lo);
    py::dict<int64_t, int64_t> high;   // register -> the value its last lis gave it
    py::set<int64_t> out;
    for (int64_t o = 0; o < code.size() - 3; o += 4) {
        const int64_t w = be32(code, o);
        const int64_t op = w >> 26, rd = (w >> 21) & 31, ra = (w >> 16) & 31, imm = w & 0xFFFF;
        int64_t a;
        if (op == 15 && ra == 0) {   // lis rD, imm
            high.set(rd, (imm << 16) & 0xFFFFFFFF);
            continue;
        }
        if (op == 14 && high.contains(ra)) {   // addi rD, rA, simm
            a = (high.at(ra) + ((imm & 0x8000) ? imm - 0x10000 : imm)) & 0xFFFFFFFF;
        } else if (op == 24 && high.contains(rd)) {   // ori rA, rS, uimm (rS is bits 21-25)
            a = high.at(rd) | imm;
        } else {
            continue;
        }
        if ((a & 3) || !(lo <= a && a < hi) || known.contains(a) || ext.interior(a) || out.contains(a)) continue;
        if (_plausible_start(code, lo, a)) out.add(a);
    }
    return out;
}

Result discover(const py::str& path, bool pointer_scan, const std::optional<py::list<int64_t>>& extra) {
    py::ref<powerpc_image::Image> img = _image(path);
    py::dict<int64_t, int64_t> funcs;
    py::dict<py::str, int64_t> sources;
    for (const char* k : {"pdata", "entry", "calls", "pointers", "constants", "seed"}) sources.set(py::str(k), 0);
    if (extra) {
        for (int64_t a : *extra) {
            if (!funcs.contains(a)) {
                funcs.set(a, 0);
                sources.set(py::str("seed"), sources.at(py::str("seed")) + 1);
            }
        }
    }
    for (const auto& kv : img->extents.items()) funcs.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)));
    sources.set(py::str("pdata"), img->extents.size());
    if (img->entry && !funcs.contains(img->entry)) {
        funcs.set(img->entry, 0);
        sources.set(py::str("entry"), 1);
    }
    auto sized = [&funcs]() {
        py::dict<int64_t, int64_t> d;
        for (const auto& kv : funcs.items()) {
            if (std::get<1>(kv)) d.set(std::get<0>(kv), std::get<1>(kv));
        }
        return d;
    };
    if (pointer_scan) {
        for (int64_t a : py::sorted(from_pointers(*img, sized()))) {   // (in address order)
            if (!funcs.contains(a)) {
                funcs.set(a, 0);
                sources.set(py::str("pointers"), sources.at(py::str("pointers")) + 1);
            }
        }
        for (int64_t a : py::sorted(from_constants(*img, sized()))) {   // (in address order)
            if (!funcs.contains(a)) {
                funcs.set(a, 0);
                sources.set(py::str("constants"), sources.at(py::str("constants")) + 1);
            }
        }
    }
    const int64_t lo = std::get<0>(img->code), hi = std::get<1>(img->code);
    const py::bytes code = img->bytes_at(lo, hi - lo);
    // follow every bl from every function to a fixed point (raw opcode bits: a bl is opcode 18 with LK)
    std::vector<int64_t> work;
    py::set<int64_t> seen;
    for (const auto& kv : funcs.items()) {
        work.push_back(std::get<0>(kv));
        seen.add(std::get<0>(kv));
    }
    while (!work.empty()) {
        const int64_t va = work.back();
        work.pop_back();
        if (!(lo <= va && va < hi)) continue;
        const int64_t size = funcs.contains(va) ? funcs.at(va) : 0;
        const int64_t end = size ? std::min(va + size, hi) : std::min(hi, va + 0x4000);
        int64_t off = va - lo;
        while (lo + off + WORD <= end) {
            const int64_t w = be32(code, off);
            const int64_t op = w >> 26;
            if (op == 18) {
                int64_t li = w & 0x03FFFFFC;
                if (li & 0x02000000) li -= 0x04000000;
                if (!(w & 2)) {
                    const int64_t t = lo + off + li;
                    if (w & 1) {
                        if (!seen.contains(t) && lo <= t && t < hi && !img->imports.contains(t)) {
                            seen.add(t);
                            if (!funcs.contains(t)) funcs.set(t, 0);
                            work.push_back(t);
                            sources.set(py::str("calls"), sources.at(py::str("calls")) + 1);
                        }
                    } else if (!size) {   // a plain branch ends a run with no record
                        break;
                    }
                }
            } else if (!size && op == 19 && _XL_ENDERS().contains((w >> 1) & 0x3FF)) {
                break;
            }
            off += WORD;
        }
    }
    return Result(funcs, sources, img);
}

int64_t main(const py::list<py::str>& argv, std::ostream& out) {
    if (argv.empty()) {
        // print(__doc__.split("\n\n")[0])
        out << "discover.py -- every function of an Xbox 360 image, for the recompiler.\n";
        return 2;
    }
    const auto [funcs, sources, img] = discover(argv[0]);
    (void)img;
    py::list<py::str> parts;
    for (const auto& kv : sources.items()) parts.append(py::percent(py::str("%s %d"), std::get<0>(kv), std::get<1>(kv)));
    out << py::percent(py::str("%d functions: %s"), static_cast<int64_t>(funcs.size()), py::join(py::str(", "), parts)).utf8() << "\n";
    bool list = false;
    for (const py::str& a : argv) {
        if (a == py::str("--list")) list = true;
    }
    if (list) {
        std::vector<int64_t> keys;
        for (const auto& kv : funcs.items()) keys.push_back(std::get<0>(kv));
        std::sort(keys.begin(), keys.end());
        for (int64_t a : keys) out << py::percent(py::str("  %08X  %d"), a, funcs.at(a)).utf8() << "\n";
    }
    return 0;
}

}  // namespace codebreaker::recompile::discover
