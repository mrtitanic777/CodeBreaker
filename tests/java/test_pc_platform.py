# -*- coding: utf-8 -*-
"""The PC platform (pc_opengl): SDL2 + real OpenGL behind LWJGL's API (codebreaker/java/platform/pc), a
generated PC entry point and a CMake project. Minecraft Alpha converted this way plays on the PC (a world,
rendered); these check the pieces a conversion depends on."""

import os
import shutil
import subprocess

import pytest

from codebreaker.java import transpile
from codebreaker.verify.lanes import java

SDL = r'C:\msys64\mingw64\include\SDL2\SDL.h'
needs_sdl = pytest.mark.skipif(not (java.compiler() and os.path.isfile(SDL)), reason='needs g++ and SDL2')

GAME = '''import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.input.Keyboard;

public class Spin {
	public static void main(String[] var0) throws Exception {
		Display.setDisplayMode(new DisplayMode(320, 240));
		Display.create();
		for (int var1 = 0; var1 < 3 && !Display.isCloseRequested(); ++var1) {
			GL11.glClearColor(0.0F, 0.0F, 1.0F, 1.0F);
			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT);
			if (Keyboard.isKeyDown(Keyboard.KEY_ESCAPE)) {
				break;
			}
			Display.update();
		}
		Display.destroy();
	}
}
'''


@pytest.fixture(scope='module')
def spin(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    (src / 'Spin.java').write_text(GAME, encoding='utf-8')
    out = tmp_path_factory.mktemp('pc')
    try:
        transpile.convert(str(src), str(out), platform='pc_opengl', write_report=False)
    except Exception as e:                                  # no LWJGL on the classpath -> javac fails
        pytest.skip('cannot convert without LWJGL: %s' % e)
    return out


def test_the_pc_layer_replaces_the_stubs(spin):
    for f in ('Display.cpp', 'GL11.cpp', 'Input.cpp', 'Main.cpp', 'CMakeLists.txt'):   # (GLU, ImageIO: pruned, unused)
        assert (spin / f).is_file(), f
    main = (spin / 'Main.cpp').read_text(encoding='utf-8')
    assert 'int main(int argc, char** argv)' in main and 'Spin::main(args)' in main
    assert '#include <GL/' not in (spin / 'GL11.h').read_text(encoding='utf-8')   # no GL macros in the API
    assert 'J2X_COMPONENT_WIDTH() j2xDisplayWidth()' in (spin / 'stdafx.h').read_text(encoding='utf-8')
    assert 'add_executable(Spin' in (spin / 'CMakeLists.txt').read_text(encoding='utf-8')


@needs_sdl
def test_it_builds_and_runs(spin):
    gxx = java.compiler()
    cpps = sorted(str(p) for p in spin.glob('*.cpp'))
    exe = str(spin / 'spin.exe')
    r = subprocess.run([gxx, '-std=gnu++17', '-w', '-fpermissive', '-I', str(spin)] + cpps
                       + ['-o', exe, '-lSDL2', '-lopengl32', '-lglu32', '-lws2_32'],
                       capture_output=True, text=True, timeout=900)
    assert r.returncode == 0, r.stderr[-3000:]
    for dll in ('SDL2.dll', 'libstdc++-6.dll', 'libgcc_s_seh-1.dll', 'libwinpthread-1.dll'):
        p = os.path.join(os.path.dirname(gxx), dll)
        if os.path.isfile(p):
            shutil.copy(p, str(spin))
    run = subprocess.run([exe], capture_output=True, text=True, timeout=120,
                         env=dict(os.environ, J2X_SCRIPT='2:shot frame.bmp'), cwd=str(spin))
    assert run.returncode == 0, (run.stdout, run.stderr)
    assert 'frame saved' in run.stderr


NOISE = '''import paulscode.sound.SoundSystem;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.URL;

public class Noise {
	public static void main(String[] var0) throws Exception {
		SoundSystem var1 = new SoundSystem();
		URL var2 = new File("tone.wav").toURI().toURL();
		var1.newSource(false, "t", var2, "tone.wav", false, 0.0F, 0.0F, 0.0F, 0, 16.0F);
		var1.play("t");
		Thread.sleep(500L);
		var1.cleanup();
		InputStream var4 = Noise.class.getResourceAsStream("/misc/res.txt");
		PrintWriter var3 = new PrintWriter(new FileWriter("out.txt"));
		var3.println("key:7");
		var3.println(var4 != null ? "res ok" : "res missing");
		var3.close();
	}
}
'''


def _tone(path):
    import math
    import struct
    import wave
    w = wave.open(path, 'wb')
    w.setnchannels(1)
    w.setsampwidth(2)
    w.setframerate(22050)
    w.writeframes(b''.join(struct.pack('<h', int(12000 * math.sin(i * 0.1))) for i in range(11025)))
    w.close()


def _peak(path):
    import struct
    import wave
    w = wave.open(path, 'rb')
    data = w.readframes(w.getnframes())
    return max(abs(v) for v in struct.unpack('<%dh' % (len(data) // 2), data)) if data else 0


@needs_sdl
def test_sound_saving_and_resources_work(tmp_path):
    """FIXED BUGs on the PC: the SoundSystem stub was silent (now SDL2 audio + libvorbisfile; a File's
    toURI().toURL() was NULL), Writer.write(String) was lost so options.txt saved empty, and a resource
    asked for under a folder ("/misc/foliagecolor.png") was missing from the flattened Content folder --
    black leaves and grass."""
    (tmp_path / 'src').mkdir()
    (tmp_path / 'src' / 'Noise.java').write_text(NOISE, encoding='utf-8')
    out = tmp_path / 'pc'
    try:
        transpile.convert(str(tmp_path / 'src'), str(out), platform='pc_opengl', write_report=False)
    except Exception as e:
        pytest.skip('cannot convert without the sound system: %s' % e)
    gxx = java.compiler()
    exe = str(out / 'noise.exe')
    r = subprocess.run([gxx, '-std=gnu++17', '-w', '-fpermissive', '-I', str(out)]
                       + sorted(str(p) for p in out.glob('*.cpp'))
                       + ['-o', exe, '-lSDL2', '-lopengl32', '-lglu32', '-lvorbisfile', '-lvorbis', '-logg',
                          '-lws2_32'], capture_output=True, text=True, timeout=900)
    assert r.returncode == 0, r.stderr[-3000:]
    content = out / 'Content'
    content.mkdir()
    _tone(str(content / 'tone.wav'))
    (content / 'res.txt').write_text('x', encoding='utf-8')
    run = subprocess.run([exe], capture_output=True, text=True, timeout=120, cwd=str(out),
                         env=dict(os.environ, J2X_AUDIO_DUMP='mix.wav', SDL_AUDIODRIVER='dummy',
                                  PATH=os.path.dirname(gxx) + os.pathsep + os.environ.get('PATH', '')))
    assert run.returncode == 0, (run.stdout, run.stderr)
    assert (content / 'out.txt').read_text(encoding='utf-8').split() == ['key:7', 'res', 'ok']
    assert _peak(str(content / 'mix.wav')) > 1000
