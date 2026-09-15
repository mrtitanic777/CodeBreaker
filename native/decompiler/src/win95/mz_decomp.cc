// End-to-end DOS/MZ + IO.SYS decompiler: load -> x86 lift (16-bit real mode) ->
// DecompileToC. The DOS sibling of pe_decomp/le_decomp. One flat image; discovery is
// entry + near/far call targets + a 55 8B EC prologue scan. For IO.SYS/WINBOOT.SYS the
// abused MZ header is detected and the WHOLE file (MSLOAD+MSBIO+MSDOS body + SYSINIT)
// is mapped flat, with far seg:off resolved through the body load base (0x0070).
//
//   win95mzdec <exe> --module          decompile the module (interproc cascade)
//   win95mzdec <exe> --disasm          instruction listing
//   win95mzdec <exe> --names           discovered function starts (addr<TAB>name)
//   win95mzdec <exe> --tree <outdir>   emit an organized readable source tree
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "win95/mz_loader.h"
#include "win95/x86_lift.h"
#include "win95/capstone_decode.h"
#include "win95/sig_emit.h"
#include "decompiler_c.h"

// Detect the IO.SYS abused-header layout: e_cparhdr*16 points well past a plausible
// header AND before the image end (the raw body sits inside the "header" region).
static bool LooksLikeIOSYS(const std::string& path) {
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f) return false;
  uint8_t h[16]; size_t got = std::fread(h, 1, 16, f);
  std::fseek(f, 0, SEEK_END); long sz = std::ftell(f); std::fclose(f);
  if (got < 16 || h[0] != 'M' || h[1] != 'Z') return false;
  uint32_t hdr = (uint32_t)(h[8] | (h[9] << 8)) * 16;
  // IO.SYS/WINBOOT.SYS abuses e_cparhdr so the raw MSBIO+MSDOS body (the majority of the
  // file) sits inside the "header" region, with only SYSINIT as the real load module.
  // A normal EXE's header is a small fraction; require the body to dominate the file.
  return sz > 0 && hdr > (uint32_t)sz / 2;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::printf("usage: %s <exe> --module|--disasm|--names|--tree <outdir>\n", argv[0]);
    return 2;
  }
  bool iosys = LooksLikeIOSYS(argv[1]);
  win95::MZImage img = iosys ? win95::LoadIOSYS(argv[1]) : win95::LoadMZ(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  std::string mode = argv[2];

  const uint8_t* image = img.image.data();
  size_t image_size = img.image.size();

  win95::X86Lifter lifter;      // 16-bit real mode is the default (bits32_ = false)

  // Resolve a far seg:off to a flat file offset. In the IO.SYS body the load base is
  // 0x0070; SYSINIT far targets fall back to seg*16+off (its own load module base 0).
  auto farToLinear = [&](uint16_t seg, uint16_t off) -> uint32_t {
    if (img.is_iosys) {
      uint32_t lin = ((uint32_t)seg - img.load_base) * 16 + off;
      if ((int32_t)((uint32_t)seg - img.load_base) >= 0 && lin < img.body_end) return lin;
      return img.body_end + (uint32_t)seg * 16 + off;   // SYSINIT-relative
    }
    return (uint32_t)seg * 16 + off;
  };

  // Scan for call targets (near E8 -> Rel; far 9A -> FarPtr).
  std::set<uint32_t> callTargets;
  for (uint32_t a = 0; a < image_size;) {
    win95::X86Insn in = win95::DecodeX86Auto(image, image_size, a, a, true);
    int len = in.valid ? in.len : 1;
    if (in.valid && in.mnem == "call") {
      if (in.ops[0].kind == win95::OpKind::Rel) callTargets.insert((uint32_t)in.ops[0].imm);
      else if (in.ops[0].kind == win95::OpKind::FarPtr)
        callTargets.insert(farToLinear(in.ops[0].far_seg, in.ops[0].far_off));
    }
    a += len;
  }

  std::map<uint32_t, std::string> names;
  if (img.entry < image_size) names[img.entry] = iosys ? "sysinit_entry" : "start";

  if (mode == "--disasm") {
    for (uint32_t a = 0; a < image_size;) {
      auto nn = names.find(a);
      if (nn != names.end()) std::printf("\n%s:\n", nn->second.c_str());
      win95::X86Insn in = win95::DecodeX86Auto(image, image_size, a, a, true);
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
    std::map<uint32_t, int> relocs;                   // segment-word fixups, 2 bytes
    for (uint32_t r : img.relocs) relocs[r] = 2;
    win95::EmitSigDump(image, image_size, ss, ee, true, &relocs);
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

  if (mode == "--tree") {
    if (argc < 4) { std::printf("usage: %s <exe> --tree <outdir>\n", argv[0]); return 2; }
    std::string outdir = argv[3];
    // Region classification. IO.SYS: MSLOAD boot stub / MSBIO+MSDOS raw body / SYSINIT
    // load module. COMMAND.COM: one flat module (resident/transient/init split refined
    // downstream by the triage pass).
    auto regionOf = [&](uint32_t a) -> const char* {
      if (!img.is_iosys) return "command";
      if (a < 0x0800) return "msload";
      if (a < img.body_end) return "msbio";     // MSBIO+MSDOS body (kernel boundary refined downstream)
      return "sysinit";
    };

    std::set<uint32_t> starts = discover();
    std::vector<uint32_t> fs(starts.begin(), starts.end());
    struct FE { uint32_t fstart, fend; std::string name, region; };
    std::vector<FE> fns;
    for (size_t i = 0; i < fs.size(); ++i) {
      uint32_t fstart = fs[i];
      uint32_t fend = (i + 1 < fs.size()) ? fs[i + 1] : (uint32_t)image_size;
      char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", fstart);
      auto nn = names.find(fstart);
      fns.push_back({fstart, fend, nn != names.end() ? nn->second : nm, regionOf(fstart)});
    }
    for (auto& fe : fns) names[fe.fstart] = fe.name;   // link internal calls
    lifter.SetNames(names);

    // Open one .c per region (append as we go), plus a manifest.
    std::map<std::string, FILE*> rf;
    auto regionFile = [&](const std::string& r) -> FILE* {
      auto it = rf.find(r);
      if (it != rf.end()) return it->second;
      std::string p = outdir + "/" + r + ".c";
      FILE* f = std::fopen(p.c_str(), "wb");
      if (f) std::fprintf(f,
        "/* %s -- %s region of %s, reconstructed by CodeBreaker win95mzdec.\n"
        " * 16-bit real-mode. Function names sub_<fileoffset> are reconstruction labels;\n"
        " * origin-triage assigns authored names where a match to the DOS 6.0 source exists. */\n\n",
        (r + ".c").c_str(), r.c_str(), img.module_name.c_str());
      rf[r] = f;
      return f;
    };

    std::string manp = outdir + "/manifest.csv";
    FILE* man = std::fopen(manp.c_str(), "wb");
    if (man) std::fprintf(man, "fstart,fend,size,region,name\n");

    bool diag = getenv("MZ_DIAG") != nullptr;
    size_t ok_c = 0;
    for (auto& fe : fns) {
      if (diag) std::fprintf(stderr, "LIFT %06X..%06X %s [%s]\n", fe.fstart, fe.fend, fe.name.c_str(), fe.region.c_str());
      ILFunction f = lifter.LiftFunction(image, image_size, fe.fstart, fe.fend, fe.name);
      std::string c = DecompileToC(f, {});
      FILE* rfp = regionFile(fe.region);
      if (rfp) { std::fprintf(rfp, "/* %s  @0x%06X..0x%06X (%u bytes) */\n%s\n",
                              fe.name.c_str(), fe.fstart, fe.fend, fe.fend - fe.fstart, c.c_str()); ok_c++; }
      if (man) std::fprintf(man, "0x%06X,0x%06X,%u,%s,%s\n",
                            fe.fstart, fe.fend, fe.fend - fe.fstart, fe.region.c_str(), fe.name.c_str());
    }
    for (auto& kv : rf) if (kv.second) std::fclose(kv.second);
    if (man) std::fclose(man);
    std::fprintf(stderr, "emitted %zu functions across %zu region file(s) to %s/ (%s)\n",
                 ok_c, rf.size(), outdir.c_str(), iosys ? "IO.SYS layout" : "MZ layout");
    return 0;
  }

  std::printf("unknown mode: %s\n", mode.c_str());
  return 2;
}
