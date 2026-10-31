# -*- coding: utf-8 -*-
"""Phase 6, C out for Java (codebreaker/translate/backends/c_java.py): a Java program as C structs and function
tables on the C runtime (translate/runtime/c), compiled with clang (or gcc) and run beside the JVM."""
import os
import subprocess

import pytest

from codebreaker.translate import javajudge
from codebreaker.translate.backends import c_java
from codebreaker.verify.lanes import java as lane

PROGRAM = '''import java.util.*;

public class Zoo {
    interface Animal { String sound(); default String greet() { return name() + " says " + sound(); } String name(); }

    abstract static class Base implements Animal {
        protected final String name;
        static int made;
        Base(String n) { name = n; made++; describe(); }
        public String name() { return name; }
        void describe() {}
    }

    static class Dog extends Base {
        int barks;
        Dog(String n) { super(n); }
        public String sound() { return "woof"; }
        void describe() { barks = 1; }
    }

    static class Cat extends Base {
        Cat(String n) { super(n); }
        public String sound() { return "meow"; }
    }

    record Pair(String a, int b) {}

    static class Empty extends RuntimeException {
        Empty(String m) { super(m); }
    }

    static int pick(int k) {
        if (k < 0) throw new Empty("negative " + k);
        return k * 3;
    }

    static int sumTo(int n) {
        int s = 0;
        for (int i : new int[] {1, 2, 3, n}) s += i;
        return s;
    }

    public static void main(String[] args) {
        List<Animal> zoo = new ArrayList<>();
        zoo.add(new Dog("rex"));
        zoo.add(new Cat("tom"));
        for (Animal a : zoo) System.out.println(a.greet());
        System.out.println(Base.made + " " + ((Dog) zoo.get(0)).barks + " " + new Pair("x", 2) + " " + sumTo(4));
        try {
            pick(-2);
        } catch (Empty e) {
            System.out.println("caught " + e.getMessage());
        } finally {
            System.out.println("finally");
        }
        Map<String, Integer> m = new LinkedHashMap<>();
        m.put("one", 1);
        m.put("two", 2);
        System.out.println(m + " " + pick(5) + " " + String.format("%5.2f|%d", 3.14159, 42));
        Comparator<String> byLen = (x, y) -> x.length() - y.length();
        List<String> ws = new ArrayList<>(List.of("ccc", "a", "bb"));
        ws.sort(byLen);
        System.out.println(ws);
    }
}
'''


@pytest.fixture(scope='module')
def units(tmp_path_factory):
    javac, _java = lane.jdk()
    if not javac:
        pytest.skip('no JDK')
    work = tmp_path_factory.mktemp('zoo')
    src = work / 'src'
    src.mkdir()
    (src / 'Zoo.java').write_text(PROGRAM, encoding='utf-8')
    return javajudge.lower_program(str(src), str(work / 'l2')), work


def test_classes_are_structs_and_tables(units):
    us, _w = units
    text = c_java.program(us)
    assert 'struct Zoo_Dog {\n\tZoo_Base super_;' in text          # the base's struct first
    assert 'struct Zoo_Base_vt {' in text and 'jobject_vt o_;' in text
    assert 'const jclass Zoo_Dog_class = {"Zoo.Dog", &Zoo_Base_class' in text
    assert 'Zoo_Animal_greet' in text                              # a default method, through the itab
    assert 'setjmp(' in text                                       # exceptions: the handler chain
    assert 'struct Lambda' in text                                 # a lambda is a closure record


def test_the_c_program_runs_as_the_jvm_runs_it(units):
    us, work = units
    javac, java = lane.jdk()
    if not javac or not (os.path.isfile(c_java.CLANG)):
        pytest.skip('no JDK or clang')
    out = work / 'c'
    out.mkdir(exist_ok=True)
    exe, why = c_java.build(us, str(out))
    assert exe, why
    classes = work / 'classes'
    classes.mkdir(exist_ok=True)
    subprocess.run([javac, '-d', str(classes), str(work / 'src' / 'Zoo.java')], check=True)
    want = subprocess.run([java, '-cp', str(classes), 'Zoo'], stdout=subprocess.PIPE).stdout.replace(b'\r\n', b'\n')
    got = subprocess.run([exe], stdout=subprocess.PIPE).stdout.replace(b'\r\n', b'\n')
    assert got == want
