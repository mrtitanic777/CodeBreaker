#ifndef XEXMAGIC_IMPORT_ANNOTATE_H_
#define XEXMAGIC_IMPORT_ANNOTATE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "import_names.h"
#include "ppc_disasm.h"

// Annotates disassembly with the kernel import each call reaches, e.g.
//
//   825698F4:  ...  bl    0x8256A494   ; NetDll_XNetStartup()
//
// Two call shapes are recognised:
//
//   direct    - `bl`/`b` straight at the import thunk. Exact, no guessing.
//   indirect  - the import's VALUE slot is loaded and called through ctr:
//                 lis  r11, 0x8200
//                 lwz  r11, 0x0654(r11)      ; <- slot address
//                 mtctr r11
//                 bctrl                      ; <- annotated here
//               Resolved by tracking lis/addis/addi/ori per register within a
//               function and remembering which import a register was loaded
//               from. This is a heuristic: it follows straight-line code and
//               forgets a register the moment anything else writes to it,
//               which is why a wrong name is far less likely than none.
//
// Annotation is applied in place to DisasmLine::text.
void AnnotateImportCalls(std::vector<DisasmLine>& lines,
                         const ImportNameTable& imports);

// Same for a single line, when the caller is streaming rather than holding the
// whole listing. Returns the comment to append (empty if none).
std::string ImportCommentForBranch(uint32_t target,
                                   const ImportNameTable& imports);

#endif  // XEXMAGIC_IMPORT_ANNOTATE_H_
