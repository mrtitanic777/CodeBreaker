// build/x86asm/preproc.hpp -- the preprocessor: NASM's asm/preproc.c (and its MASM mode) as a C++ class.
//
// What it does, as in NASM: reads the source (a main file, then every %include), joins continued lines, tokenizes each
// line, runs the directives (%define %xdefine %assign %macro %rep %if... %push %include %line ...), expands single-line
// and multi-line macros, and hands the assembler one finished line of text at a time (`getline`). In MASM mode (the
// assembly's `masm_mode`) each line first goes through the MASM front end (preproc_masm.hpp), which rewrites MASM
// source into the NASM-language lines the rest then handles.
//
//     Assembly as;
//     Preproc pp(as, host);               // `host`: what the preprocessor asks of the surroundings (below)
//     pp.init(PP_NORMAL options);         // once per run
//     pp.reset("prog.asm", PP_NORMAL, &deps);   // at the start of every pass
//     while (auto line = pp.getline()) ...      // one source line, macros expanded, until the end of the input
//     pp.cleanup_pass();                  // the end of every pass
//
// All the state of NASM's file statics is in this object; nothing is global, so preprocessors run side by side.
//
// What it needs from the rest of the engine is `PpHost` (preproc_needs.hpp): the scanner's identifier classifier, the
// expression evaluator, the file reader and the listing. They are callbacks the driver fills in, so the preprocessor
// compiles and is tested without those modules.
#pragma once

#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/pptok.hpp"
#include "build/x86asm/preproc_needs.hpp"
#include "build/x86asm/preproc_util.hpp"
#include "build/x86asm/types.hpp"

