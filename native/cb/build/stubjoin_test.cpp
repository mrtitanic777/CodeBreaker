// Tests of stubjoin: modules are built here by hand (a PE and an NE with the structures the joiner moves), joined
// behind DOS programs of different sizes, and every moved offset is checked by following it into the result and
// finding the same bytes the original pointed at.
#include "cb/build/stubjoin.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

void put16(Bytes& b, std::size_t at, unsigned v) {
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}
void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}
std::uint32_t get16(const Bytes& b, std::size_t at) { return b[at] | (b[at + 1] << 8); }
std::uint32_t get32(const Bytes& b, std::size_t at) { return get16(b, at) | (get16(b, at + 2) << 16); }

std::size_t align(std::size_t n, std::size_t a) { return (n + a - 1) / a * a; }

// A repeatable byte pattern: no two blocks alike, so a block found at the wrong place is noticed.
Bytes pattern(std::size_t n, unsigned seed) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>((i * 7 + seed * 31 + (i >> 8) * 13) ^ 0x5C);
    return b;
}

Bytes dos_stub(std::size_t n) {
    Bytes b = pattern(n, 99);
    b[0] = 'M';
    b[1] = 'Z';
    for (std::size_t i = 0x3C; i < 0x40 && i < n; ++i) b[i] = 0;
    return b;
}

// ---- a PE -------------------------------------------------------------------------------------------------------

struct PeOptions {
    bool plus = false;
    std::size_t nsec = 3;
    std::size_t falign = 0x200;
    bool certs = true;
    bool debug = true;
    bool symbols = true;
};

struct BuiltPe {
    Bytes file;
    std::size_t lfa = 0x80;
    std::size_t opt = 0, sec = 0, dirs = 0;
};

BuiltPe make_pe(const PeOptions& o) {
    BuiltPe p;
    const std::size_t optsize = o.plus ? 240 : 224;
    p.opt = p.lfa + 24;
    p.sec = p.opt + optsize;
    p.dirs = p.opt + (o.plus ? 112 : 96);
    const std::size_t soh = align(p.sec + 40 * o.nsec, o.falign);
    Bytes b(soh, 0);
    b[0] = 'M';
    b[1] = 'Z';
    put32(b, 0x3C, static_cast<std::uint32_t>(p.lfa));
    b[p.lfa] = 'P';
    b[p.lfa + 1] = 'E';
    put16(b, p.lfa + 6, static_cast<unsigned>(o.nsec));
    put16(b, p.lfa + 20, static_cast<unsigned>(optsize));
    put16(b, p.opt, o.plus ? 0x20B : 0x10B);
    put32(b, p.opt + 36, static_cast<std::uint32_t>(o.falign));
    put32(b, p.opt + 60, static_cast<std::uint32_t>(soh));
    std::size_t rva = 0x1000;
    std::vector<std::size_t> raws;
    for (std::size_t i = 0; i < o.nsec; ++i) {
        const std::size_t size = o.falign * (1 + i % 3);
        const std::size_t s = p.sec + 40 * i;
        put32(b, s + 8, static_cast<std::uint32_t>(size));
        put32(b, s + 12, static_cast<std::uint32_t>(rva));
        put32(b, s + 16, static_cast<std::uint32_t>(size));
        put32(b, s + 20, static_cast<std::uint32_t>(b.size()));
        raws.push_back(b.size());
        const Bytes data = pattern(size, static_cast<unsigned>(i + 1));
        b.insert(b.end(), data.begin(), data.end());
        if (i == 1) put32(b, s + 24, static_cast<std::uint32_t>(b.size() - 16));   // relocations, inside this section
        if (i == 2) put32(b, s + 28, static_cast<std::uint32_t>(b.size() - 32));   // line numbers
        rva += 0x1000;
    }
    if (o.debug && o.nsec >= 2) {
        // two 28-byte debug entries inside section 1, at its start + 8; their raw-data pointers into section 0
        put32(b, p.dirs + 6 * 8, 0x2000 + 8);
        put32(b, p.dirs + 6 * 8 + 4, 56);
        for (std::size_t k = 0; k < 2; ++k) put32(b, raws[1] + 8 + 28 * k + 24, static_cast<std::uint32_t>(raws[0] + 64 * (k + 1)));
    }
    if (o.symbols) {
        put32(b, p.lfa + 12, static_cast<std::uint32_t>(b.size()));
        const Bytes sym = pattern(90, 77);
        b.insert(b.end(), sym.begin(), sym.end());
    }
    if (o.certs) {
        put32(b, p.dirs + 4 * 8, static_cast<std::uint32_t>(b.size()));
        put32(b, p.dirs + 4 * 8 + 4, 40);
        const Bytes cert = pattern(40, 55);
        b.insert(b.end(), cert.begin(), cert.end());
    }
    p.file = std::move(b);
    return p;
}

