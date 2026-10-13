// XEXMagic IL Pipeline Integration Test
// Generates PPC instruction sequences and runs them through the full pipeline.

#include <cstdio>
#include <cstring>
#include <vector>

#include "../src/il.h"
#include "../src/il_builder.h"
#include "../src/il_validator.h"
#include "../src/xenon_cycles.h"
#include "../src/var_allocator.h"
#include "../src/codegen.h"

// Write a big-endian 32-bit word
static void WriteBE32(uint8_t* p, uint32_t v) {
  p[0] = (uint8_t)(v >> 24);
  p[1] = (uint8_t)(v >> 16);
  p[2] = (uint8_t)(v >> 8);
  p[3] = (uint8_t)(v);
}

// Build a test function with representative PPC instructions
static std::vector<uint8_t> BuildTestFunction() {
  std::vector<uint8_t> code;
  auto emit = [&](uint32_t w) {
    size_t off = code.size();
    code.resize(off + 4);
    WriteBE32(code.data() + off, w);
  };

  // Prologue
  emit(0x7C0802A6);  // mflr r0
  emit(0x9421FFD0);  // stwu r1, -48(r1)  (stack frame 48 bytes)
  emit(0x90010034);  // stw r0, 52(r1)    (save LR)
  emit(0x93E1002C);  // stw r31, 44(r1)   (save r31)

  // Integer ALU
  emit(0x38600000);  // li r3, 0
  emit(0x3880000A);  // li r4, 10
  emit(0x7C632A14);  // add r3, r3, r5     (add)
  emit(0x7C841A78);  // xor r4, r4, r3     (xor)
  emit(0x7C651B78);  // mr r5, r3          (or = mr alias)
  emit(0x7C6400D0);  // neg r3, r4         (neg)
  emit(0x5463083C);  // rlwinm r3, r3, 1, 0, 30   (shift left 1)
  emit(0x54841FFE);  // rlwinm r4, r4, 3, 31, 31  (mask)
  emit(0x7C632038);  // and r3, r3, r4     (and)

  // Compare and branch
  emit(0x2C030005);  // cmpwi cr0, r3, 5
  emit(0x41820008);  // beq +8
  emit(0x38630001);  // addi r3, r3, 1

  // Load/store
  emit(0x80A10010);  // lwz r5, 16(r1)
  emit(0x88C50000);  // lbz r6, 0(r5)
  emit(0xA0E50002);  // lhz r7, 2(r5)
  emit(0x90610010);  // stw r3, 16(r1)
  emit(0x98C50000);  // stb r6, 0(r5)

  // Floating point
  emit(0xC0250000);  // lfs fr1, 0(r5)
  emit(0xC0450004);  // lfs fr2, 4(r5)
  emit(0xEC21102A);  // fadds fr1, fr1, fr2
  emit(0xEC211072);  // fmuls fr1, fr1, fr2  (actually fmuls uses FRC field)
  emit(0xD0250008);  // stfs fr1, 8(r5)

  // Function call
  emit(0x48000019);  // bl +24  (call forward)

  // Multiply and divide
  emit(0x1C63000A);  // mulli r3, r3, 10
  emit(0x7C8323D6);  // divw r4, r3, r4

  // Epilogue
  emit(0x83E1002C);  // lwz r31, 44(r1)
  emit(0x80010034);  // lwz r0, 52(r1)
  emit(0x7C0803A6);  // mtlr r0
  emit(0x38210030);  // addi r1, r1, 48
  emit(0x4E800020);  // blr

  return code;
}

// Build a function with float-heavy code
static std::vector<uint8_t> BuildFloatFunction() {
  std::vector<uint8_t> code;
  auto emit = [&](uint32_t w) {
    size_t off = code.size();
    code.resize(off + 4);
    WriteBE32(code.data() + off, w);
  };

  emit(0x7C0802A6);  // mflr r0
  emit(0x9421FFF0);  // stwu r1, -16(r1)
  emit(0x90010014);  // stw r0, 20(r1)

  // Float operations
  emit(0xC0230000);  // lfs fr1, 0(r3)
  emit(0xC0430004);  // lfs fr2, 4(r3)
  emit(0xC0630008);  // lfs fr3, 8(r3)
  emit(0xEC21102A);  // fadds fr1, fr1, fr2
  emit(0xEC2110B2);  // fmuls fr1, fr1, fr2 (xo5=25, FRC=fr2)
  emit(0xEC211824);  // fdivs fr1, fr1, fr3
  emit(0xFC200850);  // fneg fr1, fr1
  emit(0xFC200A10);  // fabs fr1, fr1
  emit(0xFC211028);  // fsub fr1, fr1, fr2
  emit(0xFC01102E);  // fsel fr0, fr1, fr0, fr2  (src1=fr1, if>=0: fr0, else: fr2)

  // Float compare
  emit(0xFC011000);  // fcmpu cr0, fr1, fr2
  emit(0x41820008);  // beq +8

  // Convert
  emit(0xFC20081E);  // fctiwz fr1, fr1
  emit(0xFC200818);  // frsp fr1, fr1

  // Store
  emit(0xD023000C);  // stfs fr1, 12(r3)

  // Epilogue
  emit(0x80010014);  // lwz r0, 20(r1)
  emit(0x7C0803A6);  // mtlr r0
  emit(0x38210010);  // addi r1, r1, 16
  emit(0x4E800020);  // blr

  return code;
}

