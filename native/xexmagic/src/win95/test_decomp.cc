// End-to-end proof: NE load -> x86 lift -> shared DecompileToC -> readable C.
#include <cstdio>
#include <cstdlib>
#include "win95/ne_loader.h"
#include "win95/x86_lift.h"
#include "win95/win16_names.h"
#include "decompiler_c.h"

int main(int argc, char** argv) {
  if (argc < 4) { std::printf("usage: %s <ne> <hexentry> <hexend> [name]\n", argv[0]); return 2; }
  win95::NEImage img = win95::LoadNE(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  uint32_t entry = (uint32_t)strtoul(argv[2], nullptr, 16);
  uint32_t end   = (uint32_t)strtoul(argv[3], nullptr, 16);
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
  for (auto& e : img.entries) if (e.addr) names[e.addr] = "export_" + std::to_string(e.ordinal);
  lifter.SetNames(names);

  // Build far-call-site -> (target,name) from the relocation table, following the
  // per-segment fixup chains. The instruction is 1 byte before the patched operand.
  std::map<uint32_t, std::pair<uint32_t, std::string>> sites;
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
        uint16_t nxt = img.image[s.base_addr + site] | (img.image[s.base_addr + site + 1] << 8);
        if (r.additive || nxt == 0xFFFF || nxt == site) break;
        site = nxt;
      }
    }
  }
  lifter.SetCallSites(sites);

  ILFunction fn = lifter.LiftFunction(img.image.data(), img.image.size(), entry, end, name);
  std::printf("// lifted %s: %zu blocks, %zu SSA values, %d instrs\n",
              name.c_str(), fn.blocks.size(), fn.values.size(), fn.total_instructions);
  DecompileCOptions opts;
  std::string c = DecompileToC(fn, opts);
  std::printf("%s\n", c.c_str());
  return 0;
}
