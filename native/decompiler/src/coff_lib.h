// XEXMagic - COFF Archive (.lib) parser for Xbox 360 PPC libraries
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct CoffRelocation {
  uint32_t offset;       // Offset within section where relocation applies
  uint32_t symbol_index; // Index into symbol table
  uint16_t type;         // Relocation type
};

struct CoffSection {
  std::string name;
  uint32_t virtual_size;
  uint32_t virtual_address;
  uint32_t raw_size;
  uint32_t raw_offset;
  uint32_t characteristics;
  uint32_t reloc_offset;   // File offset of relocations
  uint16_t num_relocs;
  std::vector<uint8_t> data;
  std::vector<CoffRelocation> relocations;
};

struct CoffSymbol {
  std::string name;
  uint32_t value;         // Offset within section (for defined symbols)
  int16_t section_number; // 1-based section index, 0=external, -1=absolute
  uint16_t type;          // Complex/base type; 0x20 = DTYPE_FUNCTION
  uint8_t storage_class;
  uint8_t aux_count;

  // MSVC emits internal labels ($M77396, $LN12) with the same storage class as
  // real symbols. They are positions inside a function, not entry points.
  bool IsInternalLabel() const {
    return !name.empty() && (name[0] == '$' || name[0] == '.');
  }
  // COFF marks a symbol as a function by its derived type, which is the only
  // reliable way to tell an entry point from a label or a data object.
  bool IsFunction() const {
    return section_number > 0 && type == 0x20 && !name.empty() &&
           !IsInternalLabel();
  }
};

struct CoffObject {
  std::string member_name;
  std::vector<CoffSection> sections;
  std::vector<CoffSymbol> all_symbols;  // Full symbol table
  std::vector<std::string> symbols;     // External symbol names (for listing)

  // Get symbol name by index (resolves relocations)
  const std::string& GetSymbolName(uint32_t index) const {
    static const std::string empty;
    if (index < all_symbols.size()) return all_symbols[index].name;
    return empty;
  }

  // Build a map of (section_index, offset) -> symbol name for function labels
  std::map<std::pair<int, uint32_t>, std::string> BuildSectionSymbolMap() const {
    std::map<std::pair<int, uint32_t>, std::string> map;
    for (auto& sym : all_symbols) {
      if (sym.section_number > 0 && !sym.name.empty() &&
          sym.name[0] != '.' && sym.name.substr(0, 2) != "__") {
        // Skip compiler-internal symbols like .text, __C1, __C2
        auto key = std::make_pair(sym.section_number - 1, sym.value);
        map[key] = sym.name;
      }
    }
    return map;
  }
};

// A whole archive laid out into one flat address space so that the decompiler
// and the web UI can treat a .lib the same way they treat an image. Every
// section of every object gets its own address range - COFF sections all start
// at virtual address 0, so without this every object's functions collide and
// they all end up sharing one name.
struct CoffImage {
  uint32_t base = 0;
  std::vector<uint8_t> data;  // indexed by (address - base)

  struct Section {
    std::string name;  // "convert.obj:.text"
    uint32_t address = 0;
    uint32_t size = 0;
    bool code = false;
  };
  std::vector<Section> sections;

  std::map<uint32_t, std::string> symbols;      // address -> name (demangled)
  std::map<uint32_t, std::string> relocations;  // call site -> target name
  // Call site -> address of the definition the relocation names. In an
  // unlinked object every `bl` encodes 0, so without this the callgraph and
  // cross-references are empty and nothing in the UI is navigable.
  std::map<uint32_t, uint32_t> reloc_targets;
  std::vector<uint32_t> function_starts;        // sorted, code sections only

  // End of the function starting at `addr` - the next entry point in the same
  // section, or the end of that section.
  uint32_t FunctionEnd(uint32_t addr) const;
};

// Parse a COFF archive (.lib) file and extract all object members.
// Xbox 360 libs use big-endian PPC COFF (machine type 0x01F2).
bool ParseCoffLib(const uint8_t* data, size_t size,
                  std::vector<CoffObject>& objects);

// Lay the parsed objects out into one address space. Returns false if there is
// nothing with contents in the archive.
bool BuildCoffImage(const std::vector<CoffObject>& objects, CoffImage& out,
                    uint32_t base = 0x10000000);
