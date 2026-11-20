// cb/build/x86asm/assembly.hpp -- `Assembly`: all the state of one run of x86asm.
//
// NASM keeps its run state in globals and file statics spread over nasm.c, error.c, srcfile.c, directiv.c (the CPU
// level) and parser.c (the MASM data types): the pass number and kind, the current location, the per-pass options,
// the optimization level, the CPU, the limits, the warning classes and their stack, the collected diagnostics. Here
// all of it is one object, so every later module (scanner, evaluator, encoder, parser, preprocessor, output formats)
// takes an `Assembly&` and touches no global; two assemblies can run at once in one process.
//
// The error reporting is NASM's error.c, with the same text: "file:line: error: message" (or "file(line) : error:
// message" in the MSVC format), the same suppression rules, the same promotion of a warning to an error, the same
// buffering of warnings until an error or the final pass, the same held-error stack. Output that NASM wrote to its
// error file is collected in `error_output()` and `diagnostics()`. A FATAL, CRITICAL or PANIC error does not exit: the
// message is recorded, `exit_code()` says the status NASM would have exited with, and an `AsmError` is thrown.
//
// Messages are formatted with std::format, not printf: "%s" becomes "{}", a literal brace is doubled. The text that
// comes out is NASM's.
//
// This is the place the later modules add their own run state to (the label table, the section tables, the macro
// tables); NASM's globals of those modules become members here, as these did.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <format>
#include <functional>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cb/build/asm_text.hpp"
#include "cb/build/x86asm/types.hpp"
#include "cb/build/x86asm/warnings.hpp"

