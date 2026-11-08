// codebreaker/formats/xbox/crypto/nand.cpp -- C++ port of codebreaker.formats.xbox.crypto.nand.

#include "codebreaker/formats/xbox/crypto/nand.hpp"

namespace codebreaker::formats::xbox::crypto::nand {

const int64_t SMALL_PAGE_SIZE = 0x200;
const int64_t SMALL_SPARE_SIZE = 0x10;
const int64_t BIG_PAGE_SIZE = 0x800;
const int64_t BIG_SPARE_SIZE = 0x10;

const py::bytes MAGIC_NAND_HEADER = py::bytes::from_list(py::list<int64_t>{0xFF, 0x4F});

const int64_t HV_SIZE = 0x40000;
const int64_t KERNEL_OFFSET_IN_UPDATE = 0x40000;

py::str header_repr(const HeaderInfo& info) {
    py::str out = "{";
    bool first = true;
    auto put = [&](const char* key, const py::str& value) {
        if (!first) out += ", ";
        first = false;
        out += py::str("'") + py::str(key) + py::str("': ") + value;
    };
    if (info.magic) put("magic", py::repr(*info.magic));
    if (info.build) put("build", py::to_str(*info.build));
    if (info.qfe) put("qfe", py::to_str(*info.qfe));
    if (info.flags) put("flags", py::to_str(*info.flags));
    if (info.entry_point) put("entry_point", py::to_str(*info.entry_point));
    if (info.size) put("size", py::to_str(*info.size));
    return out + py::str("}");
}

// Detect NAND type from dump size.
py::str detect_nand_type(const py::bytes& data) {
    int64_t size = py::len(data);
    if (size == 16 * 1024 * 1024) {
        return "16MB (small block)";
    } else if (size == 64 * 1024 * 1024) {
        return "64MB (big block / jasper)";
    } else if (size == 256 * 1024 * 1024) {
        return "256MB (corona MMC)";
    } else if (size == 512 * 1024 * 1024) {
        return "512MB (corona 4GB MMC)";
    } else {
        return py::percent("Unknown (%d bytes)", size);
    }
}

// Extract basic NAND header information.
HeaderInfo extract_header_info(const py::bytes& data) {
    HeaderInfo info;
    if (py::len(data) < 0x80) {
        return info;
    }

    // Magic
    info.magic = data.slice(0, 2).hex().upper();

    // Build/version info
    if (py::len(data) >= 0x10) {
        info.build = py::unpack_from1<int64_t>(">H", data, 0x02);
        info.qfe = py::unpack_from1<int64_t>(">H", data, 0x04);
        info.flags = py::unpack_from1<int64_t>(">H", data, 0x06);
        info.entry_point = py::unpack_from1<int64_t>(">I", data, 0x08);
        info.size = py::unpack_from1<int64_t>(">I", data, 0x0C);
    }

    return info;
}

// Extract the hypervisor (first 256KB) from a NAND dump.
py::bytes extract_hv_from_nand(const py::bytes& data) {
    if (py::len(data) < HV_SIZE) {
        throw py::ValueError(py::percent("NAND dump too small (%d bytes, need %d)", py::len(data), HV_SIZE).utf8());
    }
    return data.slice_to(HV_SIZE);
}

// Extract HV and kernel from a system update file (xboxupd.bin applied kernel).
std::tuple<py::bytes, py::bytes> extract_hv_from_update(const py::bytes& update_data) {
    if (py::len(update_data) < HV_SIZE + 0x1000) {
        throw py::ValueError("Update file too small");
    }
    py::bytes hv = update_data.slice_to(HV_SIZE);
    py::bytes kernel = update_data.slice_from(HV_SIZE);
    return std::make_tuple(hv, kernel);
}

// Find the CB (2BL) bootloader offset in NAND.
int64_t get_cb_offset(const py::bytes& data) {
    if (py::len(data) < 0x40) {
        return -1;
    }
    // CB offset is typically at 0x08 in the NAND header
    int64_t offset = py::unpack_from1<int64_t>(">I", data, 0x08);
    if (offset > 0 && offset < py::len(data)) {
        return offset;
    }
    return -1;
}

// Get comprehensive NAND dump info.
NandInfo nand_info(const py::str& path) {
    // Path("") is the current folder "." (so reading it is a PermissionError, not FileNotFoundError)
    py::file f = py::open_rb(path.empty() ? py::str(".") : path);
    py::bytes data = f.read();
    f.close();
    NandInfo info;
    info.path = path;
    info.size = py::len(data);
    info.type = detect_nand_type(data);
    info.header = extract_header_info(data);
    return info;
}

const py::str DESCRIPTION = "inspect an Xbox 360 NAND dump (type, header, CB offset, HV extract)";

int64_t main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == "-h" || a[0] == "--help") {
        py::print(py::str("usage: nand info <dump>  |  nand extract-hv <dump> <out.bin>"));
        return 0;
    }
    py::str op = a[0];
    if (op == "info" && py::len(a) > 1) {
        NandInfo info = nand_info(a[1]);
        auto row = [](const char* k, const py::str& v) { py::print(py::percent("  %-20s %s", py::str(k), v)); };
        row("path", info.path);
        row("size", py::to_str(info.size));
        row("type", info.type);
        row("header", header_repr(info.header));
    } else if ((op == "extract-hv" || op == "hv") && py::len(a) > 2) {
        py::file src = py::open_rb(a[1]);
        py::bytes data = src.read();
        src.close();
        py::bytes hv = extract_hv_from_nand(data);
        py::file dst = py::file::open(a[2], py::str("wb"));
        dst.write(hv);
        dst.close();
        py::print(py::percent("extracted HV (%d bytes) -> %s", py::len(hv), a[2]));
    } else {
        py::print(py::str("usage: nand info <dump>  |  nand extract-hv <dump> <out.bin>"));
        return 2;  // sys.exit(2)
    }
    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::nand