// Every pointer the joiner moves is followed into the joined file and the bytes it points at compared with what
// the original's pointer pointed at.
void check_pe(const BuiltPe& original, const Bytes& joined, std::size_t place, const PeOptions& o) {
    const Bytes& b = original.file;
    CB_CHECK_EQ(get32(joined, 0x3C), place);
    const std::size_t lfa = place;
    CB_CHECK_EQ(joined[lfa], 'P');
    const std::size_t hdr = original.sec + 40 * o.nsec - original.lfa;
    const std::size_t new_soh = align(place + hdr, o.falign);
    CB_CHECK_EQ(get32(joined, lfa + 24 + 60), new_soh);
    const std::int64_t delta = static_cast<std::int64_t>(new_soh) - static_cast<std::int64_t>(get32(b, original.opt + 60));
    // the headers up to the section table are the original's, apart from the moved fields
    auto at_new = [&](std::size_t original_off) { return original_off - original.lfa + lfa; };
    CB_CHECK_EQ(get32(joined, at_new(original.opt + 36)), o.falign);
    for (std::size_t i = 0; i < o.nsec; ++i) {
        const std::size_t s = original.sec + 40 * i;
        const std::size_t raw_old = get32(b, s + 20);
        const std::size_t size = get32(b, s + 16);
        CB_CHECK_EQ(get32(joined, at_new(s + 20)), raw_old + delta);
        CB_CHECK_EQ(get32(joined, at_new(s + 12)), get32(b, s + 12));   // RVAs are not file offsets
        // the section's bytes are the original's -- except the debug directory's raw-data pointers, which move
        const bool has_debug_entries = o.debug && i == 1;
        const std::size_t skip_from = has_debug_entries ? 8 : size, skip_to = has_debug_entries ? 8 + 56 : size;
        CB_CHECK(std::equal(b.begin() + raw_old, b.begin() + raw_old + skip_from, joined.begin() + (raw_old + delta)));
        CB_CHECK(std::equal(b.begin() + raw_old + skip_to, b.begin() + raw_old + size, joined.begin() + (raw_old + skip_to + delta)));
        if (get32(b, s + 24) != 0) CB_CHECK_EQ(get32(joined, at_new(s + 24)), get32(b, s + 24) + delta);
        if (get32(b, s + 28) != 0) CB_CHECK_EQ(get32(joined, at_new(s + 28)), get32(b, s + 28) + delta);
    }
    if (o.symbols) {
        const std::size_t old_sym = get32(b, original.lfa + 12);
        CB_CHECK_EQ(get32(joined, lfa + 12), old_sym + delta);
        CB_CHECK(std::equal(b.begin() + old_sym, b.begin() + old_sym + 90, joined.begin() + (old_sym + delta)));
    }
    if (o.certs) {
        const std::size_t old_cert = get32(b, original.dirs + 32);
        CB_CHECK_EQ(get32(joined, at_new(original.dirs + 32)), old_cert + delta);
        CB_CHECK(std::equal(b.begin() + old_cert, b.begin() + old_cert + 40, joined.begin() + (old_cert + delta)));
    }
    if (o.debug && o.nsec >= 2) {
        const std::size_t raw1_old = get32(b, original.sec + 40 + 20);
        for (std::size_t k = 0; k < 2; ++k) {
            const std::size_t e_old = raw1_old + 8 + 28 * k + 24;
            CB_CHECK_EQ(get32(joined, e_old + delta), get32(b, e_old) + delta);
        }
    }
    // the file's size grew by exactly the stub's growth
    CB_CHECK_EQ(joined.size(), b.size() + delta);
}

// ---- an NE ------------------------------------------------------------------------------------------------------

struct NeOptions {
    unsigned shift = 9;
    std::size_t nseg = 3;
    bool resources = true;
    bool nonres_in_header = true;   // the non-resident names sit before the data (else after it)
};

struct BuiltNe {
    Bytes file;
    std::size_t lfa = 0x80;
    std::size_t segtab = 0x40;
    std::vector<std::size_t> res_entries;   // file offsets of the resource sector fields
};

