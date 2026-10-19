# -*- coding: utf-8 -*-
"""The libraries a game is judged against (settings.json `java_libs`: LWJGL, the sound system) and the
stand-ins ahead of them (probe_shims/). FIXED BUG probe-no-libraries: every class that reached LWJGL or
the sound system failed to compile on the Java side (38 of Alpha's); then everything that reached
Minecraft still failed on CodecMus, which extends Minecraft's own build of CodecJOrbis (its `url` and
`urlConnection` fields and openInputStream()), not the public jar's."""

import os

import pytest

from codebreaker.java import probe
from codebreaker.verify import java

CODEC_MUS = '''package net.minecraft.src;

import java.io.IOException;
import java.io.InputStream;
import paulscode.sound.codecs.CodecJOrbis;

public class CodecMus extends CodecJOrbis {
	protected InputStream openInputStream() throws IOException {
		return this.urlConnection.getInputStream();
	}
}
'''


def test_no_libraries_no_shims(tmp_path, monkeypatch):
    monkeypatch.setattr(probe, 'libraries', lambda: [])
    assert probe.shims(str(tmp_path), 'javac') == ''
    assert probe._cp(str(tmp_path), 'a') == 'a'


@pytest.mark.skipif(not (java.jdk()[0] and any('soundsystem' in os.path.basename(j) for j in probe.libraries())),
                    reason='needs a JDK and the sound system in java_libs')
def test_codec_mus_compiles_against_the_stand_in(tmp_path):
    javac = java.jdk()[0]
    shim = probe.shims(str(tmp_path), javac)
    assert shim and os.path.isfile(os.path.join(shim, 'paulscode', 'sound', 'codecs', 'CodecJOrbis.class'))
    cp = probe._cp(str(tmp_path))
    assert cp.split(os.pathsep)[0] == shim                    # ahead of the real codec jar
    src = tmp_path / 'src' / 'net' / 'minecraft' / 'src'
    src.mkdir(parents=True)
    (src / 'CodecMus.java').write_text(CODEC_MUS, encoding='utf-8')
    import subprocess
    r = subprocess.run([javac, '-cp', cp, '-d', str(tmp_path / 'out'), str(src / 'CodecMus.java')],
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    assert r.returncode == 0, r.stdout.decode('utf-8', 'replace')[:400]
