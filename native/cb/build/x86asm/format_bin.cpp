// cb/build/x86asm/format_bin.cpp -- the `bin` output format: NASM's output/outbin.c (flat binary, Intel hex, S-records).
//
// This is the extended version of NASM's original binary output format. It is backward compatible with the original
// BIN format, and contains support for multiple sections and advanced section ordering:
//  - any number of sections, progbits or nobits;
//  - a section is aligned after the previous one ("align="), or positioned at a byte-granular location ("start="), or
//    follows another section ("follows=");
//  - a "virtual" start address for a section ("vstart="), used for the address references to it;
//  - ORG, and the section arguments, take a critical expression;
//  - a map file is made with the `map` directive.
//
// The section list, its re-ordering and the relocation pass keep NASM's intrusive linked list and its pointer surgery,
// because the order of the output depends on every detail of it.
#include "cb/build/x86asm/format_bin.hpp"
#include "cb/build/x86asm/outlib.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <format>
#include <memory>
#include <vector>

namespace cb::build::x86asm {

namespace drvns_format_bin {

// Section flags keep track of which attributes the user has defined.
constexpr int START_DEFINED = 0x001;
constexpr int ALIGN_DEFINED = 0x002;
constexpr int FOLLOWS_DEFINED = 0x004;
constexpr int VSTART_DEFINED = 0x008;
constexpr int VALIGN_DEFINED = 0x010;
constexpr int VFOLLOWS_DEFINED = 0x020;
constexpr int TYPE_DEFINED = 0x040;
constexpr int TYPE_PROGBITS = 0x080;
constexpr int TYPE_NOBITS = 0x100;

// Stuff we need for map-file generation.
constexpr int MAP_ORIGIN = 1;
constexpr int MAP_SUMMARY = 2;
constexpr int MAP_SECTIONS = 4;
constexpr int MAP_SYMBOLS = 8;

struct Section {
    std::string name;
    Bytes contents;
    std::int64_t length = 0;    // section length in bytes

    // Section attributes
    int flags = 0;              // see the flag definitions above
    std::uint64_t align = 0;    // section alignment
    std::uint64_t valign = 0;   // notional section alignment
    std::uint64_t start = 0;    // section start address
    std::uint64_t vstart = 0;   // section virtual start address
    std::string follows;        // the section that this one will follow
    std::string vfollows;       // the section that this one will notionally follow
    std::int32_t start_index = 0;    // NASM section id for non-relocated version
    std::int32_t vstart_index = 0;   // the NASM section id

    std::vector<std::string> labels;   // label handles for map output
    Section* prev = nullptr;           // the previous section (implicit follows)
    Section* next = nullptr;           // links sections with a defined start address
};

struct Reloc {
    std::int32_t posn;
    std::int32_t bytes;
    std::int32_t secref;
    std::int32_t secrel;
    Section* target;
};

std::uint64_t align_up(std::uint64_t x, std::uint64_t a) { return (x + (a - 1)) & ~(a - 1); }

bool is_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }

bool ieq_prefix(std::string_view s, std::string_view prefix) {
    if (s.size() < prefix.size()) return false;
    for (std::size_t i = 0; i < prefix.size(); ++i) {
        char a = s[i], b = prefix[i];
        if (a >= 'A' && a <= 'Z') a = static_cast<char>(a + 32);
        if (b >= 'A' && b <= 'Z') b = static_cast<char>(b + 32);
        if (a != b) return false;
    }
    return true;
}

// exprlib.c's is_really_simple and reloc_value, over an Expr.
bool really_simple(const Expr& e) {
    std::size_t i = 0;
    while (i < e.size() && !e[i].value) ++i;
    if (i >= e.size()) return true;
    if (e[i].type != EXPR_SIMPLE) return false;
    do {
        ++i;
    } while (i < e.size() && !e[i].value);
    return i >= e.size();
}

std::int64_t value_of(const Expr& e) {
    std::size_t i = 0;
    while (i < e.size() && !e[i].value) ++i;
    if (i >= e.size()) return 0;
    return e[i].type == EXPR_SIMPLE ? e[i].value : 0;
}

void write_addr(std::uint8_t* p, std::int64_t value, int bytes) {
    std::uint64_t v = static_cast<std::uint64_t>(value);
    for (int i = 0; i < bytes; ++i) {
        p[i] = static_cast<std::uint8_t>(v);
        v >>= 8;
    }
}

enum Attrib { ATTRIB_START, ATTRIB_ALIGN, ATTRIB_FOLLOWS, ATTRIB_VSTART, ATTRIB_VALIGN, ATTRIB_VFOLLOWS, ATTRIB_NOBITS, ATTRIB_PROGBITS };

enum class BinKind { Bin, Ith, Srec };

class BinFormat final : public OutputFormat {
public:
    BinFormat(FormatHost& host, BinKind kind) : host_(host), as_(host.assembly()), kind_(kind) {}

    std::string_view fullname() const override {
        switch (kind_) {
        case BinKind::Ith: return "Intel Hex encoded flat binary";
        case BinKind::Srec: return "Motorola S-records encoded flat binary";
        default: return "Flat raw binary (MS-DOS, embedded, ...)";
        }
    }
    std::string_view shortname() const override {
        switch (kind_) {
        case BinKind::Ith: return "ith";
        case BinKind::Srec: return "srec";
        default: return "bin";
        }
    }
    std::string_view extension() const override {
        switch (kind_) {
        case BinKind::Ith: return ".ith";   // really should have been ".hex"...
        case BinKind::Srec: return ".srec";
        default: return "";
        }
    }
    unsigned flags() const override { return kind_ == BinKind::Bin ? 0 : OFMT_TEXT; }
    int maxbits() const override { return 64; }

    const std::vector<std::string>& stdmac() const override {
        static const std::vector<std::string> lines = {
            "%define __?SECT?__ [section .text]", "%imacro org 1+.nolist", "[org %1]", "%endmacro", "%macro __?NASM_CDecl?__ 1", "%endmacro"};
        return lines;
    }

    void init() override {
        relocs_.clear();
        origin_defined_ = false;
        no_seg_labels_.clear();

        // Create default section (.text).
        sections_ = last_section_ = new_section();
        last_section_->name = ".text";
        last_section_->flags = TYPE_DEFINED | TYPE_PROGBITS;
        last_section_->start_index = host_.seg_alloc();
        last_section_->vstart_index = host_.seg_alloc();
    }

    void output(const OutData& out) override;
    void symdef(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special) override;
    std::int32_t section(std::optional<std::string_view> name, int& bits) override;
    void sectalign(std::int32_t seg, unsigned int value) override;
    DirectiveResult directive(Directive d, std::optional<std::string_view> args) override;
    void cleanup() override;

private:
    Section* new_section() {
        owned_.push_back(std::make_unique<Section>());
        return owned_.back().get();
    }

