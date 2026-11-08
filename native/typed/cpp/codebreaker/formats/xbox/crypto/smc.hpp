// codebreaker/formats/xbox/crypto/smc.hpp -- typed C++ from codebreaker.formats.xbox.crypto.smc.
//
// Xbox 360 SMC (System Management Controller) -- decrypt / encrypt / extract /
// config parse.
//
// The SMC is the little always-on microcontroller that runs fans, power, front
// LEDs, temperature and the eject button. Its firmware lives in NAND, obfuscated
// with the well-known "BuNy" rolling cipher; RGH/JTAG hacks patch the decrypted
// SMC. This completes the NAND console layer (nand / bootchain / fuses / cpukey /
// keyvault / hypervisor) with the one blob those didn't touch.
//
// Flash-header fields (big-endian, documented layout):
//   0x74 u32 SMC config offset      0x78 u32 SMC boot size      0x7C u32 SMC boot offset
//
// Commands:
//   smc decrypt <in> <out>          de-obfuscate an SMC image
//   smc encrypt <in> <out>          re-obfuscate (inverse; round-trips)
//   smc info    <smc>               size, checksum, motherboard guess
//   smc extract <nand> <out>        pull the SMC firmware out of a NAND dump
//   smc config  <nand|cfg>          parse the SMC config (fan override, temps)
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::smc {

extern const std::tuple<int64_t, int64_t, int64_t, int64_t> _KEY;   // "BuNy"

// motherboard byte (SMC version[1]) -> console revision
extern const py::dict<int64_t, py::str> _MOBO;

// The config record parse_config returns (a dict of fixed keys in Python; None where the data is short).
struct Config {
    std::optional<int64_t> checksum;
    // fan-speed override: 0 = auto; else a percent-ish 0..0x7F with 0x80 set
    std::optional<int64_t> fan_override_cpu;
    std::optional<int64_t> fan_override_gpu;
    // temperature targets (raw SMC units) at the documented config offsets
    std::optional<int64_t> temp_cpu_target;
    std::optional<int64_t> temp_gpu_target;
    std::optional<int64_t> temp_ram_target;
    int64_t raw_len = 0;
};
template <class F> void for_each_field_mut(Config& c, F&& f) {
    f("checksum", c.checksum, true);
    f("fan_override_cpu", c.fan_override_cpu, true);
    f("fan_override_gpu", c.fan_override_gpu, true);
    f("temp_cpu_target", c.temp_cpu_target, true);
    f("temp_gpu_target", c.temp_gpu_target, true);
    f("temp_ram_target", c.temp_ram_target, true);
    f("raw_len", c.raw_len, true);
}

py::bytes smc_decrypt(const py::bytes& data);
py::bytes smc_encrypt(const py::bytes& data);

// The 4-byte SMC version lives near the tail of the decrypted image
// (...marked by the 0x10-terminated record). Returns (major, mobo, build, rev)
// or None if not located.
std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>> smc_version(const py::bytes& dec);

// Return (smc_bytes, offset, length) from a NAND dump via the flash header.
std::tuple<py::bytes, int64_t, int64_t> extract_from_nand(const py::bytes& nand);

// SMC config = a checksummed record of fan/temperature/power overrides. If
// given a whole NAND, pull the config via the header (0x74). Returns a dict.
Config parse_config(const py::bytes& nand_or_cfg);

extern const py::str DESCRIPTION;
extern const py::str __doc__;

// main(): the command line without the program name (sys.argv[1:]); prints to stdout.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::smc
