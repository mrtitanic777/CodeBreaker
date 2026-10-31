# -*- coding: utf-8 -*-
"""The C++17 back end (codebreaker/translate/backends/cpp17_java.py), its modern-Java front end and desugar pass:
whole programs from L2 with the idiomatic forms (lambdas, range-for, switch expressions as lambdas,
real constructors for records, auto), and inside the transpiler (cpp17.Bodies) beside cpp03. The programs
are compiled and run beside the JVM where a JDK and a C++ compiler are present."""
import os
import shutil
import subprocess

import pytest

from codebreaker.translate import desugar, ir, javajudge
from codebreaker.translate.backends import cpp17_java as cpp17, jmodel
from codebreaker.verify.lanes import java as lane

PROGRAM = '''import java.util.*;
import java.util.function.*;
import java.util.stream.*;

public class Modern {
    record Point(int x, int y) {
        int dist2() { return x * x + y * y; }
    }

    sealed interface Shape permits Sq, Circ {}
    record Sq(int side) implements Shape {}
    record Circ(int r) implements Shape {}

    enum Color { RED, GREEN, BLUE }

    static int area(Shape s) {
        if (s instanceof Sq q) return q.side() * q.side();
        if (s instanceof Circ c) return 3 * c.r() * c.r();
        return 0;
    }

    static String name(Color c) {
        return switch (c) {
            case RED -> "r";
            case GREEN -> "g";
            default -> { String b = "b"; yield b + b; }
        };
    }

    static int sum(List<Integer> xs) {
        int s = 0;
        for (int v : xs) s += v;
        return s;
    }

    static int squares(int n) {
        return IntStream.rangeClosed(1, Math.abs(n) % 50).map(x -> x * x).sum();
    }

    static String words(String s) {
        return switch (s) { case "a", "b" -> "ab"; default -> "other"; };
    }

    public static void main(String[] args) {
        List<Integer> xs = new ArrayList<>();
        for (int i = 0; i < 6; i++) xs.add(i * i);
        Function<Integer, Integer> twice = v -> v * 2;
        System.out.println(sum(xs) + " " + twice.apply(21) + " " + squares(10));
        System.out.println(new Point(3, 4) + " " + new Point(3, 4).dist2() + " " + new Point(1, 2).equals(new Point(1, 2)));
        System.out.println(area(new Sq(4)) + " " + area(new Circ(2)) + " " + name(Color.BLUE) + " " + words("b"));
        System.out.println(xs.stream().filter(v -> v % 2 == 0).map(String::valueOf).collect(Collectors.joining(",")));
        Map<String, Integer> m = new HashMap<>();
        for (String w : "x y x z y x".split(" ")) m.merge(w, 1, Integer::sum);
        System.out.println(m);
    }
}
'''


@pytest.fixture(scope='module')
def units(tmp_path_factory):
    javac, _java = lane.jdk()
    if not javac:
        pytest.skip('no JDK')
    work = tmp_path_factory.mktemp('modern')
    src = work / 'src'
    src.mkdir()
    (src / 'Modern.java').write_text(PROGRAM, encoding='utf-8')
    return javajudge.lower_program(str(src), str(work / 'l2')), work


def test_records_get_their_implicit_members(units):
    us, _w = units
    point = next(m for c in us[0].types for m in c.members if isinstance(m, ir.ClassDecl) and m.name == 'Point')
    names = sorted(m.name for m in point.members if isinstance(m, ir.MethodDecl) and 'GENERATED' in (m.mods or []))
    assert names == ['equals', 'hashCode', 'toString', 'x', 'y']
    ctor = next(m for m in point.members if isinstance(m, ir.MethodDecl) and m.kind == 'ctor')
    assigned = [s.expr.target.sym.name for s in ctor.body.stmts if isinstance(s, ir.ExprStmt) and isinstance(s.expr, ir.Assign)]
    assert assigned == ['x', 'y']


def test_instanceof_patterns_carry_their_binding(units):
    us, _w = units
    found = [n for n in ir.walk(us[0]) if isinstance(n, ir.InstanceOf) and n.binding is not None]
    assert sorted(n.binding.sym.name for n in found) == ['c', 'q']


def test_the_object_model(units):
    us, _w = units
    model = jmodel.Model(us)
    point = model.info('Modern.Point')
    assert point.kind == 'record' and point.cname == 'Modern_Point'
    to_string = next(m for m in point.methods() if m.name == 'toString')
    assert model.overrides(point, to_string)[1].name == 'java.lang.String'
    assert model.sam('java.util.function.Function')[0] == 'apply'
    assert model.is_sub('Modern.Sq', 'Modern.Shape')


