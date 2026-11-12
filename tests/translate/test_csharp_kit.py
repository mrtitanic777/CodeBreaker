# -*- coding: utf-8 -*-
"""The C# language kit (docs/TRANSLATOR_DESIGN.md, phase 7): the Roslyn front end (CsTreeDump + lower.py)
gives valid L2 and says why it refuses what it refuses; the C# back end writes C# that compiles and
does what the original does; the judge runs both. Pairs, each judged beside the original's own run:
C# -> C#, Java -> C# (the Java lane's classes, real Minecraft Alpha classes where the machine has the
source), C# -> C++ (the C++03 back end). The C# corpus programs were written for the kit
(tests/translate/corpus/csharp); the Java corpus is the Java lane's test classes and fixtures with
drivers written for the kit, plus one modern-Java program written for it."""

import json
import os
import shutil

import pytest

from codebreaker.translate import dotnet, ir, l2

HERE = os.path.dirname(os.path.abspath(__file__))
CORPUS = os.path.join(HERE, 'corpus')
CS = sorted(os.path.join(CORPUS, 'csharp', f) for f in os.listdir(os.path.join(CORPUS, 'csharp')) if f.endswith('.cs'))


def _minecraft():
    """The decompiled Minecraft Alpha source (binaries/java/original_java beside the checkout -- a worktree's
    is its main checkout's), or ''."""
    d = HERE
    for _ in range(8):
        p = os.path.join(d, 'binaries', 'java', 'original_java')
        if os.path.isdir(p):
            return p
        d = os.path.dirname(d)
    return ''


MINECRAFT = _minecraft()

needs_dotnet = pytest.mark.skipif(not dotnet.available(), reason='needs the .NET SDK')


def _jdk():
    from codebreaker.verify.lanes import java
    return java.jdk()[0]


def _cxx():
    from codebreaker.verify.lanes import java
    return java.compiler()


@pytest.fixture
def work(request):
    """A working folder under state/work (never the system temp), removed afterwards."""
    from codebreaker.base import settings
    d = settings.state('work', 'kit-csharp-tests', request.node.name.replace('[', '_').replace(']', '').replace('/', '_')
                       .replace('\\', '_').replace('.', '_'))
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    yield d
    shutil.rmtree(d, ignore_errors=True)


def _units(files, work):
    from codebreaker.translate.dotnet import pairs
    return pairs.csharp_units(files, work)


# ---------------------------------------------------------------------------------------------- front end
@needs_dotnet
def test_every_corpus_program_lowers_to_valid_l2(work):
    pytest.importorskip('jsonschema')
    units, refused = _units(CS, work)
    assert len(units) == len(CS) and not refused, refused
    for u in units:
        assert u.lang == 'csharp'
        doc = l2.to_json(u)
        l2.validate(json.loads(json.dumps(doc)))
        assert l2.to_json(l2.from_json(doc)) == doc          # round-trips, the new fields included


@needs_dotnet
def test_the_bound_tree_says_what_the_compiler_resolved(work):
    from codebreaker.translate.frontends import csharp as fcs
    out = os.path.join(work, 'dump')
    trees = fcs.dump([os.path.join(CORPUS, 'csharp', 'Shapes.cs')], out)
    assert not fcs.errors(out)
    text = json.dumps(trees['Shapes'])
    assert '"k": "Invocation"' in text and '"targetMethod"' in text and '"mk": "PropertyGet"' not in text
    units, _r = _units([os.path.join(CORPUS, 'csharp', 'Shapes.cs')], work)
    props = [n for n in ir.walk(units[0]) if isinstance(n, ir.Property)]
    assert {p.sym.name for p in props} >= {'Id', 'Name', 'W', 'H', 'Made'}
    reads = [n for n in ir.walk(units[0]) if isinstance(n, ir.Select) and n.sym.kind == 'property']
    assert reads, 'a property read is a Select of a property symbol'