namespace cb::build::x86asm {

// ---- the standard macros (macros_std.cpp) ----

// The sets NASM preloads: the version macros, the NASM-language set, and the TASM extras (-t).
enum class StandardMacros { Version, Nasm, Tasm };
const std::vector<std::string_view>& standard_macros(StandardMacros which);

// A package `%use` loads: its name, its macro lines, its number.
struct UsePackage {
    std::string_view name;
    const std::vector<std::string_view>* macros;
    int index;
};
const std::vector<UsePackage>& use_packages();

// ---- token types of the preprocessor ----

// The token type numbers as plain ints (a token's type is a character, a TokenType or an smacro-parameter number).
namespace ppt {
inline constexpr int EOS = 0;
inline constexpr int WHITESPACE = ' ';
inline constexpr int ID = token_value(TokenType::ID);
inline constexpr int NUM = token_value(TokenType::NUM);
inline constexpr int STR = token_value(TokenType::STR);
inline constexpr int ERRSTR = token_value(TokenType::ERRSTR);
inline constexpr int FLOAT = token_value(TokenType::FLOAT);
inline constexpr int HERE = token_value(TokenType::HERE);
inline constexpr int BASE = token_value(TokenType::BASE);
inline constexpr int OTHER = token_value(TokenType::OTHER);
inline constexpr int PREPROC_ID = token_value(TokenType::PREPROC_ID);
inline constexpr int MMACRO_PARAM = token_value(TokenType::MMACRO_PARAM);
inline constexpr int LOCAL_SYMBOL = token_value(TokenType::LOCAL_SYMBOL);
inline constexpr int LOCAL_MACRO = token_value(TokenType::LOCAL_MACRO);
inline constexpr int ENVIRON = token_value(TokenType::ENVIRON);
inline constexpr int INTERNAL_STR = token_value(TokenType::INTERNAL_STR);
inline constexpr int NAKED_STR = token_value(TokenType::NAKED_STR);
inline constexpr int PREPROC_Q = token_value(TokenType::PREPROC_Q);
inline constexpr int PREPROC_QQ = token_value(TokenType::PREPROC_QQ);
inline constexpr int PREPROC_SQ = token_value(TokenType::PREPROC_SQ);
inline constexpr int PREPROC_SQQ = token_value(TokenType::PREPROC_SQQ);
inline constexpr int PASTE = token_value(TokenType::PASTE);
inline constexpr int COND_COMMA = token_value(TokenType::COND_COMMA);
inline constexpr int INDIRECT = token_value(TokenType::INDIRECT);
inline constexpr int XDEF_PARAM = token_value(TokenType::XDEF_PARAM);
inline constexpr int SMAC_START_PARAMS = token_value(TokenType::SMAC_START_PARAMS);
inline constexpr int QMARK = '?';
inline constexpr int MAX_OPERATOR = token_value(TokenType::MAX_OPERATOR);
inline constexpr int SHR = token_value(TokenType::SHR);
inline constexpr int SAR = token_value(TokenType::SAR);
inline constexpr int SHL = token_value(TokenType::SHL);
inline constexpr int SDIV = token_value(TokenType::SDIV);
inline constexpr int SMOD = token_value(TokenType::SMOD);
inline constexpr int GE = token_value(TokenType::GE);
inline constexpr int LE = token_value(TokenType::LE);
inline constexpr int NE = token_value(TokenType::NE);
inline constexpr int LEG = token_value(TokenType::LEG);
inline constexpr int DBL_AND = token_value(TokenType::DBL_AND);
inline constexpr int DBL_OR = token_value(TokenType::DBL_OR);
inline constexpr int DBL_XOR = token_value(TokenType::DBL_XOR);
}  // namespace ppt

// One token of a line being preprocessed: a singly-linked list node (the list is the line). Tokens live in the
// preprocessor's pool, never freed one by one.
struct Tok {
    Tok* next = nullptr;
    int type = 0;
    std::string text;
};

// ---- the host ----

class Preproc;

// What a macro expansion function of a "magic" single-line macro (%abs(), __?FILE?__ ...) is.
struct SMacro;
using ExpandFn = Tok* (Preproc::*)(const SMacro* s, Tok** params, int nparams);

// Parameter flags of a single-line macro.
enum SparmFlags : int {
    SPARM_PLAIN = 0, SPARM_EVAL = 1, SPARM_STR = 2, SPARM_NOSTRIP = 4, SPARM_GREEDY = 8,
    SPARM_VARADIC = 16, SPARM_OPTIONAL = 32, SPARM_CONDQUOTE = 64, SPARM_UNSIGNED = 128
};

struct SmacParam {
    std::string name;
    int flags = 0;
    char radix = 0;
    Tok* def = nullptr;   // the default value (a token list), or null
};

// A single-line macro (%define).
struct SMacro {
    SMacro* next = nullptr;
    std::string name;
    Tok* expansion = nullptr;
    ExpandFn expand = nullptr;        // null: the plain expansion
    std::uint64_t expandpvt = 0;      // a private value of the expansion function
    std::vector<SmacParam> params;
    int nparam = 0;
    int nparam_min = 0;
    int in_progress = 0;
    bool recursive = false;
    bool varadic = false;
    bool casesense = false;
    bool alias = false;
};

enum NoListFlags : int { NL_LIST = 1, NL_LINE = 2 };

struct MMacro;
struct Line;

// The two stacks each invocation keeps: the macro nest, and the innermost macro (with a reference each).
struct MStk {
    MMacro* mstk = nullptr;
    MMacro* mmac = nullptr;
};

// A multi-line macro (%macro), or a %rep block while it is being defined and run. Counted by reference: a definition,
// an invocation and the lines that finish it each hold one.
struct MMacro {
    MMacro* next = nullptr;
    std::size_t refcnt = 0;
    std::string name;
    bool has_name = false;            // false for a %rep block
    int nparam_min = 0, nparam_max = 0;
    int nolist = 0;
    bool casesense = false;
    bool plus = false;
    bool capture_label = false;
    std::int32_t in_progress = 0;
    std::int32_t max_depth = 0;
    Tok* dlist = nullptr;             // the default parameters as written
    std::vector<Tok*> defaults;       // ...split, indexed from 1 (index 0 unused)
    int ndefs = 0;
    Line* expansion = nullptr;
    MStk mstk, dstk;
    std::vector<Tok*> params;         // the arguments of the current invocation, 1-based (0: the label)
    Tok* iline = nullptr;
    SrcLocation where;
    unsigned nparam = 0, rotate = 0;
    std::string iname;
    std::vector<int> paramlen;
    std::uint64_t unique = 0;
    std::uint64_t condcnt = 0;
};

struct Line {
    Line* next = nullptr;
    MMacro* finishes = nullptr;
    Tok* first = nullptr;
    SrcLocation where;
    bool suppressed = false;
};

// The state of one %if: the branch taken or not, whether the %else was seen, done (an earlier branch was taken), never (the
// whole %if is inside skipped text). EVAL_ERROR is what a failed %if expression leaves (NASM stores -1): nothing is emitted.
enum class CondState { IF_TRUE, IF_FALSE, ELSE_TRUE, ELSE_FALSE, DONE, NEVER, EVAL_ERROR = -1 };

// A %push context: a name and its own single-line macros.
using SMacroTable = std::unordered_map<std::string, SMacro*>;   // by lower-cased name; the value heads a list
struct Context {
    Context* next = nullptr;
    std::optional<std::string> name;
    SMacroTable localmac;
    std::uint64_t number = 0;
    unsigned depth = 0;
};

// An open input: a file (read from memory), or a block of standard macro lines.
struct Include {
    Include* next = nullptr;
    std::vector<CondState> conds;     // the %if stack; the back is the innermost
    Line* expansion = nullptr;
    bool is_file = false;
    std::string data;                 // a file's whole text
    std::size_t datapos = 0;
    const std::vector<std::string_view>* stdlines = nullptr;   // the lines of a macro package, when not a file
    std::size_t stdpos = 0;
    std::uint64_t nolist = 0;
    std::uint64_t noline = 0;
    MStk mstk;
    SrcLocation where;
    std::int32_t lineinc = 0;
    std::int32_t lineskip = 0;
};

class MasmFront;

// The preprocessor.
class Preproc {
public:
    Preproc(Assembly& assembly, PpHost host);
    ~Preproc();
    Preproc(const Preproc&) = delete;
    Preproc& operator=(const Preproc&) = delete;

