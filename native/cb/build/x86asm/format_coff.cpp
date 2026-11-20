#include "cb/build/x86asm/format_coff.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <format>

#include "cb/build/x86asm/format_util.hpp"

namespace cb::build::x86asm {

namespace {

using namespace fmt;

// Section flags (output/pecoff.h).
constexpr std::uint32_t IMAGE_SCN_CNT_CODE = 0x00000020;
constexpr std::uint32_t IMAGE_SCN_CNT_INITIALIZED_DATA = 0x00000040;
constexpr std::uint32_t IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x00000080;
constexpr std::uint32_t IMAGE_SCN_LNK_INFO = 0x00000200;
constexpr std::uint32_t IMAGE_SCN_LNK_REMOVE = 0x00000800;
constexpr std::uint32_t IMAGE_SCN_LNK_COMDAT = 0x00001000;
constexpr std::uint32_t IMAGE_SCN_MAX_RELOC = 0xffff;
constexpr std::uint32_t IMAGE_SCN_ALIGN_1BYTES = 0x00100000;
constexpr std::uint32_t IMAGE_SCN_ALIGN_4BYTES = 0x00300000;
constexpr std::uint32_t IMAGE_SCN_ALIGN_8BYTES = 0x00400000;
constexpr std::uint32_t IMAGE_SCN_ALIGN_16BYTES = 0x00500000;
constexpr std::uint32_t IMAGE_SCN_ALIGN_MASK = 0x00f00000;
constexpr std::uint32_t IMAGE_SCN_LNK_NRELOC_OVFL = 0x01000000;
constexpr std::uint32_t IMAGE_SCN_MEM_EXECUTE = 0x20000000;
constexpr std::uint32_t IMAGE_SCN_MEM_READ = 0x40000000;
constexpr std::uint32_t IMAGE_SCN_MEM_WRITE = 0x80000000;

// Relocation types.
constexpr std::int16_t IMAGE_REL_AMD64_ADDR64 = 0x0001;
constexpr std::int16_t IMAGE_REL_AMD64_ADDR32 = 0x0002;
constexpr std::int16_t IMAGE_REL_AMD64_ADDR32NB = 0x0003;
constexpr std::int16_t IMAGE_REL_AMD64_REL32 = 0x0004;
constexpr std::int16_t IMAGE_REL_I386_DIR32 = 0x0006;
constexpr std::int16_t IMAGE_REL_I386_REL32 = 0x0014;

constexpr unsigned IMAGE_FILE_MACHINE_AMD64 = 0x8664;
constexpr unsigned IMAGE_FILE_MACHINE_I386 = 0x014c;

constexpr int IMAGE_COMDAT_SELECT_ASSOCIATIVE = 5;

constexpr std::string_view kWrtImagebase = "..imagebase";
constexpr std::string_view kWrtSymtab = "..symtab";
constexpr std::string_view kComdatPlaceholderName = ".tmpcmdt";
constexpr std::string_view kExportSectionName = ".drectve";
constexpr unsigned kCoffMaxAlignment = 8192;

constexpr std::uint32_t kInfoFlags = IMAGE_SCN_ALIGN_1BYTES | IMAGE_SCN_LNK_INFO | IMAGE_SCN_LNK_REMOVE;
constexpr std::uint32_t kExportSectionFlags = kInfoFlags;   // EXPORT_SECTION_FLAGS
constexpr std::uint32_t kPdataFlags = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_ALIGN_4BYTES | IMAGE_SCN_MEM_READ;
constexpr std::uint32_t kXdataFlags = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_ALIGN_8BYTES | IMAGE_SCN_MEM_READ;

constexpr bool is_power2(std::uint64_t v) { return v && (v & (v - 1)) == 0; }

// alignlog2_32 + 1, shifted into the alignment field of the section flags: an alignment value of 0 sets no flags.
std::uint32_t sectalign_flags_of(unsigned align) {
    int log = -1;
    if (is_power2(align)) {
        log = 0;
        while ((1u << log) < align) ++log;
    }
    return static_cast<std::uint32_t>(log + 1) << 20;
}

}  // namespace

CoffFormat::CoffFormat(FormatHost& host, CoffKind kind) : host_(host), a_(host.assembly()), kind_(kind) {
    // The pragma facilities (coff_pragma_list, coff_win_pragma_list): names only, no handlers; the final entry without a
    // name is NASM's list terminator.
    if (kind == CoffKind::Coff) {
        pragmas_.push_back(PragmaFacility{"coff", {}});
    } else {
        pragmas_.push_back(PragmaFacility{"win", {}});
        pragmas_.push_back(PragmaFacility{"coff", {}});
    }
    pragmas_.push_back(PragmaFacility{"", {}});
}

CoffFormat::~CoffFormat() = default;

std::string_view CoffFormat::fullname() const {
    switch (kind_) {
    case CoffKind::Coff: return "COFF (i386) (DJGPP, some Unix variants)";
    case CoffKind::Win32: return "Microsoft extended COFF for Win32 (i386)";
    case CoffKind::Win64: return "Microsoft extended COFF for Win64 (x86-64)";
    }
    return {};
}

std::string_view CoffFormat::shortname() const {
    switch (kind_) {
    case CoffKind::Coff: return "coff";
    case CoffKind::Win32: return "win32";
    case CoffKind::Win64: return "win64";
    }
    return {};
}

// The standard macros of outcoff.mac, shared by all three COFF formats.
const std::vector<std::string>& CoffFormat::stdmac() const {
    static const std::vector<std::string> lines = {
        "%define __?SECT?__ [section .text]",
        "%macro __?NASM_CDecl?__ 1", "%endmacro",
        "%imacro export 1+.nolist", "[export %1]", "%endmacro",
        "%imacro safeseh 1.nolist", "[safeseh %1]", "%endmacro"};
    return lines;
}

// Some common section flags by default (the DOS ones are the standard COFF's).
std::uint32_t CoffFormat::text_flags() const {
    return win() ? (IMAGE_SCN_CNT_CODE | IMAGE_SCN_ALIGN_16BYTES | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ)
                 : IMAGE_SCN_CNT_CODE;
}
std::uint32_t CoffFormat::data_flags() const {
    return win() ? (IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_ALIGN_4BYTES | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)
                 : IMAGE_SCN_CNT_INITIALIZED_DATA;
}
std::uint32_t CoffFormat::bss_flags() const {
    return win() ? (IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_ALIGN_4BYTES | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)
                 : IMAGE_SCN_CNT_UNINITIALIZED_DATA;
}
std::uint32_t CoffFormat::rdata_flags() const {
    return win() ? (IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_ALIGN_8BYTES | IMAGE_SCN_MEM_READ)
                 : IMAGE_SCN_CNT_INITIALIZED_DATA;
}

void CoffFormat::init() {
    gen_init();
    if (win64()) {
        imagebase_sect_ = host_.seg_alloc() + 1;
        host_.backend_label(kWrtImagebase, imagebase_sect_, 0);
    }
}

void CoffFormat::gen_init() {
    sects_.clear();
    syms_.clear();
    bsym_.clear();
    symval_.clear();
    strs_.clear();
    strslen_ = 0;
    exports_.clear();
    directive_sec_ = nullptr;
    sxseg_ = -1;
    out_.clear();
    def_seg_ = host_.seg_alloc();
    symtab_sect_ = host_.seg_alloc() + 1;
    host_.backend_label(kWrtSymtab, symtab_sect_, 0);
}

void CoffFormat::add_string(std::string_view s) {
    strs_.insert(strs_.end(), s.begin(), s.end());
    strs_.push_back(0);
    strslen_ += static_cast<std::uint32_t>(s.size()) + 1;
}

int CoffFormat::make_section(std::string_view name, std::uint32_t flags) {
    auto owned = std::make_unique<Section>();
    Section* s = owned.get();

    s->has_data = flags != bss_flags();
    s->index = (name == ".text") ? def_seg_ : host_.seg_alloc();
    s->namepos = -1;
    std::size_t namelen = name.size();
    if (namelen > 8) {
        if (win()) {
            s->namepos = static_cast<std::int32_t>(strslen_ + 4);
            add_string(name);
        } else {
            namelen = 8;
        }
    }
    s->name = std::string(name.substr(0, namelen));
    s->flags = flags;

    sects_.push_back(std::move(owned));
    return static_cast<int>(sects_.size()) - 1;
}

// Update the name and flags of an existing section.
void CoffFormat::update_section(int section, std::string_view name, std::uint32_t flags) {
    Section& s = *sects_[static_cast<std::size_t>(section)];
    std::size_t namelen = name.size();

    if (namelen > 8) {
        if (win()) {
            s.namepos = static_cast<std::int32_t>(strslen_ + 4);
            add_string(name);
        } else {
            namelen = 8;
        }
    }
    s.name = std::string(name.substr(0, namelen));
    s.flags = flags;
}

// The default section flags (based on section name).
std::uint32_t CoffFormat::section_flags(std::string_view name, std::uint32_t flags) const {
    if (!flags) {
        flags = text_flags();
        if (name == ".data")
            flags = data_flags();
        else if (name == ".rdata")
            flags = rdata_flags();
        else if (name == ".bss")
            flags = bss_flags();
        else if (win64()) {
            if (name == ".pdata")
                flags = kPdataFlags;
            else if (name == ".xdata")
                flags = kXdataFlags;
        }
    }
    return flags;
}

std::int32_t CoffFormat::section(std::optional<std::string_view> name_text, int& bits) {
    // Set default bits.
    if (!name_text) {
        bits = win64() ? 64 : 32;
        return def_seg_;
    }

    std::string_view text = *name_text;
    const std::size_t wl = word_length(text);
    std::string name(text.substr(0, wl));
    std::string_view p = text.substr(wl);
    if (!p.empty()) p.remove_prefix(1);
    if (name.size() > 8) {
        if (!win()) {
            a_.warn(WARN_OTHER, "COFF section names limited to 8 characters:  truncating");
            name.resize(8);
        }
    }
    std::uint32_t flags = 0, align_flags = 0;
    std::int8_t comdat_selection = 0;
    std::int32_t comdat_associated = 0;
    std::optional<std::string> comdat_name;

    p = skip_spaces(p);
    while (!p.empty()) {
        const std::string_view q = take_word(p);

        if (equals_nocase(q, "code") || equals_nocase(q, "text")) {
            flags = text_flags();
        } else if (equals_nocase(q, "data")) {
            flags = data_flags();
        } else if (equals_nocase(q, "rdata")) {
            if (win()) {
                flags = rdata_flags();
            } else {
                flags = data_flags();   // gotta do something
                a_.nonfatal("standard COFF does not support read-only data sections");
            }
        } else if (equals_nocase(q, "bss")) {
            flags = bss_flags();
        } else if (equals_nocase(q, "info")) {
            if (win()) {
                flags = kInfoFlags;
            } else {
                flags = data_flags();   // gotta do something
                a_.nonfatal("standard COFF does not support informational sections");
            }
        } else if (starts_nocase(q, "align=")) {
            const std::string_view num = q.substr(6);
            if (num.find_first_not_of("0123456789") != std::string_view::npos) {
                a_.nonfatal("argument to `align' is not numeric");
            } else {
                const unsigned align = static_cast<unsigned>(std::atoi(std::string(num).c_str()));
                // Allow align=0 meaning use default
                if (!align) {
                    align_flags = 0;
                } else if (!is_power2(align)) {
                    a_.nonfatal("argument to `align' is not a power of two");
                } else if (align > kCoffMaxAlignment) {
                    a_.nonfatal("maximum alignment in COFF is {} bytes", static_cast<int>(kCoffMaxAlignment));
                } else {
                    align_flags = sectalign_flags_of(align);
                }
            }
        } else if (starts_nocase(q, "comdat=")) {
            // Expected format: comdat=num:name
            // where num is a number (one of the IMAGE_COMDAT_SELECT_* constants) and name is the "COMDAT name".
            const std::string qs(q);
            char* end = nullptr;
            comdat_selection = static_cast<std::int8_t>(std::strtoul(qs.c_str() + 7, &end, 10));
            if (!comdat_selection)
                a_.nonfatal("invalid argument to `comdat'");
            else if (*end != ':' || end[1] == '\0')
                a_.nonfatal("missing name in `comdat'");
            else
                comdat_name = std::string(end + 1);
        }
    }

    std::size_t i;
    for (i = 0; i < sects_.size(); i++) {
        Section& si = *sects_[i];
        if (name == si.name) {
            if (!comdat_name && !si.comdat_name) {
                break;
            } else if (comdat_name && si.comdat_name && *comdat_name == *si.comdat_name) {
                // For COMDAT, it makes sense to have multiple sections with the same name (different comdat name though)
                if ((si.comdat_selection == IMAGE_COMDAT_SELECT_ASSOCIATIVE && comdat_selection == IMAGE_COMDAT_SELECT_ASSOCIATIVE) ||
                    (si.comdat_selection != IMAGE_COMDAT_SELECT_ASSOCIATIVE && comdat_selection != IMAGE_COMDAT_SELECT_ASSOCIATIVE)) {
                    // Let's also allow an associative/other pair with the same name
                    break;
                }
            }
        } else if (comdat_name && si.comdat_name && !si.comdat_selection && *comdat_name == *si.comdat_name &&
                   comdat_selection != IMAGE_COMDAT_SELECT_ASSOCIATIVE) {
            // This seems to be a "placeholder section" we've created before to be the associate of a previous comdat
            // section. We'll just update the name and flags with the real ones now.
            flags = section_flags(name, flags);
            update_section(static_cast<int>(i), name, flags | IMAGE_SCN_LNK_COMDAT);
            si.comdat_selection = comdat_selection;
            break;
        }
    }

    if (i == sects_.size()) {
        flags = section_flags(name, flags);

        if (comdat_name) {
            flags |= IMAGE_SCN_LNK_COMDAT;

            if (comdat_selection == IMAGE_COMDAT_SELECT_ASSOCIATIVE) {
                // Find an existing section with given comdat name
                std::size_t j;
                for (j = 0; j < sects_.size(); j++)
                    if (sects_[j]->comdat_name && *sects_[j]->comdat_name == *comdat_name) break;

                if (j == sects_.size()) {
                    // The associated section doesn't exist (yet). Even though the specs don't enforce a particular
                    // order, VS (2019) linker doesn't accept .obj files where the target section is a later one (than
                    // the one with sel==5), so insert another section now (a placeholder), hoping it will be turned
                    // into the target section later.
                    j = static_cast<std::size_t>(make_section(kComdatPlaceholderName, text_flags()));
                    sects_[j]->comdat_name = *comdat_name;
                }

                comdat_associated = static_cast<std::int32_t>(j + 1);
            }
        }

        i = static_cast<std::size_t>(make_section(name, flags));
        sects_[i]->align_flags = align_flags;

        if (comdat_name) {
            sects_[i]->comdat_selection = comdat_selection;
            sects_[i]->comdat_associated = comdat_associated;
            sects_[i]->comdat_name = *comdat_name;
        }
    } else {
        if (flags) {
            if (comdat_name) flags |= IMAGE_SCN_LNK_COMDAT;

            // Warn if non-alignment flags differ
            if (((flags ^ sects_[i]->flags) & ~IMAGE_SCN_ALIGN_MASK) && sects_[i]->pass_last_seen == a_.pass_count()) {
                a_.warn(WARN_OTHER, "section attributes changed on redeclaration of section `{}'", name);
            }
        }

        // Alignment can be increased, but never decreased. However, specifying a narrower alignment is permitted and
        // ignored.
        if (align_flags > sects_[i]->align_flags) sects_[i]->align_flags = align_flags;

        if (comdat_name) {
            if ((sects_[i]->comdat_selection != comdat_selection) && sects_[i]->pass_last_seen == a_.pass_count()) {
                a_.warn(WARN_OTHER, "comdat selection changed on redeclaration of name `{}'", *comdat_name);
            }
        }
    }

    sects_[i]->pass_last_seen = a_.pass_count();
    return sects_[i]->index;
}

void CoffFormat::define_comdat_name(const std::string& name, std::int32_t segment) {
    symdef(name, segment, 0, 1, std::string_view());
}

void CoffFormat::symdef(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global,
                        std::string_view special_text) {
    // (The label manager passes no special text as null, which `special_text` empty is here.)
    const std::optional<std::string_view> special =
        special_text.empty() ? std::nullopt : std::optional<std::string_view>(special_text);
    if (name.size() >= 2 && name[0] == '.' && name[1] == '.' && (name.size() < 3 || name[2] != '@')) {
        if (name != kWrtImagebase && name != kWrtSymtab) a_.nonfatal("unrecognized special symbol `{}'", name);
        return;
    }

    if (is_global == 3) return;   // discard special-retry from pass two.

    int section;
    if (segment == NO_SEG) {
        section = -1;   // absolute symbol
    } else {
        section = 0;
        for (std::size_t i = 0; i < sects_.size(); i++)
            if (segment == sects_[i]->index) {
                section = static_cast<int>(i) + 1;

                Section& si = *sects_[i];
                if (si.comdat_name && !si.comdat_symbol) {
                    // The "comdat symbol" must be the first one in symbol table. So we'll insert/define it - before
                    // defining the other one.
                    si.comdat_symbol = 1;

                    if (si.comdat_selection != IMAGE_COMDAT_SELECT_ASSOCIATIVE && *si.comdat_name != name) {
                        define_comdat_name(*si.comdat_name, segment);
                    }
                }
                break;
            }
    }

    std::int32_t pos = static_cast<std::int32_t>(strslen_ + 4);
    if (name.size() > 8)
        add_string(name);
    else
        pos = -1;

    Symbol sym;
    sym.strpos = pos;
    sym.namlen = static_cast<std::int32_t>(name.size());
    sym.name = std::string(name);
    sym.is_global = is_global != 0;
    sym.type = 0;   // Default to T_NULL (no type)
    sym.section = section;
    if (!sym.section) sym.is_global = true;
    if (is_global == 2)
        sym.value = static_cast<std::int32_t>(offset);
    else
        sym.value = (sym.section == 0 ? 0 : static_cast<std::int32_t>(offset));

    if (special) {
        std::string_view sp = skip_spaces(*special);
        while (!sp.empty()) {
            const std::size_t wlen = word_length(sp);
            const std::string_view word = sp.substr(0, wlen);
            if (wlen == 8 && starts_nocase(word, "function"))
                sym.type = 0x20;   // DT_FCN
            else
                a_.nonfatal("unrecognised symbol type `{}' on `{}'", word, name);
            sp = skip_spaces(sp.substr(wlen));
        }
    }

    // define the references from external-symbol segment numbers to these symbol records.
    if (sym.section == 0) bsym_[segment] = static_cast<std::int64_t>(syms_.size());

    if (segment != NO_SEG) symval_[segment] = sym.section ? 0 : sym.value;

    syms_.push_back(std::move(sym));
}

std::uint32_t CoffFormat::symbol_raa_read(const std::unordered_map<std::int32_t, std::int64_t>& m, std::int32_t key) const {
    const auto it = m.find(key);
    return it == m.end() ? 0u : static_cast<std::uint32_t>(it->second);
}

std::int32_t CoffFormat::add_reloc(Section& sect, std::int32_t segment, std::int16_t type) {
    Reloc r;
    r.address = static_cast<std::int32_t>(sect.len);
    if (segment == NO_SEG) {
        r.symbol = 0;
        r.symbase = Reloc::ABS_SYMBOL;
    } else {
        r.symbase = Reloc::REAL_SYMBOLS;
        for (std::size_t i = 0; i < sects_.size(); i++) {
            if (segment == sects_[i]->index) {
                r.symbol = static_cast<std::int32_t>(i) * 2;
                r.symbase = Reloc::SECT_SYMBOLS;
                break;
            }
        }
        if (r.symbase == Reloc::REAL_SYMBOLS) r.symbol = static_cast<std::int32_t>(symbol_raa_read(bsym_, segment));
    }
    r.type = type;

    const bool real = r.symbase == Reloc::REAL_SYMBOLS;
    sect.relocs.push_back(r);
    sect.nrelocs++;

    // Return the fixup for standard COFF common variables.
    if (real && !win()) return static_cast<std::int32_t>(symbol_raa_read(symval_, segment));

    return 0;
}

// Helper for output handling wrt ..symtab addressing.
std::uint32_t CoffFormat::out_get_symbol_idx(std::int32_t tsegment, std::int64_t offset) {
    // No better way of doing this?
    int section;
    if (tsegment == NO_SEG) {
        section = -1;
    } else {
        for (section = 0; section < static_cast<int>(sects_.size()); section++)
            if (tsegment == sects_[static_cast<std::size_t>(section)]->index) break;
        section++;
    }

    // Do symbol table lookup (unless it is external).
    if (section <= static_cast<int>(sects_.size())) {
        std::uint32_t symidx = UINT32_MAX;
        for (std::uint32_t n = 0; n < syms_.size(); n++) {
            const Symbol& sym = syms_[n];
            if (sym.section == section && sym.value == static_cast<std::int32_t>(offset)) {
                if (sym.type & 0x30) return n;   // N_TMASK: prefer symbol with non-NULL type.
                if (symidx == UINT32_MAX) symidx = n;
            }
        }
        if (symidx == UINT32_MAX)
            a_.nonfatal("wrt ..symtab: Unable to find symbol for {}:{:#x}",
                        section > 0 ? sects_[static_cast<std::size_t>(section) - 1]->name : std::string("ABS"),
                        static_cast<std::uint64_t>(offset));
        return symidx;
    }

    // External symbols.
    if (tsegment != NO_SEG) return symbol_raa_read(bsym_, tsegment);

    a_.nonfatal("wrt ..symtab: Unable to find symbol for {:#x}:{:#x}", static_cast<std::uint32_t>(tsegment),
                static_cast<std::uint64_t>(offset));
    return UINT32_MAX;
}

void CoffFormat::sect_write(Section& sect, const std::uint8_t* data, std::uint32_t len) {
    if (data)
        sect.data.insert(sect.data.end(), data, data + len);
    else
        sect.data.insert(sect.data.end(), len, std::uint8_t{0});   // saa_wbytes(NULL): zeros
    sect.len += len;
}

void CoffFormat::output(const OutData& out) {
    const std::int32_t segto = out.loc.segment;
    const void* data = out.legacy.data;
    const OutType type = out.legacy.type;
    const std::uint64_t size = out.legacy.size;
    const std::int32_t segment = out.legacy.tsegment;
    std::int32_t wrt = out.legacy.twrt;
    std::uint8_t mydata[8];

    if (wrt != NO_SEG && wrt != symtab_sect_ && !win64()) {
        wrt = NO_SEG;   // continue to do _something_
        a_.nonfatal("WRT not supported by COFF output formats");
    }

    Section* s = nullptr;
    for (auto& sp : sects_) {
        if (segto == sp->index) {
            s = sp.get();
            break;
        }
    }
    if (!s) {
        int tempint;   // ignored
        if (segto != section(".text", tempint))
            a_.panic("strange segment conditions in COFF driver");
        else
            s = sects_.back().get();
    }

    // magically default to 'wrt ..imagebase' in .pdata and .xdata
    if (win64() && wrt == NO_SEG) {
        if (s->name == ".pdata" || s->name == ".xdata") wrt = imagebase_sect_;
    }

    if (!s->has_data && type != OUT_RESERVE) {
        a_.warn(WARN_OTHER, "attempt to initialize memory in BSS section `{}': ignored", s->name);
        std::uint64_t real = size;
        switch (type) {
        case OUT_REL1ADR: real = 1; break;
        case OUT_REL2ADR: real = 2; break;
        case OUT_REL4ADR: real = 4; break;
        case OUT_REL8ADR: real = 8; break;
        default: break;
        }
        s->len += static_cast<std::uint32_t>(real);
        return;
    }

    std::memset(mydata, 0, sizeof mydata);

    auto read_data = [&]() {
        std::int64_t v;
        std::memcpy(&v, data, sizeof v);
        return v;
    };
    auto put_long = [&](std::int64_t v) {   // WRITELONG
        const std::uint32_t u = static_cast<std::uint32_t>(v);
        for (int k = 0; k < 4; ++k) mydata[k] = static_cast<std::uint8_t>(u >> (8 * k));
    };

    if (type == OUT_RESERVE) {
        if (s->has_data) {
            a_.warn(WARN_ZEROING, "uninitialised space declared in non-BSS section `{}': zeroing", s->name);
            sect_write(*s, nullptr, static_cast<std::uint32_t>(size));
        } else {
            s->len += static_cast<std::uint32_t>(size);
        }
    } else if (type == OUT_RAWDATA) {
        sect_write(*s, static_cast<const std::uint8_t*>(data), static_cast<std::uint32_t>(size));
    } else if (type == OUT_ADDRESS) {
        const int asize = std::abs(static_cast<int>(size));
        if (wrt == symtab_sect_) {
            // Emit internal fixup since the table starts with sections and stuff that can be added to after this
            // statement.
            SymIdxReloc ir;
            ir.offset = s->len;
            ir.size = static_cast<std::uint8_t>(asize);
            ir.symbol = out_get_symbol_idx(segment, static_cast<std::int64_t>(out.toffset));
            s->symidx_relocs.push_back(ir);
            if (static_cast<unsigned>(asize) > sizeof mydata) a_.assert_failed("(unsigned)asize <= sizeof(mydata)");
            sect_write(*s, mydata, static_cast<std::uint32_t>(asize));
            if (asize > 4)
                a_.warn(WARN_OTHER, "zero extending 'wrt ..symtab' (32-bit) to {}-bit", asize * 8);
            else if (asize < 4)
                a_.warn(WARN_OTHER, "truncating 'wrt ..symtab' (32-bit) to {}-bit", asize * 8);
        } else if (!win64()) {
            if (asize != 4 && (segment != NO_SEG || wrt != NO_SEG)) {
                a_.nonfatal("COFF format does not support non-32-bit relocations");
            } else {
                std::int32_t fix = 0;
                if (segment != NO_SEG || wrt != NO_SEG) {
                    if (wrt != NO_SEG)
                        a_.nonfatal("COFF format does not support WRT types");
                    else if (segment % 2)
                        a_.nonfatal("COFF format does not support segment base references");
                    else
                        fix = add_reloc(*s, segment, IMAGE_REL_I386_DIR32);
                }
                put_long(read_data() + fix);
                sect_write(*s, mydata, static_cast<std::uint32_t>(asize));
            }
        } else {
            std::int32_t fix = 0;
            if (asize == 8) {
                if (wrt == imagebase_sect_) a_.nonfatal("operand size mismatch: 'wrt ..imagebase' is a 32-bit operand");
                fix = add_reloc(*s, segment, IMAGE_REL_AMD64_ADDR64);
                const std::uint64_t u = static_cast<std::uint64_t>(read_data() + fix);   // WRITEDLONG
                for (int k = 0; k < 8; ++k) mydata[k] = static_cast<std::uint8_t>(u >> (8 * k));
                sect_write(*s, mydata, static_cast<std::uint32_t>(asize));
            } else {
                fix = add_reloc(*s, segment, wrt == imagebase_sect_ ? IMAGE_REL_AMD64_ADDR32NB : IMAGE_REL_AMD64_ADDR32);
                put_long(read_data() + fix);
                sect_write(*s, mydata, static_cast<std::uint32_t>(asize));
            }
        }
    } else if (type == OUT_REL2ADR) {
        a_.nonfatal("COFF format does not support 16-bit relocations");
    } else if (type == OUT_REL4ADR) {
        if (segment == segto && !win64()) {   // Acceptable for RIP-relative
            a_.panic("intra-segment OUT_REL4ADR");
        } else if (segment == NO_SEG && win32()) {
            a_.nonfatal("Win32 COFF does not correctly support relative references to absolute addresses");
        } else {
            std::int32_t fix = 0;
            if (segment != NO_SEG && segment % 2)
                a_.nonfatal("COFF format does not support segment base references");
            else
                fix = add_reloc(*s, segment, win64() ? IMAGE_REL_AMD64_REL32 : IMAGE_REL_I386_REL32);
            if (win()) {
                put_long(read_data() + 4 - static_cast<std::int64_t>(size) + fix);
            } else {
                put_long(read_data() - static_cast<std::int64_t>(size + s->len) + fix);
            }
            sect_write(*s, mydata, 4);
        }
    }
}

// ---- .drectve: the -export directive ----

void CoffFormat::add_export(std::string_view name) {
    if (exports_.empty()) {
        std::size_t i;
        for (i = 0; i < sects_.size(); i++)
            if (sects_[i]->name == kExportSectionName) break;

        if (i == sects_.size()) i = static_cast<std::size_t>(make_section(kExportSectionName, kExportSectionFlags));

        directive_sec_ = sects_[i].get();
        exports_.emplace_back(name);
    } else {
        // (As in NASM, the last export already in the list is not compared with the new name.)
        for (std::size_t k = 0; k + 1 < exports_.size(); ++k)
            if (exports_[k] == name) return;
        exports_.emplace_back(name);
    }
}

void CoffFormat::build_export_table() {
    if (exports_.empty()) return;
    for (const std::string& p : exports_) {
        sect_write(*directive_sec_, reinterpret_cast<const std::uint8_t*>("-export:"), 8);
        sect_write(*directive_sec_, reinterpret_cast<const std::uint8_t*>(p.data()), static_cast<std::uint32_t>(p.size()));
        sect_write(*directive_sec_, reinterpret_cast<const std::uint8_t*>(" "), 1);
    }
    exports_.clear();
}

DirectiveResult CoffFormat::directive_export(std::string_view value) {
    // XXX (NASM): pass_first() is really wrong here, but AddExport() needs to be modified to handle duplicate calls for
    // the same value in order to change that. The right thing to do is probably to mark a label as an export in the
    // label structure, in case the label doesn't actually exist.
    std::string_view q = value;
    const std::string_view name = take_word(q);

    if (name.empty()) {
        a_.nonfatal("`export' directive requires export name");
        return DIRR_ERROR;
    }
    if (!q.empty()) {
        a_.nonfatal("unrecognized export qualifier `{}'", q);
        return DIRR_ERROR;
    }
    add_export(name);
    return DIRR_OK;
}

DirectiveResult CoffFormat::directive_safeseh(std::optional<std::string_view> value) {
    if (!win32()) return DIRR_UNKNOWN;   // Only applicable for -f win32

    if (!value) return DIRR_OK;

    std::int32_t ignseg;
    std::int64_t ignoff;
    (void)host_.lookup_label(*value, ignseg, ignoff);   // prevent extern elimination

    if (sxseg_ == -1) {
        std::size_t i;
        for (i = 0; i < sects_.size(); i++)
            if (sects_[i]->name == ".sxdata") break;
        if (i == sects_.size())
            sxseg_ = make_section(".sxdata", IMAGE_SCN_LNK_INFO);
        else
            sxseg_ = static_cast<int>(i);
    }
    // pass_final() is the only time when the full set of symbols are guaranteed to be present as it is the final output
    // pass.
    if (a_.pass_final()) {
        std::size_t n;
        for (n = 0; n < syms_.size(); n++) {
            Symbol& sym = syms_[n];
            if (*value == sym.name) {
                // this value arithmetic effectively reflects initsym in write(): 2 for file, 1 for .absolute and two
                // per each section
                std::uint8_t v[4];
                const std::uint32_t u = static_cast<std::uint32_t>(n + 2 + 1 + sects_.size() * 2);
                for (int k = 0; k < 4; ++k) v[k] = static_cast<std::uint8_t>(u >> (8 * k));
                sect_write(*sects_[static_cast<std::size_t>(sxseg_)], v, 4);
                sym.type = 0x20;
                break;
            }
        }
        if (n == syms_.size()) {
            a_.nonfatal("`safeseh' directive requires valid symbol");
            return DIRR_ERROR;
        }
    }
    return DIRR_OK;
}

DirectiveResult CoffFormat::directive(Directive d, std::optional<std::string_view> value) {
    switch (d) {
    case D_EXPORT:
        if (!value || !a_.pass_first()) return DIRR_OK;   // ignore in pass two
        return directive_export(*value);
    case D_SAFESEH:
        return directive_safeseh(value);
    default:
        return DIRR_UNKNOWN;
    }
}

// ---- writing the module ----

// Handle relocations storm, valid for win32/64 only.
void CoffFormat::adjust_relocs(Section& s) {
    if (static_cast<std::uint32_t>(s.nrelocs) < IMAGE_SCN_MAX_RELOC) return;
    if (kind_ == CoffKind::Coff) a_.fatal("Too many relocations ({}) for section `{}'", s.nrelocs, s.name);

    s.flags |= IMAGE_SCN_LNK_NRELOC_OVFL;
    s.nrelocs++;
}

// Make sure we satisfy all section alignment requirements and put the resulting alignment flags into the flags value in
// the header. If no user-specified alignment is given, use the default for the section type; then either way round up to
// alignment specified by sectalign directives.
void CoffFormat::adjust_alignment(Section& s) {
    std::uint32_t align_flags = s.align_flags;

    if (!align_flags) {
        // No user-specified alignment, use default for partition type
        align_flags = s.flags & IMAGE_SCN_ALIGN_MASK;
    }

    if (align_flags < s.sectalign_flags) align_flags = s.sectalign_flags;

    s.flags = (s.flags & ~IMAGE_SCN_ALIGN_MASK) | align_flags;
}

void CoffFormat::cleanup() {
    out_.clear();
    write();
    Bytes& ofile = host_.ofile();
    ofile.insert(ofile.end(), out_.begin(), out_.end());
}

// ---- the factories (format.hpp) ----

std::unique_ptr<OutputFormat> make_coff_format(FormatHost& host, std::string_view shortname) {
    if (shortname == "coff") return std::make_unique<CoffFormat>(host, CoffKind::Coff);
    if (shortname == "win32") return std::make_unique<CoffFormat>(host, CoffKind::Win32);
    if (shortname == "win64") return std::make_unique<CoffFormat>(host, CoffKind::Win64);
    return nullptr;
}

void CoffFormat::write() {
    // fill in the .drectve section with -export's
    build_export_table();

    if (win32()) {
        // add default value for @feat.00, this allows to 'link /safeseh'
        std::size_t n;
        for (n = 0; n < syms_.size(); n++)
            if (syms_[n].strpos == -1 && syms_[n].name == "@feat.00") break;
        if (n == syms_.size()) symdef("@feat.00", NO_SEG, 1, 0, std::string_view());
    }

    // Check all comdat sections
    for (std::size_t i = 0; i < sects_.size(); i++)
        if (sects_[i]->comdat_name) {
            if (!sects_[i]->comdat_symbol && sects_[i]->comdat_selection != IMAGE_COMDAT_SELECT_ASSOCIATIVE) {
                // This section doesn't have its comdat symbol defined; do it
                define_comdat_name(*sects_[i]->comdat_name, sects_[i]->index);
            }
            if (!sects_[i]->comdat_selection) {
                // This is a placeholder section that wasn't properly defined
                a_.nonfatal("`comdat' associate with symbol `{}` wasn't defined", *sects_[i]->comdat_name);
            }
        }

    // Work out how big the file will get. Calculate the start of the `real' symbols at the same time. Check for massive
    // relocations.
    std::int32_t pos = 0x14 + 0x28 * static_cast<std::int32_t>(sects_.size());
    initsym_ = 3;   // two for the file, one absolute
    for (auto& sp : sects_) {
        Section& s = *sp;
        adjust_alignment(s);
        if (s.has_data) {
            adjust_relocs(s);
            s.pos = pos;
            pos += static_cast<std::int32_t>(s.len);
            s.relpos = pos;
            pos += 10 * s.nrelocs;
        } else {
            s.pos = s.relpos = 0;
        }
        initsym_ += 2;   // two for each section
    }
    const std::int32_t sympos = pos;

    // Output the COFF header.
    put16(out_, win64() ? IMAGE_FILE_MACHINE_AMD64 : IMAGE_FILE_MACHINE_I386);   // machine type
    put16(out_, static_cast<unsigned>(sects_.size()));                           // number of sections
    put32(out_, static_cast<std::uint32_t>(a_.posix_timestamp()));               // timestamp
    put32(out_, static_cast<std::uint32_t>(sympos));
    put32(out_, static_cast<std::uint32_t>(syms_.size() + static_cast<std::size_t>(initsym_)));
    put16(out_, 0);   // no optional header
    // Flags: 32-bit, no line numbers. Win32 doesn't even bother with them.
    put16(out_, win() ? 0 : 0x104);

    // Output the section headers.
    for (auto& sp : sects_) section_header(*sp);

    // Output the sections and their relocations.
    for (auto& sp : sects_) {
        Section& s = *sp;
        if (!s.has_data) continue;
        // Apply symbol table index fixups before writing (newest first, as NASM's list)
        for (auto it = s.symidx_relocs.rbegin(); it != s.symidx_relocs.rend(); ++it) {
            std::uint8_t mydata[4];
            const std::uint32_t v = it->symbol + static_cast<std::uint32_t>(initsym_);
            for (int k = 0; k < 4; ++k) mydata[k] = static_cast<std::uint8_t>(v >> (8 * k));
            const std::size_t n = it->size >= 4 ? 4 : it->size;
            if (it->offset + n <= s.data.size()) std::memcpy(s.data.data() + it->offset, mydata, n);
        }

        out_.insert(out_.end(), s.data.begin(), s.data.end());
        write_relocs(s);

        if (s.flags & IMAGE_SCN_LNK_COMDAT) {
            // Checksum the section data
            s.checksum = crc32b(0, s.data.data(), s.data.size());
        }
    }

    // Output the symbol and string tables.
    write_symbols();
    put32(out_, strslen_ + 4);   // length includes length count
    out_.insert(out_.end(), strs_.begin(), strs_.end());
}

void CoffFormat::section_header(const Section& s) {
    char padname[8];
    std::memset(padname, 0, sizeof padname);

    if (s.namepos == -1) {
        std::memcpy(padname, s.name.data(), std::min<std::size_t>(s.name.size(), 8));
    } else {
        // If name is longer than 8 bytes, write '/' followed by offset into the strings table represented as decimal
        // number.
        std::int32_t namepos = s.namepos % 100000000;
        padname[0] = '/';
        padname[1] = static_cast<char>('0' + (namepos / 1000000));
        namepos = namepos % 1000000;
        padname[2] = static_cast<char>('0' + (namepos / 100000));
        namepos = namepos % 100000;
        padname[3] = static_cast<char>('0' + (namepos / 10000));
        namepos = namepos % 10000;
        padname[4] = static_cast<char>('0' + (namepos / 1000));
        namepos = namepos % 1000;
        padname[5] = static_cast<char>('0' + (namepos / 100));
        namepos = namepos % 100;
        padname[6] = static_cast<char>('0' + (namepos / 10));
        namepos = namepos % 10;
        padname[7] = static_cast<char>('0' + namepos);
    }
    out_.insert(out_.end(), padname, padname + 8);

    put32(out_, 0);   // Virtual size field - set to 0 or vsize
    put32(out_, 0);   // RVA/offset - we ignore
    put32(out_, s.len);
    put32(out_, static_cast<std::uint32_t>(s.pos));
    put32(out_, static_cast<std::uint32_t>(s.relpos));
    put32(out_, 0);   // no line numbers - we don't do 'em

    // a special case -- if there are too many relocs we have to put IMAGE_SCN_MAX_RELOC here and write the real relocs
    // number into VirtualAddress of first relocation
    if (s.flags & IMAGE_SCN_LNK_NRELOC_OVFL)
        put16(out_, IMAGE_SCN_MAX_RELOC);
    else
        put16(out_, static_cast<unsigned>(s.nrelocs));

    put16(out_, 0);   // again, no line numbers
    put32(out_, s.flags);
}

void CoffFormat::write_relocs(const Section& s) {
    // a real number of relocations if needed
    if (s.flags & IMAGE_SCN_LNK_NRELOC_OVFL) {
        put32(out_, static_cast<std::uint32_t>(s.nrelocs));
        put32(out_, 0);
        put16(out_, 0);
    }

    for (const Reloc& r : s.relocs) {
        put32(out_, static_cast<std::uint32_t>(r.address));
        put32(out_, static_cast<std::uint32_t>(r.symbol + (r.symbase == Reloc::REAL_SYMBOLS   ? initsym_
                                                           : r.symbase == Reloc::ABS_SYMBOL   ? initsym_ - 1
                                                           : r.symbase == Reloc::SECT_SYMBOLS ? 2
                                                                                              : 0)));
        put16(out_, static_cast<unsigned>(r.type));
    }
}

void CoffFormat::write_symbol(std::string_view name, bool has_name, std::int32_t strpos, std::int32_t value, int section,
                              int type, int storageclass, int aux) {
    if (has_name) {
        char padname[8];
        std::memset(padname, 0, sizeof padname);
        std::memcpy(padname, name.data(), std::min<std::size_t>(name.size(), 8));
        out_.insert(out_.end(), padname, padname + 8);
    } else {
        put32(out_, 0);
        put32(out_, static_cast<std::uint32_t>(strpos));
    }

    put32(out_, static_cast<std::uint32_t>(value));
    put16(out_, static_cast<unsigned>(section));
    put16(out_, static_cast<unsigned>(type));

    put8(out_, static_cast<unsigned>(storageclass));
    put8(out_, static_cast<unsigned>(aux));
}

void CoffFormat::write_symbols() {
    // The `.file' record, and the file name auxiliary record.
    write_symbol(".file", true, 0, 0, -2, 0, 0x67, 1);
    char filename[18];
    std::memset(filename, 0, sizeof filename);
    if (!a_.reproducible) std::memcpy(filename, a_.inname.data(), std::min<std::size_t>(a_.inname.size(), 18));
    out_.insert(out_.end(), filename, filename + 18);

    // The section records, with their auxiliaries.
    std::memset(filename, 0, sizeof filename);   // useful zeroed buffer

    for (std::size_t i = 0; i < sects_.size(); i++) {
        const Section& s = *sects_[i];
        write_symbol(s.name, true, 0, 0, static_cast<int>(i) + 1, 0, 3, 1);
        put32(out_, s.len);
        put16(out_, static_cast<unsigned>(s.nrelocs));
        if (s.flags & IMAGE_SCN_LNK_COMDAT) {
            put16(out_, 0);
            put32(out_, s.checksum);
            put16(out_, static_cast<unsigned>(s.comdat_associated));
            put8(out_, static_cast<unsigned>(static_cast<std::uint8_t>(s.comdat_selection)));
            out_.insert(out_.end(), filename, filename + 3);
        } else {
            out_.insert(out_.end(), filename, filename + 12);
        }
    }

    // The absolute symbol, for relative-to-absolute relocations.
    write_symbol(".absolut", true, 0, 0, -1, 0, 3, 0);

    // The real symbols.
    for (const Symbol& sym : syms_)
        write_symbol(sym.name, sym.strpos == -1, sym.strpos, sym.value, sym.section, sym.type, sym.is_global ? 2 : 3, 0);
}

void CoffFormat::sectalign(std::int32_t seg, unsigned value) {
    Section* s = nullptr;
    for (auto& sp : sects_) {
        if (sp->index == seg) {
            s = sp.get();
            break;
        }
    }

    if (!s || !is_power2(value)) return;

    if (value > kCoffMaxAlignment) value = kCoffMaxAlignment;   // Do our best...

    const std::uint32_t flags = sectalign_flags_of(value);
    if (flags > s->sectalign_flags) s->sectalign_flags = flags;
}

}  // namespace cb::build::x86asm
