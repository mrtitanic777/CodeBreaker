// XEXMagic - High-level C decompiler backend (see decompiler_c.h)
//
// Pipeline, in the order the passes run:
//   1. index defs/uses of every SSA value
//   2. dead-code elimination  - drop pure defs nothing reads (PPC flag/temp noise)
//   3. expression propagation - fold single-use defs into their consumer
//   4. condition recovery     - fuse Compare + BranchCond back into `if (a > b)`
//   5. control-flow structuring - if/else and while; goto only when irreducible
//   6. emission with C precedence so we don't drown in parentheses

#include "decompiler_c.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>
#include <set>
#include <sstream>
#include <cctype>

#ifndef WIN95DEC_NO_PPC          // x86-only builds don't link the PPC opcode tables
#include "ppc/ppc_opcode.h"
#include "ppc/ppc_opcode_info.h"
#endif
#include <unordered_map>
#include <unordered_set>

namespace {

// All of the render state below is per-thread: the whole-image export runs the
// decompiler on several functions at once, and each of these is written during
// a render and read back after it.
// Borrowed, not copied: this map holds every symbol in the image and a copy
// per render dominates a whole-image export.
thread_local const std::map<uint32_t, std::string>* g_global_syms = nullptr;
const std::map<uint32_t, std::string>& GlobalSyms() {
  static const std::map<uint32_t, std::string> kEmpty;
  return g_global_syms ? *g_global_syms : kEmpty;
}

// Struct/type inference. As `*(T *)(base + off)` derefs are rendered as
// `base->field_off`, the observed (base variable -> offset -> access type) is
// recorded here so the assembler can synthesise a struct type per pointer base
// and retype its declaration. Populated during Render, consumed in Run();
// cleared per function.
thread_local std::map<std::string, std::map<uint32_t, ILType>> g_field_uses;

// Interactive field-name overrides: base var -> offset -> name.
thread_local std::map<std::string, std::map<uint32_t, std::string>> g_field_names;

// Plain pointer usage: base var -> access type, for a `*(T *)(base)` deref with
// no offset (base isn't a struct). Lets an argument/local dereferenced as a
// bare pointer be typed `T *` instead of int. Populated during Render.
thread_local std::map<std::string, ILType> g_ptr_uses;

// Interprocedural type overrides: base var -> declared type. Filled by the
// server's whole-program pass; applied only where local evidence is absent.
thread_local std::map<std::string, std::string> g_type_overrides;

// Argument flows from the most recent render: plain vars (aN/vN) passed to a
// call, with the callee and position. Consumed by the whole-program pass.
thread_local std::vector<ArgFlow> g_arg_flows;

// Locals that hold a copy of an incoming argument, from the most recent
// render. Lets a declared parameter type reach the register the prologue
// parked it in.
thread_local std::vector<BaseParamOrigin> g_base_param_origins;

// Base variable -> the declaration to print for it, taken from the SDK header
// that declares the type it was bound to.
thread_local std::map<std::string, std::string> g_struct_decls;

// Callee declarations to print as a preamble, and the callees the current
// render actually referenced (name -> widest argument count seen).
thread_local std::map<std::string, std::string> g_callee_decls;
thread_local std::map<std::string, int> g_called_functions;

// String literals by address, and the ones referenced by the current render.
thread_local const std::map<uint32_t, std::string>* g_strings = nullptr;
thread_local std::vector<uint32_t> g_string_refs;

// User-asserted signature for the function being rendered (borrowed).
thread_local const FuncSignature* g_signature = nullptr;

// Absolute-address accesses seen this render, and whether we are currently
// rendering the destination of a store (which makes the access a write).
thread_local std::vector<DataRef> g_data_refs;
thread_local bool g_store_dest = false;

// Structs bound to fixed addresses, plus the fields touched by this render.
thread_local const std::vector<GlobalStruct>* g_gstructs = nullptr;
thread_local std::map<uint32_t, std::map<uint32_t, std::string>> g_gfield_uses;

// The bound global containing `addr`, or nullptr.
const GlobalStruct* FindGlobalStruct(uint32_t addr) {
  if (!g_gstructs) return nullptr;
  for (const auto& g : *g_gstructs) {
    if (g.size && addr >= g.addr && addr < g.addr + g.size) return &g;
  }
  return nullptr;
}

// `name.field` spelling for an address inside a bound global, recording the
// field so the UI can list what this function touches.
std::string GlobalFieldRef(const GlobalStruct& g, uint32_t addr) {
  uint32_t off = addr - g.addr;
  std::string fname;
  auto it = g.fields.find(off);
  if (it != g.fields.end() && !it->second.empty()) {
    fname = it->second;
  } else {
    char b[24];
    snprintf(b, sizeof(b), "field_%X", off);
    fname = b;
  }
  g_gfield_uses[g.addr][off] = fname;
  return g.name + "." + fname;
}

// C-escaped, length-capped spelling of a string literal.
std::string QuoteString(const std::string& s) {
  const size_t kMax = 60;
  std::string o = "\"";
  for (size_t i = 0; i < s.size() && i < kMax; i++) {
    char c = s[i];
    switch (c) {
      case '"': o += "\\\""; break;
      case '\\': o += "\\\\"; break;
      case '\n': o += "\\n"; break;
      case '\r': o += "\\r"; break;
      case '\t': o += "\\t"; break;
      default:
        if ((unsigned char)c < 0x20 || (unsigned char)c > 0x7E) {
          char b[8];
          snprintf(b, sizeof(b), "\\x%02X", (unsigned char)c);
          o += b;
        } else {
          o += c;
        }
    }
  }
  if (s.size() > kMax) o += "...";
  o += "\"";
  return o;
}

// Current display name for a field: the override if set, else field_XX.
std::string FieldName(const std::string& base, uint32_t off) {
  auto bi = g_field_names.find(base);
  if (bi != g_field_names.end()) {
    auto oi = bi->second.find(off);
    if (oi != bi->second.end() && !oi->second.empty()) return oi->second;
  }
  char fn[24];
  snprintf(fn, sizeof(fn), "field_%X", off);
  return fn;
}

int TypeSize(ILType t) {
  switch (t) {
    case ILType::U8: case ILType::S8: case ILType::Bool: return 1;
    case ILType::U16: case ILType::S16: return 2;
    case ILType::U64: case ILType::S64: case ILType::F64: return 8;
    default: return 4;
  }
}

// Struct-tag overrides for unification: base var -> shared type name.
thread_local std::map<std::string, std::string> g_struct_tags;

// Struct type name for a pointer base variable. Uses the unification override
// when one is set, else synthesizes a per-variable tag ("irp" -> "Irp_t").
std::string StructNameFor(const std::string& base) {
  auto it = g_struct_tags.find(base);
  if (it != g_struct_tags.end() && !it->second.empty()) return it->second;
  std::string n = base;
  if (!n.empty() && n[0] >= 'a' && n[0] <= 'z')
    n[0] = (char)(n[0] - 'a' + 'A');
  return n + "_t";
}

// ---------------------------------------------------------------------------
// Expressions
// ---------------------------------------------------------------------------

// C precedence; lower binds tighter. Used to suppress redundant parentheses.
enum : int {
  kPrecPrimary = 0,
  kPrecUnary = 2,
  kPrecMul = 3,
  kPrecAdd = 4,
  kPrecShift = 5,
  kPrecRel = 6,
  kPrecEq = 7,
  kPrecBitAnd = 8,
  kPrecBitXor = 9,
  kPrecBitOr = 10,
  kPrecLAnd = 11,
  kPrecLOr = 12,
  kPrecCond = 13,
  kPrecAssign = 14,
};

struct Expr;
using ExprRef = std::shared_ptr<Expr>;

enum class EK { Const, Name, Unary, Binary, Deref, Call, Raw, Ternary };

struct Expr {
  EK kind = EK::Raw;
  std::string text;   // Name / Raw / Call callee / operator spelling
  uint64_t cval = 0;  // Const
  ExprRef a, b;
  std::vector<ExprRef> args;  // Call
  ILType type = ILType::U32;
  int prec = kPrecPrimary;
  bool reads_memory = false;  // gates reordering across stores/calls
};

ExprRef MkConst(uint64_t v, ILType t = ILType::U32) {
  auto e = std::make_shared<Expr>();
  e->kind = EK::Const;
  e->cval = v;
  e->type = t;
  return e;
}

ExprRef MkName(const std::string& n, ILType t = ILType::U32) {
  auto e = std::make_shared<Expr>();
  e->kind = EK::Name;
  e->text = n;
  e->type = t;
  return e;
}

ExprRef MkRaw(const std::string& n) {
  auto e = std::make_shared<Expr>();
  e->kind = EK::Raw;
  e->text = n;
  return e;
}

ExprRef MkUnary(const std::string& op, ExprRef a, ILType t = ILType::U32) {
  if (!a) return MkRaw("?");
  auto e = std::make_shared<Expr>();
  e->kind = EK::Unary;
  e->text = op;
  e->a = a;
  e->type = t;
  e->prec = kPrecUnary;
  e->reads_memory = a->reads_memory;
  return e;
}

ExprRef MkBin(const std::string& op, ExprRef a, ExprRef b, int prec,
              ILType t = ILType::U32) {
  if (!a || !b) return MkRaw("?");
  auto e = std::make_shared<Expr>();
  e->kind = EK::Binary;
  e->text = op;
  e->a = a;
  e->b = b;
  e->prec = prec;
  e->type = t;
  e->reads_memory = a->reads_memory || b->reads_memory;
  return e;
}

ExprRef MkCall(const char* name, std::initializer_list<ExprRef> args,
               ILType type, bool impure);

ExprRef MkTernary(ExprRef cond, ExprRef t, ExprRef f) {
  auto e = std::make_shared<Expr>();
  e->kind = EK::Ternary;
  e->a = cond;
  e->b = t;
  e->args.push_back(f);
  e->prec = kPrecCond;
  e->type = t ? t->type : ILType::U32;
  e->reads_memory = (cond && cond->reads_memory) || (t && t->reads_memory) ||
                    (f && f->reads_memory);
  return e;
}

// Structural equality, enough to spot the same mask on both arms of a select.
bool ExprEqual(const ExprRef& x, const ExprRef& y) {
  if (!x || !y) return x == y;
  if (x == y) return true;
  if (x->kind != y->kind || x->text != y->text || x->cval != y->cval)
    return false;
  if (x->args.size() != y->args.size()) return false;
  for (size_t i = 0; i < x->args.size(); i++)
    if (!ExprEqual(x->args[i], y->args[i])) return false;
  return ExprEqual(x->a, y->a) && ExprEqual(x->b, y->b);
}

// `~m` for some m -> return m, else null.
ExprRef NotOperand(const ExprRef& e) {
  if (e && e->kind == EK::Unary && e->text == "~") return e->a;
  return nullptr;
}

// The compiler emits branchless selects as (~m & A) | (m & B), where m is a
// sign mask (0 or -1) produced by an arithmetic right shift. Project that back
// to the intent: a conditional, and where the shape is a limiter, min/max.
//
// This is only valid because the mask is used as all-ones-or-zero, which is
// exactly what the shape asserts -- the emitted code is wrong for any other m,
// so reading it as a select does not change the meaning of correct input.
using Resolver = std::function<ExprRef(const ExprRef&)>;

ExprRef FoldSelect(const ExprRef& lhs_in, const ExprRef& rhs_in,
                   const Resolver& rs) {
  if (!lhs_in || !rhs_in) return nullptr;
  // Both arms are usually named locals (each is read once by the `|`, but the
  // mask inside them is read twice), so look through them as well.
  ExprRef lhs = rs(lhs_in), rhs = rs(rhs_in);
  if (!lhs || !rhs) return nullptr;
  if (lhs->kind != EK::Binary || lhs->text != "&") return nullptr;
  if (rhs->kind != EK::Binary || rhs->text != "&") return nullptr;
  // Find (mask, value) on each side: one side must be ~m, the other m.
  const ExprRef sides[2][2] = {{lhs->a, lhs->b}, {rhs->a, rhs->b}};
  for (int i = 0; i < 2; i++) {        // which operand of lhs is the ~mask
    ExprRef m = NotOperand(sides[0][i]);
    if (!m) continue;
    ExprRef a_val = sides[0][1 - i];   // taken when mask == 0
    for (int j = 0; j < 2; j++) {      // which operand of rhs is the mask
      if (!ExprEqual(sides[1][j], m)) continue;
      ExprRef b_val = sides[1][1 - j]; // taken when mask == -1
      // m must be `expr >> k` to be a sign mask.
      ExprRef md = rs(m);
      if (!md || md->kind != EK::Binary || md->text != ">>") return nullptr;
      if (!md->b || md->b->kind != EK::Const) return nullptr;
      ExprRef inner = rs(md->a);
      if (!inner) return nullptr;
      // Limiter shapes: inner == X + K.
      if (inner->kind == EK::Binary && inner->text == "+" && inner->b &&
          inner->b->kind == EK::Const) {
        int32_t k = (int32_t)(uint32_t)inner->b->cval;
        int64_t thr = -(int64_t)k;
        ExprRef x = inner->a;
        auto is_thr = [&](const ExprRef& e) {
          return e && e->kind == EK::Const &&
                 (int64_t)(int32_t)(uint32_t)e->cval == thr;
        };
        if (ExprEqual(b_val, x) && is_thr(a_val))
          return MkCall("__min", {x, a_val}, x->type, false);
        if (ExprEqual(a_val, x) && is_thr(b_val))
          return MkCall("__max", {x, b_val}, x->type, false);
      }
      ExprRef cond = MkBin("<", inner, MkConst(0, ILType::S32), kPrecRel,
                           ILType::U32);
      return MkTernary(cond, b_val, a_val);
    }
  }
  return nullptr;
}

bool IsConstZero(const ExprRef& e) {
  return e && e->kind == EK::Const && e->cval == 0;
}

// Constant-fold the address arithmetic that PPC's lis/addi pairs produce, so
// `0x841E0000 + 6960` prints as the actual global address.
ExprRef MkAdd(ExprRef a, ExprRef b) {
  if (!a) return b;
  if (!b) return a;
  if (IsConstZero(b)) return a;
  if (IsConstZero(a)) return b;
  if (a->kind == EK::Const && b->kind == EK::Const)
    return MkConst((uint32_t)(a->cval + b->cval));
  // (x + c1) + c2  ->  x + (c1+c2)
  if (b->kind == EK::Const && a->kind == EK::Binary && a->text == "+" &&
      a->b && a->b->kind == EK::Const) {
    return MkAdd(a->a, MkConst((uint32_t)(a->b->cval + b->cval)));
  }
  // A negative displacement is what the instruction actually encodes - PPC
  // offsets are signed 16-bit. Printing it as 0xFFFFFF60 hides a subtraction
  // of 0xA0 behind a number that looks like an address.
  if (b->kind == EK::Const && (int32_t)b->cval < 0 &&
      (int32_t)b->cval > INT32_MIN) {
    return MkBin("-", a, MkConst((uint32_t)(-(int32_t)b->cval)), kPrecAdd);
  }
  return MkBin("+", a, b, kPrecAdd);
}

const char* CTypeName(ILType t) {
  switch (t) {
    case ILType::U8: return "unsigned __int8";
    case ILType::S8: return "__int8";
    case ILType::U16: return "unsigned __int16";
    case ILType::S16: return "__int16";
    case ILType::U32: return "unsigned int";
    case ILType::S32: return "int";
    case ILType::U64: return "unsigned __int64";
    case ILType::S64: return "__int64";
    case ILType::F32: return "float";
    case ILType::F64: return "double";
    case ILType::Bool: return "bool";
    // A whole vector register, named for how the instruction that produced it
    // reads the 128 bits.
    case ILType::V128U: return "uint128_t";
    case ILType::V128S: return "int128_t";
    case ILType::V128F: return "float128_t";
    default: return "unsigned int";
  }
}

// Pointer type spelled from LOCAL evidence only (field accesses / bare deref),
// or "" if none. Used to seed the interprocedural pass.
std::string LocalPtrType(const std::string& name) {
  auto fit = g_field_uses.find(name);
  if (fit != g_field_uses.end() && !fit->second.empty())
    return "struct " + StructNameFor(name) + " *";
  auto pit = g_ptr_uses.find(name);
  if (pit != g_ptr_uses.end())
    return std::string(CTypeName(pit->second)) + " *";
  return "";
}

// Declared type: local evidence first, then an interprocedural override, then
// int. Local evidence wins so cross-function guesses never fight what the body
// actually does.
std::string FullType(const std::string& name) {
  // Inference-based pointer types (field/bare-deref uses) are NOT spelled in the
  // declaration -- the deref sites cast instead, so a base that is also used as an
  // integer keeps one consistent type and the C compiles. Only an asserted
  // interprocedural override (SDK/demangled prototype) sets a real declared type.
  auto oit = g_type_overrides.find(name);
  if (oit != g_type_overrides.end() && !oit->second.empty()) return oit->second;
  return "int";
}

// Intrinsic call `name(args...)`. Null args are skipped, so callers can pass
// optional operands (an absent rA, say) without checking first.
ExprRef MkCall(const char* name, std::initializer_list<ExprRef> args,
               ILType type, bool impure = false) {
  auto e = std::make_shared<Expr>();
  e->kind = EK::Call;
  e->text = name;
  e->type = type;
  e->reads_memory = impure;
  for (const ExprRef& x : args) {
    if (x) e->args.push_back(x);
    if (x && x->reads_memory) e->reads_memory = true;
  }
  return e;
}

ExprRef MkDeref(ExprRef addr, ILType access) {
  auto e = std::make_shared<Expr>();
  e->kind = EK::Deref;
  e->a = addr;
  e->type = access;
  e->prec = kPrecUnary;
  e->reads_memory = true;
  return e;
}

// A float value used in an integer/bitwise context must have its BITS read, not
// be numerically converted -- `*(unsigned int *)&(x)`. Compiler FP-bit tricks
// (and the x87 lifter's float temps/fields) reach `&`, `|`, `^`, `~` and shifts
// this way; without the reinterpret wcc rejects them (E1079 not integral). The
// operand is an lvalue in these cases (a named temp, or a `base->field` the
// struct-field inference still types float, so its bits are the float's). Only
// float-typed exprs are wrapped; everything else passes through unchanged.
ExprRef BitsAsInt(ExprRef e) {
  if (!e || !ILTypeIsFloat(e->type)) return e;
  return MkDeref(MkUnary("&", e, ILType::Ptr), ILType::U32);
}

std::string HexLit(uint64_t v) {
  char buf[32];
  if (v < 10) snprintf(buf, sizeof(buf), "%llu", (unsigned long long)v);
  else snprintf(buf, sizeof(buf), "0x%llX", (unsigned long long)v);
  return buf;
}

std::string Render(const ExprRef& e, int parent_prec);

// Renders the destination of a store, flagging any absolute-address access it
// contains as a WRITE for the data cross-reference index.
std::string RenderStoreDest(const ExprRef& e) {
  g_store_dest = true;
  std::string s = Render(e, kPrecAssign);
  g_store_dest = false;
  return s;
}

std::string RenderChild(const ExprRef& e, int parent_prec) {
  if (!e) return "?";
  std::string s = Render(e, parent_prec);
  if (e->prec > parent_prec) return "(" + s + ")";
  return s;
}

std::string Render(const ExprRef& e, int parent_prec) {
  if (!e) return "?";
  switch (e->kind) {
    case EK::Const: {
      // An address inside a struct bound to a fixed address prints as a
      // pointer to the field, so `memset(0x8288F780, ...)` reads as
      // `memset(&g_ffStream, ...)`.
      if (const GlobalStruct* g = FindGlobalStruct((uint32_t)e->cval))
        return "&" + GlobalFieldRef(*g, (uint32_t)e->cval);
      // A constant that is the address of a known string prints as the literal
      // — far more useful than the number — and the reference is recorded.
      if (g_strings) {
        auto sit = g_strings->find((uint32_t)e->cval);
        if (sit != g_strings->end()) {
          g_string_refs.push_back((uint32_t)e->cval);
          return QuoteString(sit->second);
        }
      }
      // Print known globals symbolically so data refs are recognisable.
      auto it = GlobalSyms().find((uint32_t)e->cval);
      if (it != GlobalSyms().end()) return it->second;
      return HexLit(e->cval);
    }
    case EK::Name:
    case EK::Raw:
      return e->text;
    case EK::Unary: {
      if (!e->a) return "0";
      std::string c = RenderChild(e->a, e->prec);
      // Never let a unary -/+ fuse with a child that begins with the same sign:
      // `-` applied to `-0xFFFF` would print `--0xFFFF`, which C parses as a
      // pre-decrement of a constant (E1014). A space keeps it `- -0xFFFF`.
      if (!c.empty() && !e->text.empty() &&
          (e->text.back() == '-' || e->text.back() == '+') &&
          c[0] == e->text.back())
        return e->text + " " + c;
      return e->text + c;
    }
    case EK::Binary:
      // Guard a missing operand (an unresolved sub-expression) so we never emit a
      // dangling operator like `x + ` that would splice the next statement into the
      // expression -- render the present side alone (or 0 if both are gone).
      if (!e->a && !e->b) return "0";
      if (!e->b) return RenderChild(e->a, e->prec);
      if (!e->a) return RenderChild(e->b, e->prec);
      return RenderChild(e->a, e->prec) + " " + e->text + " " +
             RenderChild(e->b, e->prec);
    case EK::Deref: {
      // Record every absolute-address access so data cross-references can be
      // built with the constant already folded, and with a direction.
      if (e->a && e->a->kind == EK::Const)
        g_data_refs.push_back({(uint32_t)e->a->cval, g_store_dest});
      // Absolute access into a struct bound to a fixed address: the whole
      // point of binding, so scattered globals read as one object's fields.
      if (e->a && e->a->kind == EK::Const) {
        if (const GlobalStruct* g = FindGlobalStruct((uint32_t)e->a->cval))
          return GlobalFieldRef(*g, (uint32_t)e->a->cval);
      }
      // Struct field inference: `*(T *)(base + off)` with a small constant
      // offset and a non-constant base reads as `base->field_off`. The bare
      // `*(T *)(x)` form (no offset) and absolute `*(T *)(0x...)` globals keep
      // the canonical spelling.
      const ExprRef& a = e->a;
      // Only treat a real local/argument (vN / aN) as a struct pointer — not
      // stack (sp/r1) or system-register (r13) bases, whose offsets are stack
      // slots / TLS, not object fields.
      auto is_ptr_var = [](const ExprRef& x) {
        if (!x || x->kind != EK::Name || x->text.size() < 2) return false;
        char c = x->text[0];
        if (c != 'v' && c != 'a') return false;
        for (size_t i = 1; i < x->text.size(); i++)
          if (!std::isdigit((unsigned char)x->text[i])) return false;
        return true;
      };
      if (a && a->kind == EK::Binary && a->text == "+" && a->b &&
          a->b->kind == EK::Const && is_ptr_var(a->a)) {
        uint64_t off = a->b->cval;
        if (off > 0 && off < 0x10000) {
          std::string base = a->a->text;
          auto& fields = g_field_uses[base];
          auto it = fields.find((uint32_t)off);
          // Keep the widest access seen at this offset (this fixes the field's
          // declared type). A type-punned field -- declared float from FP math but
          // also read/written as an integer at the same offset (common in graphics
          // code), or vice versa -- would otherwise emit `float f; ... f >> n`,
          // which wcc rejects (E1079/E1090). Since the field type and THIS access
          // can disagree in class (int vs float), read the exact bytes this access
          // wants via a reinterpret when they do: `*(accessT *)&(field)`.
          if (it == fields.end() || TypeSize(e->type) > TypeSize(it->second)) {
            fields[(uint32_t)off] = e->type;
            it = fields.find((uint32_t)off);
          }
          // Cast the base at the deref site rather than declaring it a struct
          // pointer: the same value is often also used as an integer, so a pointer
          // declaration would clash (ptr == int, arithmetic). Casting here keeps
          // the readable `->field` form AND a single consistent (integer) type for
          // the base, so the C compiles.
          std::string acc = "((struct " + StructNameFor(base) + " *)" + base +
                            ")->" + FieldName(base, (uint32_t)off);
          if (ILTypeIsFloat(e->type) != ILTypeIsFloat(it->second))
            acc = "*(" + std::string(CTypeName(e->type)) + " *)&(" + acc + ")";
          return acc;
        }
      }
      // Bare pointer deref of a plain variable: record it so the base can be
      // typed `T *` (widest access wins).
      if (a && a->kind == EK::Name) {
        auto it = g_ptr_uses.find(a->text);
        if (it == g_ptr_uses.end() || TypeSize(e->type) > TypeSize(it->second))
          g_ptr_uses[a->text] = e->type;
      }
      // *(type *)(addr) — the canonical decompiler spelling.
      std::string inner = Render(e->a, kPrecAssign);
      return std::string("*(") + CTypeName(e->type) + " *)(" + inner + ")";
    }
    case EK::Ternary: {
      std::string s = Render(e->a, kPrecCond) + " ? " + Render(e->b, kPrecCond) +
                      " : " + Render(e->args[0], kPrecCond);
      return parent_prec < kPrecCond ? "(" + s + ")" : s;
    }
    case EK::Call: {
      std::string s = e->text + "(";
      for (size_t i = 0; i < e->args.size(); i++) {
        if (i) s += ", ";
        s += Render(e->args[i], kPrecAssign);
      }
      return s + ")";
    }
  }
  return "?";
}

const char* CmpOp(ILCmpKind k) {
  switch (k) {
    case ILCmpKind::EQ: return "==";
    case ILCmpKind::NE: return "!=";
    case ILCmpKind::LT: case ILCmpKind::LTU: return "<";
    case ILCmpKind::GT: case ILCmpKind::GTU: return ">";
    case ILCmpKind::LE: case ILCmpKind::LEU: return "<=";
    case ILCmpKind::GE: case ILCmpKind::GEU: return ">=";
  }
  return "==";
}

int CmpPrec(ILCmpKind k) {
  return (k == ILCmpKind::EQ || k == ILCmpKind::NE) ? kPrecEq : kPrecRel;
}

ILCmpKind InvertCmp(ILCmpKind k) {
  switch (k) {
    case ILCmpKind::EQ: return ILCmpKind::NE;
    case ILCmpKind::NE: return ILCmpKind::EQ;
    case ILCmpKind::LT: return ILCmpKind::GE;
    case ILCmpKind::GE: return ILCmpKind::LT;
    case ILCmpKind::GT: return ILCmpKind::LE;
    case ILCmpKind::LE: return ILCmpKind::GT;
    case ILCmpKind::LTU: return ILCmpKind::GEU;
    case ILCmpKind::GEU: return ILCmpKind::LTU;
    case ILCmpKind::GTU: return ILCmpKind::LEU;
    case ILCmpKind::LEU: return ILCmpKind::GTU;
  }
  return k;
}

// ---------------------------------------------------------------------------
// Decompiler state
// ---------------------------------------------------------------------------

struct Stmt {
  std::string text;
  uint32_t addr = 0;
  // Non-empty when this statement is a side-effect-free assignment to that
  // local: such a statement can be deleted if nothing ever reads the local.
  std::string pure_def;
};

class CDecompiler {
 public:
  CDecompiler(const ILFunction& f, const DecompileCOptions& o)
      : func_(f), opts_(o) {}

