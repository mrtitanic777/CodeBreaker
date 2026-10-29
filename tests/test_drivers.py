#!/usr/bin/env python3
"""Tests for virtual device drivers: the service-name table, the LE reader and writer, the
container directory, building one from assembly source, and the judge. Run:

    python tests/test_drivers.py     (exit code 0 = all passed)

Checks that need the corpus, the OpenWatcom tree, the driver kit's include files or the
assembly sources are SKIPPED when those are not present."""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus, settings              # noqa: E402
from codebreaker.formats import le                         # noqa: E402
from codebreaker.symbols import driver_services            # noqa: E402

RESULTS = []
SMALL = "vkd.vxd"                 # 45 KB, 10 objects, 21 services, an API procedure


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "skip", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _driver(name=SMALL):
    path = corpus.retail(name) if os.path.isdir(corpus.root()) else None
    if not path:
        raise Skip("no %s in the corpus" % name)
    return path


# --- the service-name table --------------------------------------------------------------------
def test_table_names_services():
    tables, names, devices = driver_services.size()
    assert tables >= 30 and names >= 1000 and devices >= 100, (tables, names, devices)
    # the kit's own include file asserts these two numbers (.errnz @@name - 100D1h / 100F1h)
    assert driver_services.service_name(1, 0xD1) == "Test_DBCS_Lead_Byte"
    assert driver_services.service_name(1, 0xF1) == "Get_Crit_Status_No_Block"
    assert driver_services.service_name(1, 0x8001) == "Get_Cur_VM_Handle"        # bit 15 = jump
    assert driver_services.service_name(0xD, 1) == "VKD_Define_Hot_Key"
    assert driver_services.device_name(0xD) == "VKD"
    assert driver_services.label(1, 0x3FF) == "VMM_Service_03FF"
    assert driver_services.label(0x7FF, 2) == "VxD_07FF_0002"
    assert driver_services.message_name(1) == "DEVICE_INIT" and driver_services.message_name(0x23) == "W32_DEVICEIOCONTROL"


def test_table_names_are_identifiers():
    seen = {}
    for device, names in driver_services._table()["services"].items():
        for i, n in enumerate(names):
            assert re.fullmatch(r"[A-Za-z_]\w*", n), n
            assert n not in seen, "%s is both %r and %r" % (n, seen[n], (device, i))
            seen[n] = (device, i)


def test_table_names_a_real_driver():
    calls, named, _missing = driver_services.coverage(_driver())
    assert calls > 100 and named == calls, (calls, named)


# --- the reader --------------------------------------------------------------------------------
def test_reader_descriptor_and_entries():
    lf = le.members(_driver())[0][1]
    d = le.descriptor(lf)
    assert d["name"] == "VKD" and d["device_id"] == 0xD and d["kit_version"] == 0x400 and d["size"] == 0x50
    assert d["service_count"] == 21 and d["control"] and d["service_table"]
    assert le.header_fields(lf)["module_flags"] == le.MODULE_STATIC
    assert [(n, o) for n, o, _obj, _off in le.exports(lf)] == [("VKD_DDB", 1)]
    assert le.exports(lf)[0][2:] == (d["obj"], d["offset"])           # the one export IS the descriptor
    table = le.service_table(lf, d)
    assert len(table) == 21 and all(table)
    points = le.entry_points(lf)
    assert "control" in points and "service_20" in points and "pm_api" in points and "v86_api" not in points
    assert all(dev and svc < 0x400 for _o, _a, dev, svc, _j in le.service_calls(lf))


def test_reader_fixups_encode_back():
    lf = le.members(_driver())[0][1]
    recs = le.fixup_records(lf)
    assert len(recs) > 100
    assert all(le.encode_fixup(r) == r["raw"] for _pg, r in recs)
    old = {k: (v["obj"], v["target"]) for k, v in lf.fixups().items() if v and v["kind"] == "internal" and v["target"] is not None}
    new = {(o, a): (to, ta) for o, a, _k, to, ta in le.references(lf)}
    assert old == new


