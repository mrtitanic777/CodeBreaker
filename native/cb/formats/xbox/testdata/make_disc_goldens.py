#!/usr/bin/env python3
"""Makes testdata/disc_goldens.txt: what `codebreaker.exe xbox-disc ...` (the Python with formats.dll's disc reader) prints and writes for the volume and the
Games on Demand package that disc_command_test.cpp builds.

    1. run the test that writes the samples:   CB_DISC_SAMPLE_OUT=<folder> cb_tests_formats --filter write_the_samples_for_the_python_goldens
    2. python make_disc_goldens.py <codebreaker.exe | @file with its path> <folder>

A case is "== name", the stdout lines, "exit=N", then what it wrote: for a folder one "<path> <size> <sha1>" line per file (sorted), for a file one
"file <name> <size> <sha1>" line.
"""
import hashlib
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))


def manifest(folder):
    lines = []
    for root, _dirs, files in os.walk(folder):
        for name in files:
            path = os.path.join(root, name)
            data = open(path, "rb").read()
            lines.append("%s %d %s\n" % (os.path.relpath(path, folder).replace("\\", "/"), len(data), hashlib.sha1(data).hexdigest()))
    return "".join(sorted(lines))


def run(exe, w, name, args, written=None):
    result = subprocess.run([exe, "xbox-disc"] + [a.replace("{w}", w) for a in args], capture_output=True)
    out = result.stdout.decode("utf-8", "replace").replace("\r\n", "\n").replace(w, "<w>")
    text = "== %s\n%sexit=%d\n" % (name, out, result.returncode)
    if written:
        path = os.path.join(w, written)
        if os.path.isdir(path):
            text += manifest(path)
        elif os.path.isfile(path):
            data = open(path, "rb").read()
            text += "file %s %d %s\n" % (os.path.basename(path), len(data), hashlib.sha1(data).hexdigest())
    return text


def main():
    exe, folder = sys.argv[1], os.path.abspath(sys.argv[2]).replace("\\", "/")
    if exe.startswith("@"):
        exe = open(exe[1:]).read().strip()
    data_dir = None
    for root, dirs, _files in os.walk(os.path.join(folder, "pkg")):
        for d in dirs:
            if d.endswith(".data"):
                data_dir = os.path.join(root, d).replace("\\", "/")
    assert data_dir, "no .data folder under pkg"
    text = ""
    text += run(exe, folder, "files_iso", ["files", "{w}/four.iso"])
    text += run(exe, folder, "files_package", ["files", data_dir])
    for sub in ("out", "out2"):
        shutil.rmtree(os.path.join(folder, sub), ignore_errors=True)
    text += run(exe, folder, "read_iso", ["read", "{w}/four.iso", "default.xex", "{w}/out/x.xex"], "out/x.xex")
    text += run(exe, folder, "read_missing", ["read", "{w}/four.iso", "nothing.xex", "{w}/out/z.xex"], "out/z.xex")
    shutil.rmtree(os.path.join(folder, "out"), ignore_errors=True)
    text += run(exe, folder, "unpack_iso", ["unpack", "{w}/four.iso", "{w}/out"], "out")
    text += run(exe, folder, "unpack_package", ["unpack", data_dir, "{w}/out2"], "out2")
    # the package's data folder is under the sample folder: the files it names are those of the volume
    open(os.path.join(HERE, "disc_goldens.txt"), "w", newline="\n").write(text)
    sys.stdout.write(text)


if __name__ == "__main__":
    main()
