// codebreaker/exact/xenix_c.cpp -- typed C++ port of codebreaker.exact.xenix_c (see xenix_c.hpp).

#include "codebreaker/exact/xenix_c.hpp"

#include <algorithm>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "json.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::xenix_c {

namespace tcm = codebreaker::exact::toolchain;
namespace cache = codebreaker::base::cache;
using py::str;

const str CACHE_VERSION("x1");

// ---- c1's assembly, revised (Microsoft's c1)
const py::list<std::tuple<str, str>> PAIRS = [] {
    py::list<std::tuple<str, str>> l;
    l.append({str("si"), str("di")});
    l.append({str("dx"), str("si")});
    return l;
}();
const str OPS("add|sub|and|or|xor|sal|shl|shr|sar|neg|not|inc|dec");

namespace {

using Lines = std::vector<str>;

Lines lines_of(const str& t) {
    Lines out;
    for (const str& l : py::split(t, str("\n"))) out.push_back(l);
    return out;
}

str text_of(const Lines& L) {
    py::list<str> l;
    for (const str& s : L) l.append(s);
    return py::join(str("\n"), l);
}

// L[a:b] = repl, as Python's slice assignment (the bounds are clamped)
void splice(Lines& L, int64_t a, int64_t b, const Lines& repl) {
    const int64_t n = static_cast<int64_t>(L.size());
    a = std::min(std::max<int64_t>(a, 0), n);
    b = std::min(std::max<int64_t>(b, a), n);
    L.erase(L.begin() + a, L.begin() + b);
    L.insert(L.begin() + a, repl.begin(), repl.end());
}

// L[k + 1:k + 3] == [x, y]
bool next_two_are(const Lines& L, int64_t k, const str& x, const str& y) {
    const int64_t n = static_cast<int64_t>(L.size());
    return k + 3 <= n && L[static_cast<size_t>(k + 1)] == x && L[static_cast<size_t>(k + 2)] == y;
}

bool full(const str& pattern, const str& s) { return static_cast<bool>(py::re::fullmatch(pattern, s)); }

}  // namespace

str rule_u(const str& t) {
    Lines L = lines_of(t);
    int64_t k = 0;
    while (k < static_cast<int64_t>(L.size())) {
        const std::optional<py::re::Match> m = py::re::fullmatch(str("sub\\t(\\w\\w),(\\w\\w)"), L[static_cast<size_t>(k)]);
        if (m && m->group(1) == m->group(2)) {
            const str hi = m->group(1);
            // lo = {h: l for l, h in PAIRS}.get(hi)
            std::optional<str> lo;
            for (const auto& pr : PAIRS)
                if (std::get<1>(pr) == hi) lo = std::get<0>(pr);
            int64_t j = k;
            while (lo && j > 0 &&
                   (full(str("(?:mov|") + OPS + str(")\\t") + *lo + str("(?:,.*)?"), L[static_cast<size_t>(j - 1)]) ||
                    (j < k && full(str("mov\\tcx,.*"), L[static_cast<size_t>(j - 1)])))) {
                j -= 1;
            }
            if ((lo && j > 0 && j == k) || (lo && j < k && !L[static_cast<size_t>(j)].startswith(str("mov\t")))) {
                j = (lo && j > 0 && full(str("(?:L\\d+:)+mov\\t") + *lo + str(",.*"), L[static_cast<size_t>(j - 1)])) ? j - 1 : k;
            }
            std::optional<py::re::Match> m2;
            if (lo && j < k) m2 = py::re::fullmatch(str("((?:L\\d+:)*)mov\\t(\\w\\w),(.*)"), L[static_cast<size_t>(j)]);
            const bool direct = m2 && m2->group(3) == str("ax") && j + 1 == k && k + 3 < static_cast<int64_t>(L.size()) &&
                                full(str("mov\\t\\S*[(_]\\S*,") + *lo, L[static_cast<size_t>(k + 1)]) &&
                                full(str("mov\\t\\S*[(_]\\S*,") + hi, L[static_cast<size_t>(k + 2)]) &&
                                static_cast<bool>(py::re::match(str("jmp\\t"), L[static_cast<size_t>(k + 3)]));
            if (m2 && m2->group(2) == *lo && !m2->group(3).contains(str("(") + *lo + str(")")) && !direct) {
                const bool pushed = next_two_are(L, k, str("push\t") + *lo, str("push\t") + hi);
                if (j + 1 == k && next_two_are(L, k, str("mov\tax,") + *lo, str("mov\tdx,") + hi)) {
                    Lines nw;
                    if (!(m2->group(3) == str("ax") && m2->group(1).empty())) nw.push_back(m2->group(1) + str("mov\tax,") + m2->group(3));
                    nw.push_back(str("sub\tdx,dx"));
                    splice(L, j, k + 3, nw);
                    k = j + static_cast<int64_t>(nw.size()) - 1;
                } else {
                    Lines nw;
                    if (j + 1 == k) {
                        nw.push_back(m2->group(1) + str("mov\tax,") + m2->group(3));
                    } else {
                        nw.push_back(m2->group(1) + str("mov\t") + hi + str(",") + m2->group(3));
                        for (int64_t q = j + 1; q < k; q++) nw.push_back(L[static_cast<size_t>(q)].replace(*lo, hi));
                        if (!pushed) nw.push_back(str("mov\tax,") + hi);
                    }
                    nw.push_back(str("sub\tdx,dx"));
                    int64_t end;
                    if (pushed) {
                        nw.push_back(str("push\t") + (j + 1 == k ? str("ax") : hi));
                        nw.push_back(str("push\tdx"));
                        end = k + 3;
                    } else {
                        nw.push_back(str("mov\t") + hi + str(",dx"));
                        nw.push_back(str("mov\t") + *lo + str(",ax"));
                        end = k + 1;
                    }
                    splice(L, j, end, nw);
                    k = j + static_cast<int64_t>(nw.size()) - 1;
                }
            }
        }
        k += 1;
    }
    return text_of(L);
}

