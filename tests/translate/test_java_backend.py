# -*- coding: utf-8 -*-
"""The Java back end (codebreaker/translate/backends/java.py, java_objects.py, java_runtime/crt): C and C++
programs written as Java and judged beside their native build (codebreaker/translate/judge_native_jvm.py).

Each program is compiled with clang and run, translated (clang front end -> L2 -> Java), compiled with javac
and run on the JVM; stdout, stderr and the exit code must agree. Needs clang and a JDK."""
import pytest

from codebreaker.translate import judge_native_jvm as judge
from codebreaker.translate.backends import java as jb
from codebreaker.translate.frontends import clang


def _tools():
    try:
        javac, java = judge._jdk()
    except Exception:
        return False
    return bool(clang.clang() and javac and java)


needs_tools = pytest.mark.skipif(not _tools(), reason='needs clang and a JDK')


def _judge(tmp_path, name, source, ext='.c', **kw):
    f = tmp_path / (name + ext)
    f.write_text(source, encoding='utf-8')
    return judge.judge([str(f)], str(tmp_path / 'run'), **kw)


FAITHFUL = r'''
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
union Pun { float f; uint32_t u; unsigned char b[4]; };
struct Rec { short id; char name[6]; double weight; };
static unsigned hash(const char *s) { unsigned h = 2166136261u; while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; } return h; }
static struct Rec make(short id, const char *n, double w) { struct Rec r; r.id = id; strcpy(r.name, n); r.weight = w; return r; }
static int cmp(const void *a, const void *b) { return ((const struct Rec *)a)->id - ((const struct Rec *)b)->id; }
int main(void) {
    union Pun p; p.f = -1.5f;
    printf("%08x %u %u\n", p.u, p.b[3], (unsigned)sizeof(struct Rec));
    struct Rec rs[3] = { make(3, "c", 1.25), make(1, "a", 2.5), make(2, "b", -3.0) };
    qsort(rs, 3, sizeof rs[0], cmp);
    for (struct Rec *q = rs; q < rs + 3; q++) printf("%d %s %.2f|", q->id, q->name, q->weight);
    printf("\n%u %u\n", hash("hello"), 4000000000u / 3u);
    int *buf = malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) buf[i] = i * i;
    int *end = buf + 5, s = 0;
    for (int *x = buf; x != end; ++x) s += *x;
    free(buf);
    long long big = -9000000000LL; unsigned char c = 250; c += 10; signed char sc = (signed char)200;
    printf("%d %lld %d %d %x %5.1e|%-6s|%+d %g %g\n", s, big / 7, c, sc, -1, 12345.678, "ab", 5, 0.0001, 1e20);
    return s & 7;
}
'''


@needs_tools
def test_faithful_tier(tmp_path):
    v = _judge(tmp_path, 'faithful_demo', FAITHFUL)
    assert v.verdict == 'equivalent', (v.reason, v.java)
    assert v.tier == 'faithful'
    assert 'Mem.getInt' in v.java and 'static final class Rec' in v.java     # memory, and offsets per struct
    assert 'Integer.divideUnsigned' in v.java                              # unsigned arithmetic written out


IDIOMATIC = r'''
#include <stdio.h>
#include <stdlib.h>
struct Node { int value; struct Node *next; };
struct Point { int x, y; };
static struct Node *push(struct Node *head, int v) {
    struct Node *n = malloc(sizeof(struct Node));
    n->value = v; n->next = head; return n;
}
static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }
static struct Point mid(struct Point a, struct Point b) { struct Point m = { (a.x + b.x) / 2, (a.y + b.y) / 2 }; return m; }
int main(void) {
    struct Node *list = NULL;
    for (int i = 1; i <= 4; i++) list = push(list, i * 10);
    for (struct Node *n = list; n; n = n->next) printf("%d ", n->value);
    int x = 1, y = 2; swap(&x, &y);
    struct Point a = { 0, 0 }, b = { 10, 4 }, c = a;
    c.x = 99;
    struct Point m = mid(a, b);
    int grid[3][4] = { { 1, 2 }, { 3 } };
    printf("\n%d %d %d %d %d %d %d\n", x, y, a.x, c.x, m.x, m.y, grid[1][0] + grid[0][1] + grid[2][3]);
    return 0;
}
'''


