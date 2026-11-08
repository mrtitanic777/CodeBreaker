// codebreaker/formats/xbox/crypto/flashplan.cpp -- C++ port of codebreaker.formats.xbox.crypto.flashplan.

#include "codebreaker/formats/xbox/crypto/flashplan.hpp"

#include <tuple>

namespace codebreaker::formats::xbox::crypto::flashplan {

const py::str DESCRIPTION("RGH/JTAG flash planner: image-type + secure-boot-block advisor (no image is written)");

const py::list<py::str> MOTHERBOARDS = [] {
    py::list<py::str> l;
    for (const char* m : {"xenon", "zephyr", "falcon", "opus", "jasper", "trinity", "corona", "winchester"}) l.append(py::str(m));
    return l;
}();

const py::dict<py::str, py::list<py::str>> _HACKS = [] {
    auto mk = [](std::initializer_list<const char*> xs) {
        py::list<py::str> l;
        for (const char* x : xs) l.append(py::str(x));
        return l;
    };
    py::dict<py::str, py::list<py::str>> d;
    d.set(py::str("glitch"), mk({"RGH 1"}));
    d.set(py::str("glitch2"), mk({"RGH 1.2", "RGH 2", "S-RGH", "RGH2"}));
    d.set(py::str("glitch3"), mk({"RGH 3", "RGH 1.3", "EXT_CLK.3", "RGH3"}));
    d.set(py::str("jtag"), mk({"Argon Data", "AUD_CLAMP", "R-JTAG", "JTAG"}));
    d.set(py::str("devgl"), mk({"DevGL"}));
    d.set(py::str("devrgl"), mk({"DevRGL"}));
    return d;
}();

const py::dict<int64_t, py::str> CPLD_IDCODES = {
    {0x06E5E093, py::str("XC2C64A (CoolRunner-II)")},
    {0x06E5C093, py::str("XC2C64A (CoolRunner-II)")},
};

namespace {

// hk in _HACKS[key]
bool hack_in(const char* key, const py::str& hk) {
    return _HACKS.at(py::str(key)).contains(hk);
}

// (x or "")
py::str or_empty(const std::optional<py::str>& s) { return s ? *s : py::str(""); }

}  // namespace

py::str classify_motherboard(const std::optional<py::str>& model) {
    py::str m = or_empty(model).lower();
    for (const py::str& mb : MOTHERBOARDS) {
        if (m.contains(mb)) return mb;
    }
    return py::str("unknown");
}

py::str image_type(const std::optional<py::str>& console_class, const std::optional<py::str>& hack) {
    py::str cc = or_empty(console_class).strip().lower();
    py::str hk = or_empty(hack).strip();
    if (cc == "retail") return py::str("retail");
    if (cc == "freeboot" || cc == "rgh" || cc == "jtag") {
        if (hack_in("glitch", hk)) return py::str("glitch");
        if (hack_in("glitch3", hk)) return py::str("glitch3");
        if (hack_in("jtag", hk)) return py::str("jtag");
        return py::str("glitch2");  // RGH 1.2 / RGH 2 / S-RGH default
    }
    if (cc == "devkit") {
        if (hack_in("devgl", hk)) return py::str("devgl");
        if (hack_in("devrgl", hk)) return py::str("devrgl");
        return py::str("devkit");
    }
    return py::str("retail");
}

py::str secure_boot_block(const std::optional<py::str>& model, bool nandmu) {
    py::str m = or_empty(model).lower();
    if (m.contains("corona") && (m.contains("4g") || nandmu)) return py::str("KSB");
    if (m.contains("jasper") || m.contains("trinity")) return py::str("PSB");
    return py::str("XSB");
}

py::str cpld_part(int64_t idcode) {
    return CPLD_IDCODES.get(idcode & 0xFFFFFFFF, py::str("Unknown JTAG device"));
}

Plan plan(const std::optional<py::str>& model, const std::optional<py::str>& hack,
          const std::optional<py::str>& console_class, bool nandmu) {
    Plan p;
    p.motherboard = classify_motherboard(model);
    p.console_class = console_class;
    p.hack = hack;
    p.image_type = image_type(console_class, hack);
    p.secure_boot_block = secure_boot_block(model, nandmu);
    p.nandmu = nandmu;
    return p;
}

// ── CLI ────────────────────────────────────────────────────────────────
void main(const py::list<py::str>& argv) {
    py::list<py::str> a = argv.slice(1, py::len(argv));
    if (py::len(a) == 0 || a[0] == "-h" || a[0] == "--help") {
        py::print(py::str("usage:\n"
                          "  flashplan plan <console-model> <hack> [--class C] [--nandmu]\n"
                          "      e.g. flashplan plan \"Corona 4G\" \"RGH 3\"\n"
                          "  flashplan image-type <Retail|FreeBoot|Devkit> <hack>\n"
                          "  flashplan secure-boot <console-model> [--nandmu]\n"
                          "  flashplan cpld <idcode-hex>\n"
                          "  flashplan motherboards            list recognized boards\n"
                          "  flashplan hacks                   list recognized hack names\n"
                          "(advisor only \xE2\x80\x94 no flashable image is produced; use xeBuild/J-Runner to build)"));
        return;
    }
    py::str op = a[0];
    const bool nandmu = a.contains(py::str("--nandmu"));
    if (op == "motherboards") {
        py::list<py::str> caps;
        for (const py::str& m : MOTHERBOARDS) caps.append(m.capitalize());
        py::print(py::str("  ") + py::join(py::str("  "), caps));
        return;
    }
    if (op == "hacks") {
        for (const auto& kv : _HACKS.items()) {
            py::print(py::percent(py::str("  %-8s %s"), std::get<0>(kv), py::join(py::str(", "), std::get<1>(kv))));
        }
        py::print(py::str("  retail   Retail"));
        return;
    }
    if (op == "cpld" && py::len(a) > 1) {
        int64_t idc = py::int_from_str(a[1], 16);
        py::print(py::percent(py::str("IDCODE 0x%08X -> %s"), idc, cpld_part(idc)));
        return;
    }
    if (op == "image-type" && py::len(a) > 2) {
        py::print(image_type(a[1], a[2]));
        return;
    }
    if (op == "secure-boot" && py::len(a) > 1) {
        py::print(secure_boot_block(a[1], nandmu));
        return;
    }
    if (op == "plan" && py::len(a) > 2) {
        py::str cc = a.contains(py::str("--class")) ? a[a.index(py::str("--class")) + 1] : py::str("FreeBoot");
        Plan p = plan(a[1], a[2], cc, nandmu);
        py::print(py::percent(py::str("console model     : %s"), a[1]));
        py::print(py::percent(py::str("motherboard       : %s"), p.motherboard));
        py::print(py::percent(py::str("console class      : %s"), *p.console_class));
        py::print(py::percent(py::str("hack               : %s"), *p.hack));
        py::print(py::percent(py::str("xeBuild image type : %s"), p.image_type));
        py::print(py::percent(py::str("secure-boot block  : %s"), p.secure_boot_block));
        py::print(py::percent(py::str("\nbuild the image with xeBuild/J-Runner (image type '%s', %s override)."),
                              p.image_type, p.secure_boot_block));
        return;
    }
    py::print(py::str("unknown/short command; run 'flashplan' for usage"));
    throw SystemExit(2);
}

}  // namespace codebreaker::formats::xbox::crypto::flashplan
