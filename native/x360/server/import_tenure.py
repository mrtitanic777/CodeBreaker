"""Imports the gamercard tenure images into the server's content subsystem (live_server.py `content_*`).

The console's tenure numbers (crown1..crown19.png, 88x40) and milestone banners (milestone1..3.png) ship inside the
system update's system content package FFFE07DF00000001 (a PIRS package that also carries firmware blobs). XAM mounts it
as TENURE: and reads crown<N>.png, N clamped to 19, and milestone<N>.png, N clamped to 3 (xam 0x81729310/0x817293C0).
These are Microsoft's files: this copies them from the user's own system update into content/tenure (gitignored),
never into the repository.

    python -I import_tenure.py "<...>/SystemUpdate/$SystemUpdate/FFFE07DF00000001" [out_dir]
"""
import os
import re
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tools", "scripts"))
from stfs_extract import Stfs  # noqa: E402

WANTED = re.compile(r"^(crown([1-9]|1[0-9])|milestone[1-3])\.png$")


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(os.path.dirname(os.path.abspath(__file__)), "content", "tenure")
    stfs = Stfs(open(sys.argv[1], "rb").read())
    os.makedirs(out, exist_ok=True)
    got = 0
    for e in stfs.entries():
        if not e["dir"] and WANTED.match(e["name"]):
            data = stfs.read(e)
            if data[:8] != b"\x89PNG\r\n\x1a\n":
                sys.exit("%s is not a PNG: wrong package?" % e["name"])
            with open(os.path.join(out, e["name"]), "wb") as f:
                f.write(data)
            got += 1
    if got != 22:
        sys.exit("found %d of the 22 tenure images: wrong package?" % got)
    print("imported crown1..19.png and milestone1..3.png into", out)


if __name__ == "__main__":
    main()
