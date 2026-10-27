# -*- coding: utf-8 -*-
"""Regressions for the classes of the Alpha corpus that did not compile with a host g++ (the java kind's
judge): emitter fixes (a member that shares a scope name, sized `new` with an indexed size, a null arm
of `?:` in a value return) and runtime fixes (JObject boxing/unboxing, URL, synchronizedList, the
no-platform stdafx/Inflate/GL constants). Each case is transpiled from Java and, where g++ is present,
compiled."""

import subprocess

import pytest

from codebreaker.java import transpile
from codebreaker.verify.lanes import java

SOURCES = {
    'Session.java': 'public class Session {\n\tpublic String inventory;\n}\n',
    'InventoryPlayer.java': 'public class InventoryPlayer {\n\tpublic int size;\n}\n',
    'EntityPlayer.java': 'public class EntityPlayer {\n\tpublic InventoryPlayer inventory;\n}\n',
    'Probe.java': '''public class Probe extends EntityPlayer {
	String[] logo = new String[]{"ab", "cd"};
	Probe[][] grid;
	Object value;
	Probe[] monsters = new Probe[]{null};
	byte[] bytes;
	public Probe(Session var3) {
		if(var3 != null && var3.inventory != null && var3.inventory.length() > 0) {
		}
		this.grid = new Probe[this.logo[0].length()][this.logo.length];
	}
	public String name(boolean var1, int var2) {
		return var1 ? "World" + var2 : null;
	}
	public Object get(Probe var2) {
		return var2 == null ? null : var2.value;
	}
	public Probe[] pick(int var1) {
		return var1 == 0 ? this.monsters : (var1 == 1 ? this.monsters : null);
	}
	public byte[] data(boolean var1) {
		return var1 ? new byte[0] : this.bytes;
	}
}
''',
}

# JObject/JavaString/JList/URL/Collections uses the Alpha classes made, compiled against the runtime
RUNTIME_UNIT = r'''#include "stdafx.h"
#include "JavaRT.h"
#include "Collections.h"
#include "URL.h"
#include "DecimalFormat.h"
int main() {
	JList<JObject> list;
	std::size_t n = 3;
	bool has = list.contains(n);                                  // JObject(size_t) -- was ambiguous
	JObject boxed(JavaString("x"));
	JavaString s = (JavaString)boxed;                             // (String)o -- was ambiguous
	JavaString t = boxed;
	JList<JObject> back = (JList<JObject>)JObject(list);          // (List)o -- was ambiguous
	JList<JObject>& sync = Collections::synchronizedList(list);
	JList<JObject> fresh = Collections::synchronizedList(JList<JObject>());   // a temporary
	URL* a = new URL("http://example/");                          // was ambiguous
	URL* b = new URL((JCat() + "http://" + 1));                   // was ambiguous
	std::string viaBase = (JCat() + "x");                         // JCat still reaches std::string
	JMap<JObject, JObject> map;
	JMap<JObject, JObject> mapBack = (JMap<JObject, JObject>)JObject(map);   // (Map)o -- was ambiguous (Beta)
	JArray<int> ints(3);
	JArray<int> intsBack = (JArray<int>)JObject(ints);            // (int[])o -- was ambiguous (Beta)
	DecimalFormat* df = new DecimalFormat("########0.00");        // was ambiguous (Beta StatBase)
	JArray<unsigned char> chars(2); chars[0] = 'h'; chars[1] = 'i';   // char[] (Java char is unsigned)
	if (String::valueOf(chars) != "hi" || String::valueOf(chars, 1, 1) != "i") return 2;   // char[] (Beta)
	if (max(3, 120) != 120 || max(2, 2.5f) != 2.5f || min(7LL, 3) != 3LL) return 3;       // Math.max/min
	if (intsBack.length != 3) return 4;
	(void)has; (void)t; (void)back; (void)sync; (void)fresh; (void)viaBase; (void)mapBack;
	delete df;
	delete a; delete b;
	return s == "x" ? 0 : 1;
}
'''


@pytest.fixture(scope='module')
def out(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    for name, text in SOURCES.items():
        (src / name).write_text(text, encoding='utf-8')
    dst = tmp_path_factory.mktemp('cpp')
    transpile.convert(str(src), str(dst), platform=None, write_report=False)
    return dst


def _probe(out):
    return (out / 'Probe.cpp').read_text(encoding='utf-8')


def test_a_member_reached_through_a_receiver_is_not_the_scope_name(out):
    # Session.inventory is a String; the inherited EntityPlayer.inventory (a pointer) must not decide it
    assert 'var3->inventory.length()' in _probe(out)


def test_sized_new_with_an_indexed_size(out):
    assert 'j2x_newarr2<Probe* >(this->logo[0].length(), this->logo.length)' in _probe(out)
    assert 'JArray<unsigned char >(0) : this->bytes' in _probe(out)     # the text after ']' is kept


def test_null_arm_of_a_value_return_is_the_empty_value(out):
    text = _probe(out)
    # the null arm is the value type's own null, whatever grouping the writer chose
    assert ': JavaString();' in text or ': JavaString());' in text
    assert '? JObject() :' in text
    assert ': JArray<Probe* >())' in text


def test_no_platform_tree_has_what_its_runtime_includes(out):
    assert (out / 'Inflate.h').is_file() and (out / 'Inflate.cpp').is_file()
    stdafx = (out / 'stdafx.h').read_text(encoding='utf-8')
    assert '#define NOGDI' in stdafx and '#include <windows.h>' in stdafx      # no GDI Rectangle()
    assert 'GL_LINEAR_MIPMAP_LINEAR = 0x2703' in (out / 'GL11.h').read_text(encoding='utf-8')


@pytest.mark.skipif(not java.compiler(), reason='needs a host C++ compiler (g++)')
def test_every_class_and_the_runtime_compile(out):
    (out / 'runtime_unit.cpp').write_text(RUNTIME_UNIT, encoding='utf-8')
    for unit in ('Probe.cpp', 'Inflate.cpp', 'runtime_unit.cpp'):
        ok, errors = java.check(java.compiler(), str(out), str(out / unit))
        assert ok, (unit, errors[:5])
    exe = out / 'runtime_unit.exe'
    r = subprocess.run([java.compiler(), '-std=c++03', '-w', '-iquote', str(out), str(out / 'runtime_unit.cpp'),
                        '-o', str(exe)], capture_output=True, text=True)
    assert r.returncode == 0, r.stderr[-2000:]
    assert subprocess.run([str(exe)]).returncode == 0                  # (String)o gives the boxed string back
