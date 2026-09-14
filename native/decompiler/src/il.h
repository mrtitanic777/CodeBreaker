// XEXMagic - Intermediate Language (IL) definitions
// Every PPC instruction maps to a typed, SSA-style IL instruction.
// No "Unknown" opcode — complete coverage of all 176 PPC mnemonics.

#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// IL Types
// ---------------------------------------------------------------------------

enum class ILType {
  Void,
  Bool,
  U8,
  U16,
  U32,
  S8,
  S16,
  S32,
  U64,
  S64,
  F32,
  F64,
  Ptr,
  // A whole VMX register. Which one is used depends on how the instruction
  // that produced it interprets the 128 bits, so the C output says what the
  // hardware was actually doing rather than calling everything a vector.
  V128U,  // four unsigned integer lanes
  V128S,  // four signed integer lanes
  V128F,  // four floats
};

inline bool ILTypeIsVector(ILType t) {
  return t == ILType::V128U || t == ILType::V128S || t == ILType::V128F;
}

inline const char* ILTypeName(ILType t) {
  switch (t) {
    case ILType::Void: return "void";
    case ILType::Bool: return "bool";
    case ILType::U8:   return "uint8_t";
    case ILType::U16:  return "uint16_t";
    case ILType::U32:  return "uint32_t";
    case ILType::S8:   return "int8_t";
    case ILType::S16:  return "int16_t";
    case ILType::S32:  return "int32_t";
    case ILType::U64:  return "uint64_t";
    case ILType::S64:  return "int64_t";
    case ILType::F32:  return "float";
    case ILType::F64:  return "double";
    case ILType::Ptr:  return "uintptr_t";
    case ILType::V128U: return "uint128_t";
    case ILType::V128S: return "int128_t";
    case ILType::V128F: return "float128_t";
  }
  return "uint32_t";
}

inline bool ILTypeIsFloat(ILType t) {
  return t == ILType::F32 || t == ILType::F64;
}

inline bool ILTypeIsInteger(ILType t) {
  return !ILTypeIsFloat(t) && t != ILType::Void && t != ILType::Bool;
}

// ---------------------------------------------------------------------------
// IL Values (SSA)
// ---------------------------------------------------------------------------

enum { kILNoValue = -1 };
enum { kILRegNone = -1 };

struct ILValue {
  int id = kILNoValue;     // unique SSA ID
  ILType type = ILType::U32;
  int ppc_reg = kILRegNone; // source register: 0-31 GPR, 32-63 FPR, -1 none
  std::string name;         // assigned by var_allocator
};

inline ILValue ILNoVal() {
  return {kILNoValue, ILType::Void, kILRegNone, {}};
}

// ---------------------------------------------------------------------------
// IL Opcodes — Complete PPC coverage
// ---------------------------------------------------------------------------

enum class ILOp {
  // Integer arithmetic
  Assign,          // dst = src1
  Add,             // dst = src1 + src2
  AddCarry,        // dst = src1 + src2 + carry
  Sub,             // dst = src1 - src2
  SubCarry,        // dst = src1 - src2 - borrow
  Mul,             // dst = src1 * src2 (low 32)
  MulHigh,         // dst = (src1 * src2) >> 32 (signed)
  MulHighU,        // dst = (src1 * src2) >> 32 (unsigned)
  Div,             // dst = src1 / src2 (signed)
  DivU,            // dst = src1 / src2 (unsigned)
  Neg,             // dst = -src1

  // Logic
  And,
  Or,
  Xor,
  Nand,
  Nor,
  Orc,             // dst = src1 | ~src2
  Andc,            // dst = src1 & ~src2
  Eqv,             // dst = ~(src1 ^ src2)
  Not,             // dst = ~src1

  // Shift/Rotate
  Shl,             // dst = src1 << src2
  Shr,             // dst = src1 >> src2 (logical)
  Sar,             // dst = src1 >> src2 (arithmetic)
  SarImm,          // dst = src1 >> imm (arithmetic)
  RotateAndMask,   // rlwinm: dst = rotl(src1, imm) & mask(imm2)
  RotateInsert,    // rlwimi: insert rotated bits into dst
  RotateAndMaskReg,// rlwnm: dst = rotl(src1, src2) & mask(imm2)
  CountLeadingZeros,

