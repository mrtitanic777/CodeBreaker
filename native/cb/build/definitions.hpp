// cb/build/definitions.hpp -- what a module offers and what it needs: exports, imports, and their source forms.
//
// A module's interface is read from the original and written three ways: a module-definition file (`.def`, the
// form a person reads and edits), linker directives (what the build passes to the linker), and a C header
// declaring the exported functions.
//
//     Interface iface = ...;                                    // the facts, read from the module
//     std::string def = definition_file(iface, "lz32.dll");     // the text of a .def
//     Directives d = export_directives(iface, symbol_at);        // linker lines, and why some exports are missing
//     std::string h = public_header(iface, "lz32.dll", declaration_at);   // the text of a C header
//
// The Interface is plain data: the readers that fill it from an NE or PE file (formats/ne, formats/pe, the
// corpus's notion of which lane a module is) belong to those modules, and a caller that has an Interface from
// anywhere can render it. How the linker is told (facts found the hard way):
//
//   * An export is tied to the function AT ITS ADDRESS, not to a function of the same name: the reconstruction's
//     name for a function need not be the exported name, and an export by ordinal has no name at all.
//     `symbol_at(export)` gives the object-file symbol there.
//   * A symbol holding `@`, `?` or `$` (a decorated __stdcall name, a C++ name) must be quoted: unquoted, the
//     linker reads `@` as "take directives from this file". Quoting is harmless for a plain symbol in an export,
//     so every symbol is quoted.
//   * The linker cannot export by ordinal alone: such an export gets a made-up name (`ordinal_N`), which the
//     build's check reports as an extra name.
//   * A 16-bit module is a library when its header says so (flag 0x8000), whatever its file is called --
//     KRNL386, USER and GDI are libraries named .EXE.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::build::definitions {

enum class ExportKind {
    Code,
    Data,
    Forward,   // `forward_target` names the function elsewhere
};

// One export. A 32-bit module's address is a virtual address; a 16-bit one's is segment:offset.
struct Export {
    std::uint32_t ordinal = 0;
    std::optional<std::string> name;   // absent: an export by ordinal only
    std::variant<std::uint64_t, Far16> address = std::uint64_t{0};
    ExportKind kind = ExportKind::Code;
    bool resident = false;             // 16-bit: the name is in the resident table
    std::string forward_target;
    bool shared_data = true;           // 16-bit: the loader gives it the module's data segment

    bool has_name() const { return name && !name->empty(); }
    // <export NAME @ORDINAL KIND>, for logs.
    std::string describe() const;
};

// What a module takes from another: (MODULE, a name or an ordinal).
struct Import {
    std::string module;
    std::variant<std::string, std::uint32_t> what;
    // The name, or the ordinal as decimal text: how the .def spells it and how its lines are ordered.
    std::string text() const;
};

enum class ModuleKind { Win16, Win32 };

// The facts a module states about its interface.
struct Interface {
    ModuleKind kind = ModuleKind::Win32;
    bool library = false;              // a DLL (a 16-bit module's header flag, not its file name)
    std::string name;                  // the name the module calls itself (the file's name when it has none)
    std::string description;           // 16-bit: the first non-resident name
    std::uint32_t heap = 0;            // 16-bit: heap and stack sizes
    std::uint32_t stack = 0;
    std::vector<Export> exports;       // by ordinal
    std::vector<Import> imports;       // each at most once
};

// The module's interface as a module-definition file. `file_name` is the module's file name, as the first
// comment line shows it (lower case).
std::string definition_file(const Interface& iface, std::string_view file_name);

// The object-file symbol for the thing an export points at, or nullopt (an empty text counts as none).
using SymbolAt = std::function<std::optional<std::string>(const Export&)>;
// The reconstruction's declaration of the function an export points at, or nullopt.
using DeclarationAt = std::function<std::optional<std::string>(const Export&)>;

struct MissingExport {
    Export what;
    std::string why;   // why it could not be exported
};

struct Directives {
    std::vector<std::string> lines;      // linker `export` lines for every export whose address has a symbol
    std::vector<MissingExport> missing;
};
Directives export_directives(const Interface& iface, const SymbolAt& symbol_at);

// A C header declaring the module's exported functions. `file_name` names the module's file, as above.
std::string public_header(const Interface& iface, std::string_view file_name, const DeclarationAt& declaration_at);

}  // namespace cb::build::definitions