    // pp_init: the options (PreprocOpt bits) for the whole run.
    void init(int opt);
    // pp_reset: begin a pass over `file` (read through the host). `dep_list` (may be null) gets every file read.
    void reset(std::string_view file, PreprocMode mode, std::vector<std::string>* dep_list);
    // pp_getline: the next line of preprocessed source, or nullopt at the end of the input.
    std::optional<std::string> getline();
    // pp_cleanup_pass / pp_cleanup_session.
    void cleanup_pass();
    void cleanup_session();
    // pp_include_path: the -I directories (each used as a prefix as written).
    void include_path(std::vector<std::string> dirs) { ipath_ = std::move(dirs); }
    // pp_pre_include / pp_pre_define / pp_pre_undefine / pp_pre_command: lines run before the source (-P, -D, -U, -p).
    void pre_include(std::string_view fname);
    void pre_define(std::string_view definition);
    void pre_undefine(std::string_view definition);
    void pre_command(std::optional<std::string_view> what, std::string_view string);
    // The output format's own standard macros (ofmt->stdmac); set before reset().
    void set_format_macros(const std::vector<std::string>* lines) { format_macros_ = lines; }
    // The listing options mask now (-L) and from the command line (for __?LIST_OPTIONS?__), as bits of a..zA..Z0..9.
    void set_list_options(std::uint64_t active, std::uint64_t cmdline) { active_list_options_ = active; cmdline_list_options_ = cmdline; }
    // True when a message of this severity is to be dropped because the preprocessor is skipping (pp_suppress_error).
    bool suppress_error(ErrFlags severity) const;
    // pp_error_list_macros: add the "from macro" lines to a message.
    void error_list_macros(ErrFlags severity);

private:
    friend class MasmFront;

