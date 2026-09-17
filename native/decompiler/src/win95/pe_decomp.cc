// End-to-end PE (32-bit) proof: PE load -> x86 lift (32-bit ABI) -> shared
// DecompileToC -> readable C. The PE-side sibling of test_decomp.cc: same lift and
// back-end, PE-specific loading, function discovery and IAT call resolution.
//
//   win95pedec <pe> --module          decompile the whole module (interproc cascade)
//   win95pedec <pe> --disasm          per-section instruction listing
//   win95pedec <pe> --names           discovered function starts (addr<TAB>name)
//   win95pedec <pe> --module --sigs F apply demangled sigs/prototypes from file F
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "win95/pe_loader.h"
#include "win95/x86_lift.h"
#include "win95/capstone_decode.h"
#include "win95/sig_emit.h"
#include "decompiler_c.h"

namespace {
// A memory operand of the form [disp32] with no base/index -> an absolute VA. On a
// call/jmp this is the classic IAT indirection: call dword [__imp__X]. Returns the
// RVA, or 0 if not that shape.
uint32_t AbsMemRVA(const win95::X86Op& o, uint32_t image_base) {
  if (o.kind == win95::OpKind::Mem && o.base == win95::kNoReg &&
      o.index == win95::kNoReg && (uint32_t)o.disp >= image_base)
    return (uint32_t)o.disp - image_base;
  return 0;
}
}  // namespace

