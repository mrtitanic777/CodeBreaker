#include "import_annotate.h"

#include <cstdio>
#include <cstring>

namespace {

// Extract "0x........" from a disassembled branch operand.
bool ParseBranchTarget(const std::string& text, uint32_t* out) {
  size_t p = text.find("0x");
  if (p == std::string::npos) return false;

  uint32_t value = 0;
  size_t digits = 0;
  for (size_t i = p + 2; i < text.size(); i++) {
    char c = text[i];
    uint32_t d;
    if (c >= '0' && c <= '9') {
      d = static_cast<uint32_t>(c - '0');
    } else if (c >= 'a' && c <= 'f') {
      d = static_cast<uint32_t>(c - 'a' + 10);
    } else if (c >= 'A' && c <= 'F') {
      d = static_cast<uint32_t>(c - 'A' + 10);
    } else {
      break;
    }
    value = (value << 4) | d;
    digits++;
  }

  if (!digits) return false;
  *out = value;
  return true;
}

bool StartsWithMnemonic(const std::string& text, const char* mnemonic) {
  size_t len = std::strlen(mnemonic);
  if (text.size() < len) return false;
  if (std::strncmp(text.c_str(), mnemonic, len) != 0) return false;
  // Must be the whole mnemonic: "bl" must not match "blr", "b" not "bctrl".
  char next = text.size() > len ? text[len] : ' ';
  return next == ' ' || next == '\t' || next == '\0';
}

// --- PPC field accessors, only what the tracking below needs ---
inline uint32_t Opcode(uint32_t c) { return (c >> 26) & 0x3F; }
inline uint32_t RT(uint32_t c) { return (c >> 21) & 0x1F; }
inline uint32_t RA(uint32_t c) { return (c >> 16) & 0x1F; }
inline int32_t SIMM(uint32_t c) { return static_cast<int16_t>(c & 0xFFFF); }
inline uint32_t UIMM(uint32_t c) { return c & 0xFFFF; }

constexpr uint32_t kOpAddi = 14;   // addi / li
constexpr uint32_t kOpAddis = 15;  // addis / lis
constexpr uint32_t kOpOri = 24;
constexpr uint32_t kOpLwz = 32;
constexpr uint32_t kOpBranch = 18;   // b / bl
constexpr uint32_t kOpBranchC = 16;  // bc
constexpr uint32_t kOpXL = 19;       // bclr / bcctr / mtctr family lives in 31

// A tracked register either holds a constant we computed, or the name of the
// import whose pointer it now carries.
struct RegState {
  bool has_value = false;
  uint32_t value = 0;
  const char* import_name = nullptr;

  void Clear() {
    has_value = false;
    value = 0;
    import_name = nullptr;
  }
};

}  // namespace

std::string ImportCommentForBranch(uint32_t target,
                                   const ImportNameTable& imports) {
  const char* name = imports.Lookup(target);
  if (!name) return std::string();
  return std::string("  ; ") + name + "()";
}

void AnnotateImportCalls(std::vector<DisasmLine>& lines,
                         const ImportNameTable& imports) {
  if (imports.empty()) return;

  RegState regs[32];
  const char* ctr_import = nullptr;

  for (auto& line : lines) {
    const uint32_t code = line.code;
    const uint32_t op = Opcode(code);

    // --- Direct call at a thunk: exact, and by far the common case. ---
    if (StartsWithMnemonic(line.text, "bl") ||
        StartsWithMnemonic(line.text, "b") ||
        StartsWithMnemonic(line.text, "ba")) {
      uint32_t target = 0;
      if (ParseBranchTarget(line.text, &target)) {
        std::string comment = ImportCommentForBranch(target, imports);
        if (!comment.empty()) {
          line.text += comment;
          continue;
        }
      }
    }

    // --- Indirect call through ctr ---
    if (line.text.rfind("bctrl", 0) == 0 || line.text.rfind("bctr", 0) == 0) {
      if (ctr_import) {
        line.text += std::string("  ; ") + ctr_import + "()";
      }
      // A call clobbers the volatile registers; anything we were tracking in
      // them is stale afterwards.
      for (int i = 3; i <= 12; i++) regs[i].Clear();
      ctr_import = nullptr;
      continue;
    }

    // An unconditional branch ends the straight-line run this tracking is
    // valid for, so drop everything rather than carry it across a join.
    if (op == kOpBranch || op == kOpBranchC || op == kOpXL) {
      for (auto& r : regs) r.Clear();
      ctr_import = nullptr;
      continue;
    }

    switch (op) {
      case kOpAddis: {  // lis rT, imm  /  addis rT, rA, imm
        uint32_t rt = RT(code), ra = RA(code);
        uint32_t base = 0;
        bool ok = true;
        if (ra == 0) {
          base = 0;  // lis
        } else if (regs[ra].has_value) {
          base = regs[ra].value;
        } else {
          ok = false;
        }
        regs[rt].Clear();
        if (ok) {
          regs[rt].has_value = true;
          regs[rt].value = base + (static_cast<uint32_t>(SIMM(code)) << 16);
        }
        break;
      }

      case kOpAddi: {  // li rT, imm  /  addi rT, rA, imm
        uint32_t rt = RT(code), ra = RA(code);
        uint32_t base = 0;
        bool ok = true;
        if (ra == 0) {
          base = 0;
        } else if (regs[ra].has_value) {
          base = regs[ra].value;
        } else {
          ok = false;
        }
        regs[rt].Clear();
        if (ok) {
          regs[rt].has_value = true;
          regs[rt].value = base + static_cast<uint32_t>(SIMM(code));
        }
        break;
      }

      case kOpOri: {  // ori rA, rS, imm   (note: rS is RT field, dest is RA)
        uint32_t rs = RT(code), ra = RA(code);
        if (regs[rs].has_value) {
          uint32_t v = regs[rs].value | UIMM(code);
          regs[ra].Clear();
          regs[ra].has_value = true;
          regs[ra].value = v;
        } else {
          regs[ra].Clear();
        }
        break;
      }

      case kOpLwz: {  // lwz rT, disp(rA) - the import pointer load
        uint32_t rt = RT(code), ra = RA(code);
        const char* name = nullptr;
        if (ra != 0 && regs[ra].has_value) {
          uint32_t addr = regs[ra].value + static_cast<uint32_t>(SIMM(code));
          name = imports.LookupValue(addr);
          if (!name) name = imports.LookupThunk(addr);
        }
        regs[rt].Clear();
        regs[rt].import_name = name;
        break;
      }

      default: {
        // Anything else that writes a GPR invalidates it. Rather than model
        // every encoding, be conservative: clear the destination field for the
        // forms that actually have one in RT, and leave the rest alone.
        if (op == 31) {
          // Most op31 forms write RA (rlwinm-style) or RT; mr/or writes RA.
          regs[RA(code)].Clear();
          regs[RT(code)].Clear();
        } else if (op >= 32 && op <= 55) {
          // Remaining load/store forms: loads write RT.
          regs[RT(code)].Clear();
        }
        break;
      }
    }

    // mtctr rS  (op31, xo=467, SPR=9) - remember which import ctr now holds.
    if (op == 31 && ((code >> 1) & 0x3FF) == 467) {
      uint32_t spr = ((code >> 16) & 0x1F) | (((code >> 11) & 0x1F) << 5);
      if (spr == 9) {
        ctr_import = regs[RT(code)].import_name;
      }
    }
  }
}