def test_the_program_reads_as_cpp17(units):
    us, _w = units
    text = cpp17.program(us)
    assert 'jrt::fn<Function>(' in text                      # a lambda is a C++ lambda
    assert 'jrt::each<int32_t>(xs)' in text                  # for-each is a range-for
    assert '[&]() -> ref<String> {' in text                   # a switch expression is a lambda
    assert 'std::make_shared<Modern_Point>(' in text         # a record has real constructors
    assert 'static inline' in text or 'struct Modern_Color' in text
    assert 'auto ' in text


def _run(cmd, env=None):
    e = dict(os.environ)
    e.update(env or {})
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=e, timeout=300)
    return r.returncode, r.stdout.decode('utf-8', 'replace').replace('\r\n', '\n')


def test_the_program_runs_as_the_jvm_runs_it(units):
    us, work = units
    cxx = lane.compiler()
    javac, java = lane.jdk()
    if not cxx or not javac:
        pytest.skip('no C++ compiler or JDK')
    exe, why = javajudge.build(us, str(work), 'cpp17')
    assert exe, why
    classes = work / 'classes'
    classes.mkdir(exist_ok=True)
    assert _run([javac, '-d', str(classes), str(work / 'src' / 'Modern.java')])[0] == 0
    want = _run([java, '-cp', str(classes), 'Modern'])
    got = _run([exe])
    assert got == want


def test_desugar_lowers_what_cpp03_cannot_say(units, tmp_path):
    us, _w = units
    import copy
    lowered = copy.deepcopy(us)
    n = desugar.run(lowered)
    assert n >= 5
    kinds = set(type(x).__name__ for x in ir.walk(lowered[0]))
    assert 'ForEach' not in kinds and 'Yield' not in kinds
    assert not any(isinstance(x, ir.InstanceOf) and x.binding is not None for x in ir.walk(lowered[0]))
    assert not any(isinstance(x, ir.SwitchExpr) for x in ir.walk(lowered[0]))
    # a for-each the C++17 back end keeps
    kept = copy.deepcopy(us)
    desugar.run(kept, keep=('foreach', 'switchexpr'))
    assert any(isinstance(x, ir.ForEach) for x in ir.walk(kept[0]))


TRANSPILED = '''import java.util.ArrayList;
import java.util.List;

public class Tally {
	public static int total(int n) {
		List<Integer> xs = new ArrayList<Integer>();
		for (int i = 0; i < n % 20; i++) xs.add(i);
		int s = 0;
		for (int v : xs) s += v;
		return s;
	}

	public static String kind(int k) {
		return switch (k % 3) { case 0 -> "zero"; case 1 -> "one"; default -> "many"; };
	}

	public static int fresh(int k) {
		Tally t = new Tally();
		return t.twice(k);
	}

	public int twice(int k) {
		return k * 2;
	}
}
'''


def test_inside_the_transpiler_beside_cpp03(tmp_path):
    from codebreaker.java import transpile
    cxx = lane.compiler()
    javac, _java = lane.jdk()
    if not javac:
        pytest.skip('no JDK')
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'Tally.java').write_text(TRANSPILED, encoding='utf-8')
    out17, out03 = tmp_path / 'cpp17', tmp_path / 'cpp03'
    transpile.convert(str(src), str(out17), write_report=False, cpp_std='C++17')
    transpile.convert(str(src), str(out03), write_report=False)
    t17 = (out17 / 'Tally.cpp').read_text(encoding='utf-8')
    t03 = (out03 / 'Tally.cpp').read_text(encoding='utf-8')
    assert 'for (int v : xs)' in t17 or 'for (Integer' in t17 or ': xs)' in t17     # a range-for
    assert '[&]() ->' in t17                                                         # the switch expression
    assert 'auto* t = new Tally' in t17
    assert 'J2X-GAP' not in t17 and 'J2X-GAP' not in t03                           # cpp03 has it, desugared
    if cxx:
        r = subprocess.run([cxx, '-std=c++17', '-fsyntax-only', '-w', '-iquote', str(out17), str(out17 / 'Tally.cpp')],
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        assert r.returncode == 0, r.stdout.decode('utf-8', 'replace')[:2000]
