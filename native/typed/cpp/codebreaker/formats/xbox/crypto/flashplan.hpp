// codebreaker/formats/xbox/crypto/flashplan.hpp -- C++ port of codebreaker.formats.xbox.crypto.flashplan.
//
// Xbox 360 RGH/JTAG flash planner -- advisor only.
//
// Given a console model and the intended hack, reports which xeBuild *image type*
// and *secure-boot block* an RGH/JTAG build needs, and identifies a glitch-chip
// CPLD by JTAG IDCODE. This is the planning layer of the J-Runner/xeBuild
// workflow; it does NOT assemble or write any image (that stays with xeBuild -- it
// needs bootloader-encrypt crypto and proprietary per-console patch data, and a
// wrong image bricks the console). Mappings reimplemented clean-room from Genexis.
//
// Commands: plan / image-type / secure-boot / cpld / motherboards.
#pragma once

#include <optional>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::flashplan {

// sys.exit's SystemExit.
using py::SystemExit;

extern const py::str DESCRIPTION;

// Motherboard family classification (by model-string substring).
extern const py::list<py::str> MOTHERBOARDS;

// Known hack names, grouped by the xeBuild image type they resolve to (_HACKS).
extern const py::dict<py::str, py::list<py::str>> _HACKS;

// JTAG CPLD IDCODE -> part (the glitch chip). CoolRunner-II XC2C64A is the
// canonical RGH glitch CPLD; both silicon revisions share the low IDCODE bits.
extern const py::dict<int64_t, py::str> CPLD_IDCODES;

// plan()'s result: a dict with fixed keys.
struct Plan {
    py::str motherboard;
    std::optional<py::str> console_class;   // (as the caller gave it; may be None)
    std::optional<py::str> hack;            // (as the caller gave it; may be None)
    py::str image_type;
    py::str secure_boot_block;
    bool nandmu = false;
};

py::str classify_motherboard(const std::optional<py::str>& model);

// Resolve the underlying xeBuild image type from a console class
// ('Retail'/'FreeBoot'/'Devkit') and a hack name. Mirrors Genexis's
// resolveUnderlyingImageType.
py::str image_type(const std::optional<py::str>& console_class, const std::optional<py::str>& hack);

// Which secure-boot block override a build needs, by motherboard.
// Corona-4G (or nandmu) -> KSB; Jasper/Trinity -> PSB; else -> XSB.
py::str secure_boot_block(const std::optional<py::str>& model, bool nandmu = false);

py::str cpld_part(int64_t idcode);

Plan plan(const std::optional<py::str>& model, const std::optional<py::str>& hack,
          const std::optional<py::str>& console_class = py::str("FreeBoot"), bool nandmu = false);

// CLI. argv is sys.argv (argv[0] is the program name); output goes to stdout; raises SystemExit(2)
// on an unknown/short command.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::flashplan
