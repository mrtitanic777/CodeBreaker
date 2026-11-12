"""Regenerates testdata/config/<case>/* from the Python platform_config module (the files `emit` writes and the JSON
`save_config` writes) for a few configs. The goldens are REVIEWED after generation, not trusted.

Intended differences from the Python, applied here so the C++ can compare exactly: the files are written with "\\n" line
ends (Python's text mode wrote CRLF on Windows), and the report says "yes"/"no" where Python printed True/False.
Run from the repository root:  python native/cb/java/console/testdata/gen_config_golden.py
"""
import copy
import json
import os
import sys

sys.path.insert(0, os.getcwd())
from codebreaker.java.console import platform_config as pc  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "config")


class Sdk:
    name = "Xbox 360 SDK (XDK)"
    root = "C:\\XDK"
    source = "env:XEDK"


def write(case, name, text):
    d = os.path.join(OUT, case)
    os.makedirs(d, exist_ok=True)
    with open(os.path.join(d, name), "w", encoding="utf-8", newline="") as f:
        f.write(text if text.endswith("\n") else text + "\n")


def md_fix(text):
    return text.replace("Invert Y: **False**", "Invert Y: **no**").replace("Invert Y: **True**", "Invert Y: **yes**")


cases = {}
cases["default"] = (pc.default_config(), None, None)

c = pc.default_config()
c["controller"].update(layout="legacy_southpaw", invert_y=True, look_sensitivity=1.25, stick_deadzone=0.3,
                       trigger_threshold=0.5)
c["controller"]["binds"]["jump"] = "X"
c["keyboard"] = "osk"
c["storage"] = "rawhdd"
c["title_id"] = "FFFE07D1"
c["subsystems"].update(audio="silent", achievements="off")
cases["legacy_southpaw_osk_rawhdd"] = (c, None, None)

c = pc.default_config()
c["keyboard"] = "stub"
c["storage"] = "memory"
c["controller"]["layout"] = "southpaw"
del c["controller"]["binds"]["chat"]            # an action with no bind prints NONE
cases["stub_memory"] = (c, None, None)

cases["with_sdk"] = (pc.default_config(), Sdk(), ["XContentClose", "XShowKeyboardUI"])

for case, (cfg, sdk, unverified) in cases.items():
    write(case, "ControllerConfig.h", pc._controller_header(cfg))
    kh, kc = pc._kernel_files(cfg)
    write(case, "KernelConfig.h", kh)
    write(case, "Kernel.cpp", kc)
    write(case, "PLATFORM_CHOICES.md", md_fix(pc._choices_md(cfg, sdk_info=sdk, unverified=unverified)))
    # what save_config writes (json.dump, indent 2, sorted keys), with a closing newline added by write()
    write(case, "platform.config.json", json.dumps(copy.deepcopy(cfg), indent=2, sort_keys=True))
    print(case)