  // Extend
  ExtendByte,      // dst = (int8_t)src1
  ExtendHalf,      // dst = (int16_t)src1
  ExtendWord,      // dst = (int32_t)src1

  // Memory - integer
  Load,            // dst = *(type*)(src1 + imm)
  Store,           // *(type*)(src1 + imm) = src2
  LoadIndexed,     // dst = *(type*)(src1 + src2)
  StoreIndexed,    // *(type*)(src1 + src2) = src3
  LoadUpdate,      // dst = *(type*)(src1 + imm); src1 += imm
  StoreUpdate,     // *(type*)(src1 + imm) = src2; src1 += imm
  LoadMultiple,    // lmw
  StoreMultiple,   // stmw
  LoadReserved,    // lwarx
  StoreConditional,// stwcx.
  LoadByteReverse, // lwbrx/lhbrx
  StoreByteReverse,// stwbrx/sthbrx

  // Floating point arithmetic
  FAdd,
  FSub,
  FMul,
  FDiv,
  FMulAdd,         // dst = src1 * src2 + src3
  FMulSub,         // dst = src1 * src2 - src3
  FNMulAdd,        // dst = -(src1 * src2 + src3)
  FNMulSub,        // dst = -(src1 * src2 - src3)
  FNeg,
  FAbs,
  FNAbs,
  FMove,
  FRecipEst,       // fres
  FRSqrtEst,       // frsqrte
  FSqrt,           // fsqrt, fsqrts
  FSelect,         // fsel: dst = (src1 >= 0) ? src2 : src3

  // Float convert
  FRoundSingle,    // frsp
  FConvToIntWord,  // fctiw
  FConvToIntWordTrunc, // fctiwz
  FConvToIntDbl,   // fctid
  FConvToIntDblTrunc,  // fctidz
  FConvFromInt,    // fcfid

  // Float memory
  FLoad,           // dst = *(float/double*)(src1 + imm)
  FStore,          // *(float/double*)(src1 + imm) = src2
  FLoadIndexed,
  FStoreIndexed,
  FLoadUpdate,
  FStoreUpdate,

  // Float special store
  FStoreIntWord,      // stfiwx: store FPR low 32 bits as integer

  // Float compare
  FCompareOrdered,    // fcmpo
  FCompareUnordered,  // fcmpu

  // Integer compare
  Compare,         // signed compare src1, src2
  CompareU,        // unsigned compare src1, src2
  CompareImm,      // signed compare src1, imm
  CompareImmU,     // unsigned compare src1, imm

  // Condition register
  CRAnd,
  CROr,
  CRXor,
  CRNand,
  CRNor,
  CRAndc,
  CROrc,
  CREqv,
  MoveCRField,     // mcrf

  // Branch / Control flow
  Branch,          // unconditional goto
  BranchCond,      // conditional goto
  BranchCTR,       // branch to CTR (tail call)
  CallCTR,         // call through CTR (bctrl — sets LR)
  BranchLR,        // branch to LR (non-return)
  BranchDecrCTRZero,    // bdz
  BranchDecrCTRNonZero, // bdnz
  Call,            // bl - function call
  Return,          // blr (unconditional return)

  // SPR / CR
  MoveSPR,         // mfspr, mtspr
  LoadTimeBase,    // mftb: read 64-bit time base counter (SPR 268/269)
  MoveCR,          // mfcr, mtcrf
  MoveMSR,         // mfmsr, mtmsr

  // System / Sync
  Syscall,
  Sync,
  LWSync,
  PTESync,
  ISync,
  EIEIO,

  // Cache
  DCacheFlush,
  DCacheStore,
  DCacheTouch,
  DCacheTouchStore,
  DCacheZero,
  ICacheInvalidate,

  // Trap
  Trap,
  TrapImm,

  // FPSCR
  LoadFPSCR,         // mffs: read FPSCR to FPR
  StoreFPSCR,        // mtfsf: write FPR bits to FPSCR fields

