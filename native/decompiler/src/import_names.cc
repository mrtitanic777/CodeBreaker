#include "import_names.h"

#include <cstdio>
#include <cstring>

#include "xex_loader.h"

namespace {

struct OrdinalEntry {
  uint32_t ordinal;
  const char* name;
};

// Baked copies of Nexia's export tables, expanded through X-macros so the
// generated .inc stays a plain list of facts.
#define KERNEL_MODULE_BEGIN(mod) static const OrdinalEntry k##mod##Ordinals[] = {
#define KERNEL_ORDINAL(ord, name) {ord, name},
#define KERNEL_MODULE_END(mod) \
  }                            \
  ;
#include "kernel_ordinals.inc"
#undef KERNEL_MODULE_BEGIN
#undef KERNEL_ORDINAL
#undef KERNEL_MODULE_END

struct ModuleTable {
  const char* module;
  const OrdinalEntry* entries;
  size_t count;
};

const ModuleTable kModules[] = {
    {"xam", kxamOrdinals, sizeof(kxamOrdinals) / sizeof(kxamOrdinals[0])},
    {"xboxkrnl", kxboxkrnlOrdinals,
     sizeof(kxboxkrnlOrdinals) / sizeof(kxboxkrnlOrdinals[0])},
};

// The library name in the XEX is a filename ("xam.xex", "xboxkrnl.exe") and
// casing is not guaranteed, so match on the stem case-insensitively rather
// than comparing the whole string.
const ModuleTable* MatchModule(const std::string& library_name) {
  std::string stem = library_name;
  size_t dot = stem.find('.');
  if (dot != std::string::npos) stem = stem.substr(0, dot);

  for (auto& c : stem) {
    if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
  }

  for (const auto& m : kModules) {
    if (stem == m.module) return &m;
  }
  return nullptr;
}

}  // namespace

const char* LookupKernelOrdinal(const char* module, uint32_t ordinal) {
  for (const auto& m : kModules) {
    if (std::strcmp(m.module, module) != 0) continue;
    // Generated in ascending ordinal order, but a linear scan is fine - this
    // runs once per import, not per instruction.
    for (size_t i = 0; i < m.count; i++) {
      if (m.entries[i].ordinal == ordinal) return m.entries[i].name;
    }
    return nullptr;
  }
  return nullptr;
}

const std::vector<KernelExport>& AllKernelExports() {
  static const std::vector<KernelExport> all = [] {
    std::vector<KernelExport> out;
    for (const auto& m : kModules)
      for (size_t i = 0; i < m.count; i++)
        out.push_back({m.module, m.entries[i].ordinal, m.entries[i].name});
    return out;
  }();
  return all;
}

void ImportNameTable::Build(const std::vector<ImportLibrary>& libraries) {
  for (const auto& lib : libraries) {
    const ModuleTable* mod = MatchModule(lib.name);

    for (const auto& imp : lib.imports) {
      std::string name;
      const char* known =
          mod ? LookupKernelOrdinal(mod->module, imp.ordinal) : nullptr;

      if (known) {
        name = known;
      } else {
        // Still worth naming: "which import" is most of the value even when
        // the ordinal is not in the table.
        char buf[128];
        std::snprintf(buf, sizeof(buf), "%s_ordinal_%u",
                      mod ? mod->module : lib.name.c_str(), imp.ordinal);
        name = buf;
      }

      if (imp.thunk_address) thunks_[imp.thunk_address] = name;
      if (imp.value_address) values_[imp.value_address] = name;
    }
  }
}

const char* ImportNameTable::LookupThunk(uint32_t address) const {
  auto it = thunks_.find(address);
  return it == thunks_.end() ? nullptr : it->second.c_str();
}

const char* ImportNameTable::LookupValue(uint32_t address) const {
  auto it = values_.find(address);
  return it == values_.end() ? nullptr : it->second.c_str();
}

const char* ImportNameTable::Lookup(uint32_t address) const {
  if (const char* n = LookupThunk(address)) return n;
  return LookupValue(address);
}