def test_writer_round_trip():
    path = _driver()
    lf = le.members(path)[0][1]
    out = le.write_image(le.read_image(lf))
    with open(path, "rb") as f:
        assert out == f.read(), "the file written back is not the file read"
    image = le.read_image(lf)
    image["objects"][0]["flags"] ^= 0x10                                   # change one thing ...
    lf2 = le.LEFile.from_buffer(le.write_image(image))
    assert lf2.objects[0]["flags"] == lf.objects[0]["flags"] ^ 0x10      # ... and only it changes
    assert all(le.object_data(lf2, a) == le.object_data(lf, b) for a, b in zip(lf2.objects, lf.objects))
    assert le.descriptor(lf2)["bytes"] == le.descriptor(lf)["bytes"]


def test_container():
    path = _driver("vmm32.vxd")
    with open(path, "rb") as f:
        data = f.read()
    c = le.container(data)
    assert c and c["kind"] == "W3" and c["members"]
    assert le.pack_directory(c) == data[c["at"]:c["at"] + 0x10 + 16 * len(c["members"])]
    name, lf = le.members(path)[0]
    d = le.descriptor(lf)
    assert name == "VMM" and d["name"] == "VMM" and d["device_id"] == 1
    assert len(le.service_table(lf, d)) == d["service_count"] > 400
    page = data[c["at"]:c["at"] + 0x2000]
    assert le.expand_page(le.compress_page(page), len(page)) == page


# --- the judge ---------------------------------------------------------------------------------
def test_judge_agrees_with_itself_and_catches_a_break():
    from codebreaker.verify.lanes import driver
    path = _driver()
    for label in ("service_0", "service_3", "pm_api"):
        verdict = driver.judge((path, path), (label, label))
        assert verdict[0] == "equivalent", (label, verdict)
    work = settings.state("driver_judge", "test")
    os.makedirs(work, exist_ok=True)
    copy = os.path.join(work, "broken.vxd")
    changed = driver.break_instruction(path, copy, "service_3", 0)
    assert changed, "nothing to break"
    driver._IMAGES.pop(copy, None)
    verdict = driver.judge((path, copy), ("service_3", "service_3"))
    assert verdict[0] == "divergent", (changed, verdict)


def test_run_records_service_calls():
    from codebreaker.verify.lanes import driver
    image = driver.load(_driver())
    v = driver.inputs("control")[0][1]                    # DEVICE_INIT
    r = driver.run(image, image.entries["control"], v["regs"], v["stack"], v["memory"], flags=v["flags"])
    assert r["calls"] and r["insns"] > 10
    f = driver.fingerprint(image, image.entries["control"], v)
    assert f["deps"] and "." in f["deps"][0] and "(" in f["deps"][0], f["deps"][:2]


# --- the builder -------------------------------------------------------------------------------
def test_build_from_source():
    from codebreaker.build import driver as builder
    from codebreaker.verify.lanes import driver
    path = _driver()
    if not builder.tool("wasm") or not builder.tool("wlink"):
        raise Skip("OpenWatcom wasm/wlink not found")
    if not builder.kit_folder():
        raise Skip("the driver kit's include files were not found")
    source = driver.source_folder(path)
    if not source:
        raise Skip("no assembly source for %s" % SMALL)
    r = builder.build(source, original=path, out=settings.state("driver_build", "test"))
    assert r["assembled"] and r["linked"], r["log"][-300:]
    assert set(r["checks"]) == {"objects", "descriptor", "services", "exports", "module"}
    assert r["checks"]["exports"] and r["checks"]["module"], r["notes"]
    built = le.descriptor(le.LEFile.from_file(r["path"]))
    assert built["name"] == "VKD" and built["service_count"] == 21


def test_source_written_from_the_image_builds_back():
    """The source the writer makes from the image has no address in it, assembles, and links
    into a driver with the original's objects, descriptor, services and exports."""
    from codebreaker.build import driver as builder
    from codebreaker.source import driver_writer
    if not builder.tool("wasm") or not builder.tool("wlink"):
        raise Skip("OpenWatcom wasm/wlink not found")
    r = driver_writer.reconstruct(_driver(), settings.state("driver_source", "test"))
    assert r["linked"] and r["checks"] and all(r["checks"].values()), (r.get("notes"), r.get("checks"))
    assert r["instructions"] > 1000 and r["raw"] * 10 < r["instructions"], (r["instructions"], r["raw"])
    text = open(r["source"], encoding="latin1").read()
    assert "VKD_DDB:" in text and "VKD_Get_Version" in text and "dd   offset L" in text
    assert driver_judge_all_equivalent(r["image"])


