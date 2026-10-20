# -*- coding: utf-8 -*-
r"""
applet_ping.py -- neutralize the applet document-base "level count" / host ANTI-PIRACY PING.

Early Minecraft (Indev's Minecraft.setLevel) opens a URL to the APPLET's document base to ping a level
count and verify the game is running on minecraft.net, gating the level load on it:

    try {
        BufferedReader var2 = new BufferedReader(new InputStreamReader(
            new URL(getDocumentBase() + "?n=" + user + "&i=" + sess).openStream()));
        Integer.parseInt(var2.readLine());
        var2.close();
        if (getDocumentBase().toString().startsWith("http://www.minecraft.net/") || ...) {
            theWorld = var1;
        }
    } catch (Throwable var3) {
    }

On the console there is NO applet -- URL.openStream() is a NULL-returning stub, so readLine() dereferences
a NULL stream. That is an ACCESS VIOLATION, not a Java exception, so the try/catch does NOT catch it -> a
hard crash the instant setLevel runs. And setLevel runs on the VERY FIRST line of generateLevel
(`setLevel((World*)NULL)`), so pressing "Create" in the new-level menu dies immediately.

The proven OG Indev hand-port (C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftIndev) DROPS the
whole ping and sets the level unconditionally ("applet host check n/a on 360"). This pass migrates that into
the generator so every edition's setLevel loses the ping. Keyed on the STRUCTURE (a try that opens
getDocumentBase()->openStream() and guards a `theWorld = <var>` assignment on the host check), not on names.

convert applies it on the IR before the back end writes (patch(), by tree shape), then checks the output
for a surviving trigger (stranded()). run()/process() are the same fix on C++ text, for `j2ctool <fix> <dir>` on
a tree converted elsewhere; that text form runs over the converted tree (xbox360), BEFORE overlays. Idempotent (after the rewrite
there is no getDocumentBase()->openStream() try left) and fail-loud via run()'s survivor check. Also exposed
as `j2ctool applet-ping <dir>`.
"""

import os
import re

# The setLevel ping: a try that (1) opens getDocumentBase()->...->openStream(), (2) guards a
# `this-><field> = <var>;` assignment inside an if on the host check, (3) is followed by an empty catch.
# `[^{}]*?` keeps each span inside the brace-free prefix so we don't cross into the if/catch bodies wrongly.
_PING = re.compile(
    r'try\s*\{[^{}]*?getDocumentBase\(\)[^{}]*?openStream\b[^{}]*?'
    r'if\s*\([^{}]*?\)\s*\{\s*(this->\w+\s*=\s*\w+\s*;)\s*\}\s*'
    r'\}\s*catch\s*\([^)]*\)\s*\{\s*\}')

_NOTE = ('   // J2X applet-ping-bypass: console has no applet -- getDocumentBase()->openStream() is a '
         'NULL-returning stub and readLine() on it FAULTS (an access violation, NOT caught by try/catch). '
         'Drop the ping and set the level directly (matches the OG Indev hand-port). [generator: applet_ping.py]')

# fail-loud: an openStream() on getDocumentBase() still guarded by a try that survived the pass.
_SURVIVOR = re.compile(r'try\s*\{[^{}]*?getDocumentBase\(\)[^{}]*?openStream\b')


def process(text):
    """Rewrite the setLevel URL ping in one .cpp. Returns (new_text, n_sites)."""
    return _PING.subn(lambda m: m.group(1) + _NOTE, text)


def run(out_dir):
    """Neutralize the applet ping across the converted tree. Returns
    {'files': [{'file','sites'}...], 'sites': N, 'stranded': [files]}. `stranded` = files where a
    getDocumentBase()->openStream() try SURVIVED the rewrite (structure drifted -> fail loud)."""
    files = [f for f in sorted(os.listdir(out_dir)) if f.endswith('.cpp')]
    touched, total, stranded = [], 0, []
    for f in files:
        path = os.path.join(out_dir, f)
        try:
            text = open(path, 'r', encoding='utf-8', errors='replace').read()
        except (IOError, OSError):
            continue
        if 'getDocumentBase' not in text or 'openStream' not in text:
            continue
        new, n = process(text)
        if new != text:
            with open(path, 'w', encoding='utf-8') as fh:
                fh.write(new)
            touched.append({'file': f, 'sites': n})
            total += n
        if _SURVIVOR.search(new):
            stranded.append(f)
    return {'files': touched, 'sites': total, 'stranded': stranded}


# ---- the same fix on the IR, before the back end writes (what convert runs) ------------------------------
def patch(units):
    """The ping on the lowered program: a try that calls getDocumentBase() and openStream(), whose only
    effect is an `if (host check) { this.f = v; }`, with empty catches -> just `this.f = v;`. Returns
    {'files': [{'file','sites'}], 'sites': N, 'stranded': []} like run()."""
    from codebreaker.translate import ir
    from codebreaker.translate import rewrite as rw
    per = {}
    for cls, m in rw.methods(units):
        for ss in rw.stmt_lists(m.body):
            for i, s in enumerate(ss):
                if not isinstance(s, ir.Try) or s.final is not None or s.resources:
                    continue
                calls = set(n.sym.name for n in ir.walk(s.body) if isinstance(n, ir.Call) and n.sym is not None)
                if not {'getDocumentBase', 'openStream'} <= calls:
                    continue
                if any(c.body is not None and c.body.stmts for c in s.catches):
                    continue
                guarded = [x for x in s.body.stmts if isinstance(x, ir.If)]
                if len(guarded) != 1 or guarded[0].orelse is not None:
                    continue
                body = guarded[0].then.stmts if isinstance(guarded[0].then, ir.Block) else [guarded[0].then]
                if len(body) != 1 or not isinstance(body[0], ir.ExprStmt):
                    continue
                a = rw.strip(body[0].expr)
                if not (isinstance(a, ir.Assign) and a.op is None and isinstance(a.target, ir.Select)
                        and isinstance(a.target.obj, ir.This) and isinstance(rw.strip(a.value), ir.Name)):
                    continue
                ss[i] = body[0]                           # set the level directly: no applet on console
                per[cls.name] = per.get(cls.name, 0) + 1
    files = [{'file': c + '.cpp', 'sites': n} for c, n in sorted(per.items())]
    return {'files': files, 'sites': sum(per.values()), 'stranded': []}


def stranded(out_dir):
    """The .cpp files where a getDocumentBase()->openStream() try survived."""
    out = []
    for f in sorted(os.listdir(out_dir)):
        if f.endswith('.cpp'):
            try:
                text = open(os.path.join(out_dir, f), encoding='utf-8', errors='replace').read()
            except (IOError, OSError):
                continue
            if _SURVIVOR.search(text):
                out.append(f)
    return out
