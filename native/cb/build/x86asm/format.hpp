// cb/build/x86asm/format.hpp -- the output-format interface: the C++ form of NASM's `struct ofmt` (and `struct dfmt`).
//
// An output format receives everything the encoder produces (`output`), every label defined (`symdef`), every section
// switch (`section`) and the directives only it understands (`directive`), and writes its file in `cleanup`. NASM's
// formats call into the rest of the assembler through globals (define_label, lookup_label, evaluate, seg_alloc, ofile);
// here a format is handed a `FormatHost` -- the services of the assembler around it -- when it is made, and keeps no
// state outside itself (two runs never share a format object).
//
// A format is made by name with `make_format` (outform.c: ofmt_find) and registered in format.cpp; format_bin.cpp,
// format_obj.cpp and format_coff.cpp each add theirs.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/directives.hpp"
#include "cb/build/x86asm/types.hpp"
#include "cb/core/bytes.hpp"

namespace cb::build::x86asm {

// The result of evaluating a critical expression at the front of a text (the evaluator call the OBJ common-variable
// element size makes): did it evaluate at all, is it a plain number (is_simple), its value, and how many characters of
// the text it consumed (stdscan_tell).
struct ExprAtFront {
    bool ok = false;
    bool simple = false;
    std::int64_t value = 0;
    std::size_t consumed = 0;
};

// A file the source depends on (the -M dependency list), with its MS-DOS packed date and time (obj_file_timestamp); a
// file whose time cannot be told is left out.
struct DependencyFile {
    std::string name;
    std::uint32_t dos_timestamp = 0;
};

// What a format may ask of the assembler around it (the globals NASM's formats use). Implemented by the driver.
class FormatHost {
public:
    virtual ~FormatHost() = default;

    virtual Assembly& assembly() = 0;

    // The primary output file (NASM's `ofile`): bytes written here are the file named by -o. A text format's lines go
    // here too (NASM opened it in text mode; on this platform that makes no difference to the bytes of a run that is
    // compared with nasm.exe's: "\n" is written as "\r\n" by a text-mode file on Windows -- see `text_mode`).
    virtual Bytes& ofile() = 0;
    // True when the format is a text format (OFMT_TEXT): the driver turns "\n" into "\r\n" when the run is on a platform
    // whose text files do (the reference nasm.exe ran on Windows).
    virtual bool ofile_is_text() const = 0;

    // Another output file by the name the source gave it (a map file): the bytes written to it come out in
    // Result::files under that name. "stdout" and "stderr" are the run's own streams (use stdout_write/stderr_write).
    // `text`: a text-mode file (NASM's NF_TEXT): the driver writes "\n" as "\r\n", as the reference nasm.exe did on Windows.
    // nullptr when the file cannot be opened.
    virtual Bytes* open_write(std::string_view name, bool text) = 0;
    virtual void stdout_write(std::string_view text) = 0;
    virtual void stderr_write(std::string_view text) = 0;

    // Evaluate an expression written as text with the standard scanner and the evaluator (stdscan_reset(text); evaluate(
    // stdscan, ...)): `critical` as the evaluator's argument. nullopt when it did not evaluate (the evaluator reported).
    virtual std::optional<Expr> evaluate_text(std::string_view text, int critical) = 0;

    // The label manager.
    virtual void define_label(std::string_view name, std::int32_t segment, std::int64_t offset, bool is_norm) = 0;
    // The type of a label and its segment and offset (LBL_none, with nothing set, for a label that does not exist).
    virtual LabelType lookup_label(std::string_view name, std::int32_t& segment, std::int64_t& offset) = 0;

    // A fresh segment number (segalloc.c: seg_alloc).
    virtual std::int32_t seg_alloc() = 0;

    // define_label for a special backend label (labels.c: backend_label): the format's `symdef` is called at once.
    virtual void backend_label(std::string_view name, std::int32_t segment, std::int64_t offset) = 0;
    // evaluate(stdscan, NULL, &tokval, NULL, critical = 1, NULL) on the front of `text`, saying how much of it was used.
    virtual ExprAtFront evaluate_critical(std::string_view text) = 0;
    // pp_pre_define: a `-d` style definition ("name=value").
    virtual void pre_define(std::string_view definition) = 0;
    // get_boolean_option (getbool.c): read an on/off/yes/no/true/false word or a constant expression from the front of `text`
    // into `value` (empty text is true; an error leaves `value` as it was).
    virtual void get_boolean_option(std::string_view text, bool& value) = 0;
    // The files the source depends on (empty: no dependency list was asked for).
    virtual std::vector<DependencyFile> dependency_files() { return {}; }
};

// A debug format (struct dfmt). Only the null one exists in x86asm (no format CodeBreaker uses writes debug
// information); the hooks are NASM's so one can be added.
class DebugFormat {
public:
    virtual ~DebugFormat() = default;
    virtual std::string_view fullname() const { return "Null"; }
    virtual std::string_view shortname() const { return "null"; }
    virtual void init() {}
    virtual void linenum(std::string_view filename, std::int32_t linenumber, std::int32_t segto) {
        (void)filename; (void)linenumber; (void)segto;
    }
    virtual void deflabel(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special) {
        (void)name; (void)segment; (void)offset; (void)is_global; (void)special;
    }
    virtual void debug_directive(std::string_view directive, std::string_view params) { (void)directive; (void)params; }
    virtual void typevalue(std::int32_t type) { (void)type; }
    virtual void output(int type, void* param) { (void)type; (void)param; }
    virtual void cleanup() {}
    virtual const std::vector<PragmaFacility>& pragmas() const;   // none
};

// The null debug format (null_debug_form), shared by every run (it has no state).
const DebugFormat& null_debug_format();

// An output format (struct ofmt). A format object is made per run.
class OutputFormat {
public:
    virtual ~OutputFormat() = default;

