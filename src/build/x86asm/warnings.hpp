// Generated from native/nasm/asm/warnings.dat (warnings.h, warnings_c.h): NASM's warning classes. Do not edit by hand.
// Regenerate from warnings.dat. The severity word's low 16 bits are the error severity and flags (error.h);
// the high 16 bits are the warning class.
#pragma once

#include <array>
#include <cstdint>

namespace cb::build::x86asm {

// How far the warning class is shifted left in an error's severity word.
inline constexpr unsigned WARN_SHR = 16;

// The state of a warning class is a bit mask: enabled, and treated as an error.
inline constexpr std::uint8_t WARN_ST_ENABLED = 1;   // warning is currently enabled
inline constexpr std::uint8_t WARN_ST_ERROR = 2;     // treat this warning as an error

// The state a class starts in.
inline constexpr std::uint8_t WARN_INIT_OFF = 0;
inline constexpr std::uint8_t WARN_INIT_ON = WARN_ST_ENABLED;
inline constexpr std::uint8_t WARN_INIT_ERR = WARN_ST_ENABLED | WARN_ST_ERROR;

// The index of each warning class (WARN_IDX_*). Index 0 is "not suppressible".
enum WarnIndex : unsigned {
    WARN_IDX_NONE = 0,   /* not suppressible */
    WARN_IDX_DB_EMPTY = 1,   /* no operand for data declaration */
    WARN_IDX_DIRECTIVE_GARBAGE_EOL = 2,   /* garbage after directive */
    WARN_IDX_EA_ABSOLUTE = 3,   /* absolute address cannot be RIP-relative */
    WARN_IDX_EA_DISPSIZE = 4,   /* displacement size ignored on absolute address */
    WARN_IDX_FLOAT_DENORM = 5,   /* floating point denormal */
    WARN_IDX_FLOAT_OVERFLOW = 6,   /* floating point overflow */
    WARN_IDX_FLOAT_TOOLONG = 7,   /* too many digits in floating-point number */
    WARN_IDX_FLOAT_UNDERFLOW = 8,   /* floating point underflow */
    WARN_IDX_FORWARD = 9,   /* forward reference may have unpredictable results */
    WARN_IDX_IMPLICIT_ABS_DEPRECATED = 10,   /* implicit DEFAULT ABS is deprecated */
    WARN_IDX_LABEL_ORPHAN = 11,   /* labels alone on lines without trailing : */
    WARN_IDX_LABEL_REDEF = 12,   /* label redefined to an identical value */
    WARN_IDX_LABEL_REDEF_LATE = 13,   /* label (re)defined during code generation */
    WARN_IDX_NUMBER_DEPRECATED_HEX = 14,   /* $ prefix for hexadecimal is deprecated */
    WARN_IDX_NUMBER_OVERFLOW = 15,   /* numeric constant does not fit */
    WARN_IDX_OBSOLETE_NOP = 16,   /* instruction obsolete and is a noop on the target CPU */
    WARN_IDX_OBSOLETE_REMOVED = 17,   /* instruction obsolete and removed on the target CPU */
    WARN_IDX_OBSOLETE_VALID = 18,   /* instruction obsolete but valid on the target CPU */
    WARN_IDX_PHASE = 19,   /* phase error during stabilization */
    WARN_IDX_PP_ELSE_ELIF = 20,   /* %elif after %else */
    WARN_IDX_PP_ELSE_ELSE = 21,   /* %else after %else */
    WARN_IDX_PP_EMPTY_BRACES = 22,   /* empty %{} construct */
    WARN_IDX_PP_ENVIRONMENT = 23,   /* nonexistent environment variable */
    WARN_IDX_PP_MACRO_DEF_CASE_SINGLE = 24,   /* single-line macro defined both case sensitive and insensitive */
    WARN_IDX_PP_MACRO_DEF_GREEDY_SINGLE = 25,   /* single-line macro */
    WARN_IDX_PP_MACRO_DEF_PARAM_SINGLE = 26,   /* single-line macro defined with and without parameters */
    WARN_IDX_PP_MACRO_DEFAULTS = 27,   /* macros with more default than optional parameters */
    WARN_IDX_PP_MACRO_PARAMS_LEGACY = 28,   /* improperly calling multi-line macro for legacy support */
    WARN_IDX_PP_MACRO_PARAMS_MULTI = 29,   /* multi-line macro calls with wrong parameter count */
    WARN_IDX_PP_MACRO_PARAMS_SINGLE = 30,   /* single-line macro calls with wrong parameter count */
    WARN_IDX_PP_MACRO_REDEF_MULTI = 31,   /* redefining multi-line macro */
    WARN_IDX_PP_OPEN_BRACES = 32,   /* unterminated %{...} */
    WARN_IDX_PP_OPEN_BRACKETS = 33,   /* unterminated %[...] */
    WARN_IDX_PP_OPEN_STRING = 34,   /* unterminated string */
    WARN_IDX_PP_REP_NEGATIVE = 35,   /* negative %rep count */
    WARN_IDX_PP_RESERVED = 36,   /* reserved unimplemented preprocessor directive */
    WARN_IDX_PP_SEL_RANGE = 37,   /* %sel() argument out of range */
    WARN_IDX_PP_TRAILING = 38,   /* trailing garbage ignored */
    WARN_IDX_PRAGMA_BAD = 39,   /* malformed %pragma */
    WARN_IDX_PRAGMA_EMPTY = 40,   /* empty %pragma directive */
    WARN_IDX_PRAGMA_NA = 41,   /* %pragma not applicable to this compilation */
    WARN_IDX_PRAGMA_UNKNOWN = 42,   /* unknown %pragma facility or directive */
    WARN_IDX_PREFIX_BADMODE_O64 = 43,   /* o64 prefix invalid in 16/32-bit mode */
    WARN_IDX_PREFIX_BND = 44,   /* invalid BND prefix */
    WARN_IDX_PREFIX_HINT_DROPPED = 45,   /* invalid branch hint prefix dropped */
    WARN_IDX_PREFIX_HLE = 46,   /* invalid HLE prefix */
    WARN_IDX_PREFIX_INVALID = 47,   /* invalid prefix for instruction */
    WARN_IDX_PREFIX_LOCK_ERROR = 48,   /* LOCK prefix on unlockable instruction */
    WARN_IDX_PREFIX_LOCK_XCHG = 49,   /* superfluous LOCK prefix on XCHG instruction */
    WARN_IDX_PREFIX_OPSIZE = 50,   /* invalid operand size prefix */
    WARN_IDX_PREFIX_SEG = 51,   /* segment prefix ignored in 64-bit mode */
    WARN_IDX_PTR = 52,   /* non-NASM keyword used in other assemblers */
    WARN_IDX_REGSIZE = 53,   /* register size specification ignored */
    WARN_IDX_RELOC_ABS_BYTE = 54,   /* 8-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_ABS_DWORD = 55,   /* 32-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_ABS_QWORD = 56,   /* 64-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_ABS_WORD = 57,   /* 16-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_REL_BYTE = 58,   /* 8-bit relative section-crossing relocation */
    WARN_IDX_RELOC_REL_DWORD = 59,   /* 32-bit relative section-crossing relocation */
    WARN_IDX_RELOC_REL_QWORD = 60,   /* 64-bit relative section-crossing relocation */
    WARN_IDX_RELOC_REL_WORD = 61,   /* 16-bit relative section-crossing relocation */
    WARN_IDX_SECTION_ALIGNMENT_ROUNDED = 62,   /* section alignment rounded up */
    WARN_IDX_UNKNOWN_WARNING = 63,   /* unknown warning in -W/-w or warning directive */
    WARN_IDX_USER = 64,   /* %warning directives */
    WARN_IDX_WARN_STACK_EMPTY = 65,   /* warning stack empty */
    WARN_IDX_ZEROING = 66,   /* RESx in initialized section becomes zero */
    WARN_IDX_ZEXT_RELOC = 67,   /* relocation zero-extended to match output format */
    WARN_IDX_OTHER = 68,   /* any warning not assigned to a specific warning class */
    WARN_IDX_ALL = 69   /* all possible warnings */
};

// How many warning classes there are (the size of the state table; WARN_IDX_OTHER is the last).
inline constexpr unsigned NUM_WARNINGS = 69;

// The command-line/directive names of the classes, indexed by WarnIndex (entry 0 has none).
inline constexpr std::array<const char*, 70> kWarningName = {{
    nullptr,
    "db-empty",
    "directive-garbage-eol",
    "ea-absolute",
    "ea-dispsize",
    "float-denorm",
    "float-overflow",
    "float-toolong",
    "float-underflow",
    "forward",
    "implicit-abs-deprecated",
    "label-orphan",
    "label-redef",
    "label-redef-late",
    "number-deprecated-hex",
    "number-overflow",
    "obsolete-nop",
    "obsolete-removed",
    "obsolete-valid",
    "phase",
    "pp-else-elif",
    "pp-else-else",
    "pp-empty-braces",
    "pp-environment",
    "pp-macro-def-case-single",
    "pp-macro-def-greedy-single",
    "pp-macro-def-param-single",
    "pp-macro-defaults",
    "pp-macro-params-legacy",
    "pp-macro-params-multi",
    "pp-macro-params-single",
    "pp-macro-redef-multi",
    "pp-open-braces",
    "pp-open-brackets",
    "pp-open-string",
    "pp-rep-negative",
    "pp-reserved",
    "pp-sel-range",
    "pp-trailing",
    "pragma-bad",
    "pragma-empty",
    "pragma-na",
    "pragma-unknown",
    "prefix-badmode-o64",
    "prefix-bnd",
    "prefix-hint-dropped",
    "prefix-hle",
    "prefix-invalid",
    "prefix-lock-error",
    "prefix-lock-xchg",
    "prefix-opsize",
    "prefix-seg",
    "ptr",
    "regsize",
    "reloc-abs-byte",
    "reloc-abs-dword",
    "reloc-abs-qword",
    "reloc-abs-word",
    "reloc-rel-byte",
    "reloc-rel-dword",
    "reloc-rel-qword",
    "reloc-rel-word",
    "section-alignment-rounded",
    "unknown-warning",
    "user",
    "warn-stack-empty",
    "zeroing",
    "zext-reloc",
    "other",
    "all"
}};

// One line of help for each class, indexed by WarnIndex.
inline constexpr std::array<const char*, 70> kWarningHelp = {{
    nullptr,
    "no operand for data declaration",
    "garbage after directive",
    "absolute address cannot be RIP-relative",
    "displacement size ignored on absolute address",
    "floating point denormal",
    "floating point overflow",
    "too many digits in floating-point number",
    "floating point underflow",
    "forward reference may have unpredictable results",
    "implicit DEFAULT ABS is deprecated",
    "labels alone on lines without trailing :",
    "label redefined to an identical value",
    "label (re)defined during code generation",
    "$ prefix for hexadecimal is deprecated",
    "numeric constant does not fit",
    "instruction obsolete and is a noop on the target CPU",
    "instruction obsolete and removed on the target CPU",
    "instruction obsolete but valid on the target CPU",
    "phase error during stabilization",
    "%elif after %else",
    "%else after %else",
    "empty %{} construct",
    "nonexistent environment variable",
    "single-line macro defined both case sensitive and insensitive",
    "single-line macro",
    "single-line macro defined with and without parameters",
    "macros with more default than optional parameters",
    "improperly calling multi-line macro for legacy support",
    "multi-line macro calls with wrong parameter count",
    "single-line macro calls with wrong parameter count",
    "redefining multi-line macro",
    "unterminated %{...}",
    "unterminated %[...]",
    "unterminated string",
    "negative %rep count",
    "reserved unimplemented preprocessor directive",
    "%sel() argument out of range",
    "trailing garbage ignored",
    "malformed %pragma",
    "empty %pragma directive",
    "%pragma not applicable to this compilation",
    "unknown %pragma facility or directive",
    "o64 prefix invalid in 16/32-bit mode",
    "invalid BND prefix",
    "invalid branch hint prefix dropped",
    "invalid HLE prefix",
    "invalid prefix for instruction",
    "LOCK prefix on unlockable instruction",
    "superfluous LOCK prefix on XCHG instruction",
    "invalid operand size prefix",
    "segment prefix ignored in 64-bit mode",
    "non-NASM keyword used in other assemblers",
    "register size specification ignored",
    "8-bit absolute section-crossing relocation",
    "32-bit absolute section-crossing relocation",
    "64-bit absolute section-crossing relocation",
    "16-bit absolute section-crossing relocation",
    "8-bit relative section-crossing relocation",
    "32-bit relative section-crossing relocation",
    "64-bit relative section-crossing relocation",
    "16-bit relative section-crossing relocation",
    "section alignment rounded up",
    "unknown warning in -W/-w or warning directive",
    "%warning directives",
    "warning stack empty",
    "RESx in initialized section becomes zero",
    "relocation zero-extended to match output format",
    "any warning not assigned to a specific warning class",
    "all possible warnings"
}};

// Every name a class answers to (the class names, and aliases such as orphan-labels); "all" is first.
struct WarningAlias {
    const char* name;
    WarnIndex warning;
};
inline constexpr unsigned NUM_WARNING_ALIAS = 85;
inline constexpr std::array<WarningAlias, NUM_WARNING_ALIAS> kWarningAlias = {{
    {"all", WARN_IDX_ALL},
    {"bad-pragma", WARN_IDX_PRAGMA_BAD},
    {"bnd", WARN_IDX_PREFIX_BND},
    {"db-empty", WARN_IDX_DB_EMPTY},
    {"directive-garbage-eol", WARN_IDX_DIRECTIVE_GARBAGE_EOL},
    {"ea-absolute", WARN_IDX_EA_ABSOLUTE},
    {"ea-dispsize", WARN_IDX_EA_DISPSIZE},
    {"environment", WARN_IDX_PP_ENVIRONMENT},
    {"float-denorm", WARN_IDX_FLOAT_DENORM},
    {"float-overflow", WARN_IDX_FLOAT_OVERFLOW},
    {"float-toolong", WARN_IDX_FLOAT_TOOLONG},
    {"float-underflow", WARN_IDX_FLOAT_UNDERFLOW},
    {"forward", WARN_IDX_FORWARD},
    {"hle", WARN_IDX_PREFIX_HLE},
    {"implicit-abs-deprecated", WARN_IDX_IMPLICIT_ABS_DEPRECATED},
    {"label-orphan", WARN_IDX_LABEL_ORPHAN},
    {"label-redef", WARN_IDX_LABEL_REDEF},
    {"label-redef-late", WARN_IDX_LABEL_REDEF_LATE},
    {"lock", WARN_IDX_PREFIX_LOCK_ERROR},
    {"macro-def-case-single", WARN_IDX_PP_MACRO_DEF_CASE_SINGLE},
    {"macro-def-greedy-single", WARN_IDX_PP_MACRO_DEF_GREEDY_SINGLE},
    {"macro-def-param-single", WARN_IDX_PP_MACRO_DEF_PARAM_SINGLE},
    {"macro-defaults", WARN_IDX_PP_MACRO_DEFAULTS},
    {"macro-params-legacy", WARN_IDX_PP_MACRO_PARAMS_LEGACY},
    {"macro-params-multi", WARN_IDX_PP_MACRO_PARAMS_MULTI},
    {"macro-params-single", WARN_IDX_PP_MACRO_PARAMS_SINGLE},
    {"negative-rep", WARN_IDX_PP_REP_NEGATIVE},
    {"not-my-pragma", WARN_IDX_PRAGMA_NA},
    {"number-deprecated-hex", WARN_IDX_NUMBER_DEPRECATED_HEX},
    {"number-overflow", WARN_IDX_NUMBER_OVERFLOW},
    {"obsolete-nop", WARN_IDX_OBSOLETE_NOP},
    {"obsolete-removed", WARN_IDX_OBSOLETE_REMOVED},
    {"obsolete-valid", WARN_IDX_OBSOLETE_VALID},
    {"orphan-labels", WARN_IDX_LABEL_ORPHAN},
    {"other", WARN_IDX_OTHER},
    {"phase", WARN_IDX_PHASE},
    {"pp-else-elif", WARN_IDX_PP_ELSE_ELIF},
    {"pp-else-else", WARN_IDX_PP_ELSE_ELSE},
    {"pp-empty-braces", WARN_IDX_PP_EMPTY_BRACES},
    {"pp-environment", WARN_IDX_PP_ENVIRONMENT},
    {"pp-macro-def-case-single", WARN_IDX_PP_MACRO_DEF_CASE_SINGLE},
    {"pp-macro-def-greedy-single", WARN_IDX_PP_MACRO_DEF_GREEDY_SINGLE},
    {"pp-macro-def-param-single", WARN_IDX_PP_MACRO_DEF_PARAM_SINGLE},
    {"pp-macro-defaults", WARN_IDX_PP_MACRO_DEFAULTS},
    {"pp-macro-params-legacy", WARN_IDX_PP_MACRO_PARAMS_LEGACY},
    {"pp-macro-params-multi", WARN_IDX_PP_MACRO_PARAMS_MULTI},
    {"pp-macro-params-single", WARN_IDX_PP_MACRO_PARAMS_SINGLE},
    {"pp-macro-redef-multi", WARN_IDX_PP_MACRO_REDEF_MULTI},
    {"pp-open-braces", WARN_IDX_PP_OPEN_BRACES},
    {"pp-open-brackets", WARN_IDX_PP_OPEN_BRACKETS},
    {"pp-open-string", WARN_IDX_PP_OPEN_STRING},
    {"pp-rep-negative", WARN_IDX_PP_REP_NEGATIVE},
    {"pp-reserved", WARN_IDX_PP_RESERVED},
    {"pp-sel-range", WARN_IDX_PP_SEL_RANGE},
    {"pp-trailing", WARN_IDX_PP_TRAILING},
    {"pragma-bad", WARN_IDX_PRAGMA_BAD},
    {"pragma-empty", WARN_IDX_PRAGMA_EMPTY},
    {"pragma-na", WARN_IDX_PRAGMA_NA},
    {"pragma-unknown", WARN_IDX_PRAGMA_UNKNOWN},
    {"prefix-badmode-o64", WARN_IDX_PREFIX_BADMODE_O64},
    {"prefix-bnd", WARN_IDX_PREFIX_BND},
    {"prefix-hint-dropped", WARN_IDX_PREFIX_HINT_DROPPED},
    {"prefix-hle", WARN_IDX_PREFIX_HLE},
    {"prefix-invalid", WARN_IDX_PREFIX_INVALID},
    {"prefix-lock-error", WARN_IDX_PREFIX_LOCK_ERROR},
    {"prefix-lock-xchg", WARN_IDX_PREFIX_LOCK_XCHG},
    {"prefix-opsize", WARN_IDX_PREFIX_OPSIZE},
    {"prefix-seg", WARN_IDX_PREFIX_SEG},
    {"ptr", WARN_IDX_PTR},
    {"regsize", WARN_IDX_REGSIZE},
    {"reloc-abs-byte", WARN_IDX_RELOC_ABS_BYTE},
    {"reloc-abs-dword", WARN_IDX_RELOC_ABS_DWORD},
    {"reloc-abs-qword", WARN_IDX_RELOC_ABS_QWORD},
    {"reloc-abs-word", WARN_IDX_RELOC_ABS_WORD},
    {"reloc-rel-byte", WARN_IDX_RELOC_REL_BYTE},
    {"reloc-rel-dword", WARN_IDX_RELOC_REL_DWORD},
    {"reloc-rel-qword", WARN_IDX_RELOC_REL_QWORD},
    {"reloc-rel-word", WARN_IDX_RELOC_REL_WORD},
    {"section-alignment-rounded", WARN_IDX_SECTION_ALIGNMENT_ROUNDED},
    {"unknown-pragma", WARN_IDX_PRAGMA_UNKNOWN},
    {"unknown-warning", WARN_IDX_UNKNOWN_WARNING},
    {"user", WARN_IDX_USER},
    {"warn-stack-empty", WARN_IDX_WARN_STACK_EMPTY},
    {"zeroing", WARN_IDX_ZEROING},
    {"zext-reloc", WARN_IDX_ZEXT_RELOC}
}};

// The state each class starts in (WARN_INIT_*: off, on, or on as an error).
inline constexpr std::array<std::uint8_t, NUM_WARNINGS> kWarningDefault = {{
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ERR,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_OFF,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_OFF,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_OFF,
    WARN_INIT_ERR,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_OFF,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ERR,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_ERR,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_OFF,
    WARN_INIT_ON,
    WARN_INIT_OFF,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON,
    WARN_INIT_ON
}};

// The same classes as flag values to OR into an error's severity word (WARN_* = index << WARN_SHR).
enum WarnConst : std::uint32_t {
    WARN_NONE = 0u << WARN_SHR,
    WARN_DB_EMPTY = 1u << WARN_SHR,
    WARN_DIRECTIVE_GARBAGE_EOL = 2u << WARN_SHR,
    WARN_EA_ABSOLUTE = 3u << WARN_SHR,
    WARN_EA_DISPSIZE = 4u << WARN_SHR,
    WARN_FLOAT_DENORM = 5u << WARN_SHR,
    WARN_FLOAT_OVERFLOW = 6u << WARN_SHR,
    WARN_FLOAT_TOOLONG = 7u << WARN_SHR,
    WARN_FLOAT_UNDERFLOW = 8u << WARN_SHR,
    WARN_FORWARD = 9u << WARN_SHR,
    WARN_IMPLICIT_ABS_DEPRECATED = 10u << WARN_SHR,
    WARN_LABEL_ORPHAN = 11u << WARN_SHR,
    WARN_LABEL_REDEF = 12u << WARN_SHR,
    WARN_LABEL_REDEF_LATE = 13u << WARN_SHR,
    WARN_NUMBER_DEPRECATED_HEX = 14u << WARN_SHR,
    WARN_NUMBER_OVERFLOW = 15u << WARN_SHR,
    WARN_OBSOLETE_NOP = 16u << WARN_SHR,
    WARN_OBSOLETE_REMOVED = 17u << WARN_SHR,
    WARN_OBSOLETE_VALID = 18u << WARN_SHR,
    WARN_PHASE = 19u << WARN_SHR,
    WARN_PP_ELSE_ELIF = 20u << WARN_SHR,
    WARN_PP_ELSE_ELSE = 21u << WARN_SHR,
    WARN_PP_EMPTY_BRACES = 22u << WARN_SHR,
    WARN_PP_ENVIRONMENT = 23u << WARN_SHR,
    WARN_PP_MACRO_DEF_CASE_SINGLE = 24u << WARN_SHR,
    WARN_PP_MACRO_DEF_GREEDY_SINGLE = 25u << WARN_SHR,
    WARN_PP_MACRO_DEF_PARAM_SINGLE = 26u << WARN_SHR,
    WARN_PP_MACRO_DEFAULTS = 27u << WARN_SHR,
    WARN_PP_MACRO_PARAMS_LEGACY = 28u << WARN_SHR,
    WARN_PP_MACRO_PARAMS_MULTI = 29u << WARN_SHR,
    WARN_PP_MACRO_PARAMS_SINGLE = 30u << WARN_SHR,
    WARN_PP_MACRO_REDEF_MULTI = 31u << WARN_SHR,
    WARN_PP_OPEN_BRACES = 32u << WARN_SHR,
    WARN_PP_OPEN_BRACKETS = 33u << WARN_SHR,
    WARN_PP_OPEN_STRING = 34u << WARN_SHR,
    WARN_PP_REP_NEGATIVE = 35u << WARN_SHR,
    WARN_PP_RESERVED = 36u << WARN_SHR,
    WARN_PP_SEL_RANGE = 37u << WARN_SHR,
    WARN_PP_TRAILING = 38u << WARN_SHR,
    WARN_PRAGMA_BAD = 39u << WARN_SHR,
    WARN_PRAGMA_EMPTY = 40u << WARN_SHR,
    WARN_PRAGMA_NA = 41u << WARN_SHR,
    WARN_PRAGMA_UNKNOWN = 42u << WARN_SHR,
    WARN_PREFIX_BADMODE_O64 = 43u << WARN_SHR,
    WARN_PREFIX_BND = 44u << WARN_SHR,
    WARN_PREFIX_HINT_DROPPED = 45u << WARN_SHR,
    WARN_PREFIX_HLE = 46u << WARN_SHR,
    WARN_PREFIX_INVALID = 47u << WARN_SHR,
    WARN_PREFIX_LOCK_ERROR = 48u << WARN_SHR,
    WARN_PREFIX_LOCK_XCHG = 49u << WARN_SHR,
    WARN_PREFIX_OPSIZE = 50u << WARN_SHR,
    WARN_PREFIX_SEG = 51u << WARN_SHR,
    WARN_PTR = 52u << WARN_SHR,
    WARN_REGSIZE = 53u << WARN_SHR,
    WARN_RELOC_ABS_BYTE = 54u << WARN_SHR,
    WARN_RELOC_ABS_DWORD = 55u << WARN_SHR,
    WARN_RELOC_ABS_QWORD = 56u << WARN_SHR,
    WARN_RELOC_ABS_WORD = 57u << WARN_SHR,
    WARN_RELOC_REL_BYTE = 58u << WARN_SHR,
    WARN_RELOC_REL_DWORD = 59u << WARN_SHR,
    WARN_RELOC_REL_QWORD = 60u << WARN_SHR,
    WARN_RELOC_REL_WORD = 61u << WARN_SHR,
    WARN_SECTION_ALIGNMENT_ROUNDED = 62u << WARN_SHR,
    WARN_UNKNOWN_WARNING = 63u << WARN_SHR,
    WARN_USER = 64u << WARN_SHR,
    WARN_WARN_STACK_EMPTY = 65u << WARN_SHR,
    WARN_ZEROING = 66u << WARN_SHR,
    WARN_ZEXT_RELOC = 67u << WARN_SHR,
    WARN_OTHER = 68u << WARN_SHR
};

}  // namespace cb::build::x86asm
