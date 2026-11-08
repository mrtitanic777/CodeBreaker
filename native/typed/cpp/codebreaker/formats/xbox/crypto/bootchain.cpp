// codebreaker/formats/xbox/crypto/bootchain.cpp -- typed C++ from codebreaker.formats.xbox.crypto.bootchain.

#include "codebreaker/formats/xbox/crypto/bootchain.hpp"

#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::bootchain {

const py::dict<int64_t, py::str> BL_NAMES = {
    {BL_1BL, py::str("1BL")},       {BL_CB, py::str("CB/2BL")}, {BL_SC, py::str("SC/3BL")},
    {BL_CD, py::str("CD/4BL")},     {BL_CE, py::str("CE/5BL")}, {BL_CF, py::str("CF/6BL")},
    {BL_CG, py::str("CG/7BL")},     {BL_HV, py::str("HV")},
};

const py::dict<int64_t, int64_t> MAGIC_MAP = {
    {MAGIC_CB, BL_CB}, {MAGIC_CD, BL_CD}, {MAGIC_CE, BL_CE},
    {MAGIC_CF, BL_CF}, {MAGIC_CG, BL_CG}, {MAGIC_HV, BL_HV},
};

// -- Parsing --

std::optional<BootloaderHeader> parse_header(const py::bytes& data, int64_t offset) {
    if (offset + 16 > py::len(data)) return std::nullopt;

    BootloaderHeader h;
    h.magic = py::unpack_from1<int64_t>(">H", data, offset);
    h.version = py::unpack_from1<int64_t>(">H", data, offset + 2);
    h.pairing = py::unpack_from1<int64_t>(">H", data, offset + 4);
    h.flags = py::unpack_from1<int64_t>(">H", data, offset + 6);
    h.entrypoint = py::unpack_from1<int64_t>(">I", data, offset + 8);
    h.size = py::unpack_from1<int64_t>(">I", data, offset + 12);
    h.offset_in_nand = offset;

    h.bl_type = MAGIC_MAP.get(h.magic, -1);
    h.bl_name = BL_NAMES.get(h.bl_type, py::percent(py::str("Unknown(0x%04X)"), h.magic));
    h.is_devkit = (h.pairing & 0x8000) != 0;
    h.is_manufacturing = (h.flags & 0x0001) != 0;
    h.is_cba = (h.flags & 0x0800) != 0;

    // Key is at offset +16 for CB, CD, CE, CF, CG (16 bytes)
    if ((h.bl_type == BL_CB || h.bl_type == BL_CD || h.bl_type == BL_CE || h.bl_type == BL_CF ||
         h.bl_type == BL_CG) &&
        offset + 32 <= py::len(data)) {
        h.key = data.slice(offset + 16, offset + 32);
    }

    return h;
}

bool _is_valid_header(const BootloaderHeader& h) {
    if (h.bl_type == -1) return false;
    if (h.size == 0 || h.size > 0x1000000) return false;   // Max 16MB
    if (h.version == 0 || h.version > 0xFFFF) return false;
    return true;
}

// -- NAND scanning --

const py::dict<py::str, int64_t> NAND_OFFSETS = {
    {py::str("header"), 0x0},
    {py::str("smc"), 0x1000},
    {py::str("kv"), 0x4000},
    {py::str("cb"), 0x8000},
    // CB_B follows CB (variable size)
    // CD follows CB_B (variable size)
    // CE/kernel at 0x70000 or 0x73000 depending on NAND type
    // CF+CG = xboxupd.bin sections
};

