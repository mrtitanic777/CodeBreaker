// End-to-end proof: NE load -> x86 lift -> shared DecompileToC -> readable C.
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "win95/ne_loader.h"
#include "win95/x86_lift.h"
#include "win95/capstone_decode.h"
#include "win95/win16_names.h"
#include "win95/sig_emit.h"
#include "decompiler_c.h"

int main(int argc, char** argv) {
  bool modeFlag = argc >= 3 && argv[2][0] == '-' && argv[2][1] == '-';
  if (argc < 4 && !modeFlag) {
    std::printf("usage: %s <ne> <hexentry> <hexend> [name]\n"
                "       %s <ne> --module 0        (decompile whole module)\n"
                "       %s <ne> --disasm          (disassemble whole module)\n",
                argv[0], argv[0], argv[0]); return 2;
  }
  win95::NEImage img = win95::LoadNE(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  uint32_t entry = (uint32_t)strtoul(argv[2], nullptr, 16);
  uint32_t end   = argc > 3 ? (uint32_t)strtoul(argv[3], nullptr, 16) : 0;
  std::string name = argc > 4 ? argv[4] : "func";

  // Complete import naming: read the system DLLs' own export name tables.
  std::string dir(argv[1]);
  size_t sl = dir.find_last_of("/\\"); dir = sl == std::string::npos ? "." : dir.substr(0, sl);
  std::map<std::string, std::map<int, std::string>> sysExports;
  auto loadSys = [&](const std::string& mod, const std::string& file) {
    win95::NEImage d = win95::LoadNE(dir + "/" + file);
    if (!d.ok) return;
    auto& mm = sysExports[mod];
    for (auto& pr : d.resident_names)    if (pr.second) mm[pr.second] = pr.first;
    for (auto& pr : d.nonresident_names) if (pr.second) mm[pr.second] = pr.first;
  };
  loadSys("KERNEL", "krnl386.exe"); loadSys("USER", "user.exe"); loadSys("GDI", "gdi.exe");

  win95::X86Lifter lifter;
  std::map<uint32_t, std::string> names;
  // real export names: the module's own resident/nonresident name tables give
  // name<->ordinal (entry 0 is the module/description, skip it); join via entries.
  std::map<int, std::string> ordName;
  for (size_t i = 1; i < img.resident_names.size(); ++i)
    ordName[img.resident_names[i].second] = img.resident_names[i].first;
  for (size_t i = 1; i < img.nonresident_names.size(); ++i)
    ordName[img.nonresident_names[i].second] = img.nonresident_names[i].first;
  for (auto& e : img.entries) if (e.addr) {
    auto it = ordName.find(e.ordinal);
    names[e.addr] = it != ordName.end() ? it->second : ("export_" + std::to_string(e.ordinal));
  }

  // Optional --sigs FILE: demangled names + prototypes, addr-keyed, produced by
  // the Python demangler (CodeBreaker's, the sole source of truth). One record
  // per line: <addrhex>\t<display>\t<ret>\t<argtype[|name]>\t...  A display name
  // overrides the raw (mangled) export name for the definition AND its call
  // sites; ret/args feed the decompiler's per-function signature. arg i -> a(i+1).
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
        sig.args[(int)(i - 3) + 1] = {ty, nm};    // a1 = first detected arg (r3)
      }
      sigMap[ad] = sig;
    }
    std::fclose(sf);
  }

  lifter.SetNames(names);

  // Build far-call-site -> (target,name) from the relocation table, following the
  // per-segment fixup chains. The instruction is 1 byte before the patched operand.
  std::map<uint32_t, std::pair<uint32_t, std::string>> sites;
  std::set<uint32_t> reloc_sites;                 // linear addrs of fixup operands (pointer data)
  for (auto& s : img.segments) {
    auto rit = img.relocs.find(s.idx);
    if (rit == img.relocs.end()) continue;
    for (auto& r : rit->second) {
      std::string nm; uint32_t tgt = 0;
      if (r.kind == win95::NEReloc::INTERNAL) {
        tgt = img.LinearAddr(r.seg, r.entry);
        auto n = names.find(tgt);
        nm = n != names.end() ? n->second : ("sub_" + std::to_string(tgt));
      } else if (r.kind == win95::NEReloc::IMPORTORD) {
        std::string mod = (r.module >= 1 && r.module <= (int)img.module_refs.size())
                              ? img.module_refs[r.module - 1] : "IMP";
        std::string um = mod; for (auto& ch : um) ch = (char)toupper((unsigned char)ch);
        auto me = sysExports.find(um);
        std::string known;
        if (me != sysExports.end()) { auto oi = me->second.find(r.ordinal); if (oi != me->second.end()) known = oi->second; }
        if (known.empty()) known = win95::Win16Name(mod, r.ordinal);   // seed fallback
        nm = !known.empty() ? known : (mod + "_" + std::to_string(r.ordinal));
      } else if (r.kind == win95::NEReloc::IMPORTNAME) {
        std::string mod = (r.module >= 1 && r.module <= (int)img.module_refs.size())
                              ? img.module_refs[r.module - 1] : "IMP";
        nm = mod + "_name" + std::to_string(r.name_off);
      } else continue;
      // walk the fixup chain (word at each site points to the next, 0xFFFF ends)
      uint32_t site = r.offset;
      for (int guard = 0; guard < 4096; ++guard) {
        uint32_t lin = s.base_addr + site;
        if (lin < 1 || lin >= img.image.size()) break;
        sites[lin - 1] = {tgt, nm};                 // instruction is opcode before operand
        reloc_sites.insert(lin);                    // the fixup operand itself is pointer data
        uint16_t nxt = img.image[s.base_addr + site] | (img.image[s.base_addr + site + 1] << 8);
        if (r.additive || nxt == 0xFFFF || nxt == site) break;
        site = nxt;
      }
    }
  }
  lifter.SetCallSites(sites);
  lifter.SetRelocSites(reloc_sites);

  // Disassembler engine: emit an instruction listing (addr: bytes  mnem ops),
  // annotating call targets with resolved names. Whole module by default.
  if (std::string(argv[2]) == "--disasm") {
    for (auto& s : img.segments) {
      if (s.is_data) continue;
      std::printf("; ---- segment %d  (%u bytes) ----\n", s.idx, s.length);
      for (uint32_t a = s.base_addr, e2 = s.base_addr + s.length; a < e2;) {
        auto nn = names.find(a);
        if (nn != names.end()) std::printf("\n%s:\n", nn->second.c_str());
        win95::X86Insn in = win95::DecodeX86Auto(img.image.data(), img.image.size(), a, a, true);
        int len = in.valid ? in.len : 1;
        std::string bytes;
        for (int i = 0; i < len && a + i < img.image.size(); ++i) {
          char b[4]; std::snprintf(b, sizeof b, "%02X", img.image[a + i]); bytes += b;
        }
        std::string text = in.valid ? win95::FormatX86(in) : "(bad)";
        // annotate resolved call / branch targets
        auto si = sites.find(a);
        if (si != sites.end()) { text += "  ; -> "; text += si->second.second; }
        else if (in.valid && (in.mnem == "call" || in.mnem[0] == 'j') && in.ops[0].kind == win95::OpKind::Rel) {
          auto tn = names.find((uint32_t)in.ops[0].imm);
          if (tn != names.end()) { text += "  ; -> "; text += tn->second; }
        }
        std::printf("  %04X:%04X %-14s %s\n", a >> 16, a & 0xFFFF, bytes.c_str(), text.c_str());
        a += len;
      }
    }
    return 0;
  }

  // Function-start discovery shared by --names and --module.
  auto discover = [&]() {
    std::map<int, std::set<uint32_t>> starts;   // seg idx -> function start addrs
    for (auto& s : img.segments) if (!s.is_data) starts[s.idx].insert(s.base_addr);
    for (auto& e : img.entries) if (e.addr) starts[e.seg].insert(e.addr);
    for (auto& kv : sites) { uint32_t t = kv.second.first; if (t) starts[t >> 16].insert(t); }
    for (auto& s : img.segments) {              // near-call (E8) targets via linear scan
      if (s.is_data) continue;
      for (uint32_t a = s.base_addr, e2 = s.base_addr + s.length; a < e2;) {
        win95::X86Insn in = win95::DecodeX86Auto(img.image.data(), img.image.size(), a, a, true);
        if (in.valid && in.mnem == "call" && in.ops[0].kind == win95::OpKind::Rel)
          starts[s.idx].insert((uint32_t)in.ops[0].imm);
        a += in.valid ? in.len : 1;
      }
    }
    return starts;
  };

  // --names: dump discovered function starts (addr<TAB>name). Feeds the Python
  // demangler, which writes back a --sigs file.
  if (std::string(argv[2]) == "--names") {
    auto starts = discover();
    for (auto& s : img.segments) {
      if (s.is_data) continue;
      for (uint32_t fstart : starts[s.idx]) {
        if (fstart >= s.base_addr + s.length) continue;
        auto nn = names.find(fstart);
        char ab[16]; std::snprintf(ab, sizeof ab, "%X", fstart);
        std::printf("%s\t%s\n", ab, nn != names.end() ? nn->second.c_str() : "");
      }
    }
    return 0;
  }

  // --sigdump: per-function raw prefix bytes + mnemonics, for the cross-lane
  // crtsig/sigscan pass (nativesig.py).
  if (std::string(argv[2]) == "--sigdump") {
    auto starts = discover();
    std::vector<uint32_t> ss, ee;
    for (auto& s : img.segments) {
      if (s.is_data) continue;
      std::vector<uint32_t> fs(starts[s.idx].begin(), starts[s.idx].end());
      for (size_t i = 0; i < fs.size(); ++i) {
        if (fs[i] >= s.base_addr + s.length) continue;
        ss.push_back(fs[i]);
        ee.push_back((i + 1 < fs.size()) ? fs[i + 1] : s.base_addr + s.length);
      }
    }
    // Relocation sites (linear addr -> width), walking each fixup chain. NE src_type
    // low nibble: 0=lobyte(1), 2=segment(2), 3=far pointer(4), 5=offset(2).
    std::map<uint32_t, int> relocs;
    auto rwidth = [](uint8_t st) { st &= 0x0F; return st == 0 ? 1 : st == 3 ? 4 : 2; };
    for (auto& s : img.segments) {
      auto rit = img.relocs.find(s.idx);
      if (rit == img.relocs.end()) continue;
      for (auto& r : rit->second) {
        int w = rwidth(r.src_type);
        uint32_t off = r.offset;
        for (int guard = 0; guard < 4096; ++guard) {
          uint32_t lin = s.base_addr + off;
          if (lin < 1 || lin + 1 >= img.image.size()) break;
          relocs[lin] = w;
          uint16_t nxt = img.image[lin] | (img.image[lin + 1] << 8);
          if (r.additive || nxt == 0xFFFF || nxt == off) break;
          off = nxt;
        }
      }
    }
    win95::EmitSigDump(img.image.data(), img.image.size(), ss, ee, true, &relocs);
    return 0;
  }

  // Whole-module mode: discover function starts and decompile every one, with a
  // two-pass interprocedural value/type cascade.
  if (std::string(argv[2]) == "--module") {
    auto starts = discover();
    struct FE { uint32_t fstart, fend; std::string name; };
    std::vector<FE> fns;
    for (auto& s : img.segments) {
      if (s.is_data) continue;
      std::vector<uint32_t> fs(starts[s.idx].begin(), starts[s.idx].end());
      for (size_t i = 0; i < fs.size(); ++i) {
        uint32_t fstart = fs[i];
        uint32_t fend = (i + 1 < fs.size()) ? fs[i + 1] : s.base_addr + s.length;
        // Reject starts outside this segment's range: a bogus target (e.g. an
        // unresolved 0) below the base would otherwise span a giant range and
        // render a runaway body (and duplicate across segments).
        if (fstart < s.base_addr || fstart >= s.base_addr + s.length) continue;
        char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", fstart);
        auto nn = names.find(fstart);
        fns.push_back({fstart, fend, nn != names.end() ? nn->second : nm});
      }
    }
    auto is_var = [](const std::string& v) {
      if (v.size() < 2 || (v[0] != 'a' && v[0] != 'v')) return false;
      for (size_t k = 1; k < v.size(); ++k) if (!std::isdigit((unsigned char)v[k])) return false;
      return true;
    };
    std::map<std::string,std::string> kEmpty;
    // Pass 1: decompile once per function to harvest argument flows; propagate a
    // callee's known parameter types (from demangled/asserted sigs) onto the
    // caller variables passed there. Only where the callee's param type is known
    // and the arg is a plain local/param (aN/vN); struct-offset flows skipped.
    std::map<uint32_t, std::map<std::string,std::string>> overrides;   // caller fstart -> var -> type
    for (auto& fe : fns) {
      ILFunction f = lifter.LiftFunction(img.image.data(), img.image.size(), fe.fstart, fe.fend, fe.name);
      auto sg = sigMap.find(fe.fstart);
      SetSignature(sg != sigMap.end() ? &sg->second : nullptr);
      SetTypeOverrides(kEmpty);
      DecompileToC(f, {});                     // discard; populates GetArgFlows()
      SetSignature(nullptr);
      for (auto& fl : GetArgFlows()) {
        if (fl.offset != 0 || !fl.callee_target) continue;
        auto cs = sigMap.find(fl.callee_target);
        if (cs == sigMap.end()) continue;
        auto ai = cs->second.args.find(fl.position + 1);     // arg key is 1-based (a1=r3)
        if (ai == cs->second.args.end() || ai->second.first.empty()) continue;
        if (!is_var(fl.arg)) continue;
        overrides[fe.fstart][fl.arg] = ai->second.first;
      }
    }
    // Pass 2: emit with the propagated type overrides applied.
    for (auto& fe : fns) {
      ILFunction f = lifter.LiftFunction(img.image.data(), img.image.size(), fe.fstart, fe.fend, fe.name);
      auto sg = sigMap.find(fe.fstart);
      SetSignature(sg != sigMap.end() ? &sg->second : nullptr);
      auto ov = overrides.find(fe.fstart);
      SetTypeOverrides(ov != overrides.end() ? ov->second : kEmpty);
      std::printf("%s\n", DecompileToC(f, {}).c_str());
      SetSignature(nullptr); SetTypeOverrides(kEmpty);
    }
    std::fprintf(stderr, "decompiled %zu functions across %zu code segments (interproc cascade)\n",
                 fns.size(), starts.size());
    return 0;
  }

  ILFunction fn = lifter.LiftFunction(img.image.data(), img.image.size(), entry, end, name);
  std::printf("// lifted %s: %zu blocks, %zu SSA values, %d instrs\n",
              name.c_str(), fn.blocks.size(), fn.values.size(), fn.total_instructions);
  DecompileCOptions opts;
  std::string c = DecompileToC(fn, opts);
  std::printf("%s\n", c.c_str());
  return 0;
}
