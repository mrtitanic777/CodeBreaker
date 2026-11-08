// codebreaker/formats/xbox/crypto/smc.cpp -- typed C++ from codebreaker.formats.xbox.crypto.smc.

#include "codebreaker/formats/xbox/crypto/smc.hpp"

#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::smc {

const std::tuple<int64_t, int64_t, int64_t, int64_t> _KEY{0x42, 0x75, 0x4E, 0x79};   // "BuNy"

const py::dict<int64_t, py::str> _MOBO = {
    {0x00, py::str("Xenon")}, {0x01, py::str("Zephyr")}, {0x02, py::str("Falcon/Opus")}, {0x03, py::str("Jasper")},
    {0x04, py::str("Trinity/Corona?")}, {0x05, py::str("Corona")}, {0x06, py::str("Winchester")},
};

const py::str DESCRIPTION("Xbox 360 SMC: decrypt/encrypt/info/extract/config (fans, temps, power)");

// (the module's docstring, as __doc__ gives it to main's usage text)
const py::str __doc__(
    "Xbox 360 SMC (System Management Controller) \xE2\x80\x94 decrypt / encrypt / extract /\n"
    "config parse.\n"
    "\n"
    "The SMC is the little always-on microcontroller that runs fans, power, front\n"
    "LEDs, temperature and the eject button. Its firmware lives in NAND, obfuscated\n"
    "with the well-known \"BuNy\" rolling cipher; RGH/JTAG hacks patch the decrypted\n"
    "SMC. This completes the NAND console layer (nand / bootchain / fuses / cpukey /\n"
    "keyvault / hypervisor) with the one blob those didn't touch.\n"
    "\n"
    "Flash-header fields (big-endian, documented layout):\n"
    "  0x74 u32 SMC config offset      0x78 u32 SMC boot size      0x7C u32 SMC boot offset\n"
    "\n"
    "Commands:\n"
    "  smc decrypt <in> <out>          de-obfuscate an SMC image\n"
    "  smc encrypt <in> <out>          re-obfuscate (inverse; round-trips)\n"
    "  smc info    <smc>               size, checksum, motherboard guess\n"
    "  smc extract <nand> <out>        pull the SMC firmware out of a NAND dump\n"
    "  smc config  <nand|cfg>          parse the SMC config (fan override, temps)\n");

py::bytes smc_decrypt(const py::bytes& data) {
    py::list<int64_t> key{std::get<0>(_KEY), std::get<1>(_KEY), std::get<2>(_KEY), std::get<3>(_KEY)};
    py::bytearray out = py::bytearray::zeros(py::len(data));
    for (int64_t i = 0; i < py::len(data); ++i) {
        int64_t j = data.byte_(i);
        int64_t mod = (j * 0xFB) & 0xFFFFFFFF;
        out.raw_mut()[static_cast<size_t>(i)] = static_cast<char>(j ^ (key[i & 3] & 0xFF));
        key[(i + 1) & 3] = (key[(i + 1) & 3] + mod) & 0xFFFFFFFF;
        key[(i + 2) & 3] = (key[(i + 2) & 3] + (mod >> 8)) & 0xFFFFFFFF;
    }
    return py::bytes(out.raw());
}

py::bytes smc_encrypt(const py::bytes& data) {
    py::list<int64_t> key{std::get<0>(_KEY), std::get<1>(_KEY), std::get<2>(_KEY), std::get<3>(_KEY)};
    py::bytearray out = py::bytearray::zeros(py::len(data));
    for (int64_t i = 0; i < py::len(data); ++i) {
        int64_t j = data.byte_(i) ^ (key[i & 3] & 0xFF);
        int64_t mod = (j * 0xFB) & 0xFFFFFFFF;
        out.raw_mut()[static_cast<size_t>(i)] = static_cast<char>(j);
        key[(i + 1) & 3] = (key[(i + 1) & 3] + mod) & 0xFFFFFFFF;
        key[(i + 2) & 3] = (key[(i + 2) & 3] + (mod >> 8)) & 0xFFFFFFFF;
    }
    return py::bytes(out.raw());
}

std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>> smc_version(const py::bytes& dec) {
    // the version record is 4 bytes whose first byte is a small length/tag;
    // scan the last 0x400 bytes for the canonical 4-byte version block.
    py::bytes tail = py::len(dec) > 0x400 ? dec.slice_from(-0x400) : dec;
    for (int64_t o = 0; o < py::len(tail) - 4; ++o) {
        py::bytes b = tail.slice(o, o + 4);
        // heuristic: version bytes are all < 0x40 and mobo byte is known
        if (b.byte_(0) && b.byte_(0) < 0x10 && _MOBO.contains(b.byte_(1)) && b.byte_(2) < 0x40)
            return std::make_tuple(b.byte_(0), b.byte_(1), b.byte_(2), b.byte_(3));
    }
    return std::nullopt;
}