namespace cb::build::x86asm {

// ---- error severities and flags (include/error.h) ----

// A severity word: the low bits are the severity (ERR_MASK) and flags, the high 16 bits the warning class (or the
// debug/info level).
using ErrFlags = std::uint32_t;

inline constexpr ErrFlags ERR_LISTMSG = 0x00000000;   // for the listing file only (comment prefix)
inline constexpr ErrFlags ERR_NOTE = 0x00000001;      // for the listing file only (with prefix)

// Non-terminating diagnostics; can be suppressed.
inline constexpr ErrFlags ERR_DEBUG = 0x00000004;     // internal debugging message
inline constexpr ErrFlags ERR_INFO = 0x00000005;      // informational message
inline constexpr ErrFlags ERR_WARNING = 0x00000006;   // warning

// Errors which terminate assembly without output.
inline constexpr ErrFlags ERR_NONFATAL = 0x00000008;   // terminate assembly after the current pass

// From here errors cannot be suppressed.
inline constexpr ErrFlags ERR_FATAL = 0x0000000c;      // terminate immediately, but perform cleanup

// Abort conditions: terminate with minimal or no cleanup.
inline constexpr ErrFlags ERR_CRITICAL = 0x0000000e;   // fatal, but minimize code before the end (system errors)
inline constexpr ErrFlags ERR_PANIC = 0x0000000f;      // internal error: a bug in the code itself

inline constexpr ErrFlags ERR_MASK = 0x0000000f;           // mask off the above codes
inline constexpr ErrFlags ERR_UNDEAD = 0x00000010;         // skip if we already have errors
inline constexpr ErrFlags ERR_NOFILE = 0x00000020;         // don't give source file name/line
inline constexpr ErrFlags ERR_HERE = 0x00000040;           // point to a specific source location
inline constexpr ErrFlags ERR_USAGE = 0x00000080;          // print a usage message
inline constexpr ErrFlags ERR_PASS2 = 0x00000100;          // ignore unless on pass_final
inline constexpr ErrFlags ERR_NO_SEVERITY = 0x00000200;    // suppress printing severity
inline constexpr ErrFlags ERR_PP_PRECOND = 0x00000400;     // for preprocessor use
inline constexpr ErrFlags ERR_PP_LISTMACRO = 0x00000800;   // from pp_error_list_macros()
inline constexpr ErrFlags ERR_HOLD = 0x00001000;           // this error/warning can be held
inline constexpr ErrFlags ERR_PERROR = 0x00002000;         // append strerror(errno)

// The warning class of a severity word, and a class (or a debug/info level) as flag bits.
constexpr ErrFlags warn_idx(ErrFlags x) { return x >> WARN_SHR; }
inline constexpr ErrFlags WARN_MASK = ~ErrFlags{0} << WARN_SHR;
constexpr ErrFlags warning(ErrFlags x) { return x << WARN_SHR; }
constexpr ErrFlags level(ErrFlags x) { return warning(x); }   // the same field is used for debug and info levels

// NASM's -w/-W warning-stack, error-hold and run-status bookkeeping (struct errinfo): `worst` is the worst severity
// class seen, `never` the flags to suppress unconditionally, the others the debug and info message levels.
struct ErrInfo {
    ErrFlags worst = 0;
    ErrFlags never = 0;
    unsigned debug_nasm = 0;
    unsigned verbose_info = 0;
};

// One line NASM would have written to its error file: the severity it was given, the severity it ended up with (a
// warning treated as an error is NONFATAL), and the full line without its newline.
struct Diagnostic {
    ErrFlags severity = 0;
    ErrFlags true_type = 0;
    std::string text;
};

// The text before a message of a severity: "error: ", "warning: ", ... (error_pfx).
const char* error_prefix(ErrFlags severity);

// A hold on the error stack (nasm_error_hold_push): 0 is "no hold".
using ErrHold = std::size_t;

// ---- what the surroundings of an assembly supply ----

// The callbacks error.c makes into the preprocessor, the listing and the driver. Unset ones do nothing (and nothing
// is suppressed).
struct AssemblyHooks {
    std::function<bool(ErrFlags)> pp_suppress_error;                       // pp_suppress_error: true to suppress the message
    std::function<void(ErrFlags)> pp_error_list_macros;                    // pp_error_list_macros: unwind the macro stack in the message
    std::function<void(ErrFlags, const std::string&)> list_error;          // lfmt->error: the listing file's copy of a message
    std::function<void(bool failure)> final_report;                        // print_final_report + lfmt->cleanup, before a FATAL error ends the run
    std::function<void(bool failure)> close_output;                        // close_output, before any ending error
    std::function<std::int64_t(std::string_view, bool& error)> readnum;    // readnum: a number as the source writes it (default: decimal and 0x hex)
};

// ---- the source position stack (srcfile.c) ----

// Where in the input the assembly is, and where inside a non-.nolist macro: a stack (outermost first) of locations,
// each with the macro it is in. Error messages and listings read it.
class SourcePosition {
public:
    SourcePosition() : frames_(1), error_(0) {}

    // The current (innermost) file name and line.
    std::string_view get_fname() const { return frames_.back().where.filename; }
    std::int32_t get_linnum() const { return frames_.back().where.lineno; }
    // Set the file name (interned; the old one is returned) and the line (the old one is returned).
    std::string_view set_fname(std::string_view name);
    std::int32_t set_linnum(std::int32_t line);
    void set(std::int32_t line, std::string_view fname);

    // The innermost location; the outermost; and the one error messages use (the outermost except while the macro
    // hierarchy is being walked down).
    SrcLocation where() const { return frames_.back().where; }
    SrcLocation where_top() const { return frames_.front().where; }
    SrcLocation where_error() const { return frames_[error_].where; }
    // Walk one level down for the error message: the macro there, or nullptr at the bottom.
    const void* error_down();
    void error_reset() { error_ = 0; }

    // Replace the innermost location, returning the old one. The file name must have come from this object.
    SrcLocation update(SrcLocation whence);

    // src_get: the change since the position the caller keeps in (xline, xname): 0 if the same, -2 if the file
    // changed, else the line difference. Updates the caller's copy.
    std::int32_t get(std::int32_t& xline, std::string_view& xname) const;

