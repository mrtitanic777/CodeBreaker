"""driver_services.py -- the real names of the services ring-0 drivers call.

A virtual device driver calls the system and other drivers with `int 20h` followed by a
service number and a device number; the loader turns those six bytes into a call. The numbers
mean nothing by themselves. The driver kit's include files give each device its number
(`VKD_Device_ID EQU 0000DH`) and list its services in order between `Begin_Service_Table VKD`
and `End_Service_Table` -- the Nth `VKD_Service` line is service N. That list, read once from
the include files, is kept beside this module in driver_services.json; the package only reads
the table (through the knowledge base: platform vxd, kinds device, service_table, driver, message).

    from codebreaker.symbols import driver_services
    driver_services.service_name(0x0001, 0x0001)     -> "Get_Cur_VM_Handle"
    driver_services.service_name(0x000D, 0x0001)     -> "VKD_Define_Hot_Key"
    driver_services.device_name(0x000D)              -> "VKD"
    driver_services.label(0x0001, 0x0123)            -> the real name, else a placeholder
    driver_services.message_name(1)                  -> "DEVICE_INIT"   (what a control procedure is sent)
    driver_services.generate()                       -> rewrite the table from the include files

    python -m codebreaker driver-services [FILE ...]      the table's size; with driver files,
                                                          how many of their service calls it names
    python -m codebreaker driver-services --generate [INCLUDE_DIR]

Bit 15 of a service number says "jump, do not call"; it is not part of the number. Device
numbers the kit does not name are filled in from the corpus: every driver states its own
number and name in its device descriptor block (kept apart, under "drivers", because that is
what a file says about itself, not what the kit publishes).
"""
import json
import os
import re

TABLE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "driver_services.json")
_loaded = None

_BEGIN = re.compile(r"^\s*Begin_Service_Table\s+(\w+)", re.I)
_END = re.compile(r"^\s*End_Service_Table\b", re.I)
_ID_ASM = re.compile(r"^\s*(\w+?)_Device_ID\s+(?:equ|=)\s+([^;]+)", re.I)
_ID_C = re.compile(r"^\s*#define\s+(\w+?)_Device_ID\s+(\w+)", re.I)
_EQU = re.compile(r"^\s*(\w+)\s+(?:equ|=)\s+([0-9][0-9A-Fa-f]*h?)\s*(?:;.*)?$", re.I)


def _table():
    global _loaded
    if _loaded is None:
        from codebreaker import knowledge                    # (driver_services.json, as the knowledge base has it)
        _loaded = {part: {int(k, 16): v for k, v in knowledge.table("vxd", kind).items()}
                   for part, kind in (("devices", "device"), ("services", "service_table"), ("drivers", "driver"),
                                      ("messages", "message"))}
    return _loaded


def device_name(device):
    """The name of a device number: the kit's, else what the corpus driver with that number
    calls itself, else None."""
    t = _table()
    return t["devices"].get(device) or t["drivers"].get(device)


def service_name(device, service):
    """The kit's name for service `service` of device `device`, or None."""
    names = _table()["services"].get(device) or ()
    n = service & 0x7FFF
    return names[n] if n < len(names) and names[n] else None


def label(device, service):
    """A name to print for a service call: the real one when the table has it; for a known
    device with a service past its table, DEVICE_Service_NNNN; else the number alone."""
    real = service_name(device, service)
    if real:
        return real
    n = service & 0x7FFF
    dev = device_name(device)
    if dev:
        return "%s_Service_%04X" % (re.sub(r"\W", "_", dev), n)
    return ("VMM_%04X" % n) if device == 1 else "VxD_%04X_%04X" % (device, n)


def messages():
    """{number: name} of the messages the system sends every driver's control procedure."""
    return dict(_table()["messages"])


def message_name(number):
    return _table()["messages"].get(number)