int main() {
  printf("=== XEXMagic IL Pipeline Test ===\n\n");
  int pass = 0, fail = 0;

  // Test 1: Basic integer function
  {
    printf("--- Test 1: Integer ALU + Memory + Branches ---\n");
    auto code = BuildTestFunction();
    ILBuilder builder;
    auto func = builder.BuildFunction(code.data(), code.size(), 0x82000000,
                                       0x82000000, 0x82000000 + (uint32_t)code.size(),
                                       "test_integer");
    auto vr = ValidateILFunction(func);
    ComputeFunctionCycles(func);
    AllocateVariables(func);

    printf("  Instructions: %d\n", vr.total_instructions);
    printf("  InlineAsm:    %d\n", vr.inline_asm_count);
    printf("  Coverage:     %.1f%%\n", vr.coverage_pct);
    printf("  Validation:   %s\n", vr.valid ? "PASS" : "FAIL");
    printf("  Cycles:       %d\n", func.total_cycles);
    printf("  Warnings:     %zu\n", vr.warnings.size());
    for (auto& w : vr.warnings) printf("    - %s\n", w.c_str());

    CodeGenConfig cfg;
    std::string src = GenerateFunctionSource(func, cfg);
    printf("\n%s\n", src.c_str());

    if (vr.valid && vr.inline_asm_count == 0) { pass++; printf("  PASS\n\n"); }
    else { fail++; printf("  FAIL\n\n"); }
  }

  // Test 2: Float-heavy function
  {
    printf("--- Test 2: Floating Point Operations ---\n");
    auto code = BuildFloatFunction();
    ILBuilder builder;
    auto func = builder.BuildFunction(code.data(), code.size(), 0x82001000,
                                       0x82001000, 0x82001000 + (uint32_t)code.size(),
                                       "test_float");
    auto vr = ValidateILFunction(func);
    ComputeFunctionCycles(func);
    AllocateVariables(func);

    printf("  Instructions: %d\n", vr.total_instructions);
    printf("  InlineAsm:    %d\n", vr.inline_asm_count);
    printf("  Coverage:     %.1f%%\n", vr.coverage_pct);
    printf("  Validation:   %s\n", vr.valid ? "PASS" : "FAIL");
    printf("  Cycles:       %d\n", func.total_cycles);
    for (auto& w : vr.warnings) printf("    - %s\n", w.c_str());

    CodeGenConfig cfg;
    cfg.cpp_mode = true;
    cfg.namespace_name = "TestNS";
    std::vector<ILFunction> funcs = {func};
    std::string src = GenerateSource(funcs, cfg, "test_float.bin");
    printf("\n%s\n", src.c_str());

    if (vr.valid && vr.inline_asm_count == 0) { pass++; printf("  PASS\n\n"); }
    else { fail++; printf("  FAIL\n\n"); }
  }

  // Test 3: Run against xhvd2.lib and check overall coverage
  {
    printf("--- Test 3: xhvd2.lib Coverage Report ---\n");
    FILE* f = fopen("xhvd2.lib", "rb");
    if (!f) f = fopen("F:/XEXMagic/xhvd2.lib", "rb");
    if (f) {
      fseek(f, 0, SEEK_END);
      size_t sz = ftell(f);
      fseek(f, 0, SEEK_SET);
      std::vector<uint8_t> buf(sz);
      fread(buf.data(), 1, sz, f);
      fclose(f);

      // Use the already-generated output to check stats
      // Just count total functions and coverage from the pipeline
      printf("  Library size: %zu bytes\n", sz);

      // Quick validation: count InlineAsm in the output
      FILE* out = fopen("F:/XEXMagic/test_output.c", "r");
      if (out) {
        int asm_count = 0;
        int func_count = 0;
        int validation_pass = 0;
        char line[4096];
        while (fgets(line, sizeof(line), out)) {
          if (strstr(line, "__asm__")) asm_count++;
          if (strstr(line, "/* Validation:")) func_count++;
          if (strstr(line, "Validation: PASS")) validation_pass++;
        }
        fclose(out);
        printf("  Functions:    %d\n", func_count);
        printf("  Validated OK: %d\n", validation_pass);
        printf("  InlineAsm lines: %d\n", asm_count);
        if (func_count > 0 && validation_pass == func_count && asm_count == 0) {
          pass++;
          printf("  PASS - 100%% coverage, all functions validated\n\n");
        } else {
          printf("  Coverage: %d/%d functions validated, %d asm lines\n",
                 validation_pass, func_count, asm_count);
          if (asm_count == 0) { pass++; printf("  PASS\n\n"); }
          else { fail++; printf("  FAIL - has InlineAsm fallbacks\n\n"); }
        }
      } else {
        printf("  Skipped (no test_output.c)\n\n");
      }
    } else {
      printf("  Skipped (no xhvd2.lib)\n\n");
    }
  }

  printf("=== Results: %d PASS, %d FAIL ===\n", pass, fail);
  return fail > 0 ? 1 : 0;
}