str rule_d(const str& t_in) {
    str t = py::re::sub(
        str(R"RX((movb\tdx,[^\n]+\n)((?:inc|dec)\t(?:\w\w|[*#]?-?\d+\.\(bp\)|_\w+)\n)(and\tdx,#255\.\n|mov\t(?:si|di),dx\n))RX"),
        str(R"RX(\1\3\2)RX"), t_in);
    const str whole = t;
    auto through_di = [&whole](const py::re::Match& m) -> str {
        const py::list<str> n = py::re::findall(str(R"RX(\|NREG (\d+))RX"), whole.slice(0, m.start()));
        if (n.empty() || n[-1] != str("3")) return m.group(0);
        return str("mov\tdi,") + m.group(1) + str("\nmovb\tax,(di)\ncbw\n") + m.group(2) + str("\t") + m.group(1) + str("\nadd\tax,");
    };
    return py::re::sub(str(R"RX(mov\tax,(\*-?\d+\.\(bp\))\nmov\tbx,ax\nmovb\tax,\(bx\)\ncbw\n(inc|dec)\t\1\nadd\tax,)RX"), through_di, whole);
}

str rule_m(const str& t) {
    const str mem(R"RX((?:[*#]?-?\d+\.)?(?:\+?_\w+)?\((?:bp|bx|si|di)\)|(?:\d+\.\+)?_\w+)RX");
    return py::re::sub(
        str("(^|\\n)((?:L\\d+:)*)(add|sub|and|or|xor)\\t(\\((?:bx|si|di)\\)),(") + mem + str(")(?=\\n)"),
        [](const py::re::Match& m) -> str {
            return m.group(1) + m.group(2) + str("mov\tax,") + m.group(5) + str("\n") + m.group(3) + str("\t") + m.group(4) + str(",ax");
        },
        t);
}

str rule_b(const str& t) {
    return py::re::sub(str(R"RX((^|\n)((?:L\d+:)*)orb\t(ax|dx|bx|cx),(#\d+\.)(?=\n))RX"), str(R"RX(\1\2or\t\3,\4)RX"), t);
}

const py::dict<str, std::function<str(const str&)>> RULES = [] {
    py::dict<str, std::function<str(const str&)>> d;
    d.set(str("U"), [](const str& t) { return rule_u(t); });
    d.set(str("D"), [](const str& t) { return rule_d(t); });
    d.set(str("M"), [](const str& t) { return rule_m(t); });
    d.set(str("B"), [](const str& t) { return rule_b(t); });
    return d;
}();

// ---- c2's block moves (Microsoft's c2), from outside: hooks at codemove
const int64_t JBR = 1, CBR = 2, JMP = 3, LABEL = 4;

