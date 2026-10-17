"""sanitize.py -- what the port does wrong only when it runs: found by AddressSanitizer on the verify cases, mended.

An old program may write past the end of an array where its own linker put the next variable - harmless there
(what it overwrote it did not read again, or read as it meant to), memory of something else here. The compiler
cannot see it; a run can. The port is built with -fsanitize=address, every verify case run, and each overflow the
sanitizer reports becomes a rule of the port:

  a global array written (or read) past its end   the array given room for what the original reached
"""
import glob
import math
import os
import re

GLOBAL = re.compile(r"AddressSanitizer: global-buffer-overflow.*?\n(?P<rw>READ|WRITE) of size (?P<sz>\d+).*?"
                    r"is located (?P<after>\d+) bytes after global variable '(?P<var>\w+)'.*?of size (?P<size>\d+)", re.S)


def runtime_dir(cc):
    """where clang's sanitizer runtime is, beside the compiler (Windows: a DLL the program needs on its path)"""
    root = os.path.dirname(os.path.dirname(os.path.abspath(cc)))
    for p in sorted(glob.glob(os.path.join(root, "lib", "clang", "*", "lib", "windows")), reverse=True):
        if glob.glob(os.path.join(p, "clang_rt.asan_dynamic*.dll")):
            return p
    return None


def mend(port, out, reports):
    """the rules from the sanitizer's reports; -> what was mended ([(variable, why)])"""
    done = []
    for case, text in reports:
        for m in GLOBAL.finditer(text):
            var = m.group("var")
            decl = None
            for f, t in out.items():
                d = re.search(r"(?m)^\s*(?:static\s+)?[\w ]+?[\s*]+%s\s*\[\s*(\d+)\s*\]" % re.escape(var), t)
                if d:
                    decl = int(d.group(1))
                    break
            if not decl or port.grown.get(var, 0) > decl:
                continue                                    # (grown already, this round)
            elem = int(m.group("size")) // decl
            extra = math.ceil((int(m.group("after")) + int(m.group("sz"))) / elem)
            port.grown[var] = decl + extra
            port.decl_rules.append((r"(?m)^(\s*(?:static\s+)?[\w ]+?[\s*]+%s\s*\[)\s*%d\s*\]" % (re.escape(var), decl),
                                    r"\g<1>%d]" % (decl + extra)))
            why = "%s[%d] %s (case '%s'): the original reached into the variable its linker put after it" % (
                var, decl + extra - 1, "written" if m.group("rw") == "WRITE" else "read", case)
            port.mended["array reached past its end (as the original did, harmlessly there): room made for it"].append(why)
            done.append((var, why))
    return done
