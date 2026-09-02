"""Title-Update (TU) and DLC manager for Xbox 360.

A title update ships as either a raw `.xexp` delta patch or an STFS package
(CON/LIVE/PIRS) wrapping one. `tu apply` reconstructs the fully-updated title
from a base XEX plus the update, reusing this project's byte-exact XEX
delta-apply. `tu info` reads the delta descriptor (the base image size + SHA-1
the patch must be applied against, and the target). `tu dlc` inspects a DLC
content package (an STFS container of add-on content).

Commands: info / apply / dlc.
"""

import os
import sys

DESCRIPTION = "Xbox 360 Title-Update / DLC manager: inspect a TU, apply it to a base title, read DLC"

_STFS_MAGICS = (b"CON ", b"LIVE", b"PIRS")


def _is_stfs(data):
    return data[:4] in _STFS_MAGICS


def _is_xexp(data):
    return data[:4] == b"XEX2"


def _patch_bytes_from(path):
    """Return the raw .xexp delta-patch bytes from a TU input: either the file
    itself (a raw .xexp) or the patch file extracted from an STFS TU package."""
    data = open(path, "rb").read()
    if _is_xexp(data):
        return data, None
    if _is_stfs(data):
        import stfs
        pkg = stfs.STFS(data)
        # the delta XEX inside a TU package: a XEX2 with a patch module-flag bit
        cand = None
        for name in pkg.files:
            try:
                fb = pkg.read_file(name)
            except Exception:
                continue
            if fb[:4] == b"XEX2" and len(fb) >= 8 and (int.from_bytes(fb[4:8], "big") & 0x70):
                cand = (fb, name)
                break
        if cand is None:
            raise ValueError("no delta-patch XEX found inside the STFS package")
        return cand[0], cand[1]
    raise ValueError("input is neither a .xexp nor an STFS TU package")


def apply_update(base_xex_path, tu_path, out_path):
    """Apply a TU (raw .xexp or STFS package) to a base XEX; write the updated
    standalone XEX. Returns (patch_source_name, out_size)."""
    import xex
    patch, inner = _patch_bytes_from(tu_path)
    base = open(base_xex_path, "rb").read()
    updated = xex.apply_delta_patch(base, patch)
    with open(out_path, "wb") as f:
        f.write(updated)
    return inner, len(updated)


def _delta_info(patch):
    import xex
    x = xex.XEXFile(patch)
    mt = x.module_type
    d = {"module_flags": x.module_flag_names(), "patch_type": mt() if callable(mt) else mt}
    dd = getattr(x, "delta_descriptor", None)
    if dd:
        d["source_version"] = dd.get("source_version")
        d["target_version"] = dd.get("target_version")
        d["base_sha1"] = dd.get("source_digest")
        d["target_header_size"] = dd.get("target_header_size")
    try:
        if x.execution_info:
            d["title_id"] = x.execution_info.get("title_id")
            d["version"] = x.execution_info.get("version_str")
    except Exception:
        pass
    return d


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  tu info  <update.xexp | TU-package>          delta descriptor + title\n"
              "  tu apply <base.xex> <update> <out.xex>       reconstruct the updated title\n"
              "  tu dlc   <content-package>                   list a DLC STFS package")
        return
    op = a[0]
    if op == "info":
        patch, inner = _patch_bytes_from(a[1])
        print("update: %s%s" % (a[1], ("  (from STFS: %s)" % inner) if inner else ""))
        info = _delta_info(patch)
        for k, v in info.items():
            print("  %-14s %s" % (k, v))
        return
    if op == "apply" and len(a) >= 4:
        inner, n = apply_update(a[1], a[2], a[3])
        print("applied %s%s to %s -> %s (%d bytes)" % (
            os.path.basename(a[2]), ("[%s]" % inner) if inner else "",
            os.path.basename(a[1]), a[3], n))
        return
    if op == "dlc" and len(a) >= 2:
        import stfs
        pkg = stfs.STFS(open(a[1], "rb").read())
        print("DLC package: %s  (%d files)" % (a[1], len(pkg.files)))
        for name in list(pkg.files)[:60]:
            print("   %s" % name)
        return
    print("usage: tu info|apply|dlc  (run 'tu' for details)"); sys.exit(2)


if __name__ == "__main__":
    main()
