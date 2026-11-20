#pragma once

#include <cstdint>
#include <string>
#include <vector>

// The user's console keyvault (decrypted, 0x4000 bytes), from console_settings.ini: console_keyvault (a decrypted
// KV.bin), or console_flash + console_cpu_key (the console's flash dump and its CPU key: the keyvault decrypted from
// the flash in memory). Read once; its bytes are never written, copied to a file or logged. Empty when none is set.
namespace kernel
{
    const std::vector<uint8_t>& ConsoleKeyvault();
    // Where it came from, for messages ("console_keyvault <path>" / "console_flash <path>"); empty when none.
    const std::string& ConsoleKeyvaultSource();
}
