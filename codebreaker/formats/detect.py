#!/usr/bin/env python3
"""detect.py -- the ONE place that says what kind of executable a file is (formats.dll answers:
native/formats/detect, docs/NATIVE.md step 4).

Every MZ-family executable starts with a DOS header whose field at 0x3C (e_lfanew, 4 bytes)
points at the "new" header; its two-byte signature decides the format.

    signature(path)   -> b"PE" | b"NE" | b"LE" | b"LX" | b"W3" | b"W4" | b"" (MZ, no known new
                         header) | None (not MZ)
    format_of(path)   -> "PE" | "NE" | "LE" | "LX" | "W3" | "W4" | "MZ" | None
    lane(path)        -> "pe" | "ne" | "le" | "dos" | None      (the decompiler lane)
    kind(path)        -> lane(path), and "dos" for a DOS program with no header (.COM, a driver)
    flat(kind)        -> whether a module of that kind is a flat 32-bit image (PE)
    wide_resources(kind) -> whether its resources keep their strings wide (a PE resource tree)
    holds_drivers(path, bundles=(b"W3", b"W4"))  an LE or LX image, or a bundle of them

W3 and W4 are Windows 95's bundles of drivers (VMM32.VXD: VMM and the drivers loaded with it; W4
is a W3 compressed). formats/le.py reads them. No lane reads a bundle whole: its lane is its
DOS stub's, "dos".
    identify(path)    -> [{format, detail, family, command}]: what ANY file is (executables,
                         Xbox containers and assets, disc and disk images, objects, bytecode)
"""
import os

from codebreaker.formats import native


def _by_path(path, *rest):
    return None, (os.fspath(path),) + rest


@native.answered("detect.signature", args_of=_by_path)
def signature(path):
    """New-header signature of the file at `path` (see module docstring), or None when the file
    is unreadable or not an MZ executable."""


@native.answered("detect.format_of", args_of=_by_path)
def format_of(path):
    """'PE' | 'NE' | 'LE' | 'LX' | 'W3' | 'W4' | 'MZ' (plain DOS executable) | None (not an MZ
    executable)."""


@native.answered("detect.lane", args_of=_by_path)
def lane(path):
    """The decompiler lane for a file: 'pe', 'ne', 'le' (VxD; LE and LX), 'dos', or None."""


def kind(path):
    """lane(path), and for a file with no MZ header 'dos' when it is a DOS program all the same
    (a .COM file, a device driver: formats/dos.py tells), else None."""
    out = lane(path)
    if out is None:
        from codebreaker.formats import dos
        out = "dos" if dos.kind(path) else None
    return out


def flat(kind):
    """Whether a module of this kind (as kind() names it) is a flat 32-bit image: one address
    space counted from its image base, run in user mode (PE). A driver (LE) is 32-bit too, but is
    placed object by object and runs privileged."""
    return kind == "pe"


def wide_resources(kind):
    """Whether a module of this kind keeps its resources' strings wide (UTF-16), as a PE resource
    tree does; a 16-bit module's are single-byte."""
    return kind == "pe"


@native.answered("detect.holds_drivers", args_of=_by_path)
def holds_drivers(path, bundles=(b"W3", b"W4")):
    """Whether the file is an LE or LX image (a VxD), or one of the `bundles` of them."""


@native.answered("detect.dos_header")
def dos_header(data):
    """The DOS (MZ) header every one of these formats starts with, by field name, with the size of
    the program it loads and its relocation entries; None when `data` is not an MZ executable.
    For a plain DOS program this is the whole description of the file."""


@native.answered("detect.identify", args_of=_by_path)
def identify(path):
    """What the file at `path` is, from its own bytes. -> [{"format", "detail", "family", "command"}]
    (usually one; empty when nothing is recognised). `command` is the codebreaker command that
    reads it ("" when none does yet). Reads the header and a few fixed offsets, never the file."""