    // Push/pop a macro expansion level; `macro` must stay valid until the same level is popped.
    void macro_push(const void* macro, SrcLocation where);
    const void* macro_current() const { return frames_.back().macro; }
    void macro_pop();

    // A file name kept for the whole run (so its characters stay valid).
    std::string_view intern(std::string_view name);

private:
    struct Frame {
        SrcLocation where;
        const void* macro = nullptr;
    };
    std::vector<Frame> frames_;   // [0] is the top level, back() the innermost
    std::size_t error_;           // the frame error messages use
    std::unordered_set<std::string> names_;
};

// ---- the run ----

// The time of the start of the compilation (for __DATE__, __TIME__ and the like).
struct CompileTime {
    std::time_t t = 0;
    bool have_local = false, have_gm = false, have_posix = false;
    std::int64_t posix = 0;
    std::tm local{};
    std::tm gm{};
};

// One forward reference recorded in an earlier pass: the operand of the instruction on a line.
struct ForwRefInfo {
    std::int64_t lineno = 0;
    int operand = 0;
};

// All the state of one assembly (see the file comment).
class Assembly {
public:
    Assembly();

    // ---- files and modes (nasm.c, parser.c) ----

    std::string progname = "nasm";   // program name for messages (_progname)
    std::string inname;    // primary input filename
    std::string outname;   // output filename

    bool tasm_compatible_mode = false;
    bool masm_mode = false;           // --masm: MASM 6.x source compatibility
    bool masm_if_undef_zero = false;  // --masm: undefined symbol in %if -> 0
    bool reproducible = false;        // attempt to produce run-to-run identical output

    // The time stamp for the official start of compilation; `set_compile_time` fills it as NASM's timestamp() does.
    CompileTime official_compile_time;
    void set_compile_time(std::time_t now);
    // POSIX timestamp if and only if the build is not reproducible.
    std::int64_t posix_timestamp() const { return reproducible ? 0 : official_compile_time.posix; }

    // The MASM data-type size (bytes) of a label, so a bare reference to it reads its contents; names are folded to
    // lower case; size 0 is ignored (non-data). masm_type_query: 0 if none.
    void masm_type_note(std::string_view name, int size);
    int masm_type_query(std::string_view name) const;

    // ---- passes (nasm.h, nasm.c) ----

    PassType pass_type() const { return pass_type_; }
    const char* pass_type_name() const;
    bool not_started() const { return pass_type_ == PASS_INIT; }                       // no code read yet
    bool pass_first() const { return pass_type_ <= PASS_FIRST; }                       // the initial pass and setup
    bool pass_stable() const { return pass_type_ >= PASS_STAB; }                       // definitions must be stable
    bool pass_final() const { return pass_type_ >= PASS_FINAL; }                       // the code generation pass
    bool pass_final_or_preproc() const { return pass_type_ >= PASS_FINAL || pass_type_ == PASS_PREPROC; }
    // The actual pass number: 0 during initialization, the very first pass is 1, then increasing.
    std::int64_t pass_count() const { return passn_; }

    // Go to preprocess-only mode (the one pass of nasm -E).
    void begin_preproc_pass();
    // Start the next pass (the top of nasm.c's pass loop): the pass number and kind advance (an optimization pass
    // repeats while a label moved), warnings are buffered unless final, per-pass defaults and the CPU are reset, and
    // the location goes back to the start. (The driver then switches to the format's first segment and resets the
    // preprocessor.)
    void begin_pass();

    // The optimization level (-O), and whether the user gave one explicitly.
    Optimization optimizing = OPTIM_DEFAULT;
    bool optimizing_set = false;

    int cmd_sb = 16;   // the default bits mode from the command line
    GlobalOpt globl;   // pass-wide state, reset at the top of each pass
    void reset_global_defaults(int bits);