_List::_List(xenix::Machine& uc_, int64_t ds) : uc(uc_), b(ds * 16) {}

int64_t _List::w(int64_t a) const {
    const py::bytes m = uc.mem_read(b + a, 2);
    return m.byte_(0) | (m.byte_(1) << 8);
}
void _List::setw(int64_t a, int64_t v) const {
    const int64_t x = v & 0xFFFF;
    std::string s;
    s.push_back(static_cast<char>(x & 0xFF));
    s.push_back(static_cast<char>(x >> 8));
    uc.mem_write(b + a, py::bytes(s));
}
int64_t _List::op(int64_t p) const { return uc.mem_read(b + p, 1).byte_(0); }
int64_t _List::sub(int64_t p) const { return uc.mem_read(b + p + 1, 1).byte_(0); }
int64_t _List::forw(int64_t p) const { return w(p + 2); }
int64_t _List::back(int64_t p) const { return w(p + 4); }
int64_t _List::ref(int64_t p) const { return w(p + 6); }
void _List::link(int64_t a, int64_t b_) const {
    setw(a + 2, b_);
    setw(b_ + 4, a);
}
void _List::unlink(int64_t p) const { link(back(p), forw(p)); }

std::optional<std::tuple<int64_t, int64_t, int64_t>> _one_block(const _List& l, int64_t p1) {
    const int64_t L = l.forw(p1);
    if (l.op(L) != LABEL) return std::nullopt;
    int64_t t = l.ref(p1);
    while (l.op(l.back(t)) == LABEL) t = l.back(t);
    const int64_t bend = l.back(t);
    if (l.op(bend) != JBR && l.op(bend) != JMP) return std::nullopt;
    int64_t p = l.forw(L);
    while (p != bend) {
        if (p == 0 || l.op(p) == JBR || l.op(p) == JMP || l.op(p) == LABEL) return std::nullopt;
        p = l.forw(p);
    }
    return std::make_tuple(L, bend, t);
}

std::optional<int64_t> _swap(const _List& l, int64_t p1) {
    const auto blk = _one_block(l, p1);
    if (!blk) throw py::TypeError("cannot unpack non-iterable NoneType object");
    const int64_t L = std::get<0>(*blk), bend = std::get<1>(*blk), t = std::get<2>(*blk);
    const int64_t T = l.ref(p1);
    if (l.w(L + 0xC) != 1) return std::nullopt;
    int64_t q = l.back(p1);
    int64_t n = 0;
    while (!(l.op(q) == CBR && l.ref(q) == L)) {
        if (q == 0 || l.op(q) == JBR || l.op(q) == JMP || l.op(q) == LABEL || n > 64) return std::nullopt;
        q = l.back(q);
        n += 1;
    }
    const int64_t x = l.forw(q);
    if (x == p1) return std::nullopt;
    const int64_t b0 = l.forw(L);
    l.link(l.back(p1), t);
    l.link(q, b0);
    l.link(bend, L);
    l.link(L, x);
    const int64_t rev = l.uc.mem_read(l.b + 0x106 + l.sub(q), 1).byte_(0);
    l.uc.mem_write(l.b + q + 1, py::bytes(std::string(1, static_cast<char>(rev))));
    const int64_t c = l.w(T + 0xC) - 1;
    l.setw(T + 0xC, c);
    if (c <= 0) l.unlink(T);
    l.setw(0x800, l.w(0x800) + 1);
    return q;
}

py::dict<int64_t, xenix::Hook> c2_hooks(const str& mode) {
    const int64_t DI = tcm::x86_reg(str("DI")), SI = tcm::x86_reg(str("SI")), BP = tcm::x86_reg(str("BP")), DX = tcm::x86_reg(str("DX")),
                  IP = tcm::x86_reg(str("IP"));
    auto pending = std::make_shared<std::vector<int64_t>>();
    const bool swap_on = mode.contains(str("S"));

    xenix::Hook at_ce3 = [=](xenix::Machine& uc, int64_t cs, int64_t ds) {
        (void)cs;
        const _List l(uc, ds);
        const int64_t p1 = uc.reg_read(DI);
        const int64_t bp = uc.reg_read(BP);
        const int64_t p3 = l.w(bp - 6);
        if (p3 == p1 || l.ref(p3) != l.ref(p1) || !_one_block(l, p1)) return;
        if (swap_on) {
            const std::optional<int64_t> q = _swap(l, p1);
            if (q && *q != 0) {
                uc.reg_write(DX, *q);
                uc.reg_write(IP, 0xE5A);
                return;
            }
        }
        pending->push_back(p1);
        l.setw(bp + 4, p3);
        uc.reg_write(DI, p3);
        uc.reg_write(SI, l.back(uc.reg_read(SI)));
        uc.reg_write(IP, 0xD4F);
    };
    xenix::Hook at_e5a = [=](xenix::Machine& uc, int64_t cs, int64_t ds) {
        (void)cs;
        (void)ds;
        if (!pending->empty()) {
            const int64_t v = pending->back();
            pending->pop_back();
            uc.reg_write(DX, v);
        }
    };
    py::dict<int64_t, xenix::Hook> d;
    d.set(0xCE3, at_ce3);
    d.set(0xE5A, at_e5a);
    return d;
}

