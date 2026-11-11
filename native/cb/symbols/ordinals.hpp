// cb/symbols/ordinals.hpp -- the name of a function that is imported by number.
//
//     const Ordinals ordinals(exports, shipped, spelling);
//     ordinals.name("USER", 91)            "GetDlgItem"
//     ordinals.name("MFC30", 800)          "??1CString@@QAE@XZ"     (demangle::readable() makes it a declaration)
//     ordinals.name("KERNEL", 99999)       nullopt
//     ordinals.source("USER", 91)          "user.exe"               where the name was read
//     ordinals.table("COMMDLG")            {ordinal: name}
//
// The 16-bit system modules, the MFC libraries and parts of SHELL32, COMCTL32 and KERNEL32 are called by number: the
// caller's file holds `USER.91`, not a name. Two places know the name:
//
//  1. the module that exports the function -- its own name tables say which name has which number. This is read from
//     the corpus and is the same binary the call reaches (the caller says how, through ModuleExports);
//  2. for exports the module itself leaves unnamed (every MFC function, the undocumented system ones), the definition
//     file or import library the module's maker shipped. Those are read once into ordinal_names.json
//     (`codebreaker names --ordinals FILE...`, collect() below), each entry kept only where the corpus module really
//     has an export with that number.
//
// 16-bit name tables are in capitals; a name is given as the Windows headers spell it when they declare it (the
// `spelling` function, prototypes::PrototypeTable::spelling). How many bytes of arguments a 16-bit import takes is
// symbols/api_arguments's business and is not repeated here.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::symbols::ordinals {

// "KERNEL32.dll" and "c:\\x\\kernel32.DLL" are the module KERNEL32: the file name in capitals without the extension
// of a module file (.dll .exe .drv .cpl .ocx).
std::string module_stem(std::string_view module);

// What the ordinal tables need to know of the modules of the corpus. The caller implements it over the format readers
// and the corpus (NE and PE export tables).
class ModuleExports {
public:
    virtual ~ModuleExports() = default;

    // The names a module's own tables give to its export numbers, and the file they were read from (its name, in lower
    // case). nullopt when the module is not in the corpus.
    struct Table {
        std::map<std::uint32_t, std::string> names;
        std::string file;
    };
    virtual std::optional<Table> table(std::string_view stem) const = 0;
};

// What collect() asks besides: every number a module exports (named or not), whether the code of an export takes the
// arguments a name says, and which modules the corpus imports by number.
class ExportCheck {
public:
    virtual ~ExportCheck() = default;

    // Every export number of the module (nullopt: it is not in the corpus).
    virtual std::optional<std::set<std::uint32_t>> numbers(std::string_view stem) const = 0;
    // Does the export with that number remove `argument_bytes` bytes of arguments on return? nullopt when the code does
    // not settle it. A table made for another release of a module gives itself away here: its names land on other
    // functions.
    virtual std::optional<bool> arguments_agree(std::string_view stem, std::uint32_t ordinal, std::uint32_t argument_bytes) const = 0;
    // The modules something in the corpus imports by number.
    virtual std::set<std::string> imported_by_number() const = 0;
};

// One module's entry in ordinal_names.json: where the names were read from, how many were checked against the code
// (agreeing, disagreeing), and the names by number.
struct ShippedModule {
    std::string from;
    std::uint32_t checked_yes = 0;
    std::uint32_t checked_no = 0;
    std::map<std::uint32_t, std::string> names;
};
using ShippedTables = std::map<std::string, ShippedModule>;   // by module name, in capitals

// ordinal_names.json: {"MODULE": {"from": "file", "checked": [yes, no], "names": {"number": "name"}}}. ParseError for
// text that is not that.
ShippedTables parse_shipped(std::string_view json_text);
// The file's text: keys sorted, numbers as text keys.
std::string format_shipped(const ShippedTables& tables);