std::tuple<py::bytes, int64_t, int64_t> extract_from_nand(const py::bytes& nand) {
    if (py::len(nand) < 0x80) throw py::ValueError("not a NAND image");
    int64_t length = py::unpack_from1<int64_t>(">I", nand, 0x78);
    int64_t offset = py::unpack_from1<int64_t>(">I", nand, 0x7C);
    if (!(0 < length && length <= 0x4000) || offset + length > py::len(nand))
        throw py::ValueError(py::percent(py::str("SMC boot fields invalid (off=0x%X len=0x%X)"), offset, length));
    return {nand.slice(offset, offset + length), offset, length};
}

Config parse_config(const py::bytes& nand_or_cfg) {
    py::bytes cfg = nand_or_cfg;
    if (py::len(nand_or_cfg) >= 0x80 && nand_or_cfg.slice(0, 2) == py::bytes("\xff\x4f", 2)) {
        int64_t off = py::unpack_from1<int64_t>(">I", nand_or_cfg, 0x74);
        cfg = nand_or_cfg.slice(off, off + 0x2A0);
    }
    Config d;
    if (py::len(cfg) >= 4) d.checksum = py::unpack_from1<int64_t>(">I", cfg, 0x00);
    // fan-speed override: 0 = auto; else a percent-ish 0..0x7F with 0x80 set
    if (py::len(cfg) > 0x08) d.fan_override_cpu = cfg.byte_(0x08);
    if (py::len(cfg) > 0x09) d.fan_override_gpu = cfg.byte_(0x09);
    // temperature targets (raw SMC units) at the documented config offsets
    if (py::len(cfg) > 0x10) d.temp_cpu_target = cfg.byte_(0x10);
    if (py::len(cfg) > 0x11) d.temp_gpu_target = cfg.byte_(0x11);
    if (py::len(cfg) > 0x12) d.temp_ram_target = cfg.byte_(0x12);
    d.raw_len = py::len(cfg);
    return d;
}

void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        // __doc__.split("Commands:")[1].rstrip()
        py::print(std::get<2>(__doc__.partition(py::str("Commands:"))).rstrip());
        return;
    }
    py::str op = a[0];
    if (op == py::str("decrypt") || op == py::str("encrypt")) {
        py::bytes data = py::open_rb(a[1]).read();
        py::bytes out = op == py::str("decrypt") ? smc_decrypt(data) : smc_encrypt(data);
        py::open(a[2], py::str("wb")).write(out);
        py::print(py::percent(py::str("%sed %d bytes -> %s"), op, py::len(out), a[2]));
        return;
    }
    if (op == py::str("extract")) {
        py::bytes nand = py::open_rb(a[1]).read();
        auto [smc_bytes, off, ln] = extract_from_nand(nand);
        py::open(a[2], py::str("wb")).write(smc_bytes);
        py::print(py::percent(py::str("SMC @0x%X len 0x%X -> %s"), off, ln, a[2]));
        return;
    }
    if (op == py::str("info")) {
        py::bytes data = py::open_rb(a[1]).read();
        py::bytes dec = smc_decrypt(data);
        auto ver = smc_version(dec);
        py::print(py::percent(py::str("size        : %d bytes"), py::len(data)));
        int64_t total = 0;
        for (int64_t b : dec) total += b;
        py::print(py::percent(py::str("checksum(dec): 0x%08X"), total & 0xFFFFFFFF));
        if (ver) {
            py::print(py::percent(py::str("version     : %d.%d.%d.%d  (motherboard: %s)"), std::get<0>(*ver),
                                  std::get<1>(*ver), std::get<2>(*ver), std::get<3>(*ver),
                                  _MOBO.get(std::get<1>(*ver), py::str("?"))));
        } else {
            py::print(py::str("version     : (not located)"));
        }
        return;
    }
    if (op == py::str("config")) {
        Config d = parse_config(py::open_rb(a[1]).read());
        py::print(py::percent(py::str("SMC config (%d bytes)  checksum=0x%08X"), d.raw_len,
                              d.checksum.value_or(0)));   // d["checksum"] or 0
        py::print(py::percent(py::str("  fan override  cpu=%s gpu=%s"), d.fan_override_cpu, d.fan_override_gpu));
        py::print(py::percent(py::str("  temp targets  cpu=%s gpu=%s ram=%s"), d.temp_cpu_target, d.temp_gpu_target,
                              d.temp_ram_target));
        return;
    }
    py::print(py::percent(py::str("unknown op: %s"), op));
}

}  // namespace codebreaker::formats::xbox::crypto::smc