  std::string Run();

 private:
  const ILFunction& func_;
  const DecompileCOptions& opts_;

  std::unordered_map<int, ExprRef> expr_of_;    // SSA id -> folded expression
  std::unordered_map<int, int> use_count_;
  // Definitions per SSA id. A merge value (see ILBuilder's predecessor merge)
  // is assigned once per incoming arm, so it is NOT single-assignment and must
  // never be folded into its consumer -- doing so picks one arm's expression
  // and silently drops the other.
  std::unordered_map<int, int> def_count_;
  // SSA-destruction copies (`vDst = vSrc` at the end of a predecessor) exist
  // only to give a merge point one value. Both sides are the same architectural
  // register, which means they were one variable in the original source, so
  // they get one name here and the copy disappears. Union-find over those
  // copies; VarName() resolves an id to its root.
  std::unordered_map<int, int> coalesce_;
  // Declared type per variable (keyed by coalescing root). A phi web can be
  // written by instructions that read the register differently - a float
  // multiply on one arm, a bitwise or on another. The variable gets one type,
  // and any definition that produces a different one is cast at the point of
  // assignment rather than silently relabelled.
  std::unordered_map<int, ILType> decl_type_;
  std::vector<std::pair<int, ILType>> def_types_;    // filled by IndexUses
  std::vector<std::pair<int, int>> merge_moves_;     // (dst, src), same
  std::unordered_map<int, std::set<int>> interfere_;  // value -> values live with it
  void MarkOutgoingStackArgs();
  void BuildInterference();
  void CoalesceMergeCopies();
  ILType DeclType(int id) {
    auto it = decl_type_.find(Root(id));
    return it == decl_type_.end() ? ILType::U32 : it->second;
  }
  int Root(int id) {
    while (true) {
      auto it = coalesce_.find(id);
      if (it == coalesce_.end() || it->second == id) return id;
      // Path compression, so a long phi web costs one step next time.
      auto nit = coalesce_.find(it->second);
      if (nit != coalesce_.end() && nit->second != it->second)
        it->second = nit->second;
      id = it->second;
    }
  }
  std::string VarName(int id) { return "v" + std::to_string(Root(id)); }
  // Every definition's expression, kept even when the value is materialised as
  // a local. Lets the select recogniser look through `v111` to the shift that
  // produced it -- the mask of a branchless select is usually a named variable
  // because it is read twice.
  std::unordered_map<int, ExprRef> orig_expr_;
  ExprRef ResolveDef(const ExprRef& e) const {
    if (!e || e->kind != EK::Name) return e;
    const std::string& n = e->text;
    if (n.size() < 2 || n[0] != 'v') return e;
    for (size_t i = 1; i < n.size(); i++)
      if (n[i] < '0' || n[i] > '9') return e;
    auto it = orig_expr_.find(std::atoi(n.c_str() + 1));
    return it == orig_expr_.end() ? e : it->second;
  }
  std::unordered_map<int, const ILInstr*> def_of_;
  std::unordered_map<int, int> def_barrier_;    // store/call counter at def time
  std::unordered_map<int, std::string> pending_cmp_;  // SSA id -> rendered cmp
  std::unordered_map<int, ILCmpKind> cmp_kind_of_;
  std::unordered_set<uint32_t> label_targets_;
  std::vector<std::string> locals_;
  std::unordered_set<int> spr_values_;   // SSA ids holding LR/CTR (frame setup)
  std::unordered_set<int> live_in_args_; // ppc_reg of used, never-defined regs
  std::map<int, ExprRef> cur_arg_val_;   // r3-r10 -> value currently held
  // Stores that are really outgoing arguments (see MarkOutgoingStackArgs),
  // and the values they staged, keyed by frame offset so the call can take
  // them in parameter order.
  std::set<const ILInstr*> stack_arg_stores_;
  struct StagedArg {
    ExprRef value;
    std::string stmt;   // what to emit instead, if it turns out not to be one
    uint32_t address = 0;
  };
  std::map<uint32_t, StagedArg> pending_stack_args_;
  int last_r3_ = kILNoValue;             // current value of the return register
  int last_vr1_ = kILNoValue;            // same, for a vector return value
  int barrier_ = 0;
  int next_var_ = 1;