  // VMX / AltiVec
  VMXOp,             // generic VMX instruction (asm_text has details)
  VMXLoad,           // lvx: load 16-byte vector from aligned address
  VMXStore,          // stvx: store 16-byte vector to aligned address
  VMXLoad128,        // VMX128 load (opcodes 5/6 with Xbox 360 extensions)
  VMXStore128,       // VMX128 store

  // Stack frame setup (special: stwu r1, -N(r1))
  StackAlloc,

  // One VMX / VMX128 instruction, kept as an operation on whole registers
  // rather than modelled lane by lane. call_name carries the mnemonic, so it
  // reads as the intrinsic the compiler emitted it from - and, unlike inline
  // asm, it defines its destination, so the dataflow stays connected.
  VectorOp,

  Nop,

  // x86 setcc: dst = (src1 <cmp_kind> src2)  -- a relational as a VALUE (unlike
  // Compare, whose operator lives on the consuming branch).
  SetCond,

  // Last resort — the disassembler decoded it but our IL builder
  // hasn't mapped it yet. Emitted as an inline asm comment.
  InlineAsm,
};

inline const char* ILOpName(ILOp op) {
  switch (op) {
    case ILOp::Assign: return "assign";
    case ILOp::Add: return "add";
    case ILOp::AddCarry: return "addc";
    case ILOp::Sub: return "sub";
    case ILOp::SubCarry: return "subc";
    case ILOp::Mul: return "mul";
    case ILOp::MulHigh: return "mulhi";
    case ILOp::MulHighU: return "mulhiu";
    case ILOp::Div: return "div";
    case ILOp::DivU: return "divu";
    case ILOp::Neg: return "neg";
    case ILOp::And: return "and";
    case ILOp::Or: return "or";
    case ILOp::Xor: return "xor";
    case ILOp::Nand: return "nand";
    case ILOp::Nor: return "nor";
    case ILOp::Orc: return "orc";
    case ILOp::Andc: return "andc";
    case ILOp::Eqv: return "eqv";
    case ILOp::Not: return "not";
    case ILOp::Shl: return "shl";
    case ILOp::Shr: return "shr";
    case ILOp::Sar: return "sar";
    case ILOp::SarImm: return "sari";
    case ILOp::RotateAndMask: return "rlwinm";
    case ILOp::RotateInsert: return "rlwimi";
    case ILOp::RotateAndMaskReg: return "rlwnm";
    case ILOp::CountLeadingZeros: return "cntlzw";
    case ILOp::ExtendByte: return "extsb";
    case ILOp::ExtendHalf: return "extsh";
    case ILOp::ExtendWord: return "extsw";
    case ILOp::Load: return "load";
    case ILOp::Store: return "store";
    case ILOp::LoadIndexed: return "loadx";
    case ILOp::StoreIndexed: return "storex";
    case ILOp::LoadUpdate: return "loadu";
    case ILOp::StoreUpdate: return "storeu";
    case ILOp::LoadMultiple: return "lmw";
    case ILOp::StoreMultiple: return "stmw";
    case ILOp::LoadReserved: return "lwarx";
    case ILOp::StoreConditional: return "stwcx";
    case ILOp::LoadByteReverse: return "lwbrx";
    case ILOp::StoreByteReverse: return "stwbrx";
    case ILOp::FAdd: return "fadd";
    case ILOp::FSub: return "fsub";
    case ILOp::FMul: return "fmul";
    case ILOp::FDiv: return "fdiv";
    case ILOp::FMulAdd: return "fmadd";
    case ILOp::FMulSub: return "fmsub";
    case ILOp::FNMulAdd: return "fnmadd";
    case ILOp::FNMulSub: return "fnmsub";
    case ILOp::FNeg: return "fneg";
    case ILOp::FAbs: return "fabs";
    case ILOp::FNAbs: return "fnabs";
    case ILOp::FMove: return "fmr";
    case ILOp::FRecipEst: return "fres";
    case ILOp::FRSqrtEst: return "frsqrte";
    case ILOp::FSqrt: return "fsqrt";
    case ILOp::FSelect: return "fsel";
    case ILOp::FRoundSingle: return "frsp";
    case ILOp::FConvToIntWord: return "fctiw";
    case ILOp::FConvToIntWordTrunc: return "fctiwz";
    case ILOp::FConvToIntDbl: return "fctid";
    case ILOp::FConvToIntDblTrunc: return "fctidz";
    case ILOp::FConvFromInt: return "fcfid";
    case ILOp::FLoad: return "fload";
    case ILOp::FStore: return "fstore";
    case ILOp::FLoadIndexed: return "floadx";
    case ILOp::FStoreIndexed: return "fstorex";
    case ILOp::FLoadUpdate: return "floadu";
    case ILOp::FStoreUpdate: return "fstoreu";
    case ILOp::FStoreIntWord: return "stfiwx";
    case ILOp::FCompareOrdered: return "fcmpo";
    case ILOp::FCompareUnordered: return "fcmpu";
    case ILOp::Compare: return "cmp";
    case ILOp::CompareU: return "cmpu";
    case ILOp::CompareImm: return "cmpi";
    case ILOp::CompareImmU: return "cmpui";
    case ILOp::CRAnd: return "crand";
    case ILOp::CROr: return "cror";
    case ILOp::CRXor: return "crxor";
    case ILOp::CRNand: return "crnand";
    case ILOp::CRNor: return "crnor";
    case ILOp::CRAndc: return "crandc";
    case ILOp::CROrc: return "crorc";
    case ILOp::CREqv: return "creqv";
    case ILOp::MoveCRField: return "mcrf";
    case ILOp::Branch: return "br";
    case ILOp::BranchCond: return "brc";
    case ILOp::BranchCTR: return "bctr";
    case ILOp::CallCTR: return "bctrl";
    case ILOp::BranchLR: return "blr";
    case ILOp::BranchDecrCTRZero: return "bdz";
    case ILOp::BranchDecrCTRNonZero: return "bdnz";
    case ILOp::Call: return "call";
    case ILOp::Return: return "ret";
    case ILOp::MoveSPR: return "mtspr";
    case ILOp::LoadTimeBase: return "mftb";
    case ILOp::MoveCR: return "mtcr";
    case ILOp::MoveMSR: return "mtmsr";
    case ILOp::Syscall: return "sc";
    case ILOp::Sync: return "sync";
    case ILOp::LWSync: return "lwsync";
    case ILOp::PTESync: return "ptesync";
    case ILOp::ISync: return "isync";
    case ILOp::EIEIO: return "eieio";
    case ILOp::DCacheFlush: return "dcbf";
    case ILOp::DCacheStore: return "dcbst";
    case ILOp::DCacheTouch: return "dcbt";
    case ILOp::DCacheTouchStore: return "dcbtst";
    case ILOp::DCacheZero: return "dcbz";
    case ILOp::ICacheInvalidate: return "icbi";
    case ILOp::LoadFPSCR: return "mffs";
    case ILOp::StoreFPSCR: return "mtfsf";
    case ILOp::VMXOp: return "vmx";
    case ILOp::VMXLoad: return "lvx";
    case ILOp::VMXStore: return "stvx";
    case ILOp::VMXLoad128: return "lvx128";
    case ILOp::VMXStore128: return "stvx128";
    case ILOp::Trap: return "tw";
    case ILOp::TrapImm: return "twi";
    case ILOp::StackAlloc: return "stackalloc";
    case ILOp::VectorOp: return "vectorop";
    case ILOp::Nop: return "nop";
    case ILOp::SetCond: return "setcc";
    case ILOp::InlineAsm: return "asm";
  }
  return "???";
}

