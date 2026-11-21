// build/masm.hpp -- assemble reconstructed assembly with the period MASM.
//
//     codebreaker masm [file.asm] [--selftest]
//
//     masm::Assembler assembler(masm::Context{tools, dos, &cache});
//     Product p = assembler.assemble(source);                                      // .obj, .lst, .log
//     Finished many = assembler.assemble_many({{"F0", src0}, {"F1", src1}});
//     auto functions = assembler.functions(source);                                // {name: bytes, fix-up mask}
//
// Much of KERNEL/USER/GDI was hand-written assembly, and assembly can only be proven byte-exact by RE-ASSEMBLING it and
// diffing the bytes: translating it to C never reproduces the original encoding. MASM is a 16-bit DOS program (there is no
// NTVDM on 64-bit Windows), so this runs it on CodeBreaker's own DOS executive (build/native_dos), in-process. A run the
// executive cannot complete throws a ToolError that says why. MASM emits an OMF `.obj` exactly like the C compilers, so
// everything downstream is shared: formats/omf pulls the code bytes and the relocation mask. This module only runs the
// assembler.
//
// `masm_root` selects the assembler: the default is MASM 4.00 (16-bit, proven byte-exact for the NE core); an override such
// as the vendored MASM 5.10A (32-bit `.386` / USE32) is what the PE path needs.
//
// Assembling is deterministic -- the same source, assembler and INCLUDE/LIB files give the same bytes -- so an Assembler
// given a cache keeps what it made (kind "masm", keyed by the source, the stem and the bytes of every file under the
// assembler's folder), and a call whose sources are all kept runs no program at all.
//
// Nothing here is global: the tools, the executive and the cache belong to the caller.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/cache.hpp"
#include "base/config.hpp"
#include "build/native_dos.hpp"
#include "core/cli.hpp"
#include "core/ordered_map.hpp"
#include "formats/omf.hpp"

namespace cb::build::masm {

using native_dos::Finished;
using native_dos::Product;
using native_dos::Source;

// Where the assembler is (settings.json: masm).
struct Tools {
    std::filesystem::path masm;       // the default assembler's folder: BIN/MASM.EXE, INC, LIB

    static Tools from_config(const config::Config& config);
};

// What an Assembler needs: the tools, the DOS executive that runs MASM, and (optional) the cache.
struct Context {
    Tools tools;
    const native_dos::DosExecutive& dos;
    base::cache::Cache* cache = nullptr;
};

class Assembler {
public:
    explicit Assembler(Context context) : context_(std::move(context)) {}
    Assembler(const Assembler&) = delete;
    Assembler& operator=(const Assembler&) = delete;

    // `source` assembled: the object, the listing and the log MASM wrote (a part it did not make is nullopt). `root` replaces
    // the default assembler folder. NotFoundError when MASM.EXE is not under the folder; native_dos::NativeDosError (with its
    // reason) when the executive cannot complete the run.
    Product assemble(std::string_view source, std::string_view stem = "t",
                     const std::optional<std::filesystem::path>& root = std::nullopt);

    // Many (stem, source) pairs, using the cache: a source already kept is served at once, only the rest is assembled (and
    // then kept). The results are in the order the sources were given. Stems must be unique (UsageError otherwise) and DOS
    // 8.3-safe (callers use F0..FN).
    Finished assemble_many(std::span<const Source> items, const std::optional<std::filesystem::path>& root = std::nullopt);

    // The public functions of `source`'s object with their bytes and fix-up masks (formats/omf). ToolError when MASM made no
    // object (its log, cut to 600 characters, is in the message).
    OrderedMap<std::string, formats::omf::FunctionBytes> functions(std::string_view source);

private:
    void require_executive() const;

    Context context_;
};

// MASM's own selftest program: a proc `_addasm` that pushes bp, adds two arguments and returns.
extern const std::string_view kSelfTestSource;

// Assemble kSelfTestSource and check the bytes of `_addasm`; the account of it goes to `report`. True when it is byte-exact.
bool selftest(Assembler& assembler, std::string& report);

// `masm [file.asm] [--selftest]`: assemble a file and list the segments of its object, or run the selftest. Exit code 1
// when nothing was made.
cli::Command command(const Context& context);

}  // namespace cb::build::masm