// ---- the passes, changed as Microsoft's
namespace {

uint32_t u16_at(const std::string& b, int64_t at) {
    if (at < 0 || at + 2 > static_cast<int64_t>(b.size())) throw py::StructError("unpack_from requires a buffer of at least 2 bytes");
    return static_cast<unsigned char>(b[static_cast<size_t>(at)]) | (static_cast<unsigned char>(b[static_cast<size_t>(at + 1)]) << 8);
}

void put_u16(std::string& b, int64_t at, int64_t v) {          // struct.pack_into("<H", b, at, v)
    if (v < 0 || v > 0xFFFF) throw py::StructError("ushort format requires 0 <= number <= 65535");
    if (at < 0 || at + 2 > static_cast<int64_t>(b.size())) throw py::StructError("pack_into requires a buffer of at least 2 bytes");
    b[static_cast<size_t>(at)] = static_cast<char>(v & 0xFF);
    b[static_cast<size_t>(at + 1)] = static_cast<char>(v >> 8);
}

std::string pack_h(int64_t v) {                                 // struct.pack("<h", v)
    if (v < -32768 || v > 32767) throw py::StructError("short format requires (-32768) <= number <= 32767");
    std::string s;
    s.push_back(static_cast<char>(v & 0xFF));
    s.push_back(static_cast<char>((v >> 8) & 0xFF));
    return s;
}

std::string hex(const char* h) { return py::bytes::fromhex(str(h)).raw(); }

// b[a:c] as Python's slice
std::string slice_of(const std::string& b, int64_t a, int64_t c) {
    const int64_t n = static_cast<int64_t>(b.size());
    a = std::min(std::max<int64_t>(a, 0), n);
    c = std::min(std::max<int64_t>(c, a), n);
    return b.substr(static_cast<size_t>(a), static_cast<size_t>(c - a));
}

// b[a:c] = repl
void set_slice(std::string& b, int64_t a, int64_t c, const std::string& repl) {
    const int64_t n = static_cast<int64_t>(b.size());
    a = std::min(std::max<int64_t>(a, 0), n);
    c = std::min(std::max<int64_t>(c, a), n);
    b.replace(static_cast<size_t>(a), static_cast<size_t>(c - a), repl);
}

[[noreturn]] void not_this(const char* which) { throw py::RuntimeError(std::string("xenixc: not the ") + which + " this was written for"); }

}  // namespace

py::bytes _c1(const py::bytes& in) {
    std::string b = in.raw();
    const int64_t q = 16 + static_cast<int64_t>(u16_at(b, 2)) + 0x472;
    if (u16_at(b, q) != 100) not_this("c1");
    put_u16(b, q, 0x7FFF);
    return py::bytes(b);
}

