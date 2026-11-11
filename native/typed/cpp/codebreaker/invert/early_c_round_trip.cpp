// codebreaker/invert/early_c_round_trip.cpp -- C++17 port of codebreaker/invert/early_c_round_trip.py (see the header).

#include "codebreaker/invert/early_c_round_trip.hpp"

#include <algorithm>

#include "codebreaker/compare/mask.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "codebreaker/invert/early_c.hpp"
#include "codebreaker/invert/early_c_check.hpp"

namespace codebreaker::invert::early_c_round_trip {

namespace masks = codebreaker::compare::mask;

const py::str BATTERY = py::str(
    "\n"
    "t01(){int a,b; a=1; b=2;}\n"
    "t02(){int a,b,c; a=1; c=3;}\n"
    "t04(){int a,b,c,d; a=1; d=4;}\n"
    "t07(){int a,b,c; a=b+c+1;}\n"
    "r09(){int a,b; a=1; b=a+a;}\n"
    "r11(){int a,b; a=5; b=a;}\n"
    "r12(){int a,b,c; a=1; b=2; c=a*b;}\n"
    "r13(p)int p;{int a; a=p; return a+1;}\n"
    "r14(){int a,b; a=1; b=2; return a+b;}\n"
    "r10(){int a,b,c; a=1; b=2; c=a+b;}\n"
    "r15(){int a,b,c; a=1; b=2; c=a-b;}\n"
    "r16(){int a,b; a=3; b=a*a;}\n"
    "c01(){int a,b; a=5; b=a;}\n"
    "c02(){int a,b,c; a=11; b=a; c=b;}\n"
    "c03(){int a,b,c,d; a=11; b=22; c=a; d=44;}\n"
    "c04(){int a,b,c,d; a=11; b=22; c=33; d=a;}\n"
    "c05(){int a,b,c; a=11; b=22; c=b;}\n"
    "c06(){int a,b; a=11; b=a; a=99;}\n"
    "c07(){int a,b,c,d; a=11; b=a; c=33; d=c;}\n");

std::optional<std::tuple<py::bytes, std::optional<py::set<int64_t>>>> extract_one(const py::str& csrc) {
    using R = std::tuple<py::bytes, std::optional<py::set<int64_t>>>;
    std::optional<py::bytes> obj = compilers::mc_compile(csrc);
    if (!obj || obj->size() == 0) return std::nullopt;
    auto fns = formats::omf::function_bytes(*obj);
    if (fns.empty()) return std::nullopt;
    auto v = std::get<1>(fns.items()[0]);   // (a copy: items() is a temporary)
    return R(std::get<0>(v), std::get<1>(v));
}

std::tuple<bool, py::str> masked_eq(const py::bytes& a, const py::bytes& b, const std::optional<py::set<int64_t>>& extra_mask) {
    if (a.size() != b.size()) return std::make_tuple(false, py::percent(py::str("len %d/%d"), a.size(), b.size()));
    masks::Mask m = (extra_mask && !extra_mask->empty()) ? masks::Mask(py::bytes(masks::of(*extra_mask, a.size()))) : masks::Mask(py::bytes(std::string(static_cast<size_t>(a.size()), '\0')));
    py::list<masks::MaybeMask> ms{masks::MaybeMask(m), masks::MaybeMask(masks::Mask(early_c_check::external_branches(a)))};
    py::list<int64_t> diffs = masks::differing(a, b, ms);
    bool ok = diffs.empty();
    if (ok) return std::make_tuple(true, py::str("BYTE-EXACT"));
    std::string s = "diff@[";
    for (int64_t i = 0; i < std::min<int64_t>(diffs.size(), 6); ++i) s += (i ? ", " : "") + std::to_string(diffs[i]);
    return std::make_tuple(false, py::str(s + "]"));
}

void main() {
    std::optional<py::bytes> obj = compilers::mc_compile_multi(BATTERY);
    // (omf.function_bytes(None) is {}: the report then counts 0 of 0)
    auto fns = obj ? formats::omf::function_bytes(*obj) : py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>>();
    int64_t ok = 0, tot = 0;
    std::vector<py::str> keys;
    for (const auto& kv : fns.items()) keys.push_back(std::get<0>(kv));
    std::stable_sort(keys.begin(), keys.end(), [](const py::str& x, const py::str& y) { return x.upper() < y.upper(); });
    for (const py::str& k : keys) {
        py::bytes code = std::get<0>(fns.at(k));
        ++tot;
        py::str name = k.lstrip(py::str("_"));
        py::str csrc;
        int64_t fs = 0;
        py::list<int64_t> loc;
        try {
            auto r = early_c::recover(code);
            fs = std::get<0>(r);
            loc = std::get<4>(r);
            csrc = early_c::to_c(name, fs, std::get<1>(r), std::get<2>(r), std::get<3>(r), std::optional<py::list<int64_t>>(loc));
        } catch (const py::Exception& e) {
            py::print(py::percent(py::str("  %-5s INVERSE-FAIL %s"), name, py::str(e.what())));
            continue;
        }
        auto re = extract_one(csrc);
        if (!re) {
            py::print(py::percent(py::str("  %-5s RECOMPILE-FAIL"), name));
            continue;
        }
        auto [good, note] = masked_eq(std::get<0>(*re), code, std::get<1>(*re));
        ok += good;
        std::vector<int64_t> sl(loc.vec().begin(), loc.vec().end());
        std::sort(sl.begin(), sl.end());
        std::string ls = "[";
        for (size_t i = 0; i < sl.size(); ++i) ls += (i ? ", " : "") + std::to_string(sl[i]);
        py::print(py::percent(py::str("  %-5s fs=%-3d loc=%-14s %s"), name, fs, py::str(ls + "]"), note));
    }
    py::print(py::percent(py::str("\nrtlocals: %d/%d round-trip BYTE-EXACT with framemodel local layout"), ok, tot));
}

}  // namespace codebreaker::invert::early_c_round_trip