BuiltNe make_ne(const NeOptions& o) {
    BuiltNe n;
    const std::size_t unit = std::size_t{1} << o.shift;
    Bytes b(n.lfa + 0x40, 0);
    b[0] = 'M';
    b[1] = 'Z';
    put32(b, 0x3C, static_cast<std::uint32_t>(n.lfa));
    b[n.lfa] = 'N';
    b[n.lfa + 1] = 'E';
    put16(b, n.lfa + 0x1C, static_cast<unsigned>(o.nseg));
    put16(b, n.lfa + 0x22, static_cast<unsigned>(n.segtab));
    put16(b, n.lfa + 0x32, o.shift);
    b.resize(n.lfa + n.segtab + 8 * o.nseg, 0);
    // the resource table, behind the segment table
    const std::size_t restab = n.segtab + 8 * o.nseg;
    put16(b, n.lfa + 0x24, static_cast<unsigned>(restab));
    std::size_t resname = restab;
    const std::size_t entries = 2;
    if (o.resources) {
        const std::size_t r = n.lfa + restab;
        b.resize(r + 2 + 8 + 12 * entries + 2, 0);
        put16(b, r, o.shift);                    // rscAlignShift
        put16(b, r + 2, 0x8001);                 // a type
        put16(b, r + 4, static_cast<unsigned>(entries));
        for (std::size_t k = 0; k < entries; ++k) n.res_entries.push_back(r + 2 + 8 + 12 * k);
        resname = restab + 2 + 8 + 12 * entries + 2;   // the type list ends with a zero word
    }
    put16(b, n.lfa + 0x26, static_cast<unsigned>(resname));
    // the non-resident name table: before the data, or after
    const Bytes names = pattern(30, 5);
    std::size_t nonres_at = 0;
    if (o.nonres_in_header) {
        nonres_at = b.size();
        b.insert(b.end(), names.begin(), names.end());
    }
    // the data starts at the next sector boundary
    b.resize(align(b.size(), unit), 0);
    for (std::size_t i = 0; i < o.nseg; ++i) {
        const std::size_t seg = 100 + 50 * i;
        put16(b, n.lfa + n.segtab + 8 * i, static_cast<unsigned>(b.size() / unit));
        put16(b, n.lfa + n.segtab + 8 * i + 2, static_cast<unsigned>(seg));
        const Bytes data = pattern(seg, static_cast<unsigned>(i + 10));
        b.insert(b.end(), data.begin(), data.end());
        b.resize(align(b.size(), unit), 0);
    }
    for (std::size_t k = 0; k < n.res_entries.size(); ++k) {
        put16(b, n.res_entries[k], static_cast<unsigned>(b.size() / unit));
        put16(b, n.res_entries[k] + 2, 60);
        const Bytes data = pattern(60, static_cast<unsigned>(k + 20));
        b.insert(b.end(), data.begin(), data.end());
        b.resize(align(b.size(), unit), 0);
    }
    if (!o.nonres_in_header) {
        nonres_at = b.size();
        b.insert(b.end(), names.begin(), names.end());
    }
    put32(b, n.lfa + 0x2C, static_cast<std::uint32_t>(nonres_at));
    n.file = std::move(b);
    return n;
}

void check_ne(const BuiltNe& original, const Bytes& joined, std::size_t place, const NeOptions& o) {
    const Bytes& b = original.file;
    const std::size_t unit = std::size_t{1} << o.shift;
    CB_CHECK_EQ(get32(joined, 0x3C), place);
    CB_CHECK_EQ(joined[place], 'N');
    const std::size_t lfa = place;
    for (std::size_t i = 0; i < o.nseg; ++i) {
        const std::size_t sector_old = get16(b, original.lfa + original.segtab + 8 * i);
        const std::size_t length = get16(b, original.lfa + original.segtab + 8 * i + 2);
        const std::size_t sector_new = get16(joined, lfa + original.segtab + 8 * i);
        CB_CHECK(std::equal(b.begin() + sector_old * unit, b.begin() + sector_old * unit + length, joined.begin() + sector_new * unit));
    }
    for (const std::size_t entry : original.res_entries) {
        const std::size_t sector_old = get16(b, entry);
        const std::size_t sector_new = get16(joined, entry - original.lfa + lfa);
        CB_CHECK(std::equal(b.begin() + sector_old * unit, b.begin() + sector_old * unit + 60, joined.begin() + sector_new * unit));
    }
    const std::size_t nonres_old = get32(b, original.lfa + 0x2C);
    const std::size_t nonres_new = get32(joined, lfa + 0x2C);
    CB_CHECK(std::equal(b.begin() + nonres_old, b.begin() + nonres_old + 30, joined.begin() + nonres_new));
}

}  // namespace