py::bytes _c2(const py::bytes& in) {
    std::string c2 = in.raw();
    struct Fix {
        int64_t at;
        const char* old_;
        const char* new_;
    };
    const Fix C2FIX[] = {{0x2F82, "7417", "74e7"}, {0x332A, "803c017509837ef6ff7418e9ccfe", "803c017405803c3f7504e9cdfe"}};
    for (const Fix& f : C2FIX) {
        const std::string old = hex(f.old_), nw = hex(f.new_);
        if (slice_of(c2, 16 + f.at, 16 + f.at + static_cast<int64_t>(old.size())) != old) not_this("c2");
        set_slice(c2, 16 + f.at, 16 + f.at + static_cast<int64_t>(nw.size()), nw);
    }
    const int64_t tsz = u16_at(c2, 2);
    if (!(u16_at(c2, 16 + 0x1885) == 0x17BD && u16_at(c2, 16 + 0x18E1) == 0x17BD)) not_this("c2");
    // a jump or call through a pointer counts 6 more (the figure for jumps and calls, 17BD, through a stub)
    std::string stub = hex("8b5c0a" "803f40" "7504" "8346fa06") + std::string(1, '\xE9') + pack_h(0x17BD - (tsz + 15));
    set_slice(c2, 16 + tsz, 16 + tsz, stub);
    put_u16(c2, 2, tsz + static_cast<int64_t>(stub.size()));
    put_u16(c2, 16 + 0x1885, tsz);
    put_u16(c2, 16 + 0x18E1, tsz);
    // a constant is not taken from SI or DI while a variable lives there (1DDE)
    const int64_t at4 = tsz + static_cast<int64_t>(stub.size());
    std::string s4;
    auto J = [&](int op, int64_t to) { return std::string(1, static_cast<char>(op)) + pack_h(to - (at4 + static_cast<int64_t>(s4.size()) + 3)); };
    s4 = hex("807f0164" "7503");
    s4 += J(0xE9, 0x1E3B);
    s4 += hex("803e8c092a" "752b" "83ff02" "7c26" "83ff03" "7f21" "8bc7" "48" "39060208" "7f18" "b82800" "50" "b88c09" "50");
    s4 += J(0xE8, 0x410E);
    s4 += hex("83c404" "85c0" "7506" "bfffff");
    s4 += J(0xE9, 0x1E24);
    s4 += J(0xE9, 0x1E64);
    if (slice_of(c2, 16 + 0x1E35, 16 + 0x1E3B) != hex("807f01647529")) not_this("c2");
    set_slice(c2, 16 + at4, 16 + at4, s4);
    stub += s4;
    put_u16(c2, 2, at4 + static_cast<int64_t>(s4.size()));
    set_slice(c2, 16 + 0x1E35, 16 + 0x1E3B, std::string(1, '\xE9') + pack_h(at4 - 0x1E38) + std::string(3, '\x90'));
    // a call through a pointer uses every register (2788, and the fold of a load into its push, 2436)
    const int64_t at5 = tsz + static_cast<int64_t>(stub.size());
    std::string s5 = hex("c6440fff" "8b5c0a" "803f40" "7403");
    s5 += std::string(1, '\xE9') + pack_h(0x2A6E - (at5 + static_cast<int64_t>(s5.size()) + 3));
    s5 += hex("c6440eff");
    s5 += std::string(1, '\xE9') + pack_h(0x2C74 - (at5 + static_cast<int64_t>(s5.size()) + 3));
    const int64_t at6 = at5 + static_cast<int64_t>(s5.size());
    std::string s6 = hex("803c2f" "7403");
    s6 += std::string(1, '\xE9') + pack_h(0x243B - (at6 + static_cast<int64_t>(s6.size()) + 3));
    s6 += hex("8b5c0a" "803f40" "7503");
    s6 += std::string(1, '\xE9') + pack_h(0x2487 - (at6 + static_cast<int64_t>(s6.size()) + 3));
    s6 += std::string(1, '\xE9') + pack_h(0x2453 - (at6 + static_cast<int64_t>(s6.size()) + 3));
    if (slice_of(c2, 16 + 0x2436, 16 + 0x243B) != hex("803c2f7418") || u16_at(c2, 16 + 0x2C52) != 0x2A6A) not_this("c2");
    set_slice(c2, 16 + 0x2436, 16 + 0x243B, std::string(1, '\xE9') + pack_h(at6 - 0x2439) + std::string(2, '\x90'));
    s5 += s6;
    set_slice(c2, 16 + at5, 16 + at5, s5);
    put_u16(c2, 2, at5 + static_cast<int64_t>(s5.size()));
    put_u16(c2, 16 + 0x2C52, at5);
    return py::bytes(c2);
}

// the profile's JSON, as the Python's dict (its keys in the order of the file)
struct XenixC::Raw {
    py::V profile;
};

