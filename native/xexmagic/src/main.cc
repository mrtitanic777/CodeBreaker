// XEXMagic - Standalone XEX decompressor / disassembler / decompiler
// Main entry point with full CLI

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "byte_order.h"
#include "codegen.h"
#include "codegen_rt360.h"
#include "coff_lib.h"
#include "il.h"
#include "il_builder.h"
#include "il_validator.h"
#include "msvc_demangle.h"
#include "ppc_decompiler.h"
#include "decompiler_c.h"
#include "ppc_disasm.h"
#include "progress.h"
#include "import_annotate.h"
#include "import_names.h"
#include "var_allocator.h"
#include "web_server.h"
#include "xex_info.h"
#include "xex_loader.h"
#include "xenon_cycles.h"

// ---------------------------------------------------------------------------
// PE section parsing from decompressed image
// ---------------------------------------------------------------------------

#include "pe_section.h"

// Read a uint16/uint32 with endian detection
static uint16_t ReadPE16(const uint8_t* p, bool big_endian) {
  if (big_endian) return (p[0] << 8) | p[1];
  return p[0] | (p[1] << 8);
}
static uint32_t ReadPE32(const uint8_t* p, bool big_endian) {
  if (big_endian)
    return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
  return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

static std::vector<PESectionInfo> ParsePESections(const uint8_t* image,
                                                   size_t image_size) {
  std::vector<PESectionInfo> sections;
  if (image_size < 0x40) return sections;

  // Try LE first, then BE for Xbox 360 PE images
  bool big_endian = false;
  uint32_t pe_offset = ReadPE32(image + 0x3C, false);
  if (pe_offset + 0x18 > image_size) {
    pe_offset = ReadPE32(image + 0x3C, true);
    big_endian = true;
  }
  if (pe_offset + 0x18 > image_size) return sections;

  // PE signature check
  if (image[pe_offset] != 'P' || image[pe_offset + 1] != 'E' ||
      image[pe_offset + 2] != 0 || image[pe_offset + 3] != 0) {
    return sections;
  }

  // COFF header at pe_offset + 4
  uint16_t num_sections = ReadPE16(image + pe_offset + 6, big_endian);
  uint16_t opt_header_size = ReadPE16(image + pe_offset + 20, big_endian);

  // Sanity check
  if (num_sections > 100 || opt_header_size > 4096) return sections;

  // Section headers start after optional header
  uint32_t section_offset = pe_offset + 24 + opt_header_size;

  for (uint16_t i = 0; i < num_sections; i++) {
    uint32_t off = section_offset + i * 40;
    if (off + 40 > image_size) break;

    PESectionInfo sec = {};
    memcpy(sec.name, image + off, 8);
    sec.name[8] = '\0';
    sec.virtual_size = ReadPE32(image + off + 8, big_endian);
    sec.virtual_address = ReadPE32(image + off + 12, big_endian);
    sec.raw_size = ReadPE32(image + off + 16, big_endian);
    sec.raw_offset = ReadPE32(image + off + 20, big_endian);
    sec.characteristics = ReadPE32(image + off + 36, big_endian);

    sections.push_back(sec);
  }
  return sections;
}

// ---------------------------------------------------------------------------
// Export table parsing
// ---------------------------------------------------------------------------

struct ExportEntry {
  uint32_t ordinal;
  uint32_t address;
};

static std::vector<ExportEntry> ParseXexExports(const XexLoader& loader) {
  std::vector<ExportEntry> exports;

  auto* sec_info = loader.xex_security_info();
  if (!sec_info || !sec_info->export_table) return exports;

  uint32_t export_table_va = sec_info->export_table;
  uint32_t base = loader.GetBaseAddress();
  uint32_t image_offset = export_table_va - base;
  auto& image = loader.GetDecompressedImage();

  if (image_offset + sizeof(xe::xex2_export_table) > image.size())
    return exports;

  auto* table =
      reinterpret_cast<const xe::xex2_export_table*>(image.data() + image_offset);

  uint32_t count = table->count;
  uint32_t base_ordinal = table->base;
  uint32_t imagebaseaddr = table->imagebaseaddr;

  for (uint32_t i = 0; i < count; i++) {
    uint32_t ord_off_raw;
    memcpy(&ord_off_raw, &table->ordOffset[i], sizeof(uint32_t));
    // The ordOffset values are big-endian
    uint32_t ord_val = ((ord_off_raw >> 24) & 0xFF) |
                       ((ord_off_raw >> 8) & 0xFF00) |
                       ((ord_off_raw << 8) & 0xFF0000) |
                       ((ord_off_raw << 24) & 0xFF000000);

    // imagebaseaddr is the image base SHIFTED RIGHT BY 16, so it has to be
    // shifted back before adding. Without this the exports come out as bare
    // RVAs, match no function entry, and every export loses its name.
    uint32_t addr = ord_val + (imagebaseaddr << 16);

    ExportEntry entry;
    entry.ordinal = base_ordinal + i;
    entry.address = addr;
    exports.push_back(entry);
  }

  return exports;
}

// ---------------------------------------------------------------------------
// Raw PE export table parsing (for xboxkrnl.exe, xam.exe, etc.)
// ---------------------------------------------------------------------------

static std::vector<ExportEntry> ParsePEExports(const uint8_t* image,
                                                size_t image_size,
                                                uint32_t base_address) {
  std::vector<ExportEntry> exports;
  if (image_size < 0x40) return exports;

  // PE headers are always little-endian, even for PPC targets.
  uint32_t pe_offset = ReadPE32(image + 0x3C, false);
  if (pe_offset + 0x78 > image_size) return exports;
  if (image[pe_offset] != 'P' || image[pe_offset + 1] != 'E') return exports;

  // Export directory RVA from data directory[0]
  uint16_t opt_magic = ReadPE16(image + pe_offset + 24, false);
  uint32_t dd_offset;
  if (opt_magic == 0x10B)
    dd_offset = pe_offset + 24 + 96;   // PE32
  else
    dd_offset = pe_offset + 24 + 112;  // PE32+

  if (dd_offset + 8 > image_size) return exports;
  uint32_t export_rva = ReadPE32(image + dd_offset, false);
  uint32_t export_size = ReadPE32(image + dd_offset + 4, false);
  if (!export_rva || !export_size) return exports;
  if (export_rva + 40 > image_size) return exports;

  const uint8_t* ed = image + export_rva;
  uint32_t num_funcs = ReadPE32(ed + 0x14, false);
  uint32_t base_ordinal = ReadPE32(ed + 0x10, false);
  uint32_t addr_table_rva = ReadPE32(ed + 0x1C, false);

  if (addr_table_rva + num_funcs * 4 > image_size) return exports;

  for (uint32_t i = 0; i < num_funcs; i++) {
    uint32_t func_rva = ReadPE32(image + addr_table_rva + i * 4, false);
    ExportEntry entry;
    entry.ordinal = base_ordinal + i;
    entry.address = base_address + func_rva;
    exports.push_back(entry);
  }

  return exports;
}

// ---------------------------------------------------------------------------
// CLI
// ---------------------------------------------------------------------------

// Find where a function ends, starting at func_start. Walks forward tracking
// the furthest forward branch target so an early `blr` inside an if/else body
// doesn't terminate the scan; the function ends at the first `blr` at or past
// every branch target seen so far.
static uint32_t ScanFunctionEnd(const uint8_t* data, size_t len,
                                uint32_t base_addr, uint32_t func_start) {
  const uint32_t kMaxScan = 0x8000;  // don't run away on a bad address
  uint32_t max_target = func_start;
  for (uint32_t a = func_start; a < base_addr + len; a += 4) {
    if (a - func_start >= kMaxScan) break;
    size_t o = a - base_addr;
    if (o + 4 > len) break;
    uint32_t w = ((uint32_t)data[o] << 24) | ((uint32_t)data[o + 1] << 16) |
                 ((uint32_t)data[o + 2] << 8) | (uint32_t)data[o + 3];
    uint32_t prim = (w >> 26) & 0x3F;
    if (prim == 18) {  // b / bl
      int32_t li = (int32_t)((w & 0x03FFFFFCu) << 6) >> 6;
      uint32_t tgt = (w & 2) ? (uint32_t)li : a + (uint32_t)li;
      if (!(w & 1)) {  // unconditional b (LK=0)
        if (tgt > max_target && tgt < func_start + kMaxScan)
          max_target = tgt;
        // An unconditional branch that nothing branches past ends the
        // function: `b` has no fall-through, and `a >= max_target` means no
        // later instruction is a branch target, so the bytes after are
        // unreachable. This catches tail restores (`b __restgprlr_NN`) and
        // tail calls (`b <other_func>`) that carry no `blr` of their own --
        // without it the scan runs into the next function and grabs *its*
        // `blr`, splicing that function's body onto this one.
        if (a >= max_target) return a + 4;
      }
    } else if (prim == 16) {  // bc
      int32_t bd = (int32_t)(int16_t)(uint16_t)(w & 0xFFFCu);
      uint32_t tgt = (w & 2) ? (uint32_t)bd : a + (uint32_t)bd;
      if (!(w & 1) && tgt > max_target && tgt < func_start + kMaxScan)
        max_target = tgt;
    }
    // blr, or an unconditional bctr tail jump, at/after the last branch
    // target ends the function.
    if ((w == 0x4E800020u || w == 0x4E800420u) && a >= max_target)
      return a + 4;  // blr / bctr
  }
  return func_start + 0x400;
}

static void print_usage(const char* argv0) {
  fprintf(stderr, "XEXMagic - Xbox 360 XEX Tool\n\n");
  fprintf(stderr, "Usage: %s [options] <input.xex>\n\n", argv0);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  --info           Show XEX header info\n");
  fprintf(stderr, "  --exports        List export ordinals and addresses\n");
  fprintf(stderr, "  --imports        List import libraries and ordinals\n");
  fprintf(stderr, "  -d, --decompress Decompress and write PE image\n");
  fprintf(stderr, "  -a, --disasm     Disassemble code sections\n");
  fprintf(stderr, "  -c, --decompile  Decompile code sections\n");
  fprintf(stderr, "  --func <addr>    Decompile ONE function to readable pseudocode\n");
  fprintf(stderr, "  --func-end <addr> Explicit end address for --func\n");
  fprintf(stderr, "  --pseudo         Readable IDA-style C (not RT360 recompile)\n");
  fprintf(stderr, "  --no-fold        Disable expression propagation\n");
  fprintf(stderr, "  --no-dce         Disable dead-code elimination\n");
  fprintf(stderr, "  -o <file>        Output file path\n");
  fprintf(stderr, "  --export-map     Print address->name map for exports\n");
  fprintf(stderr, "  --lib            Disassemble a PPC COFF archive (.lib)\n");
  fprintf(stderr, "  --lib-decompile  Decompile a PPC COFF archive to pseudocode\n");
  fprintf(stderr, "  --lib-symbols    List symbols from a PPC COFF archive\n");
  fprintf(stderr, "  --cpp            Emit C++ instead of C\n");
  fprintf(stderr, "  --namespace <n>  Wrap output in namespace (implies --cpp)\n");
  fprintf(stderr, "  --no-cycles      Suppress cycle count comments\n");
  fprintf(stderr, "  --addr-comments  Add PPC addresses as line comments\n");
  fprintf(stderr, "  --rt360          Emit RT360 static recompilation output (default)\n");
  fprintf(stderr, "  --rt360-module <name>  Set RT360 module name\n");
  fprintf(stderr, "  --rt360-compile  Compile RT360 .cpp to .dll (uses toolchain/)\n");
  fprintf(stderr, "  -h, --help       Show this help\n");
}

enum Mode {
  kModeInfo = 0,
  kModeDecompress,
  kModeDisasm,
  kModeDecompile,
  kModeExports,
  kModeImports,
  kModeExportMap,
  kModeLib,
  kModeLibDecompile,
  kModeLibSymbols,
};

extern int ApplyXexPatchCLI(const char* base_path, const char* patch_path,
                            const char* out_path);

// True if the file starts with the AR archive signature, i.e. it is a .lib
// rather than an image. Checked before the XEX loader so --serve can take
// either one.
static bool FileIsCoffArchive(const char* path) {
  FILE* f = fopen(path, "rb");
  if (!f) return false;
  char sig[8] = {};
  size_t read = fread(sig, 1, sizeof(sig), f);
  fclose(f);
  return read == sizeof(sig) && memcmp(sig, "!<arch>\n", sizeof(sig)) == 0;
}

// Serves a COFF archive through the same UI the images use. The archive is
// laid out into one flat address space, every function in it is decompiled,
// and the result is handed over as a WebProgram - so renames, xrefs, the
// memory view and the .xdb sidecar all work exactly as they do for a XEX.
static int ServeCoffArchive(const char* input_path, int port) {
  FILE* f = fopen(input_path, "rb");
  if (!f) {
    fprintf(stderr, "Failed to open: %s\n", input_path);
    return 1;
  }
  fseek(f, 0, SEEK_END);
  size_t fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  std::vector<uint8_t> buf(fsize);
  fread(buf.data(), 1, fsize, f);
  fclose(f);

  std::vector<CoffObject> objects;
  if (!ParseCoffLib(buf.data(), buf.size(), objects)) {
    fprintf(stderr, "Failed to parse COFF archive: %s\n", input_path);
    return 1;
  }

  // Leaked deliberately: the server never returns, and WebProgram borrows the
  // image for the life of the process.
  auto* img = new CoffImage();
  if (!BuildCoffImage(objects, *img)) {
    fprintf(stderr, "Archive has no section contents: %s\n", input_path);
    return 1;
  }
  printf("COFF archive: %zu objects, %zu sections, %zu functions, %zu bytes\n",
         objects.size(), img->sections.size(), img->function_starts.size(),
         img->data.size());
  fflush(stdout);

  auto* builder = new ILBuilder();
  builder->SetFunctionNames(img->symbols);
  builder->SetRelocationMap(img->relocations);
  builder->SetRelocationTargets(img->reloc_targets);

  WebProgram wp;
  wp.input_path = input_path;
  wp.is_archive = true;
  wp.base = img->base;
  wp.symbols = img->symbols;
  wp.image = &img->data;

  for (const auto& s : img->sections) {
    WebProgram::Section ws;
    ws.name = s.name;
    ws.address = s.address;
    ws.size = s.size;
    ws.file_offset = s.address - img->base;
    ws.code = s.code;
    wp.sections.push_back(ws);

    if (s.code) continue;
    // Same string harvest the image path does, so literals are searchable and
    // show up inline at their referencing instruction.
    const uint8_t* p = img->data.data() + (s.address - img->base);
    uint32_t run = 0;
    for (uint32_t i = 0; i < s.size; i++) {
      uint8_t c = p[i];
      if ((c >= 0x20 && c <= 0x7E) || c == '\t') {
        run++;
        continue;
      }
      if (c == 0 && run >= 4)
        wp.strings[s.address + i - run] =
            std::string((const char*)p + i - run, run);
      run = 0;
    }
  }

  xexmagic::progress::BeginPhase("decompiling the archive",
                                 img->function_starts.size());
  for (uint32_t func_start : img->function_starts) {
    xexmagic::progress::SetCurrent(builder->GetFunctionName(func_start));
    const CoffImage::Section* sec = nullptr;
    for (const auto& s : img->sections) {
      if (func_start >= s.address && func_start < s.address + s.size) {
        sec = &s;
        break;
      }
    }
    if (!sec || !sec->code) {
      xexmagic::progress::Complete();
      continue;
    }
    auto il_func = builder->BuildFunction(
        img->data.data() + (sec->address - img->base), sec->size, sec->address,
        func_start, img->FunctionEnd(func_start),
        builder->GetFunctionName(func_start));
    ValidateILFunction(il_func);
    ComputeFunctionCycles(il_func);
    AllocateVariables(il_func);
    wp.funcs.push_back(std::move(il_func));
    xexmagic::progress::Complete();
  }
  printf("Functions: %zu   Strings: %zu\n", wp.funcs.size(), wp.strings.size());
  fflush(stdout);

  wp.build_at = [img, builder](uint32_t addr, ILFunction* out) -> bool {
    for (const auto& s : img->sections) {
      if (!s.code || addr < s.address || addr >= s.address + s.size) continue;
      *out = builder->BuildFunction(img->data.data() + (s.address - img->base),
                                    s.size, s.address, addr,
                                    img->FunctionEnd(addr),
                                    builder->GetFunctionName(addr));
      ValidateILFunction(*out);
      ComputeFunctionCycles(*out);
      AllocateVariables(*out);
      return true;
    }
    return false;
  };

  return RunWebServer(std::move(wp), port);
}

int main(int argc, char** argv) {
  // No arguments at all: the UI is what most people want, so start it rather
  // than printing usage at them. --help still prints usage.
  std::vector<std::string> default_args;
  if (argc < 2) {
    default_args = {argv[0], "--serve"};
    static std::vector<char*> argv_default;
    argv_default.clear();
    for (auto& s : default_args) argv_default.push_back(&s[0]);
    argv = argv_default.data();
    argc = (int)argv_default.size();
  }

  // --apply-patch <xexp> <base.xex> -o <out>: apply a TU delta patch.
  {
    const char* patch_path = nullptr;
    const char* base_path = nullptr;
    const char* out_path = nullptr;
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--apply-patch") == 0 && i + 1 < argc) {
        patch_path = argv[++i];
      } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
        out_path = argv[++i];
      } else if (argv[i][0] != '-') {
        base_path = argv[i];
      }
    }
    if (patch_path) {
      if (!base_path || !out_path) {
        fprintf(stderr,
                "Usage: %s --apply-patch <patch.xexp> <base.xex> -o <out.xex>\n",
                argv[0]);
        return 1;
      }
      return ApplyXexPatchCLI(base_path, patch_path, out_path);
    }
  }

  // Default: generate RT360 DLL (static recompilation).
  // Override with --info, --disasm, -c, etc.
  Mode mode = kModeDecompile;
  const char* input_path = nullptr;
  const char* output_path = nullptr;
  // --func <addr>: decompile one function to readable pseudocode. Decompiling a
  // whole 47 MB title is useless for RE work; targeting one address is the
  // normal case.
  uint32_t target_func = 0;
  uint32_t target_func_end = 0;
  bool pseudo_mode = false;
  bool serve_mode = false;
  int serve_port = 8360;
  DecompileCOptions pseudo_opts;
  CodeGenConfig codegen_config;
  codegen_config.rt360_mode = true;
  codegen_config.rt360_compile = true;
  codegen_config.emit_address_comments = true;
  codegen_config.cpp_mode = true;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--info") == 0) {
      mode = kModeInfo;
    } else if (strcmp(argv[i], "--exports") == 0) {
      mode = kModeExports;
    } else if (strcmp(argv[i], "--imports") == 0) {
      mode = kModeImports;
    } else if (strcmp(argv[i], "-d") == 0 ||
               strcmp(argv[i], "--decompress") == 0) {
      mode = kModeDecompress;
    } else if (strcmp(argv[i], "-a") == 0 ||
               strcmp(argv[i], "--disasm") == 0) {
      mode = kModeDisasm;
    } else if (strcmp(argv[i], "-c") == 0 ||
               strcmp(argv[i], "--decompile") == 0) {
      mode = kModeDecompile;
      // Explicit decompile without --rt360 = standard C output
      codegen_config.rt360_mode = false;
      codegen_config.rt360_compile = false;
    } else if (strcmp(argv[i], "--func") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "Error: --func requires an address (e.g. 0x8259B4F8)\n");
        return 1;
      }
      target_func = (uint32_t)strtoul(argv[++i], nullptr, 0);
      mode = kModeDecompile;
      pseudo_mode = true;
      codegen_config.rt360_mode = false;
      codegen_config.rt360_compile = false;
    } else if (strcmp(argv[i], "--func-end") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "Error: --func-end requires an address\n");
        return 1;
      }
      target_func_end = (uint32_t)strtoul(argv[++i], nullptr, 0);
    } else if (strcmp(argv[i], "--pseudo") == 0) {
      mode = kModeDecompile;
      pseudo_mode = true;
      codegen_config.rt360_mode = false;
      codegen_config.rt360_compile = false;
    } else if (strcmp(argv[i], "--serve") == 0) {
      // Whole-program interactive UI. Reuses the pseudo decompile pipeline to
      // build every function, then serves them over localhost HTTP.
      mode = kModeDecompile;
      pseudo_mode = true;
      serve_mode = true;
      codegen_config.rt360_mode = false;
      codegen_config.rt360_compile = false;
    } else if (strcmp(argv[i], "--no-browser-launch") == 0) {
      // Serve headlessly: never open or re-open a UI on this machine. For
      // scripted use, where the process gets restarted after every rebuild and
      // anything that pops a window is noise.
      //
      // It also suppresses the drag-and-drop self-relaunch (the one place that
      // spawns a process) and stops a bare `xexmagic` from defaulting into the
      // UI. Grep the sources before assuming a window came from here: nothing
      // in this program calls ShellExecute or system("start").
      SetNoBrowserLaunch(true);
    } else if (strcmp(argv[i], "--port") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "--port requires a value\n");
        return 1;
      }
      serve_port = (int)strtoul(argv[++i], nullptr, 10);
    } else if (strcmp(argv[i], "--no-fold") == 0) {
      pseudo_opts.no_fold = true;
    } else if (strcmp(argv[i], "--no-dce") == 0) {
      pseudo_opts.no_dce = true;
    } else if (strcmp(argv[i], "--export-map") == 0) {
      mode = kModeExportMap;
    } else if (strcmp(argv[i], "--lib") == 0) {
      mode = kModeLib;
    } else if (strcmp(argv[i], "--lib-decompile") == 0) {
      mode = kModeLibDecompile;
    } else if (strcmp(argv[i], "--lib-symbols") == 0) {
      mode = kModeLibSymbols;
    } else if (strcmp(argv[i], "--cpp") == 0) {
      codegen_config.cpp_mode = true;
    } else if (strcmp(argv[i], "--namespace") == 0) {
      if (i + 1 < argc) {
        codegen_config.namespace_name = argv[++i];
        codegen_config.cpp_mode = true;
      } else {
        fprintf(stderr, "Error: --namespace requires a name argument\n");
        return 1;
      }
    } else if (strcmp(argv[i], "--no-cycles") == 0) {
      codegen_config.emit_cycle_comments = false;
    } else if (strcmp(argv[i], "--addr-comments") == 0) {
      codegen_config.emit_address_comments = true;
      pseudo_opts.addr_comments = true;
    } else if (strcmp(argv[i], "--rt360-compile") == 0) {
      codegen_config.rt360_compile = true;
      codegen_config.rt360_mode = true;
      codegen_config.emit_address_comments = true;
      codegen_config.cpp_mode = true;
    } else if (strcmp(argv[i], "--rt360") == 0) {
      mode = kModeDecompile;
      codegen_config.rt360_mode = true;
      codegen_config.emit_address_comments = true;
      codegen_config.cpp_mode = true;
    } else if (strcmp(argv[i], "--rt360-module") == 0) {
      if (i + 1 < argc) {
        codegen_config.rt360_module_name = argv[++i];
        codegen_config.rt360_mode = true;
        codegen_config.emit_address_comments = true;
        codegen_config.cpp_mode = true;
      } else {
        fprintf(stderr, "Error: --rt360-module requires a name argument\n");
        return 1;
      }
    } else if (strcmp(argv[i], "--trace") == 0) {
      codegen_config.trace_mode = true;
    } else if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 < argc)
        output_path = argv[++i];
      else {
        fprintf(stderr, "Error: -o requires a path argument\n");
        return 1;
      }
    } else if (strcmp(argv[i], "-h") == 0 ||
               strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return 0;
    } else if (argv[i][0] == '-') {
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      print_usage(argv[0]);
      return 1;
    } else {
      input_path = argv[i];
    }
  }

  // --serve is the one mode that does not need a file up front: it puts the
  // port up and waits for one to be dropped on the UI.
  if (!input_path && !serve_mode) {
    fprintf(stderr, "Error: no input file specified\n");
    print_usage(argv[0]);
    return 1;
  }

  // Auto-derive output path for RT360 mode when -o is not given.
  // input.xex → input.xex.RT360.cpp (compile step then produces .RT360.dll)
  static char auto_output_path[4096];
  if (!output_path && codegen_config.rt360_mode && mode == kModeDecompile) {
    snprintf(auto_output_path, sizeof(auto_output_path), "%s.RT360.cpp", input_path);
    output_path = auto_output_path;
    printf("RT360: Auto output path: %s\n", output_path);
  }

  // --serve: the port goes up before any analysis, so the browser can be
  // pointed at it straight away and watch the load instead of being refused.
  if (serve_mode) {
    if (StartWebServer(serve_port) != 0) return 1;
    // First run: index an SDK before touching an image. Library names have to
    // exist before the analysis, not after it.
    if (SetupRequired()) RunSetup();
    // --serve with nothing to open: wait on the UI for a file.
    if (!input_path) {
      printf("No file given - drop one on http://127.0.0.1:%d/\n", serve_port);
      fflush(stdout);
      SetAwaitingFile(true);
      WaitWebServer();
      return 0;
    }
    xexmagic::progress::BeginPhase("loading the image");
  }

  // --serve on a .lib: the UI works on the archive laid out as one image.
  if (serve_mode && input_path && FileIsCoffArchive(input_path)) {
    return ServeCoffArchive(input_path, serve_port);
  }

  // Handle .lib modes separately (no XEX loading)
  if (mode == kModeLib || mode == kModeLibSymbols || mode == kModeLibDecompile) {
    FILE* f = fopen(input_path, "rb");
    if (!f) {
      fprintf(stderr, "Failed to open: %s\n", input_path);
      return 1;
    }
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::vector<uint8_t> buf(fsize);
    fread(buf.data(), 1, fsize, f);
    fclose(f);

    std::vector<CoffObject> objects;
    if (!ParseCoffLib(buf.data(), buf.size(), objects)) {
      fprintf(stderr, "Failed to parse COFF archive: %s\n", input_path);
      return 1;
    }

    FILE* out = stdout;
    if (output_path) {
      out = fopen(output_path, "w");
      if (!out) {
        fprintf(stderr, "Failed to open output: %s\n", output_path);
        return 1;
      }
    }

    if (mode == kModeLibDecompile) {
      // Lay the archive out into one address space first. COFF sections all
      // start at virtual address 0, so decompiling them in place made every
      // object's functions share addresses - and therefore share one name.
      CoffImage img;
      if (!BuildCoffImage(objects, img)) {
        fprintf(stderr, "Archive has no section contents: %s\n", input_path);
        return 1;
      }

      ILBuilder builder;
      builder.SetFunctionNames(img.symbols);
      builder.SetRelocationMap(img.relocations);
      builder.SetRelocationTargets(img.reloc_targets);

      std::vector<ILFunction> all_funcs;
      for (uint32_t func_start : img.function_starts) {
        const CoffImage::Section* sec = nullptr;
        for (const auto& s : img.sections) {
          if (func_start >= s.address && func_start < s.address + s.size) {
            sec = &s;
            break;
          }
        }
        if (!sec || !sec->code) continue;

        auto il_func = builder.BuildFunction(
            img.data.data() + (sec->address - img.base), sec->size,
            sec->address, func_start, img.FunctionEnd(func_start),
            builder.GetFunctionName(func_start));

        ValidateILFunction(il_func);
        ComputeFunctionCycles(il_func);
        AllocateVariables(il_func);
        all_funcs.push_back(std::move(il_func));
      }

      std::string source;
      if (codegen_config.rt360_mode) {
        source = GenerateRT360Source(all_funcs, codegen_config, nullptr, input_path);
      } else {
        source = GenerateSource(all_funcs, codegen_config, input_path);
      }
      fprintf(out, "%s", source.c_str());

      // Write .rt360map if in RT360 mode
      if (codegen_config.rt360_mode && output_path) {
        std::string map_path = std::string(output_path) + ".rt360map";
        FILE* map_file = fopen(map_path.c_str(), "w");
        if (map_file) {
          std::string map = GenerateRT360SourceMap(all_funcs, codegen_config, input_path);
          fprintf(map_file, "%s", map.c_str());
          fclose(map_file);
          printf("RT360 source map written to: %s\n", map_path.c_str());
        }
      }
    } else if (mode == kModeLibSymbols) {
      fprintf(out, "COFF Archive: %s (%zu objects)\n\n", input_path,
              objects.size());
      for (auto& obj : objects) {
        if (obj.symbols.empty()) continue;
        fprintf(out, "--- %s ---\n", obj.member_name.c_str());
        for (auto& sym : obj.symbols) {
          std::string demangled = MsvcDemangle(sym);
          if (demangled != sym) {
            fprintf(out, "  %s\n    -> %s\n", sym.c_str(), demangled.c_str());
          } else {
            fprintf(out, "  %s\n", sym.c_str());
          }
        }
        fprintf(out, "\n");
      }
    } else {
      fprintf(out, "; COFF Archive disassembly: %s (%zu objects)\n\n",
              input_path, objects.size());
      for (auto& obj : objects) {
        bool has_code = false;
        for (size_t si = 0; si < obj.sections.size(); si++) {
          auto& sec = obj.sections[si];
          if ((sec.characteristics & 0x00000020) ||
              (sec.characteristics & 0x20000000)) {
            has_code = true;
            break;
          }
        }
        if (!has_code) continue;

        fprintf(out, "; ====== Object: %s ======\n", obj.member_name.c_str());
        if (!obj.symbols.empty()) {
          fprintf(out, "; Exports:\n");
          for (auto& sym : obj.symbols) {
            std::string demangled = MsvcDemangle(sym);
            if (demangled != sym) {
              fprintf(out, ";   %s\n", demangled.c_str());
            } else {
              fprintf(out, ";   %s\n", sym.c_str());
            }
          }
        }

        // Build symbol map for labels: (section_index, offset) -> name
        auto sym_map = obj.BuildSectionSymbolMap();

        for (size_t si = 0; si < obj.sections.size(); si++) {
          auto& sec = obj.sections[si];
          bool is_code = (sec.characteristics & 0x00000020) ||
                         (sec.characteristics & 0x20000000);
          if (!is_code || sec.data.empty()) continue;

          fprintf(out, "; --- %s (size=0x%X, flags=0x%08X, relocs=%u) ---\n",
                  sec.name.c_str(), sec.raw_size, sec.characteristics,
                  (unsigned)sec.relocations.size());

          // Build relocation map: offset_in_section -> symbol name
          std::map<uint32_t, std::string> reloc_map;
          for (auto& rel : sec.relocations) {
            std::string sym_name = obj.GetSymbolName(rel.symbol_index);
            if (!sym_name.empty()) {
              std::string demangled = MsvcDemangle(sym_name);
              reloc_map[rel.offset] = (demangled != sym_name) ? demangled : sym_name;
            }
          }

          auto lines =
              DisasmBuffer(sec.data.data(), sec.data.size(), sec.virtual_address);
          for (auto& line : lines) {
            uint32_t offset_in_sec = line.address - sec.virtual_address;

            // Print label if a symbol is defined at this offset
            auto label_it = sym_map.find({(int)si, offset_in_sec});
            if (label_it != sym_map.end()) {
              std::string demangled = MsvcDemangle(label_it->second);
              fprintf(out, "\n%s:\n",
                      (demangled != label_it->second) ? demangled.c_str()
                                                       : label_it->second.c_str());
            }

            // Check if this instruction has a relocation
            auto rel_it = reloc_map.find(offset_in_sec);
            if (rel_it != reloc_map.end()) {
              fprintf(out, "%08X:  %08X  %-40s ; -> %s\n",
                      line.address, line.code, line.text.c_str(),
                      rel_it->second.c_str());
            } else {
              fprintf(out, "%08X:  %08X  %s\n", line.address, line.code,
                      line.text.c_str());
            }
          }
          fprintf(out, "\n");
        }
        fprintf(out, "\n");
      }
    }

    if (output_path && out != stdout) {
      fclose(out);
      printf("Output written to: %s\n", output_path);
    }
    return 0;
  }

  // Detect file type: raw PPC PE (MZ header) or XEX
  bool is_raw_pe = false;
  std::vector<uint8_t> raw_pe_image;
  uint32_t raw_pe_base = 0;
  uint32_t raw_pe_entry = 0;
  {
    FILE* probe = fopen(input_path, "rb");
    if (probe) {
      uint8_t magic[4] = {};
      fread(magic, 1, 4, probe);
      // MZ header = raw PE; XEX2/XEX1 = XEX wrapper
      if (magic[0] == 'M' && magic[1] == 'Z') {
        is_raw_pe = true;
        fseek(probe, 0, SEEK_END);
        size_t sz = ftell(probe);
        fseek(probe, 0, SEEK_SET);
        raw_pe_image.resize(sz);
        fread(raw_pe_image.data(), 1, sz, probe);

        // Parse PE header for base address and entry point
        bool be = false;
        uint32_t pe_off = ReadPE32(raw_pe_image.data() + 0x3C, false);
        if (pe_off + 0x30 > sz) {
          pe_off = ReadPE32(raw_pe_image.data() + 0x3C, true);
          be = true;
        }
        // PE headers are always little-endian, even for PPC targets.
        // Only the code/data sections contain big-endian PPC instructions.
        be = false;
        raw_pe_base = ReadPE32(raw_pe_image.data() + pe_off + 24 + 28, false);
        raw_pe_entry = raw_pe_base + ReadPE32(raw_pe_image.data() + pe_off + 24 + 16, false);
        printf("Raw PPC PE: base=0x%08X entry=0x%08X size=%zu\n",
               raw_pe_base, raw_pe_entry, sz);
      }
      fclose(probe);
    }
  }

  // Load XEX (or skip for raw PE)
  XexLoader loader;
  if (!is_raw_pe) {
    if (!loader.LoadFromFile(input_path)) {
      fprintf(stderr, "Failed to load XEX file: %s\n", input_path);
      return 1;
    }
    loader.ParseImports();
    loader.ParseExports();
  }

  switch (mode) {
    // -----------------------------------------------------------------------
    // Info mode (default)
    // -----------------------------------------------------------------------
    case kModeInfo: {
      if (is_raw_pe) {
        printf("XEXMagic - Raw PPC PE Info\n");
        printf("File: %s\n\n", input_path);
        printf("Base address: 0x%08X\n", raw_pe_base);
        printf("Entry point:  0x%08X\n", raw_pe_entry);
        printf("Image size:   %zu bytes\n", raw_pe_image.size());
        auto sections = ParsePESections(raw_pe_image.data(), raw_pe_image.size());
        printf("\nPE Sections:\n");
        for (auto& s : sections)
          printf("  %-10s VA=0x%08X  Size=0x%08X  Flags=0x%08X%s%s\n",
                 s.name, s.virtual_address, s.virtual_size, s.characteristics,
                 (s.characteristics & 0x20000000) ? " [EXEC]" : "",
                 (s.characteristics & 0x00000020) ? " [CODE]" : "");
        auto pe_exports = ParsePEExports(raw_pe_image.data(), raw_pe_image.size(), raw_pe_base);
        printf("\nExports: %zu\n", pe_exports.size());
        break;
      }
      printf("XEXMagic - XEX Info\n");
      printf("File: %s\n\n", input_path);
      printf("Format:       %s\n",
             loader.format() == XexLoader::kFormatXex1 ? "XEX1" : "XEX2");
      printf("Encryption:   %s key\n",
             loader.is_dev_kit() ? "devkit" : "retail");
      printf("Base address: 0x%08X\n", loader.GetBaseAddress());
      printf("Entry point:  0x%08X\n", loader.GetEntryPoint());
      printf("Title ID:     0x%08X\n", loader.GetTitleId());
      printf("Image size:   %u bytes\n",
             static_cast<uint32_t>(loader.GetDecompressedImage().size()));
      printf("Page size:    %u bytes\n", loader.page_size());

      auto* exec_info = loader.opt_execution_info();
      if (exec_info) {
        auto ver = exec_info->version();
        printf("Version:      %u.%u.%u.%u\n", ver.major, ver.minor,
               ver.build, ver.qfe);
        printf("Media ID:     0x%08X\n", (uint32_t)exec_info->media_id);
        printf("Disc:         %u of %u\n", exec_info->disc_number,
               exec_info->disc_count);
      }

      // PE sections
      auto& image = loader.GetDecompressedImage();
      auto sections = ParsePESections(image.data(), image.size());
      if (!sections.empty()) {
        printf("\nPE Sections:\n");
        for (auto& sec : sections) {
          printf("  %-8s  VA=0x%08X  Size=0x%08X  Flags=0x%08X",
                 sec.name, sec.virtual_address, sec.virtual_size,
                 sec.characteristics);
          if (sec.characteristics & kXEPESectionContainsCode)
            printf(" [CODE]");
          if (sec.characteristics & kXEPESectionMemoryExecute)
            printf(" [EXEC]");
          printf("\n");
        }
      }

      // Import summary
      auto& libs = loader.import_libraries();
      if (!libs.empty()) {
        printf("\nImport libraries: %zu\n", libs.size());
        for (auto& lib : libs) {
          printf("  %s (%zu imports)\n", lib.name.c_str(), lib.imports.size());
        }
      }

      // Export summary
      auto exports = ParseXexExports(loader);
      if (!exports.empty()) {
        printf("\nExports: %zu\n", exports.size());
      }

      break;
    }

    // -----------------------------------------------------------------------
    // Decompress mode
    // -----------------------------------------------------------------------
    case kModeDecompress: {
      const char* out = output_path ? output_path : "output.bin";
      auto& image = loader.GetDecompressedImage();
      FILE* f = fopen(out, "wb");
      if (!f) {
        fprintf(stderr, "Failed to open output file: %s\n", out);
        return 1;
      }
      fwrite(image.data(), 1, image.size(), f);
      fclose(f);
      printf("Decompressed PE image written to: %s (%zu bytes)\n", out,
             image.size());
      break;
    }

    // -----------------------------------------------------------------------
    // Exports mode
    // -----------------------------------------------------------------------
    case kModeExports: {
      auto exports = ParseXexExports(loader);
      if (exports.empty()) {
        printf("No exports found.\n");
      } else {
        printf("Exports (%zu):\n", exports.size());
        printf("  %-10s %s\n", "Ordinal", "Address");
        printf("  %-10s %s\n", "-------", "----------");
        for (auto& exp : exports) {
          printf("  %-10u 0x%08X\n", exp.ordinal, exp.address);
        }
      }
      break;
    }

    // -----------------------------------------------------------------------
    // Export map mode
    // -----------------------------------------------------------------------
    case kModeExportMap: {
      auto exports = ParseXexExports(loader);
      for (auto& exp : exports) {
        printf("0x%08X = ordinal_%u\n", exp.address, exp.ordinal);
      }
      break;
    }

    // -----------------------------------------------------------------------
    // Imports mode
    // -----------------------------------------------------------------------
    case kModeImports: {
      auto& libs = loader.import_libraries();
      if (libs.empty()) {
        printf("No imports found.\n");
      } else {
        printf("Import libraries (%zu):\n\n", libs.size());
        for (auto& lib : libs) {
          auto ver = lib.version;
          printf("%s  (id=0x%08X, v%u.%u.%u.%u)\n", lib.name.c_str(), lib.id,
                 ver.major, ver.minor, ver.build, ver.qfe);
          ImportNameTable names;
          names.Build({lib});

          for (auto& imp : lib.imports) {
            const char* n = imp.thunk_address
                                ? names.LookupThunk(imp.thunk_address)
                                : names.LookupValue(imp.value_address);
            printf("  ordinal %-6u  value_addr=0x%08X  thunk_addr=0x%08X  %s\n",
                   imp.ordinal, imp.value_address, imp.thunk_address,
                   n ? n : "");
          }
          printf("\n");
        }
      }
      break;
    }

    // -----------------------------------------------------------------------
    // Disasm mode
    // -----------------------------------------------------------------------
    case kModeDisasm: {
      const auto& image = is_raw_pe ? raw_pe_image : loader.GetDecompressedImage();
      auto sections = ParsePESections(image.data(), image.size());
      uint32_t base = is_raw_pe ? raw_pe_base : loader.GetBaseAddress();

      FILE* out = stdout;
      if (output_path) {
        out = fopen(output_path, "w");
        if (!out) {
          fprintf(stderr, "Failed to open output file: %s\n", output_path);
          return 1;
        }
      }

      // Import names are the single most useful annotation in a XEX listing:
      // every call into the kernel is otherwise a bare address.
      ImportNameTable imports;
      if (!is_raw_pe) {
        imports.Build(loader.import_libraries());
        if (!imports.empty()) {
          fprintf(out, "; %zu kernel imports resolved to names\n\n",
                  imports.size());
        }
      }

      bool found_code = false;
      for (auto& sec : sections) {
        bool is_code = (sec.characteristics & kXEPESectionContainsCode) ||
                       (sec.characteristics & kXEPESectionMemoryExecute);
        if (!is_code) continue;
        found_code = true;

        fprintf(out, "; === Section: %s (VA=0x%08X, Size=0x%X) ===\n\n",
                sec.name, sec.virtual_address, sec.virtual_size);

        // The section's virtual address is relative to the image base in the PE
        uint32_t sec_offset = sec.virtual_address;
        uint32_t sec_size = sec.virtual_size;
        if (sec.raw_size > 0 && sec.raw_size < sec_size)
          sec_size = sec.raw_size;

        if (sec_offset + sec_size > image.size()) {
          sec_size = (uint32_t)(image.size() - sec_offset);
        }

        uint32_t sec_addr = base + sec_offset;
        auto lines =
            DisasmBuffer(image.data() + sec_offset, sec_size, sec_addr);

        AnnotateImportCalls(lines, imports);

        for (auto& line : lines) {
          fprintf(out, "%08X:  %08X  %s\n", line.address, line.code,
                  line.text.c_str());
        }
        fprintf(out, "\n");
      }

      if (!found_code) {
        // No PE sections with code flag found; disassemble entire image
        fprintf(out, "; === Full image disassembly ===\n\n");
        auto lines = DisasmBuffer(image.data(), image.size(), base);
        AnnotateImportCalls(lines, imports);
        for (auto& line : lines) {
          fprintf(out, "%08X:  %08X  %s\n", line.address, line.code,
                  line.text.c_str());
        }
      }

      if (output_path && out != stdout) {
        fclose(out);
        printf("Disassembly written to: %s\n", output_path);
      }
      break;
    }

    // -----------------------------------------------------------------------
    // Decompile mode
    // -----------------------------------------------------------------------
    case kModeDecompile: {
      const auto& image = is_raw_pe ? raw_pe_image : loader.GetDecompressedImage();
      auto sections = ParsePESections(image.data(), image.size());
      uint32_t base = is_raw_pe ? raw_pe_base : loader.GetBaseAddress();

      // Build export address map
      std::vector<ExportEntry> exports;
      if (is_raw_pe) {
        exports = ParsePEExports(image.data(), image.size(), base);
        printf("Raw PE: Found %zu exports\n", exports.size());
      } else {
        exports = ParseXexExports(loader);
      }
      // Resolve export ordinals to their real kernel/xam names when the image
      // is a known module (xboxkrnl.exe / xam.xex). The module is taken from
      // the input filename stem, matched case-insensitively.
      std::string module_stem;
      if (input_path) {
        module_stem = input_path;
        size_t slash = module_stem.find_last_of("/\\");
        if (slash != std::string::npos) module_stem = module_stem.substr(slash + 1);
        size_t dot = module_stem.find('.');
        if (dot != std::string::npos) module_stem = module_stem.substr(0, dot);
        for (auto& c : module_stem) {
          if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
        }
      }
      // Accept decorated stems like "xboxkrnl_17559" or "xam.20th".
      const char* module_name = nullptr;
      if (module_stem.rfind("xboxkrnl", 0) == 0) {
        module_name = "xboxkrnl";
      } else if (module_stem.rfind("xam", 0) == 0) {
        module_name = "xam";
      }

      std::map<uint32_t, std::string> export_map;
      std::vector<uint32_t> export_addrs;
      for (auto& exp : exports) {
        const char* known =
            module_name ? LookupKernelOrdinal(module_name, exp.ordinal) : nullptr;
        if (known) {
          export_map[exp.address] = known;
        } else {
          char name[32];
          snprintf(name, sizeof(name), "export_%u", exp.ordinal);
          export_map[exp.address] = name;
        }
        export_addrs.push_back(exp.address);
      }
      if (module_name) {
        printf("Ordinal map: resolved names via '%s' export table\n",
               module_name);
      }

      // Entry point
      uint32_t entry = is_raw_pe ? raw_pe_entry : loader.GetEntryPoint();
      if (entry) {
        export_map[entry] = "entry_point";
        export_addrs.push_back(entry);
      }

      // Read extra seed addresses from file (if exists).
      {
        FILE* sf = fopen("extra_seeds.txt", "r");
        if (sf) {
          char line[64];
          int count = 0;
          while (fgets(line, sizeof(line), sf)) {
            uint32_t addr = 0;
            if (sscanf(line, "0x%X", &addr) == 1 && addr != 0) {
              export_addrs.push_back(addr);
              count++;
            }
          }
          fclose(sf);
          printf("RT360: Read %d seed addresses from extra_seeds.txt\n", count);
        }
      }

      // Collect import thunk addresses — these are NOT real code.
      std::set<uint32_t> import_thunk_addrs;
      if (!is_raw_pe) {
        for (auto& lib : loader.import_libraries()) {
          for (auto& imp : lib.imports) {
            if (imp.thunk_address) {
              import_thunk_addrs.insert(imp.thunk_address);
              import_thunk_addrs.insert(imp.thunk_address - 4);
              import_thunk_addrs.insert(imp.thunk_address - 8);
              import_thunk_addrs.insert(imp.thunk_address + 4);
            }
            if (imp.value_address) {
              import_thunk_addrs.insert(imp.value_address);
            }
          }
        }
      }

      // Also scan code sections for sc 2 instructions (0x44000002) — these are
      // import thunks that XEXMagic's import parser didn't capture (e.g., xam).
      {
        for (auto& sec : sections) {
          if (!(sec.characteristics & 0x20000000)) continue;  // code sections only
          for (uint32_t off = 0; off + 3 < sec.raw_size; off += 4) {
            size_t file_off = sec.raw_offset + off;
            if (file_off + 3 >= image.size()) break;
            uint32_t instr = (image[file_off] << 24) | (image[file_off+1] << 16) |
                             (image[file_off+2] << 8) | image[file_off+3];
            if (instr == 0x44000002) {  // sc 2
              uint32_t addr = base + sec.virtual_address + off;
              for (int j = -8; j <= 8; j += 4) {
                import_thunk_addrs.insert(addr + j);
              }
            }
          }
        }
      }

      printf("RT360: Excluding %zu import thunk addresses from decompilation\n",
             import_thunk_addrs.size()); fflush(stdout);

      // Pass import thunk entry addresses to codegen for inline HLE calls.
      // Only the actual thunk entry addresses (not the ±4/±8 padding).
      if (!is_raw_pe) {
        for (auto& lib : loader.import_libraries()) {
          for (auto& imp : lib.imports) {
            if (imp.thunk_address)
              codegen_config.import_thunk_addrs.insert(imp.thunk_address);
          }
        }
      }

      // New IL pipeline
      ILBuilder builder;
      builder.SetImportThunkAddresses(import_thunk_addrs);
      builder.SetFunctionNames(export_map);

      FILE* out = stdout;
      if (output_path) {
        out = fopen(output_path, "w");
        if (!out) {
          fprintf(stderr, "Failed to open output file: %s\n", output_path);
          return 1;
        }
      }

      std::vector<ILFunction> all_funcs;
      bool found_code = false;

      // --func <addr>: build IL for just this one function and emit readable
      // pseudocode, skipping whole-image analysis entirely.
      if (pseudo_mode && target_func) {
        for (auto& sec : sections) {
          bool is_code = (sec.characteristics & kXEPESectionContainsCode) ||
                         (sec.characteristics & kXEPESectionMemoryExecute);
          if (!is_code) continue;

          uint32_t sec_offset = sec.virtual_address;
          uint32_t sec_size = sec.virtual_size;
          if (sec.raw_size > 0 && sec.raw_size < sec_size)
            sec_size = sec.raw_size;
          if (sec_offset + sec_size > image.size())
            sec_size = (uint32_t)(image.size() - sec_offset);
          uint32_t sec_addr = base + sec_offset;

          if (target_func < sec_addr || target_func >= sec_addr + sec_size)
            continue;

          // Without an explicit end, stop at the first terminator (blr/b) that
          // isn't followed by more of this function; ScanFunctionEnd handles it.
          uint32_t fend = target_func_end;
          if (!fend) fend = ScanFunctionEnd(image.data() + sec_offset, sec_size,
                                            sec_addr, target_func);

          auto f = builder.BuildFunction(
              image.data() + sec_offset, sec_size, sec_addr, target_func, fend,
              builder.GetFunctionName(target_func));
          ValidateILFunction(f);
          ComputeFunctionCycles(f);

          SetGlobalSymbolMap(export_map);
          for (auto& exp : exports)
            if (exp.address == target_func) pseudo_opts.ordinal = exp.ordinal;
          fprintf(out, "%s", DecompileToC(f, pseudo_opts).c_str());
          if (out != stdout) fclose(out);
          return 0;
        }
        fprintf(stderr, "Address %08X not found in any code section\n",
                target_func);
        return 1;
      }

      for (auto& sec : sections) {
        bool is_code = (sec.characteristics & kXEPESectionContainsCode) ||
                       (sec.characteristics & kXEPESectionMemoryExecute);
        if (!is_code) continue;
        found_code = true;

        uint32_t sec_offset = sec.virtual_address;
        uint32_t sec_size = sec.virtual_size;
        if (sec.raw_size > 0 && sec.raw_size < sec_size)
          sec_size = sec.raw_size;
        if (sec_offset + sec_size > image.size())
          sec_size = (uint32_t)(image.size() - sec_offset);

        uint32_t sec_addr = base + sec_offset;

        auto funcs = builder.DetectAndBuildFunctions(
            image.data() + sec_offset, sec_size, sec_addr, export_addrs);

        for (auto& f : funcs) {
          ValidateILFunction(f);
          ComputeFunctionCycles(f);
          AllocateVariables(f);
          all_funcs.push_back(std::move(f));
        }
      }

      if (!found_code) {
        auto funcs = builder.DetectAndBuildFunctions(
            image.data(), image.size(), base, export_addrs);
        for (auto& f : funcs) {
          ValidateILFunction(f);
          ComputeFunctionCycles(f);
          AllocateVariables(f);
          all_funcs.push_back(std::move(f));
        }
      }

      // --serve: hand the whole-program model to the interactive web UI.
      if (serve_mode) {
        if (out != stdout) fclose(out);
        WebProgram wp;
        wp.module_name = module_name ? module_name : "";
        wp.input_path = input_path ? input_path : "image";
        wp.base = base;
        wp.symbols = export_map;
        for (auto& exp : exports) wp.ordinals[exp.address] = exp.ordinal;
        wp.entry_point = entry;
        // Harvest NUL-terminated printable strings from the data sections so
        // they can be searched, cross-referenced, and printed inline in code.
        for (const auto& sec : sections) {
          if ((sec.characteristics & kXEPESectionContainsCode) ||
              (sec.characteristics & kXEPESectionMemoryExecute))
            continue;  // code sections: not string data
          // A section whose virtual address is past the loaded image has no
          // bytes here to scan. Guard it: image.size() - virtual_address would
          // wrap to ~4GB, the clamp below would keep that huge ssize, and the
          // loop would read gigabytes off the end of the buffer - the runaway
          // that ballooned the process past 2GB and then faulted.
          if (sec.virtual_address >= image.size()) continue;
          uint32_t avail = (uint32_t)(image.size() - sec.virtual_address);
          uint32_t ssize = sec.virtual_size;
          if (sec.raw_size > 0 && sec.raw_size < ssize) ssize = sec.raw_size;
          if (ssize > avail) ssize = avail;
          const uint8_t* p = image.data() + sec.virtual_address;
          uint32_t run = 0;
          for (uint32_t i = 0; i < ssize; i++) {
            uint8_t c = p[i];
            bool printable = (c >= 0x20 && c <= 0x7E) || c == '\t';
            if (printable) {
              run++;
            } else {
              // Only a NUL terminator makes it a C string.
              if (c == 0 && run >= 4) {
                uint32_t start = i - run;
                wp.strings[base + sec.virtual_address + start] =
                    std::string((const char*)p + start, run);
              }
              run = 0;
            }
          }
        }
        printf("Strings: %zu\n", wp.strings.size());
        fflush(stdout);
        // Imports, with the ordinal resolved to a name where the baked kernel
        // tables know it. Searchable alongside the SDK's own symbols.
        for (const auto& lib : loader.import_libraries()) {
          ImportNameTable names;
          names.Build({lib});
          for (const auto& imp : lib.imports) {
            WebProgram::Import wi;
            wi.module = lib.name;
            wi.ordinal = imp.ordinal;
            wi.thunk_address = imp.thunk_address;
            wi.value_address = imp.value_address;
            const char* n = imp.thunk_address
                                ? names.LookupThunk(imp.thunk_address)
                                : names.LookupValue(imp.value_address);
            if (n) wi.name = n;
            wp.imports.push_back(std::move(wi));
          }
        }
        printf("Imports: %zu\n", wp.imports.size());
        fflush(stdout);
        // Section map + image view for the memory/table tools.
        wp.image = &image;
        for (const auto& sec : sections) {
          WebProgram::Section s;
          s.name = std::string(sec.name);
          s.address = base + sec.virtual_address;
          uint32_t ssz = sec.virtual_size;
          if (sec.raw_size > 0 && sec.raw_size < ssz) ssz = sec.raw_size;
          // Clamp to the bytes actually present in the loaded image. A section
          // whose virtual address is past the image has none, and the old
          // image.size() - virtual_address wrapped to ~4GB - a "negative" size
          // that the memory/table views then read off the end of the buffer.
          uint32_t savail = sec.virtual_address < image.size()
                                ? (uint32_t)(image.size() - sec.virtual_address)
                                : 0;
          if (ssz > savail) ssz = savail;
          s.size = ssz;
          s.file_offset = sec.virtual_address;
          s.code = (sec.characteristics & kXEPESectionContainsCode) ||
                   (sec.characteristics & kXEPESectionMemoryExecute);
          wp.sections.push_back(s);
        }
        wp.funcs = std::move(all_funcs);
        // On-demand builder for addresses whole-program detection missed, so
        // the UI can navigate to any call target.
        auto* img = &image;
        auto secs = sections;
        auto* bld = new ILBuilder(builder);
        wp.build_at = [img, secs, base, bld](uint32_t addr,
                                             ILFunction* out) -> bool {
          for (const auto& sec : secs) {
            bool is_code = (sec.characteristics & kXEPESectionContainsCode) ||
                           (sec.characteristics & kXEPESectionMemoryExecute);
            if (!is_code) continue;
            uint32_t sec_size = sec.virtual_size;
            if (sec.raw_size > 0 && sec.raw_size < sec_size)
              sec_size = sec.raw_size;
            uint32_t avail = sec.virtual_address < img->size()
                                 ? (uint32_t)(img->size() - sec.virtual_address)
                                 : 0;
            if (sec_size > avail) sec_size = avail;
            uint32_t sec_addr = base + sec.virtual_address;
            if (addr < sec_addr || addr >= sec_addr + sec_size) continue;
            const uint8_t* data = img->data() + sec.virtual_address;
            uint32_t end = ScanFunctionEnd(data, sec_size, sec_addr, addr);
            *out = bld->BuildFunction(data, sec_size, sec_addr, addr, end,
                                      bld->GetFunctionName(addr));
            ValidateILFunction(*out);
            ComputeFunctionCycles(*out);
            AllocateVariables(*out);
            return true;
          }
          return false;
        };
        return RunWebServer(std::move(wp), serve_port);
      }

      // Detect PPC save/restore stubs for inline codegen
      if (codegen_config.rt360_mode) {
        static constexpr uint32_t gprlr_pattern[] = {
          0x68FFC1F9, 0x70FFE1F9, 0x78FF01FA, 0x80FF21FA,  // savegprlr 14-17
          0x88FF41FA, 0x90FF61FA, 0x98FF81FA, 0xA0FFA1FA,  // 18-21
          0xA8FFC1FA, 0xB0FFE1FA, 0xB8FF01FB, 0xC0FF21FB,  // 22-25
          0xC8FF41FB, 0xD0FF61FB, 0xD8FF81FB, 0xE0FFA1FB,  // 26-29
          0xE8FFC1FB, 0xF0FFE1FB,                            // 30-31
          0xF8FF8191, 0x2000804E,                            // stw r12,-8(r1); blr
          0x68FFC1E9, 0x70FFE1E9, 0x78FF01EA, 0x80FF21EA,  // restgprlr 14-17
        };
        const size_t pattern_size = sizeof(gprlr_pattern);
        for (size_t i = 0; i + pattern_size <= image.size(); i += 4) {
          if (memcmp(image.data() + i, gprlr_pattern, pattern_size) == 0) {
            uint32_t addr = base + (uint32_t)i;
            codegen_config.gplr_save_start = addr;          // __savegprlr_14
            codegen_config.gplr_rest_start = addr + 20 * 4; // __restgprlr_14
            printf("RT360: Found savegprlr stubs at 0x%08X, restgprlr at 0x%08X\n",
                   codegen_config.gplr_save_start, codegen_config.gplr_rest_start);
            break;
          }
        }
      }

      std::string source;
      if (codegen_config.rt360_mode) {
        // Build guest init data from the loaded XEX
        RT360GuestInitData init_data = BuildRT360GuestInitData(
            loader, sections, image.data(), image.size());
        // Embed the raw XEX file in the DLL
        init_data.xex_image = loader.GetRawXexData();
        if (!codegen_config.rt360_module_name.empty())
          init_data.module_name = codegen_config.rt360_module_name;
        printf("RT360 guest init: %zu data sections, %zu import thunks, "
               "%zu header fields, %zu exports\n",
               init_data.data_sections.size(), init_data.import_thunks.size(),
               init_data.header_fields.size(), init_data.exports.size());
        if (init_data.tls_info.slot_count > 0)
          printf("  TLS: %u slots, %u bytes template data\n",
                 init_data.tls_info.slot_count,
                 (uint32_t)init_data.tls_info.template_data.size());
        source = GenerateRT360Source(all_funcs, codegen_config, &init_data, input_path);
      } else {
        source = GenerateSource(all_funcs, codegen_config, input_path);
      }
      fprintf(out, "%s", source.c_str());

      if (output_path && out != stdout) {
        fclose(out);
        printf("Decompiler output written to: %s\n", output_path);

        // Write .rt360map if in RT360 mode
        if (codegen_config.rt360_mode) {
          std::string map_path = std::string(output_path) + ".rt360map";
          FILE* map_file = fopen(map_path.c_str(), "w");
          if (map_file) {
            std::string map = GenerateRT360SourceMap(all_funcs, codegen_config, input_path);
            fprintf(map_file, "%s", map.c_str());
            fclose(map_file);
            printf("RT360 source map written to: %s\n", map_path.c_str());
          }
        }

        // Auto-compile RT360 .cpp → .dll if --rt360-compile
        if (codegen_config.rt360_compile && codegen_config.rt360_mode) {
          // Find toolchain relative to executable
          // Try: <exe_dir>/toolchain/llvm-mingw-*/bin/clang++.exe
          // Or:  <exe_dir>/../toolchain/llvm-mingw-*/bin/clang++.exe
          std::string cpp_path = output_path;
          std::string dll_path = cpp_path;
          // Replace .cpp with .dll, or append .RT360.dll
          size_t dot = dll_path.rfind('.');
          if (dot != std::string::npos && dll_path.substr(dot) == ".cpp")
            dll_path = dll_path.substr(0, dot) + ".dll";
          else
            dll_path += ".RT360.dll";
          std::string map_out = dll_path.substr(0, dll_path.size() - 4) + ".map";

          // Locate clang++ in toolchain/
          std::string clang;
          {
#ifdef _WIN32
            // Helper: search base_dir\toolchain\llvm-mingw-*\bin\clang++.exe
            // Returns full path if found, empty string otherwise.
            auto find_clang_in = [](const std::string& base) -> std::string {
              std::string tc_dir = base + "\\toolchain";
              std::string glob = tc_dir + "\\llvm-mingw-*";
              WIN32_FIND_DATAA fd;
              HANDLE h = FindFirstFileA(glob.c_str(), &fd);
              if (h == INVALID_HANDLE_VALUE) return "";
              do {
                if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  std::string candidate = tc_dir + "\\" + fd.cFileName +
                                         "\\bin\\clang++.exe";
                  if (GetFileAttributesA(candidate.c_str()) !=
                      INVALID_FILE_ATTRIBUTES) {
                    FindClose(h);
                    return candidate;
                  }
                }
              } while (FindNextFileA(h, &fd));
              FindClose(h);
              return "";
            };

            // Get directory of current executable
            char exe_path[MAX_PATH] = {};
            GetModuleFileNameA(nullptr, exe_path, MAX_PATH);
            std::string exe_dir = exe_path;
            size_t sl = exe_dir.find_last_of("\\/");
            if (sl != std::string::npos) exe_dir = exe_dir.substr(0, sl);

            // Walk up from exe_dir looking for toolchain\ at each level.
            // Covers: <project>/build/Release/xexmagic.exe  (up 2)
            //         <project>/build/xexmagic.exe           (up 1)
            //         <project>/xexmagic.exe                 (up 0)
            std::string dir = exe_dir;
            for (int up = 0; up <= 3 && clang.empty(); up++) {
              clang = find_clang_in(dir);
              // Go one level up
              size_t sep = dir.find_last_of("\\/");
              if (sep == std::string::npos) break;
              dir = dir.substr(0, sep);
            }
            // Fallback: try CWD
            if (clang.empty()) {
              char cwd[MAX_PATH] = {};
              GetCurrentDirectoryA(MAX_PATH, cwd);
              clang = find_clang_in(std::string(cwd));
            }
#endif
          }

          if (clang.empty()) {
            fprintf(stderr, "RT360 compile: Could not find clang++ in toolchain/\n");
            fprintf(stderr, "  Expected: toolchain/llvm-mingw-*/bin/clang++.exe\n");
          } else {
            // Derive project src/ from clang path (4 levels up):
            // <project>\toolchain\llvm-mingw-*\bin\clang++.exe
            //   strip filename  → ...\bin
            //   strip bin       → ...\llvm-mingw-*
            //   strip llvm-*    → ...\toolchain
            //   strip toolchain → <project>
            //   + \src          → <project>\src
            std::string runtime_inc;
            {
              std::string p = clang;
              for (int up = 0; up < 4; up++) {
                size_t s = p.find_last_of("\\/");
                if (s == std::string::npos) break;
                p = p.substr(0, s);
              }
              runtime_inc = p + "\\src";
              // Sanity check
              std::string check = runtime_inc + "\\rt360_runtime.h";
              if (GetFileAttributesA(check.c_str()) == INVALID_FILE_ATTRIBUTES) {
                fprintf(stderr, "RT360 compile: rt360_runtime.h not found at %s\n",
                        runtime_inc.c_str());
              }
            }

            // Build command.
            // Don't wrap clang in quotes — it has no spaces, and cmd.exe's
            // system() mangles the command when it starts with a '"'.
            char cmd[4096];
            snprintf(cmd, sizeof(cmd),
                "%s -shared -O3 -static "
                "-target x86_64-w64-mingw32 -m64 "
                "-fseh-exceptions -funwind-tables "
                "-fstack-protector-strong -mstack-probe-size=4096 "
                "-Wno-c++23-extensions "
                "\"-Wl,--Map=%s\" "
                "-o \"%s\" \"%s\" \"-I%s\" -lntdll 2>&1",
                clang.c_str(), map_out.c_str(),
                dll_path.c_str(), cpp_path.c_str(), runtime_inc.c_str());

            printf("RT360 compile: %s\n", cmd);
            printf("Compiling RT360 DLL (this may take several minutes)...\n");
            fflush(stdout);

            int rc = system(cmd);
            if (rc == 0) {
              printf("RT360 DLL compiled: %s\n", dll_path.c_str());
              printf("RT360 MAP written: %s\n", map_out.c_str());
            } else {
              fprintf(stderr, "RT360 compile failed (exit code %d)\n", rc);
            }
          }
        }
      }
      break;
    }
  }

  return 0;
}