def driver_judge_all_equivalent(image):
    from codebreaker.verify.lanes import driver
    results = driver.judge_files(image, _driver())
    return results and all(v[0] in ("equivalent", "consistent") for v in results.values())


def test_kit_macro_source():
    """The driver kit's macro form: the descriptor block, the control dispatch, named services."""
    from codebreaker.formats import le
    from codebreaker.source.vxd import disassembly, idiom, run
    image = le.LEFile.from_file(_driver("vkd.vxd"))
    d = disassembly.VxDDisasm(image).run()
    d.v = image
    text = idiom.emit_idiomatic(d)
    assert "include vmm.inc" in text and "Begin_Service_Table VKD" in text
    assert "Begin_Control_Dispatch VKD" in text and "Declare_Virtual_Device" in text
    assert set(run.TOOLS) >= {"idiom", "tree", "masm", "link", "from-source", "owbuild", "finalize"}


def test_kit_macro_source_builds_back():
    """ML and LINK under the emulator make a driver of the original's size, code object identical."""
    from codebreaker.build import dos_tools
    from codebreaker.formats import le
    from codebreaker.source.vxd import from_source
    if not dos_tools.emulator():
        raise Skip("DOSBox-X is not set up")
    image = le.open_vxds(_driver("vkd.vxd"))[0][1]
    r = from_source.build_from_source(image)
    assert r["ok"], r.get("stage")
    assert r["vxd_len"] == r["oracle_len"] and r["obj1_diffs"] == 0 and r["page_map_identity"]


def test_boot_disk_injection():
    """`vxd boot` without the machine: what goes where on an installed disk, written and read back."""
    import struct
    import tempfile
    from test_media import _blank_fat
    from codebreaker.formats import media
    from codebreaker.source.vxd import boot
    vtd = _driver("vtd.vxd")
    d = tempfile.mkdtemp()
    disk = os.path.join(d, "hd.img")
    _blank_fat(disk, 16)
    bundle = bytearray(0x40 + 16 + 4 * 16)                 # an installed VMM32.VXD's directory: W3
    bundle[0:2] = b"MZ"
    struct.pack_into("<I", bundle, 0x3C, 0x40)
    struct.pack_into("<2sHH", bundle, 0x40, b"W3", 0x400, 4)
    for i, name in enumerate((b"VMM", b"VTD", b"VKD", b"INT13")):
        struct.pack_into("<8sII", bundle, 0x50 + 16 * i, name.ljust(8), 0x1000 * (i + 1), 0x100)
    msdos = b"[Paths]\r\nWinDir=C:\\WINDOWS\r\n\r\n[Options]\r\nBootMulti=1\r\nBootLog=0\r\n" + b";x" * 600
    media.put(disk, {"MSDOS.SYS": msdos, "WINDOWS/SYSTEM/VMM32.VXD": bytes(bundle),
                     "WINDOWS/SYSTEM/VMM32/INT13.VXD": b"the machine's own"})
    files = boot.plan(disk, ["vtd"], vmm32=True, retail=True, log=lambda *a: None)
    with open(vtd, "rb") as f:
        assert files["WINDOWS/SYSTEM/VMM32/VTD.VXD"] == f.read()
    assert set(files) == {"WINDOWS/SYSTEM/VMM32/VTD.VXD", "WINDOWS/SYSTEM/VMM32.VXD",   # VKD: from the corpus,
                          "WINDOWS/SYSTEM/VMM32/VKD.VXD", "MSDOS.SYS"}, sorted(files)  # INT13: the disk's own kept
    options = files["MSDOS.SYS"].decode("latin1").split("\r\n")
    assert "BootLog=1" in options and "AutoScan=0" in options and "BootLog=0" not in options
    assert options.index("BootLog=1") == options.index("[Options]") + 1
    rebuilt = boot.plan(disk, ["vtd"], log=lambda *a: None)["WINDOWS/SYSTEM/VMM32/VTD.VXD"]
    assert le.LEFile.from_buffer(rebuilt).ddb()["name"] == "VTD"
    media.put(disk, files)
    v, = media.volumes(disk)
    on = {e.path.upper(): e for e in v.files()}
    for path, data in files.items():
        assert v.read(on[path]) == data, path
    assert v.read(on["WINDOWS/SYSTEM/VMM32/INT13.VXD"]) == b"the machine's own"
    assert boot.guest_path(_driver("vdhcp.386")) == "WINDOWS/SYSTEM/VDHCP.386"
    assert boot.loaded("[0000123F] Loading Vxd = C:\\WINDOWS\\system\\VMM32\\VTD.VXD\n"
                       "[00001241] LoadSuccess = C:\\WINDOWS\\system\\VMM32\\VTD.VXD\n"
                       "[000012AD] SYSCRITINIT  = VTD     \n[000012AD] SYSCRITINITSUCCESS  = VTD     \n"
                       "[000012B4] DEVICEINIT   = VTD     \n", "WINDOWS/SYSTEM/VMM32/VTD.VXD") \
        == "LoadSuccess SYSCRITINITSUCCESS DEVICEINIT"


