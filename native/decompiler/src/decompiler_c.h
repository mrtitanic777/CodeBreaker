// XEXMagic - High-level C decompiler backend
//
// Turns the SSA IL (il.h) into IDA-style readable pseudocode, as opposed to
// codegen_rt360 which emits runnable static-recompilation C. The difference is
// the pass pipeline: dead-code elimination drops the flag/temporary noise that
// dominates PPC, expression propagation folds single-use definitions into
// nested expressions, memory accesses become typed derefs, and the CFG is
// structured back into if/else/while instead of one statement per instruction.

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "il.h"

struct DecompileCOptions {
  // Annotate each emitted statement with the PPC address it came from.
  bool addr_comments = false;
  // Skip expression propagation (one statement per IL instruction). Useful when
  // a folded expression looks wrong and the raw form is needed to check it.
  bool no_fold = false;
  // Skip dead-code elimination.
  bool no_dce = false;
  // Emit the raw disassembly for each block as a leading comment.
  bool show_asm = false;
  // Export ordinal for the annotation header (0 = not an export / unknown).
  uint32_t ordinal = 0;
};

// Emit IDA-style C pseudocode for one IL function.
std::string DecompileToC(const ILFunction& func,
                         const DecompileCOptions& opts = {});

// Resolve a global address to a symbol name, when known. Used to print
// `dword_841E1B30` style names for data references.
void SetGlobalSymbolMap(const std::map<uint32_t, std::string>& syms);

// Interactive struct-field renaming. Overrides the default `field_XX` name for
// a (base variable, offset) pair. Keyed by the base as the decompiler spells
// it (vN / aN). Applied to both field accesses and the emitted struct type.
// Cleared/replaced per call; pass {} to reset.
void SetFieldNames(
    const std::map<std::string, std::map<uint32_t, std::string>>& names);

// Explicit signature override for one function: return type, and per-argument
// type/name. Anything left empty falls back to inference. This is what lets a
// prototype known from documentation (HANDLE, PVOID, ...) be asserted, since
// usage alone can never recover those names.
struct FuncSignature {
  std::string ret;                                  // "" = infer
  std::map<int, std::pair<std::string, std::string>> args;  // pos -> (type, name)
};
void SetSignature(const FuncSignature* sig);

// Interactive struct unification. Overrides the synthesized struct tag for a
// base variable so multiple bases (across functions / parameters) can share one
// named type. Keyed by the base as the decompiler spells it (vN / aN). Pass {}
// to reset to the default per-variable tag.
void SetStructTags(const std::map<std::string, std::string>& tags);

// One inferred struct pointer base from the most recent DecompileToC call.
struct InferredField {
  uint32_t offset = 0;
  std::string ctype;  // C type spelling for the access width
  std::string name;   // current display name (override or field_XX)
};
struct InferredStruct {
  std::string base;       // base variable, as spelled by the decompiler (vN/aN)
  std::string type_name;  // synthesized struct tag (e.g. "V34_t")
  std::vector<InferredField> fields;
};
std::vector<InferredStruct> GetInferredStructs();

// A local that holds a copy of an incoming argument. Compilers park a pointer
// parameter in a callee-saved register and dereference that, so the base the
// struct inference sees is the local, not the parameter - and any type known
// for the parameter has to travel across that copy to be of use.
struct BaseParamOrigin {
  std::string base;  // local as the decompiler spells it (vN)
  int position = 0;  // 1-based argument position it was copied from
};
std::vector<BaseParamOrigin> GetBaseParamOrigins();

// Forces the declared type of a base variable (arg aN / local vN), overriding
// the int default. Applied only where no local pointer/struct evidence exists,
// so interprocedural inference fills gaps without fighting local inference.
// Pass {} to reset.
void SetTypeOverrides(const std::map<std::string, std::string>& types);

// Base variable -> the declaration text to print for it. Set for bases bound
// to a type the SDK headers declare, so the output carries the real struct
// instead of a reconstruction from the offsets the code happened to touch.
void SetStructDecls(const std::map<std::string, std::string>& decls);

// Callee name -> its declaration, printed as a preamble so the emitted C
// stands on its own. The caller supplies the real prototype where the SDK
// declares one; anything else gets a plausible one built from the call.
void SetCalleeDecls(const std::map<std::string, std::string>& decls);

// Every function called by the most recent render, with the widest argument
// count seen at any of its call sites. Lets the caller build declarations for
// exactly the callees that appear.
std::map<std::string, int> GetCalledFunctions();

// One argument-flow fact from the most recent DecompileToC: a plain
// variable (aN / vN) passed to a call at a given position. Lets the caller
// infer a type from the callee's known parameter types (data passed to the
// routine's callees), and vice-versa across the call graph.
struct ArgFlow {
  std::string callee;        // resolved callee name
  uint32_t callee_target;    // callee address (0 if indirect)
  int position = 0;          // argument position (0 = first / r3)
  std::string arg;           // the variable passed (aN / vN)
  // Byte offset added to `arg` at the call, for `arg + 0xNN`. Non-zero means
  // the callee's parameter type describes what lives at that offset inside
  // `arg`, not `arg` itself.
  uint32_t offset = 0;
};
std::vector<ArgFlow> GetArgFlows();

// String literals by address. When a constant in the code matches one, it is
// rendered as the literal instead of a bare number, and the reference is
// recorded so string cross-references can be built. Pointer is borrowed and
// must outlive decompilation; pass nullptr to disable.
void SetStringMap(const std::map<uint32_t, std::string>* strings);

// Addresses of string literals referenced by the most recent DecompileToC.
std::vector<uint32_t> GetStringRefs();

// A struct bound to a fixed address. Absolute-address accesses that land
// inside it render as `name.field` instead of `*(T *)(0x...)`, which is how
// scattered globals get recognised as one object.
struct GlobalStruct {
  uint32_t addr = 0;
  uint32_t size = 0;
  std::string name;
  std::map<uint32_t, std::string> fields;  // offset -> field name
};

// Borrowed pointer; must outlive decompilation. nullptr disables.
void SetGlobalStructs(const std::vector<GlobalStruct>* gs);

// An absolute-address memory access seen while rendering. The address is the
// FOLDED constant (lis/addi already combined), which the raw IL does not have,
// and `write` distinguishes a store from a load.
struct DataRef {
  uint32_t addr = 0;
  bool write = false;
};
std::vector<DataRef> GetDataRefs();

// Offsets of each bound global actually touched by the most recent
// DecompileToC, keyed by the global's base address.
std::map<uint32_t, std::map<uint32_t, std::string>> GetGlobalFieldUses();