def size():
    """(devices with a service table, services named, device numbers named)."""
    t = _table()
    return (len(t["services"]), sum(1 for v in t["services"].values() for n in v if n),
            len(set(t["devices"]) | set(t["drivers"])))


# Making the table -----------------------------------------------------------------------------
def include_dir():
    """Where the driver kit's 32-bit include files are on this machine, or ''. CB_DRIVER_KIT
    names the folder; otherwise it is looked for in the reference folder beside the corpus."""
    from codebreaker.base import corpus
    cands = [os.environ.get("CB_DRIVER_KIT") or ""]
    ref = os.path.join(corpus.design_dir(), "reference")
    cands += [os.path.join(ref, "Microsoft Windows 95 DDK", "INC32"), os.path.join(ref, "win95ddk", "INC32")]
    for c in cands:
        if c and os.path.isfile(os.path.join(c, "VMM.INC")):
            return c
    return ""


def _number(text, known):
    """A device number as the include files write it: 0000DH, 38H, 0x28, another device's
    name, or NAME + n. None when it cannot be worked out."""
    total = 0
    for part in text.replace("(", " ").replace(")", " ").split("+"):
        part = part.strip()
        m = re.fullmatch(r"0x([0-9A-Fa-f]+)", part) or re.fullmatch(r"([0-9][0-9A-Fa-f]*)h", part, re.I)
        if m:
            total += int(m.group(1), 16)
        elif part.isdigit():
            total += int(part)
        elif part.upper() in known:
            total += known[part.upper()]
        else:
            return None
    return total


def read_include_files(folder):
    """-> ({device number: name}, {device number: [service names in order]}) from every .INC
    and .H in `folder`. A header's assembly block lists services the same way an include file
    does; where two tables claim one device number (the old block device and its successor),
    the longer one is kept, as the newer driver answers the older one's services first."""
    files = sorted(os.listdir(folder), key=lambda n: (not n.upper().endswith(".INC"), n.upper()))
    texts = {}
    for fn in files:
        if fn.upper().endswith((".INC", ".H")):
            with open(os.path.join(folder, fn), encoding="latin1") as f:
                texts[fn] = f.read().splitlines()
    equates, pending, spelled, first = {}, {}, {}, {}
    for fn, lines in texts.items():
        for line in lines:
            m = _ID_ASM.match(line) or _ID_C.match(line)
            if m:
                pending.setdefault(m.group(1).upper(), m.group(2).strip())
                spelled.setdefault(m.group(1).upper(), m.group(1))
                first.setdefault(m.group(1).upper(), fn.upper())
                continue
            m = _EQU.match(line)
            if m:
                v = _number(m.group(2), {})
                if v is not None:
                    equates.setdefault(m.group(1).upper(), v)
    ids = {}
    for _round in range(4):                                  # a number may be another device's name
        known = dict(equates)
        known.update({k + "_DEVICE_ID": v for k, v in ids.items()})
        for name, text in pending.items():
            if name not in ids:
                v = _number(text, known)
                if v is not None:
                    ids[name] = v
    tables = {}
    for fn, lines in texts.items():
        cur = macro = None
        for line in lines:
            code = line.split(";", 1)[0]
            m = _BEGIN.match(code)
            if m and "MACRO" not in code.upper():
                cur = m.group(1)
                macro = re.compile(r"^\s*%s_Service\s+(\w+)" % re.escape(cur), re.I)
                tables.setdefault(cur.upper(), {}).setdefault(fn, [])
                continue
            if cur and _END.match(code):
                cur = None
            elif cur:
                m = macro.match(code)
                if m:
                    tables[cur.upper()][fn].append(m.group(1))
    devices, services = {}, {}
    for name in sorted(ids, key=lambda n: (first[n] != "VMM.INC", first[n], n)):
        if 0 < ids[name] < 0x10000:                          # several names, one number: the kit's
            devices.setdefault(ids[name], spelled[name])     # main list (VMM.INC) names it
    for name, by_file in tables.items():
        number = ids.get(name)
        best = max(by_file.values(), key=len)
        if number is None or not best:
            continue
        if len(best) > len(services.get(number, ())):
            services[number] = best
            devices[number] = spelled.get(name, name)
    return devices, services


