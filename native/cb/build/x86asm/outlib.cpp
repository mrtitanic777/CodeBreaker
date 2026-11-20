// cb/build/x86asm/outlib.cpp -- the helpers common to the output formats (NASM's output/outlib.c).
#include "cb/build/x86asm/outlib.hpp"

namespace cb::build::x86asm {

std::uint64_t realsize(OutType type, std::uint64_t size) {
    switch (type) {
    case OUT_REL1ADR: return 1;
    case OUT_REL2ADR: return 2;
    case OUT_REL4ADR: return 4;
    case OUT_REL8ADR: return 8;
    default: return size;
    }
}

// Allocate a segment index and add a section or metasymbol to the by-index table. A zero index allocates a new one.
std::uint32_t OutLib::seg_alloc_for(OlSect* s, OlSym* x, std::uint32_t ix) {
    if (!ix) ix = static_cast<std::uint32_t>(alloc_());
    by_index_[ix >> 1] = Indexed{s, x};
    return ix;
}

OlSect* OutLib::sect_by_name(std::string_view name) const {
    const auto it = by_name_.find(std::string(name));
    return it == by_name_.end() ? nullptr : it->second;
}

OlSect* OutLib::sect_by_index(std::int32_t index) const {
    const std::uint32_t ix = static_cast<std::uint32_t>(index);
    if (ix >= static_cast<std::uint32_t>(SEG_ABS)) return nullptr;
    const auto it = by_index_.find(ix >> 1);
    return it == by_index_.end() ? nullptr : it->second.sect;
}

OlSym* OutLib::sym_by_index(std::int32_t index) const {
    const std::uint32_t ix = static_cast<std::uint32_t>(index);
    if (ix >= static_cast<std::uint32_t>(SEG_ABS)) return nullptr;
    const auto it = by_index_.find(ix >> 1);
    return it == by_index_.end() ? nullptr : it->second.sym;
}

std::int32_t OutLib::new_subsection(OlSect* sect) {
    if (!sect) return NO_SEG;
    return static_cast<std::int32_t>(sect->subindex = seg_alloc_for(sect, nullptr, 0));
}

void OutLib::mkloc(OlLoc& loc, std::int64_t offs, std::int32_t seg) const {
    loc = OlLoc{};
    loc.offs = offs;
    if (static_cast<std::uint32_t>(seg) >= static_cast<std::uint32_t>(SEG_ABS)) {
        if (seg == NO_SEG) {
            loc.seg.t = OS_NOSEG;
        } else {
            loc.seg.t = OS_ABS;
            loc.seg.index = static_cast<std::uint32_t>(seg - SEG_ABS);
        }
    } else {
        loc.seg.index = static_cast<std::uint32_t>(seg) & ~1u;
        loc.seg.t = static_cast<OlSegType>(OS_SECT | (seg & 1));
        loc.seg.sect = sect_by_index(static_cast<std::int32_t>(loc.seg.index));
        loc.seg.sym = sym_by_index(static_cast<std::int32_t>(loc.seg.index));
    }
}

void OutLib::add_sym_to(OlSect& sect, OlSym& sym, std::uint64_t offs, bool global) {
    if (global) {
        sect.symg_by_offs.emplace(offs, &sym);
        sect.symg.push_back(&sym);
    } else {
        sect.syml_by_offs.emplace(offs, &sym);
        sect.syml.push_back(&sym);
    }
}

// The body of NASM's _ol_new_sym after the allocation: place the symbol in its section, or give a metasymbol its segment.
void OutLib::place(OlSym& sym, std::uint32_t flags) {
    if (sym.v.seg.t & OS_SECT) {
        OlSect* const sect = sym.v.seg.sect;
        if (!sect || (sect->flags & OF_SYMBOL)) flags |= OF_IMPSEC;   // must be an external or common reference

        if (flags & OF_IMPSEC) {
            // Metasection
            if (!sym.v.seg.sect && !sym.v.seg.sym) {
                sym.v.seg.sym = &sym;
                sym.v.seg.index = seg_alloc_for(nullptr, &sym, sym.v.seg.index);
            }
        } else if (sym.v.seg.t == OS_OFFS) {
            const std::uint64_t offs = static_cast<std::uint64_t>(sym.v.offs);
            add_sym_to(*sect, sym, offs, false);
            if (flags & OF_GLOBAL) add_sym_to(*sect, sym, offs, true);
        }
    }
    sym.flags = flags;
}

OlSym* OutLib::sym_by_name(std::string_view name) const {
    const auto it = by_symname_.find(std::string(name));
    return it == by_symname_.end() ? nullptr : it->second;
}

OlSym* OutLib::sym_by_address(OlSect& sect, std::int64_t addr, bool global) const {
    const auto& tree = global ? sect.symg_by_offs : sect.syml_by_offs;
    auto it = tree.upper_bound(static_cast<std::uint64_t>(addr));
    if (it == tree.begin()) return nullptr;
    --it;
    return it->second;
}

}  // namespace cb::build::x86asm