    void add_reloc(Section* s, std::int32_t bytes, std::int32_t secref, std::int32_t secrel) {
        relocs_.push_back(Reloc{static_cast<std::int32_t>(s->length), bytes, secref, secrel, s});
    }

    Section* find_section_by_name(std::string_view name) const {
        for (Section* s = sections_; s; s = s->next)
            if (s->name == name) return s;
        return nullptr;
    }

    Section* find_section_by_index(std::int32_t index) const {
        for (Section* s = sections_; s; s = s->next)
            if (index == s->vstart_index || index == s->start_index) return s;
        return nullptr;
    }

    Section* create_section(std::string_view name) {
        Section* s = new_section();
        s->prev = last_section_;
        s->name = std::string(name);

        // Register our sections with NASM.
        s->vstart_index = host_.seg_alloc();
        s->start_index = host_.seg_alloc();

        last_section_->next = s;
        last_section_ = s;
        return last_section_;
    }

    int read_attribute(std::string_view& line, int& attribute, std::uint64_t& value);
    void assign_attributes(Section& sec, std::string_view astring);
    void define_section_labels();

    void do_output_bin();
    void do_output_ith();
    void do_output_srec();
    void write_ith_record(unsigned len, std::uint16_t addr, std::uint8_t type, const std::uint8_t* data);
    void write_srecord(unsigned len, unsigned alen, std::uint32_t addr, std::uint8_t type, const std::uint8_t* data);
    void rf_write(const std::string& text);

    FormatHost& host_;
    Assembly& as_;
    BinKind kind_;

    std::vector<std::unique_ptr<Section>> owned_;
    Section* sections_ = nullptr;
    Section* last_section_ = nullptr;
    std::vector<Reloc> relocs_;
    std::vector<std::string> no_seg_labels_;   // labels in no section, for the map file

    std::uint64_t origin_ = 0;
    bool origin_defined_ = false;

