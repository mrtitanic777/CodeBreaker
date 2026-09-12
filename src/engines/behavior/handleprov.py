#!/usr/bin/env python3
r"""handleprov.py -- construct a VALID GDI/USER handle so handle-gated functions trace (Phase C).

gdi32/user32 functions validate their handle before doing anything and bail CLR_INVALID on a fake
one, which is why they were untraceable (~8% reconstructed). Reverse-engineering the GDI handle
manager (from gdi32 SetBkColor + its lookup) gives the exact recipe:

  lookup(handle):                       ; gdi32 sub at the head of every handle function
    slot   = handle + [G1]              ; G1 = [0xbff4b090], the handle-table base
    if byte[slot+2] & 0x40:             ; indirection flag
        objword = [ word[slot] + [G2] ] ; G2 = [0xbff4b094]
    else:
        objword = word[slot]
    object = [G1] + objword
  validate: word[object+2] (high byte &0x5f) in [0x4f4d..0x4f51]   ; GDI object magic ('MO'..)

So a valid handle is an offset into a table at G1 whose slot points (via objword) at an object
whose +2 word is a GDI magic. This builds that memory image. With it, the function executes its
REAL path past validation instead of erroring -- the prerequisite for the struct/gptr lanes to
then model the object (DC attribute) fields.

NOTE: validation is the cracked keystone. Observing the deepest attribute writes may need the DC
object's internal attribute-block pointer seeded too (the object carries a pointer to the real
attribute storage); this provides the valid-handle image those lanes build on.
"""
import struct

# GDI handle-manager globals (gdi32 4.00.950). Exposed so callers can override per-build.
G1_ADDR = 0xBFF4B090          # [G1] = handle-table base
G2_ADDR = 0xBFF4B094          # [G2] = secondary base (indirection path)
TABLE = 0x205000              # where we place the synthetic handle table (mapped scratch region)
OBJW = 0x800                  # object offset within the table region
OBJ = TABLE + OBJW
HANDLE = 0x10                 # the synthetic handle value (a slot offset)
MAGIC = 0x4F4D                # GDI object type magic ('MO') -> passes the [0x4f4d..0x4f51] check


def valid_handle(obj_size=0x400, obj_fields=None, g1=G1_ADDR, g2=G2_ADDR):
    """Return (handle_value, mem_init) constructing a valid GDI handle. The object is zeroed with
    the type magic at +2; pass obj_fields={off: 4-byte-int} to seed internal pointers/fields (e.g.
    an attribute-block pointer) so deeper writes land in a controlled buffer."""
    obj = bytearray(obj_size)
    obj[2:4] = struct.pack("<H", MAGIC)
    for off, val in (obj_fields or {}).items():
        if 0 <= off <= obj_size - 4:
            obj[off:off + 4] = struct.pack("<I", val & 0xFFFFFFFF)
    mem = [
        (g1, struct.pack("<I", TABLE)),                 # [G1] -> table base
        (g2, struct.pack("<I", 0)),                     # [G2] -> 0 (no indirection)
        (TABLE + HANDLE, struct.pack("<HBB", OBJW, 0x00, 0)),   # slot: objword, flag clear
        (OBJ, bytes(obj)),
    ]
    return HANDLE, mem


def trace_handle_fn(module, func, extra_args=(), obj_fields=None, insn_cap=6000):
    """Trace a handle-gated function with arg0 = a valid handle. extra_args are the remaining
    scalar args. Returns the TraceRecord; the object region is [OBJ, OBJ+size)."""
    from src.engines.behavior import tracer
    h, mem = valid_handle(obj_fields=obj_fields)
    args = [h] + list(extra_args)
    return tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap, mem_init=mem)
