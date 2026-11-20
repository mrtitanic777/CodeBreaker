#!/usr/bin/env python3
"""Makes testdata/sample_encrypted.xcp and sample_plain.xcp (small XCP packages made here) and testdata/xcp_goldens.txt: what `codebreaker.exe crypto xcp_dumper
--ignore --no-backup [-k KEY] <file>` (the translated Python, with Windows' expand.exe) prints and writes for each.

    python make_xcp_samples.py <codebreaker.exe | @file with its path> <work folder>

A package is a cabinet with a header reserve of 0x158 bytes (cbCFFolder 0x1C): the three 28-byte RC4 records at 0x28 (the folder table's), 0x44 (the file
names') and 0x60 (the header's) lie in it, the folder table at 0x180 (an 8-byte folder and its record), the files after it and the data of the one folder
(uncompressed, one block) at the end. See xcp_dumper.hpp for what is encrypted by which record. Three files; the names end in three characters the tool
overwrites with the file's number.
"""
import hashlib
import hmac
import os
import shutil
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
KEY = bytes.fromhex("00112233445566778899AABBCCDDEEFF")


def pattern(n, seed):
    return bytes(((i * 131 + seed * 17 + (i >> 8) * 7) & 0xFF) for i in range(n))


def rc4(key, data):
    s = list(range(256))
    j = 0
    for i in range(256):
        j = (j + s[i] + key[i % len(key)]) & 255
        s[i], s[j] = s[j], s[i]
    out = bytearray()
    i = j = 0
    for byte in data:
        i = (i + 1) & 255
        j = (j + s[i]) & 255
        s[i], s[j] = s[j], s[i]
        out.append(byte ^ s[(s[i] + s[j]) & 255])
    return bytes(out)


def crypt_with_record(record, data):
    """What the tool does: the RC4 key is HMAC-SHA1(KEY, record[:20]); the confounder is run through it first and dropped."""
    key = hmac.new(KEY, record[:20], hashlib.sha1).digest()
    return rc4(key, record[20:28] + data)[8:]


def make_cabinet(files):
    names = [b"data_a.bin", b"data_b.bin", b"data_c.bin"]
    blob = b"".join(files)
    folder_at = 0x180
    files_at = folder_at + 0x24
    table = b""
    offset = 0
    for name, data in zip(names, files):
        table += struct.pack("<IIHHHH", len(data), offset, 0, 0x5A21, 0x6000, 0x20) + name + b"\0"
        offset += len(data)
    data_at = files_at + len(table)
    block = struct.pack("<IHH", 0, len(blob), len(blob)) + blob
    total = data_at + len(block)
    head = b"MSCF" + struct.pack("<IIIIIBBHHHHH", 0, total, 0, files_at, 0, 3, 1, 1, len(files), 4, 0x1234, 0)
    assert len(head) == 0x24
    cab = bytearray(total)
    cab[0:0x24] = head
    cab[0x24:0x28] = struct.pack("<HBB", 0x158, 0x1C, 0)
    cab[folder_at:folder_at + 8] = struct.pack("<IHH", data_at, 1, 0)
    cab[files_at:files_at + len(table)] = table
    cab[data_at:] = block
    return bytes(cab), folder_at, files_at, len(table), data_at


def encrypt(cab, folder_at, files_at, names_len, data_at):
    cab = bytearray(cab)
    records = {0x28: pattern(28, 80), 0x44: pattern(28, 81), 0x60: pattern(28, 82)}
    folder_record = pattern(28, 83)
    cab[folder_at + 8:folder_at + 8 + 28] = folder_record          # the folder's record, in its xenon data
    for at, record in records.items():
        cab[at:at + 28] = record
    cab[data_at:] = crypt_with_record(folder_record, bytes(cab[data_at:]))                                          # (d) the folder's data
    cab[files_at:files_at + names_len] = crypt_with_record(records[0x44], bytes(cab[files_at:files_at + names_len]))   # (c) the file table
    size = 0x24
    cab[folder_at:folder_at + size] = crypt_with_record(records[0x28], bytes(cab[folder_at:folder_at + size]))        # (b) the folder table
    cab[0:0x60] = crypt_with_record(records[0x60], bytes(cab[0:0x60]))                                                # (a) the header
    return bytes(cab)


def run(exe, w, args):
    result = subprocess.run([exe, "crypto", "xcp_dumper", "--ignore", "--no-backup"] + args, capture_output=True, cwd=w)
    return result.stdout.decode("utf-8", "replace").replace("\r\n", "\n").replace(w, "<w>"), result.returncode


def main():
    exe, work = sys.argv[1], os.path.abspath(sys.argv[2]).replace("\\", "/")
    if exe.startswith("@"):
        exe = open(exe[1:]).read().strip()
    files = [pattern(5000, 70), pattern(300, 71), pattern(20000, 72)]
    cab, folder_at, files_at, names_len, data_at = make_cabinet(files)
    plain = cab
    # the tool renames the files whatever the package: names end in the file's number
    encrypted = encrypt(cab, folder_at, files_at, names_len, data_at)
    open(os.path.join(HERE, "sample_plain.xcp"), "wb").write(plain)
    open(os.path.join(HERE, "sample_encrypted.xcp"), "wb").write(encrypted)
    text = ""
    for name, args_for in (("encrypted", ["-k", KEY.hex()]), ("plain", [])):
        w = os.path.join(work, name).replace("\\", "/")
        shutil.rmtree(w, ignore_errors=True)
        os.makedirs(w)
        shutil.copy(os.path.join(HERE, "sample_%s.xcp" % name), os.path.join(w, "sample.xcp"))
        out, code = run(exe, w, args_for + [w + "/sample.xcp"])
        text += "== %s\n%sexit=%d\n" % (name, out, code)
        for rel in sorted(os.listdir(w)):
            data = open(os.path.join(w, rel), "rb").read()
            text += "file %s %d %s\n" % (rel, len(data), hashlib.sha1(data).hexdigest())
    open(os.path.join(HERE, "xcp_goldens.txt"), "w", newline="\n").write(text)
    sys.stdout.write(text)


if __name__ == "__main__":
    main()