// ---------------------------------------------------------------------------
// Comparison kind (for Compare + BranchCond pairing)
// ---------------------------------------------------------------------------

enum class ILCmpKind {
  EQ, NE, LT, GT, LE, GE,   // signed
  LTU, GTU, LEU, GEU,        // unsigned
};

inline const char* ILCmpKindStr(ILCmpKind k) {
  switch (k) {
    case ILCmpKind::EQ: return "==";
    case ILCmpKind::NE: return "!=";
    case ILCmpKind::LT: return "<";
    case ILCmpKind::GT: return ">";
    case ILCmpKind::LE: return "<=";
    case ILCmpKind::GE: return ">=";
    case ILCmpKind::LTU: return "<";   // unsigned, but same C operator
    case ILCmpKind::GTU: return ">";
    case ILCmpKind::LEU: return "<=";
    case ILCmpKind::GEU: return ">=";
  }
  return "??";
}

inline bool ILCmpIsUnsigned(ILCmpKind k) {
  return k >= ILCmpKind::LTU;
}

// ---------------------------------------------------------------------------
// SPR identifiers
// ---------------------------------------------------------------------------

enum class ILSPR {
  LR = 8,
  CTR = 9,
  XER = 1,
  VRSAVE = 256,
  PVR = 287,
  Other = 0,
};

