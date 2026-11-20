/**
 * @file        rex/runtime/native_image.h
 * @brief       Native pre-baked image descriptor + module wrapper
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 *
 * @remarks     Describes a pre-decrypted guest image + baked metadata so the
 *              runtime can boot without parsing a XEX at runtime. The data here
 *              is exactly what the XEX loader produces (image bytes, variable
 *              import slots, page protections, entry point), extracted once by
 *              Runtime::DumpNativeImagePackage and replayed by
 *              Runtime::LoadNativeImage.
 */

#pragma once

#include <cstdint>
#include <string>

#include <rex/system/module.h>

namespace rex::runtime {

// One variable-import slot to patch in guest memory (data pointers the
// recompiled code reads via memory loads). Mirrors up_native::VarImport.
struct NativeImageImport {
  uint32_t slot;
  const char* lib;
  uint16_t ordinal;
};

// One page-protection range. Mirrors up_native::ProtRange.
struct NativeImageProtection {
  uint32_t addr;
  uint32_t size;
  uint8_t writable;
};

// Everything the native loader needs to replay the XEX loader's output.
struct NativeImageDesc {
  uint32_t base;
  uint32_t image_size;
  uint32_t entry_point;
  const void* image;
  uint32_t image_bytes;
  // Plaintext XEX header, mapped into guest memory by the native loader so the
  // guest's RtlImageXexHeaderField + GetOptHeader work.
  const void* xex_header;
  uint32_t xex_header_size;
  uint32_t full_image_size;
  const NativeImageImport* imports;
  uint32_t import_count;
  const NativeImageProtection* protections;
  uint32_t protection_count;
};

// Minimal Module implementation for a pre-baked native image. Plays the same
// role as XexModule/ElfModule as the processor_module_ behind a UserModule, but
// carries no XEX/ELF parsing state -- just the base/size/entry it was given.
class NativeImageModule : public Module {
 public:
  NativeImageModule(FunctionDispatcher* function_dispatcher, uint32_t base, uint32_t image_size,
                    uint32_t entry_point)
      : Module(function_dispatcher),
        name_("default.xex"),
        base_address_(base),
        image_size_(image_size),
        entry_point_(entry_point) {}

  const std::string& name() const override { return name_; }
  bool is_executable() const override { return true; }

  uint32_t base_address() const override { return base_address_; }
  uint32_t image_size() const override { return image_size_; }
  uint32_t entry_point() const override { return entry_point_; }

 private:
  std::string name_;
  uint32_t base_address_ = 0;
  uint32_t image_size_ = 0;
  uint32_t entry_point_ = 0;
};

}  // namespace rex::runtime
