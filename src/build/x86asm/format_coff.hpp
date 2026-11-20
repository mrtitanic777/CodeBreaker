// build/x86asm/format_coff.hpp -- the COFF object formats: `-f coff` (DJGPP and some Unix variants), `-f win32`
// (Microsoft COFF for i386) and `-f win64` (Microsoft COFF for x86-64): the port of NASM's output/outcoff.c with the
// constants of output/pecoff.h.
//
// A COFF object is a file header, the section headers, each section's data followed by its relocations, the symbol
// table (a .file record, two records per section, the absolute symbol, then the module's symbols) and the string table.
// Everything is written when the module ends. Win32 and Win64 add the `.drectve` section for -export directives, COMDAT
// sections, `.sxdata` for SAFESEH, the `@feat.00` symbol and `wrt ..imagebase` / `wrt ..symtab`.
//
// The debug format of these formats (CodeView 8, output/codeview.c) is not ported: nothing in CodeBreaker asks for -g.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/directives.hpp"
#include "build/x86asm/format.hpp"

namespace cb::build::x86asm {

// Which COFF: the standard one (-f coff), Microsoft's for Win32, or for Win64.
enum class CoffKind : int { Coff, Win32, Win64 };

class CoffFormat final : public OutputFormat {
public:
    CoffFormat(FormatHost& host, CoffKind kind);
    ~CoffFormat() override;

    std::string_view fullname() const override;
    std::string_view shortname() const override;
    std::string_view extension() const override { return kind_ == CoffKind::Coff ? ".o" : ".obj"; }
    unsigned flags() const override { return 0; }
    int maxbits() const override { return kind_ == CoffKind::Win64 ? 64 : 32; }
    const std::vector<std::string>& stdmac() const override;
    const std::vector<PragmaFacility>& pragmas() const override { return pragmas_; }

    void init() override;
    void output(const OutData& data) override;
    void symdef(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global,
                std::string_view special) override;
    std::int32_t section(std::optional<std::string_view> name, int& bits) override;
    void sectalign(std::int32_t segment, unsigned value) override;
    DirectiveResult directive(Directive directive, std::optional<std::string_view> value) override;
    void cleanup() override;

private:
    struct Reloc {
        enum Base { SECT_SYMBOLS, ABS_SYMBOL, REAL_SYMBOLS };
        std::int32_t address = 0;   // relative to _start_ of section
        std::int32_t symbol = 0;    // symbol number
        Base symbase = SECT_SYMBOLS;
        std::int16_t type = 0;
    };
    // A fixup of a symbol-table index (wrt ..symtab) applied to the section data when the file is written.
    struct SymIdxReloc {
        std::uint32_t symbol = 0;
        std::uint32_t offset = 0;
        std::uint32_t size = 0;
    };
    struct Section {
        bool has_data = false;                 // false for BSS
        Bytes data;
        std::uint32_t len = 0;
        int nrelocs = 0;
        std::int32_t index = 0;
        std::vector<Reloc> relocs;
        std::uint32_t flags = 0;
        std::uint32_t align_flags = 0;         // user-specified alignment flags
        std::uint32_t sectalign_flags = 0;     // minimum alignment from sectalign
        std::string name;
        std::int32_t namepos = -1;             // offset of the name into the strings table
        std::int32_t pos = 0, relpos = 0;
        std::int64_t pass_last_seen = 0;
        std::vector<SymIdxReloc> symidx_relocs;   // in the order they were made; applied newest first
        // COMDAT
        std::optional<std::string> comdat_name;
        std::uint32_t checksum = 0;            // set only for comdat sections
        std::int8_t comdat_selection = 0;
        std::int8_t comdat_symbol = 0;         // is the "comdat name" in symbol table?
        std::int32_t comdat_associated = 0;    // associated section for selection==5
    };
    struct Symbol {
        std::string name;
        std::int32_t strpos = -1;              // string table position of the name (-1: stored in the symbol)
        std::int32_t value = 0;                // address, or COMMON variable size
        int section = 0;                       // section number where it's defined, in COFF codes
        bool is_global = false;
        std::int16_t type = 0;                 // 0 - notype, 0x20 - function
        std::int32_t namlen = 0;               // full name length
    };

    bool win() const { return kind_ != CoffKind::Coff; }
    bool win64() const { return kind_ == CoffKind::Win64; }
    bool win32() const { return kind_ == CoffKind::Win32; }

    std::uint32_t text_flags() const;
    std::uint32_t data_flags() const;
    std::uint32_t bss_flags() const;
    std::uint32_t rdata_flags() const;

    void gen_init();
    int make_section(std::string_view name, std::uint32_t flags);
    void update_section(int section, std::string_view name, std::uint32_t flags);
    std::uint32_t section_flags(std::string_view name, std::uint32_t flags) const;
    void add_string(std::string_view s);   // append to the string table (with its terminating NUL)
    void sect_write(Section& s, const std::uint8_t* data, std::uint32_t len);
    std::int32_t add_reloc(Section& s, std::int32_t segment, std::int16_t type);
    std::uint32_t out_get_symbol_idx(std::int32_t tsegment, std::int64_t offset);
    void define_comdat_name(const std::string& name, std::int32_t segment);
    void add_export(std::string_view name);
    void build_export_table();
    DirectiveResult directive_export(std::string_view value);
    DirectiveResult directive_safeseh(std::optional<std::string_view> value);
    void adjust_relocs(Section& s);
    void adjust_alignment(Section& s);
    void write();
    void section_header(const Section& s);
    void write_relocs(const Section& s);
    void write_symbol(std::string_view name, bool has_name, std::int32_t strpos, std::int32_t value, int section, int type,
                      int storageclass, int aux);
    void write_symbols();
    std::uint32_t symbol_raa_read(const std::unordered_map<std::int32_t, std::int64_t>& m, std::int32_t key) const;

    FormatHost& host_;
    Assembly& a_;
    CoffKind kind_;
    Bytes out_;
    std::vector<PragmaFacility> pragmas_;   // the pragma facility names (`%pragma win ...`, `%pragma coff ...`)

    std::int32_t imagebase_sect_ = 0;
    std::int32_t symtab_sect_ = 0;
    std::vector<std::unique_ptr<Section>> sects_;
    std::vector<Symbol> syms_;
    Bytes strs_;                       // the string table, without its leading length
    std::uint32_t strslen_ = 0;
    std::int32_t def_seg_ = 0;
    int initsym_ = 0;
    std::unordered_map<std::int32_t, std::int64_t> bsym_;     // external-symbol segment number -> symbol index (RAA)
    std::unordered_map<std::int32_t, std::int64_t> symval_;   // segment -> common variable size (RAA)

    // .drectve: the exports, in order, and the section that holds them
    std::vector<std::string> exports_;
    Section* directive_sec_ = nullptr;
    int sxseg_ = -1;                   // the .sxdata section (SAFESEH), by position
};

}  // namespace cb::build::x86asm