CB_TEST("build.stubjoin.pe_behind_stubs_of_every_size", [] {
    // shorter than the module's own stub (0x80), longer (grows), a multiple of 16 or not, with an explicit place
    for (const std::size_t stub_size : {0x40u, 0x70u, 0x91u, 0x200u, 0x433u}) {
        for (const bool plus : {false, true}) {
            PeOptions o;
            o.plus = plus;
            const BuiltPe pe = make_pe(o);
            const Bytes stub = dos_stub(stub_size);
            const Bytes joined = stubjoin::join(stub, pe.file);
            const std::size_t place = align(stub_size, 16);
            check_pe(pe, joined, place, o);
            CB_CHECK(std::equal(stub.begin(), stub.begin() + 0x3C, joined.begin()));   // the program itself is kept
        }
    }
    PeOptions o;
    const BuiltPe pe = make_pe(o);
    // `at` is honoured when the program ends before it, ignored when it does not (or is zero)
    CB_CHECK_EQ(get32(stubjoin::join(dos_stub(0x70), pe.file, 0x100), 0x3C), 0x100u);
    CB_CHECK_EQ(get32(stubjoin::join(dos_stub(0x70), pe.file, 0x50), 0x3C), 0x70u);
    CB_CHECK_EQ(get32(stubjoin::join(dos_stub(0x70), pe.file, 0), 0x3C), 0x70u);
    check_pe(pe, stubjoin::join(dos_stub(0x70), pe.file, 0x100), 0x100, o);
});

CB_TEST("build.stubjoin.pe_without_optional_parts", [] {
    for (const bool symbols : {false, true}) {
        for (const bool certs : {false, true}) {
            for (const bool debug : {false, true}) {
                PeOptions o;
                o.symbols = symbols;
                o.certs = certs;
                o.debug = debug;
                o.nsec = debug ? 3 : 1;
                const BuiltPe pe = make_pe(o);
                check_pe(pe, stubjoin::join(dos_stub(0x240), pe.file), 0x240, o);
            }
        }
    }
    PeOptions big;
    big.falign = 0x1000;   // a coarser file alignment: the data moves to the next 4 KB boundary
    const BuiltPe pe = make_pe(big);
    check_pe(pe, stubjoin::join(dos_stub(0x300), pe.file), 0x300, big);
});

CB_TEST("build.stubjoin.ne_segments_resources_and_names", [] {
    for (const unsigned shift : {4u, 9u}) {
        for (const bool resources : {false, true}) {
            for (const bool in_header : {false, true}) {
                NeOptions o;
                o.shift = shift;
                o.resources = resources;
                o.nonres_in_header = in_header;
                const BuiltNe ne = make_ne(o);
                for (const std::size_t stub_size : {0x40u, 0x80u, 0x150u}) {
                    const Bytes joined = stubjoin::join(dos_stub(stub_size), ne.file);
                    check_ne(ne, joined, align(stub_size, 16), o);
                }
            }
        }
    }
});

CB_TEST("build.stubjoin.errors", [] {
    const Bytes stub = dos_stub(0x40);
    PeOptions po;
    const BuiltPe pe = make_pe(po);
    CB_CHECK_THROWS_AS(TruncatedError, (void)stubjoin::join(stub, Bytes(0x20, 0)));
    Bytes neither = pe.file;
    neither[pe.lfa] = 'L';
    CB_CHECK_THROWS_AS(UnsupportedError, (void)stubjoin::join(stub, neither));
    Bytes far = pe.file;
    put32(far, 0x3C, 0x100000);                                     // e_lfanew past the file
    CB_CHECK_THROWS_AS(FormatError, (void)stubjoin::join(stub, far));
    Bytes cut(pe.file.begin(), pe.file.begin() + pe.lfa + 40);      // headers cut short
    CB_CHECK_THROWS_AS(Error, (void)stubjoin::join(stub, cut));
    Bytes no_align = pe.file;
    put32(no_align, pe.opt + 36, 0);                                // a file alignment of zero
    CB_CHECK_THROWS_AS(FormatError, (void)stubjoin::join(stub, no_align));
    // a stub with no room for its new-header field
    CB_CHECK_THROWS_AS(FormatError, (void)stubjoin::join(Bytes(0x10, 0), pe.file));

    NeOptions no;
    BuiltNe ne = make_ne(no);
    BuiltNe other = ne;
    // resources aligned differently from segments
    put16(other.file, other.lfa + other.segtab + 8 * no.nseg, 8);
    CB_CHECK_THROWS_AS(UnsupportedError, (void)stubjoin::join(stub, other.file));
    // a moved sector that no longer fits its 16-bit field: a stub of more than 32 MB
    CB_CHECK_THROWS_AS(FormatError, (void)stubjoin::join(dos_stub(0x2001000), ne.file));
});