  bool IsFrameNoise(const ILInstr& in) const;
  void IndexUses();
  void CollectLabels();
  ExprRef Get(const ILValue& v);
  ExprRef BuildExpr(const ILInstr& in, bool* pure);
  std::string CondText(const ILInstr& br, bool invert);
  std::string NameForLiveIn(const ILValue& v);
  std::string RenderTailCall(const ILInstr& in);
  bool IsPure(const ILInstr& in) const;
};

bool IsVectorOp(const ILInstr& in) {
  switch (in.op) {
    case ILOp::VectorOp:
    case ILOp::VMXLoad: case ILOp::VMXLoad128:
    case ILOp::VMXStore: case ILOp::VMXStore128:
      return true;
    default:
      return false;
  }
}

// The name a vector instruction is spelled with. The opcode tables carry a
// description of every instruction, and "LoadVectorIndexed128" says what the
// line does where "__lvx128" needs a comment to say the same thing. The
// mnemonic is not lost - it moves to the annotation, so a line still matches
// up against the disassembly.
std::string VectorOpName(const ILInstr& in) {
  const char* fallback_mnem =
      in.call_name.empty() ? "vectorop" : in.call_name.c_str();
  if (!in.ppc_word) return std::string("__") + fallback_mnem;
#ifdef WIN95DEC_NO_PPC
  return std::string("__") + fallback_mnem;   // no PPC tables in x86-only build
#else
  using namespace xe::cpu::ppc;
  PPCOpcode opcode = LookupOpcode(in.ppc_word);
  if (opcode == PPCOpcode::kInvalid) return std::string("__") + fallback_mnem;
  const char* desc = GetOpcodeDisasmInfo(opcode).description;
  if (!desc || !*desc) return std::string("__") + fallback_mnem;
  std::string name;
  bool upper = true;
  for (const char* p = desc; *p; p++) {
    if (std::isalnum((unsigned char)*p)) {
      name += upper ? (char)std::toupper((unsigned char)*p) : *p;
      upper = false;
    } else {
      upper = true;  // space, hyphen, slash: start the next word
    }
  }
  return name.empty() ? std::string("__") + fallback_mnem : name;
#endif  // WIN95DEC_NO_PPC
}

// The mnemonic the line came from, so the C and the disassembly line up.
std::string OpAnnotation(const ILInstr& in) {
  if (!IsVectorOp(in) || in.call_name.empty()) return "";
  return "  // " + in.call_name;
}

// A demangled C++ name is a whole signature - "XMVECTORU32::operator struct
// __vector4(void)const". Printed where an identifier belongs it produces
// something that is neither readable nor C, so the parameter list and
// qualifiers are dropped and the rest is reduced to identifier characters.
// The full signature still appears in the header comment above the function.
std::string CodeIdent(const std::string& name) {
  size_t end = name.find('(');
  if (end == std::string::npos) end = name.size();
  std::string base = name.substr(0, end);
  // A qualified or operator name is spelled across several space-separated
  // pieces ("XMVECTORU32::operator struct __vector4"), so keep everything from
  // where the qualification starts. A plain name is the last piece, with the
  // return type and calling convention in front of it.
  size_t scope = base.find("::");
  if (scope == std::string::npos) scope = base.find("operator");
  if (scope != std::string::npos) {
    size_t begin = base.rfind(' ', scope);
    base = base.substr(begin == std::string::npos ? 0 : begin + 1);
  } else {
    size_t sp = base.rfind(' ');
    if (sp != std::string::npos) base = base.substr(sp + 1);
  }
  std::string out;
  for (size_t i = 0; i < base.size(); i++) {
    char c = base[i];
    if (std::isalnum((unsigned char)c) || c == '_') {
      out += c;
    } else if (!out.empty() && out.back() != '_') {
      out += '_';
    }
  }
  while (!out.empty() && out.back() == '_') out.pop_back();
  if (out.empty() || std::isdigit((unsigned char)out[0])) out = "fn_" + out;
  return out;
}

// Renders an out-of-function `b`/`bc` terminator as a returning tail call.
// Restore/save helper thunks carry no result, so they become a plain `return`.
std::string CDecompiler::RenderTailCall(const ILInstr& in) {
  std::string callee;
  auto sit = GlobalSyms().find(in.target);
  if (sit != GlobalSyms().end()) {
    callee = sit->second;
  } else if (!in.call_name.empty()) {
    callee = in.call_name;
  } else {
    char nb[32];
    snprintf(nb, sizeof(nb), "sub_%08X", in.target);
    callee = nb;
  }

  // Compiler register save/restore thunks are epilogue glue, not a real call.
  if (callee.rfind("__rest", 0) == 0 || callee.rfind("__save", 0) == 0)
    return "return;";

  auto ce = std::make_shared<Expr>();
  ce->kind = EK::Call;
  ce->text = CodeIdent(callee);
  ce->reads_memory = true;
  // Same heuristic as ILOp::Call: take the contiguous run of argument
  // registers r3.. that currently hold a value.
  for (int r = 3; r <= 10; r++) {
    auto vit = cur_arg_val_.find(r);
    if (vit == cur_arg_val_.end()) break;
    ce->args.push_back(vit->second);
  }
  return "return " + Render(ce, kPrecAssign) + ";";
}

bool CDecompiler::IsPure(const ILInstr& in) const {
  switch (in.op) {
    case ILOp::Store: case ILOp::StoreIndexed: case ILOp::StoreUpdate:
    case ILOp::StoreMultiple: case ILOp::StoreConditional:
    case ILOp::StoreByteReverse: case ILOp::FStore: case ILOp::FStoreIndexed:
    case ILOp::FStoreUpdate: case ILOp::FStoreIntWord:
    case ILOp::VMXStore: case ILOp::VMXStore128:
    case ILOp::Call: case ILOp::CallCTR: case ILOp::Return:
    case ILOp::Branch: case ILOp::BranchCond: case ILOp::BranchCTR:
    case ILOp::BranchLR: case ILOp::Syscall: case ILOp::Trap:
    case ILOp::TrapImm: case ILOp::InlineAsm:
    // lwarx sets the reservation — a side effect, so it must never be folded
    // into a consumer or eliminated as an unused pure load.
    case ILOp::LoadReserved:
    // MSR access enables/disables interrupts — never fold or eliminate.
    case ILOp::MoveMSR:
    // Register/cache/vector side effects and volatile reads: keep them ordered
    // and never eliminate them as dead.
    case ILOp::MoveCR: case ILOp::LoadTimeBase:
    case ILOp::LoadFPSCR: case ILOp::StoreFPSCR:
    case ILOp::DCacheTouch: case ILOp::DCacheTouchStore:
    case ILOp::LoadMultiple: case ILOp::VMXOp:
    case ILOp::VMXLoad: case ILOp::VMXLoad128:
    case ILOp::BranchDecrCTRZero: case ILOp::BranchDecrCTRNonZero:
    case ILOp::CRAnd: case ILOp::CROr: case ILOp::CRXor:
    case ILOp::CRNand: case ILOp::CRNor: case ILOp::CRAndc:
    case ILOp::CROrc: case ILOp::CREqv: case ILOp::MoveCRField:
    case ILOp::DCacheFlush: case ILOp::DCacheStore: case ILOp::DCacheZero:
    case ILOp::ICacheInvalidate: case ILOp::Sync: case ILOp::LWSync:
    case ILOp::PTESync: case ILOp::ISync: case ILOp::EIEIO:
      return false;
    default:
      return true;
  }
}

// Prologue/epilogue bookkeeping carries no meaning for a reader: saving LR,
// spilling callee-saved registers to the frame, and moving the stack pointer.
// IDA hides all of it; so do we.
bool CDecompiler::IsFrameNoise(const ILInstr& in) const {
  switch (in.op) {
    case ILOp::StackAlloc:
      return true;
    case ILOp::MoveSPR:
      // mflr/mtlr around the frame; CTR reads that feed a real bctrl are kept
      // because CallCTR renders them itself.
      return in.spr == ILSPR::LR;
    case ILOp::Store: case ILOp::StoreUpdate: case ILOp::FStore:
      // Spilling LR or a callee-saved register into our own frame.
      if (in.src1.ppc_reg == 1 || in.src1.ppc_reg == 12) {
        if (spr_values_.count(in.src2.id)) return true;
        if (in.src2.ppc_reg >= 13 && in.src2.ppc_reg < 32) return true;
        if (in.src2.ppc_reg == 12) return true;
      }
      return false;
    case ILOp::Load: case ILOp::LoadUpdate:
      // Reloading a callee-saved register in the epilogue.
      if (in.src1.ppc_reg == 1 && in.dst.ppc_reg >= 13 && in.dst.ppc_reg < 32)
        return true;
      if (in.src1.ppc_reg == 1 && in.dst.ppc_reg == 12) return true;
      return false;
    case ILOp::Add: case ILOp::Assign:
      // Stack pointer adjustment.
      return in.dst.ppc_reg == 1;
    default:
      return false;
  }
}

std::string CDecompiler::NameForLiveIn(const ILValue& v) {
  if (v.ppc_reg >= 3 && v.ppc_reg <= 10) {
    // A user-supplied argument name wins over the positional default, so the
    // body reads with real parameter names rather than a1..a8.
    if (g_signature) {
      auto it = g_signature->args.find(v.ppc_reg - 2);
      if (it != g_signature->args.end() && !it->second.second.empty())
        return it->second.second;
    }
    return "a" + std::to_string(v.ppc_reg - 2);  // r3->a1 .. r10->a8
  }
  if (v.ppc_reg == 1) return "sp";
  if (v.ppc_reg >= 0 && v.ppc_reg < 32) return "r" + std::to_string(v.ppc_reg);
  if (v.ppc_reg >= 32 && v.ppc_reg < 64)
    return "f" + std::to_string(v.ppc_reg - 32);
  return VarName(v.id);
}

ExprRef CDecompiler::Get(const ILValue& v) {
  if (v.id == kILNoValue) return nullptr;
  // ILBuilder materialises immediates as SSA values with no defining
  // instruction, carrying the literal text in .name (see makeImm/makeUImm).
  // Recognising those is what turns `a1 * v6` into `a1 * 10512`.
  // Only *numeric* names are immediates. BuildFunction also pre-names the
  // parameter values "arg0", "arg1", ... and strtoull would silently turn
  // those into the constant 0.
  if (!v.name.empty() && !def_of_.count(v.id)) {
    const char* s = v.name.c_str();
    bool numeric = (s[0] == '-' && s[1]) || (s[0] >= '0' && s[0] <= '9');
    if (numeric) {
      if (s[0] == '-') {
        long long sv = strtoll(s, nullptr, 0);
        return MkConst((uint64_t)(uint32_t)(int32_t)sv, v.type);
      }
      return MkConst(strtoull(s, nullptr, 0), v.type);
    }
  }
  auto it = expr_of_.find(v.id);
  if (it != expr_of_.end()) {
    // Don't move a memory read across an intervening store/call.
    if (it->second->reads_memory) {
      auto bit = def_barrier_.find(v.id);
      if (bit != def_barrier_.end() && bit->second != barrier_) {
        // fall through to a plain name below
      } else {
        return it->second;
      }
    } else {
      return it->second;
    }
  }
  if (def_of_.count(v.id)) return MkName(VarName(v.id), v.type);
  return MkName(NameForLiveIn(v), v.type);
}

// Two values interfere when both are live at the same point, which is the only
// thing that makes sharing one variable for them wrong. Anything weaker is a
// guess: "the source has only one reader" refuses most legal merges and leaves
// the output full of `vA = vB;`, while merging unconditionally corrupts code
// where the two really are live together.
// Arguments nine and up are passed in the caller's outgoing parameter save
// area, at the bottom of its own frame. Rendered literally that is a store to
// `sp + 0x54` sitting next to a call that appears to take fewer arguments than
// it does - the argument is dropped and the store looks like the function
// writing to itself.
//
// A store qualifies only when it is r1-relative, below this function's frame,
// and reaches a call before the slot is read again or the block ends, with
// every argument register already occupied - stack arguments exist only once
// r3..r10 are full. Anything else is an ordinary local and is left alone.
void CDecompiler::MarkOutgoingStackArgs() {
  if (!func_.stack_frame_size) return;

  for (const auto& b : func_.blocks) {
    std::map<uint32_t, const ILInstr*> staged;
    for (const auto& in : b.instrs) {
      const bool r1_rel = in.src1.ppc_reg == 1;
      const uint32_t off = (uint32_t)in.imm;

      if ((in.op == ILOp::Store || in.op == ILOp::FStore) && r1_rel &&
          in.imm > 0 && off < func_.stack_frame_size) {
        staged[off] = &in;
        continue;
      }
      // Reading the slot back means it is a local, not an argument in flight.
      if ((in.op == ILOp::Load || in.op == ILOp::FLoad) && r1_rel)
        staged.erase(off);

      // Whether the argument registers are full is only known while emitting,
      // so this marks the candidates and the call site makes the call.
      if (in.op == ILOp::Call || in.op == ILOp::CallCTR) {
        for (const auto& kv : staged) stack_arg_stores_.insert(kv.second);
        staged.clear();
      }
    }
  }
}

void CDecompiler::BuildInterference() {
  const size_t nblk = func_.blocks.size();
  std::map<uint32_t, size_t> block_at;
  for (size_t i = 0; i < nblk; i++) block_at[func_.blocks[i].start_addr] = i;

  // Only the two ends of a copy can ever be coalesced, and the only question
  // asked of this graph is whether two such values conflict. Tracking every
  // value instead makes the whole pass quadratic in the size of the function:
  // the 30 KB shader-compiler routines in xgraphics.lib took minutes on their
  // own while 13,956 other functions finished in seconds. A value that is not
  // a copy endpoint can never be the root of a class, so its liveness cannot
  // change any answer - it is left out entirely.
  std::set<int> cand;
  for (const auto& [dst_id, src_id] : merge_moves_) {
    cand.insert(dst_id);
    cand.insert(src_id);
  }
  if (cand.empty()) return;
  auto is_cand = [&](int id) { return id != kILNoValue && cand.count(id) != 0; };

  // Upward-exposed uses and definitions per block, then the usual backward
  // fixpoint for live-in / live-out.
  std::vector<std::set<int>> use(nblk), def(nblk), live_in(nblk), live_out(nblk);
  for (size_t b = 0; b < nblk; b++) {
    for (const auto& in : func_.blocks[b].instrs) {
      auto read = [&](const ILValue& v) {
        if (is_cand(v.id) && !def[b].count(v.id)) use[b].insert(v.id);
      };
      read(in.src1);
      read(in.src2);
      read(in.src3);
      if (is_cand(in.cmp_ref) && !def[b].count(in.cmp_ref))
        use[b].insert(in.cmp_ref);
      if (is_cand(in.dst.id)) def[b].insert(in.dst.id);
    }
  }
  for (bool changed = true; changed;) {
    changed = false;
    for (size_t i = nblk; i-- > 0;) {
      std::set<int> out;
      for (uint32_t succ : func_.blocks[i].successors) {
        auto it = block_at.find(succ);
        if (it != block_at.end())
          out.insert(live_in[it->second].begin(), live_in[it->second].end());
      }
      if (out != live_out[i]) {
        live_out[i] = std::move(out);
        changed = true;
      }
      std::set<int> in = use[i];
      for (int v : live_out[i])
        if (!def[i].count(v)) in.insert(v);
      if (in != live_in[i]) {
        live_in[i] = std::move(in);
        changed = true;
      }
    }
  }

  // Walk each block backwards; whatever is live where a value is defined
  // interferes with it. A merge copy's own source is excluded - being the
  // operand of the copy that ends its range is exactly what makes it a
  // candidate to share the destination's variable.
  for (size_t b = 0; b < nblk; b++) {
    std::set<int> live = live_out[b];
    const auto& instrs = func_.blocks[b].instrs;
    for (size_t i = instrs.size(); i-- > 0;) {
      const ILInstr& in = instrs[i];
      const bool is_copy = in.op == ILOp::Assign && in.src1.id != kILNoValue;
      if (is_cand(in.dst.id)) {
        live.erase(in.dst.id);
        for (int v : live) {
          // The operand of a copy does not interfere with the copy's result:
          // that is precisely what lets the two share a variable.
          if (is_copy && v == in.src1.id) continue;
          interfere_[in.dst.id].insert(v);
          interfere_[v].insert(in.dst.id);
        }
      } else if (in.dst.id != kILNoValue) {
        live.erase(in.dst.id);
      }
      auto read = [&](const ILValue& v) {
        if (is_cand(v.id)) live.insert(v.id);
      };
      read(in.src1);
      read(in.src2);
      read(in.src3);
      if (is_cand(in.cmp_ref)) live.insert(in.cmp_ref);
    }
  }
}

void CDecompiler::CoalesceMergeCopies() {
  BuildInterference();
  // Interference of a merged class is the union of its members', so a class
  // that has absorbed a value cannot later merge with something that value
  // conflicts with.
  std::unordered_map<int, std::set<int>> class_interf;
  auto interf_of = [&](int root) -> std::set<int>& {
    auto it = class_interf.find(root);
    if (it != class_interf.end()) return it->second;
    auto& s = class_interf[root];
    auto src = interfere_.find(root);
    if (src != interfere_.end()) s = src->second;
    return s;
  };

  for (const auto& [dst_id, src_id] : merge_moves_) {
    int a = Root(dst_id), b = Root(src_id);
    if (a == b) continue;
    auto& ia = interf_of(a);
    auto& ib = interf_of(b);
    bool conflict = false;
    for (int v : ib) {
      if (Root(v) == a) { conflict = true; break; }
    }
    if (!conflict) {
      for (int v : ia) {
        if (Root(v) == b) { conflict = true; break; }
      }
    }
    if (conflict) continue;
    // Keep the lower id as the root so the surviving name is the one defined
    // first, which reads as the original variable.
    int keep = a < b ? a : b, drop = a < b ? b : a;
    coalesce_[drop] = keep;
    auto& merged = interf_of(keep);
    for (int v : interf_of(drop)) merged.insert(v);
    class_interf.erase(drop);
  }
}

void CDecompiler::IndexUses() {
  auto use = [&](const ILValue& v) {
    if (v.id != kILNoValue) use_count_[v.id]++;
  };
  for (const auto& b : func_.blocks) {
    for (const auto& in : b.instrs) {
      use(in.src1);
      use(in.src2);
      use(in.src3);
      if (in.dst.id != kILNoValue) {
        def_of_[in.dst.id] = &in;
        def_count_[in.dst.id]++;
      }
      // Merge copies join the two values into one variable. A merge move is
      // only ever emitted between the two halves of one register slot, so both
      // sides are the same source variable by construction - including the
      // slots that have no PPC register number of their own, like the
      // condition and vector state.
      // Every register-to-register copy, not only the ones inserted when
      // leaving SSA. `mr r25, r8` is the machine moving a value between
      // registers; there is no assignment in the source it came from, so the
      // two sides are one variable whenever their live ranges allow it.
      if (in.op == ILOp::Assign && in.dst.id != kILNoValue &&
          in.src1.id != kILNoValue) {
        merge_moves_.emplace_back(in.dst.id, in.src1.id);
      }
      // A branch consumes its compare result.
      if (in.cmp_ref != kILNoValue) use_count_[in.cmp_ref]++;
      if (in.dst.id != kILNoValue) def_types_.emplace_back(in.dst.id,
                                                           in.dst.type);
      if (in.op == ILOp::MoveSPR && in.spr_read && in.dst.id != kILNoValue)
        spr_values_.insert(in.dst.id);
    }
  }
  CoalesceMergeCopies();

  // One declared type per variable. Where the definitions in a coalesced web
  // disagree, the widest reading of the register wins and the definitions that
  // produce something else are cast at their assignment.
  for (const auto& [id, ty] : def_types_) {
    int root = Root(id);
    auto it = decl_type_.find(root);
    if (it == decl_type_.end()) {
      decl_type_[root] = ty;
      continue;
    }
    if (it->second == ty) continue;
    bool a_vec = ILTypeIsVector(it->second), b_vec = ILTypeIsVector(ty);
    if (b_vec && !a_vec) it->second = ty;
    else if (a_vec && b_vec) it->second = ILType::V128U;
  }

  // Anything read from r3-r10 before this function ever writes it is an
  // incoming argument.
  std::unordered_set<int> defined_regs;
  for (const auto& b : func_.blocks) {
    for (const auto& in : b.instrs) {
      auto note = [&](const ILValue& v) {
        if (v.id == kILNoValue) return;
        if (def_of_.count(v.id)) return;
        if (v.ppc_reg >= 3 && v.ppc_reg <= 10 && !defined_regs.count(v.ppc_reg))
          live_in_args_.insert(v.ppc_reg);
      };
      note(in.src1);
      note(in.src2);
      note(in.src3);
      if (in.dst.id != kILNoValue && in.dst.ppc_reg >= 0)
        defined_regs.insert(in.dst.ppc_reg);
    }
  }
  // A `return` implicitly reads r3. Without counting that use, the final
  // `li r3, 0` looks dead and DCE drops the return value.
  std::vector<const ILBasicBlock*> ordered;
  for (const auto& b : func_.blocks) ordered.push_back(&b);
  std::sort(ordered.begin(), ordered.end(),
            [](const ILBasicBlock* x, const ILBasicBlock* y) {
              return x->start_addr < y->start_addr;
            });
  // vr1 is the same thing for a function that returns a vector - a value left
  // there at the end is the result, not dead. Without this the load that
  // produces it has no users and DCE removes the only work the function does.
  static constexpr int kVRReturnReg = 64 + 1;
  int cur_r3 = kILNoValue;
  int cur_vr1 = kILNoValue;
  for (const ILBasicBlock* bp : ordered) {
    for (const auto& in : bp->instrs) {
      if (in.dst.id != kILNoValue && in.dst.ppc_reg == 3) cur_r3 = in.dst.id;
      if (in.dst.id != kILNoValue && in.dst.ppc_reg == kVRReturnReg)
        cur_vr1 = in.dst.id;
      if (in.op == ILOp::Return || in.op == ILOp::BranchLR) {
        if (cur_r3 != kILNoValue) use_count_[cur_r3]++;
        if (cur_vr1 != kILNoValue) use_count_[cur_vr1]++;
      }
    }
  }
}

void CDecompiler::CollectLabels() {
  for (const auto& b : func_.blocks) {
    for (const auto& in : b.instrs) {
      if (in.op == ILOp::Branch || in.op == ILOp::BranchCond) {
        if (in.target >= func_.entry && in.target < func_.end)
          label_targets_.insert(in.target);
      }
    }
  }
}

// Build the value expression for an instruction, or null if it has none.
ExprRef CDecompiler::BuildExpr(const ILInstr& in, bool* pure) {
  *pure = IsPure(in);
  ExprRef a = Get(in.src1);
  ExprRef b = Get(in.src2);
  ILType t = in.dst.type;

  switch (in.op) {
    case ILOp::Assign: return a ? a : MkConst((uint32_t)in.imm, t);
    case ILOp::Add:
      // addi with no RA is a plain constant load (li/lis).
      if (!a) return MkConst((uint32_t)in.imm, t);
      return b ? MkAdd(a, b) : MkAdd(a, MkConst((uint32_t)in.imm, t));
    case ILOp::Sub:
      return MkBin("-", a, b ? b : MkConst((uint32_t)in.imm, t), kPrecAdd, t);
    case ILOp::Neg: return MkUnary("-", a, t);
    case ILOp::Mul:
      return MkBin("*", a, b ? b : MkConst((uint32_t)in.imm, t), kPrecMul, t);
    case ILOp::Div: case ILOp::DivU:
      return MkBin("/", a, b, kPrecMul, t);
    case ILOp::And:
      return MkBin("&", BitsAsInt(a), b ? BitsAsInt(b) : MkConst((uint32_t)in.imm, t), kPrecBitAnd, t);
    case ILOp::Or:
      {
        ExprRef rb = b ? BitsAsInt(b) : MkConst((uint32_t)in.imm, t);
        ExprRef la = BitsAsInt(a);
        Resolver rs = [this](const ExprRef& x) -> ExprRef {
          return ResolveDef(x);
        };
        if (ExprRef sel = FoldSelect(la, rb, rs)) return sel;
        if (ExprRef sel = FoldSelect(rb, la, rs)) return sel;
        return MkBin("|", la, rb, kPrecBitOr, t);
      }
    case ILOp::Xor:
      return MkBin("^", BitsAsInt(a), b ? BitsAsInt(b) : MkConst((uint32_t)in.imm, t), kPrecBitXor, t);
    case ILOp::Not: return MkUnary("~", BitsAsInt(a), t);
    case ILOp::Andc: return MkBin("&", BitsAsInt(a), MkUnary("~", BitsAsInt(b)), kPrecBitAnd, t);
    case ILOp::Orc: return MkBin("|", BitsAsInt(a), MkUnary("~", BitsAsInt(b)), kPrecBitOr, t);
    case ILOp::Nand: return MkUnary("~", MkBin("&", BitsAsInt(a), BitsAsInt(b), kPrecBitAnd), t);
    case ILOp::Nor: return MkUnary("~", MkBin("|", BitsAsInt(a), BitsAsInt(b), kPrecBitOr), t);
    case ILOp::Eqv: return MkUnary("~", MkBin("^", BitsAsInt(a), BitsAsInt(b), kPrecBitXor), t);
    case ILOp::Shl:
      return MkBin("<<", BitsAsInt(a), b ? BitsAsInt(b) : MkConst((uint32_t)in.imm), kPrecShift, t);
    case ILOp::Shr:
      return MkBin(">>", BitsAsInt(a), b ? BitsAsInt(b) : MkConst((uint32_t)in.imm), kPrecShift, t);
    case ILOp::Sar: case ILOp::SarImm:
      return MkBin(">>", BitsAsInt(a), b ? BitsAsInt(b) : MkConst((uint32_t)in.imm), kPrecShift,
                   ILType::S32);
    // One vector instruction, spelled as the intrinsic it came from. Modelling
    // the lanes would not make it more readable - what matters is that the
    // value is defined and its operands are visible, so the dataflow through
    // the vector unit reads like the rest of the function.
    case ILOp::VectorOp: {
      auto e = std::make_shared<Expr>();
      e->kind = EK::Call;
      e->text = VectorOpName(in);
      if (a) e->args.push_back(a);
      if (b) e->args.push_back(b);
      if (auto c = Get(in.src3)) e->args.push_back(c);
      e->type = t;
      for (const auto& arg : e->args)
        if (arg && arg->reads_memory) e->reads_memory = true;
      return e;
    }
    case ILOp::VMXLoad: case ILOp::VMXLoad128: {
      auto e = std::make_shared<Expr>();
      e->kind = EK::Call;
      e->text = VectorOpName(in);
      if (a) e->args.push_back(a);
      if (b) e->args.push_back(b);
      e->type = t;
      e->reads_memory = true;
      return e;
    }
    case ILOp::ExtendByte: return MkUnary("(__int8)", a, ILType::S8);
    case ILOp::ExtendHalf: return MkUnary("(__int16)", a, ILType::S16);
    case ILOp::ExtendWord: return MkUnary("(int)", a, ILType::S32);
    case ILOp::CountLeadingZeros: {
      auto e = std::make_shared<Expr>();
      e->kind = EK::Call;
      e->text = "__lzcnt32";
      e->args.push_back(a);
      e->type = t;
      if (a) e->reads_memory = a->reads_memory;
      return e;
    }
    case ILOp::RotateAndMask: {
      // `cntlzw x; rlwinm r, 27, 31, 31` is this compiler's spelling of
      // (x == 0): cntlzw(0) is 32, and rotating that left by 27 leaves bit 0
      // set, while any non-zero input clears it. Recognising it removes a lot
      // of noise — the connection code uses it for every boolean.
      if (in.imm == 27 && in.mask == 1 && a && a->kind == EK::Call &&
          a->text == "__lzcnt32" && a->args.size() == 1) {
        ExprRef inner = a->args[0];
        // The compare is usually built as (x - k), so `(x + -k) == 0` is really
        // `x == k`. Undo the subtraction rather than printing 0xFFFFFFFB.
        if (inner && inner->kind == EK::Binary && inner->text == "+" &&
            inner->b && inner->b->kind == EK::Const) {
          uint32_t k = (uint32_t)(-(int32_t)(uint32_t)inner->b->cval);
          return MkBin("==", inner->a, MkConst(k), kPrecEq, ILType::Bool);
        }
        return MkBin("==", inner, MkConst(0), kPrecEq, ILType::Bool);
      }
      // Same shape with the subtract folded in: (x - k) == 0  ->  x == k.
      ExprRef rot = a;
      if (in.imm) {
        auto e = std::make_shared<Expr>();
        e->kind = EK::Call;
        e->text = "__rol32";
        e->args.push_back(a);
        e->args.push_back(MkConst((uint32_t)in.imm));
        if (a) e->reads_memory = a->reads_memory;
        rot = e;
      }
      if (in.mask == 0xFFFFFFFFu) return rot;
      return MkBin("&", rot, MkConst(in.mask), kPrecBitAnd, t);
    }
    case ILOp::Load: case ILOp::LoadUpdate: {
      ExprRef addr = a ? MkAdd(a, MkConst((uint32_t)(int32_t)in.imm))
                       : MkConst((uint32_t)(int32_t)in.imm);
      return MkDeref(addr, in.access_type);
    }
    case ILOp::LoadIndexed: {
      return MkDeref(MkAdd(a, b), in.access_type);
    }
    case ILOp::LoadReserved: {
      // lwarx/ldarx: an indexed load that also sets the reservation. Rendered
      // as an intrinsic rather than a plain deref so the pairing with stwcx.
      // survives, and so it is never folded/eliminated as a pure load.
      auto e = std::make_shared<Expr>();
      e->kind = EK::Call;
      e->text = in.access_type == ILType::U64 ? "__ldarx" : "__lwarx";
      // rA may be r0 (= literal 0), in which case the address is just rB.
      e->args.push_back(a && b ? MkAdd(a, b) : (b ? b : a));
      e->type = in.access_type;
      e->reads_memory = true;
      return e;
    }
    case ILOp::FLoad: case ILOp::FLoadUpdate: {
      ExprRef addr = a ? MkAdd(a, MkConst((uint32_t)(int32_t)in.imm))
                       : MkConst((uint32_t)(int32_t)in.imm);
      return MkDeref(addr, in.access_type);
    }
    case ILOp::FLoadIndexed: return MkDeref(MkAdd(a, b), in.access_type);
    case ILOp::FAdd: return MkBin("+", a, b, kPrecAdd, t);
    case ILOp::FSub: return MkBin("-", a, b, kPrecAdd, t);
    case ILOp::FMul: return MkBin("*", a, b, kPrecMul, t);
    case ILOp::FDiv: return MkBin("/", a, b, kPrecMul, t);
    case ILOp::FNeg: return MkUnary("-", a, t);
    case ILOp::FMove: return a;
    case ILOp::CompareImm:
      return MkCall("__cmp", {a, MkConst((uint32_t)(int32_t)in.imm)},
                    ILType::Bool);
    case ILOp::CompareImmU:
      return MkCall("__cmp", {a, MkConst((uint32_t)in.imm)}, ILType::Bool);
    case ILOp::Compare: case ILOp::CompareU:
    case ILOp::FCompareOrdered: case ILOp::FCompareUnordered:
      // Not C: a CR-setting compare whose result the branch re-derives. Spelled
      // as an intrinsic so nobody reads it as an expression.
      return MkCall("__cmp", {a, b}, ILType::Bool);
    // ---- integer arithmetic with carry / wide multiply ----
    case ILOp::AddCarry:  // adde: rD = rA + rB + CA
      return MkBin("+", MkBin("+", a, b, kPrecAdd, t), MkName("CA"), kPrecAdd,
                   t);
    case ILOp::SubCarry:  // subfe: rD = ~rA + rB + CA  ==  rB - rA - !CA
      return MkBin("-", MkBin("-", b, a, kPrecAdd, t),
                   MkUnary("!", MkName("CA"), ILType::Bool), kPrecAdd, t);
    case ILOp::MulHigh: return MkCall("__mulhw", {a, b}, ILType::S32);
    case ILOp::MulHighU: return MkCall("__mulhwu", {a, b}, ILType::U32);

    // ---- rotate/mask forms ----
    case ILOp::RotateAndMaskReg: {  // rlwnm: rotate by rB, then mask
      ExprRef rot = MkCall("__rotl32", {a, b}, ILType::U32);
      return MkBin("&", rot, MkConst(in.mask), kPrecBitAnd, ILType::U32);
    }
    case ILOp::RotateInsert: {  // rlwimi: splice rotated bits into the old value
      ExprRef rot =
          MkCall("__rotl32", {a, MkConst((uint32_t)in.imm)}, ILType::U32);
      ExprRef ins = MkBin("&", rot, MkConst(in.mask), kPrecBitAnd, ILType::U32);
      // The complement stays 32-bit: MkConst takes a uint64_t, and widening a
      // bare ~mask would carry ones into the high half of a U32 constant.
      ExprRef keep = MkBin("&", b, MkConst((uint32_t)~in.mask), kPrecBitAnd,
                           ILType::U32);
      return MkBin("|", keep, ins, kPrecBitOr, ILType::U32);
    }

    // ---- special-register / timebase reads ----
    case ILOp::LoadTimeBase:
      return MkCall(in.imm == 269 ? "__mftbu" : "__mftb", {}, t, true);
    case ILOp::MoveCR:
      if (!in.spr_read) return nullptr;  // mtcrf -> statement form
      return MkCall("__mfcr", {}, ILType::U32, true);
    case ILOp::LoadFPSCR: return MkCall("__mffs", {}, ILType::F64, true);

    // ---- byte-reversed load ----
    case ILOp::LoadByteReverse:
      return MkCall(in.access_type == ILType::U16 ? "__lhbrx" : "__lwbrx",
                    {a && b ? MkAdd(a, b) : (b ? b : a)}, in.access_type, true);

    // ---- floating point ----
    case ILOp::FAbs: return MkCall("fabs", {a}, t);
    case ILOp::FNAbs: return MkUnary("-", MkCall("fabs", {a}, t), t);
    case ILOp::FSqrt: return MkCall("sqrt", {a}, t);
    case ILOp::FRecipEst: return MkCall("__fres", {a}, t);
    case ILOp::FRSqrtEst: return MkCall("__frsqrte", {a}, t);
    case ILOp::FRoundSingle: return MkUnary("(float)", a, ILType::F32);
    case ILOp::FConvFromInt: return MkUnary("(double)", a, ILType::F64);
    case ILOp::FConvToIntWord:
    case ILOp::FConvToIntWordTrunc: return MkUnary("(int)", a, ILType::S32);
    case ILOp::FConvToIntDbl:
    case ILOp::FConvToIntDblTrunc: return MkUnary("(__int64)", a, ILType::S64);
    // fmadd family: FRT = FRA*FRC (+/-) FRB, so src2 = FRC and src3 = FRB.
    case ILOp::FMulAdd:
      return MkBin("+", MkBin("*", a, b, kPrecMul, t), Get(in.src3), kPrecAdd,
                   t);
    case ILOp::FMulSub:
      return MkBin("-", MkBin("*", a, b, kPrecMul, t), Get(in.src3), kPrecAdd,
                   t);
    case ILOp::FNMulAdd:
      return MkUnary("-",
                     MkBin("+", MkBin("*", a, b, kPrecMul, t), Get(in.src3),
                           kPrecAdd, t),
                     t);
    case ILOp::FNMulSub:
      return MkUnary("-",
                     MkBin("-", MkBin("*", a, b, kPrecMul, t), Get(in.src3),
                           kPrecAdd, t),
                     t);
    case ILOp::FSelect: {  // fsel: FRT = (FRA >= 0.0) ? FRC : FRB
      auto e = std::make_shared<Expr>();
      e->kind = EK::Raw;
      e->text = Render(a, kPrecRel) + " >= 0.0 ? " + Render(b, kPrecAssign) +
                " : " + Render(Get(in.src3), kPrecAssign);
      e->type = t;
      e->prec = kPrecAssign;
      return e;
    }

    case ILOp::MoveMSR: {
      // mfmsr reads the machine state register; mtmsr/mtmsrd write it (the
      // write is emitted as a statement, not an expression). Named after the
      // XDK/VS PPC intrinsics.
      if (!in.spr_read) return nullptr;
      auto e = std::make_shared<Expr>();
      e->kind = EK::Call;
      e->text = "__mfmsr";
      e->type = ILType::U32;
      e->reads_memory = true;
      return e;
    }
    case ILOp::MoveSPR:
      return MkName(in.spr == ILSPR::LR ? "LR"
                    : in.spr == ILSPR::CTR ? "CTR" : "SPR", t);
    case ILOp::SetCond: {
      // x86 setcc: a relational rendered as a value, e.g. dst = (a < b).
      if (!a || !b) return nullptr;
      const char* op = CmpOp(in.cmp_kind);
      int prec = CmpPrec(in.cmp_kind);
      auto e = std::make_shared<Expr>();
      e->kind = EK::Raw;
      e->text = "(" + RenderChild(a, prec) + " " + op + " " +
                RenderChild(b, prec) + ")";
      e->type = ILType::Bool;
      e->prec = kPrecPrimary;
      return e;
    }
    default:
      return nullptr;
  }
}

// Render the condition for a conditional branch by fusing in its compare.
std::string CDecompiler::CondText(const ILInstr& br, bool invert) {
  ILCmpKind k = invert ? InvertCmp(br.cmp_kind) : br.cmp_kind;
  const char* op = CmpOp(k);
  int prec = CmpPrec(k);

  if (br.cmp_ref != kILNoValue) {
    auto dit = def_of_.find(br.cmp_ref);
    if (dit != def_of_.end()) {
      const ILInstr& c = *dit->second;
      ExprRef lhs = Get(c.src1);
      ExprRef rhs;
      if (c.op == ILOp::CompareImm)
        rhs = MkConst((uint32_t)(int32_t)c.imm);
      else if (c.op == ILOp::CompareImmU)
        rhs = MkConst((uint32_t)c.imm);
      else
        rhs = Get(c.src2);
      if (lhs && rhs) {
        // Signed compares read more naturally with an explicit cast when the
        // value came from an unsigned context.
        return RenderChild(lhs, prec) + " " + op + " " + RenderChild(rhs, prec);
      }
    }
    // Compare we couldn't resolve — fall back to the SSA name.
    return std::string("v") + std::to_string(br.cmp_ref) + " " + op + " 0";
  }
  return std::string("CR") + std::to_string(br.cr_field) + " " + op + " 0";
}

std::string CDecompiler::Run() {
  g_field_uses.clear();
  g_ptr_uses.clear();
  g_arg_flows.clear();
  g_string_refs.clear();
  g_gfield_uses.clear();
  g_data_refs.clear();
  g_base_param_origins.clear();
  g_called_functions.clear();
  g_store_dest = false;
  IndexUses();
  MarkOutgoingStackArgs();
  CollectLabels();

  // Locals that are copies of an incoming argument. The prologue parks a
  // pointer parameter in a callee-saved register and everything downstream
  // dereferences that copy, so a type declared for the parameter has to be
  // carried across to the local or it never reaches the code that uses it.
  for (const auto& b : func_.blocks) {
    for (const auto& in : b.instrs) {
      if (in.op != ILOp::Assign || in.dst.id == kILNoValue) continue;
      const ILValue& s = in.src1;
      if (s.id == kILNoValue) continue;
      if (s.ppc_reg < 3 || s.ppc_reg > 10) continue;
      // Only a genuine incoming value: something this function defined itself
      // and parked in r3..r10 is an outgoing argument, not a parameter.
      if (def_of_.count(s.id)) continue;
      BaseParamOrigin o;
      o.base = VarName(in.dst.id);
      o.position = s.ppc_reg - 2;
      g_base_param_origins.push_back(o);
    }
  }
  // Arguments past the eighth arrive on the stack, above our frame and the
  // register save area. They are parameters exactly like a1..a8, and their
  // declared types are just as usable - but a scan of r3..r10 never sees them,
  // which is why a struct named in the prototype could still come out as an
  // inferred layout.
  if (func_.stack_frame_size) {
    // A slot the function writes to is its own spill, not an incoming
    // argument: the caller's parameter area is read-only from in here. Without
    // this, a spill just below the parameter area takes the first argument
    // slot and shifts every real one up by one - which is what put pRect's
    // declared type on Flags.
    std::set<uint32_t> written;
    for (const auto& b : func_.blocks)
      for (const auto& in : b.instrs)
        if ((in.op == ILOp::Store || in.op == ILOp::StoreUpdate ||
             in.op == ILOp::FStore) &&
            in.src1.ppc_reg == 1 && in.imm > 0)
          written.insert((uint32_t)in.imm);
    std::set<uint32_t> stack_offsets;
    for (const auto& b : func_.blocks)
      for (const auto& in : b.instrs)
        if ((in.op == ILOp::Load || in.op == ILOp::LoadUpdate ||
             in.op == ILOp::FLoad) &&
            in.src1.ppc_reg == 1 && in.imm > 0 &&
            (uint32_t)in.imm >= func_.stack_frame_size + 0x40 &&
            !written.count((uint32_t)in.imm))
          stack_offsets.insert((uint32_t)in.imm);
    // Argument slots are 8 bytes apart, and a 32-bit value is read from either
    // half of its slot - so counting distinct offsets in order numbers the
    // halves as separate arguments and shifts everything after them. Position
    // comes from the distance to the first slot instead.
    std::map<uint32_t, int> position_of;
    if (!stack_offsets.empty()) {
      uint32_t first = *stack_offsets.begin();
      for (uint32_t off : stack_offsets)
        position_of[off] = 9 + (int)((off - first) / 8);
    }
    if (getenv("XEXMAGIC_TRACE_BIND")) {
      printf("[stack] frame=0x%X slots:", func_.stack_frame_size);
      for (const auto& kv : position_of)
        printf(" 0x%X=a%d", kv.first, kv.second);
      printf("\n");
      fflush(stdout);
    }
    for (const auto& b : func_.blocks) {
      for (const auto& in : b.instrs) {
        if (in.op != ILOp::Load && in.op != ILOp::LoadUpdate &&
            in.op != ILOp::FLoad)
          continue;
        if (in.src1.ppc_reg != 1 || in.dst.id == kILNoValue) continue;
        auto pit = position_of.find((uint32_t)in.imm);
        if (pit == position_of.end()) continue;
        BaseParamOrigin o;
        o.base = VarName(in.dst.id);
        o.position = pit->second;
        g_base_param_origins.push_back(o);
      }
    }
  }

  // Carry the origin across merge points. `v247` is a phi of "the pPoint the
  // caller passed" and "a default the function built on its own stack", and it
  // is the phi that gets dereferenced - so without this the declared type
  // stops one step short of the code that uses it. One typed arm is enough:
  // the other arm is a locally built object of the very same shape, which is
  // why the compiler could merge them in the first place.
  {
    std::map<std::string, int> by_base;
    for (const auto& o : g_base_param_origins) by_base[o.base] = o.position;
    bool changed = true;
    for (int round = 0; round < 8 && changed; round++) {
      changed = false;
      for (const auto& [dst_id, src_id] : merge_moves_) {
        std::string dst_base = VarName(dst_id), src_base = VarName(src_id);
        auto it = by_base.find(src_base);
        if (it == by_base.end() || by_base.count(dst_base)) continue;
        by_base[dst_base] = it->second;
        changed = true;
      }
    }
    g_base_param_origins.clear();
    for (const auto& [base, pos] : by_base)
      g_base_param_origins.push_back({base, pos});
  }

  // Incoming arguments start out live in their registers.
  for (int r : live_in_args_) {
    ILValue v;
    v.id = kILNoValue;
    v.ppc_reg = r;
    cur_arg_val_[r] = MkName(NameForLiveIn(v));
  }

  std::ostringstream out;
  std::vector<Stmt> body;

  auto emit = [&](const std::string& s, uint32_t addr,
                  const std::string& pure_def = std::string()) {
    body.push_back({s, addr, pure_def});
  };

  // Field-type pre-pass: fix every struct field's declared (widest) type BEFORE
  // any body renders. Otherwise an integer read that renders before a wider float
  // access at the same offset misses the bit-reinterpret, and the field -- later
  // widened to float -- breaks its integer uses (`f & 1`, `f >> 1`: E1079). This
  // mirrors the Deref field-inference (base = a v/a pointer var + small constant
  // offset) so the two agree on which offsets are fields and how wide.
  {
    auto isVA = [](const std::string& n) {
      if (n.size() < 2 || (n[0] != 'v' && n[0] != 'a')) return false;
      for (size_t i = 1; i < n.size(); i++)
        if (!std::isdigit((unsigned char)n[i])) return false;
      return true;
    };
    for (const auto& b : func_.blocks) {
      for (const ILInstr& in : b.instrs) {
        switch (in.op) {
          case ILOp::Load: case ILOp::LoadUpdate:
          case ILOp::Store: case ILOp::StoreUpdate:
          case ILOp::FLoad: case ILOp::FLoadUpdate:
          case ILOp::FStore: case ILOp::FStoreUpdate: break;
          default: continue;
        }
        if (in.src1.id == kILNoValue || !(in.imm > 0 && in.imm < 0x10000))
          continue;
        std::string base = VarName(in.src1.id);
        if (!isVA(base)) continue;
        auto& fields = g_field_uses[base];
        auto it = fields.find((uint32_t)in.imm);
        if (it == fields.end() || TypeSize(in.access_type) > TypeSize(it->second))
          fields[(uint32_t)in.imm] = in.access_type;
      }
    }
  }

  // Blocks in address order so the listing follows the original layout.
  std::vector<const ILBasicBlock*> blocks;
  for (const auto& b : func_.blocks) blocks.push_back(&b);
  std::sort(blocks.begin(), blocks.end(),
            [](const ILBasicBlock* x, const ILBasicBlock* y) {
              return x->start_addr < y->start_addr;
            });

  for (const ILBasicBlock* bp : blocks) {
    const ILBasicBlock& b = *bp;
    if (label_targets_.count(b.start_addr)) {
      char lbl[64];
      snprintf(lbl, sizeof(lbl), "LABEL_%08X:", b.start_addr);
      emit(lbl, b.start_addr);
    }

    for (const ILInstr& in : b.instrs) {
      if (IsFrameNoise(in)) {
        // Still record the value so anything referencing it resolves.
        if (in.dst.id != kILNoValue && in.op == ILOp::MoveSPR)
          expr_of_[in.dst.id] = MkName("LR");
        continue;
      }
      if (in.dst.id != kILNoValue && in.dst.ppc_reg == 3) last_r3_ = in.dst.id;
      if (in.dst.id != kILNoValue && in.dst.ppc_reg == 64 + 1)
        last_vr1_ = in.dst.id;

      switch (in.op) {
        case ILOp::Nop:
          continue;

        case ILOp::Store: case ILOp::StoreUpdate: {
          ExprRef base = Get(in.src1);
          ExprRef val = Get(in.src2);
          ExprRef addr = base ? MkAdd(base, MkConst((uint32_t)(int32_t)in.imm))
                              : MkConst((uint32_t)(int32_t)in.imm);
          const std::string stmt =
              RenderStoreDest(MkDeref(addr, in.access_type)) + " = " +
              Render(val, kPrecAssign) + ";";
          // Staging an argument for the call below, not a store. Held until
          // the call decides; if it turns out not to be one, this is emitted
          // unchanged.
          if (stack_arg_stores_.count(&in)) {
            pending_stack_args_[(uint32_t)in.imm] = {val, stmt, in.address};
            continue;
          }
          emit(stmt, in.address);
          barrier_++;
          continue;
        }
        case ILOp::StoreIndexed: {
          ExprRef addr = MkAdd(Get(in.src1), Get(in.src2));
          emit(RenderStoreDest(MkDeref(addr, in.access_type)) + " = " +
                   Render(Get(in.src3), kPrecAssign) + ";",
               in.address);
          barrier_++;
          continue;
        }
        case ILOp::MoveCR: {
          // mtcrf writes CR fields — no destination, so emit as a statement.
          if (in.spr_read) break;  // mfcr: value-producing, handled below
          char m[16];
          snprintf(m, sizeof(m), "0x%X", in.mask);
          emit(std::string("__mtcrf(") + m + ", " +
                   Render(Get(in.src1), kPrecAssign) + ");",
               in.address);
          barrier_++;
          continue;
        }
        case ILOp::DCacheTouch: case ILOp::DCacheTouchStore: {
          // dcbt/dcbtst are prefetch hints: no value, but keep them visible.
          ExprRef base = Get(in.src1);
          ExprRef idx = Get(in.src2);
          ExprRef addr = (base && idx) ? MkAdd(base, idx) : (idx ? idx : base);
          emit(std::string(in.op == ILOp::DCacheTouch ? "__dcbt(" : "__dcbtst(") +
                   (addr ? Render(addr, kPrecAssign) : "") + ");",
               in.address);
          continue;
        }
        case ILOp::StoreFPSCR: {
          emit("__mtfsf(" + Render(Get(in.src1), kPrecAssign) + ");",
               in.address);
          barrier_++;
          continue;
        }
        case ILOp::LoadMultiple: case ILOp::StoreMultiple: {
          // lmw/stmw move a run of registers; there is no single C expression
          // for it, so keep the instruction verbatim.
          if (!in.asm_text.empty())
            emit("__asm { " + in.asm_text + " }", in.address);
          barrier_++;
          continue;
        }
        case ILOp::CRAnd: case ILOp::CROr: case ILOp::CRXor:
        case ILOp::CRNand: case ILOp::CRNor: case ILOp::CRAndc:
        case ILOp::CROrc: case ILOp::CREqv: case ILOp::MoveCRField: {
          // CR-bit logic operates on individual condition bits, which the IL
          // doesn't model as values — emit verbatim rather than drop it.
          if (!in.asm_text.empty())
            emit("__asm { " + in.asm_text + " }", in.address);
          barrier_++;
          continue;
        }
        case ILOp::VMXOp: {
          // Vector ops aren't modelled as expressions — emit the instruction so
          // the code isn't silently missing work.
          if (!in.asm_text.empty())
            emit("__asm { " + in.asm_text + " }", in.address);
          barrier_++;
          continue;
        }
        case ILOp::VMXStore: case ILOp::VMXStore128: {
          std::string s = VectorOpName(in) + "(";
          if (auto base = Get(in.src1)) s += Render(base, kPrecAssign);
          else s += "0";
          if (auto idx = Get(in.src2)) s += ", " + Render(idx, kPrecAssign);
          if (auto val = Get(in.src3)) s += ", " + Render(val, kPrecAssign);
          emit(s + ");" + OpAnnotation(in), in.address);
          barrier_++;
          continue;
        }
        case ILOp::BranchDecrCTRZero: case ILOp::BranchDecrCTRNonZero: {
          // bdz/bdnz: decrement CTR, branch on the result.
          char lbl[80];
          snprintf(lbl, sizeof(lbl), "if ( --CTR %s 0 )",
                   in.op == ILOp::BranchDecrCTRZero ? "==" : "!=");
          std::string s = lbl;
          if (!label_targets_.count(in.target)) {
            emit(s + " " + RenderTailCall(in), in.address);
          } else {
            char tgt[64];
            snprintf(tgt, sizeof(tgt), " goto LABEL_%08X;", in.target);
            emit(s + tgt, in.address);
          }
          continue;
        }
        case ILOp::MoveMSR: {
          // The write form has no destination — emitted as a statement so the
          // interrupt-enable/disable it performs stays visible.
          if (in.spr_read) break;  // read form: value-producing, handled below
          emit("__mtmsr(" + Render(Get(in.src1), kPrecAssign) + ");",
               in.address);
          barrier_++;
          continue;
        }
        case ILOp::StoreConditional: {
          // stwcx./stdcx.: conditional store paired with lwarx. Emitted as an
          // intrinsic so the reservation pairing is visible; it also sets CR0,
          // which the following branch reads.
          ExprRef base = Get(in.src1);
          ExprRef idx = Get(in.src2);
          ExprRef addr = (base && idx) ? MkAdd(base, idx) : (idx ? idx : base);
          std::string fn =
              in.access_type == ILType::U64 ? "__stdcx" : "__stwcx";
          emit("CR0 = " + fn + "(" + Render(addr, kPrecAssign) + ", " +
                   Render(Get(in.src3), kPrecAssign) + ");",
               in.address);
          barrier_++;
          continue;
        }
        case ILOp::FStore: case ILOp::FStoreUpdate: {
          ExprRef base = Get(in.src1);
          ExprRef addr = base ? MkAdd(base, MkConst((uint32_t)(int32_t)in.imm))
                              : MkConst((uint32_t)(int32_t)in.imm);
          emit(RenderStoreDest(MkDeref(addr, in.access_type)) + " = " +
                   Render(Get(in.src2), kPrecAssign) + ";",
               in.address);
          barrier_++;
          continue;
        }

        case ILOp::Call: case ILOp::CallCTR: {
          // Prefer the live global symbol map so an interactive rename of the
          // callee propagates to every call site on re-decompile, rather than
          // the name baked into the IL at build time.
          std::string callee;
          if (in.op != ILOp::CallCTR) {
            auto sit = GlobalSyms().find(in.target);
            if (sit != GlobalSyms().end()) callee = sit->second;
          }
          if (callee.empty()) callee = in.call_name;
          if (callee.empty()) {
            if (in.op == ILOp::CallCTR) {
              callee = "(*(int (**)())CTR)";
            } else if (in.target == 0) {
              // A target that folded to zero is a decompilation failure, not a
              // function at address 0. Say so rather than inventing sub_0.
              callee = "(*(int (**)())0 /*unresolved*/)";
            } else {
              char nb[32];
              snprintf(nb, sizeof(nb), "sub_%08X", in.target);
              callee = nb;
            }
          }
          // Arguments past the eighth were staged in the outgoing parameter
          // area. They only exist once r3..r10 are full, so anything staged
          // without a full register set was a local after all and is emitted
          // as the store it is.
          int reg_args = 0;
          for (int rr = 3; rr <= 10; rr++) {
            if (!cur_arg_val_.count(rr)) break;
            reg_args++;
          }
          std::vector<ExprRef> stack_args;
          if (!pending_stack_args_.empty()) {
            if (reg_args == 8) {
              // Ascending offset is parameter order.
              for (auto& kv : pending_stack_args_)
                stack_args.push_back(kv.second.value);
            } else {
              for (auto& kv : pending_stack_args_) {
                emit(kv.second.stmt, kv.second.address);
                barrier_++;
              }
            }
            pending_stack_args_.clear();
          }

          auto ce = std::make_shared<Expr>();
          ce->kind = EK::Call;
          // A call to a C++ symbol must spell an identifier, not the whole
          // demangled signature.
          ce->text = callee[0] == '(' ? callee : CodeIdent(callee);
          if (callee[0] != '(') {
            int& argc = g_called_functions[ce->text];
            int here = 0;
            for (int rr = 3; rr <= 10; rr++) {
              if (!cur_arg_val_.count(rr)) break;
              here++;
            }
            if (here > argc) argc = here;
          }
          ce->reads_memory = true;
          // Arity isn't recoverable without interprocedural analysis, so take
          // the contiguous run of argument registers r3.. that currently hold a
          // value; that is what the caller just set up.
          for (int r = 3; r <= 10; r++) {
            auto vit = cur_arg_val_.find(r);
            if (vit == cur_arg_val_.end()) break;
            ce->args.push_back(vit->second);
            // Record the flow when a plain variable is passed straight through:
            // the callee's parameter type pins this variable's type.
            const ExprRef& av = vit->second;
            // `base` or `base + 0xNN`. The offset form matters as much as the
            // plain one: passing `v9 + 0xE0` to a parameter declared
            // `TexelDesc *` proves the frame holds a TexelDesc at +0xE0, and
            // dropping it is what leaves those fields as field_XX.
            const ExprRef* name_e = &av;
            uint32_t field_off = 0;
            if (av && av->kind == EK::Binary && av->text == "+" && av->a &&
                av->b && av->b->kind == EK::Const) {
              name_e = &av->a;
              field_off = (uint32_t)av->b->cval;
            }
            const ExprRef& ne = *name_e;
            if (ne && ne->kind == EK::Name && ne->text.size() >= 2 &&
                (ne->text[0] == 'a' || ne->text[0] == 'v')) {
              bool digits = true;
              for (size_t i = 1; i < ne->text.size(); i++)
                if (!std::isdigit((unsigned char)ne->text[i])) digits = false;
              if (digits) {
                ArgFlow fl;
                fl.callee = callee;
                fl.callee_target = in.target;
                fl.position = r - 3;
                fl.arg = ne->text;
                fl.offset = field_off;
                g_arg_flows.push_back(fl);
              }
            }
          }
          // Arguments nine and up, staged in the outgoing parameter area.
          // Ascending offset is parameter order, so they follow the eight
          // register arguments as they stand.
          for (const ExprRef& sa : stack_args) ce->args.push_back(sa);
          if (!stack_args.empty() && callee[0] != '(') {
            int& argc = g_called_functions[ce->text];
            const int total = 8 + (int)stack_args.size();
            if (total > argc) argc = total;
          }
          std::string rendered = Render(ce, kPrecAssign);

          // Always bind the result to a temporary. The IL doesn't model call
          // operands, so a result consumed only as a later call's argument
          // looks unused here — emitting it bare would leave that argument
          // referring to a variable that was never assigned.
          if (in.dst.id != kILNoValue) {
            std::string vn = VarName(in.dst.id);
            locals_.push_back(std::string(CTypeName(in.dst.type)) + " " + vn +
                              ";");
            emit(vn + " = " + rendered + ";", in.address);
            expr_of_[in.dst.id] = MkName(vn, in.dst.type);
          } else {
            emit(rendered + ";", in.address);
          }
          barrier_++;
          // A call clobbers r4-r12; r3 becomes its result.
          for (int r = 4; r <= 10; r++) cur_arg_val_.erase(r);
          if (in.dst.id != kILNoValue)
            cur_arg_val_[3] = MkName(VarName(in.dst.id));
          else
            cur_arg_val_.erase(3);
          continue;
        }

        case ILOp::Return: case ILOp::BranchLR: {
          // The return value is whatever r3 currently holds - or vr1, for a
          // function whose result is a vector and which therefore never
          // touches r3 at all.
          int ret_id = last_r3_;
          int ret_reg = 3;
          if (ret_id == kILNoValue && last_vr1_ != kILNoValue) {
            ret_id = last_vr1_;
            ret_reg = 64 + 1;
          }
          if (ret_id != kILNoValue) {
            ILValue rv;
            rv.id = ret_id;
            rv.ppc_reg = ret_reg;
            auto it = def_of_.find(ret_id);
            if (it != def_of_.end()) rv.type = it->second->dst.type;
            ExprRef e = Get(rv);
            emit("return " + Render(e, kPrecAssign) + ";", in.address);
          } else {
            emit("return;", in.address);
          }
          continue;
        }

        case ILOp::Branch: {
          // A `b` whose target isn't a label inside this function is a tail
          // call (or tail restore), not an intra-function goto. Emitting a
          // `goto LABEL_X` here would dangle — there is no such label.
          if (!label_targets_.count(in.target)) {
            emit(RenderTailCall(in), in.address);
            continue;
          }
          // A jump to the very next instruction is a no-op; printing it makes
          // straight-line code look like control flow.
          if (in.target == in.address + 4) continue;
          char lbl[64];
          snprintf(lbl, sizeof(lbl), "goto LABEL_%08X;", in.target);
          emit(lbl, in.address);
          continue;
        }

        case ILOp::BranchCond: {
          // std::string, not a fixed buffer: a long condition (e.g. a nested
          // bit-test) must not be truncated -- truncation left a dangling operator
          // and spliced the goto into the expression.
          std::string s = "if ( " + CondText(in, false) + " )";
          // Conditional branch out of the function is a conditional tail call.
          if (!label_targets_.count(in.target)) {
            emit(s + " " + RenderTailCall(in), in.address);
            continue;
          }
          char tgt[64];
          snprintf(tgt, sizeof(tgt), " goto LABEL_%08X;", in.target);
          emit(s + tgt, in.address);
          continue;
        }

        default: break;
      }

      // Value-producing instruction.
      bool pure = true;
      ExprRef e = BuildExpr(in, &pure);
      if (!e) {
        if (in.op == ILOp::InlineAsm && !in.asm_text.empty()) {
          emit("__asm { " + in.asm_text + " }", in.address);
          continue;
        }
        // No expression was built, but the instruction still defines a value.
        // Dropping it silently leaves every later use referring to a variable
        // that was never assigned (the lwarx bug). Emit an explicit intrinsic
        // so the definition exists and the gap is visible.
        if (in.dst.id != kILNoValue) {
          std::string vn = VarName(in.dst.id);
          std::string rhs = std::string("__") + ILOpName(in.op) + "(";
          bool first = true;
          for (const ILValue* s : {&in.src1, &in.src2, &in.src3}) {
            if (s->id == kILNoValue && s->ppc_reg < 0) continue;
            ExprRef se = Get(*s);
            if (!se) continue;
            if (!first) rhs += ", ";
            first = false;
            rhs += Render(se, kPrecAssign);
          }
          rhs += ")";
          locals_.push_back(std::string(CTypeName(in.dst.type)) + " " + vn +
                            ";");
          emit(vn + " = " + rhs + ";", in.address);
          expr_of_[in.dst.id] = MkName(vn, in.dst.type);
          def_barrier_[in.dst.id] = barrier_;
          if (in.dst.ppc_reg >= 3 && in.dst.ppc_reg <= 10)
            cur_arg_val_[in.dst.ppc_reg] = expr_of_[in.dst.id];
        }
        continue;
      }
      if (in.dst.id == kILNoValue) continue;

      // Track argument registers before DCE. The IL doesn't model call
      // operands, so `mr r3, r31` setting up an argument has no IL use and
      // would otherwise be eliminated — taking the call's argument with it.
      if (in.dst.ppc_reg >= 3 && in.dst.ppc_reg <= 10)
        cur_arg_val_[in.dst.ppc_reg] = e;

      if (e) orig_expr_[in.dst.id] = e;
      int uses = use_count_[in.dst.id];
      bool single_def = def_count_[in.dst.id] <= 1;
      // Zero uses means dead whatever the definition count: nothing reads it,
      // so dropping every arm's assignment is safe (this is what keeps unused
      // merge values from littering the output).
      if (uses == 0 && pure && !opts_.no_dce) {
        continue;  // dead
      }
      // Copy propagation: a bare name or constant costs nothing to duplicate,
      // so fold it however many times it's read (kills `v4 = a1;` chains that
      // come from callee-saved register shuffling).
      bool trivial = e && (e->kind == EK::Name || e->kind == EK::Const);
      if ((uses <= 1 || trivial) && pure && !opts_.no_fold && single_def) {
        expr_of_[in.dst.id] = e;
        def_barrier_[in.dst.id] = barrier_;
        if (in.dst.ppc_reg >= 3 && in.dst.ppc_reg <= 10)
          cur_arg_val_[in.dst.ppc_reg] = e;
        continue;  // folded into its single consumer
      }
      std::string vn = VarName(in.dst.id);
      std::string rendered = Render(e, kPrecAssign);
      if (rendered == vn) {
        // `vN = vN;` -- a merge arm that already holds the value. Nothing to
        // say, and it is pure noise in the output.
        expr_of_[in.dst.id] = MkName(vn, in.dst.type);
        def_barrier_[in.dst.id] = barrier_;
        continue;
      }
      ILType decl = DeclType(in.dst.id);
      locals_.push_back(std::string(CTypeName(decl)) + " " + vn + ";");
      // The variable is declared once; a definition that reads the register
      // differently says so with a cast instead of being mislabelled.
      if (decl != in.dst.type && ILTypeIsVector(decl))
        rendered = "(" + std::string(CTypeName(decl)) + ")" + rendered;
      emit(vn + " = " + rendered + ";" + OpAnnotation(in), in.address,
           pure ? vn : std::string());
      expr_of_[in.dst.id] = MkName(vn, in.dst.type);
      def_barrier_[in.dst.id] = barrier_;
      if (in.dst.ppc_reg >= 3 && in.dst.ppc_reg <= 10)
        cur_arg_val_[in.dst.ppc_reg] = expr_of_[in.dst.id];
    }
  }

  // ---- assemble ----
  // Inferred struct types: a pointer base dereferenced at constant offsets
  // becomes `Struct_t *`, with one field per observed offset/width.
  // Declarations for everything this function calls, so the emitted C names
  // its dependencies instead of relying on the reader to know them. Real
  // prototypes where the SDK declares one; the rest are built from the call.
  if (!g_callee_decls.empty()) {
    out << "#include <stdint.h>\n\n";
    for (const auto& kv : g_callee_decls) out << kv.second << "\n";
    out << "\n";
  }

  {
    std::set<std::string> printed;
    for (const auto& kv : g_field_uses) {
      if (kv.second.empty()) continue;
      // A base bound to a type the SDK declares prints the declaration, not a
      // reconstruction of it: real name, real field names, real field types,
      // and the fields the code never touched as well.
      auto dit = g_struct_decls.find(kv.first);
      if (dit != g_struct_decls.end()) {
        if (printed.insert(dit->second).second) out << dit->second;
        continue;
      }
      // No name was found for this one. Saying "inferred" implies an
      // inference succeeded; what actually happened is that the layout was
      // reconstructed from the offsets the code touches and nothing in the
      // headers matched it.
      out << "struct " << StructNameFor(kv.first)
          << " {  // UNIDENTIFIED - no matching type in the headers\n";
      for (const auto& fo : kv.second) {
        out << "  " << CTypeName(fo.second) << " "
            << FieldName(kv.first, fo.first) << ";";
        char oc[24];
        snprintf(oc, sizeof(oc), "  // +0x%X", fo.first);
        out << oc << "\n";
      }
      out << "};\n";
    }
  }
  if (!g_field_uses.empty()) out << "\n";

  // Inferred type spelling for a variable name from how it is dereferenced:
  //   struct base (field accesses) -> "struct Tag *"
  //   bare pointer deref           -> "<inner> *"
  //   otherwise                    -> "int"
  auto type_of = [&](const std::string& name) -> std::string {
    return FullType(name);
  };
  // Declaration "TYPE name", tightening the spacing for pointer types.
  auto decl_of = [&](const std::string& name) -> std::string {
    std::string t = type_of(name);
    if (!t.empty() && t.back() == '*') return t + name;  // "T *" + name
    return t + " " + name;
  };
  // Same, but for argument position N, honouring a user-asserted signature.
  auto arg_decl = [&](int pos) -> std::string {
    std::string nm = "a" + std::to_string(pos), ty;
    if (g_signature) {
      auto it = g_signature->args.find(pos);
      if (it != g_signature->args.end()) {
        if (!it->second.second.empty()) nm = it->second.second;
        ty = it->second.first;
      }
    }
    if (ty.empty()) return decl_of(nm);
    return (!ty.empty() && ty.back() == '*') ? ty + nm : ty + " " + nm;
  };
  // Pointer-typed declaration for a base, or empty if it's a plain int.
  auto ptr_decl = [&](const std::string& name) -> std::string {
    std::string t = type_of(name);
    return t == "int" ? "" : decl_of(name);
  };

  // Incoming stack arguments (a9+): r1-relative loads above our frame and the
  // 8-slot register save area. Heuristic, but recovers the args that don't fit
  // in r3..r10. Shared by the annotation and the signature.
  std::vector<uint32_t> stack_args;
  if (func_.stack_frame_size) {
    // Same rule as the argument-origin scan: a slot this function stores to is
    // its own spill, and counting it as an argument shifts every real one.
    std::set<uint32_t> written;
    for (const auto& b : func_.blocks)
      for (const auto& in : b.instrs)
        if ((in.op == ILOp::Store || in.op == ILOp::StoreUpdate ||
             in.op == ILOp::FStore) &&
            in.src1.ppc_reg == 1 && in.imm > 0)
          written.insert((uint32_t)in.imm);
    std::set<uint32_t> stk;
    for (const auto& b : func_.blocks)
      for (const auto& in : b.instrs)
        if ((in.op == ILOp::Load || in.op == ILOp::LoadUpdate ||
             in.op == ILOp::FLoad) &&
            in.src1.ppc_reg == 1 && in.imm > 0 &&
            (uint32_t)in.imm >= func_.stack_frame_size + 0x40 &&
            !written.count((uint32_t)in.imm))
          stk.insert((uint32_t)in.imm);
    stack_args.assign(stk.begin(), stk.end());
  }

  // The function's own name comes from the live symbol map when present, not
  // the name baked into the IL at build time — otherwise an interactive rename
  // updates every call site but not the definition it renamed.
  std::string fname = func_.name;
  {
    auto sit = GlobalSyms().find(func_.entry);
    if (sit != GlobalSyms().end() && !sit->second.empty()) fname = sit->second;
    if (fname.empty()) fname = "sub";
  }
  // What the header comment shows: the name as recorded, signature and all.
  const std::string fdisplay = fname;
  fname = CodeIdent(fname);

  // Annotation header: address / ordinal, and the argument -> register map
  // (incoming rN, and the callee-saved register the prologue parks it in).
  {
    std::map<int, int> saved;  // arg reg -> saved reg (from prologue moves)
    if (!func_.blocks.empty()) {
      for (const auto& in : func_.blocks[0].instrs) {
        if (in.op == ILOp::Assign && in.src1.ppc_reg >= 3 &&
            in.src1.ppc_reg <= 10 && in.dst.ppc_reg >= 14 &&
            in.dst.ppc_reg <= 31 && !saved.count(in.src1.ppc_reg))
          saved[in.src1.ppc_reg] = in.dst.ppc_reg;
      }
    }
    out << "// "
        << "-------------------------------------------------------------\n";
    out << "// " << fdisplay << "  @ 0x";
    char eb[16];
    snprintf(eb, sizeof(eb), "%08X", func_.entry);
    out << eb;
    if (opts_.ordinal) {
      char ob[24];
      snprintf(ob, sizeof(ob), "  |  ord 0x%X", opts_.ordinal);
      out << ob;
    }
    out << "\n";
    if (!live_in_args_.empty()) {
      int hi = 3;
      for (int r : live_in_args_) hi = std::max(hi, r);
      for (int r = 3; r <= hi; r++) {
        char line[64];
        auto si = saved.find(r);
        if (si != saved.end())
          snprintf(line, sizeof(line), "//   a%-2d  r%-2d -> r%d", r - 2, r,
                   si->second);
        else
          snprintf(line, sizeof(line), "//   a%-2d  r%-2d", r - 2, r);
        out << line << "\n";
      }
    }
    int argn = 9;
    for (uint32_t off : stack_args) {
      char line[64];
      snprintf(line, sizeof(line), "//   a%-2d  [sp+0x%X]  (stack arg %d)", argn,
               off, argn);
      out << line << "\n";
      argn++;
    }
    out << "// "
        << "-------------------------------------------------------------\n";
  }

  out << "//----- (" ;
  char ab[16];
  snprintf(ab, sizeof(ab), "%08X", func_.entry);
  out << ab << ") ";
  out << "--------------------------------------------------------\n";

  std::string ret_ty = (g_signature && !g_signature->ret.empty())
                           ? g_signature->ret
                           : (last_r3_ != kILNoValue ? "int" : "void");
  out << ret_ty << " __fastcall " << fname << "(";
  if (live_in_args_.empty() && stack_args.empty()) {
    out << "void";
  } else {
    // Arguments are contiguous from r3; a gap means we mis-detected, so print
    // through the highest one actually read. Types are inferred from usage.
    bool wrote = false;
    int hi = live_in_args_.empty() ? 2 : 3;
    for (int r : live_in_args_) hi = std::max(hi, r);
    for (int r = 3; r <= hi; r++) {
      if (wrote) out << ", ";
      wrote = true;
      out << arg_decl(r - 2);
    }
    // Stack arguments a9+ (type unknown from usage -> int).
    int argn = 9;
    for (size_t i = 0; i < stack_args.size(); i++) {
      if (wrote) out << ", ";
      wrote = true;
      out << arg_decl(argn++);
    }
  }
  out << ")\n{\n";

  // Drop dead assignments. Expression folding and the select recogniser
  // consume their operands, which leaves the locals that held them defined and
  // never read -- the register-shuffle noise that makes optimised output hard
  // to read. A name referenced only by its own definition is dead; removing it
  // can kill others, so iterate. Counting occurrences over the whole body
  // (rather than scanning backwards) keeps this sound across the gotos.
  {
    auto name_at = [](const std::string& t, size_t i, size_t* len) {
      if (t[i] != 'v' || (i && (isalnum((unsigned char)t[i - 1]) ||
                                t[i - 1] == '_'))) return false;
      size_t j = i + 1;
      while (j < t.size() && isdigit((unsigned char)t[j])) j++;
      if (j == i + 1) return false;
      *len = j - i;
      return true;
    };
    for (;;) {
      std::unordered_map<std::string, int> refs;
      for (const auto& st : body) {
        for (size_t i = 0; i < st.text.size();) {
          size_t len = 0;
          if (name_at(st.text, i, &len)) {
            refs[st.text.substr(i, len)]++;
            i += len;
          } else {
            i++;
          }
        }
      }
      size_t before = body.size();
      std::vector<Stmt> kept;
      kept.reserve(body.size());
      std::set<std::string> dropped;
      for (auto& st : body) {
        if (!st.pure_def.empty() && refs[st.pure_def] <= 1) {
          dropped.insert(st.pure_def);
          continue;
        }
        kept.push_back(std::move(st));
      }
      body.swap(kept);
      if (body.size() == before) break;
      // Their declarations go too.
      std::vector<std::string> live;
      for (const auto& l : locals_) {
        size_t sp = l.rfind(' '), semi = l.rfind(';');
        std::string nm =
            (sp != std::string::npos && semi != std::string::npos && semi > sp)
                ? l.substr(sp + 1, semi - sp - 1)
                : std::string();
        if (!nm.empty() && dropped.count(nm)) continue;
        live.push_back(l);
      }
      locals_.swap(live);
    }
  }

  if (!locals_.empty()) {
    // One declaration per variable, not per spelling. Coalescing gives a
    // variable the name of its phi web, and two definitions in that web can
    // have different result types - a vector register written by a float
    // instruction and by an integer one, say. Declaring it twice is not C.
    std::map<std::string, std::string> by_name;   // name -> chosen type
    for (const auto& l : locals_) {
      size_t sp = l.rfind(' ');
      size_t semi = l.rfind(';');
      if (sp == std::string::npos || semi == std::string::npos || semi <= sp)
        continue;
      std::string nm = l.substr(sp + 1, semi - sp - 1);
      std::string ty = l.substr(0, sp);
      auto it = by_name.find(nm);
      if (it == by_name.end()) {
        by_name[nm] = ty;
        continue;
      }
      if (it->second == ty) continue;
      auto is_vec = [](const std::string& t) {
        return t == "uint128_t" || t == "int128_t" || t == "float128_t";
      };
      // A register-width value beats a scalar spelling of the same variable.
      if (is_vec(ty) && !is_vec(it->second)) {
        it->second = ty;
      } else if (is_vec(ty) && is_vec(it->second)) {
        // Definitions in one phi web that read the register differently. The
        // register holds bits; the operations say how those bits are read, so
        // claiming one lane interpretation for the variable would be a
        // statement the code does not support.
        it->second = "uint128_t";
      }
    }
    std::set<std::string> uniq;
    for (const auto& kv : by_name) uniq.insert(kv.second + " " + kv.first + ";");
    for (const auto& l : uniq) {
      // Retype a local that turned out to be a struct pointer base. Locals are
      // "TYPE name;" — swap TYPE when `name` has inferred fields.
      size_t sp = l.rfind(' ');
      size_t semi = l.rfind(';');
      std::string nm =
          (sp != std::string::npos && semi != std::string::npos && semi > sp)
              ? l.substr(sp + 1, semi - sp - 1)
              : "";
      std::string pd = ptr_decl(nm);
      if (!pd.empty())
        out << "  " << pd << ";\n";
      else
        out << "  " << l << "\n";
    }
    out << "\n";
  }

  for (const auto& s : body) {
    bool is_label = !s.text.empty() && s.text.back() == ':';
    if (is_label) {
      out << s.text << "\n";
      continue;
    }
    out << "  " << s.text;
    if (opts_.addr_comments) {
      char c[32];
      snprintf(c, sizeof(c), "  // %08X", s.addr);
      out << c;
    }
    out << "\n";
  }

  out << "}\n";
  return out.str();
}

}  // namespace