// An export only documentation names: (name, where the documentation is).
struct Documented {
    std::string name;
    std::string where;
};
// The documented name of an export, for people to read. name() does not use them: the decompiler's names come from
// files only, and these would change what it writes.
std::optional<Documented> documented(std::string_view module, std::uint32_t ordinal);

// A function a module imports by number: the module as the importer wrote it (the caller capitalises or takes the stem
// as it needs), and the number.
struct ImportRef {
    std::string module;
    std::uint32_t ordinal = 0;
    friend auto operator<=>(const ImportRef&, const ImportRef&) = default;
};
struct NamedImport {
    ImportRef import;
    std::optional<std::string> name;
};

class Ordinals {
public:
    using Spelling = std::function<std::string(std::string_view)>;

    // `spelling` turns a name the tables hold in capitals into the headers' spelling (nothing given: names as they are).
    Ordinals(const ModuleExports& exports, ShippedTables shipped, Spelling spelling = {});

    // The export's name exactly as the tables hold it (decorated, capitals), or nullopt.
    std::optional<std::string> linker_name(std::string_view module, std::uint32_t ordinal) const;
    // The export's name: as the headers spell it when they know it, else as the tables hold it.
    std::optional<std::string> name(std::string_view module, std::uint32_t ordinal) const;
    // Where name() read it: the exporting file's name, or the definition file / library; "" when it is unknown.
    std::string source(std::string_view module, std::uint32_t ordinal) const;
    // {ordinal: name} -- everything known for one module (the corpus's names over the shipped ones).
    std::map<std::uint32_t, std::string> table(std::string_view module) const;
    // The name as a C identifier, or nullopt when it has none (unknown, or a C++ name).
    std::optional<std::string> identifier(std::string_view module, std::uint32_t ordinal) const;
    // Each function of `imports` (a module's imports by number) with its name, once each, in the order first met.
    std::vector<NamedImport> names_of(std::span<const ImportRef> imports) const;

private:
    struct FromFile {
        const std::map<std::uint32_t, std::string>* names;
        const std::string* from;
    };
    std::shared_ptr<const ModuleExports::Table> from_corpus(const std::string& stem) const;
    FromFile from_file(const std::string& stem) const;

    const ModuleExports& exports_;
    ShippedTables shipped_;
    Spelling spelling_;
    mutable std::mutex mutex_;
    mutable std::map<std::string, std::shared_ptr<const ModuleExports::Table>> corpus_;   // by stem: what the modules say, read once
};

// ---- reading definition files and import libraries (to make ordinal_names.json) --------------------------------

// {MODULE: {ordinal: name}} from the text of a linker definition file (`name @ number [NONAME]`; `LIBRARY name` names
// the module, else it is the file's own name, `module_from_file`).
std::map<std::string, std::map<std::uint32_t, std::string>> read_definition_file(std::string_view file_text,
                                                                                   std::string_view module_from_file);

// {MODULE: {ordinal: name}} of the functions an import library reaches by number. Reads the 32-bit form (an archive of
// import members) and the 16-bit one (import records). A damaged member or record is passed over.
std::map<std::string, std::map<std::uint32_t, std::string>> read_import_library(ByteView data);

// The files of `paths` (a folder gives its files, in the order of their names, skipping the folders of other
// processors: ALPHA, MIPS, PPC).
std::vector<std::filesystem::path> library_files(std::span<const std::filesystem::path> paths);

// Make ordinal_names.json from definition files and import libraries (files or folders; earlier ones win). Kept: names
// for numbers the corpus module exports without a name, and whole tables of modules the corpus does not hold but its
// modules import from. Each name is checked against the export it would name (arguments_agree); one that disagrees is
// dropped, and a table that disagrees for more than one function in twenty is dropped whole. `log` hears of each table
// used or refused.
ShippedTables collect(std::span<const std::filesystem::path> paths, const ModuleExports& exports, const ExportCheck& check,
                      const std::function<void(std::string_view)>& log = {});

}  // namespace cb::symbols::ordinals