XenixC::XenixC(const toolchain::Profile& profile, const std::optional<str>& variant_in) : toolchain::Toolchain(profile, std::nullopt) {
    auto raw = std::make_shared<Raw>();
    str text = profile.keyed;
    const str file = py::os::path::join(tcm::HERE(), str("toolchains"), profile.name + str(".json"));
    if (py::os::path::exists(file)) text = py::open_rb(file).read().decode("utf-8");
    raw->profile = py::json_parse(text.utf8());
    raw_ = raw;
    name = profile.name;
    // self.variant = variant or profile.get("default_variant") or "microsoft"
    if (variant_in && !variant_in->empty()) variant = *variant_in;
    else if (profile.default_variant && !profile.default_variant->empty()) variant = *profile.default_variant;
    else variant = str("microsoft");
    // key = sha1((repr(sorted(profile.items())) + self.variant + CACHE_VERSION).encode()).hexdigest()[:12]
    std::vector<py::V> items;
    for (const auto& kv : raw->profile.items()) items.push_back(py::V::tuple({kv.first, kv.second}));
    std::sort(items.begin(), items.end(), [](const py::V& a, const py::V& b) { return a[0].str() < b[0].str(); });
    key = py::hashlib::sha1((str(py::repr(py::V::list(items))) + variant + CACHE_VERSION).encode()).hexdigest().slice(0, 12);
    // v = profile.get("variants", {}).get(self.variant, {})
    const py::V& variants = raw->profile.get("variants");
    const py::V& v = variants.is_dict() ? variants.get(variant.utf8().c_str()) : variants;
    rules = (v.is_dict() && v.get("rules").is_str()) ? str(v.get("rules").str()) : str("");
    patch = v.is_dict() && py::truthy(v.get("patch"));
    c2move = (v.is_dict() && v.get("c2move").is_str()) ? str(v.get("c2move").str()) : str("");
}

namespace {

py::bytes lf(const py::bytes& b) {
    // b.replace(b"\r\n", b"\n")
    const std::string& s = b.raw();
    std::string out;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\r' && i + 1 < s.size() && s[i + 1] == '\n') continue;
        out.push_back(s[i]);
    }
    return py::bytes(out);
}

void write_file(const str& path, const py::bytes& data) {
    py::file f = py::file::open(path, str("wb"));
    f.write(data);
    f.close();
}

py::bytes read_file(const str& path) {
    py::file f = py::open_rb(path);
    const py::bytes b = f.read();
    f.close();
    return b;
}

str pair_repr(const std::tuple<str, py::bytes>& h) { return str("(") + py::repr(std::get<0>(h)) + str(", ") + py::repr(std::get<1>(h)) + str(")"); }

str list_repr(const std::vector<std::tuple<str, py::bytes>>& hs) {
    py::list<str> shown;
    for (const auto& h : hs) shown.append(pair_repr(h));
    return str("[") + py::join(str(", "), shown) + str("]");
}

}  // namespace

std::tuple<std::optional<py::bytes>, str> XenixC::compile(const py::bytes& src, const py::dict<str, py::bytes>& headers,
                                                          const py::list<str>& include, const std::optional<str>& switches) const {
    return compile(src, std::optional<py::dict<str, py::bytes>>(headers), include, switches, true);
}

std::tuple<std::optional<py::bytes>, str> XenixC::compile(const py::bytes& src_in, const std::optional<py::dict<str, py::bytes>>& headers,
                                                          const py::list<str>& include, const std::optional<str>& switches,
                                                          bool use_cache) const {
    (void)switches;
    const py::bytes src = lf(src_in);
    // hs = sorted((headers or {}).items())
    std::vector<std::tuple<str, py::bytes>> hs;
    if (headers)
        for (const auto& kv : headers->items()) hs.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(hs.begin(), hs.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a).u32() != std::get<0>(b).u32()) return std::get<0>(a).u32() < std::get<0>(b).u32();
        return std::get<1>(a).raw() < std::get<1>(b).raw();
    });
    std::vector<std::tuple<str, py::bytes>> incs;
    for (const str& d : include) {
        std::vector<str> names;
        for (const str& f : py::os::listdir(d)) names.push_back(f);
        std::sort(names.begin(), names.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
        for (const str& f : names)
            if (f.lower().endswith(str(".h"))) incs.emplace_back(f, read_file(py::os::path::join(d, f)));
    }
    std::string joined = src.raw();
    joined.push_back('\0');
    joined += list_repr(hs).encode().raw();
    joined.push_back('\0');
    joined += list_repr(incs).encode().raw();
    joined.push_back('\0');
    joined += key.utf8();
    const str k = py::hashlib::sha1(py::bytes(joined)).hexdigest();
    using Result = std::tuple<std::optional<py::bytes>, str>;
    if (use_cache) {
        const std::optional<Result> got = cache::value<Result>(str("exact"), k);
        if (got) return *got;
    }
    py::list<std::tuple<str, py::bytes>> all;
    for (const auto& h : hs) all.append(h);
    for (const auto& h : incs) all.append(h);
    const Result res = _compile(src, all);
    cache::keep<Result>(str("exact"), k, res);
    return res;
}

