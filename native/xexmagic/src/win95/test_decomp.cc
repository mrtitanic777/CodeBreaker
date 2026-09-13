// End-to-end proof: NE load -> x86 lift -> shared DecompileToC -> readable C.
#include <cstdio>
#include <cstdlib>
#include "win95/ne_loader.h"
#include "win95/x86_lift.h"
#include "decompiler_c.h"

int main(int argc, char** argv) {
  if (argc < 4) { std::printf("usage: %s <ne> <hexentry> <hexend> [name]\n", argv[0]); return 2; }
  win95::NEImage img = win95::LoadNE(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  uint32_t entry = (uint32_t)strtoul(argv[2], nullptr, 16);
  uint32_t end   = (uint32_t)strtoul(argv[3], nullptr, 16);
  std::string name = argc > 4 ? argv[4] : "func";

  win95::X86Lifter lifter;
  std::map<uint32_t, std::string> names;
  for (auto& e : img.entries) if (e.addr) names[e.addr] = "export_" + std::to_string(e.ordinal);
  lifter.SetNames(names);

  ILFunction fn = lifter.LiftFunction(img.image.data(), img.image.size(), entry, end, name);
  std::printf("// lifted %s: %zu blocks, %zu SSA values, %d instrs\n",
              name.c_str(), fn.blocks.size(), fn.values.size(), fn.total_instructions);
  DecompileCOptions opts;
  std::string c = DecompileToC(fn, opts);
  std::printf("%s\n", c.c_str());
  return 0;
}
