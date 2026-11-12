// cb/source/classes_writer.hpp -- the C++ classes of a module, written as class declarations.
//
//     classes_writer::write(classes, "comctl32.dll", "comctl32", out_dir, notes)
//         -> the file written (<stem>_classes.hpp), or nothing when the module shows no class
//
// A compiled C++ program keeps little of its classes, but not nothing: every class with a virtual function has a table of them, the
// code that installs a table in an object is the class's constructor or destructor, and MFC programs and programs built with run-time
// type information carry the classes' names and bases outright (analyze/classes finds all of this, and says for each fact where it came
// from). This writes what was found as C++:
//
//     class CMainFrame : public CFrameWnd {      // 92 virtual functions, object of 0x1a8 bytes
//     public:
//         virtual int4 vf_0();                    // GetRuntimeClass    [VERIFIED]
//         virtual int4 vf_1();                    // sub_004A10         [UNVERIFIED]
//         ...
//     };
//
// Each virtual function is tied to the reconstructed function that implements it (in <stem>.c) and carries that function's verdict. The
// file is a description in C++ syntax, to read beside the C: it is not compiled, the functions' real parameter lists are in the C, and a
// name the binary does not state is a placeholder (`class_<address of its table>`, `vf_<slot>`). What is inferred rather than stated is
// marked "probably".
//
// 32-bit Windows modules get everything analyze/classes finds. A PowerPC image gets its virtual tables (tables, in its data, of
// addresses of its own functions): the image carries no names. A function that is only a jump through an import (a class of MFC's has
// such functions in its table) is named for the import: ImportThunks.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core16_types.hpp"
#include "cb/formats/pe.hpp"
#include "cb/symbols/ordinals.hpp"

namespace cb::source::classes_writer {

using core16::ClassRecord;
using core16::Va;

// What the file says of each function a table names: the name it has in the written source, and its verdict's title ("" when it has none).
class FunctionNotes {
public:
    virtual ~FunctionNotes() = default;
    virtual std::string name_at(Va address) const = 0;
    virtual std::string status_at(Va address) const = 0;
};

// The file's text: a head, then each class as a declaration. `module_file` is the name of the module's file (the head says it), `stem` the
// name of the C file it is read beside. No final line end.
std::string classes_text(std::string_view module_file, std::string_view stem, std::span<const ClassRecord> classes, const FunctionNotes& notes);

// Write the text to <out_dir>/<stem>_classes.hpp. Nothing is written, and nothing is answered, for a module with no class.
std::optional<std::filesystem::path> write(std::span<const ClassRecord> classes, std::string_view module_file, std::string_view stem,
                                           const std::filesystem::path& out_dir, const FunctionNotes& notes);

// ---- PowerPC images -----------------------------------------------------------------------------------------------

// What the PowerPC image reader says of an image: the addresses of its functions, the range of its code, its sections, and the bytes of
// a section.
class PowerPcSource {
public:
    struct Section {
        std::string name;
        std::uint32_t va = 0;
        std::uint32_t size = 0;
    };
    virtual ~PowerPcSource() = default;
    virtual const std::set<std::uint32_t>& functions() const = 0;
    virtual std::optional<std::pair<std::uint32_t, std::uint32_t>> code() const = 0;   // [first, last)
    virtual std::span<const Section> sections() const = 0;
    virtual Bytes bytes_at(std::uint32_t va, std::uint32_t size) const = 0;
};

// Runs of two or more words in a PowerPC image's data that are each the address of one of its functions. The addresses kept in an
// extracted image's data are for the base it was linked at, which need not be where it is read to here; the table of function entries
// (.pdata) says by how much they differ: its first word is the first function's address, as linked.
std::vector<ClassRecord> powerpc_tables(const PowerPcSource& image);

// ---- jumps through an import -------------------------------------------------------------------------------------------

// The functions of a 32-bit module that are nothing but `jmp [slot]`, named for the import: LIBRARY.name, or LIBRARY.number.
class ImportThunks {
public:
    // The imports of the image itself (a damaged import table names no thunk).
    explicit ImportThunks(const formats::pe::Image& image);
    // The imports given (what a caller that has read them already, or a test, holds): the image supplies the code.
    ImportThunks(const formats::pe::Image& image, std::span<const formats::pe::Import> imports);

    // The import the function at `address` is a jump to, or nothing when it is not one. An import by number is named by `ordinals` where
    // it knows the function.
    std::optional<std::string> behind(Va address, const symbols::ordinals::Ordinals& ordinals) const;

private:
    const formats::pe::Image& image_;
    std::unordered_map<std::uint64_t, std::pair<std::string, std::string>> slots_;   // slot address -> (library, symbol); looked up only
};

}  // namespace cb::source::classes_writer