void SetGlobalSymbolMap(const std::map<uint32_t, std::string>& syms) {
  g_global_syms = &syms;
}

void SetFieldNames(
    const std::map<std::string, std::map<uint32_t, std::string>>& names) {
  g_field_names = names;
}

void SetStructTags(const std::map<std::string, std::string>& tags) {
  g_struct_tags = tags;
}

void SetStructDecls(const std::map<std::string, std::string>& decls) {
  g_struct_decls = decls;
}

void SetCalleeDecls(const std::map<std::string, std::string>& decls) {
  g_callee_decls = decls;
}

std::map<std::string, int> GetCalledFunctions() { return g_called_functions; }

void SetTypeOverrides(const std::map<std::string, std::string>& types) {
  g_type_overrides = types;
}

std::vector<ArgFlow> GetArgFlows() { return g_arg_flows; }

void SetStringMap(const std::map<uint32_t, std::string>* strings) {
  g_strings = strings;
}

std::vector<uint32_t> GetStringRefs() { return g_string_refs; }

std::vector<DataRef> GetDataRefs() { return g_data_refs; }

void SetSignature(const FuncSignature* sig) { g_signature = sig; }

void SetGlobalStructs(const std::vector<GlobalStruct>* gs) { g_gstructs = gs; }

std::map<uint32_t, std::map<uint32_t, std::string>> GetGlobalFieldUses() {
  return g_gfield_uses;
}

std::vector<BaseParamOrigin> GetBaseParamOrigins() {
  return g_base_param_origins;
}

std::vector<InferredStruct> GetInferredStructs() {
  std::vector<InferredStruct> out;
  for (const auto& kv : g_field_uses) {
    InferredStruct s;
    s.base = kv.first;
    s.type_name = StructNameFor(kv.first);
    for (const auto& fo : kv.second) {
      InferredField f;
      f.offset = fo.first;
      f.ctype = CTypeName(fo.second);
      f.name = FieldName(kv.first, fo.first);
      s.fields.push_back(f);
    }
    out.push_back(std::move(s));
  }
  return out;
}

std::string DecompileToC(const ILFunction& func, const DecompileCOptions& opts) {
  CDecompiler d(func, opts);
  return d.Run();
}