def test_boot_screens():
    """The boot check's reading of the screen: the desktop, a blue screen, a black one, text."""
    from PIL import Image
    from codebreaker.source.vxd import boot
    desktop = Image.new("RGB", (640, 480), (87, 168, 168))
    desktop.paste((192, 199, 200), (136, 200, 512, 320))            # a message box on it
    assert boot.classify(desktop) == "desktop"
    wallpaper = Image.new("RGB", (640, 480), (0, 0, 120))
    wallpaper.paste((192, 199, 200), (0, 452, 640, 480))            # the taskbar
    assert boot.classify(wallpaper) == "desktop"
    assert boot.classify(Image.new("RGB", (720, 400), (0, 0, 168))) == "blue"
    assert boot.classify(Image.new("RGB", (640, 480), (0, 0, 0))) == "black"
    text = Image.new("RGB", (720, 400), (0, 0, 0))
    text.paste((168, 168, 168), (40, 100, 680, 140))
    assert boot.classify(text) == "text"


def test_boot_bisect():
    """--bisect: halves of a failing set, down to the drivers that fail alone (or together)."""
    from codebreaker.source.vxd import boot
    units = ["drvspacx", "dosmgr", "ifsmgr", "shell", "vtd", "vxdldr", "VMM32"]
    tried = []

    def trial(bad):
        def go(part, tag):
            tried.append(tag)
            return "FAIL(text screen)" if bad(part) else "PASS"
        return go
    assert boot.bisect(units, trial(lambda part: "vtd" in part or "dosmgr" in part), "t", log=lambda *a: None) \
        == ["dosmgr", "vtd"]
    assert boot.bisect(units, trial(lambda part: "shell" in part and "vtd" in part), "t", log=lambda *a: None) \
        == ["shell", "vtd"]                                # (a pair that fails only together)
    assert boot.bisect(units, trial(lambda part: "dosmgr" in part and "vtd" in part), "t", log=lambda *a: None) \
        == units                                           # (split by the first halving)
    assert tried[:3] == ["t_b0", "t_b00", "t_b01"] and "t_b10" in tried     # (a half's halves at once)


def main():
    for fn in (test_table_names_services, test_table_names_are_identifiers, test_table_names_a_real_driver,
               test_reader_descriptor_and_entries, test_reader_fixups_encode_back, test_writer_round_trip,
               test_container, test_judge_agrees_with_itself_and_catches_a_break,
               test_run_records_service_calls, test_build_from_source, test_source_written_from_the_image_builds_back,
               test_kit_macro_source, test_kit_macro_source_builds_back, test_boot_disk_injection,
               test_boot_screens, test_boot_bisect):
        check(fn)
    for name, state, why in RESULTS:
        print("%-5s %s%s" % (state, name, ("  -- " + why) if why else ""))
    failed = sum(1 for _n, s, _w in RESULTS if s == "FAIL")
    print("%d passed, %d failed, %d skipped" % (sum(1 for _n, s, _w in RESULTS if s == "ok"), failed,
                                                sum(1 for _n, s, _w in RESULTS if s == "skip")))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
