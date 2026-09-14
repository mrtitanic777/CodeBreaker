// End-to-end DOS/MZ proof: MZ load -> x86 lift (16-bit real mode) -> DecompileToC.
// The DOS sibling of pe_decomp/le_decomp. One flat image; discovery is entry +
// near/far call targets + a 55 8B EC prologue scan.
//
//   win95mzdec <exe> --module          decompile the module (interproc cascade)
//   win95mzdec <exe> --disasm          instruction listing
//   win95mzdec <exe> --names           discovered function starts (addr<TAB>name)
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "win95/mz_loader.h"
#include "win95/x86_lift.h"
#include "win95/sig_emit.h"
#include "decompiler_c.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    std::printf("usage: %s <exe> --module|--disasm|--names\n", argv[0]);
    return 2;
  }
  win95::MZImage img = win95::LoadMZ(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  std::string mode = argv[2];

  const uint8_t* image = img.image.data();
  size_t image_size = img.image.size();

  win95::X86Lifter lifter;      // 16-bit real mode is the default (bits32_ = false)

  // Scan for call targets (near E8 -> Rel; far 9A -> FarPtr).
  std::set<uint32_t> callTargets;
  for (uint32_t a = 0; a < image_size;) {
    win95::X86Insn in = win95::DecodeX86(image, image_size, a, a, true);
    int len = in.valid ? in.len : 1;
    if (in.valid && in.mnem == "call") {
      if (in.ops[0].kind == win95::OpKind::Rel) callTargets.insert((uint32_t)in.ops[0].imm);
      else if (in.ops[0].kind == win95::OpKind::FarPtr)
        callTargets.insert((uint32_t)in.ops[0].far_seg * 16 + in.ops[0].far_off);
    }
    a += len;
  }

  std::map<uint32_t, std::string> names;
  if (img.entry < image_size) names[img.entry] = "start";

  if (mode == "--disasm") {
    for (uint32_t a = 0; a < image_size;) {
      auto nn = names.find(a);
      if (nn != names.end()) std::printf("\n%s:\n", nn->second.c_str());
      win95::X86Insn in = win95::DecodeX86(image, image_size, a, a, true);
      int len = in.valid ? in.len : 1;
      std::string bytes;
      for (int i = 0; i < len && a + i < image_size; ++i) {
        char b[4]; std::snprintf(b, sizeof b, "%02X", image[a + i]); bytes += b;
      }
      std::string text = in.valid ? win95::FormatX86(in) : "(bad)";
      std::printf("  %04X  %-14s %s\n", a, bytes.c_str(), text.c_str());
      a += len;
    }
    return 0;
  }

  auto discover = [&]() {
    std::set<uint32_t> starts;
    auto add = [&](uint32_t a) { if (a < image_size) starts.insert(a); };
    add(img.entry);
    for (uint32_t t : callTargets) add(t);
    for (uint32_t a = 0; a + 3 <= image_size; ++a)     // push bp; mov bp,sp
      if (image[a] == 0x55 && image[a + 1] == 0x8B && image[a + 2] == 0xEC) add(a);
    return starts;
  };

  if (mode == "--names") {
    for (uint32_t f : discover()) {
      auto nn = names.find(f);
      char ab[16]; std::snprintf(ab, sizeof ab, "%X", f);
      std::printf("%s\t%s\n", ab, nn != names.end() ? nn->second.c_str() : "");
    }
    return 0;
  }

  if (mode == "--sigdump") {
    std::set<uint32_t> starts = discover();
    std::vector<uint32_t> fs(starts.begin(), starts.end());
    std::vector<uint32_t> ss, ee;
    for (size_t i = 0; i < fs.size(); ++i) {
      ss.push_back(fs[i]);
      ee.push_back((i + 1 < fs.size()) ? fs[i + 1] : (uint32_t)image_size);
    }
    win95::EmitSigDump(image, image_size, ss, ee, true);
    return 0;
  }

  if (mode == "--module") {
    std::set<uint32_t> starts = discover();
    std::vector<uint32_t> fs(starts.begin(), starts.end());
    struct FE { uint32_t fstart, fend; std::string name; };
    std::vector<FE> fns;
    for (size_t i = 0; i < fs.size(); ++i) {
      uint32_t fstart = fs[i];
      uint32_t fend = (i + 1 < fs.size()) ? fs[i + 1] : (uint32_t)image_size;
      char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", fstart);
      auto nn = names.find(fstart);
      fns.push_back({fstart, fend, nn != names.end() ? nn->second : nm});
    }
    for (auto& fe : fns) names[fe.fstart] = fe.name;   // link internal calls
    lifter.SetNames(names);
    bool diag = getenv("MZ_DIAG") != nullptr;
    for (auto& fe : fns) {
      if (diag) std::fprintf(stderr, "LIFT %06X..%06X %s\n", fe.fstart, fe.fend, fe.name.c_str());
      ILFunction f = lifter.LiftFunction(image, image_size, fe.fstart, fe.fend, fe.name);
      if (diag) std::fprintf(stderr, "  DECOMP (%zu blocks)\n", f.blocks.size());
      std::printf("%s\n", DecompileToC(f, {}).c_str());
    }
    std::fprintf(stderr, "decompiled %zu functions (DOS/MZ, 16-bit)\n", fns.size());
    return 0;
  }

  std::printf("unknown mode: %s\n", mode.c_str());
  return 2;
}