std::tuple<std::optional<py::bytes>, str> XenixC::_compile(const py::bytes& src, const py::list<std::tuple<str, py::bytes>>& headers) const {
    const py::V& pk = raw_->profile.get("kit");
    if (!pk.is_str()) throw py::KeyError("'kit'");
    const str root = tcm::kit_dir(str(pk.str()));
    const str w = py::tempfile::mkdtemp(std::nullopt, str("xenixc_"));
    str log("");
    try {
        py::os::makedirs(py::os::path::join(w, str("tmp")), 0777, false);
        py::os::makedirs(py::os::path::join(w, str("lib")), 0777, false);
        write_file(py::os::path::join(w, str("x.c")), src);
        for (const auto& h : headers) {
            // b.replace(b"\r\n", b"\n")
            write_file(py::os::path::join(w, std::get<0>(h)), lf(std::get<1>(h)));
        }
        py::bytes c1 = read_file(py::os::path::join(root, str("lib"), str("c1")));
        py::bytes c2 = read_file(py::os::path::join(root, str("lib"), str("c2")));
        if (patch) {
            c1 = _c1(c1);
            c2 = _c2(c2);
        }
        write_file(py::os::path::join(w, str("lib"), str("c1")), c1);
        write_file(py::os::path::join(w, str("lib"), str("c2")), c2);
        py::dict<str, py::dict<int64_t, xenix::Hook>> hooks;
        if (!c2move.empty()) hooks.set(str("lib/c2"), c2_hooks(c2move));
        struct Step {
            const char* prog;
            std::vector<const char*> args;
        };
        const std::vector<Step> steps = {{"lib/cpp", {"x.c", "x.i"}},
                                         {"lib/c0", {"x.i", "t1", "t2"}},
                                         {"lib/c1", {"t1", "t2", "x.s"}},
                                         {"lib/c2", {"x.s", "x.o.s"}},
                                         {"bin/as", {"-o", "x.o", "x.o.s"}}};
        std::tuple<std::optional<py::bytes>, str> result;
        bool stopped = false;
        for (const Step& s : steps) {
            const str prog(s.prog);
            py::list<xenix::Arg> args;
            for (const char* a : s.args) args.append(xenix::Arg(str(a)));
            const xenix::Result r = xenix::run(root, w, prog, args, py::bytes(), false, 400000000, hooks.get(prog, py::dict<int64_t, xenix::Hook>()));
            log = log + r.console.decode("latin1");
            if ((r.status && *r.status != 0) || !r.status || r.error) {      // r.status != 0 (None != 0 is true) or r.error
                const str status = r.status ? py::to_str(*r.status) : str("None");
                result = {std::nullopt, log + prog + str(": exit ") + status + str(" ") + (r.error ? *r.error : str("")) +
                                            str("\n(the pass that stopped: ") + prog + str(")\n")};
                stopped = true;
                break;
            }
            if (prog == str("lib/c1") && !rules.empty()) {
                const str f = py::os::path::join(w, str("x.s"));
                str t = read_file(f).decode("latin1");
                for (const char32_t c : rules.u32()) t = RULES.at(str(c))(t);
                write_file(f, t.encode("latin1"));
            }
        }
        if (!stopped) result = {read_file(py::os::path::join(w, str("x.o"))), log};
        py::shutil::rmtree(w, true);
        return result;
    } catch (...) {
        py::shutil::rmtree(w, true);
        throw;
    }
}

void register_plugin() {
    tcm::register_plugin(str("codebreaker.exact.xenix_c:XenixC"),
                         [](const toolchain::Profile& p, const std::optional<str>& variant) -> std::shared_ptr<toolchain::Toolchain> {
                             return std::make_shared<XenixC>(p, variant);
                         });
}

}  // namespace codebreaker::exact::xenix_c