int main(int argc, char** argv) {
  if (argc < 3) {
    std::printf("usage: %s <pe> --module|--disasm|--names [--sigs FILE]\n", argv[0]);
    return 2;
  }
  win95::PEImage img = win95::LoadPE(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  std::string mode = argv[2];

  const uint8_t* image = img.image.data();
  size_t image_size = img.image.size();

  // Names: exports (RVA -> export name) resolve definitions and direct-call sites.
  std::map<uint32_t, std::string> names;
  for (auto& e : img.exports) names[e.second] = e.first;

  // --imports: `MODULE<TAB>func` per distinct import (from the IAT slot -> "MODULE.func" map), so the
  // build lane can emit real wlink imports instead of stubbing the Win32 API to return 0.
  if (mode == "--imports") {
    std::set<std::string> seen;
    for (auto& kv : img.imports) {
      const std::string& mf = kv.second;                  // "MODULE.func"
      size_t dot = mf.rfind('.');
      if (dot == std::string::npos) continue;
      std::string line = mf.substr(0, dot) + "\t" + mf.substr(dot + 1);
      if (seen.insert(line).second) std::printf("%s\n", line.c_str());
    }
    return 0;
  }
  // --exports: `name` per export, so the build can emit `export name` (publishes + keeps the code).
  if (mode == "--exports") {
    for (auto& e : img.exports) std::printf("%s\n", e.first.c_str());
    return 0;
  }
  // --entry: the entry-point RVA (AddressOfEntryPoint), so an EXE build can set `option start` and
  // keep the whole call tree instead of dead-code-eliminating everything.
  if (mode == "--entry") {
    std::printf("%X\n", img.entry);
    return 0;
  }

  // Optional --sigs FILE: demangled display names + prototypes, addr(RVA)-keyed,
  // produced by CodeBreaker's Python demangler. Same record format as the NE lane.
  std::map<uint32_t, FuncSignature> sigMap;
  for (int ai = 2; ai + 1 < argc; ++ai) {
    if (std::string(argv[ai]) != "--sigs") continue;
    FILE* sf = std::fopen(argv[ai + 1], "r");
    if (!sf) { std::fprintf(stderr, "warning: --sigs %s not found\n", argv[ai + 1]); break; }
    char buf[8192];
    while (std::fgets(buf, sizeof buf, sf)) {
      std::string L(buf);
      while (!L.empty() && (L.back() == '\n' || L.back() == '\r')) L.pop_back();
      if (L.empty()) continue;
      std::vector<std::string> f; size_t p = 0;
      for (;;) { size_t t = L.find('\t', p);
        if (t == std::string::npos) { f.push_back(L.substr(p)); break; }
        f.push_back(L.substr(p, t - p)); p = t + 1; }
      if (f.size() < 2) continue;
      uint32_t ad = (uint32_t)strtoul(f[0].c_str(), nullptr, 16);
      if (!f[1].empty()) names[ad] = f[1];
      FuncSignature sig;
      if (f.size() >= 3) sig.ret = f[2];
      for (size_t i = 3; i < f.size(); ++i) {
        if (f[i].empty()) continue;
        std::string ty = f[i], nm; size_t bar = f[i].find('|');
        if (bar != std::string::npos) { ty = f[i].substr(0, bar); nm = f[i].substr(bar + 1); }
        sig.args[(int)(i - 3) + 1] = {ty, nm};
      }
      sigMap[ad] = sig;
    }
    std::fclose(sf);
  }

  win95::X86Lifter lifter;
  lifter.SetBits32(true);           // 32-bit PE ABI

  // Scan code sections once: resolve IAT-indirect call/jmp sites to import names,
  // and record direct near-call (E8) targets as function starts.
  std::map<uint32_t, std::pair<uint32_t, std::string>> sites;   // insn addr -> (target, name)
  std::set<uint32_t> callTargets;
  for (auto& s : img.sections) {
    if (!s.code) continue;
    uint32_t a = s.va, e2 = s.va + (s.vsize ? s.vsize : s.raw_size);
    while (a < e2 && a < image_size) {
      win95::X86Insn in = win95::DecodeX86Auto(image, image_size, a, a, false);
      int len = in.valid ? in.len : 1;
      if (in.valid && (in.mnem == "call" || in.mnem == "jmp")) {
        if (in.ops[0].kind == win95::OpKind::Rel) {
          if (in.mnem == "call") callTargets.insert((uint32_t)in.ops[0].imm);
        } else {
          uint32_t rva = AbsMemRVA(in.ops[0], img.image_base);
          auto imp = img.imports.find(rva);
          if (imp != img.imports.end()) sites[a] = {0, imp->second};
        }
      }
      a += len;
    }
  }
  lifter.SetNames(names);
  lifter.SetCallSites(sites);

  // --disasm: per-section listing with resolved import/target annotations.
  if (mode == "--disasm") {
    for (auto& s : img.sections) {
      if (!s.code) continue;
      std::printf("; ---- section %s  rva %08X  (%u bytes) ----\n",
                  s.name.c_str(), s.va, s.vsize ? s.vsize : s.raw_size);
      uint32_t e2 = s.va + (s.vsize ? s.vsize : s.raw_size);
      for (uint32_t a = s.va; a < e2 && a < image_size;) {
        auto nn = names.find(a);
        if (nn != names.end()) std::printf("\n%s:\n", nn->second.c_str());
        win95::X86Insn in = win95::DecodeX86Auto(image, image_size, a, a, false);
        int len = in.valid ? in.len : 1;
        std::string bytes;
        for (int i = 0; i < len && a + i < image_size; ++i) {
          char b[4]; std::snprintf(b, sizeof b, "%02X", image[a + i]); bytes += b;
        }
        std::string text = in.valid ? win95::FormatX86(in) : "(bad)";
        auto si = sites.find(a);
        if (si != sites.end()) { text += "  ; -> "; text += si->second.second; }
        else if (in.valid && (in.mnem == "call" || in.mnem[0] == 'j') &&
                 in.ops[0].kind == win95::OpKind::Rel) {
          auto tn = names.find((uint32_t)in.ops[0].imm);
          if (tn != names.end()) { text += "  ; -> "; text += tn->second; }
        }
        std::printf("  %08X %-16s %s\n", a, bytes.c_str(), text.c_str());
        a += len;
      }
    }
    return 0;
  }

  // Function-start discovery: entry + exports + direct-call targets, each snapped
  // to the code section that contains it.
  // Function-start discovery -- the same three-part recovery the NE decoder uses, ported to PE
  // (32-bit near, sections instead of segments). Weak linear-only discovery left ~150 undefined
  // internal calls per PE module (user32 etc.); this recovers them:
  //   (1) seeds: entry + exports + linear E8 near-call targets (the union base);
  //   (2) recursive-descent from every seed, following jmp/jcc and harvesting near-call targets as
  //       NEW seeds to a fixpoint -- finds functions interleaved past inline data a linear scan skips
  //       and whole clusters entered only through the calls inside them;
  //   (3) tail-jmp/jcc recovery: a jmp/jcc that crosses a discovered function boundary is a tail call
  //       into a shared body / alternate entry -- seed its target (the cross-slot test keeps intra-
  //       function loops and if/else, which never cross a boundary, from being split).
  auto secOf = [&](uint32_t rva) -> int {
    for (size_t i = 0; i < img.sections.size(); ++i) {
      auto& s = img.sections[i];
      if (s.code && rva >= s.va && rva < s.va + (s.vsize ? s.vsize : s.raw_size)) return (int)i;
    }
    return -1;
  };
  auto secEnd = [&](int si) -> uint32_t {
    auto& s = img.sections[si]; return s.va + (s.vsize ? s.vsize : s.raw_size);
  };
  // A relocated DWORD that points into code is either a function pointer (vtable / dispatch table /
  // window proc -- a real function entry) or a switch jump-table entry (points MID-function). Only the
  // former should seed a function start; splitting a function at a switch target corrupts it. Gate on
  // a recognizable 32-bit function prologue -- switch-table blocks don't start with one.
  auto looksLikeProlog = [&](uint32_t rva) -> bool {
    if (rva + 3 > image_size) return false;
    uint8_t b0 = image[rva], b1 = image[rva + 1], b2 = image[rva + 2];
    if (b0 == 0x8B && b1 == 0xFF) return true;                       // mov edi,edi (hot-patch pad)
    if (b0 == 0x55 && b1 == 0x8B && b2 == 0xEC) return true;         // push ebp; mov ebp,esp
    if (b0 == 0x55 && (b1 == 0x53 || b1 == 0x56 || b1 == 0x57)) return true;  // push ebp; push reg
    if (b0 == 0x83 && b1 == 0xEC) return true;                       // sub esp, imm8
    if (b0 == 0x81 && b1 == 0xEC) return true;                       // sub esp, imm32
    if ((b0 == 0x53 || b0 == 0x56 || b0 == 0x57) &&
        (b1 == 0x53 || b1 == 0x56 || b1 == 0x57 || b1 == 0x8B)) return true;  // push reg; push/mov
    return false;
  };
  auto discover = [&]() {
    std::map<int, std::set<uint32_t>> starts;         // section index -> starts
    std::vector<uint32_t> seeds;
    auto addSeed = [&](uint32_t rva) {
      int si = secOf(rva);
      if (si >= 0 && starts[si].insert(rva).second) seeds.push_back(rva);
    };
    if (img.entry) addSeed(img.entry);
    for (auto& e : img.exports) addSeed(e.second);
    for (uint32_t t : callTargets) addSeed(t);
    // (1c) relocated code-pointers that begin with a function prologue: vtable / dispatch-table /
    // callback (window-proc) targets reached only through `call [reg]`, which control-flow discovery
    // can't follow. Prologue-gated so switch jump-table entries (mid-function, no prologue) never split
    // a real function.
    for (uint32_t r : img.base_relocs) {
      if (r + 4 > image_size) continue;
      uint32_t va = image[r] | (image[r + 1] << 8) | (image[r + 2] << 16) |
                    ((uint32_t)image[r + 3] << 24);
      if (va <= img.image_base) continue;
      uint32_t t = va - img.image_base;
      if (secOf(t) >= 0 && looksLikeProlog(t)) addSeed(t);
    }
    // (2) recursive-descent, harvesting near-call targets to a fixpoint
    for (size_t qi = 0; qi < seeds.size(); ++qi) {
      uint32_t f = seeds[qi];
      int si = secOf(f); if (si < 0) continue;
      uint32_t send = secEnd(si);
      std::set<uint32_t> vis;
      std::vector<uint32_t> w{f};
      while (!w.empty()) {
        uint32_t a = w.back(); w.pop_back();
        while (a >= img.sections[si].va && a < send && a < image_size && !vis.count(a)) {
          vis.insert(a);
          win95::X86Insn in = win95::DecodeX86Auto(image, image_size, a, a, false);
          if (!in.valid) break;
          const std::string& m = in.mnem;
          if (m == "call" && in.ops[0].kind == win95::OpKind::Rel)
            addSeed((uint32_t)in.ops[0].imm);
          if (m == "jmp") {
            if (in.ops[0].kind == win95::OpKind::Rel) w.push_back((uint32_t)in.ops[0].imm);
            break;
          }
          if (m.size() && m[0] == 'j') {                 // jcc: target + fallthrough
            if (in.ops[0].kind == win95::OpKind::Rel) w.push_back((uint32_t)in.ops[0].imm);
            a += in.len; continue;
          }
          if (m == "ret" || m == "retf" || m == "iret") break;
          a += in.len;
        }
      }
    }
    // (3) tail-jmp/jcc cross-slot recovery, to a fixpoint
    bool grew = true;
    while (grew) {
      grew = false;
      for (size_t si = 0; si < img.sections.size(); ++si) {
        if (!img.sections[si].code) continue;
        std::vector<uint32_t> fs(starts[si].begin(), starts[si].end());
        std::sort(fs.begin(), fs.end());
        uint32_t send = secEnd((int)si);
        for (size_t i = 0; i < fs.size(); ++i) {
          uint32_t f0 = fs[i], f1 = (i + 1 < fs.size()) ? fs[i + 1] : send;
          for (uint32_t a = f0; a < f1 && a < image_size;) {
            win95::X86Insn in = win95::DecodeX86Auto(image, image_size, a, a, false);
            if (!in.valid) break;
            if (in.mnem.size() && in.mnem[0] == 'j' && in.ops[0].kind == win95::OpKind::Rel) {
              uint32_t t = (uint32_t)in.ops[0].imm;
              if (t < f0 || t >= f1) {
                size_t b = seeds.size(); addSeed(t);
                if (seeds.size() != b) grew = true;
              }
            }
            a += in.len;
          }
        }
      }
    }
    return starts;
  };

  // --at <hexRVA>: decompile the ONE function beginning at the given RVA, even when the module-wide
  // discovery never classified it as a function head (a tracer found it via exports+heuristics). The
  // function end is the next discovered start above it in the same code section, else the section end.
  if (mode == "--at") {
    if (argc < 4) { std::fprintf(stderr, "usage: %s <pe> --at <hexRVA>\n", argv[0]); return 2; }
    uint32_t fstart = (uint32_t)strtoul(argv[3], nullptr, 16);
    // locate the containing code section
    int si = -1; uint32_t send = 0;
    for (size_t i = 0; i < img.sections.size(); ++i) {
      auto& s = img.sections[i];
      uint32_t se = s.va + (s.vsize ? s.vsize : s.raw_size);
      if (s.code && fstart >= s.va && fstart < se) { si = (int)i; send = se; break; }
    }
    if (si < 0) { std::fprintf(stderr, "FAIL: RVA %X not in a code section\n", fstart); return 1; }
    uint32_t fend = send;                                 // next known start above fstart, else send
    auto starts = discover();
    auto it = starts.find(si);
    if (it != starts.end())
      for (uint32_t s : it->second) if (s > fstart && s < fend) fend = s;
    char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", fstart);
    auto nn = names.find(fstart);
    std::string fname = nn != names.end() ? nn->second : nm;
    names[fstart] = fname; lifter.SetNames(names);        // self-calls render the same symbol
    ILFunction f = lifter.LiftFunction(image, image_size, fstart, fend, fname);
    auto sg = sigMap.find(fstart);
    SetSignature(sg != sigMap.end() ? &sg->second : nullptr);
    std::printf("%s\n", DecompileToC(f, {}).c_str());
    SetSignature(nullptr);
    return 0;
  }

  if (mode == "--names") {
    auto starts = discover();
    for (auto& kv : starts)
      for (uint32_t fstart : kv.second) {
        auto nn = names.find(fstart);
        char ab[16]; std::snprintf(ab, sizeof ab, "%X", fstart);
        std::printf("%s\t%s\n", ab, nn != names.end() ? nn->second.c_str() : "");
      }
    return 0;
  }

  if (mode == "--sigdump") {
    auto starts = discover();
    std::vector<uint32_t> ss, ee;
    for (auto& kv : starts) {
      auto& s = img.sections[kv.first];
      uint32_t send = s.va + (s.vsize ? s.vsize : s.raw_size);
      std::vector<uint32_t> fs(kv.second.begin(), kv.second.end());
      for (size_t i = 0; i < fs.size(); ++i) {
        ss.push_back(fs[i]);
        ee.push_back((i + 1 < fs.size()) ? fs[i + 1] : send);
      }
    }
    std::map<uint32_t, int> relocs;                   // RVA -> 4-byte HIGHLOW span
    for (uint32_t r : img.base_relocs) relocs[r] = 4;
    win95::EmitSigDump(image, image_size, ss, ee, false, &relocs);
    return 0;
  }

  if (mode == "--module") {
    auto starts = discover();
    struct FE { uint32_t fstart, fend; std::string name; };
    std::vector<FE> fns;
    for (auto& kv : starts) {
      auto& s = img.sections[kv.first];
      uint32_t send = s.va + (s.vsize ? s.vsize : s.raw_size);
      std::vector<uint32_t> fs(kv.second.begin(), kv.second.end());
      for (size_t i = 0; i < fs.size(); ++i) {
        uint32_t fstart = fs[i];
        uint32_t fend = (i + 1 < fs.size()) ? fs[i + 1] : send;
        char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", fstart);
        auto nn = names.find(fstart);
        fns.push_back({fstart, fend, nn != names.end() ? nn->second : nm});
      }
    }
    // Seed every discovered start's name so internal call sites render the SAME
    // symbol as the definition (call -> sub_0014AD, not the back-end's decimal
    // fallback) -- cross-references link and the C reads as one program.
    for (auto& fe : fns) names[fe.fstart] = fe.name;
    lifter.SetNames(names);
    auto is_var = [](const std::string& v) {
      if (v.size() < 2 || (v[0] != 'a' && v[0] != 'v')) return false;
      for (size_t k = 1; k < v.size(); ++k) if (!std::isdigit((unsigned char)v[k])) return false;
      return true;
    };
    std::map<std::string, std::string> kEmpty;
    std::map<uint32_t, std::map<std::string, std::string>> overrides;
    // Pass 1: harvest argument flows, propagate known callee param types.
    for (auto& fe : fns) {
      ILFunction f = lifter.LiftFunction(image, image_size, fe.fstart, fe.fend, fe.name);
      auto sg = sigMap.find(fe.fstart);
      SetSignature(sg != sigMap.end() ? &sg->second : nullptr);
      SetTypeOverrides(kEmpty);
      DecompileToC(f, {});
      SetSignature(nullptr);
      for (auto& fl : GetArgFlows()) {
        if (fl.offset != 0 || !fl.callee_target) continue;
        auto cs = sigMap.find(fl.callee_target);
        if (cs == sigMap.end()) continue;
        auto ai = cs->second.args.find(fl.position + 1);
        if (ai == cs->second.args.end() || ai->second.first.empty()) continue;
        if (!is_var(fl.arg)) continue;
        overrides[fe.fstart][fl.arg] = ai->second.first;
      }
    }
    // Pass 2: emit with propagated overrides.
    for (auto& fe : fns) {
      ILFunction f = lifter.LiftFunction(image, image_size, fe.fstart, fe.fend, fe.name);
      auto sg = sigMap.find(fe.fstart);
      SetSignature(sg != sigMap.end() ? &sg->second : nullptr);
      auto ov = overrides.find(fe.fstart);
      SetTypeOverrides(ov != overrides.end() ? ov->second : kEmpty);
      std::printf("%s\n", DecompileToC(f, {}).c_str());
      SetSignature(nullptr); SetTypeOverrides(kEmpty);
    }
    std::fprintf(stderr, "decompiled %zu functions across %zu code sections (PE, interproc cascade)\n",
                 fns.size(), starts.size());
    return 0;
  }

  std::printf("unknown mode: %s\n", mode.c_str());
  return 2;
}