@needs_dotnet
def test_what_cannot_be_carried_is_refused_with_its_reason(work):
    src = os.path.join(work, 'Refused.cs')
    with open(src, 'w', encoding='utf-8') as f:
        f.write('''using System.Collections.Generic; using System.Threading.Tasks;
static class P {
    static IEnumerable<int> Count() { yield return 1; }
    static async Task<int> Later() { await Task.Delay(1); return 2; }
    static (int, int) Pair() => (1, 2);
    static void Main() { int Local() => 3; System.Console.WriteLine(Local()); }
}''')
    units, refused = _units([src], work)
    kinds = {k for _w, k, _why in refused}
    assert {'iterator', 'await', 'tuple', 'local-function'} <= kinds, refused
    from codebreaker.translate.backends import csharp as bcs
    prog = bcs.Program(units)
    text = prog.files()['Refused.cs']
    assert text.count('CB-GAP') >= 4 and prog.ledger


# ---------------------------------------------------------------------------------------------- C# -> C#
@needs_dotnet
@pytest.mark.parametrize('name', [os.path.basename(f)[:-3] for f in CS])
def test_csharp_round_trip_is_equivalent(name, work):
    from codebreaker.translate.dotnet import pairs
    v = pairs.csharp_to_csharp([os.path.join(CORPUS, 'csharp', name + '.cs')], work)
    assert v.verdict == 'equivalent', (v.detail, v.refused)


@needs_dotnet
def test_the_back_end_writes_csharp_idioms(work):
    from codebreaker.translate.backends import csharp as bcs
    units, _r = _units([os.path.join(CORPUS, 'csharp', f) for f in ('Linq.cs', 'Bank.cs', 'Records.cs', 'Inventory.cs')],
                       work)
    files = bcs.Program(units).files()
    assert 'new List<Person> { new Person { Name = "Ann"' in files['Linq.cs']        # initialisers put back
    assert '.Where(p => p.Age >= 21)' in files['Linq.cs']                           # an extension call, a lambda
    assert 'using (var log = new Ledger("day1"))' in files['Bank.cs']
    assert 'public int Short { get; }' in files['Bank.cs']
    assert 'public record Circle(double R) : Shape;' in files['Records.cs']
    assert 'maybe?.Length' in files['Records.cs']
    assert 'public static Price operator +(Price a, Price b)' in files['Inventory.cs']
    assert 'g.Name.Shout()' in files['Inventory.cs']


def test_units_of_one_file_stem_are_separate_files_and_big_constants_fold_exactly():
    from codebreaker.translate.backends import csharp as bcs
    t32 = ir.Type('i32')
    t64 = ir.Type('i64')

    def unit(path, name):
        return ir.Unit(path=path, lang='csharp', types=[ir.ClassDecl(name=name, qname=name, kind='class', mods=['public'], members=[])])
    files = bcs.Program([unit('a/Same.cs', 'A'), unit('b/Same.cs', 'B')]).files()
    assert list(files) == ['Same.cs', 'Same_2.cs'] and 'class A' in files['Same.cs'] and 'class B' in files['Same_2.cs']

    def lit(v, t):
        return ir.Lit(value=v, type=t)
    # 2**62 * 4 is a constant that leaves long; long.MaxValue / 1 is not (a quotient is not rounded through a double)
    big = ir.Binary(op='*', left=lit(1 << 62, t64), right=lit(4, t64), type=t64)
    quotient = ir.Binary(op='/', left=lit((1 << 63) - 1, t64), right=lit(1, t64), type=t64)
    from codebreaker.translate.backends.csharp import _overflows
    assert _overflows(big) and not _overflows(quotient)


# ---------------------------------------------------------------------------------------------- Java -> C#
JAVA =[('calc', 'CalcMain'), ('fixtures', 'sample.FixtureMain'), ('modern', 'Library')]


@needs_dotnet
@pytest.mark.skipif(not _jdk(), reason='needs a JDK')
@pytest.mark.parametrize('folder,main', JAVA)
def test_java_to_csharp_is_equivalent(folder, main, work):
    from codebreaker.translate.dotnet import pairs
    v = pairs.java_to_csharp(os.path.join(CORPUS, 'java', folder), main, work)
    assert v.verdict == 'equivalent', (v.detail, v.refused)


