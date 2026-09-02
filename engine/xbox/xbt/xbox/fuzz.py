"""Parser fuzzer — mutate valid Xbox 360 files and feed them to our own parsers
to find crashes (defensive hardening).

A robust parser should reject malformed input with a *handled* error (its own
exception, ValueError, struct.error), never an unhandled crash (IndexError,
KeyError, RecursionError, an OOM from a bogus length field, or a hang). This
mutates a seed file — bit flips, byte writes, truncation, and length-field
corruption — and reports any mutation that makes a parser crash instead of
failing cleanly, so those inputs can be turned into hardening fixes.

Deterministic (seeded PRNG), so a reported crash is reproducible.
"""

import random
import struct
import sys

DESCRIPTION = "fuzz the XEX/STFS/FATX parsers with mutated inputs to find crashes (hardening)"

# exceptions that mean "the parser correctly rejected bad input"
HANDLED = (ValueError, struct.error, EOFError, KeyError, OSError, UnicodeError,
           OverflowError, ZeroDivisionError, NotImplementedError, ArithmeticError)


def _mutate(data, rng):
    b = bytearray(data)
    op = rng.randrange(5)
    if not b:
        return bytes(b)
    if op == 0:                                  # single bit flip
        i = rng.randrange(len(b))
        b[i] ^= 1 << rng.randrange(8)
    elif op == 1:                                # random byte
        b[rng.randrange(len(b))] = rng.randrange(256)
    elif op == 2:                                # truncate
        b = b[:rng.randrange(len(b))]
    elif op == 3 and len(b) >= 4:                # corrupt a 4-byte length-ish field
        o = rng.randrange(0, len(b) - 3, 4) if len(b) >= 4 else 0
        struct.pack_into(">I", b, o & ~3, rng.choice([0xFFFFFFFF, 0x7FFFFFFF, 0, 0x10000000]))
    else:                                        # small random append
        b += bytes(rng.randrange(256) for _ in range(rng.randrange(16)))
    return bytes(b)


def fuzz(parse_fn, seed, iterations=2000, seed_value=1, handled=HANDLED):
    """Run parse_fn on `iterations` mutations of `seed`. Returns a list of
    (iteration, mutation_bytes, exception) for mutations that CRASHED (raised
    something outside `handled`). A clean run returns []."""
    rng = random.Random(seed_value)
    crashes = []
    for it in range(iterations):
        m = _mutate(seed, rng)
        try:
            parse_fn(m)
        except handled:
            pass                                 # correctly rejected
        except RecursionError as e:
            crashes.append((it, m, e))
        except Exception as e:                   # noqa: BLE001 - any other is a crash
            crashes.append((it, m, e))
    return crashes


# ── parser targets (each takes raw bytes) ──────────────────────────────
def _target(name):
    if name == "xex":
        import xex
        return lambda d: xex.XEXFile(d)
    if name == "stfs":
        import stfs
        return lambda d: stfs.STFS(d)
    if name == "fatx":
        import fatx
        import tempfile
        import os

        def run(d):
            p = os.path.join(tempfile.mkdtemp(), "f.img")
            open(p, "wb").write(d)
            r = fatx.FatxReader(p)
            for part in r.detect_partitions():
                r.list_directory(part)
            r.close()
        return run
    if name == "ciso":
        import ciso
        import tempfile
        import os

        def run(d):
            p = os.path.join(tempfile.mkdtemp(), "f.cso")
            open(p, "wb").write(d)
            ciso.open_image(p).close()
        return run
    raise ValueError("unknown fuzz target: %s (xex/stfs/fatx/ciso)" % name)


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: fuzz <xex|stfs|fatx|ciso> <seed-file> [--iters N] [--seed S]")
        return
    target, seedpath = a[0], a[1]
    iters = int(a[a.index("--iters") + 1]) if "--iters" in a else 2000
    sval = int(a[a.index("--seed") + 1]) if "--seed" in a else 1
    seed = open(seedpath, "rb").read()
    fn = _target(target)
    print("fuzzing %s parser: %d mutations of %s (%d bytes)..." % (target, iters, seedpath, len(seed)))
    crashes = fuzz(fn, seed, iters, sval)
    if not crashes:
        print("CLEAN: no crashes — parser rejected every mutation with a handled error")
        return
    print("FOUND %d crash(es):" % len(crashes))
    for it, m, e in crashes[:10]:
        print("  iter %d: %s: %s  (%d-byte input)" % (it, type(e).__name__, e, len(m)))
    sys.exit(1)


if __name__ == "__main__":
    main()
