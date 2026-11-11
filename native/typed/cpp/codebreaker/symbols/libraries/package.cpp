// codebreaker/symbols/libraries/package.cpp -- typed C++ port of codebreaker/symbols/libraries/__init__.py
// (see package.hpp).
#include "codebreaker/symbols/libraries/package.hpp"

#include "codebreaker/compare/mask.hpp"
#include "codebreaker/symbols/libraries/coff.hpp"
#include "codebreaker/symbols/libraries/omf.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::libraries {

using py::str;

const py::dict<int64_t, Kind>& _KIND() {
    static const py::dict<int64_t, Kind> k = [] {
        py::dict<int64_t, Kind> d;
        d.set(0x014C, Kind(int64_t(32)));
        d.set(0x01F2, Kind(str("powerpc")));
        return d;
    }();
    return k;
}

static Function from_coff(const coff::Function& f) {
    Function o;
    o.name = f.name;
    o.code = f.code;
    if (const auto* b = std::get_if<py::bytearray>(&f.mask)) o.mask = py::bytes(b->raw());
    else o.mask = std::get<py::list<int64_t>>(f.mask);
    o.at = f.at;
    return o;
}

static Function from_omf(const omf::Function& f) {
    Function o;
    o.name = f.name;
    o.code = f.code;
    o.mask = py::bytes(f.mask.raw());
    o.at = f.at;
    return o;
}

std::tuple<Kind, py::list<Function>> functions(const str& path) {
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read();
    }
    if (data.slice_to(8) == py::bytes("!<arch>\n")) {
        py::list<Function> out;
        std::optional<Kind> kind;
        for (const auto& member : coff::members(data)) {
            const py::bytes& body = std::get<1>(member);
            py::list<coff::Function> found;
            try {
                found = coff::functions(body);
            } catch (const py::StructError&) {
                continue;
            } catch (const py::IndexError&) {
                continue;
            }
            if (py::len(found) > 0 && !kind) {
                int64_t machine = std::get<0>(py::unpack_from<int64_t>("<H", body, 0));
                if (auto k = _KIND().get(machine)) kind = *k;
            }
            for (const coff::Function& f : found) out.append(from_coff(f));
        }
        return {kind ? *kind : Kind(int64_t(32)), out};   // kind or 32
    }
    if (data.slice_to(2) == py::bytes("\x4c\x01", 2)) {
        py::list<Function> out;
        for (const coff::Function& f : coff::functions(data)) out.append(from_coff(f));
        return {Kind(int64_t(32)), out};
    }
    if (data.slice_to(2) == py::bytes("\xf2\x01", 2)) {
        py::list<Function> out;
        for (const coff::Function& f : coff::functions(data)) out.append(from_coff(f));
        return {Kind(str("powerpc")), out};
    }
    py::list<Function> out;
    for (const omf::Function& f : omf::functions(data)) out.append(from_omf(f));
    return {Kind(int64_t(16)), out};
}

bool fits(const Function& function, const py::bytes& code, const std::optional<py::bytes>& code_mask) {
    int64_t n = py::len(function.code);
    if (n > py::len(code)) return false;
    py::bytes theirs = (code_mask && py::len(*code_mask) > 0) ? code_mask->slice_to(n) : py::bytes();
    py::bytes padded(theirs.raw() + std::string(static_cast<size_t>(n - py::len(theirs)), '\0'));   // theirs + bytes(n - len(theirs))
    py::list<compare::mask::MaybeMask> masks;
    masks.append(compare::mask::MaybeMask(function.mask));
    masks.append(compare::mask::MaybeMask(compare::mask::Mask(padded)));
    return compare::mask::same(function.code, code.slice_to(n), masks);
}

Index::Index(const py::list<int64_t>& units_) : units(units_) {
    for (int64_t i = 0; i + 1 < py::len(units); i++) {
        auto key = std::make_tuple(units[i], units[i + 1]);
        pairs.setdefault(key, py::list<int64_t>()).append(i);
    }
}

py::list<int64_t> Index::find(const py::list<int64_t>& sig, const py::list<int64_t>& ignore) const {
    std::optional<int64_t> anchor;
    std::optional<int64_t> fewest;
    for (int64_t i = 0; i + 1 < py::len(ignore); i++) {
        if (!ignore[i] && !ignore[i + 1]) {
            auto it = pairs.get(std::make_tuple(sig[i], sig[i + 1]));
            int64_t n = it ? py::len(*it) : 0;
            if (!fewest || n < *fewest) {
                anchor = i;
                fewest = n;
                if (n == 0) break;
            }
        }
    }
    if (!anchor || !fewest || *fewest == 0) return py::list<int64_t>();   // anchor is None or not fewest
    py::list<int64_t> hits;
    auto at = pairs.get(std::make_tuple(sig[*anchor], sig[*anchor + 1]));
    for (int64_t p : *at) {
        int64_t s = p - *anchor;
        if (s < 0 || s + py::len(sig) > py::len(units)) continue;
        if (compare::mask::same_words(units.slice(s, s + py::len(sig)), sig, ignore)) hits.append(s);
    }
    return hits;
}

}  // namespace codebreaker::symbols::libraries