def read_messages(folder):
    """{number: name} of the system control messages, from the kit's main include file: the
    run of four-digit equates from SYS_CRITICAL_INIT up to MAX_SYSTEM_CONTROL."""
    out, inside = {}, False
    with open(os.path.join(folder, "VMM.INC"), encoding="latin1") as f:
        for line in f:
            m = re.match(r"^(\w+)\s+EQU\s+(00[0-9A-F]{2})H\s*$", line, re.I)
            if m and m.group(1).upper() == "SYS_CRITICAL_INIT":
                inside = True
            if m and m.group(1).upper() == "MAX_SYSTEM_CONTROL":
                break
            if m and inside:
                out.setdefault(int(m.group(2), 16), m.group(1).upper())
    return out


def corpus_drivers():
    """{device number: name} as every driver in the corpus states them in its own descriptor."""
    from codebreaker.formats import le
    out = {}
    for path in le.corpus_files():
        try:
            for _name, lf in le.members(path):
                d = le.descriptor(lf)
                if d and d["device_id"] and d["name"]:
                    out.setdefault(d["device_id"], d["name"])
        except Exception:
            continue
    return out


def generate(folder=None, out=TABLE):
    """Rewrite the table from the include files in `folder`. -> size()"""
    global _loaded
    folder = folder or include_dir()
    if not folder:
        raise SystemExit("the driver kit's include files were not found; set CB_DRIVER_KIT to its INC32 folder")
    devices, services = read_include_files(folder)
    drivers = {k: v for k, v in corpus_drivers().items() if k not in devices}
    hx = lambda d: {"%04X" % k: d[k] for k in sorted(d)}     # noqa: E731
    with open(out, "w", newline="\n", encoding="utf-8") as f:
        json.dump({"source": "Windows 95 driver kit include files (INC32): Begin_Service_Table lists and "
                             "*_Device_ID equates; \"drivers\" from the corpus drivers' own descriptors",
                   "devices": hx(devices), "services": hx(services), "drivers": hx(drivers),
                   "messages": hx(read_messages(folder))},
                  f, indent=1)
        f.write("\n")
    from codebreaker.knowledge import sources
    sources.changed(out)                                     # the knowledge base loads it again
    _loaded = None
    return size()


def coverage(path):
    """How the table does on one driver file: -> (service calls, named, {unnamed (device, service): count})."""
    from codebreaker.formats import le
    calls = named = 0
    missing = {}
    for _name, lf in le.members(path):
        for _obj, _off, device, service, _jump in le.service_calls(lf):
            calls += 1
            if service_name(device, service):
                named += 1
            else:
                key = (device, service & 0x7FFF)
                missing[key] = missing.get(key, 0) + 1
    return calls, named, missing


def main(argv):
    """python -m codebreaker driver-services [--generate [INCLUDE_DIR]] [FILE ...]"""
    from codebreaker.base import corpus
    if argv and argv[0] == "--generate":
        tables, names, devices = generate(argv[1] if len(argv) > 1 else None)
        print("%s: %d service tables, %d services, %d device numbers" % (TABLE, tables, names, devices))
        return 0
    tables, names, devices = size()
    print("%d service tables, %d services named, %d device numbers named" % (tables, names, devices))
    for arg in argv:
        path = corpus.retail(arg)
        if not path:
            print("%s: not found" % arg)
            continue
        calls, named, missing = coverage(path)
        print("%-16s %4d service calls, %4d named%s" % (
            os.path.basename(path), calls, named,
            "".join("  %s x%d" % (label(d, s), n) for (d, s), n in sorted(missing.items())[:6])))
    return 0
