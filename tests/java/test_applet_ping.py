# -*- coding: utf-8 -*-
"""Regressions for applet_ping.py -- neutralizing the setLevel document-base anti-piracy URL ping that
crashes on console (URL.openStream() is a NULL stub -> readLine() faults, uncatchable)."""

import os
import tempfile

from codebreaker.java.console import applet_ping


_SETLEVEL = (
    'void Minecraft::setLevel(World* var1)\n{\n'
    '\tif(this->theWorld != NULL) {\n\tthis->theWorld->setLevel();\n\t}\n'
    '\ttry {\n'
    '\tBufferedReader* var2 = new BufferedReader(new InputStreamReader((new URL((JCat() + '
    'this->mcApplet->getDocumentBase() + "?n=" + this->session->username + "&i=" + '
    'this->session->sessionId)))->openStream()));\n'
    '\tInteger::parseInt(var2->readLine());\n'
    '\tvar2->close();\n'
    '\tif(this->mcApplet->getDocumentBase()->toString().startsWith("http://www.minecraft.net/") || '
    'this->mcApplet->getDocumentBase()->toString().startsWith("http://minecraft.net/")) {\n'
    '\tthis->theWorld = var1;\n'
    '\t}\n'
    '\t} catch (Throwable*) {\n'
    '\t}\n'
    '\tif(var1 != NULL) {\n\tvar1->load();\n\t}\n}\n'
)


def test_ping_dropped_level_set_directly():
    new, n = applet_ping.process(_SETLEVEL)
    assert n == 1
    # the network CODE is gone (check code-specific strings the bypass COMMENT doesn't itself contain)
    assert 'new URL(' not in new
    assert 'InputStreamReader' not in new
    assert 'var2->readLine' not in new
    assert 'var2->close' not in new
    assert 'catch (' not in new and 'catch(' not in new    # the catch STATEMENT is gone
    # the level is still assigned (unconditionally now)
    assert 'this->theWorld = var1;' in new
    # and the rest of setLevel (the real load path) is untouched
    assert 'var1->load();' in new


def test_idempotent():
    once, _ = applet_ping.process(_SETLEVEL)
    twice, n = applet_ping.process(once)
    assert n == 0 and twice == once


def test_run_reports_and_no_survivor():
    d = tempfile.mkdtemp()
    with open(os.path.join(d, 'Minecraft.cpp'), 'w', encoding='utf-8') as f:
        f.write(_SETLEVEL)
    # a file with no ping is left alone
    with open(os.path.join(d, 'Other.cpp'), 'w', encoding='utf-8') as f:
        f.write('void Other::f() {}\n')
    res = applet_ping.run(d)
    assert res['sites'] == 1
    assert res['stranded'] == []          # no getDocumentBase()->openStream() try survived
    assert any(t['file'] == 'Minecraft.cpp' for t in res['files'])


def test_unrelated_getdocumentbase_untouched():
    # the applet's own getDocumentBase() use (no openStream ping try) must NOT be rewritten
    src = ('void MinecraftApplet::init() {\n'
           '\tthis->mc->minecraftUri = this->getDocumentBase()->getHost();\n}\n')
    new, n = applet_ping.process(src)
    assert n == 0 and new == src
