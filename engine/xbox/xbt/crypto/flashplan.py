"""Xbox 360 RGH/JTAG flash planner — advisor only.

Given a console model and the intended hack, reports which xeBuild *image type*
and *secure-boot block* an RGH/JTAG build needs, and identifies a glitch-chip
CPLD by JTAG IDCODE. This is the planning layer of the J-Runner/xeBuild
workflow; it does NOT assemble or write any image (that stays with xeBuild — it
needs bootloader-encrypt crypto and proprietary per-console patch data, and a
wrong image bricks the console). Mappings reimplemented clean-room from Genexis.

Commands: plan / image-type / secure-boot / cpld / motherboards.
"""

import sys

DESCRIPTION = "RGH/JTAG flash planner: image-type + secure-boot-block advisor (no image is written)"

# Motherboard family classification (by model-string substring).
MOTHERBOARDS = ("xenon", "zephyr", "falcon", "opus", "jasper", "trinity", "corona", "winchester")

# Known hack names, grouped by the xeBuild image type they resolve to.
_HACKS = {
    "glitch":  ["RGH 1"],
    "glitch2": ["RGH 1.2", "RGH 2", "S-RGH", "RGH2"],
    "glitch3": ["RGH 3", "RGH 1.3", "EXT_CLK.3", "RGH3"],
    "jtag":    ["Argon Data", "AUD_CLAMP", "R-JTAG", "JTAG"],
    "devgl":   ["DevGL"],
    "devrgl":  ["DevRGL"],
}

# JTAG CPLD IDCODE -> part (the glitch chip). CoolRunner-II XC2C64A is the
# canonical RGH glitch CPLD; both silicon revisions share the low IDCODE bits.
CPLD_IDCODES = {
    0x06E5E093: "XC2C64A (CoolRunner-II)",
    0x06E5C093: "XC2C64A (CoolRunner-II)",
}


def classify_motherboard(model):
    m = (model or "").lower()
    for mb in MOTHERBOARDS:
        if mb in m:
            return mb
    return "unknown"


def image_type(console_class, hack):
    """Resolve the underlying xeBuild image type from a console class
    ('Retail'/'FreeBoot'/'Devkit') and a hack name. Mirrors Genexis's
    resolveUnderlyingImageType."""
    cc = (console_class or "").strip().lower()
    hk = (hack or "").strip()
    if cc == "retail":
        return "retail"
    if cc in ("freeboot", "rgh", "jtag"):
        if hk in _HACKS["glitch"]:
            return "glitch"
        if hk in _HACKS["glitch3"]:
            return "glitch3"
        if hk in _HACKS["jtag"]:
            return "jtag"
        return "glitch2"                       # RGH 1.2 / RGH 2 / S-RGH default
    if cc == "devkit":
        if hk in _HACKS["devgl"]:
            return "devgl"
        if hk in _HACKS["devrgl"]:
            return "devrgl"
        return "devkit"
    return "retail"


def secure_boot_block(model, nandmu=False):
    """Which secure-boot block override a build needs, by motherboard.
    Corona-4G (or nandmu) -> KSB; Jasper/Trinity -> PSB; else -> XSB."""
    m = (model or "").lower()
    if "corona" in m and ("4g" in m or nandmu):
        return "KSB"
    if "jasper" in m or "trinity" in m:
        return "PSB"
    return "XSB"


def cpld_part(idcode):
    return CPLD_IDCODES.get(idcode & 0xFFFFFFFF, "Unknown JTAG device")


def plan(model, hack, console_class="FreeBoot", nandmu=False):
    return {
        "motherboard": classify_motherboard(model),
        "console_class": console_class,
        "hack": hack,
        "image_type": image_type(console_class, hack),
        "secure_boot_block": secure_boot_block(model, nandmu),
        "nandmu": nandmu,
    }


# ── CLI ────────────────────────────────────────────────────────────────
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  flashplan plan <console-model> <hack> [--class C] [--nandmu]\n"
              "      e.g. flashplan plan \"Corona 4G\" \"RGH 3\"\n"
              "  flashplan image-type <Retail|FreeBoot|Devkit> <hack>\n"
              "  flashplan secure-boot <console-model> [--nandmu]\n"
              "  flashplan cpld <idcode-hex>\n"
              "  flashplan motherboards            list recognized boards\n"
              "  flashplan hacks                   list recognized hack names\n"
              "(advisor only — no flashable image is produced; use xeBuild/J-Runner to build)")
        return
    op = a[0]
    if op == "motherboards":
        print("  " + "  ".join(m.capitalize() for m in MOTHERBOARDS)); return
    if op == "hacks":
        for it, names in _HACKS.items():
            print("  %-8s %s" % (it, ", ".join(names)))
        print("  retail   Retail")
        return
    if op == "cpld" and len(a) > 1:
        idc = int(a[1], 16)
        print("IDCODE 0x%08X -> %s" % (idc, cpld_part(idc))); return
    if op == "image-type" and len(a) > 2:
        print(image_type(a[1], a[2])); return
    if op == "secure-boot" and len(a) > 1:
        print(secure_boot_block(a[1], "--nandmu" in a)); return
    if op == "plan" and len(a) > 2:
        cc = a[a.index("--class") + 1] if "--class" in a else "FreeBoot"
        p = plan(a[1], a[2], cc, "--nandmu" in a)
        print("console model     : %s" % a[1])
        print("motherboard       : %s" % p["motherboard"])
        print("console class      : %s" % p["console_class"])
        print("hack               : %s" % p["hack"])
        print("xeBuild image type : %s" % p["image_type"])
        print("secure-boot block  : %s" % p["secure_boot_block"])
        print("\nbuild the image with xeBuild/J-Runner (image type '%s', %s override)."
              % (p["image_type"], p["secure_boot_block"]))
        return
    print("unknown/short command; run 'flashplan' for usage"); sys.exit(2)


if __name__ == "__main__":
    main()
