"""Xbox 360 disc stealth verification (abgx360-class) — SS / DMI / PFI.

Parses the three security structures a genuine Xbox 360 disc carries and runs
the cross-consistency checks that answer "is this a clean, matched stealth
rip": the Media ID must agree across DMI, SS and the game's default.xex, the
DMI magic must be present, and the SS XGD version must be self-consistent.

Structure offsets are clean-room from the documented format (ParseXboxMetadata
by Deterous, and the public XGD1/2/3 specs). This is the verification half of
the disc tooling — it complements gdf/xgd (which read the filesystem). It does
NOT reimplement abgx360's deep security-angle / CRC-database replay checks;
those need a per-title database and the full SS challenge tables.

Commands:
    stealth <SS.bin> <DMI.bin> [PFI.bin] [--xex default.xex]
    stealth <dir>                      auto-find ss.bin / dmi.bin / pfi.bin
"""

import os
import struct
import sys

DMI_MAGIC = bytes.fromhex("0002000058424F5800000000")   # ...\0\2\0\0 "XBOX" \0\0\0\0

# SS layer-0 end PSN (bytes 0x0D..0x10) -> XGD generation.
_XGD_L0 = {bytes([0x20, 0x33, 0xAF]): 1,
           bytes([0x20, 0x33, 0x9F]): 2,
           bytes([0x23, 0x8E, 0x0F]): 3}


def _mid(b):
    """Media ID as the canonical XXXXXXXX-XXXXXXXX hyphenated hex string."""
    h = b.hex().upper()
    return h[:16] + "-" + h[16:] if len(h) >= 24 else h


def parse_dmi(data: bytes) -> dict:
    if len(data) < 0x800:
        return {"error": "DMI < 2048 bytes"}
    system = data[0]
    return {
        "system": system,
        "is_360": system == 0x02,
        "xgd": "XGD1 (original Xbox)" if system == 0x01 else "XGD2/XGD3 (Xbox 360)",
        "timestamp": int.from_bytes(data[0x10:0x18], "little"),
        "key_type": {1: "Beta", 2: "Retail"}.get(data[0x18], "0x%02X" % data[0x18]),
        "media_id": data[0x20:0x30],
        "magic_ok": data[0x7E4:0x7F0] == DMI_MAGIC,
    }


def parse_ss(data: bytes) -> dict:
    if len(data) < 0x800:
        return {"error": "SS < 2048 bytes"}
    l0 = data[0x0D:0x10]
    xgd = _XGD_L0.get(bytes(l0))
    secondary = data[0x4BA]                              # 1=XGD1, 2=XGD2/3
    return {
        "xgd": xgd,
        "l0_end_psn": int.from_bytes(l0, "big"),
        "secondary_xgd": secondary,
        "version_consistent": (xgd == 1) == (secondary == 1),
        "media_id": data[0x460:0x470],
        "cpr_mai": data[0x2D0:0x2D4].hex().upper(),
    }


def parse_pfi(data: bytes) -> dict:
    if len(data) < 0x10:
        return {"error": "PFI too small"}
    start = int.from_bytes(data[4:8], "big")
    l0_end = int.from_bytes(data[12:16], "big")
    return {
        "data_start_lba": start - 0x30000,
        "layerbreak_lba": l0_end - 0x30000,
        "l0_end_psn": l0_end,
    }