    // ---- the location (nasm.c) ----

    Location location;       // current assembly location
    Location absolute;       // segment/offset inside ABSOLUTE
    bool in_absolute = false;   // we are in an ABSOLUTE segment
    std::uint64_t global_offset_changed = 0;   // (labels.c) a label moved in this pass

    // Switch to a different segment and return the current offset in it.
    std::int64_t switch_segment(std::int32_t segment);
    // Set the offset at the current location; returns it.
    std::int64_t set_curr_offs(std::int64_t offset);
    // Move the offset on by `delta`; returns the new offset.
    std::int64_t increment_offset(std::int64_t delta);

    // The forward references of the previous pass, by line: put on the operands of the instruction at the current
    // line (forward_refs). `current_line` counts the lines of this pass.
    std::int64_t globallineno = 0;
    void forward_refs(Insn& instruction);

    // ---- the CPU (directiv.c) ----

    IFlags cpu;        // the CPU level and features in force
    IFlags cmd_cpu;    // the one the command line set
    // `CPU` directive and -- the same text: "8086", "386", "p6", "+sse", "nofpu" ... A null value sets the default.
    // Reports an unknown name as a non-fatal error.
    void set_cpu(std::optional<std::string_view> value);
    // The bits value of a BITS directive: 16, 32 or 64, else an error and 16.
    int get_bits(std::string_view value);

    // ---- limits (nasm.c) ----

    std::array<std::int64_t, LIMIT_MAX> nasm_limit{};              // the limits in force
    std::array<std::int64_t, LIMIT_MAX> nasm_limit_from_cmdline{}; // the limits as the command line left them
    void set_default_limits();
    void reset_limits_before_pass() { nasm_limit = nasm_limit_from_cmdline; }
    // Remember the limits as the command line left them (after it is parsed).
    void keep_limits_from_cmdline() { nasm_limit_from_cmdline = nasm_limit; }
    // %pragma limit / --limit-X: set one by name from a value ("reset", "default", "unlimited", or a number).
    DirectiveResult set_limit(std::string_view limit, std::string_view valstr);
    // Read one limit by name (0 if there is none such).
    std::int64_t get_limit(std::string_view limit, GetLimitWhich which) const;
    static const char* limit_name(NasmLimit limit);
    static std::int64_t limit_default(NasmLimit limit);
    static std::int64_t limit_max(NasmLimit limit);
    // A limit as text: its number, or "unlimited".
    static std::string limit_str(std::int64_t limit_val);

    // ---- errors and warnings (error.c, errstubs.c) ----

    ErrInfo erropt;
    AssemblyHooks hooks;
    // The state of every warning class (WARN_ST_* bits), indexed by WarnIndex.
    std::array<std::uint8_t, NUM_WARNINGS> warning_state{};

    // Where in the input we are, for messages and listings.
    SourcePosition src;

    // The text NASM wrote to its error file, and the same as lines.
    const std::string& error_output() const { return error_output_; }
    const std::vector<Diagnostic>& diagnostics() const { return diagnostics_; }
    void clear_diagnostics() { error_output_.clear(); diagnostics_.clear(); }
    // The status NASM would have exited with after a fatal error (FATAL 1, CRITICAL 3, PANIC 4); 0 until then.
    int exit_code() const { return exit_code_; }
    // Set the format of the "file:line:" part: "gnu"/"gcc" (the default) or "vc"/"msvc"/"ms". 0 on success, -1 unknown.
    int set_error_format(std::string_view fmt);

    // Called once after the command line is parsed (the first push of the warning stack), and at the start and end of
    // every pass (warnings are buffered unless `final`; the end drops what was not printed and unwinds the stack).
    void error_init();
    void error_pass_start(bool final);
    void error_pass_end();