    // ---- types private to the implementation ----
    enum ClearWhat : int {
        CLEAR_NONE = 0, CLEAR_DEFINE = 1, CLEAR_DEFALIAS = 2, CLEAR_ALLDEFINE = 3, CLEAR_MMACRO = 4, CLEAR_ALL = 7
    };
    enum IncOpenMode : int { INC_OPTIONAL = 0, INC_NEEDED = 1, INC_REQUIRED = 2, INC_PROBE = 4, INC_EXACT = 8 };
    enum ConcatFlags : int {
        CONCAT_ID = 0x01, CONCAT_LOCAL_MACRO = 0x02, CONCAT_ENVIRON = 0x04, CONCAT_PREPROC_ID = 0x08,
        CONCAT_NUM = 0x10, CONCAT_FLOAT = 0x20, CONCAT_OP = 0x40
    };
    struct ConcatMask {
        int mask_head;
        int mask_tail;
    };
    struct FileHashEntry {
        std::string path;
        bool has_path = false;
        FileHashEntry* full = nullptr;
        std::int64_t include_pass = 0;
        int omode = 0;
    };
    struct Deadman {
        std::int64_t total = 0;
        std::int64_t levels = 0;
        bool triggered = false;
    };
    // A token list being scanned by the expression evaluator (struct ppscan).
    struct PpScan {
        Tok* tptr = nullptr;
        int ntokens = 0;
    };
    // pp_config and the debug flags.
    struct Config {
        bool noaliases = false;
        bool sane_empty_expansion = false;
    };
    enum PpDebugFlags : int { PDBG_MMACROS = 1, PDBG_SMACROS = 2, PDBG_LIST_SMACROS = 4, PDBG_INCLUDE = 8 };

    static constexpr int NO_DIRECTIVE_FOUND = 0;
    static constexpr int DIRECTIVE_FOUND = 1;
    static constexpr int PARAM_DELTA = 16;

    // ---- tokens (preproc_tok.cpp) ----
    Tok* alloc_token();
    Tok* free_token(Tok* t);
    Tok* do_delete_token(Tok*& t);
    void free_tlist(Tok* list);
    void delete_tlist(Tok*& list);
    Tok* new_token(Tok* next, int type, std::string_view text, std::size_t = 0);
    Tok* new_token_str(Tok* next, int type, std::string text);
    Tok* dup_token(Tok* next, const Tok* src);
    Tok* new_white(Tok* next);
    Tok* steal_token(Tok* dst, Tok* src);
    Tok* dup_tlist(const Tok* list, Tok*** tailp);
    Tok* dup_tlistn(const Tok* list, std::size_t cnt, Tok*** tailp);
    Tok* dup_tlist_reverse(const Tok* list, Tok* tail);
    Tok** steal_tlist(Tok* tlist, Tok** tailp);
    Tok* cut_tlist(Tok* t);
    Tok** tlist_endptr(Tok** tptr);
    Tok* reverse_tokens(Tok* t);
    Tok* make_tok_num(Tok* next, std::int64_t val);
    Tok* make_tok_num_radix(Tok* next, std::int64_t val, char radix, bool uns);
    std::int64_t get_tok_num(const Tok* t, bool* err);
    Tok* make_tok_qstr(Tok* next, std::string_view str);
    Tok* make_tok_char(Tok* next, char op);
    Tok* tokenize(std::string_view line);
    std::string detoken(Tok* tlist, bool expand_locals);
    void set_text(Tok* t, std::string_view text) { t->text.assign(text); }
    const char* unquote_token_anystr(Tok* t, std::uint32_t badctl, char qstart);
    const char* unquote_token(Tok* t) { return unquote_token_anystr(t, 0, pp::STR_NASM); }
    const char* unquote_token_cstr(Tok* t) { return unquote_token_anystr(t, pp::BADCTL, pp::STR_NASM); }
    Tok* quote_any_token(Tok* t);
    std::optional<std::string> pp_getenv(const Tok* t, bool warn);

