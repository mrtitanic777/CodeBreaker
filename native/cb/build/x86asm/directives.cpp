// cb/build/x86asm/directives.cpp -- assembler directives and pragmas: NASM's asm/directiv.c and asm/pragma.c.
#include "cb/build/x86asm/directives.hpp"

#include <array>
#include <cstring>

#include "cb/build/x86asm/expr_util.hpp"
#include "cb/build/x86asm/floats.hpp"
#include "cb/build/x86asm/format.hpp"
#include "cb/build/x86asm/listing.hpp"
#include "cb/build/x86asm/nctype.hpp"
#include "cb/build/x86asm/quote.hpp"
#include "cb/build/x86asm/scan.hpp"

namespace cb::build::x86asm {

namespace drvns_directives {

// The names of the directives, from D_ABSOLUTE on, nullptr for the separators (directive_tbl).
constexpr std::array<const char*, 65> kDirectiveNames = {{
    "absolute", "bits", "common", "cpu", "debug", "default", "dollarhex", "extern", "float", "global", "list", "pragma", "required",
    "sectalign", "section", "segment", "static", "warning",
    "prefix", "suffix", "postfix", "gprefix", "gsuffix", "gpostfix", "lprefix", "lsuffix", "lpostfix",
    nullptr,   // pseudo_ops
    "db", "dw", "dd", "dq", "dt", "do", "dy", "dz", "resb", "resw", "resd", "resq", "rest", "reso", "resy", "resz", "incbin", "equ",
    nullptr,   // ofmt
    "export", "group", "import", "library", "map", "module", "org", "osabi", "safeseh", "uppercase",
    nullptr,   // pragma_tokens
    "limit", "options", "subsections_via_symbols", "no_dead_strip", "maxdump", "nodepend", "noseclabels"}};

bool dir_ieq(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (nasm_tolower(a[i]) != nasm_tolower(b[i])) return false;
    return true;
}

// The C string that begins at `i` of `s` (it ends at the first NUL or the end): NASM edits its line in place by writing
// NULs, and so does this port, on a copy.
std::string_view cstr(const std::string& s, std::size_t i) {
    if (i >= s.size()) return {};
    const std::size_t nul = s.find('\0', i);
    return std::string_view(s).substr(i, nul == std::string::npos ? std::string::npos : nul - i);
}
char at(const std::string& s, std::size_t i) { return i < s.size() ? s[i] : '\0'; }
std::size_t dir_skip_spaces(const std::string& s, std::size_t i) {
    while (at(s, i) && nasm_isspace(at(s, i))) ++i;
    return i;
}

struct DirectiveLine {
    Directive d = D_none;
    std::string text;           // the line, edited in place as NASM edits it
    std::size_t directive = 0;  // where the directive's name starts in `text`
    std::size_t value = 0;      // where its value starts
};

}  // namespace drvns_directives
using namespace drvns_directives;

Directive directive_find(std::string_view name) {
    if (name.empty()) return D_unknown;
    for (std::size_t i = 0; i < kDirectiveNames.size(); ++i)
        if (kDirectiveNames[i] && dir_ieq(name, kDirectiveNames[i])) return static_cast<Directive>(i + 3);
    return D_unknown;
}

const char* directive_name(Directive d) {
    const std::size_t ix = static_cast<std::size_t>(d) - 3;
    return ix < kDirectiveNames.size() ? kDirectiveNames[ix] : nullptr;
}

// ---- directiv.c ----

namespace drvns_directives {

// Find the directive in a line: `[directive value]`, with a possible comment after the ]. D_none if it is not one, D_corrupt if
// it is malformed. (parse_directive_line)
Directive parse_directive_line(Assembly& as, DirectiveLine& dl, std::string_view line) {
    std::string& s = dl.text;
    s = std::string(line);

    std::size_t buf = dir_skip_spaces(s, 0);

    // It should be enclosed in [ ]. Strip off the comments. We should really strip the comments in generic code, not here.
    if (at(s, buf) != '[') return D_none;

    std::size_t q = buf;
    char c;
    while ((c = at(s, q)) != ']') {
        switch (c) {
        case '\0':
        case ';': return D_corrupt;   // no ] in directive
        case '\'':
        case '"':
        case '`': {
            const auto end = nasm_skip_string(cstr(s, q));
            q += end ? *end : cstr(s, q).size();
            if (!at(s, q)) return D_corrupt;
            ++q;
            break;
        }
        default: ++q; break;
        }
    }

    // Found the ] at the end of the directive. Make sure there isn't anything else at the end of the line, except a possible
    // comment.
    const std::size_t eol = dir_skip_spaces(s, q + 1);
    if (at(s, eol) != '\0' && at(s, eol) != ';') as.warn(WARN_DIRECTIVE_GARBAGE_EOL, "garbage found on line after directive");

    // no brace, no trailing spaces
    s[q] = '\0';
    // nasm_zap_spaces_rev(--q)
    --q;
    while (q < s.size() && at(s, q) && nasm_isspace(at(s, q))) {
        s[q] = '\0';
        if (q == 0) break;
        --q;
    }

    // directive
    std::size_t p = dir_skip_spaces(s, ++buf);
    q = p;
    while (at(s, q) && !nasm_isspace(at(s, q))) ++q;   // nasm_skip_word
    s[q] = '\0';
    dl.directive = p;

    // and value finally
    p = dir_skip_spaces(s, ++q);
    dl.value = p;

    return directive_find(cstr(s, dl.directive));
}

}  // namespace drvns_directives
using namespace drvns_directives;

bool Directives::directive_valid(std::string_view directive) {
    const Directive d = directive_find(directive);

    if (d <= D_corrupt) return false;
    if (d < D_ofmt) return true;   // global directive or pseudo-op
    if (d < D_pragma_tokens) return host_.ofmt->directive(d, std::nullopt) == DIRR_OK;
    return false;
}

bool Directives::process_directives(std::string_view line) {
    DirectiveLine dl;
    bool bad_param = false;

    const Directive d = parse_directive_line(as_, dl, line);
    std::string& s = dl.text;
    const std::string directive(cstr(s, dl.directive));
    std::string value(cstr(s, dl.value));

    // Common tail of several cases.
    auto unknown = [&] { as_.nonfatal("unrecognized directive [{}]", directive); };

    switch (d) {
    case D_none:
        return false;

    case D_corrupt:
        as_.nonfatal("invalid directive line");
        break;

    default:
        if (d > D_ofmt && d < D_pragma_tokens) {
            // It's a backend-specific directive.
            switch (host_.ofmt->directive(d, std::string_view(value))) {
            case DIRR_UNKNOWN: unknown(); break;
            case DIRR_OK:
            case DIRR_ERROR: break;
            case DIRR_BADPARAM: bad_param = true; break;
            default: as_.panic_here();
            }
        } else if (d < D_pseudo_ops) {
            as_.nonfatal("internal error: unimplemented directive [{}]", directive);
        } else {
            unknown();
        }
        break;

    case D_unknown:
        unknown();
        break;

    case D_SEGMENT:   // [SEGMENT n]
    case D_SECTION: {
        int sb = as_.globl.bits;
        const std::int32_t seg = host_.ofmt->section(std::string_view(value), sb);

        if (seg == NO_SEG) {
            as_.nonfatal("segment name `{}' not recognized", value);
        } else {
            as_.globl.bits = sb;
            as_.switch_segment(seg);
        }
        break;
    }

    case D_SECTALIGN:   // [SECTALIGN n]
        if (!value.empty()) {
            const std::optional<Expr> e = host_.evaluate_text(value, 1);
            if (e) {
                const std::uint64_t align = static_cast<std::uint64_t>(eu::value_at(*e, 0));

                if (!(align && (align & (align - 1)) == 0)) {
                    as_.nonfatal("segment alignment `{}' is not power of two", value);
                } else if (align > std::uint64_t{0x7fffffff}) {
                    // FIXME (NASM): please make some sane message here; ofmt should have some 'check' method which would
                    // report segment alignment bounds.
                    unsigned lg = 0;
                    for (std::uint64_t v = align; v > 1; v >>= 1) ++lg;
                    as_.nonfatal("absurdly large segment alignment `{}' (2^{})", value, lg);
                }

                // callee should be able to handle all details
                if (as_.location.segment != NO_SEG) host_.ofmt->sectalign(as_.location.segment, static_cast<unsigned>(align));
            }
        }
        break;

    case D_BITS:   // [BITS bits]
        as_.globl.bits = as_.get_bits(value);
        break;

    case D_GLOBAL:   // [GLOBAL|STATIC|EXTERN|COMMON symbol:special]
    case D_STATIC:
    case D_EXTERN:
    case D_REQUIRED:
    case D_COMMON: {
        LabelType type = LBL_GLOBAL;
        switch (d) {
        case D_STATIC: type = LBL_STATIC; break;
        case D_EXTERN: type = LBL_EXTERN; break;
        case D_REQUIRED: type = LBL_REQUIRED; break;
        case D_COMMON: type = LBL_COMMON; break;
        default: break;
        }

        // Work on the text as NASM does: `v` is the C string, edited in place with NULs.
        std::string v = value;
        std::size_t vi = 0;
        bool validid;
        std::int64_t size = 0;
        const std::string fullvalue = value;   // including leading $ if present

        if (at(v, vi) == '$') {
            ++vi;   // skip escaping $ if present
            validid = nasm_isidchar(at(v, vi));
            if (as_.globl.dollarhex) validid = validid && !nasm_isnumstart(at(v, vi));
        } else {
            validid = nasm_isidstart(at(v, vi));
        }

        std::size_t q = vi;
        if (validid) {
            ++q;
            while (at(v, q) && at(v, q) != ':' && !nasm_isspace(at(v, q))) {
                if (!nasm_isidchar(at(v, q))) validid = false;
                ++q;
            }
        }
        if (!validid) {
            as_.nonfatal("identifier expected after {}, got `{}'", directive, fullvalue);
            break;
        }

        std::optional<std::size_t> sizestr;
        if (nasm_isspace(at(v, q))) {
            v[q++] = '\0';
            q = dir_skip_spaces(v, q);
            sizestr = q;
            const std::size_t colon = v.find(':', q);   // strchr(q, ':')
            q = colon == std::string::npos ? v.size() : colon;
        }

        std::optional<std::size_t> special;
        if (at(v, q) == ':') {
            v[q++] = '\0';
            special = q;
        }

        if (type == LBL_COMMON) {
            bool rn_error = false;
            if (sizestr) size = as_.hooks.readnum ? as_.hooks.readnum(cstr(v, *sizestr), rn_error) : 0;
            if (!sizestr || rn_error) as_.nonfatal("{} size specified in common declaration", sizestr ? "invalid" : "no");
        } else if (sizestr) {
            as_.nonfatal("invalid syntax in {} declaration", directive);
        }

        const std::string name(cstr(v, vi));
        std::optional<std::string_view> special_text;
        if (special) special_text = cstr(v, *special);
        if (!host_.declare_label(name, type, special_text)) break;

        if (type == LBL_COMMON || type == LBL_EXTERN || type == LBL_REQUIRED) host_.define_label(name, 0, size, false);

        break;
    }

    case D_ABSOLUTE: {   // [ABSOLUTE address]
        const std::optional<Expr> e = host_.evaluate_text(value, 1);
        if (e) {
            if (!eu::is_reloc(*e, as_.location.segment)) {
                as_.nonfatal("cannot use non-relocatable expression as ABSOLUTE address");
            } else {
                as_.absolute.segment = eu::reloc_seg(*e);
                as_.absolute.offset = eu::reloc_value(*e);
            }
        } else if (as_.pass_first()) {
            as_.absolute.offset = 0x100;   // don't go near zero in case of /
        } else {
            as_.nonfatal("invalid ABSOLUTE address");
        }
        as_.in_absolute = true;
        as_.location.segment = NO_SEG;
        as_.location.offset = as_.absolute.offset;
        break;
    }

    case D_DEBUG: {   // [DEBUG]
        bool badid, overlong;
        std::string debugid;
        std::size_t p = 0;
        badid = overlong = false;
        if (at(value, p) == '$') {
            // Skip $ used to escape an identifier.
            ++p;
            badid = !nasm_isidchar(at(value, p));
            if (as_.globl.dollarhex) badid = badid || nasm_isnumchar(at(value, p));
        } else {
            badid = !nasm_isidstart(at(value, p));
        }

        if (!badid) {
            while (at(value, p) && !nasm_isspace(at(value, p))) {
                if (debugid.size() >= 127) {   // sizeof debugid - 1
                    overlong = true;
                    break;
                }
                if (!nasm_isidchar(at(value, p))) badid = true;
                debugid += at(value, p++);
            }
        }
        if (badid) {
            as_.nonfatal("identifier expected after DEBUG");
            break;
        }
        if (overlong) {
            as_.nonfatal("DEBUG identifier too long");
            break;
        }
        p = dir_skip_spaces(value, p);
        if (as_.pass_final() && host_.dfmt) host_.dfmt->debug_directive(debugid, std::string_view(value).substr(p));
        break;
    }

    case D_WARNING: {   // [WARNING {push|pop|{+|-|*}warn-name}]
        std::string_view v = std::string_view(value).substr(dir_skip_spaces(value, 0));
        if (!v.empty() && (v[0] | 0x20) == 'p') {
            if (dir_ieq(v, "push"))
                as_.push_warnings();
            else if (dir_ieq(v, "pop"))
                as_.pop_warnings();
        } else {
            as_.set_warning_status(v);
        }
        break;
    }

    case D_CPU:   // [CPU]
        as_.set_cpu(std::string_view(value));
        break;

    case D_LIST: {   // [LIST {+|-}]
        const std::string_view v = std::string_view(value).substr(dir_skip_spaces(value, 0));
        if (!v.empty() && v[0] == '+') {
            if (host_.lfmt) host_.lfmt->user_nolist = false;
        } else if (!v.empty() && v[0] == '-') {
            if (host_.lfmt) host_.lfmt->user_nolist = !host_.lfmt->option('F');
        } else {
            bad_param = true;
        }
        break;
    }

    case D_DEFAULT: {   // [DEFAULT]
        int relabs_applies = EAF_NOTFSGS;
        bool eat_colon = false;
        Scanner& scan = *host_.scanner;
        TokenVal tokval;

        scan.reset(value);
        tokval.type = TokenType::INVALID;
        while (!bad_param) {
            const int type = scan.scan(tokval);
            if (type <= 0) break;

            const int tt = token_value(tokval.type);
            if (tt == token_value(TokenType::REG) || tt == token_value(TokenType::SPECIAL) || tt == token_value(TokenType::PREFIX)) {
                switch (tokval.integer) {
                case R_FS:
                    relabs_applies = EAF_FS;
                    eat_colon = true;
                    continue;   // next_token
                case R_GS:
                    relabs_applies = EAF_GS;
                    eat_colon = true;
                    continue;
                case S_REL:
                    as_.globl.rel |= relabs_applies;
                    as_.globl.reldef |= relabs_applies;
                    break;
                case S_ABS:
                    as_.globl.rel &= ~relabs_applies;
                    as_.globl.reldef |= relabs_applies;
                    break;
                case P_BND: as_.globl.bnd = true; break;
                case P_NOBND: as_.globl.bnd = false; break;
                default: bad_param = true; break;
                }
            } else if (tt == ',') {
                // nothing
            } else if (tt == ':' && eat_colon) {
                eat_colon = false;
                continue;   // next_token
            } else {
                bad_param = true;
            }

            eat_colon = false;
            relabs_applies = EAF_NOTFSGS;
        }
        break;
    }

    case D_FLOAT:
        if (host_.floats->float_option(value)) as_.nonfatal("unknown 'float' directive: {}", value);
        break;

    case D_DOLLARHEX:
        host_.get_boolean_option(value, as_.globl.dollarhex);
        break;

    case D_PRAGMA:
        process_pragma(value);
        break;

    case D_PREFIX:
    case D_GPREFIX:
    case D_SUFFIX:
    case D_GSUFFIX:
    case D_POSTFIX:
    case D_GPOSTFIX:
    case D_LPREFIX:
    case D_LSUFFIX:
    case D_LPOSTFIX:
        host_.set_label_mangle(d, value);
        break;
    }

    // A common error message.
    if (bad_param) as_.nonfatal("invalid parameter to [{}] directive", directive);

    return true;
}

// ---- pragma.c ----

namespace drvns_directives {

// Invoke a pragma handler.
DirectiveResult call_pragma(const PragmaFacility* pf, Pragma& pragma) {
    if (!pf || !pf->handler) return DIRR_UNKNOWN;

    pragma.facility = pf;
    return pf->handler(pragma);
}

}  // namespace drvns_directives
using namespace drvns_directives;

// Search a pragma list for a known pragma facility and if so, invoke the handler. Return true if processing is complete.
// The "default name", *or def->name*, if set, matches the final NULL entry (used for backends, so multiple backends can
// share the same list under some circumstances, and the backends can implement common operations.) A list ends at its first
// entry with an empty name; that entry's handler, if it has one, is the handler of the "NULL entry".
DirectiveResult Directives::search_pragma_list(const std::vector<PragmaFacility>* list, std::string_view defaultname,
                                               const PragmaFacility* def, const Pragma& cpragma) {
    const PragmaFacility* pf = nullptr;
    DirectiveResult rv = DIRR_UNKNOWN;
    bool facility_match, is_default;
    Pragma pragma = cpragma;
    std::string facname = pragma.facility_name;

    // Is there a default facility and we match its name?
    is_default = def && !def->name.empty() && dir_ieq(facname, def->name);
    facility_match = is_default;

    // Promote def->name to defaultname if both are set. This handles e.g. output -> elf32 so that we can handle
    // elf32-specific directives in that handler.
    if (!defaultname.empty()) {
        if (is_default)
            facname = std::string(defaultname);
        else
            facility_match = dir_ieq(facname, defaultname);
    }

    bool found = false;
    if (!facname.empty() && list) {
        std::size_t k = 0;
        for (; k < list->size() && !(*list)[k].name.empty(); ++k) {
            pf = &(*list)[k];
            if (dir_ieq(facname, pf->name)) {
                facility_match = true;
                rv = call_pragma(pf, pragma);
                if (rv != DIRR_UNKNOWN) {
                    found = true;
                    break;
                }
            }
        }

        if (!found && facility_match) {
            // Facility name match but no matching directive; handler in NULL entry at end of list?
            pf = k < list->size() ? &(*list)[k] : nullptr;
            rv = call_pragma(pf, pragma);
            if (rv != DIRR_UNKNOWN) found = true;
        }
    }

    if (!found) {
        if (facility_match) {
            // Facility match but still nothing: def->handler if it exists.
            rv = call_pragma(def, pragma);
        } else {
            // No facility matched.
            return DIRR_UNKNOWN;
        }
    }

    // Otherwise we found the facility but not any supported directive, fall through...
    switch (rv) {
    case DIRR_UNKNOWN:
        switch (pragma.opcode) {
        case D_none: as_.warn(ERR_PASS2 | WARN_PRAGMA_BAD, "empty %pragma {}", pragma.facility_name); break;
        default: as_.warn(ERR_PASS2 | WARN_PRAGMA_UNKNOWN, "unknown %pragma {} {}", pragma.facility_name, pragma.opname); break;
        }
        rv = DIRR_ERROR;   // already printed an error message
        break;

    case DIRR_OK:
    case DIRR_ERROR:
        break;   // nothing to do

    case DIRR_BADPARAM:
        // This one is an error. Don't use it if forward compatibility would be compromised, as opposed to an inherent
        // error.
        as_.error(ERR_NONFATAL, "bad argument to %pragma {} {}", pragma.facility_name, pragma.opname);
        break;

    default:
        as_.panic_here();
    }
    return rv;
}

// Naked %pragma.
void Directives::process_pragma(std::string_view str) {
    Pragma pragma;

    // pragma.facility_name = nasm_get_word(str, &p)
    std::size_t p = 0;
    while (p < str.size() && nasm_isspace(str[p])) ++p;
    std::size_t next = p;
    while (next < str.size() && !nasm_isspace(str[next])) ++next;
    if (p >= str.size()) {
        // Empty %pragma.
        as_.warn(ERR_PASS2 | WARN_PRAGMA_EMPTY, "empty %pragma directive, ignored");
        return;
    }
    pragma.facility_name = std::string(str.substr(p, next - p));
    p = next < str.size() ? next + 1 : next;

    // pragma.opname = nasm_get_word(p, &p)
    std::size_t w = p;
    while (w < str.size() && nasm_isspace(str[w])) ++w;
    std::size_t wend = w;
    while (wend < str.size() && !nasm_isspace(str[wend])) ++wend;
    if (w >= str.size()) {
        pragma.opcode = D_none;
        p = str.size();   // (NASM's tail is NULL here and the next line would not survive it)
    } else {
        pragma.opname = std::string(str.substr(w, wend - w));
        pragma.opcode = directive_find(pragma.opname);
        p = wend < str.size() ? wend + 1 : wend;
    }

    // This used to use nasm_trim_spaces, but that assumes a single word. Instead, strip spaces at either end of the
    // directive, but allow interior spaces.
    while (p < str.size() && nasm_isspace(str[p])) ++p;
    std::size_t tend = str.size();
    // (a NUL ends the C string)
    const std::size_t nul = str.find('\0', p);
    if (nul != std::string_view::npos) tend = nul;
    while (tend > p && nasm_isspace(str[tend - 1])) --tend;
    pragma.tail = std::string(str.substr(p, tend - p));

    // Search the global pragma namespaces. This is done as a loop rather than letting search_pragma_list() just run, because
    // we don't want to keep searching if we have a facility match, thus we want to call search_pragma_list() individually for
    // each namespace.
    const auto output_common = [this](const Pragma& pr) -> DirectiveResult {
        // Generic pragmas that apply to all output backends.
        switch (pr.opcode) {
        case D_PREFIX:
        case D_GPREFIX:
        case D_SUFFIX:
        case D_GSUFFIX:
        case D_POSTFIX:
        case D_GPOSTFIX:
        case D_LPREFIX:
        case D_LSUFFIX:
        case D_LPOSTFIX:
            host_.set_label_mangle(pr.opcode, pr.tail);
            return DIRR_OK;
        default:
            return DIRR_UNKNOWN;
        }
    };
    const PragmaFacility output_pragma_def{"output", output_common};
    const PragmaFacility debug_pragma_def{"debug", nullptr};
    const auto output_pragma = [&](const Pragma& pr) {
        return search_pragma_list(&host_.ofmt->pragmas(), host_.ofmt->shortname(), &output_pragma_def, pr);
    };
    const auto debug_pragma = [&](const Pragma& pr) {
        const std::vector<PragmaFacility>* dl = host_.dfmt ? &host_.dfmt->pragmas() : nullptr;
        return search_pragma_list(dl, host_.dfmt ? host_.dfmt->shortname() : std::string_view(), &debug_pragma_def, pr);
    };

    const std::vector<PragmaFacility> global_pragmas = {
        {"asm", nullptr},
        {"limit", [this](const Pragma& pr) { return as_.set_limit(pr.opname, pr.tail); }},
        {"list", [this](const Pragma& pr) { return host_.lfmt ? host_.lfmt->pragma(pr) : DIRR_UNKNOWN; }},
        {"file", nullptr},
        {"input", nullptr},
        {"output", output_pragma},
        {"debug", debug_pragma},
        {"ignore", [](const Pragma&) { return DIRR_OK; }},   // even for D_none!
        {"preproc", nullptr},   // handled in the preprocessor by necessity
    };
    for (const PragmaFacility& pf : global_pragmas)
        if (search_pragma_list(nullptr, {}, &pf, pragma) != DIRR_UNKNOWN) return;

    // Is it an output pragma?
    if (output_pragma(pragma) != DIRR_UNKNOWN) return;

    // Is it a debug pragma?
    if (debug_pragma(pragma) != DIRR_UNKNOWN) return;

    // Note: it would be nice to warn for an unknown namespace, but in order to do so we need to walk *ALL* the backends in
    // order to make sure we aren't dealing with a pragma that is for another backend. On the other hand, that could also be a
    // warning with a separate warning flag.
}

}  // namespace cb::build::x86asm
