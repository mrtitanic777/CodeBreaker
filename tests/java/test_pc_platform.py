# -*- coding: utf-8 -*-
"""The PC platform (pc_opengl): SDL2 + real OpenGL behind LWJGL's API (codebreaker/java/platform/pc), a
generated PC entry point and a CMake project. Minecraft Alpha converted this way plays on the PC (a world,
rendered); these check the pieces a conversion depends on."""

import os
import shutil
import subprocess

import pytest

from codebreaker.java import transpile
from codebreaker.verify import java

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