def verify(ss: dict, dmi: dict, pfi: dict = None, xex_media_id: bytes = None) -> list:
    """Return a list of (name, ok, detail) checks; overall pass = all critical ok."""
    checks = []

    def add(name, ok, detail, critical=True):
        checks.append((name, ok, detail, critical))

    add("DMI is Xbox 360", dmi.get("is_360", False),
        dmi.get("xgd", "?"))
    add("DMI magic present", dmi.get("magic_ok", False),
        "0x7E4 == 0002000058424F5800000000")
    add("SS XGD version detected", ss.get("xgd") is not None,
        "XGD%s" % ss.get("xgd") if ss.get("xgd") else "unknown L0-end PSN")
    add("SS version self-consistent", ss.get("version_consistent", False),
        "L0-end vs 0x4BA (=%s)" % ss.get("secondary_xgd"))
    same = dmi.get("media_id") == ss.get("media_id")
    add("Media ID: DMI == SS", same,
        "%s vs %s" % (_mid(dmi.get("media_id", b"")), _mid(ss.get("media_id", b""))))
    if xex_media_id is not None:
        add("Media ID: DMI == default.xex", dmi.get("media_id") == xex_media_id,
            "%s vs %s" % (_mid(dmi.get("media_id", b"")), _mid(xex_media_id)))
    if pfi and "layerbreak_lba" in pfi:
        # SS L0-end PSN and PFI L0-end PSN should agree on a genuine disc.
        add("PFI L0-end == SS L0-end", pfi.get("l0_end_psn") == ss.get("l0_end_psn"),
            "PFI 0x%X vs SS 0x%X" % (pfi.get("l0_end_psn", 0), ss.get("l0_end_psn", 0)),
            critical=False)
    return checks


DESCRIPTION = ("verify Xbox 360 disc stealth (SS/DMI/PFI): media-ID match, DMI "
               "magic, XGD version consistency")


# ------------------------------------------------------------------------ CLI
def _find(d, *names):
    for f in os.listdir(d):
        low = f.lower()
        if any(n in low for n in names) and f.lower().endswith(".bin"):
            return os.path.join(d, f)
    return None


def main():
    a = sys.argv[1:]
    xex_path = None
    if "--xex" in a:
        i = a.index("--xex")
        xex_path = a[i + 1]
        del a[i:i + 2]
    if not a or a[0] in ("-h", "--help"):
        print("usage: stealth <SS.bin> <DMI.bin> [PFI.bin] [--xex default.xex]\n"
              "       stealth <dir>   (auto-find ss/dmi/pfi .bin files)")
        return

    if len(a) == 1 and os.path.isdir(a[0]):
        ss_p, dmi_p, pfi_p = _find(a[0], "ss"), _find(a[0], "dmi"), _find(a[0], "pfi")
    else:
        ss_p = a[0]
        dmi_p = a[1] if len(a) > 1 else None
        pfi_p = a[2] if len(a) > 2 else None
    if not ss_p or not dmi_p:
        print("need at least SS.bin and DMI.bin"); sys.exit(2)

    ss = parse_ss(open(ss_p, "rb").read())
    dmi = parse_dmi(open(dmi_p, "rb").read())
    pfi = parse_pfi(open(pfi_p, "rb").read()) if pfi_p and os.path.exists(pfi_p) else None
    xex_mid = None
    if xex_path:
        sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
        import xex as _xex
        xex_mid = _xex.open_xex(xex_path).security()["media_id"]

    print("DMI : %s  key=%s  media=%s  magic=%s" % (
        dmi.get("xgd"), dmi.get("key_type"), _mid(dmi.get("media_id", b"")),
        "ok" if dmi.get("magic_ok") else "BAD"))
    print("SS  : XGD%s  media=%s  cpr_mai=%s" % (
        ss.get("xgd"), _mid(ss.get("media_id", b"")), ss.get("cpr_mai")))
    if pfi:
        print("PFI : data_start=0x%X  layerbreak=0x%X" % (
            pfi.get("data_start_lba", 0), pfi.get("layerbreak_lba", 0)))
    print()
    checks = verify(ss, dmi, pfi, xex_mid)
    ok_all = True
    for name, ok, detail, critical in checks:
        if critical and not ok:
            ok_all = False
        tag = "PASS" if ok else ("FAIL" if critical else "warn")
        print("  [%s] %-30s %s" % (tag, name, detail))
    print()
    print("VERDICT: %s" % ("STEALTH OK — media ID matched, structures consistent"
                           if ok_all else "FAILED — see checks above"))
    sys.exit(0 if ok_all else 1)


if __name__ == "__main__":
    main()
