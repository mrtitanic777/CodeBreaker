"""early_c_check.py -- read a function of a program built with the C compiler of 1983-84, and
put the text to that compiler.

    python -m codebreaker invert --tool early-c-check IMAGE OFFSET LENGTH     (offset in hex)
    python -m codebreaker invert --tool early-c-check IMAGE --batch LIST.json  [{name, off, size}]

    from codebreaker.invert import early_c_check
    text, same, note = early_c_check.reconstruct(image_bytes, offset, size)

The model (early_c.py) reads the function into C; the text is compiled by the compiler itself --
Microsoft C 2.03, MC1 and MC2 (settings: kits.msc203, the folder holding them) -- and the
function's bytes are compared with the original's, outside the places the linker fills in: the
fix-ups of the object file and the distance of every call or jump that leaves the function.
`same` is None when the text was not compiled (verify=False).
"""
import json
import os
import re
import sys

from codebreaker.invert import early_c


def external_branches(code):
    """1 for each byte of a call's or jump's distance whose target is outside `code`."""
    mask = bytearray(len(code))
    for ins in early_c._MD.disasm(code, 0):
        mn, ops = ins.mnemonic, ins.op_str
        if (mn in ("call", "jmp") or mn.startswith(("j", "loop"))) and ops.startswith("0x"):
            try:
                target = int(ops, 16)
            except ValueError:
                continue
            if 0 <= target < len(code):
                continue
            first = code[ins.address]
            width = 1 if first == 0xEB or 0x70 <= first <= 0x7F else 2 if ins.size >= 4 or first in (0xE8, 0xE9) else 1
            for k in range(width):
                mask[ins.address + ins.size - width + k] = 1
    return mask


def source(code, name):
    """The C text the model reads `code` as, or raises."""
    recover = early_c.recover if os.environ.get("NO_CFG") else early_c.recover_cfg
    size, params, statements, kind, local = recover(code)
    globals_ = sorted(set(re.findall(r"g_[0-9a-f]+", " ".join(statements))))
    signature = "%s(%s)" % (name, ", ".join(params))
    declared = ("%s %s;" % (kind, ", ".join(params))) if params else ""
    ordered, _ = early_c.framemodel.emit_local_decls(local or [], size, None, early_c.detect_copies(statements))
    locals_ = "".join(early_c.framemodel.fmt_decl(*d) + " " for d in ordered)
    statements = ([locals_.strip()] if locals_ else []) + statements
    head = ("int %s;\n" % ", ".join(globals_)) if globals_ else ""
    return "%s%s\n%s\n{\n%s\n}\n" % (head, signature, declared, "\n".join("    " + s for s in statements))


def reconstruct(image, off, size, verify=True):
    """-> (C text or None, True/False/None, what happened)"""
    from codebreaker.invert import compilers, omf
    code = bytes(image[off:off + size])
    if len(code) < 4:
        return None, False, "too small to be a function"
    name = "f%05x" % off
    try:
        text = source(code, name)
    except Exception as e:
        return None, False, "the model failed: %s" % e
    if not verify:
        return text, None, "not compiled"
    obj = compilers.mc_compile(text)
    if not obj:
        return text, False, "does not compile"
    functions = omf.functions(obj)
    built = next((v for k, v in functions.items() if k.upper().lstrip("_") == name.upper()), None) or \
        (next(iter(functions.values())) if functions else None)
    if built is None:
        return text, False, "no function in the object file"
    got, fixed = built
    mask = bytearray(len(got))
    for i in fixed:
        if 0 <= i < len(mask):
            mask[i] = 1
    for i, m in enumerate(external_branches(got)):
        if m:
            mask[i] = 1
    same = len(got) == len(code) and all(mask[i] or got[i] == code[i] for i in range(len(code)))
    return text, same, ("identical (%d bytes the linker fills in)" % sum(mask)) if same else \
        "different (%d bytes against %d)" % (len(got), len(code))


def main():
    with open(sys.argv[1], "rb") as f:
        image = f.read()
    if "--batch" in sys.argv:
        with open(sys.argv[sys.argv.index("--batch") + 1]) as f:
            work = json.load(f)
        same = 0
        for item in work:
            _text, ok, note = reconstruct(image, int(item["off"], 16), item["size"])
            same += ok is True
            print("  %-16s %-34s %s" % (item["name"], note, "OK" if ok else ""))
        print("\n%d of %d functions give the original's bytes back" % (same, len(work)))
    else:
        text, ok, note = reconstruct(image, int(sys.argv[2], 16), int(sys.argv[3]))
        print("same:", ok, "|", note)
        print()
        print(text)


if __name__ == "__main__":
    main()
