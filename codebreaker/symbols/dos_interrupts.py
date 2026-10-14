"""dos_interrupts.py -- names for the services a real-mode program asks of DOS and the BIOS.

    from codebreaker.symbols import dos_interrupts
    s = dos_interrupts.name(0x21, ah=0x3D)        .name "DOS_OpenFile"  .arguments "AL = access mode, ..."
    s = dos_interrupts.name(0x2F, 0x16, 0x00)     .name "Multiplex_WindowsEnhancedModeCheck"
    s.exact        False when only the interrupt (or only its group) could be named
    text = dos_interrupts.annotate(text, interrupts)    C text with each call's service noted

A program calls the system with `int n`; which service it wants is a number in AH (for some a
second one in AL, for the mouse the whole of AX). The table (dos_interrupts.json, written for
this project from the documented interfaces: interrupt -> AH -> name and register arguments,
with an "al" table for services that take a subfunction) covers DOS (21h, 20h-2Fh), the BIOS
(10h-1Ah), the multiplex interrupt (2Fh), the mouse (33h) and expanded memory (67h).

The decompiler shows an interrupt as `swi(0x21)` without the registers, so the service number
comes from the machine code (codebreaker.analyze.dos_functions finds the constant loaded before
each `int`). annotate() writes what is known in front of each function, and inline where every
call of that interrupt in the function asks for the same service.
"""
import json
import os
import re

_TABLE = None


class Service(object):
    def __init__(self, name, arguments, exact):
        self.name, self.arguments, self.exact = name, arguments, exact

    def __repr__(self):
        return "Service(%s%s)" % (self.name, "" if self.exact else ", not exact")


def table():
    """{interrupt number: entry of dos_interrupts.json}, read once."""
    global _TABLE
    if _TABLE is None:
        with open(os.path.splitext(os.path.abspath(__file__))[0] + ".json", encoding="utf-8") as f:
            _TABLE = {int(k, 16): v for k, v in json.load(f).items()}
    return _TABLE


def name(number, ah=None, al=None):
    """The service an `int number` asks for, given what is known of AH and AL. -> Service.
    Always answers: an interrupt outside the table is `int_NNh`, a service outside it
    `<group>_AH_NNh`; `exact` says whether the service itself was named."""
    entry = table().get(number)
    if entry is None:
        return Service("int_%02Xh" % number, "", False)
    group = entry["name"]
    if "services" not in entry:
        return Service(group, entry.get("arguments", ""), True)
    if entry.get("select") == "AX":                           # the mouse: the number is all of AX
        ah = al if ah == 0 else None
    if ah is None:
        return Service(group, "service number not a constant here", False)
    sub = entry.get("al", {}).get("%02X" % ah, {}).get("%02X" % al) if al is not None else None
    hit = sub or entry["services"].get("%02X" % ah)
    if hit is None:
        return Service("%s_AH_%02Xh" % (group, ah), "", False)
    return Service("%s_%s" % (group, hit[0]), hit[1], True)


def describe(interrupt):
    """One line for an Interrupt of dos_functions: `int 21h AH=3Dh  DOS_OpenFile(AL = ...)`."""
    s = name(interrupt.number, interrupt.ah, interrupt.al)
    regs = "".join(" %s=%02Xh" % (r, v) for r, v in (("AH", interrupt.ah), ("AL", interrupt.al)) if v is not None)
    return "int %02Xh%s  %s%s" % (interrupt.number, regs, s.name, "(%s)" % s.arguments if s.arguments else "")


_SWI = re.compile(r"\bswi\(0x([0-9a-fA-F]+)\)")


def annotate(text, interrupts):
    """A function's C text with its interrupt calls named. `interrupts` are those of that
    function, in address order. A comment in front lists each; `swi(0xNN)` gets its service
    inline only where all the function's calls of that interrupt ask for the same one."""
    if not interrupts:
        return text
    by_number = {}
    for i in interrupts:
        by_number.setdefault(i.number, set()).add(name(i.number, i.ah, i.al).name)

    def inline(m):
        names = by_number.get(int(m.group(1), 16), ())
        return "%s /* %s */" % (m.group(0), next(iter(names))) if len(names) == 1 else m.group(0)
    lines = ["/* interrupt calls, in address order:"] + ["     %05X  %s" % (i.address, describe(i)) for i in interrupts]
    return "\n".join(lines) + " */\n" + _SWI.sub(inline, text)


def main(argv):
    """python -m codebreaker dos-interrupts [<interrupt> [<AH> [<AL>]]]

    Name a service (numbers in hex), or with no arguments say how many are in the table."""
    if not argv:
        t = table()
        services = sum(len(e.get("services", {})) + sum(len(x) for x in e.get("al", {}).values()) or 1 for e in t.values())
        print("%d interrupts, %d services" % (len(t), services))
        return 0
    s = name(*[int(x, 16) for x in argv[:3]])
    print("%s%s%s" % (s.name, "(%s)" % s.arguments if s.arguments else "", "" if s.exact else "   [not exact]"))
    return 0
