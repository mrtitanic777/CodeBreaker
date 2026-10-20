# -*- coding: utf-8 -*-
"""The 360 fixes to the game's own logic, made on the IR before the back end writes (console/sound_fix,
applet_ping, render_fix: patch()). Each is matched by tree shape -- the text versions missed Indev's
updateRenderers once the back end printed `list.get(i)` where the old path printed `list[i]`."""

import pytest

from codebreaker.java.projects.minecraft import applet_ping, render_fix, sound_fix
from codebreaker.translate import ir
from codebreaker.translate.backends import cpp03
from codebreaker.verify import java

needs_jdk = pytest.mark.skipif(not java.jdk()[0], reason='needs a JDK')

SOURCES = {
    'Entry.java': '''public class Entry {
	public Object soundUrl;
	public String soundName;
}
''',
    'Pool.java': '''public class Pool {
	public Entry getRandomSoundFromSoundPool(String var1) {
		return null;
	}

	public Entry getRandomSound() {
		return null;
	}
}
''',
    'Snd.java': '''public class Snd {
	public void newSource(boolean var1, String var2, Object var3, String var4) {
	}

	public void backgroundMusic(String var1, Object var2, String var3) {
	}

	public void play(String var1) {
	}
}
''',
    'SoundManager.java': '''public class SoundManager {
	private Pool soundPoolSounds = new Pool();
	private Pool soundPoolMusic = new Pool();
	private Snd sndSystem = new Snd();

	public void playSound(String var1, float var2) {
		Entry var3 = this.soundPoolSounds.getRandomSoundFromSoundPool(var1);
		if (var3 != null && var2 > 0.0F) {
			this.sndSystem.newSource(false, "s", var3.soundUrl, var3.soundName);
		}
	}

	public void music() {
		Entry var1 = this.soundPoolMusic.getRandomSound();
		if (var1 != null) {
			this.sndSystem.backgroundMusic("BgMusic", var1.soundUrl, var1.soundName);
			this.sndSystem.play("BgMusic");
		}
	}
}
''',
    'Applet.java': '''import java.net.URL;

public class Applet {
	public URL getDocumentBase() {
		return null;
	}
}
''',
    'Game.java': '''import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;

public class Game {
	public Applet mcApplet;
	public Object theWorld;

	public void setLevel(Object var1) {
		try {
			BufferedReader var2 = new BufferedReader(new InputStreamReader((new URL(this.mcApplet.getDocumentBase() + "?n=")).openStream()));
			Integer.parseInt(var2.readLine());
			if (this.mcApplet.getDocumentBase().toString().startsWith("http://www.minecraft.net/")) {
				this.theWorld = var1;
			}
		} catch (Throwable var3) {
		}
	}
}
''',
    'Chunk.java': '''public class Chunk {
	public boolean needsUpdate;

	public float distanceToEntitySquared(Object var1) {
		return 0.0F;
	}

	public void updateRenderer() {
	}
}
''',
    'Living.java': '''public class Living {
	public int hurtTime;
	public int maxHurtTime;
}
''',
    'Render.java': '''import java.util.ArrayList;
import java.util.List;

public class Render {
	private List worldRenderersToUpdate = new ArrayList();

	public void updateRenderers(Object var1) {
		int var2 = this.worldRenderersToUpdate.size() - 1;
		int var3 = this.worldRenderersToUpdate.size();
		for (int var4 = 0; var4 < var3; ++var4) {
			Chunk var5 = (Chunk)this.worldRenderersToUpdate.get(var2 - var4);
			if (var5.distanceToEntitySquared(var1) > 2500.0F && var4 > 4) {
				return;
			}
			this.worldRenderersToUpdate.remove(var5);
			var5.updateRenderer();
			var5.needsUpdate = false;
		}
	}

	public float hurt(Living var2, float var3) {
		if (var3 >= 0.0F) {
			var3 /= (float)var2.maxHurtTime;
			var3 = var3 * 2.0F;
		}
		return var3;
	}
}
''',
}


@pytest.fixture(scope='module')
def units(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    for n, t in SOURCES.items():
        (src / n).write_text(t, encoding='utf-8')
    return cpp03.java_units(str(src))


def _method(units, cls, name):
    for u in units:
        for c in u.types:
            if c.name == cls:
                return next(m for m in c.members if isinstance(m, ir.MethodDecl) and m.name == name)


@needs_jdk
def test_sound_routes_the_name_and_drops_the_pool_query(units):
    r = sound_fix.patch(units)
    assert r['sites'] == 2 and r['files'] == [{'file': 'SoundManager.cpp', 'sfx': 1, 'music': 1}], r
    play = _method(units, 'SoundManager', 'playSound').body.stmts
    assert isinstance(play[0].init, ir.Lit) and play[0].init.value is None            # no query
    assert isinstance(play[1].cond, ir.Binary) and play[1].cond.op == '>'              # the volume gate kept
    call = play[1].then.stmts[0].expr
    assert isinstance(call.args[2], ir.Lit) and isinstance(call.args[3], ir.Name)    # url NULL, name the cue
    music = _method(units, 'SoundManager', 'music').body.stmts[1]
    assert isinstance(music.cond, ir.Lit) and music.cond.value is True                 # play("BgMusic") always
    assert isinstance(music.then.stmts[0], ir.If) and isinstance(music.then.stmts[1], ir.ExprStmt)


@needs_jdk
def test_applet_ping_becomes_the_assignment(units):
    r = applet_ping.patch(units)
    assert r['sites'] == 1, r
    body = _method(units, 'Game', 'setLevel').body.stmts
    assert len(body) == 1 and isinstance(body[0], ir.ExprStmt) and isinstance(body[0].expr, ir.Assign)


@needs_jdk
def test_render_budget_and_hurt_guard(units):
    r = render_fix.patch(units)
    assert r['update'] == 1 and r['hurt'] == 1, r
    body = _method(units, 'Render', 'updateRenderers').body.stmts
    assert len(body) == 2 and body[0].init.value == render_fix._BUDGET               # int B = 8; for ...
    loop = body[1].body.stmts
    assert isinstance(loop[0], ir.Local) and isinstance(loop[1], ir.If) and isinstance(loop[4], ir.If)
    hurt = _method(units, 'Render', 'hurt').body.stmts[0].cond
    assert hurt.op == '&&' and hurt.right.left.sym.name == 'hurtTime'
    assert render_fix.patch(units)['update'] == 0                                      # idempotent
