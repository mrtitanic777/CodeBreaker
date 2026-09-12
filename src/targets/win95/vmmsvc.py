#!/usr/bin/env python3
"""
vmmsvc.py — extract VxD service-ID -> name tables from the Win95 DDK includes.

The VxD world's equivalent of the NE ordinal problem. A VxD calls another VxD's
service by a 32-bit ID: (Device_ID << 16) | index, dispatched by the `VxDCall`
macro / `int 20h` + dw <id> idiom. The IDs are opaque numbers in the binary; the
DDK's service-table macros are what map them back to names.

The DDK (`reference/win95ddk/INC32/*.INC`) declares each table as:

    Begin_Service_Table VMM, VMM
    VMM_Service  Get_VMM_Version, LOCAL
    VMM_Service  Get_Cur_VM_Handle
    ...
    End_Service_Table VMM, VMM

The Nth `<Device>_Service` line is service index N (0-based). Combined with the
device's ID (VMM = 0x0001, VPICD = 0x0003, ...), that gives the full 32-bit
service ID the binary encodes. This tool reads those tables and emits a flat
`index  0xID  Name` oracle — the thing the future LE/VxD carver (PLAN.md Phase
3) resolves `Get_VxD_Service` targets against.

Doctrine: this is a reference oracle (hypothesis). It names what a service ID
*is called*; the retail VxD must still be read to confirm what it *does*. The
DDK is a slightly different build era than retail OSR2.5, so treat late-table
entries with care (service tables only ever append, so early indices are safe).

Usage:
    python vmmsvc.py                         # VMM table -> reference/vmm_services.txt
    python vmmsvc.py --inc VPICD.INC --dev VPICD --id 0x0003
    python vmmsvc.py --list                  # every service table found in INC32
"""
import argparse
import glob
import os
import re
import sys

from src.core import layout

DDK_INC = os.path.join(layout.reference_sub("win95ddk"), "INC32")

# Well-known VxD Device IDs (DDK VMM.INC / the published list). Extend as needed.
DEVICE_IDS = {
    "VMM": 0x0001, "VPICD": 0x0003, "VDMAD": 0x0004, "VTD": 0x0005,
    "V86MMGR": 0x0006, "PAGESWAP": 0x0007, "PARITY": 0x0008, "REBOOT": 0x0009,
    "VDD": 0x000A, "VSD": 0x000B, "VMD": 0x000C, "VKD": 0x000D,
    "VPD": 0x000F, "BLOCKDEV": 0x0010, "VMCPD": 0x0011, "EBIOS": 0x0012,
    "VNETBIOS": 0x0014, "DOSMGR": 0x0015, "SHELL": 0x0017, "VMPOLL": 0x0018,
    "VXDLDR": 0x0027, "NDIS": 0x0028, "VWIN32": 0x002A, "VCOMM": 0x002B,
    "IFSMGR": 0x0040, "IOS": 0x0010, "CONFIGMG": 0x0033, "VCOND": 0x0038,
}


def _merge_ddb_devices():
    """Augment DEVICE_IDS with the device-ID->name map DERIVED FROM THE RETAIL DDBs
    (reference/vxd_devices.txt, regenerate via `--gen-devices`). The hardcoded list above
    is only the ~28 published core VxDs; the shipped system ships 200+ VxDs, each carrying
    its own Device_ID in its DDB. This is EVIDENCE (read from the binaries), so it names
    the imports the curated list can't (VCACHE, NWLINK, VFLATD, ...). Curated names WIN on
    an ID clash: we only add IDs not already claimed, so the resolver's id->name inversion
    stays deterministic and the published names remain authoritative."""
    path = layout.reference_sub("vxd_devices.txt")
    if not os.path.exists(path):
        return
    claimed = set(DEVICE_IDS.values())
    names = set(DEVICE_IDS)
    try:
        for line in open(path, encoding="latin1"):
            line = line.split("#", 1)[0].strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 2:
                continue
            did = int(parts[0], 16)
            nm = parts[1].upper()
            if did in claimed or nm in names:
                continue
            DEVICE_IDS[nm] = did
            claimed.add(did)
            names.add(nm)
    except (OSError, ValueError):
        pass


def ddb_device_map():
    """{device_id: DDB_name} read from the DDB_Req_Device_Number (+0x06) of EVERY retail
    VxD's DDB -- the authoritative device table. Static evidence; no execution."""
    import struct, glob as _glob
    import target, le as le_mod, w3
    out = {}
    dirs = [d for d in _glob.glob("src/asm/**/*.vxd", recursive=True) if os.path.isdir(d)]
    dirs += [d for d in _glob.glob("src/asm/**/*.386", recursive=True) if os.path.isdir(d)]
    for mod in sorted({os.path.basename(d) for d in dirs}):
        try:
            L = le_mod.LEFile(target.resolve(mod))
            objs = list(L.objects())
            for name, (obj_idx, ddb_off, _cnt) in L.find_all_ddbs().items():
                b = bytes(L.object_bytes(objs[obj_idx - 1] if obj_idx >= 1 else objs[obj_idx]))
                did = struct.unpack_from("<H", b, ddb_off + 6)[0]
                if did and did not in out:
                    out[did] = name.upper()
        except Exception:
            continue
    try:                                              # VMM core: DDB lives in the LE preamble
        w3.embedded_les("vmm32.vxd")                  # (presence check; VMM=0x0001 already curated)
        out.setdefault(0x0001, "VMM")
    except Exception:
        pass
    return out