    virtual std::string_view fullname() const = 0;    // one-line description
    virtual std::string_view shortname() const = 0;   // the -f keyword
    virtual std::string_view extension() const = 0;   // default output extension, "" for none
    virtual unsigned flags() const = 0;               // OFMT_TEXT, OFMT_KEEP_ADDR, OFMT_ZERODATA
    virtual int maxbits() const = 0;                  // the largest segment size
    // The debug formats the format supports; the first is the default. (Always only the null one for now.)
    virtual std::vector<const DebugFormat*> debug_formats() const { return {&null_debug_format()}; }
    virtual const DebugFormat* default_dfmt() const { return &null_debug_format(); }

    // Extra standard macros the format supplies, as NASM's macros.pl compiled them from outbin.mac/outobj.mac/outcoff.mac:
    // one line each, comments and the `OUT:` line removed, runs of white space squeezed to one space.
    virtual const std::vector<std::string>& stdmac() const = 0;

    virtual void init() = 0;    // the start of an output session
    virtual void reset() {}     // the start of each pass
    virtual void output(const OutData& data) = 0;
    // A symbol is defined (`is_global`: 0 local, 1 global, 2 common (offset is the size), else the format's own). `name`
    // stays valid for the whole run.
    virtual void symdef(std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special) = 0;
    // A segment change by name -> its segment number (NO_SEG if there is no such), or the default section when `name`
    // is empty and `default_section` is true. `*bits` may be set to 16, 32 or 64 (or left alone).
    virtual std::int32_t section(std::optional<std::string_view> name, int& bits) = 0;
    // A label is defined at the current offset: the format may move it to another section. The default does not.
    virtual std::int32_t herelabel(std::string_view name, LabelType type, std::int32_t seg, std::int32_t& subsection, bool& copyoffset) {
        (void)name; (void)type; (void)subsection; (void)copyoffset;
        return seg;
    }
    virtual bool has_herelabel() const { return false; }   // NASM's `herelabel` pointer is NULL
    virtual void sectalign(std::int32_t seg, unsigned int value) { (void)seg; (void)value; }
    // SEG's segment base value (null_segbase: itself).
    virtual std::int32_t segbase(std::int32_t segment) { return segment; }
    // A directive for the format (`value` null: only ask whether it is supported -- %ifdirective).
    virtual DirectiveResult directive(Directive d, std::optional<std::string_view> value) { (void)d; (void)value; return DIRR_UNKNOWN; }
    // After assembly: write the file into host().ofile().
    virtual void cleanup() = 0;
    virtual const std::vector<PragmaFacility>& pragmas() const;   // none
};

// The formats: every format the engine knows, by shortname, plus the aliases (outform.c).
struct FormatName {
    std::string shortname;
    std::string fullname;
    std::string alias_of;   // "" for a format; else the shortname of the format an alias names
};

// A format by its -f name (case-blind) or alias; `alias` is set to the alias used (else empty). nullptr when unknown.
std::unique_ptr<OutputFormat> make_format(std::string_view name, FormatHost& host, std::string* alias = nullptr);
// The formats and aliases in NASM's listing order (ofmt_list).
std::vector<FormatName> list_formats();
// A debug format by name for an output format (dfmt_find).
const DebugFormat* find_debug_format(const OutputFormat& fmt, std::string_view name);

// Each format's factory; format.cpp calls these. `shortname` is the keyword the user gave (the factory makes the one object
// for that name; one factory serves a family):
//   format_bin.cpp   (the driver agent):      bin, ith, srec
//   format_obj.cpp   (the object-format agent): obj, obj2
//   format_coff.cpp  (the object-format agent): coff, win32, win64
// A factory returns nullptr for a name it does not serve.
std::unique_ptr<OutputFormat> make_bin_format(FormatHost& host, std::string_view shortname);
std::unique_ptr<OutputFormat> make_obj_format(FormatHost& host, std::string_view shortname);
std::unique_ptr<OutputFormat> make_coff_format(FormatHost& host, std::string_view shortname);

}  // namespace cb::build::x86asm
