// build/x86asm/listing.cpp -- the listing file generator (NASM's asm/listing.c).
#include "build/x86asm/listing.hpp"

#include <format>

namespace cb::build::x86asm {

namespace drvns_listing {

constexpr std::size_t LIST_MAX_LEN = 1024;   // something sensible
constexpr std::size_t LIST_HEXBIT = 18;

// The options to enable by increasing levels of -L+ on the command line. Each string is inclusive of the previous
// level; "" is allowed to mean that a level has no impact.
constexpr const char* kListPlusOptions[] = {"bdefFmps", "ct"};

constexpr char kXdigit[] = "0123456789ABCDEF";

char err_fill_char(ErrFlags severity) {
    severity &= ERR_MASK;
    if (severity < ERR_NOTE) return ' ';
    if (severity < ERR_WARNING) return '-';
    if (severity < ERR_CRITICAL) return '*';
    return 'X';
}

void put(Bytes& fp, std::string_view text) { fp.insert(fp.end(), text.begin(), text.end()); }

}  // namespace drvns_listing
using namespace drvns_listing;

void Listing::emit() {
    if (!fp_) return;
    if (listlinep_ || !listdata_.empty()) {
        put(*fp_, std::format("{:6} ", listlineno_));

        if (!listdata_.empty())
            put(*fp_, std::format("{:08X} {:<{}}", static_cast<std::uint32_t>(listoffset_), listdata_, LIST_HEXBIT + 1));
        else
            put(*fp_, std::string(LIST_HEXBIT + 10, ' '));

        if (listlevel_e_)
            put(*fp_, std::format("{}<{}>", listlevel_ < 10 ? " " : "", listlevel_e_));
        else if (listlinep_)
            put(*fp_, "    ");

        if (listlinep_) put(*fp_, " " + listline_);

        put(*fp_, "\n");
        listlinep_ = false;
        listdata_.clear();
    }

    if (!list_errors_.empty()) {
        for (const auto& [severity, text] : list_errors_) {
            put(*fp_, std::format("{:6}          ", listlineno_));
            put(*fp_, std::string(LIST_HEXBIT, err_fill_char(severity)));

            if (listlevel_e_)
                put(*fp_, std::format(" {}<{}>", listlevel_ < 10 ? " " : "", listlevel_e_));
            else
                put(*fp_, "     ");

            put(*fp_, "  " + text + "\n");
        }
        list_errors_.clear();
    }
}

void Listing::cleanup() {
    if (!fp_) return;
    emit();
    fp_ = nullptr;
    active_list_options = 0;
}

void Listing::init(std::string_view fname) {
    if (fp_) cleanup();

    if (fname.empty()) {
        fp_ = nullptr;
        return;
    }

    fp_ = open_(fname);
    if (!fp_) {
        as_.nonfatal("unable to open listing file `{}'", fname);
        return;
    }
    fp_->clear();

    active_list_options = list_options | 1;

    listline_.clear();
    listlineno_ = 0;
    list_errors_.clear();
    listlevel_ = 0;
    suppress_ = 0;
    user_nolist = false;
}

void Listing::out(std::int64_t offset, std::string_view str) {
    if (listdata_.size() + str.size() > LIST_HEXBIT) {
        listdata_ += '-';
        emit();
    }
    if (listdata_.empty()) listoffset_ = static_cast<std::int32_t>(offset);
    listdata_ += str;
}

void Listing::address(std::int64_t offset, const char* brackets, std::int64_t addr, int size) {
    if (size > 8) as_.assert_failed("size <= 8");
    std::string q;
    q += brackets[0];
    std::uint64_t a = static_cast<std::uint64_t>(addr);
    while (size--) {
        const unsigned b = static_cast<unsigned>(a & 0xff);
        q += kXdigit[(b >> 4) & 15];
        q += kXdigit[b & 15];
        a >>= 8;
    }
    q += brackets[1];
    out(offset, q);
}

void Listing::size_tag(std::int64_t offset, const char* tag, std::uint64_t size) {
    if (option('d'))
        out(offset, std::format("<{} {}>", tag, size));
    else
        out(offset, std::format("<{} {:X}h>", tag, size));
}

void Listing::output(const OutData& data) {
    if (!fp_ || suppress_ || user_nolist) return;

    std::uint64_t size = data.size;
    std::uint64_t offset = static_cast<std::uint64_t>(data.loc.offset);
    const std::uint8_t* p = static_cast<const std::uint8_t*>(data.data);
    static const std::uint8_t zero_buffer[16] = {};

    switch (data.type) {
    case OUT_ZERODATA:
        if (size > 16) {
            size_tag(static_cast<std::int64_t>(offset), "zero", size);
            break;
        }
        p = zero_buffer;
        [[fallthrough]];
    case OUT_RAWDATA:
        if (size == 0) {
            if (listdata_.empty()) listoffset_ = static_cast<std::int32_t>(data.loc.offset);
        } else if (p) {
            while (size--) {
                std::string q;
                q += kXdigit[(*p >> 4) & 15];
                q += kXdigit[*p & 15];
                out(static_cast<std::int64_t>(offset++), q);
                ++p;
            }
        } else {
            // used for listing on non-code generation passes with -Lp
            size_tag(static_cast<std::int64_t>(offset), "len", size);
        }
        break;
    case OUT_ADDRESS:
        address(static_cast<std::int64_t>(offset), "[]", static_cast<std::int64_t>(data.toffset), static_cast<int>(size));
        break;
    case OUT_SEGMENT: {
        std::string q = "[";
        q.append(size << 1, 's');
        q += ']';
        out(static_cast<std::int64_t>(offset), q);
        offset += size;
        break;
    }
    case OUT_RELADDR:
        address(static_cast<std::int64_t>(offset), "()", static_cast<std::int64_t>(data.toffset), static_cast<int>(size));
        break;
    case OUT_RESERVE:
        if (size > 8) {
            size_tag(static_cast<std::int64_t>(offset), "res", size);
        } else {
            out(static_cast<std::int64_t>(offset), std::string(size << 1, '?'));
        }
        break;
    default:
        as_.panic_here();
    }
}

void Listing::line(ListType type, std::int32_t lineno, std::string_view text) {
    (void)type;
    if (!fp_) return;
    if (user_nolist) return;

    emit();
    if (lineno >= 0) listlineno_ = lineno;
    listlinep_ = true;
    // strlcpy(listline, line, LIST_MAX_LEN-3) then "..." at LIST_MAX_LEN-4: a line of 1020 characters or more is cut and dotted
    if (text.size() >= LIST_MAX_LEN - 4)
        listline_ = std::string(text.substr(0, LIST_MAX_LEN - 4)) + "...";
    else
        listline_ = std::string(text);
    listlevel_e_ = listlevel_;
}

void Listing::uplevel(ListType type, std::int64_t size) {
    if (!fp_) return;

    switch (type) {
    case ListType::INCBIN:
        if (!option('c')) {
            suppress_ |= 1;
            size_tag(listoffset_, "bin", static_cast<std::uint64_t>(size));
        }
        break;
    case ListType::TIMES:
        if (!option('t')) {
            suppress_ |= 2;
            size_tag(listoffset_, "rep", static_cast<std::uint64_t>(size));
        }
        break;
    case ListType::INCLUDE:
        ++listlevel_;
        break;
    default:
        ++listlevel_;
        break;
    }
}

void Listing::downlevel(ListType type) {
    if (!fp_) return;

    switch (type) {
    case ListType::INCBIN:
        suppress_ &= ~1;
        break;
    case ListType::TIMES:
        suppress_ &= ~2;
        break;
    default:
        --listlevel_;
        break;
    }
}

void Listing::error(ErrFlags severity, std::string_view text) {
    if (!fp_) return;

    list_errors_.emplace_back(severity, std::string(text));

    if ((severity & ERR_MASK) >= ERR_FATAL) emit();
}

// from_cmdline: treats + as -L+... (list_plus_options) rather than a modifier; disallows the * modifier (= set to the
// command line default); and suppresses setting active_list_options (list_init does that). As listing options are assumed
// non-critical, errors are ignored to help forward compatibility.
void Listing::update_options(std::string_view str, bool from_cmdline) {
    std::uint64_t setmask = LIST_ALL_OPTIONS_MASK;
    const std::uint64_t amask = from_cmdline ? 0 : setmask;
    constexpr std::size_t nplus = std::size(kListPlusOptions);
    std::size_t cmdline_next_plus = from_cmdline ? 0 : nplus;   // an index past the end: none left (NASM points at the NULL)

    for (const char ch : str) {
        const unsigned char c = static_cast<unsigned char>(ch);
        std::uint64_t mask = 0;
        switch (c) {
        case '+':
            if (cmdline_next_plus < nplus) update_options(kListPlusOptions[cmdline_next_plus++], true);
            setmask = LIST_ALL_OPTIONS_MASK;
            break;
        case '-':
            setmask = 0;
            break;
        case '*':
            if (!from_cmdline) setmask = cmdline_list_options;
            break;
        case '!':
            mask = LIST_ALL_OPTIONS_MASK;
            break;
        default:
            mask = list_option_mask(c);
            break;
        }

        if (mask) {
            list_options = (list_options & ~mask) | (setmask & mask);
            active_list_options = (active_list_options & ~mask) | (setmask & amask & mask);
        }
    }

    if (from_cmdline) cmdline_list_options = list_options;
}

DirectiveResult Listing::pragma(const Pragma& p) {
    switch (p.opcode) {
    case D_OPTIONS:
        update_options(p.tail, false);
        return DIRR_OK;
    default:
        return DIRR_UNKNOWN;
    }
}

}  // namespace cb::build::x86asm