// ---------------------------------------------------------------------------
// IL Instruction
// ---------------------------------------------------------------------------

struct ILInstr {
  ILOp op = ILOp::Nop;

  ILValue dst;             // destination SSA value
  ILValue src1, src2;      // source SSA values
  ILValue src3;            // third source (fmadd, fsel, rlwimi, store-indexed)

  ILType access_type = ILType::U32;  // for Load/Store: memory width
  uint32_t address = 0;              // source PPC address
  uint32_t target = 0;               // branch/call target address
  int32_t imm = 0;                   // immediate (displacement, shift amount)
  uint32_t mask = 0;                 // computed mask for rlwinm/rlwimi
  uint32_t ppc_word = 0;             // raw PPC instruction word (for cycle counter)

  std::string call_name;   // resolved function name for Call
  std::string asm_text;    // for InlineAsm: raw disassembly text

  // Inserted when leaving SSA, to give a merge point one value per register.
  // These are not instructions the CPU executes, so a reader should never see
  // them: both sides are the same variable in the original source.
  bool merge_move = false;

  // Condition info
  ILCmpKind cmp_kind = ILCmpKind::EQ;
  int cmp_ref = kILNoValue; // SSA id of Compare result this branch references
  int cr_field = 0;         // condition register field (0-7)

  // For MoveSPR: which SPR
  ILSPR spr = ILSPR::Other;
  bool spr_read = false;   // true = mfspr (read), false = mtspr (write)

  // Flags for carry/overflow/record
  bool set_ca = false;     // instruction sets XER[CA] (addc, subfc, addic, srawi)
  bool set_ov = false;     // instruction sets XER[OV/SO] (addo, subfo, etc.)
  bool rc = false;         // Rc bit — instruction updates CR0 (dot-forms: add., subf.)
  bool is_cond_return = false;  // conditional bclr — BranchCond that returns, not calls
  bool is_cond_ctr = false;     // conditional bcctr — BranchCond that calls/branches through CTR
  bool is_cond_ctr_link = false; // conditional bcctr with LK=1 (bctrl) — sets LR before call

  // Cycle cost (filled by xenon_cycles)
  int cycles = 0;
};

// ---------------------------------------------------------------------------
// IL Basic Block
// ---------------------------------------------------------------------------

struct ILBasicBlock {
  uint32_t start_addr = 0;
  uint32_t end_addr = 0;
  int block_id = 0;
  std::vector<ILInstr> instrs;
  std::vector<uint32_t> successors;
  bool is_return = false;
  int total_cycles = 0;
};

// ---------------------------------------------------------------------------
// IL Function
// ---------------------------------------------------------------------------

struct ILFunction {
  std::string name;
  uint32_t entry = 0;
  uint32_t end = 0;
  uint32_t stack_frame_size = 0;

  std::vector<ILValue> params;
  ILType return_type = ILType::Void;

  std::vector<ILBasicBlock> blocks;

  // SSA value pool
  std::vector<ILValue> values;
  int next_value_id = 0;

  // Called functions (for forward declarations)
  std::vector<std::string> called_functions;

  // Cycle count summary
  int total_cycles = 0;
  int hot_path_cycles = 0;

