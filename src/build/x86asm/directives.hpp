// build/x86asm/directives.hpp -- assembler directives and pragmas: the directive names, the directive line handler
// (NASM's directiv.c), the pragma dispatcher (pragma.c) and the segment-number allocator (segalloc.c).
//
// The names and the numbering of `Directive` are NASM's (asm/directiv.dat, as directiv.h generated them), because the
// output formats and the preprocessor compare against them.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/types.hpp"

namespace cb::build::x86asm {

// The directive and pragma-operation names (enum directive).
enum Directive : int {
    D_none,
    D_unknown,
    D_corrupt,
    D_ABSOLUTE,
    D_BITS,
    D_COMMON,
    D_CPU,
    D_DEBUG,
    D_DEFAULT,
    D_DOLLARHEX,
    D_EXTERN,
    D_FLOAT,
    D_GLOBAL,
    D_LIST,
    D_PRAGMA,
    D_REQUIRED,
    D_SECTALIGN,
    D_SECTION,
    D_SEGMENT,
    D_STATIC,
    D_WARNING,
    D_PREFIX,
    D_SUFFIX,
    D_POSTFIX,
    D_GPREFIX,
    D_GSUFFIX,
    D_GPOSTFIX,
    D_LPREFIX,
    D_LSUFFIX,
    D_LPOSTFIX,
    D_pseudo_ops,
    D_DB,
    D_DW,
    D_DD,
    D_DQ,
    D_DT,
    D_DO,
    D_DY,
    D_DZ,
    D_RESB,
    D_RESW,
    D_RESD,
    D_RESQ,
    D_REST,
    D_RESO,
    D_RESY,
    D_RESZ,
    D_INCBIN,
    D_EQU,
    D_ofmt,
    D_EXPORT,
    D_GROUP,
    D_IMPORT,
    D_LIBRARY,
    D_MAP,
    D_MODULE,
    D_ORG,
    D_OSABI,
    D_SAFESEH,
    D_UPPERCASE,
    D_pragma_tokens,
    D_LIMIT,
    D_OPTIONS,
    D_SUBSECTIONS_VIA_SYMBOLS,
    D_NO_DEAD_STRIP,
    D_MAXDUMP,
    D_NODEPEND,
    D_NOSECLABELS
};

// The directive a name is (case-blind), D_unknown when it is none, D_none for an empty name (directive_find).
Directive directive_find(std::string_view name);
// The lower-case name of a directive, "" for the special values (directive_name).
const char* directive_name(Directive d);

// One %pragma, split into its words (struct pragma). `tail` is what follows the operation, trimmed.
struct PragmaFacility;
struct Pragma {
    const PragmaFacility* facility = nullptr;   // the facility the handler belongs to
    std::string facility_name;                  // the first word
    std::string opname;                         // the second word, "" if none
    Directive opcode = D_none;                  // the second word as a directive
    std::string tail;                           // the rest of the line
};

// A pragma facility and its handler (struct pragma_facility): an output or debug format lists its own.
struct PragmaFacility {
    std::string name;
    std::function<DirectiveResult(const Pragma&)> handler;   // empty: none
};

// ---- the directive layer (directiv.c, pragma.c) ----

class OutputFormat;
class DebugFormat;
class Listing;
class Scanner;
class Floats;

// What the directive layer asks of the modules around it. Every callback is the other module's function of the same name in
// NASM; `evaluate_text` is `stdscan_reset(text); tokval.t_type = TOKEN_INVALID; evaluate(stdscan, NULL, &tokval, NULL,
// critical, NULL)`.
struct DirectiveHost {
    OutputFormat* ofmt = nullptr;
    DebugFormat* dfmt = nullptr;
    Listing* lfmt = nullptr;
    Scanner* scanner = nullptr;   // for [DEFAULT]
    Floats* floats = nullptr;     // for [FLOAT]
    std::function<std::optional<Expr>(std::string_view text, int critical)> evaluate_text;
    std::function<bool(std::string_view name, LabelType type, std::optional<std::string_view> special)> declare_label;
    std::function<void(std::string_view name, std::int32_t segment, std::int64_t offset, bool is_norm)> define_label;
    std::function<void(Directive which, std::string_view value)> set_label_mangle;
    // get_boolean_option: read an on/off/yes/no/true/false word or a constant expression from the front of `text` into
    // `value` (empty text is true; an error leaves `value` as it was).
    std::function<void(std::string_view text, bool& value)> get_boolean_option;
};

// The directive and pragma handlers of one run.
class Directives {
public:
    Directives(Assembly& assembly, DirectiveHost host) : as_(assembly), host_(std::move(host)) {}

    // Does a string name a valid directive (sans [], white space already trimmed)? (directive_valid)
    bool directive_valid(std::string_view directive);

    // Process a line from the assembler and try to handle it if it is a directive. Returns true if the line was handled
    // (including if it was an error), false otherwise. (process_directives)
    bool process_directives(std::string_view line);

    // A naked %pragma (process_pragma).
    void process_pragma(std::string_view str);

private:
    DirectiveResult search_pragma_list(const std::vector<PragmaFacility>* list, std::string_view defaultname, const PragmaFacility* def,
                                       const Pragma& cpragma);

    Assembly& as_;
    DirectiveHost host_;
};

// ---- segment numbers (segalloc.c) ----

// Hands out segment numbers: even values from 2 (the odd one of each pair is the segment base). One per run.
class SegmentAllocator {
public:
    std::int32_t alloc() {
        const std::int32_t this_seg = next_;
        next_ += 2;
        return this_seg;
    }

private:
    std::int32_t next_ = 2;
};

}  // namespace cb::build::x86asm
