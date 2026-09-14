// End-to-end LE (VxD) proof: LE load -> x86 lift (32-bit ABI) -> DecompileToC. The
// VxD sibling of pe_decomp.cc. LE modules carry no export name table, so function
// discovery is entry + executable-object starts + direct-call targets.
//
//   win95ledec <vxd> --module          decompile the whole module (interproc cascade)
//   win95ledec <vxd> --disasm          per-object instruction listing
//   win95ledec <vxd> --names           discovered function starts (addr<TAB>name)
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "win95/le_loader.h"
#include "win95/x86_lift.h"
#include "decompiler_c.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    std::printf("usage: %s <vxd> --module|--disasm|--names\n", argv[0]);
    return 2;
  }
  win95::LEImage img = win95::LoadLE(argv[1]);
  if (!img.ok) { std::printf("FAIL: %s\n", img.error.c_str()); return 1; }
  std::string mode = argv[2];

  const uint8_t* image = img.image.data();
  size_t image_size = img.image.size();

  win95::X86Lifter lifter;
  lifter.SetBits32(true);           // LE = 32-bit flat ring-0 code
  lifter.SetVxDMode(true);          // model int 20h VxD service calls

  // Executable object ranges.
  struct Range { uint32_t lo, hi; };
  std::vector<Range> code;
  for (auto& o : img.objects) {
    if (!o.code) continue;
    uint32_t hi = o.base_addr + o.vsize;         // object's mapped span
    if (hi > image_size || hi < o.base_addr) hi = (uint32_t)image_size;
    code.push_back({o.base_addr, hi});
  }

  auto inCode = [&](uint32_t a) {
    for (auto& r : code) if (a >= r.lo && a < r.hi) return true;
    return false;
  };

  // Scan code for direct-call targets (function starts).
  std::set<uint32_t> callTargets;
  for (auto& r : code) {
    for (uint32_t a = r.lo; a < r.hi && a < image_size;) {
      win95::X86Insn in = win95::DecodeX86(image, image_size, a, a, false);
      int len = in.valid ? in.len : 1;
      if (in.valid && in.mnem == "call" && in.ops[0].kind == win95::OpKind::Rel) {
        uint32_t t = (uint32_t)in.ops[0].imm;
        if (inCode(t)) callTargets.insert(t);
      }
      // VxD service call idiom: int 20h; dd <VMM/VxD service id>. Skip the 4-byte
      // service dword so it is not misdecoded as code.
      if (in.valid && in.mnem == "int" && in.ops[0].kind == win95::OpKind::Imm &&
          in.ops[0].imm == 0x20)
        len += 4;
      a += len;
    }
  }

  std::map<uint32_t, std::string> names;
  if (img.entry) names[img.entry] = "VxD_entry";

  // --disasm
  if (mode == "--disasm") {
    for (size_t oi = 0; oi < img.objects.size(); ++oi) {
      auto& o = img.objects[oi];
      if (!o.code) continue;
      std::printf("; ---- object %zu  base %08X  vsize %u %s ----\n",
                  oi + 1, o.base_addr, o.vsize, o.code ? "EXEC" : "");
      uint32_t hi = o.base_addr + o.vsize; if (hi > image_size) hi = (uint32_t)image_size;
      for (uint32_t a = o.base_addr; a < hi;) {
        auto nn = names.find(a);
        if (nn != names.end()) std::printf("\n%s:\n", nn->second.c_str());
        win95::X86Insn in = win95::DecodeX86(image, image_size, a, a, false);
        int len = in.valid ? in.len : 1;
        std::string bytes;
        for (int i = 0; i < len && a + i < image_size; ++i) {
          char b[4]; std::snprintf(b, sizeof b, "%02X", image[a + i]); bytes += b;
        }
        std::string text = in.valid ? win95::FormatX86(in) : "(bad)";
        if (in.valid && in.mnem == "int" && in.ops[0].kind == win95::OpKind::Imm &&
            in.ops[0].imm == 0x20 && a + len + 4 <= image_size) {
          uint32_t svc = image[a + len] | (image[a + len + 1] << 8) |
                         (image[a + len + 2] << 16) | ((uint32_t)image[a + len + 3] << 24);
          char sb[32]; std::snprintf(sb, sizeof sb, "  ; VxD service %08X", svc);
          text += sb;
          len += 4;
        }
        std::printf("  %08X %-16s %s\n", a, bytes.c_str(), text.c_str());
        a += len;
      }
    }
    return 0;
  }

  // Discovery for VxDs. A VxD object begins with the DDB (Device Descriptor Block)
  // and service/control tables -- DATA, not a function -- so the object base is NOT
  // a function start. Instead: entry + direct-call targets + every DWORD in the
  // object that points into the code range (the DDB's control proc + service tables
  // are arrays of code pointers; that is how a VxD's procedures are reached). A
  // discovered start that lands mid-instruction is harmless -- it just yields a
  // small extra function -- but it recovers the DDB-referenced procs call-scan alone
  // would miss.
  auto discover = [&]() {
    std::map<int, std::set<uint32_t>> starts;    // code-range index -> starts
    auto rangeOf = [&](uint32_t a) -> int {
      for (size_t i = 0; i < code.size(); ++i) if (a >= code[i].lo && a < code[i].hi) return (int)i;
      return -1;
    };
    auto add = [&](uint32_t a) { int r = rangeOf(a); if (r >= 0) starts[r].insert(a); };
    if (img.entry) add(img.entry);
    for (uint32_t t : callTargets) add(t);
    // An object base is a function start when it begins with a real opcode (a VxD
    // control procedure often sits right at offset 0). When it begins with 0x00 it
    // is the DDB / a data table -- decoding that as code yields garbage, so skip it
    // and let the pointer/prologue/call scans find the real entry points.
    for (auto& r : code)
      if (r.lo < image_size && image[r.lo] != 0x00) add(r.lo);
    // DWORD code-pointer scan (DDB service/control tables are pointer arrays) AND a
    // function-prologue scan. The prologue scan is relocation-independent -- it
    // finds `push ebp; mov ebp,esp` (55 8B EC) and `enter` (C8) directly -- so it
    // recovers functions even when the DDB pointers are relocated to a base our
    // flat layout doesn't match (LE fixups aren't applied here).
    for (auto& r : code) {
      for (uint32_t a = r.lo; a < r.hi && a < image_size; ++a) {
        if (a + 4 <= r.hi && a + 4 <= image_size) {
          uint32_t v = image[a] | (image[a + 1] << 8) | (image[a + 2] << 16) | ((uint32_t)image[a + 3] << 24);
          if (rangeOf(v) >= 0 && (a % 4) == 0) add(v);
        }
        if (a + 3 <= image_size && image[a] == 0x55 && image[a + 1] == 0x8B && image[a + 2] == 0xEC)
          add(a);                              // push ebp; mov ebp,esp -- the VxD prologue
      }
    }
    return starts;
  };

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

  if (mode == "--module") {
    auto starts = discover();
    struct FE { uint32_t fstart, fend; std::string name; };
    std::vector<FE> fns;
    for (auto& kv : starts) {
      uint32_t rhi = code[kv.first].hi;
      std::vector<uint32_t> fs(kv.second.begin(), kv.second.end());
      for (size_t i = 0; i < fs.size(); ++i) {
        uint32_t fstart = fs[i];
        uint32_t fend = (i + 1 < fs.size()) ? fs[i + 1] : rhi;
        char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", fstart);
        auto nn = names.find(fstart);
        fns.push_back({fstart, fend, nn != names.end() ? nn->second : nm});
      }
    }
    for (auto& fe : fns) names[fe.fstart] = fe.name;   // internal calls link by symbol
    lifter.SetNames(names);
    for (auto& fe : fns) {
      ILFunction f = lifter.LiftFunction(image, image_size, fe.fstart, fe.fend, fe.name);
      std::printf("%s\n", DecompileToC(f, {}).c_str());
    }
    std::fprintf(stderr, "decompiled %zu functions across %zu executable objects (LE/VxD)\n",
                 fns.size(), code.size());
    return 0;
  }

  std::printf("unknown mode: %s\n", mode.c_str());
  return 2;
}