    static bool emitting(CondState s) { return s == CondState::IF_TRUE || s == CondState::ELSE_TRUE; }
    void masm_prescan(const std::string& text);

    static int tok_type(const Tok* x) { return x ? x->type : ppt::EOS; }
    static bool tok_is(const Tok* x, int t) { return tok_type(x) == t; }
    static bool tok_isnt(const Tok* x, int t) { return x && x->type != t; }
    static bool tok_white(const Tok* x) { return tok_is(x, ppt::WHITESPACE); }
    static bool tok_string(const Tok* x) { return x && (x->type == ppt::STR || x->type == ppt::INTERNAL_STR); }
    static bool tok_macro_id(const Tok* x) { return x && (x->type == ppt::ID || x->type == ppt::LOCAL_MACRO); }
    static bool tok_macro_or_func_id(const Tok* x) {
        return x && (x->type == ppt::ID || x->type == ppt::PREPROC_ID || x->type == ppt::LOCAL_MACRO);
    }
    static Tok* skip_tok(Tok* x) { return x ? x->next : nullptr; }
    static Tok* skip_white(Tok* x) { while (tok_white(x)) x = x->next; return x; }
    static Tok* skip_tok_white(Tok* x) { return skip_white(skip_tok(x)); }
    Tok* zap_white(Tok* x) { while (tok_white(x)) x = free_token(x); return x; }
    static int tok_smac_param(int param) { return ppt::SMAC_START_PARAMS + param; }
    static int smac_nparam(int toktype) { return toktype - ppt::SMAC_START_PARAMS; }
    static bool is_smac_param(int toktype) { return toktype >= ppt::SMAC_START_PARAMS; }
    static bool tok_text_match(const Tok* a, const std::string& b) { return a->text == b; }

    // ---- macros and contexts (preproc_macro.cpp) ----
    MMacro* get_mmacro(MMacro* m);
    MMacro* pop_mmacro(MMacro** mp, MMacro* next);
    void put_mmacro(MMacro** mp);
    void pop_mstk(MStk* msp, MMacro* nextp);
    MMacro* new_mmacro();
    void clear_mmacro(MMacro* m);
    void free_mmacro(MMacro* m);
    void free_mmacro_list(MMacro* list);
    void free_line(Line* l);
    void free_llist(Line* list);
    void free_smacro_members(SMacro* s);
    void clear_smacro(SMacro* s);
    void free_smacro(SMacro* s);
    void clear_smacro_table(SMacroTable& smt, ClearWhat what);
    void free_mmacro_table(std::unordered_map<std::string, MMacro*>& mmt);
    void free_macros();
    void ctx_pop();
    Line* new_line();
    Context* get_ctx(std::string_view name, std::string_view* namep);
    bool smacro_defined(Context* ctx, std::string_view name, int nparam, SMacro** defn, bool nocase, bool find_alias);
    static bool streq(std::string_view p, std::string_view q, bool casesense) { return casesense ? p == q : pp::ieq(p, q); }
    SMacro* find_smacro_head(SMacroTable& tbl, std::string_view name);
    int parse_smacro_template(Tok*** tpp, SMacro* tmpl);
    SMacro* define_smacro(std::string_view mname, bool casesense, Tok* expansion, SMacro* tmpl);
    void undef_smacro(std::string_view mname, bool undefalias);
    bool parse_mmacro_spec(Tok* tline, MMacro* def, std::string_view directive);
    void mark_smac_params(Tok* tline, const SMacro* tmpl, int type);
    void do_clear(ClearWhat what, bool context);
    void define_stack_smacro(std::string_view name, int offset);
    void assign_smacro(std::string_view mname, bool casesense, Tok* tline, std::string_view dname);
    void list_smacro_def(int op, const Context* ctx, const SMacro* m);
    int read_param_count(std::string_view str);
    Tok** count_mmac_params(Tok* tline, int* nparamp, std::vector<Tok*>* paramsp);
    Tok* smacro_expand_default(const SMacro* s, Tok** params, int nparams);