@needs_dotnet
@pytest.mark.skipif(not _jdk(), reason='needs a JDK')
@pytest.mark.skipif(not os.path.isfile(os.path.join(MINECRAFT, 'NoiseGeneratorPerlin.java')),
                    reason='needs the decompiled Minecraft Alpha source (binaries/java/original_java)')
def test_minecraft_alpha_noise_java_to_csharp_is_equivalent(work):
    from codebreaker.translate.dotnet import pairs
    src = os.path.join(work, 'src', 'net', 'minecraft', 'src')
    os.makedirs(src)
    for n in ('NoiseGenerator', 'NoiseGeneratorPerlin', 'NoiseGeneratorOctaves', 'MathHelper'):
        shutil.copy(os.path.join(MINECRAFT, n + '.java'), src)
    shutil.copy(os.path.join(CORPUS, 'java', 'minecraft', 'NoiseMain.java'), src)
    v = pairs.java_to_csharp(os.path.join(work, 'src'), 'net.minecraft.src.NoiseMain', work)
    assert v.verdict == 'equivalent', (v.detail, v.refused)
    text = open(os.path.join(work, 'cs', 'NoiseGeneratorPerlin.cs'), encoding='utf-8').read()
    assert 'public virtual double generateNoise(' in text and 'JavaRandom' in text


@needs_dotnet
@pytest.mark.skipif(not _jdk(), reason='needs a JDK')
def test_java_semantics_are_kept_in_csharp(work):
    from codebreaker.translate.dotnet import pairs
    from codebreaker.translate.backends import csharp as bcs
    units = pairs.java_units(os.path.join(CORPUS, 'java', 'modern'), work)
    prog = bcs.Program(units)
    text = prog.files()['Library.cs']
    assert 'goto outer_break;' in text and 'outer_continue:;' in text     # labelled break / continue
    assert 'goto case "beta";' in text                                     # fall-through
    assert 'public override string ToString()' in text and 'public override int GetHashCode()' in text
    assert 'JavaRt.Str(' in text and 'JavaRt.cs' in prog.files()            # Java's text for values


# ---------------------------------------------------------------------------------------------- C# -> C++
@needs_dotnet
@pytest.mark.skipif(not _cxx(), reason='needs the host C++ compiler')
def test_csharp_to_cpp03_is_equivalent(work):
    from codebreaker.translate.dotnet import pairs
    v = pairs.csharp_to_cpp([os.path.join(CORPUS, 'csharp', 'Shapes.cs')], work)
    assert v.verdict == 'equivalent', (v.detail, v.refused)


@needs_dotnet
def test_csharp_to_cpp03_refuses_on_a_runtime_gap(work):
    from codebreaker.translate.dotnet import cs2cpp
    units, _r = _units([os.path.join(CORPUS, 'csharp', 'Accounts.cs')], work)
    refused = cs2cpp.ToJava(units).run()
    assert [k for _w, k, _why in refused] == ['runtime'], refused      # e.Message, and nothing else


@needs_dotnet
def test_csharp_to_cpp03_refuses_what_it_cannot_carry(work):
    from codebreaker.translate.dotnet import cs2cpp
    units, _r = _units([os.path.join(CORPUS, 'csharp', 'Records.cs')], work)
    refused = cs2cpp.ToJava(units).run()
    assert any(k == 'record' for _w, k, _why in refused), refused


# ---------------------------------------------------------------------------------------------- the judge
def test_the_judge_compares_output_and_exit_code():
    from codebreaker.translate.dotnet import judge
    a = judge.Run(True, 0, 'one\r\ntwo\n')
    assert judge.compare(a, judge.Run(True, 0, 'one\ntwo'))[0] == 'equivalent'
    assert judge.compare(a, judge.Run(True, 1, 'one\ntwo'))[0] == 'divergent'
    v, d = judge.compare(a, judge.Run(True, 0, 'one\nthree'))
    assert v == 'divergent' and 'line 2' in d
    assert judge.compare(a, judge.Run(False, log='error CS0001'))[0] == 'no-compile'
    assert judge.compare(a, a, refused=['x: why'])[0] == 'refused'