    // A warning option or directive: "all", "other", "+name", "-name", "no-name", "*name" (reset), "error=name", ...
    // Returns true if the action was successful (an unknown name warns).
    bool set_warning_status(std::string_view value);
    // Warning stack management (%pragma warning push/pop).
    void push_warnings();
    void pop_warnings();

    // True if a warning is enabled, either as a warning or an error.
    bool warn_active(ErrFlags warn) const;
    bool debug_level(unsigned lvl) const { return lvl <= erropt.debug_nasm; }
    bool info_level(unsigned lvl) const { return lvl <= erropt.verbose_info; }

    // The common back end of all reporting (nasm_verror): `severity` carries the severity, the flags and the warning
    // class; `message` is the finished text. A FATAL, CRITICAL or PANIC severity (after a warning's promotion) throws.
    void report(ErrFlags severity, std::string message);

    // nasm_error with a format: the severity is the flags' own.
    template <class... A>
    void error(ErrFlags flags, std::format_string<A...> fmt, A&&... args) {
        report(flags, std::format(fmt, std::forward<A>(args)...));
    }
    // The helpers of errstubs.c: a message at a fixed severity (the "f" variants take more flags).
    template <class... A>
    void listmsg(std::format_string<A...> fmt, A&&... args) { report(ERR_LISTMSG, std::format(fmt, std::forward<A>(args)...)); }
    template <class... A>
    void listmsgf(ErrFlags flags, std::format_string<A...> fmt, A&&... args) { report(ERR_LISTMSG | flags, std::format(fmt, std::forward<A>(args)...)); }
    template <class... A>
    void note(std::format_string<A...> fmt, A&&... args) { report(ERR_NOTE, std::format(fmt, std::forward<A>(args)...)); }
    template <class... A>
    void notef(ErrFlags flags, std::format_string<A...> fmt, A&&... args) { report(ERR_NOTE | flags, std::format(fmt, std::forward<A>(args)...)); }
    template <class... A>
    void nonfatal(std::format_string<A...> fmt, A&&... args) { report(ERR_NONFATAL, std::format(fmt, std::forward<A>(args)...)); }
    template <class... A>
    void nonfatalf(ErrFlags flags, std::format_string<A...> fmt, A&&... args) { report(ERR_NONFATAL | flags, std::format(fmt, std::forward<A>(args)...)); }
    template <class... A>
    [[noreturn]] void fatal(std::format_string<A...> fmt, A&&... args) { report(ERR_FATAL, std::format(fmt, std::forward<A>(args)...)); never_returned(); }
    template <class... A>
    [[noreturn]] void fatalf(ErrFlags flags, std::format_string<A...> fmt, A&&... args) { report(ERR_FATAL | flags, std::format(fmt, std::forward<A>(args)...)); never_returned(); }
    template <class... A>
    [[noreturn]] void critical(std::format_string<A...> fmt, A&&... args) { report(ERR_CRITICAL, std::format(fmt, std::forward<A>(args)...)); never_returned(); }
    template <class... A>
    [[noreturn]] void criticalf(ErrFlags flags, std::format_string<A...> fmt, A&&... args) { report(ERR_CRITICAL | flags, std::format(fmt, std::forward<A>(args)...)); never_returned(); }
    template <class... A>
    [[noreturn]] void panic(std::format_string<A...> fmt, A&&... args) { report(ERR_PANIC, std::format(fmt, std::forward<A>(args)...)); never_returned(); }
    template <class... A>
    [[noreturn]] void panicf(ErrFlags flags, std::format_string<A...> fmt, A&&... args) { report(ERR_PANIC | flags, std::format(fmt, std::forward<A>(args)...)); never_returned(); }

