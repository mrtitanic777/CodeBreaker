#!/usr/bin/env python3
"""Makes testdata/xenon_tools_goldens.txt: what `codebreaker.exe xex-basic | xenon-config | switch-tables` (the translated Python) write for
LoganMinecraftTest.xex of C:/Nexia360/Games (the Minecraft test program), as "file <name> <size> <sha1>" lines after "== case" and the stdout/exit.

    python make_xenon_tools_goldens.py <codebreaker.exe | @file with its path> <work folder>
"""
import hashlib
import os
import shutil
import subprocess
import sys

XEX = "C:/Nexia360/Games/LoganMinecraftTest/LoganMinecraftTest.xex"
HERE = os.path.dirname(os.path.abspath(__file__))


def record(w, rel):
    data = open(os.path.join(w, rel), "rb").read()
    return "file %s %d %s\n" % (rel, len(data), hashlib.sha1(data).hexdigest())


def run(exe, w, name, args, outputs):
    result = subprocess.run([exe] + [a.replace("{w}", w) for a in args], capture_output=True, cwd=w)
    out = result.stdout.decode("utf-8", "replace").replace("\r\n", "\n").replace(w, "<w>")
    text = "== %s\n%sexit=%d\n" % (name, out, result.returncode)
    for rel in outputs:
        text += record(w, rel) if os.path.exists(os.path.join(w, rel)) else "file %s missing\n" % rel
    return text


def main():
    exe, w = sys.argv[1], os.path.abspath(sys.argv[2]).replace("\\", "/")
    if exe.startswith("@"):
        exe = open(exe[1:]).read().strip()
    shutil.rmtree(w, ignore_errors=True)
    os.makedirs(w)
    text = run(exe, w, "xex_basic", ["xex-basic", XEX, "{w}/basic/default.xex"], ["basic/default.xex"])
    text += run(exe, w, "xenon_config", ["xenon-config", "{w}/basic/default.xex", "{w}/cfg.toml", "--out-dir", "ppc", "--file-path", "default.xex"], ["cfg.toml"])
    text += run(exe, w, "switch_tables", ["switch-tables", "{w}/basic/default.xex", "{w}/basic/switch_tables.toml"],
                ["basic/switch_tables.toml", "basic/switch_tables.analysed.toml"])
    text += run(exe, w, "xenon_config_with_tables", ["xenon-config", "{w}/basic/default.xex", "{w}/cfg2.toml", "--out-dir", "ppc.new", "--switch-tables", "switch_tables.toml",
                                                    "--file-path", "default.xex"], ["cfg2.toml"])
    open(os.path.join(HERE, "xenon_tools_goldens.txt"), "w", newline="\n").write(text)
    sys.stdout.write(text)


if __name__ == "__main__":
    main()