    int map_control_ = 0;
    enum class Report { None, Stdout, Stderr, File } rf_kind_ = Report::None;
    Bytes* rf_file_ = nullptr;
    bool labels_defined_ = false;
};

// The `map` report goes to the file, stdout or stderr the directive chose.
void BinFormat::rf_write(const std::string& text) {
    switch (rf_kind_) {
    case Report::Stdout: host_.stdout_write(text); break;
    case Report::Stderr: host_.stderr_write(text); break;
    case Report::File:
        if (rf_file_) rf_file_->insert(rf_file_->end(), text.begin(), text.end());
        break;
    default: break;
    }
}

void BinFormat::cleanup() {
    Section* g;
    Section** gp;
    Section* gs = nullptr;
    Section** gsp;
    Section* s;
    Section** sp;
    Section* nobits = nullptr;
    Section** nt;
    Section* last_progbits;
    int h;

    if (as_.debug_level(2)) {
        as_.debug(2, "bin_cleanup: Sections were initially referenced in this order:\n");
        h = 0;
        for (s = sections_; s; ++h, s = s->next) as_.debug(2, "{}. {}\n", h, s->name);
    }

    // Assembly has completed, so now we need to generate the output file.
    // Step 1: Separate progbits and nobits sections into separate lists.
    // Step 2: Sort the progbits sections into their output order.
    // Step 3: Compute start addresses for all progbits sections.
    // Step 4: Compute vstart addresses for all sections.
    // Step 5: Apply relocations.
    // Step 6: Write the sections' data to the output file.
    // Step 7: Generate the map file.

    // Step 1: Split progbits and nobits sections into separate lists.

    nt = &nobits;
    // Move nobits sections into a separate list. Also pre-process nobits sections' attributes.
    for (sp = &sections_->next, s = sections_->next; s; s = *sp) {   // skip progbits sections
        if (s->flags & TYPE_PROGBITS) {
            sp = &s->next;
            continue;
        }
        // Do some special pre-processing on nobits sections' attributes.
        if (s->flags & (START_DEFINED | ALIGN_DEFINED | FOLLOWS_DEFINED)) {
            // Check for a mixture of real and virtual section attributes.
            if (s->flags & (VSTART_DEFINED | VALIGN_DEFINED | VFOLLOWS_DEFINED))
                as_.fatal("cannot mix real and virtual attributes in nobits section ({})", s->name);
            // Real and virtual attributes mean the same thing for nobits sections.
            if (s->flags & START_DEFINED) {
                s->vstart = s->start;
                s->flags |= VSTART_DEFINED;
            }
            if (s->flags & ALIGN_DEFINED) {
                s->valign = s->align;
                s->flags |= VALIGN_DEFINED;
            }
            if (s->flags & FOLLOWS_DEFINED) {
                s->vfollows = s->follows;
                s->flags |= VFOLLOWS_DEFINED;
                s->flags &= ~FOLLOWS_DEFINED;
            }
        }
        // Every section must have a start address.
        if (s->flags & VSTART_DEFINED) {
            s->start = s->vstart;
            s->flags |= START_DEFINED;
        }
        // Move the section into the nobits list.
        *sp = s->next;
        s->next = nullptr;
        *nt = s;
        nt = &s->next;
    }

    // Step 2: Sort the progbits sections into their output order.

    // In Step 2 we move around sections in groups. A group begins with a section (group leader) that has a user-defined
    // start address or follows section. The remainder of the group is made up of the sections that implicitly follow the
    // group leader (i.e., they were defined after the group leader and were not given an explicit start address or
    // follows section by the user).
    //
    // g (group) points to a group of sections, the first one of which has a user-defined start address or follows
    // section. gp (g previous) holds the location of the pointer to g. gs (g scan) is a temp variable that we use to scan
    // to the end of the group. gsp (gs previous) holds the location of the pointer to gs. nt (nobits tail) points to the
    // nobits section-list tail.

    // Link all 'follows' groups to their proper position. To do this we need to know three things: the start of the group
    // to relocate (g), the section it is following (s), and the end of the group we're relocating (gs).
    for (gp = &sections_, g = sections_; g; g = gs) {   // find the next follows group that is out of place (g)
        if (!(g->flags & FOLLOWS_DEFINED)) {
            while (g->next) {
                if ((g->next->flags & FOLLOWS_DEFINED) && g->name != g->next->follows) break;
                g = g->next;
            }
            if (!g->next) break;
            gp = &g->next;
            g = g->next;
        }
        // Find the section that this group follows (s).
        for (sp = &sections_, s = sections_; s && s->name != g->follows; sp = &s->next, s = s->next) {
        }
        if (!s) as_.fatal("section {} follows an invalid or unknown section ({})", g->name, g->follows);
        if (s == g) as_.fatal("section {} is self following", s->name);
        if (s->next && (s->next->flags & FOLLOWS_DEFINED) && s->name == s->next->follows)
            as_.fatal("sections {} and {} can't both follow section {}", g->name, s->next->name, s->name);
        // Find the end of the current follows group (gs).
        for (gsp = &g->next, gs = g->next; gs && gs != s && !(gs->flags & START_DEFINED); gsp = &gs->next, gs = gs->next) {
            if (gs->next && (gs->next->flags & FOLLOWS_DEFINED) && gs->name != gs->next->follows) {
                gsp = &gs->next;
                gs = gs->next;
                break;
            }
        }
        // Re-link the group after its follows section.
        *gsp = s->next;
        s->next = g;
        *gp = gs;
    }

    // Link all 'start' groups to their proper position. Once again we need to know g, s, and gs (see above). The main
    // difference is we already know g since we sort by moving groups from the 'unsorted' list into a 'sorted' list (g will
    // always be the first section in the unsorted list).
    for (g = sections_, sections_ = nullptr; g; g = gs) {   // find the section that we will insert this group before (s)
        for (sp = &sections_, s = sections_; s; sp = &s->next, s = s->next)
            if ((s->flags & START_DEFINED) && g->start < s->start) break;
        // Find the end of the group (gs).
        for (gs = g->next, gsp = &g->next; gs && !(gs->flags & START_DEFINED); gsp = &gs->next, gs = gs->next) {
        }
        // Re-link the group before the target section.
        *sp = g;
        *gsp = s;
    }

    // Step 3: Compute start addresses for all progbits sections.

    // Make sure we have an origin and a start address for the first section.
    if (origin_defined_) {
        if (sections_->flags & START_DEFINED) {
            // Make sure this section doesn't begin before the origin.
            if (sections_->start < origin_) as_.fatal("section {} begins before program origin", sections_->name);
        } else if (sections_->flags & ALIGN_DEFINED) {
            sections_->start = align_up(origin_, sections_->align);
        } else {
            sections_->start = origin_;
        }
    } else {
        if (!(sections_->flags & START_DEFINED)) sections_->start = 0;
        origin_ = sections_->start;
    }
    sections_->flags |= START_DEFINED;

    // Make sure each section has an explicit start address. If it doesn't, then compute one based its alignment and the end
    // of the previous section.
    std::uint64_t pend = sections_->start;
    for (g = s = sections_; g; g = g->next) {
        // Find the next section that could cause an overlap situation (has a defined start address, and is not zero length).
        if (g == s)
            for (s = g->next; s && (s->length == 0 || !(s->flags & START_DEFINED)); s = s->next) {
            }
        // Compute the start address of this section, if necessary.
        if (!(g->flags & START_DEFINED)) {
            // Default to an alignment of 4 if unspecified.
            if (!(g->flags & ALIGN_DEFINED)) {
                g->align = 4;
                g->flags |= ALIGN_DEFINED;
            }
            // Set the section start address.
            g->start = align_up(pend, g->align);
            g->flags |= START_DEFINED;
        }
        // Ugly special case for progbits sections' virtual attributes: if there is a defined valign, but no vstart and no
        // vfollows, then we valign after the previous progbits section. This case doesn't really make much sense for
        // progbits sections with a defined start address, but it is possible and we must do *something*. Not-so-ugly special
        // case: if a progbits section has no virtual attributes, we set the vstart equal to the start address.
        if (!(g->flags & (VSTART_DEFINED | VFOLLOWS_DEFINED))) {
            if (g->flags & VALIGN_DEFINED)
                g->vstart = align_up(pend, g->valign);
            else
                g->vstart = g->start;
            g->flags |= VSTART_DEFINED;
        }
        // Ignore zero-length sections.
        if (g->start < pend) continue;
        // Compute the span of this section.
        pend = g->start + static_cast<std::uint64_t>(g->length);
        // Check for section overlap.
        if (s) {
            if (s->start < origin_) as_.fatal("section {} beings before program origin", s->name);
            if (g->start > s->start) as_.fatal("sections {} ~ {} and {} overlap!", gs ? gs->name : std::string("(null)"), g->name, s->name);
            if (pend > s->start) as_.fatal("sections {} and {} overlap!", g->name, s->name);
        }
        // Remember this section as the latest >0 length section.
        gs = g;
    }

    // Step 4: Compute vstart addresses for all sections.

    // Attach the nobits sections to the end of the progbits sections.
    for (s = sections_; s->next; s = s->next) {
    }
    s->next = nobits;
    last_progbits = s;
    // Scan for sections that don't have a vstart address. If we find one we'll attempt to compute its vstart. If we can't
    // compute the vstart, we leave it alone and come back to it in a subsequent scan. We continue scanning and re-scanning
    // until we've gone one full cycle without computing any vstarts.
    do {   // do one full scan of the sections list
        for (h = 0, g = sections_; g; g = g->next) {
            if (g->flags & VSTART_DEFINED) continue;
            // Find the section that this one virtually follows.
            if (g->flags & VFOLLOWS_DEFINED) {
                for (s = sections_; s && g->vfollows != s->name; s = s->next) {
                }
                if (!s) as_.fatal("section {} vfollows unknown section ({})", g->name, g->vfollows);
            } else if (g->prev != nullptr) {
                for (s = sections_; s && s != g->prev; s = s->next) {
                }
            } else {
                // The .bss section is the only one with prev = NULL. In this case we implicitly follow the last progbits
                // section.
                s = last_progbits;
            }

            // If the section we're following has a vstart, we can proceed.
            if (s->flags & VSTART_DEFINED) {   // default to virtual alignment of four
                if (!(g->flags & VALIGN_DEFINED)) {
                    g->valign = 4;
                    g->flags |= VALIGN_DEFINED;
                }
                // Compute the vstart address.
                g->vstart = align_up(s->vstart + static_cast<std::uint64_t>(s->length), g->valign);
                g->flags |= VSTART_DEFINED;
                h++;
                // Start and vstart mean the same thing for nobits sections.
                if (g->flags & TYPE_NOBITS) g->start = g->vstart;
            }
        }
    } while (h);

    // Now check for any circular vfollows references, which will manifest themselves as sections without a defined vstart.
    for (h = 0, s = sections_; s; s = s->next) {
        if (!(s->flags & VSTART_DEFINED)) {
            // Non-fatal errors after assembly has completed are generally a no-no, but we'll throw a fatal one eventually
            // so it's ok.
            as_.nonfatal("cannot compute vstart for section {}", s->name);
            h++;
        }
    }
    if (h) as_.fatal("circular vfollows path detected");

    if (as_.debug_level(2)) {
        as_.debug(2, "bin_cleanup: Confirm final section order for output file:\n");
        h = 0;
        for (s = sections_; s && (s->flags & TYPE_PROGBITS); ++h, s = s->next) as_.debug(2, "{}. {}\n", h, s->name);
    }

    // Step 5: Apply relocations.

    for (const Reloc& r : relocs_) {
        std::uint8_t mydata[8] = {};
        if (r.bytes > 8) as_.assert_failed("r->bytes <= 8");

        // saa_fread(r->target->contents, r->posn, mydata, r->bytes)
        for (int b = 0; b < r.bytes; ++b) {
            const std::size_t at = static_cast<std::size_t>(r.posn) + static_cast<std::size_t>(b);
            mydata[b] = at < r.target->contents.size() ? r.target->contents[at] : std::uint8_t{0};
        }
        std::int64_t l = 0;
        for (int b = r.bytes - 1; b >= 0; b--) l = static_cast<std::int64_t>((static_cast<std::uint64_t>(l) << 8) + mydata[b]);

        s = find_section_by_index(r.secref);
        if (s) {
            if (r.secref == s->start_index)
                l += static_cast<std::int64_t>(s->start);
            else
                l += static_cast<std::int64_t>(s->vstart);
        }
        s = find_section_by_index(r.secrel);
        if (s) {
            if (r.secrel == s->start_index)
                l -= static_cast<std::int64_t>(s->start);
            else
                l -= static_cast<std::int64_t>(s->vstart);
        }

        write_addr(mydata, l, r.bytes);
        for (int b = 0; b < r.bytes; ++b) {
            const std::size_t at = static_cast<std::size_t>(r.posn) + static_cast<std::size_t>(b);
            if (at < r.target->contents.size()) r.target->contents[at] = mydata[b];
        }
    }

    // Step 6: Write the section data to the output file.
    switch (kind_) {
    case BinKind::Ith: do_output_ith(); break;
    case BinKind::Srec: do_output_srec(); break;
    default: do_output_bin(); break;
    }

    // Step 7: Generate the map file.

    if (map_control_) {
        static const char not_defined[] = "not defined";
        std::string m;

        // Display input and output file names.
        m += "\n- NASM Map file ";
        m.append(63, '-');
        m += std::format("\n\nSource file:  {}\nOutput file:  {}\n\n", as_.inname, as_.outname);

        if (map_control_ & MAP_ORIGIN) {   // display program origin
            m += "-- Program origin ";
            m.append(61, '-');
            m += std::format("\n\n{:08X}\n\n", origin_);
        }
        // Display sections summary.
        if (map_control_ & MAP_SUMMARY) {
            m += "-- Sections (summary) ";
            m.append(57, '-');
            m += "\n\nVstart            Start             Stop              Length    Class     Name\n";
            for (s = sections_; s; s = s->next) {
                m += std::format("{:16X}  {:16X}  {:16X}  {:08X}  ", s->vstart, s->start, s->start + static_cast<std::uint64_t>(s->length),
                                 static_cast<std::uint64_t>(s->length));
                if (s->flags & TYPE_PROGBITS)
                    m += "progbits  ";
                else
                    m += "nobits    ";
                m += s->name + "\n";
            }
            m += "\n";
        }
        // Display detailed section information.
        if (map_control_ & MAP_SECTIONS) {
            m += "-- Sections (detailed) ";
            m.append(56, '-');
            m += "\n\n";
            for (s = sections_; s; s = s->next) {
                m += "---- Section " + s->name + " ";
                if (s->name.size() < 65) m.append(65 - s->name.size(), '-');
                m += "\n\nclass:     ";
                if (s->flags & TYPE_PROGBITS)
                    m += "progbits";
                else
                    m += "nobits";
                m += std::format("\nlength:    {:16X}\nstart:     {:16X}\nalign:     ", static_cast<std::uint64_t>(s->length), s->start);
                if (s->flags & ALIGN_DEFINED)
                    m += std::format("{:16X}", s->align);
                else
                    m += not_defined;
                m += "\nfollows:   ";
                if (s->flags & FOLLOWS_DEFINED)
                    m += s->follows;
                else
                    m += not_defined;
                m += std::format("\nvstart:    {:16X}\nvalign:    ", s->vstart);
                if (s->flags & VALIGN_DEFINED)
                    m += std::format("{:16X}", s->valign);
                else
                    m += not_defined;
                m += "\nvfollows:  ";
                if (s->flags & VFOLLOWS_DEFINED)
                    m += s->vfollows;
                else
                    m += not_defined;
                m += "\n\n";
            }
        }
        // Display symbols information.
        if (map_control_ & MAP_SYMBOLS) {
            std::int32_t segment;
            std::int64_t offset;

            m += "-- Symbols ";
            m.append(68, '-');
            m += "\n\n";
            if (!no_seg_labels_.empty()) {
                m += "---- No Section ";
                m.append(63, '-');
                m += "\n\nValue     Name\n";
                for (const std::string& l : no_seg_labels_) {
                    const LabelType found = host_.lookup_label(l, segment, offset);
                    if (found == LBL_none) as_.assert_failed("found_label != LBL_none");
                    m += std::format("{:08X}  {}\n", static_cast<std::uint64_t>(offset), l);
                }
                m += "\n\n";
            }
            for (s = sections_; s; s = s->next) {
                if (!s->labels.empty()) {
                    m += "---- Section " + s->name + " ";
                    // for (h = 65 - strlen(s->name); h; h--): a name longer than 65 makes h negative and the loop runs on
                    // (NASM's behaviour is undefined there; a section name that long does not occur).
                    if (s->name.size() < 65) m.append(65 - s->name.size(), '-');
                    m += "\n\nReal              Virtual           Name\n";
                    for (const std::string& l : s->labels) {
                        const LabelType found = host_.lookup_label(l, segment, offset);
                        if (found == LBL_none) as_.assert_failed("found_label != LBL_none");
                        m += std::format("{:16X}  {:16X}  {}\n", s->start + static_cast<std::uint64_t>(offset), s->vstart + static_cast<std::uint64_t>(offset), l);
                    }
                    m += "\n";
                }
            }
        }
        rf_write(m);
    }
}

void BinFormat::output(const OutData& out) {
    // OUT_LEGACY(out, segto, data, type, size, segment, wrt)
    const std::int32_t segto = out.loc.segment;
    const void* data = out.legacy.data;
    const OutType type = out.legacy.type;
    std::uint64_t size = out.legacy.size;
    std::int32_t segment = out.legacy.tsegment;
    std::int32_t wrt = out.legacy.twrt;
    std::uint8_t mydata[8];

    if (wrt != NO_SEG) {
        wrt = NO_SEG;   // continue to do _something_
        as_.nonfatal("WRT not supported by binary output format");
    }

    // Find the segment we are targeting.
    Section* s = find_section_by_index(segto);
    if (!s) as_.panic("code directed to nonexistent segment?");

    // "Smart" section-type adaptation code.
    if (!(s->flags & TYPE_DEFINED)) {
        if (type == OUT_RESERVE)
            s->flags |= TYPE_DEFINED | TYPE_NOBITS;
        else
            s->flags |= TYPE_DEFINED | TYPE_PROGBITS;
    }

    if ((s->flags & TYPE_NOBITS) && type != OUT_RESERVE) as_.warn(WARN_OTHER, "attempt to initialize memory in a nobits section: ignored");

    switch (type) {
    case OUT_ADDRESS: {
        const int asize = std::abs(static_cast<int>(size));

        if (segment != NO_SEG && !find_section_by_index(segment)) {
            if (segment % 2)
                as_.nonfatal("binary output format does not support segment base references");
            else
                as_.nonfatal("binary output format does not support external references");
            segment = NO_SEG;
        }
        if (s->flags & TYPE_PROGBITS) {
            if (segment != NO_SEG) add_reloc(s, asize, segment, -1);
            std::int64_t value = 0;
            std::memcpy(&value, data, sizeof value);
            write_addr(mydata, value, asize);
            s->contents.insert(s->contents.end(), mydata, mydata + asize);
        }

        // Reassign size with sign dropped, we will need it for section length calculation.
        size = static_cast<std::uint64_t>(asize);
        break;
    }

    case OUT_RAWDATA:
        if (s->flags & TYPE_PROGBITS) {
            const auto* p = static_cast<const std::uint8_t*>(data);
            s->contents.insert(s->contents.end(), p, p + size);
        }
        break;

    case OUT_RESERVE:
        if (s->flags & TYPE_PROGBITS) {
            as_.warn(WARN_ZEROING, "uninitialized space declared in {} section: zeroing", s->name);
            s->contents.insert(s->contents.end(), size, std::uint8_t{0});
        }
        break;

    case OUT_REL1ADR:
    case OUT_REL2ADR:
    case OUT_REL4ADR:
    case OUT_REL8ADR: {
        std::int64_t value = 0;
        std::memcpy(&value, data, sizeof value);
        const std::int64_t addr = value - static_cast<std::int64_t>(size);
        size = realsize(type, size);
        if (segment != NO_SEG && !find_section_by_index(segment)) {
            if (segment % 2)
                as_.nonfatal("binary output format does not support segment base references");
            else
                as_.nonfatal("binary output format does not support external references");
            segment = NO_SEG;
        }
        if (s->flags & TYPE_PROGBITS) {
            add_reloc(s, static_cast<std::int32_t>(size), segment, segto);
            write_addr(mydata, addr - s->length, static_cast<int>(size));
            s->contents.insert(s->contents.end(), mydata, mydata + size);
        }
        break;
    }

    default:
        as_.nonfatal("unsupported relocation type {}\n", static_cast<int>(type));
        break;
    }

    s->length += static_cast<std::int64_t>(size);
}

void BinFormat::symdef(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special) {
    (void)offset;
    // NASM passes `special` as a pointer: null when the label has none. An empty string is none here.
    if (!special.empty())
        as_.nonfatal("binary format does not support any special symbol types");
    else if (name.size() >= 2 && name[0] == '.' && name[1] == '.' && !(name.size() > 2 && name[2] == '@'))
        as_.nonfatal("unrecognised special symbol `{}'", name);
    else if (is_global == 2)
        as_.nonfatal("binary output format does not support common variables");
    else {
        // Remember label definition so we can look it up later when creating the map file.
        Section* s = find_section_by_index(segment);
        if (s)
            s->labels.emplace_back(name);
        else
            no_seg_labels_.emplace_back(name);
    }
}

// Parse one attribute assignment of a `section` directive from the front of `line` (which is advanced past it): returns 1
// for an attribute read, 0 for none, -1 for a bad one (already reported).
int BinFormat::read_attribute(std::string_view& line, int& attribute, std::uint64_t& value) {
    // Skip whitespace.
    while (!line.empty() && is_space(line.front())) line.remove_prefix(1);
    if (line.empty()) return 0;

    int attrib_name_size;

    // Figure out what attribute we're reading.
    if (ieq_prefix(line, "align=")) {
        attribute = ATTRIB_ALIGN;
        attrib_name_size = 6;
    } else {
        if (ieq_prefix(line, "start=")) {
            attribute = ATTRIB_START;
            attrib_name_size = 6;
        } else if (ieq_prefix(line, "follows=")) {
            attribute = ATTRIB_FOLLOWS;
            line.remove_prefix(8);
            return 1;
        } else if (ieq_prefix(line, "vstart=")) {
            attribute = ATTRIB_VSTART;
            attrib_name_size = 7;
        } else if (ieq_prefix(line, "valign=")) {
            attribute = ATTRIB_VALIGN;
            attrib_name_size = 7;
        } else if (ieq_prefix(line, "vfollows=")) {
            attribute = ATTRIB_VFOLLOWS;
            line.remove_prefix(9);
            return 1;
        } else if (ieq_prefix(line, "nobits") && (line.size() == 6 || is_space(line[6]))) {
            attribute = ATTRIB_NOBITS;
            line.remove_prefix(6);
            return 1;
        } else if (ieq_prefix(line, "progbits") && (line.size() == 8 || is_space(line[8]))) {
            attribute = ATTRIB_PROGBITS;
            line.remove_prefix(8);
            return 1;
        } else {
            return 0;
        }
    }

    std::string exp;   // the expression's text

    // Find the end of the expression.
    if (line.size() <= static_cast<std::size_t>(attrib_name_size) || line[static_cast<std::size_t>(attrib_name_size)] != '(') {
        // Single term (no parenthesis).
        line.remove_prefix(static_cast<std::size_t>(attrib_name_size));
        std::size_t n = 0;
        while (n < line.size() && !is_space(line[n])) ++n;
        exp = std::string(line.substr(0, n));
        line.remove_prefix(n);
        if (!line.empty()) line.remove_prefix(1);   // the NUL NASM put over the white space
    } else {
        int pcount = 1;

        // Full expression (delimited by parenthesis).
        line.remove_prefix(static_cast<std::size_t>(attrib_name_size) + 1);
        const std::string_view whole = line;
        std::size_t pos = 0;   // the cursor in `whole`
        auto cur = [&]() -> char { return pos < whole.size() ? whole[pos] : '\0'; };
        while (true) {
            // (*line) += strcspn(*line, "()'\"")
            while (pos < whole.size() && std::strchr("()'\"", whole[pos]) == nullptr) ++pos;
            if (cur() == '(') {
                ++pos;
                ++pcount;
            }
            if (cur() == ')') {
                ++pos;
                --pcount;
                if (!pcount) break;
            }
            if (cur() == '"' || cur() == '\'') {
                const char c = cur();
                while (cur()) {
                    ++pos;
                    if (cur() == c) break;
                }
                if (!cur()) {
                    as_.nonfatal("invalid syntax in `section' directive");
                    line = whole.substr(pos);
                    return -1;
                }
                ++pos;
            }
            if (!cur()) {
                as_.nonfatal("expecting `)'");
                line = whole.substr(pos);
                return -1;
            }
        }
        exp = std::string(whole.substr(0, pos - 1));   // terminate the expression at the closing parenthesis
        line = whole.substr(pos);
    }

    // Check for no value given.
    if (exp.empty()) {
        as_.warn(WARN_OTHER, "No value given to attribute in `section' directive");
        return -1;
    }

    // Read and evaluate the expression.
    const std::optional<Expr> e = host_.evaluate_text(exp, 1);
    if (e) {
        if (!really_simple(*e)) {
            as_.nonfatal("section attribute value must be a critical expression");
            return -1;
        }
    } else {
        as_.nonfatal("Invalid attribute value specified in `section' directive.");
        return -1;
    }
    value = static_cast<std::uint64_t>(value_of(*e));
    return 1;
}

void BinFormat::sectalign(std::int32_t seg, unsigned int value) {
    Section* s = find_section_by_index(seg);

    if (!s || !(value && !(value & (value - 1)))) return;   // is_power2

    if (value > s->align) s->align = value;

    if (!(s->flags & ALIGN_DEFINED)) s->flags |= ALIGN_DEFINED;
}

void BinFormat::assign_attributes(Section& sec, std::string_view astring) {
    int attribute = 0, check;
    std::uint64_t value = 0;

    while (true) {   // get the next attribute
        check = read_attribute(astring, attribute, value);
        // Skip bad attribute.
        if (check == -1) continue;
        // Unknown section attribute, so skip it and warn the user.
        if (!check) {
            if (astring.empty()) break;   // end of line
            std::size_t n = 0;
            while (n < astring.size() && !is_space(astring[n])) ++n;
            const std::string p(astring.substr(0, n));
            astring.remove_prefix(n);
            if (!astring.empty()) astring.remove_prefix(1);
            as_.warn(WARN_OTHER, "ignoring unknown section attribute: \"{}\"", p);
            continue;
        }

        switch (attribute) {
        // Handle nobits attribute.
        case ATTRIB_NOBITS:
            if ((sec.flags & TYPE_DEFINED) && (sec.flags & TYPE_PROGBITS))
                as_.nonfatal("attempt to change section type from progbits to nobits");
            else
                sec.flags |= TYPE_DEFINED | TYPE_NOBITS;
            continue;

        // Handle progbits attribute.
        case ATTRIB_PROGBITS:
            if ((sec.flags & TYPE_DEFINED) && (sec.flags & TYPE_NOBITS))
                as_.nonfatal("attempt to change section type from nobits to progbits");
            else
                sec.flags |= TYPE_DEFINED | TYPE_PROGBITS;
            continue;

        // Handle align attribute.
        case ATTRIB_ALIGN:
            if (!value || ((value - 1) & value)) {
                as_.nonfatal("argument to `align' is not a power of two");
            } else {
                // Alignment is already satisfied if the previous align value is greater.
                if ((sec.flags & ALIGN_DEFINED) && value < sec.align) value = sec.align;

                // Don't allow a conflicting align value.
                if ((sec.flags & START_DEFINED) && (sec.start & (value - 1))) {
                    as_.nonfatal("`align' value conflicts with section start address");
                } else {
                    sec.align = value;
                    sec.flags |= ALIGN_DEFINED;
                }
            }
            continue;

        // Handle valign attribute.
        case ATTRIB_VALIGN:
            if (!value || ((value - 1) & value)) {
                as_.nonfatal("argument to `valign' is not a power of two");
            } else {   // alignment is already satisfied if the previous align value is greater
                if ((sec.flags & VALIGN_DEFINED) && value < sec.valign) value = sec.valign;

                // Don't allow a conflicting valign value.
                if ((sec.flags & VSTART_DEFINED) && (sec.vstart & (value - 1))) {
                    as_.nonfatal("`valign' value conflicts with `vstart' address");
                } else {
                    sec.valign = value;
                    sec.flags |= VALIGN_DEFINED;
                }
            }
            continue;

        // Handle start attribute.
        case ATTRIB_START:
            if (sec.flags & FOLLOWS_DEFINED) {
                as_.nonfatal("cannot combine `start' and `follows' section attributes");
            } else if ((sec.flags & START_DEFINED) && value != sec.start) {
                as_.nonfatal("section start address redefined");
            } else {
                sec.start = value;
                sec.flags |= START_DEFINED;
                if (sec.flags & ALIGN_DEFINED) {
                    if (sec.start & (sec.align - 1)) as_.nonfatal("`start' address conflicts with section alignment");
                    sec.flags ^= ALIGN_DEFINED;
                }
            }
            continue;

        // Handle vstart attribute.
        case ATTRIB_VSTART:
            if (sec.flags & VFOLLOWS_DEFINED) {
                as_.nonfatal("cannot combine `vstart' and `vfollows' section attributes");
            } else if ((sec.flags & VSTART_DEFINED) && value != sec.vstart) {
                as_.nonfatal("section virtual start address (vstart) redefined");
            } else {
                sec.vstart = value;
                sec.flags |= VSTART_DEFINED;
                if (sec.flags & VALIGN_DEFINED) {
                    if (sec.vstart & (sec.valign - 1)) as_.nonfatal("`vstart' address conflicts with `valign' value");
                    sec.flags ^= VALIGN_DEFINED;
                }
            }
            continue;

        // Handle follows attribute.
        case ATTRIB_FOLLOWS: {
            std::size_t n = 0;
            while (n < astring.size() && astring[n] != ' ' && astring[n] != '\t') ++n;   // strcspn(astring, " \t")
            if (n == 0) {
                as_.nonfatal("expecting section name for `follows' attribute");
            } else {
                const std::string p(astring.substr(0, n));
                astring.remove_prefix(n);
                if (!astring.empty()) astring.remove_prefix(1);
                if (sec.flags & START_DEFINED) as_.nonfatal("cannot combine `start' and `follows' section attributes");
                sec.follows = p;
                sec.flags |= FOLLOWS_DEFINED;
            }
            continue;
        }

        // Handle vfollows attribute.
        case ATTRIB_VFOLLOWS:
            if (sec.flags & VSTART_DEFINED) {
                as_.nonfatal("cannot combine `vstart' and `vfollows' section attributes");
            } else {
                std::size_t n = 0;
                while (n < astring.size() && astring[n] != ' ' && astring[n] != '\t') ++n;
                if (n == 0) {
                    as_.nonfatal("expecting section name for `vfollows' attribute");
                } else {
                    const std::string p(astring.substr(0, n));
                    astring.remove_prefix(n);
                    if (!astring.empty()) astring.remove_prefix(1);
                    sec.vfollows = p;
                    sec.flags |= VFOLLOWS_DEFINED;
                }
            }
            continue;
        }
    }
}

void BinFormat::define_section_labels() {
    if (labels_defined_) return;
    for (Section* sec = sections_; sec; sec = sec->next) {
        host_.define_label("section." + sec->name + ".start", sec->start_index, 0, false);   // section.<name>.start
        host_.define_label("section." + sec->name + ".vstart", sec->vstart_index, 0, false);   // section.<name>.vstart
    }
    labels_defined_ = true;
}

std::int32_t BinFormat::section(std::optional<std::string_view> name, int& bits) {
    // bin_secname is called with *name = NULL at the start of each pass. Use this opportunity to establish the default
    // section (default is BITS-16 ".text" segment).
    if (!name) {
        // Reset ORG and section attributes at the start of each pass.
        origin_defined_ = false;
        for (Section* sec = sections_; sec; sec = sec->next)
            sec->flags &= ~(START_DEFINED | VSTART_DEFINED | ALIGN_DEFINED | VALIGN_DEFINED);

        // Define section start and vstart labels.
        if (!as_.pass_first()) define_section_labels();

        // Establish the default (.text) section.
        bits = 16;
        Section* sec = find_section_by_name(".text");
        sec->flags |= TYPE_DEFINED | TYPE_PROGBITS;
        return sec->vstart_index;
    }

    // Attempt to find the requested section. If it does not exist, create it.
    std::size_t p = 0;
    while (p < name->size() && !is_space((*name)[p])) ++p;
    const std::string_view secname = name->substr(0, p);
    const std::string_view rest = p < name->size() ? name->substr(p + 1) : std::string_view();
    Section* sec = find_section_by_name(secname);
    if (!sec) {
        sec = create_section(secname);
        if (secname == ".data") {
            sec->flags |= TYPE_DEFINED | TYPE_PROGBITS;
        } else if (secname == ".bss") {
            sec->flags |= TYPE_DEFINED | TYPE_NOBITS;
            sec->prev = nullptr;
        }
    }

    // Handle attribute assignments.
    if (!as_.pass_first()) assign_attributes(*sec, rest);

    // The following line disables smart adaptation of PROGBITS/NOBITS section types (it forces sections to default to
    // PROGBITS).
    if (!as_.pass_first() && !(sec->flags & TYPE_DEFINED)) sec->flags |= TYPE_DEFINED | TYPE_PROGBITS;

    return sec->vstart_index;
}

DirectiveResult BinFormat::directive(Directive d, std::optional<std::string_view> args) {
    switch (d) {
    case D_ORG:
        if (args) {
            const std::optional<Expr> e = host_.evaluate_text(*args, 1);
            if (!e) {
                as_.nonfatal("No or invalid offset specified in ORG directive.");
                return DIRR_ERROR;
            }
            if (!really_simple(*e)) {
                as_.nonfatal("ORG value must be a critical expression");
                return DIRR_ERROR;
            }
            const std::uint64_t value = static_cast<std::uint64_t>(value_of(*e));
            // Check for ORG redefinition.
            if (origin_defined_ && value != origin_) {
                as_.nonfatal("program origin redefined");
                return DIRR_ERROR;
            }
            origin_ = value;
            origin_defined_ = true;
        }
        return DIRR_OK;

    case D_MAP:
        // The 'map' directive allows the user to generate section and symbol information to stdout, stderr, or to a file.
        if (args && as_.pass_first()) {
            std::string_view rest = *args;
            while (!rest.empty() && (rest.front() == ' ' || rest.front() == '\t')) rest.remove_prefix(1);   // strspn(args, " \t")
            while (!rest.empty()) {
                std::size_t n = 0;
                while (n < rest.size() && rest[n] != ' ' && rest[n] != '\t') ++n;
                const std::string p(rest.substr(0, n));
                rest.remove_prefix(n);
                if (!rest.empty()) rest.remove_prefix(1);
                if (ieq_prefix(p, "all") && p.size() == 3)
                    map_control_ |= MAP_ORIGIN | MAP_SUMMARY | MAP_SECTIONS | MAP_SYMBOLS;
                else if (ieq_prefix(p, "brief") && p.size() == 5)
                    map_control_ |= MAP_ORIGIN | MAP_SUMMARY;
                else if (ieq_prefix(p, "sections") && p.size() == 8)
                    map_control_ |= MAP_ORIGIN | MAP_SUMMARY | MAP_SECTIONS;
                else if (ieq_prefix(p, "segments") && p.size() == 8)
                    map_control_ |= MAP_ORIGIN | MAP_SUMMARY | MAP_SECTIONS;
                else if (ieq_prefix(p, "symbols") && p.size() == 7)
                    map_control_ |= MAP_SYMBOLS;
                else if (rf_kind_ == Report::None) {
                    if (ieq_prefix(p, "stdout") && p.size() == 6) {
                        rf_kind_ = Report::Stdout;
                    } else if (ieq_prefix(p, "stderr") && p.size() == 6) {
                        rf_kind_ = Report::Stderr;
                    } else {   // must be a filename
                        rf_file_ = host_.open_write(p, true);
                        if (!rf_file_) {
                            as_.nonfatal("unable to open map file `{}'", p);
                            map_control_ = 0;
                            return DIRR_OK;
                        }
                        rf_kind_ = Report::File;
                    }
                } else {
                    as_.warn(WARN_OTHER, "map file already specified");
                }
            }
            if (map_control_ == 0) map_control_ |= MAP_ORIGIN | MAP_SUMMARY;
            if (rf_kind_ == Report::None) rf_kind_ = Report::Stdout;
        }
        return DIRR_OK;

    default:
        return DIRR_UNKNOWN;
    }
}

// Generate binary file output.
void BinFormat::do_output_bin() {
    Bytes& ofile = host_.ofile();
    std::uint64_t addr = origin_;

    // Write the progbits sections to the output file.
    for (Section* s = sections_; s; s = s->next) {
        // Skip non-progbits sections.
        if (!(s->flags & TYPE_PROGBITS)) continue;
        // Skip zero-length sections.
        if (s->length == 0) continue;

        // Pad the space between sections.
        if (addr > s->start) as_.assert_failed("addr <= s->start");
        ofile.insert(ofile.end(), s->start - addr, std::uint8_t{0});

        // Write the section to the output file.
        ofile.insert(ofile.end(), s->contents.begin(), s->contents.end());

        // Keep track of the current file position.
        addr = s->start + static_cast<std::uint64_t>(s->length);
    }
}

// Generate Intel hex file output.
void BinFormat::write_ith_record(unsigned len, std::uint16_t addr, std::uint8_t type, const std::uint8_t* data) {
    if (len > 255) as_.assert_failed("len <= 255");

    std::uint8_t csum = static_cast<std::uint8_t>(len + addr + (addr >> 8) + type);
    for (unsigned i = 0; i < len; i++) csum = static_cast<std::uint8_t>(csum + data[i]);
    csum = static_cast<std::uint8_t>(-csum);

    std::string line = std::format(":{:02X}{:04X}{:02X}", len, addr, type);
    for (unsigned i = 0; i < len; i++) line += std::format("{:02X}", data[i]);
    line += std::format("{:02X}\n", csum);

    host_.ofile().insert(host_.ofile().end(), line.begin(), line.end());
}

void BinFormat::do_output_ith() {
    std::uint8_t buf[32];
    std::uint64_t hilba = 0;

    // Write the progbits sections to the output file.
    for (Section* s = sections_; s; s = s->next) {
        // Skip non-progbits sections.
        if (!(s->flags & TYPE_PROGBITS)) continue;
        // Skip zero-length sections.
        if (s->length == 0) continue;

        std::uint64_t addr = s->start;
        std::uint64_t length = static_cast<std::uint64_t>(s->length);
        std::size_t rpos = 0;   // saa_rewind

        while (length) {
            const std::uint64_t hiaddr = addr >> 16;
            if (hiaddr != hilba) {
                buf[0] = static_cast<std::uint8_t>(hiaddr >> 8);
                buf[1] = static_cast<std::uint8_t>(hiaddr);
                write_ith_record(2, 0, 4, buf);
                hilba = hiaddr;
            }

            unsigned chunk = static_cast<unsigned>(32 - (addr & 31));
            if (length < chunk) chunk = static_cast<unsigned>(length);

            std::memcpy(buf, s->contents.data() + rpos, chunk);   // saa_rnbytes
            rpos += chunk;
            write_ith_record(chunk, static_cast<std::uint16_t>(addr), 0, buf);

            addr += chunk;
            length -= chunk;
        }
    }

    // Write closing record.
    write_ith_record(0, 0, 1, nullptr);
}

// Generate Motorola S-records.
void BinFormat::write_srecord(unsigned len, unsigned alen, std::uint32_t addr, std::uint8_t type, const std::uint8_t* data) {
    if (len > 255) as_.assert_failed("len <= 255");

    switch (alen) {
    case 2: addr &= 0xffff; break;
    case 3: addr &= 0xffffff; break;
    case 4: break;
    default: as_.panic_here();
    }

    std::uint8_t csum = static_cast<std::uint8_t>((len + alen + 1) + addr + (addr >> 8) + (addr >> 16) + (addr >> 24));
    for (unsigned i = 0; i < len; i++) csum = static_cast<std::uint8_t>(csum + data[i]);
    csum = static_cast<std::uint8_t>(0xff - csum);

    std::string line = std::format("S{:c}{:02X}{:0{}X}", static_cast<char>(type), len + alen + 1, addr, alen * 2);
    for (unsigned i = 0; i < len; i++) line += std::format("{:02X}", data[i]);
    line += std::format("{:02X}\n", csum);

    host_.ofile().insert(host_.ofile().end(), line.begin(), line.end());
}

void BinFormat::do_output_srec() {
    std::uint8_t buf[32];
    std::uint64_t maxaddr = 0;

    for (Section* s = sections_; s; s = s->next) {
        // Skip non-progbits sections.
        if (!(s->flags & TYPE_PROGBITS)) continue;
        // Skip zero-length sections.
        if (s->length == 0) continue;

        const std::uint64_t addr = s->start + static_cast<std::uint64_t>(s->length) - 1;
        if (addr > maxaddr) maxaddr = addr;
    }

    unsigned alen;
    char dtype, etype;
    if (maxaddr <= 0xffff) {
        alen = 2;
        dtype = '1';   // S1 = 16-bit data
        etype = '9';   // S9 = 16-bit end
    } else if (maxaddr <= 0xffffff) {
        alen = 3;
        dtype = '2';   // S2 = 24-bit data
        etype = '8';   // S8 = 24-bit end
    } else {
        alen = 4;
        dtype = '3';   // S3 = 32-bit data
        etype = '7';   // S7 = 32-bit end
    }

    // Write head record.
    write_srecord(0, 2, 0, '0', nullptr);

    // Write the progbits sections to the output file.
    for (Section* s = sections_; s; s = s->next) {
        // Skip non-progbits sections.
        if (!(s->flags & TYPE_PROGBITS)) continue;
        // Skip zero-length sections.
        if (s->length == 0) continue;

        std::uint64_t addr = s->start;
        std::uint64_t length = static_cast<std::uint64_t>(s->length);
        std::size_t rpos = 0;

        while (length) {
            unsigned chunk = static_cast<unsigned>(32 - (addr & 31));
            if (length < chunk) chunk = static_cast<unsigned>(length);

            std::memcpy(buf, s->contents.data() + rpos, chunk);
            rpos += chunk;
            write_srecord(chunk, alen, static_cast<std::uint32_t>(addr), static_cast<std::uint8_t>(dtype), buf);

            addr += chunk;
            length -= chunk;
        }
    }

    // Write closing record.
    write_srecord(0, alen, 0, static_cast<std::uint8_t>(etype), nullptr);
}

}  // namespace drvns_format_bin
using namespace drvns_format_bin;

std::unique_ptr<OutputFormat> make_bin_format(FormatHost& host, std::string_view shortname) {
    if (shortname == "bin") return std::make_unique<BinFormat>(host, BinKind::Bin);
    if (shortname == "ith") return std::make_unique<BinFormat>(host, BinKind::Ith);
    if (shortname == "srec") return std::make_unique<BinFormat>(host, BinKind::Srec);
    return nullptr;
}

}  // namespace cb::build::x86asm
