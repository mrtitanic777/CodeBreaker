#!/usr/bin/env python3
"""extract_iso.py -- extract every file from an ISO 9660 image to a directory.

    extract_iso <image.iso> <out-dir>
"""
import os
import sys

import pycdlib


def extract(iso_path, out_dir):
    iso = pycdlib.PyCdlib()
    iso.open(iso_path)
    count = total = 0
    for root, _dirs, files in iso.walk(iso_path="/"):
        for f in files:
            ip = root.rstrip("/") + "/" + f
            clean = f.split(";")[0]                       # strip the ISO version (';1')
            rel = (root.rstrip("/") + "/" + clean).lstrip("/")
            outpath = os.path.join(out_dir, rel)
            os.makedirs(os.path.dirname(outpath), exist_ok=True)
            try:
                iso.get_file_from_iso(outpath, iso_path=ip)
                sz = os.path.getsize(outpath)
                total += sz
                count += 1
                print("%9d  %s" % (sz, rel))
            except Exception as e:
                print("ERR  %s: %s" % (ip, e))
    iso.close()
    print("\n%d files, %d bytes" % (count, total))


def main():
    if len(sys.argv) < 3:
        sys.exit("usage: extract_iso <image.iso> <out-dir>")
    extract(sys.argv[1], sys.argv[2])


if __name__ == "__main__":
    main()