@needs_tools
def test_idiomatic_tier(tmp_path):
    v = _judge(tmp_path, 'idiomatic_demo', IDIOMATIC)
    assert v.verdict == 'equivalent', (v.reason, v.java)
    assert v.tier == 'idiomatic'
    assert 'Mem.' not in v.java                                          # no emulated memory
    assert 'static class Node' in v.java and 'Node next;' in v.java       # a struct is a class, a pointer a reference
    assert 'new Node()' in v.java and 'int[] x = {1}' in v.java          # malloc is new; &x boxes x
    assert 'new Point(c' not in v.java and 'new Point(a)' in v.java      # a struct value copied as C copies it


CPP = r'''
#include <cstdio>
class Animal {
public:
    explicit Animal(int legs) : legs_(legs) { ++count; }
    virtual ~Animal() {}
    virtual const char *sound() const = 0;
    int legs() const { return legs_; }
    static int count;
private:
    int legs_;
};
int Animal::count = 0;
class Dog : public Animal { public: Dog() : Animal(4) {} const char *sound() const override { return "woof"; } };
class Bird : public Animal { public: Bird() : Animal(2) {} const char *sound() const override { return "tweet"; } };
struct V2 { double x, y; V2(double a, double b) : x(a), y(b) {} V2 operator*(double k) const { return V2(x * k, y * k); } };
template <typename T> T twice(T v) { return v + v; }
static void inc(int &r) { r++; }
int main() {
    Animal *zoo[2] = { new Dog(), new Bird() };
    for (int i = 0; i < 2; i++) std::printf("%s %d\n", zoo[i]->sound(), zoo[i]->legs());
    V2 v = V2(1.5, -2) * 2;
    int n = 41; inc(n);
    std::printf("%g %g %d %d %g %d\n", v.x, v.y, n, twice(21), twice(1.25), Animal::count);
    for (int i = 0; i < 2; i++) delete zoo[i];
    return 0;
}
'''


@needs_tools
def test_cpp_classes(tmp_path):
    v = _judge(tmp_path, 'cpp_demo', CPP, '.cpp')
    assert v.verdict == 'equivalent', (v.reason, v.java)
    assert v.tier == 'idiomatic'
    assert 'static abstract class Animal' in v.java and 'class Dog extends Animal' in v.java
    assert 'super(4);' in v.java and 'abstract byte[] sound();' in v.java
    assert 'V2 times(double k)' in v.java                                  # operator* as a named method
    assert 'static int twice(int v)' in v.java and 'static double twice(double v)' in v.java


GOTO_VARARGS = r'''
#include <stdio.h>
#include <stdarg.h>
static int total(int n, ...) { va_list ap; int s = 0; va_start(ap, n); while (n--) s += va_arg(ap, int); va_end(ap); return s; }
static void say(const char *fmt, ...) { va_list ap; va_start(ap, fmt); vprintf(fmt, ap); va_end(ap); }
int main(void) {
    int i = 0, found = -1;
    int data[6] = { 4, 8, 15, 16, 23, 42 };
retry:
    i++;
    if (i < 3) goto retry;
    for (int k = 0; k < 6; k++) if (data[k] == 16) { found = k; goto done; }
    say("not found\n");
done:
    say("%d %d %s %.1f\n", i, found, "ok", 2.5);
    return total(3, 1, 2, 3);
}
'''


@needs_tools
def test_goto_and_varargs(tmp_path):
    v = _judge(tmp_path, 'goto_demo', GOTO_VARARGS)
    assert v.verdict == 'equivalent', (v.reason, v.java)
    assert 'break done;' in v.java and 'continue retry;' in v.java        # goto as Java's labeled jumps
    assert 'Object... va' in v.java and 'Mem.varargs(va)' in v.java


@needs_tools
def test_refusals_say_why(tmp_path):
    v = _judge(tmp_path, 'jmp', '#include <setjmp.h>\n#include <stdio.h>\nstatic jmp_buf b;\n'
                                'int main(void) { if (!setjmp(b)) longjmp(b, 1); puts("back"); return 0; }\n')
    assert v.verdict == 'refused' and 'setjmp' in v.reason.lower() and 'library' in v.reason, v.reason
    raii = ('#include <cstdio>\nstruct G { ~G() { std::puts("bye"); } };\n'
            'int main() { G g; std::puts("hi"); return 0; }\n')
    v = _judge(tmp_path, 'raii', raii, '.cpp')
    assert v.verdict == 'refused' and 'destructor' in v.reason, v.reason


def test_java_literals_and_names():
    assert jb.java_string('a"\\\n\x01\xe9') == '"a\\"\\\\\\n\\001\\351"'
    assert jb.int_lit(0xFFFFFFFF, 'int').code == '-1'
    assert jb.int_lit(1 << 40, 'long').code == '1099511627776L'
    assert jb.jname('class') == 'class_' and jb.jname('frame') == 'frame_'