def write_device_map(out_path=None):
    """Regenerate reference/vxd_devices.txt from the DDBs."""
    out_path = out_path or layout.reference_sub("vxd_devices.txt")
    m = ddb_device_map()
    with open(out_path, "w") as f:
        f.write("# VxD Device_ID -> device name, read from DDB_Req_Device_Number (+0x06) of\n"
                "# every retail VxD's DDB. EVIDENCE (derived from the shipped binaries), not the\n"
                "# published-list hypothesis. Regenerate: wt --target win95 vmmsvc --gen-devices\n")
        for did in sorted(m):
            f.write("0x%04x  %s\n" % (did, m[did]))
    return out_path, len(m)


_merge_ddb_devices()                                  # augment DEVICE_IDS at import (cheap file read)

BEGIN_RX = re.compile(r"^\s*Begin_Service_Table\s+(\w+)", re.I | re.M)
END_RX = re.compile(r"^\s*End_Service_Table\s+(\w+)", re.I | re.M)


def parse_table(path, device):
    """Ordered list of service names for `device` in the .INC at `path`."""
    svc_rx = re.compile(rf"^\s*{re.escape(device)}_Service\s+([A-Za-z_]\w*)", re.I)
    names, inside = [], False
    with open(path, encoding="latin1") as f:
        for line in f:
            if not inside:
                m = BEGIN_RX.match(line)
                if m and m.group(1).upper() == device.upper():
                    inside = True
                continue
            if END_RX.match(line):
                break
            m = svc_rx.match(line)
            if m:
                names.append(m.group(1))
    return names


def find_tables(incdir):
    """{device: path} for every Begin_Service_Table across the INC dir."""
    out = {}
    for p in glob.glob(os.path.join(incdir, "*.INC")):
        try:
            txt = open(p, encoding="latin1").read()
        except OSError:
            continue
        for m in BEGIN_RX.finditer(txt):
            out.setdefault(m.group(1).upper(), p)
    return out


def emit(device, dev_id, names, fh):
    fh.write(f"# {device} service table  (Device_ID = {dev_id:#06x}, "
             f"{len(names)} services)\n")
    fh.write(f"# service ID = (Device_ID << 16) | index. Source: Win95 DDK "
             f"INC32. Reference/hypothesis — confirm against the retail VxD.\n")
    fh.write("index  service_id   name\n")
    for i, nm in enumerate(names):
        sid = (dev_id << 16) | i
        fh.write(f"{i:>5}  0x{sid:08x}  {nm}\n")
    fh.write("\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--inc", default="VMM.INC", help="include file under the DDK INC32 dir")
    ap.add_argument("--dev", default="VMM", help="device name (service-table prefix)")
    ap.add_argument("--id", default=None, help="Device_ID (hex); looked up if known")
    ap.add_argument("--incdir", default=DDK_INC)
    ap.add_argument("--out", default=layout.reference_sub("vmm_services.txt"))
    ap.add_argument("--list", action="store_true", help="list all service tables found")
    ap.add_argument("--gen-devices", action="store_true",
                    help="regenerate reference/vxd_devices.txt (Device_ID->name) from the retail DDBs")
    args = ap.parse_args()

    if args.gen_devices:
        p, n = write_device_map()
        print("%s: %d VxD Device_IDs (from DDBs)" % (p, n))
        return

    if args.list:
        tables = find_tables(args.incdir)
        print(f"# service tables under {args.incdir}:")
        for dev, path in sorted(tables.items()):
            names = parse_table(path, dev)
            did = DEVICE_IDS.get(dev)
            print(f"  {dev:<12} {len(names):>4} services  id="
                  f"{('0x%04x' % did) if did else '?':<7} {os.path.basename(path)}")
        return

    path = os.path.join(args.incdir, args.inc)
    if not os.path.exists(path):
        sys.exit(f"not found: {path} (extract the DDK into reference/win95ddk first)")
    dev = args.dev.upper()
    dev_id = int(args.id, 16) if args.id else DEVICE_IDS.get(dev)
    if dev_id is None:
        sys.exit(f"unknown Device_ID for {dev}; pass --id 0xNNNN")
    names = parse_table(path, dev)
    if not names:
        sys.exit(f"no {dev}_Service entries found in {path}")
    with open(args.out, "w", encoding="utf-8") as fh:
        emit(dev, dev_id, names, fh)
    print(f"{args.out}: {len(names)} {dev} services (Device_ID {dev_id:#06x})")


if __name__ == "__main__":
    main()
