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
# --- the builder -------------------------------------------------------------------------------
def test_compare_share_ignores_patched_places_outside_the_object():
    """The share of equal bytes is over the bytes outside the places the loader patches. A patched place (four bytes for
    each fix-up, even a 16-bit one) can run past the end of its object: those bytes are no part of it, so they must not
    shorten the object (it once counted 4 - 5 = -1 bytes of a 4-byte object)."""
    from codebreaker.build import driver as builder
    equal, total = builder._equal_share(b"abcd", b"abXd", {1, 3, 4, 5, -1})
    assert (equal, total) == (1, 2), (equal, total)                 # bytes 0 and 2 are compared; 1 and 3 are patched
    assert builder._equal_share(b"abcd", b"abcd", set()) == (4, 4)
    assert builder._equal_share(b"abcd", b"abcd", {0, 1, 2, 3}) == (0, 0)
    assert builder._equal_share(b"abcd", b"abcd", {6, 7, 8, 9}) == (4, 4)    # nothing of the object is patched
    assert builder._equal_share(b"", b"", {0, 1}) == (0, 0)


def main():
    for fn in (test_table_names_services, test_table_names_are_identifiers, test_table_names_a_real_driver,
               test_reader_descriptor_and_entries, test_reader_fixups_encode_back, test_writer_round_trip,
               test_container, test_compare_share_ignores_patched_places_outside_the_object):
        check(fn)
    for name, state, why in RESULTS:
        print("%-5s %s%s" % (state, name, ("  -- " + why) if why else ""))
    failed = sum(1 for _n, s, _w in RESULTS if s == "FAIL")
    print("%d passed, %d failed, %d skipped" % (sum(1 for _n, s, _w in RESULTS if s == "ok"), failed,
                                                sum(1 for _n, s, _w in RESULTS if s == "skip")))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
