// cb/build/x86asm/preproc_masm.hpp -- the MASM front end of the preprocessor (preproc.c: masm_pp_xform and the masm_* state).
//
// In MASM mode (`--masm`) every source line goes through here before the NASM-language preprocessor sees it: MASM
// directives and syntax (MACRO/ENDM, IRP, REPT, STRUCT, PROC, SEGMENT, EQU, OPTION, IF/ENDIF, expression operators ...) are
// rewritten into NASM's own directives, and a rewrite may queue extra lines (a `%assign` for each constant, an `%ifndef`
// guard ...) that are handed out before the next line is read. The state it keeps (open structures, procedures, constants
// seen, macro blocks) lives in this object, one per preprocessor.
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "cb/build/x86asm/assembly.hpp"

namespace cb::build::x86asm {

class MasmFront {
public:
    // `skipping` says whether the preprocessor is inside a skipped conditional (false when not asked).
    MasmFront(Assembly& assembly, std::function<bool()> skipping);
    ~MasmFront();
    MasmFront(const MasmFront&) = delete;
    MasmFront& operator=(const MasmFront&) = delete;

    // Forget everything: the start of a pass.
    void reset();
    // The next queued extra line, if there is one (masm_ppq_get).
    std::optional<std::string> take_queued();
    // Rewrite one source line (masm_pp_xform); extra lines are queued.
    std::string xform(std::string line);
    // Look through a source file for the names it defines as constants, so a forward use is known (masm_prescan_consts).
    void prescan_consts(std::string_view text);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::build::x86asm