    // ---- input (preproc.cpp) ----
    bool inc_open(std::string_view file, std::vector<std::string>* dhead, FileHashEntry** found_fhe, int omode, std::string* text);
    std::optional<std::string> inc_search(std::string_view file, std::string* slpath, int* omp, bool probe);
    std::optional<std::string> line_from_file(Include* inc);
    std::optional<std::string> line_from_stdmac();
    std::optional<std::string> read_line();
    void inject_predefs();
    void pp_start_stdmac();
    void pp_add_stdmac(const std::vector<std::string_view>* macros);
    void pp_reset_stdmac(PreprocMode mode);
    void pp_add_limits_stdmac();
    void pp_add_list_options_default_stdmac();
    Include* pop_include_stack();
    Tok* pp_tokline();
    Tok* tlist_filename(Tok** tp, Tok** otp, std::string_view dname);
    Tok* pp_do_pathsearch(Tok** tp, std::string_view dname);
    Tok* pp_do_depend(Tok** tp, std::string_view dname);
    const UsePackage* get_use_pkg(Tok* t, std::string_view dname, std::optional<std::string>& name);

    // ---- directives (preproc_directive.cpp) ----
    int do_directive(Tok* tline, Tok** output, bool suppressed);
    int line_directive(Tok* origline, Tok* tline);
    CondState if_condition(Tok* tline, int ct, std::string_view dname);
    static bool is_condition(int arg) { return pp_is_cond(arg) || arg == PP_ELSE || arg == PP_ENDIF; }
    int pp_get_nasm_directive(std::string_view dname) const;
    int pp_get_tasm_directive(std::string_view dname) const;
    int pp_get_directive(std::string_view dname) const;
    bool is_directive(std::string_view dname);
    bool pp_get_boolean_option(Tok* tline, bool defval);
    void do_pragma_preproc(Tok* tline);
    std::string_view get_id(Tok** tp, std::string_view dname, bool& ok);
    std::string_view get_id_noskip(Tok** tp, std::string_view dname, bool& ok);
    Tok* pp_strcat(Tok* tline, std::string_view dname);
    Tok* pp_substr(Tok* tline, std::string_view dname);
    std::string_view pp_get_substr(Tok* t, std::int64_t start, std::int64_t* countp);
    Tok* pp_substr_common(Tok* t, std::int64_t start, std::int64_t count);
    MMacro* do_exit_macro(std::string_view dname, bool named);
    void user_error(int op, Tok** tlinep);
    int parse_size(std::string_view str);

    // ---- expansion (preproc_expand.cpp) ----
    static int find_cc(Tok* t);
    static bool pp_concat_match(const Tok* t, int mask);
    bool paste_tokens(Tok** head, const ConcatMask* m, std::size_t mnum, bool handle_explicit);
    int mmac_rotate(const MMacro* mac, unsigned n);
    void expand_mmac_params_range(MMacro* mac, Tok* tline, Tok*** tail);
    Tok* expand_mmac_params(Tok* tline);
    Tok* expand_sparm_str(Tok* param, int flags);
    Tok* expand_smacro_with_params(SMacro* m, Tok* mstart, Tok** params, int nparam, Tok*** epp);
    int count_smacro_args(Tok* t, Tok** tp);
    std::vector<Tok*> parse_smacro_args(Tok** tp, int* nparamp, const SMacro* m);
    SMacro* expand_one_smacro(Tok*** tpp);
    Tok* expand_smacro(Tok* tline);
    Tok* expand_smacro_noreset(Tok* org_tline);
    Tok* expand_id(Tok* tline);
    MMacro* use_mmacro(MMacro* m, int* nparamp, std::vector<Tok*>* paramsp);
    MMacro* find_mmacro_in_list(MMacro* m, std::string_view finding, int* nparamp, std::vector<Tok*>* paramsp);
    MMacro* is_mmacro(Tok* tline, int* nparamp, std::vector<Tok*>* paramsp);
    void list_mmacro_call(const MMacro* m);
    int expand_mmacro(Tok* tline);