BootchainInfo scan_nand(const py::bytes& data) {
    BootchainInfo info;
    info.nand_size = py::len(data);
    py::set<int64_t> found_offsets;

    // Check standard CB offset
    int64_t cb_offset = 0x8000;
    std::optional<BootloaderHeader> h = parse_header(data, cb_offset);
    if (h && _is_valid_header(*h)) {
        info.stages.append(*h);
        found_offsets.add(cb_offset);
        info.cb_version = h->version;

        // CB_B follows CB
        int64_t cb_b_offset = cb_offset + h->size;
        std::optional<BootloaderHeader> h2 = parse_header(data, cb_b_offset);
        if (h2 && _is_valid_header(*h2) && h2->bl_type == BL_CB) {
            h2->bl_name = py::str("CB_B/2BL_B");
            info.stages.append(*h2);
            found_offsets.add(cb_b_offset);

            // CD follows CB_B
            int64_t cd_offset = cb_b_offset + h2->size;
            std::optional<BootloaderHeader> h3 = parse_header(data, cd_offset);
            if (h3 && _is_valid_header(*h3) && h3->bl_type == BL_CD) {
                info.stages.append(*h3);
                found_offsets.add(cd_offset);
                info.cd_version = h3->version;
            }
        }
    }

    // Scan for CE (kernel) at common offsets
    for (int64_t ce_off : {0x70000, 0x73000, 0x95000}) {
        if (found_offsets.contains(ce_off) || ce_off >= py::len(data)) continue;
        h = parse_header(data, ce_off);
        if (h && _is_valid_header(*h) && h->bl_type == BL_CE) {
            info.stages.append(*h);
            found_offsets.add(ce_off);
            info.ce_version = h->version;
            break;
        }
    }

    // Brute-force scan for any remaining stages
    for (int64_t offset = 0; offset < py::min(py::len(data), int64_t(0x200000)); offset += 0x10) {
        if (found_offsets.contains(offset)) continue;
        if (offset + 16 > py::len(data)) break;
        int64_t magic = py::unpack_from1<int64_t>(">H", data, offset);
        if (MAGIC_MAP.contains(magic)) {
            h = parse_header(data, offset);
            if (h && _is_valid_header(*h) && !found_offsets.contains(offset)) {
                // Avoid duplicates (same version at different offset)
                bool is_dup = false;
                for (const BootloaderHeader& s : info.stages) {
                    if (s.version == h->version && s.bl_type == h->bl_type) {
                        is_dup = true;
                        break;
                    }
                }
                if (!is_dup) {
                    info.stages.append(*h);
                    found_offsets.add(offset);
                }
            }
        }
    }

    // Sort by offset
    info.stages.sort([](const BootloaderHeader& s) { return s.offset_in_nand; });
    return info;
}

py::bytes extract_stage(const py::bytes& data, const BootloaderHeader& stage) {
    int64_t start = stage.offset_in_nand;
    int64_t end = start + stage.size;
    if (end > py::len(data)) end = py::len(data);
    return data.slice(start, end);
}

py::str format_bootchain(const BootchainInfo& info) {
    py::list<py::str> lines;
    lines.append(py::str("NAND Bootchain Analysis (") + py::format(info.nand_size, ",") + py::str(" bytes)"));
    lines.append(py::str(""));

    if (info.stages.empty()) {
        lines.append(py::str("  No bootloader stages found."));
        return py::join(py::str("\n"), lines);
    }

    for (const BootloaderHeader& s : info.stages) {
        py::list<py::str> flags;
        if (s.is_devkit) flags.append(py::str("DEVKIT"));
        if (s.is_manufacturing) flags.append(py::str("MFG"));
        if (s.is_cba) flags.append(py::str("CB_A"));
        py::str flag_str = !flags.empty() ? py::str("  [") + py::join(py::str(", "), flags) + py::str("]") : py::str("");

        lines.append(py::str("  ") + py::format(s.bl_name, "<12") + py::str(" v") + py::format(s.version, "<6") +
                     py::str(" @ 0x") + py::format(s.offset_in_nand, "06X") + py::str("  size=0x") +
                     py::format(s.size, "06X") + py::str("  entry=0x") + py::format(s.entrypoint, "08X") +
                     flag_str);

        if (!s.key.empty() && !(s.key == py::bytes::zeros(16))) {
            lines.append(py::str("    ") + py::format(py::str("Key:"), "<12") + py::str(" ") + s.key.hex().upper());
        }
    }

    return py::join(py::str("\n"), lines);
}

const py::str DESCRIPTION("parse the Xbox 360 boot chain (CB/CD/CE/CF/CG stages) from a NAND dump");

void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        py::print(py::str("usage: bootchain <nand-dump> [--extract STAGE out.bin]"));
        return;
    }
    py::bytes data = py::open_rb(a[0]).read();
    BootchainInfo info = scan_nand(data);
    py::print(format_bootchain(info));
}

}  // namespace codebreaker::formats::xbox::crypto::bootchain
