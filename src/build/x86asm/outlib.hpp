// build/x86asm/outlib.hpp -- helpers common to the output formats (NASM's output/outlib.c and outlib.h).
//
// NASM keeps the sections and symbols of a format in a list and a hash table embedded at the top of the format's own
// structures (cast to and from `struct ol_sect`). Here a format derives its section from `OlSect` and its symbol from
// `OlSym`, and `OutLib` -- one per format object -- owns them, finds them by name and by segment index, and walks them in
// declared order.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "build/x86asm/directives.hpp"
#include "build/x86asm/types.hpp"
#include "core/bytes.hpp"

namespace cb::build::x86asm {

// The real size of a legacy relocation output type (realsize).
std::uint64_t realsize(OutType type, std::uint64_t size);

// Common flags for sections and symbols; low values reserved for the backend.
inline constexpr std::uint32_t OF_SYMBOL = 0x80000000;
inline constexpr std::uint32_t OF_GLOBAL = 0x40000000;
inline constexpr std::uint32_t OF_IMPSEC = 0x20000000;
inline constexpr std::uint32_t OF_COMMON = 0x10000000;

struct OlSym;

// The kind of segment reference in an `OlLoc` (enum ol_seg_type).
enum OlSegType : int {
    OS_NOSEG = 0,    // plain number (no segment)
    OS_SEGREF = 1,   // it is a segment reference
    OS_ABS = 1,      // absolute segment reference
    OS_SECT = 2,     // it is a real section
    OS_OFFS = OS_SECT,        // offset reference in section
    OS_SEG = OS_SECT | OS_SEGREF   // section reference
};

// A section or an external symbol seen as a section: the part both have. Both begin with `flags`, so an external
// symbol found by segment index is told from a section by OF_SYMBOL.
struct OlSect {
    virtual ~OlSect() = default;

    std::uint32_t flags = 0;       // section/symbol flags
    std::string name;              // name of the section
    // All the symbols in this section (declared order), and the global ones, each by offset (NASM keeps a tree to find the
    // symbol at or before an address).
    std::vector<OlSym*> syml;
    std::vector<OlSym*> symg;
    std::multimap<std::uint64_t, OlSym*> syml_by_offs;
    std::multimap<std::uint64_t, OlSym*> symg_by_offs;
    Bytes data;                    // contents of the section
    std::uint32_t index = 0;       // primary section index
    std::uint32_t subindex = 0;    // current subsection index
};

// Segment reference: which section (or external symbol), how it is referred to, and an index.
struct OlSeg {
    OlSect* sect = nullptr;        // section structure (or the external symbol's own `OlSect` view)
    OlSym* sym = nullptr;          // external symbol structure
    OlSegType t = OS_NOSEG;
    // For a section: subsection index. For a metasymbol: virtual segment index. For an absolute symbol: the value.
    std::uint32_t index = 0;
};

// seg:offs representing the full location value and type.
struct OlLoc {
    std::int64_t offs = 0;
    OlSeg seg;
};

// Common symbol structure.
struct OlSym {
    virtual ~OlSym() = default;

    std::uint32_t flags = 0;     // section/symbol flags
    std::uint32_t size = 0;      // size value (for the backend)
    std::string name;            // symbol name
    OlLoc p;                     // symbol position ("where")
    OlLoc v;                     // symbol value ("what")
};

// The sections and symbols of one output format object.
class OutLib {
public:
    // `alloc` gives a fresh segment number (seg_alloc).
    explicit OutLib(std::function<std::int32_t()> alloc) : alloc_(std::move(alloc)) {}

    // Find a section by name, or create it (a `S`, derived from OlSect) and give it an index.
    template <class S>
    S& get_sect(std::string_view name) {
        if (OlSect* s = sect_by_name(name)) return static_cast<S&>(*s);
        auto owned = std::make_unique<S>();
        S& s = *owned;
        s.name = std::string(name);
        s.index = s.subindex = seg_alloc_for(&s, nullptr, 0);
        sects_.push_back(std::move(owned));
        by_name_[std::string(name)] = &s;
        return s;
    }

    // Find a section by name without creating one.
    OlSect* sect_by_name(std::string_view name) const;
    // Find a real section by segment index; nullptr when the index is not a section's (an external symbol's segment index
    // is found with sym_by_index).
    OlSect* sect_by_index(std::int32_t index) const;
    // Find the external symbol (or other metasymbol) that owns a segment index; nullptr when none.
    OlSym* sym_by_index(std::int32_t index) const;

    // The sections in declared order (not including external symbols), and how many.
    const std::vector<std::unique_ptr<OlSect>>& sections() const { return sects_; }
    std::uint64_t nsects() const { return sects_.size(); }

    // Start a new subsection of `sect`; the index of the new subsection (NO_SEG for a null section).
    std::int32_t new_subsection(OlSect* sect);

    // Convert offs:seg to a location structure (ol_mkloc).
    void mkloc(OlLoc& loc, std::int64_t offs, std::int32_t seg) const;

    // Create a new symbol (a `Y`, derived from OlSym). nullptr if the symbol already exists. See outlib.c for what the flags do.
    template <class Y>
    Y* new_sym(std::string_view name, const OlLoc& v, std::uint32_t flags) {
        if (sym_by_name(name)) return nullptr;
        auto owned = std::make_unique<Y>();
        Y* sym = owned.get();
        sym->name = std::string(name);
        sym->v = v;
        flags |= OF_SYMBOL;
        place(*sym, flags);
        syms_.push_back(std::move(owned));
        by_symname_[std::string(name)] = sym;
        return sym;
    }

    // Find a symbol in the global namespace.
    OlSym* sym_by_name(std::string_view name) const;
    // Find a symbol by address in a section: the one at that address, else the nearest previous. `global` restricts to global
    // symbols. nullptr if none.
    OlSym* sym_by_address(OlSect& sect, std::int64_t addr, bool global) const;

    // The symbols in declared order, and how many.
    const std::vector<std::unique_ptr<OlSym>>& symbols() const { return syms_; }
    std::uint64_t nsyms() const { return syms_.size(); }

private:
    void place(OlSym& sym, std::uint32_t flags);
    std::uint32_t seg_alloc_for(OlSect* s, OlSym* x, std::uint32_t ix);
    static void add_sym_to(OlSect& sect, OlSym& sym, std::uint64_t offs, bool global);

    std::function<std::int32_t()> alloc_;
    std::vector<std::unique_ptr<OlSect>> sects_;
    std::unordered_map<std::string, OlSect*> by_name_;
    struct Indexed {
        OlSect* sect = nullptr;
        OlSym* sym = nullptr;
    };
    std::unordered_map<std::uint32_t, Indexed> by_index_;   // segment index >> 1 -> section or metasymbol
    std::vector<std::unique_ptr<OlSym>> syms_;
    std::unordered_map<std::string, OlSym*> by_symname_;
};

}  // namespace cb::build::x86asm