    // ---- the magic macros (preproc_magic.cpp) ----
    SMacro* define_magic(std::string_view mname, bool casesense, SMacro* tmpl);
    void pp_add_magic_simple();
    void pp_add_magic_isfunc();
    void pp_add_magic_miscfunc();
    void pp_add_magic_stdmac();
    Tok* get_list_options(std::uint64_t optmask);
    Tok* stdmac_file(const SMacro*, Tok**, int);
    Tok* stdmac_line(const SMacro*, Tok**, int);
    Tok* stdmac_bits(const SMacro*, Tok**, int);
    Tok* stdmac_ptr(const SMacro*, Tok**, int);
    Tok* stdmac_default(const SMacro*, Tok**, int);
    Tok* stdmac_is(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_join(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_strcat(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_hs2b(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_b2hs(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_substr(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_ord(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_chr(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_strlen(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_tok(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_sel(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_cond(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_selbits(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_count(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_num(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_abs(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_map(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_pathsearch(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_depend(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_realpath(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_null(const SMacro*, Tok**, int);
    Tok* stdmac_find(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_env(const SMacro*, Tok** params, int nparams);
    Tok* stdmac_limit(const SMacro* s, Tok** params, int nparams);
    Tok* stdmac_list_options(const SMacro*, Tok**, int);

    // ---- the expression evaluator through a token list ----
    // Evaluate the tokens from `pps` with the host's evaluator (the call `evaluate(ppscan, &pps, &tokval, NULL, true, NULL)`).
    std::optional<Expr> evaluate_tokens(PpScan& pps, TokenVal& tokval);
    TokenType ppscan(PpScan& pps, TokenVal& tokval);

    // ---- state ----
    Assembly& as_;
    PpHost host_;
    std::unique_ptr<MasmFront> masm_;

    Config ppconf_;
    int ppdbg_ = 0;
    int ppopt_ = 0;

    std::unordered_map<std::string, MMacro*> mmacros_;
    SMacroTable smacros_;
    MMacro* defining_ = nullptr;
    std::array<bool, PP_count> pp_op_may_be_function_{};

    std::deque<Tok> token_pool_;
    Tok* free_tokens_ = nullptr;
    std::deque<Line> line_pool_;
    Line* free_lines_ = nullptr;

    std::unordered_map<std::string, std::unique_ptr<FileHashEntry>> file_hash_;
    Deadman smacro_deadman_, mmacro_deadman_;

    int stack_size_ = 4;
    std::string stack_pointer_ = "ebp";
    int arg_offset_ = 8;
    int local_offset_ = 0;
    Context* cstk_ = nullptr;
    Include* istk_ = nullptr;
    std::vector<std::string> ipath_;
    std::vector<std::string>* deplist_ = nullptr;
    std::uint64_t unique_ = 0;
    Line* predef_ = nullptr;
    bool do_predef_ = false;
    PreprocMode pp_mode_ = PP_NORMAL;
    std::uint64_t nested_mac_count_ = 0;
    std::uint64_t nested_rep_count_ = 0;
    std::array<const std::vector<std::string_view>*, 8> stdmacset_{};   // the standard macro blocks still to be read
    std::size_t stdmaclist_ = 0;                                         // the next one to read
    const std::vector<std::string_view>* stdcur_ = nullptr;              // the block being read, and where
    std::size_t stdpos_ = 0;
    const std::vector<std::string>* format_macros_ = nullptr;
    std::vector<std::string_view> format_views_;   // the format's lines as views (valid while format_macros_ is)
    std::vector<bool> use_loaded_;
    std::uint64_t active_list_options_ = 0;
    std::uint64_t cmdline_list_options_ = 0;
    Tok tok_pop_;   // the marker pp_tokline returns for "a level was popped"
};

}  // namespace cb::build::x86asm