  // Instruction category breakdown for cycle report
  int count_alu = 0, cycles_alu = 0;
  int count_load = 0, cycles_load = 0;
  int count_store = 0, cycles_store = 0;
  int count_fp = 0, cycles_fp = 0;
  int count_branch = 0, cycles_branch = 0;
  int count_mul = 0, cycles_mul = 0;
  int count_div = 0, cycles_div = 0;
  int count_system = 0, cycles_system = 0;

  // Validation
  bool validated = false;
  std::vector<std::string> validation_errors;
  std::vector<std::string> validation_warnings;
  int total_instructions = 0;
  int inline_asm_count = 0;

  // Allocate a new SSA value
  ILValue AllocValue(ILType type, int ppc_reg = kILRegNone) {
    ILValue v;
    v.id = next_value_id++;
    v.type = type;
    v.ppc_reg = ppc_reg;
    values.push_back(v);
    return v;
  }
};

// ---------------------------------------------------------------------------
// CodeGen Configuration
// ---------------------------------------------------------------------------

struct CodeGenConfig {
  bool cpp_mode = false;
  std::string namespace_name;
  bool emit_cycle_comments = true;
  bool emit_validation = true;
  bool emit_address_comments = false;
  bool rt360_mode = false;
  bool rt360_compile = false;  // auto-compile .cpp → .dll via toolchain
  std::string rt360_module_name;

  // PPC save/restore stub addresses (detected from XEX binary).
  // __savegprlr_14 at gplr_save_start, +4 per register up to _31.
  // __restgprlr_14 at gplr_rest_start, +4 per register up to _31.
  // 0 = not detected.
  uint32_t gplr_save_start = 0;
  uint32_t gplr_rest_start = 0;
  uint32_t fpr_save_start = 0;
  uint32_t fpr_rest_start = 0;
  bool trace_mode = false;  // --trace: emit register dump after every instruction

  // Import thunk addresses — calls to these emit direct _thunk_XXXXXXXX()
  // instead of rt360_call() to avoid volatile register clobbering.
  std::set<uint32_t> import_thunk_addrs;
};

// ---------------------------------------------------------------------------
// RT360 Guest Memory Init Data
// Baked at decompile time from the XEX — no XEX needed at runtime.
// ---------------------------------------------------------------------------

struct RT360DataSection {
  std::string name;          // ".rdata", ".data", ".pdata", etc.
  uint32_t virtual_address;  // VA relative to image base (offset into guest memory)
  uint32_t virtual_size;
  uint32_t characteristics;
  std::vector<uint8_t> data; // Raw section bytes
};

struct RT360ImportThunk {
  uint32_t thunk_address;    // Guest address of the import thunk
  uint32_t value_address;    // Guest address of the import value slot
  std::string module_name;   // "xboxkrnl.exe" or "xam.xex"
  uint32_t ordinal;
};

struct RT360TLSInfo {
  uint32_t slot_count = 0;
  uint32_t raw_data_address = 0;  // Original VA in XEX
  uint32_t data_size = 0;
  uint32_t raw_data_size = 0;
  std::vector<uint8_t> template_data;  // TLS initial data bytes
};

struct RT360XexHeaderField {
  uint32_t key;
  uint32_t value;
};

struct RT360GuestInitData {
  // Module identity
  std::string module_name;
  uint32_t base_address = 0;
  uint32_t image_size = 0;
  uint32_t entry_point = 0;
  uint32_t title_id = 0;
  uint32_t default_stack_size = 0;

  // Full decompressed PE image (sections mapped to VAs)
  std::vector<uint8_t> xex_image;

  // PE sections (data only — code is in the DLL)
  std::vector<RT360DataSection> data_sections;

  // Import thunks
  std::vector<RT360ImportThunk> import_thunks;

  // TLS
  RT360TLSInfo tls_info;

  // XEX header fields (for RtlImageXexHeaderField queries)
  std::vector<RT360XexHeaderField> header_fields;

  // Export table entries (ordinal → address for XexGetProcedureAddress)
  std::vector<std::pair<uint32_t, uint32_t>> exports; // ordinal, address
};