    // A warning of class `warn` (a WarnConst): the message is not even formatted when the class is off.
    template <class... A>
    void warn(ErrFlags warn_class, std::format_string<A...> fmt, A&&... args) {
        if (warn_active(warn_class)) report(ERR_WARNING | warn_class, std::format(fmt, std::forward<A>(args)...));
    }
    // Informational and debug messages, enabled by their level.
    template <class... A>
    void info(unsigned lvl, std::format_string<A...> fmt, A&&... args) {
        if (info_level(lvl)) report(ERR_INFO | level(lvl), std::format(fmt, std::forward<A>(args)...));
    }
    template <class... A>
    void debug(unsigned lvl, std::format_string<A...> fmt, A&&... args) {
        if (debug_level(lvl)) report(ERR_DEBUG | level(lvl), std::format(fmt, std::forward<A>(args)...));
    }

    // NASM's panic() macro and nasm_assert: "internal error in <function> at <file>:<line>" and "assertion <text> failed ...".
    [[noreturn]] void panic_here(std::source_location where = std::source_location::current());
    [[noreturn]] void assert_failed(const char* text, std::source_location where = std::source_location::current());

    // True if no ERR_NONFATAL (or worse) has been reported: the run can continue past this phase.
    bool terminate_after_phase() const { return erropt.worst >= ERR_NONFATAL; }

    // Tentative error hold: errors/warnings flagged ERR_HOLD (up to NONFATAL) are kept until the hold is popped. The
    // pop must be of the most recent push; with `issue` they are printed (or handed to the enclosing hold), else
    // dropped. Returns the highest severity issued or dropped (0 when handed on). Errors stronger than NONFATAL are
    // never held. Popping 0 does nothing.
    ErrHold error_hold_push();
    ErrFlags error_hold_pop(ErrHold hold, bool issue);

    // print_final_report: the "assembly completed/failed after 1+N+M passes" info line.
    void print_final_report(bool failure);

    // The "$ prefix for hexadecimal is deprecated" warning (warn_dollar_hex).
    void warn_dollar_hex();

private:
    // A message waiting on a hold, or being issued (struct nasm_errtext).
    struct ErrText {
        std::string msg;
        std::string filename;       // where.filename ("" for none)
        std::int32_t lineno = 0;    // where.lineno
        ErrFlags severity = 0;
        ErrFlags true_type = 0;
        int c_errno = 0;
    };
    struct ErrFormat {
        const char* beforeline;   // before the line number, if present
        const char* afterline;    // after the line number, if present
        const char* beforemsg;    // before the actual message
    };

    bool skip_this_pass(ErrFlags severity) const;
    bool is_suppressed(ErrFlags flags);
    ErrFlags true_error_type(ErrFlags severity) const;
    // The file and line a message is about (error_where); the file is a copy.
    std::pair<std::string, std::int32_t> error_where(ErrFlags severity) const;
    void write_line(ErrFlags severity, ErrFlags true_type, std::string line);
    void report_critical(ErrFlags severity, const std::string& message);
    void issue_error(ErrText& et);
    [[noreturn]] void die_hard(ErrFlags true_type, ErrFlags severity, const std::string& message);
    [[noreturn]] void never_returned();
    void init_warnings();
    void reset_warnings();

    PassType pass_type_ = PASS_INIT;
    std::int64_t passn_ = 0;
    std::unordered_map<std::int32_t, std::int64_t> offsets_;   // the current offset in each segment

    std::vector<ForwRefInfo> forwrefs_;
    std::size_t forwref_ = 0;   // the next forward reference to apply (== size: none)

    std::unordered_map<std::string, int> masm_types_;

    ErrFormat errfmt_;
    std::vector<std::array<std::uint8_t, NUM_WARNINGS>> warning_stack_;
    std::array<std::uint8_t, NUM_WARNINGS> warning_state_init_{};
    std::optional<std::vector<Diagnostic>> warn_list_;   // warnings buffered until an error or the final pass
    std::vector<std::vector<ErrText>> errhold_stack_;
    bool been_here_ = false;   // a critical error is being reported
    int exit_code_ = 0;
    std::string error_output_;
    std::vector<Diagnostic> diagnostics_;
};

}  // namespace cb::build::x86asm
