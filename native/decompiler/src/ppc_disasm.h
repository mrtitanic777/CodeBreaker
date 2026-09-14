// XEXMagic - PPC Disassembler
// Standalone PowerPC disassembler for Xbox 360 executables

#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct DisasmLine {
  uint32_t address;
  uint32_t code;
  std::string text;
};

// Disassemble a single 32-bit big-endian PPC instruction at the given address.
// Returns a string like "add     r3, r4, r5" or "lwz     r11, 0x1B44(r3)".
std::string DisasmInstruction(uint32_t address, uint32_t code);

// Disassemble a buffer of big-endian PPC instructions.
// data: pointer to raw bytes, len: byte count, base_addr: virtual address of first instruction.
std::vector<DisasmLine> DisasmBuffer(const uint8_t* data, size_t len,
                                     uint32_t base_addr);
