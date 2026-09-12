#!/usr/bin/env python3
r"""installer.py -- identify installer/packaging formats and extract embedded scripts (niche).

Setup programs wrap the real payload in a packaging format: MS Cabinet (MSCF), SZDD/KWAJ
compression, InstallShield, Wise, or the MS ACME/STF setup tables -- and often carry an embedded
INF/STF script describing what to install. Recognizing the family tells you how a distribution is
built, and pulling the embedded INF/text sections recovers the setup script (the [Version]/[Files]/
[Setup] directives). Niche and heterogeneous, but it turns an opaque installer blob into its family
+ its script. Decompression of the payload is out of scope; this identifies and extracts text.
"""
import re

_SIGS = [
    (b"MSCF", "MS Cabinet (CAB)"), (b"SZDD\x88\xf0\x27\x33", "MS SZDD compressed"),
    (b"KWAJ\x88\xf0\x27\xd1", "MS KWAJ compressed"), (b"InstallShield", "InstallShield"),
    (b"ISSetup", "InstallShield"), (b"Wise Installation", "Wise Installer"),
    (b"WiseMain", "Wise Installer"), (b"MSCUISTF", "MS ACME Setup (STF)"),
    (b"Inno Setup", "Inno Setup"), (b"Nullsoft", "NSIS"),
    (b"_winzip_", "WinZip Self-Extractor"), (b"aPKG", "aPackage"),
]
_INF_SECTION = re.compile(rb"\[[A-Za-z][A-Za-z0-9 ._%-]{1,40}\]\r?\n")


def detect(data):
    """[(family, offset)] for every packaging/installer signature present."""
    out = []
    for sig, family in _SIGS:
        i = data.find(sig)
        if i >= 0:
            out.append((family, i))
    return out


def extract_inf(data, min_sections=2):
    """Embedded INF/STF-style script text: contiguous ASCII spanning >= min_sections [Section] heads."""
    out = []
    for m in re.finditer(rb"\[Version\]\r?\n", data):
        start = m.start()
        end = start
        # extend while the bytes stay printable-ish (an INF is ASCII text)
        p = start
        while p < len(data) and (32 <= data[p] < 127 or data[p] in (9, 10, 13)):
            p += 1
        block = data[start:p]
        if len(_INF_SECTION.findall(block)) >= min_sections and len(block) >= 40:
            out.append(block.decode("latin1", "replace"))
    return out


def reconstruct(path):
    """Identify a file's installer/packaging family and extract any embedded INF script. {'ok',...}."""
    try:
        with open(path, "rb") as f:
            data = f.read()
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    families = detect(data)
    infs = extract_inf(data)
    if not families and not infs:
        return {"ok": False, "reason": "no installer/packaging signature or embedded INF"}
    return {"ok": True, "families": [f for f, _o in families], "ninf": len(infs),
            "inf": (infs[0][:1200] if infs else None)}
