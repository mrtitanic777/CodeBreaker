// Tests of cb::translate::frontends::python::methods: small programs run through the inference with the real library, and
// what it says of them -- the type a function returns, the issues a function gets. The expectations are what the original
// (codebreaker/translate/frontends/python/methods.py) answered for the same calls, written out and read through by hand.
//
// DIVERGES: (1) an iterator type prints as iter[<item>]; (2) the issue of a call is "<class>:<detail>" at its line, as
// Issue::reason writes it; (3) a call with a missing argument (`list.insert(0)`, `set.add()`) is refused as a conflict where the
// Python raised IndexError and the function was refused as a front-end crash.
#include "cb/translate/frontends/python/methods.hpp"

#include <algorithm>
#include <format>

#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/infer_util.hpp"

namespace {

namespace in = cb::translate::frontends::python::infer;
namespace pr = cb::translate::frontends::python::program;
namespace pt = cb::translate::frontends::python::pytypes;
namespace me = cb::translate::frontends::python::methods;

struct Solved {
    pr::Program program;
    explicit Solved(const std::string& source) {
        program.add_module("app", source);
        program.index();
        in::Inference inference(me::library());
        program.solve(inference);
    }
    // what the function returns, as the types print
    std::string returns(const std::string& fn) { return pt::describe(program.returns.at("app." + fn)); }
    // the reasons (class:detail) of the issues of the function's recording pass, sorted and joined with " | "
    std::string issues(const std::string& fn) {
        pr::Fn& f = *program.fns.at("app." + fn);
        const std::vector<in::Issue> found = program.evaluate([&] {
            in::Eval ev(program, &f, true, me::library());
            ev.run();
            return ev.issues;
        });
        std::vector<std::string> out;
        for (const in::Issue& i : found) out.push_back(i.reason());
        std::ranges::sort(out);
        return cb::text::join(out, " | ");
    }
};

}  // namespace

CB_TEST("translate.python.methods.len_and_sizes", [] {
    Solved s(
        "def a(s: str) -> int:\n    return len(s)\n"
        "def b(d: bytes) -> int:\n    return len(d)\n"
        "def c(x: int) -> int:\n    return len(x)\n"
        "def d() -> int:\n    return len((1, 2, 3))\n");
    CB_CHECK_EQ(s.returns("a"), "int[0..140737488355328]");   // the most an address space holds: 2**47
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.returns("d"), "int[3..3]");
    CB_CHECK_EQ(s.issues("c"), "conflict:len of a int");
});

CB_TEST("translate.python.methods.range_enumerate_zip", [] {
    Solved s(
        "def a() -> int:\n"
        "    total = 0\n"
        "    for i in range(10):\n"
        "        total += 1\n"
        "    return total\n"
        "def b(items: list[str]) -> int:\n"
        "    last = 0\n"
        "    for i, x in enumerate(items):\n"
        "        last = i\n"
        "    return last\n"
        "def c() -> int:\n"
        "    for i in range(0, 10, n):\n        pass\n    return 1\n"
        "def d(s: str) -> int:\n    for i in range(s):\n        pass\n    return 1\n");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.returns("a"), "int[0..10]");   // a counter that steps by one in a loop of ten passes
    CB_CHECK_EQ(s.issues("b"), "");
    CB_CHECK_EQ(s.issues("d"), "conflict:range arguments");
});

CB_TEST("translate.python.methods.str_methods", [] {
    Solved s(
        "def a(s: str) -> str:\n    return s.upper()\n"
        "def b(s: str) -> list[str]:\n    return s.split(',')\n"
        "def c(s: str) -> int:\n    return s.find('x')\n"
        "def d(s: str) -> bool:\n    return s.startswith(('a', 'b'))\n"
        "def e(s: str) -> bool:\n    return s.startswith(1)\n"
        "def f(s: str) -> str:\n    return s.format(1)\n"
        "def g(s: str) -> str:\n    return s.nosuch()\n"
        "def h(s: str) -> str:\n    return ','.join(s.split(' '))\n"
        "def i(s: str) -> str:\n    return ','.join([1, 2])\n"
        "def j(s: str) -> bytes:\n    return s.encode('utf-8')\n"
        "def k(s: str) -> bytes:\n    return s.encode('klingon')\n");
    CB_CHECK_EQ(s.returns("a"), "str");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.issues("b"), "");
    CB_CHECK_EQ(s.returns("c"), "int[-1..140737488355328]");
    CB_CHECK_EQ(s.issues("d"), "");
    CB_CHECK_EQ(s.issues("e"), "conflict:str.startswith of a int");
    CB_CHECK_EQ(s.issues("f"), "syntax:str.format");
    CB_CHECK_EQ(s.issues("g"), "stdlib:str.nosuch");
    CB_CHECK_EQ(s.issues("h"), "");
    CB_CHECK_EQ(s.issues("i"), "conflict:str.join of int items");
    CB_CHECK_EQ(s.returns("j"), "bytes");
    CB_CHECK_EQ(s.issues("j"), "");
    CB_CHECK_EQ(s.issues("k"), "stdlib:str.encode('klingon')");
});

CB_TEST("translate.python.methods.bytes_and_bytearray", [] {
    Solved s(
        "def a(d: bytes) -> str:\n    return d.hex()\n"
        "def b(d: bytes) -> str:\n    return d.decode('latin1')\n"
        "def c(d: bytes) -> str:\n    return d.decode('latin1', 'surrogate')\n"
        "def d(n: int) -> bytearray:\n    b = bytearray(4)\n    b.append(n)\n    return b\n"
        "def e() -> bytearray:\n    b = bytearray(4)\n    b.append('x')\n    return b\n"
        "def f(d: bytes) -> int:\n    return int.from_bytes(d[0:4], 'little')\n"
        "def g(d: bytes) -> int:\n    return int.from_bytes(d[0:2], 'big', signed=True)\n"
        "def h(d: bytes, o: str) -> int:\n    return int.from_bytes(d, o)\n");
    CB_CHECK_EQ(s.returns("a"), "str");
    CB_CHECK_EQ(s.returns("b"), "str");
    CB_CHECK_EQ(s.issues("b"), "");
    CB_CHECK_EQ(s.issues("c"), "stdlib:bytes.decode errors=['surrogate']");
    CB_CHECK_EQ(s.issues("d"), "");
    CB_CHECK_EQ(s.issues("e"), "conflict:bytearray.append");
    CB_CHECK_EQ(s.returns("f"), "int[0..4294967295]");
    CB_CHECK_EQ(s.returns("g"), "int[-32768..32767]");
    CB_CHECK_EQ(s.issues("h"), "dynamic:int.from_bytes byte order");
});

CB_TEST("translate.python.methods.list_methods_grow_the_container", [] {
    Solved s(
        "def a() -> list[int]:\n    xs = []\n    xs.append(1)\n    xs.append(300)\n    return xs\n"
        "def b() -> int:\n    xs = [1]\n    xs.extend([2, 3])\n    return xs.pop()\n"
        "def c(xs: list[int]) -> list[int]:\n    xs.sort(key=lambda v: v)\n    return xs\n"
        "def d(xs: list[int]) -> list[int]:\n    xs.sort(reverse=True, cmp=1)\n    return xs\n"
        "def e(xs: list[int]) -> int:\n    return xs.index(3)\n");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.returns("b"), "int[1..3]");
    CB_CHECK_EQ(s.issues("c"), "");
    CB_CHECK_EQ(s.issues("d"), "syntax:list.sort(cmp=)");
    CB_CHECK_EQ(s.returns("e"), "int[0..140737488355328]");
});

CB_TEST("translate.python.methods.dict_and_set_methods", [] {
    Solved s(
        "def a() -> int:\n    r = {'a': 1}\n    return r.get('a', 0)\n"
        "def b(d: dict[str, int]) -> int:\n    return d.get('zz', 5)\n"
        "def c(d: dict[str, int]) -> int:\n    d.update(3)\n    return 1\n"
        "def e() -> bool:\n    s = set()\n    s.add(1)\n    return s.isdisjoint(s)\n"
        "def f() -> int:\n    s = {1}\n    s.update([2, 3])\n    s.discard(1)\n    return len(s)\n");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.returns("a"), "int[0..1]");   // the record's field joined with the default
    CB_CHECK_EQ(s.issues("b"), "int-range:return value in [None, None]");   // an int the annotation does not bound
    CB_CHECK_EQ(s.issues("c"), "syntax:dict.update arguments");
    CB_CHECK_EQ(s.issues("e"), "");
    CB_CHECK_EQ(s.issues("f"), "");
});

CB_TEST("translate.python.methods.builtin_functions", [] {
    Solved s(
        "def a(s: str, t: str) -> int:\n    return min(len(s), len(t))\n"
        "def b() -> int:\n    return max([1, 5, 3])\n"
        "def c(s: str) -> int:\n    return abs(len(s) - 4)\n"
        "def d(xs: list[str]) -> list[str]:\n    return sorted(xs)\n"
        "def e() -> int:\n    return sum([1, 2, 3])\n"
        "def f(x: str) -> int:\n    return ord(x)\n"
        "def g(s: str) -> str:\n    return chr(len(s))\n"
        "def h() -> bool:\n    return any(x > 1 for x in [1, 2])\n"
        "def i(s: str) -> tuple[int, int]:\n    return divmod(len(s), 7)\n"
        "def j(x: str) -> int:\n    return int(x)\n"
        "def k(x: int) -> list[str]:\n    return list(map(str, [x]))\n"
        "def l(x: str) -> int:\n    return getattr(x, 'a')\n"
        "def m(x: str) -> int:\n    return frobnicate(x)\n");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.returns("a"), "int[0..140737488355328]");   // the smaller of two sizes
    CB_CHECK_EQ(s.issues("b"), "");
    CB_CHECK_EQ(s.returns("b"), "int[1..5]");   // the join of the list's items
    CB_CHECK_EQ(s.returns("c"), "int[0..140737488355324]");   // |size - 4|: the larger end is 2**47 - 4
    CB_CHECK(s.returns("d").starts_with("list#"));
    CB_CHECK_EQ(s.returns("e"), "int[0..422212465065984]");   // three at most, a list holds at most 2**47 items
    CB_CHECK_EQ(s.issues("e"), "");
    CB_CHECK_EQ(s.returns("f"), "int[0..1114111]");
    CB_CHECK_EQ(s.returns("g"), "str");
    CB_CHECK_EQ(s.issues("h"), "");
    CB_CHECK_EQ(s.returns("i"), "tuple[int[0..20105355479332], int[0..6]]");   // size // 7 and size % 7
    CB_CHECK_EQ(s.issues("j"), "int-range:return value in [None, None]");   // int(str) is an unbounded int: refused until bounded
    CB_CHECK_EQ(s.issues("k"), "dynamic:map");
    CB_CHECK_EQ(s.issues("l"), "dynamic:getattr");
    CB_CHECK_EQ(s.issues("m"), "unresolved:frobnicate");
});

CB_TEST("translate.python.methods.isinstance_is_decided_by_the_types_or_refused", [] {
    Solved s(
        "def a(x: int) -> bool:\n    return isinstance(x, int)\n"
        "def b(x: str) -> bool:\n    return isinstance(x, (int, bytes))\n"
        "def c(x: int | None) -> bool:\n    return isinstance(x, int)\n"
        "def d(x: int) -> bool:\n    return isinstance(x, Frob)\n");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.issues("b"), "");
    CB_CHECK_EQ(s.issues("c"), "dynamic:isinstance of an optional value (use `is not None`)");
    CB_CHECK_EQ(s.issues("d"), "dynamic:isinstance Frob | unresolved:Frob");
});

CB_TEST("translate.python.methods.struct_and_the_standard_library", [] {
    Solved s(
        "import struct\nimport os\nimport zlib\nimport math\nimport re\n"
        "def a(d: bytes) -> int:\n    return struct.unpack('<HI', d)[1]\n"
        "def b(d: bytes) -> int:\n    return struct.calcsize('<HI')\n"
        "def c(d: bytes, f: str) -> int:\n    return struct.unpack(f, d)[0]\n"
        "def d(n: int) -> bytes:\n    return struct.pack('<I', n)\n"
        "def e(n: int) -> bytes:\n    return struct.pack('<I', 'x')\n"
        "def f(a: str, b: str) -> str:\n    return os.path.join(a, b)\n"
        "def g(d: bytes) -> int:\n    return zlib.crc32(d)\n"
        "def h(x: float) -> int:\n    return math.floor(x)\n"
        "def i(s: str) -> int:\n    return re.match('a', s)\n"
        "def j(d: bytes) -> int:\n    return struct.unpack('Z', d)[0]\n");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.returns("a"), "int[0..4294967295]");
    CB_CHECK_EQ(s.returns("b"), "int[6..6]");
    CB_CHECK_EQ(s.issues("c"), "dynamic:struct.unpack with a computed format");
    CB_CHECK_EQ(s.returns("d"), "bytes(<=4)");
    CB_CHECK_EQ(s.issues("e"), "conflict:struct.pack of a str for I");
    CB_CHECK_EQ(s.returns("f"), "str");
    CB_CHECK_EQ(s.returns("g"), "int[0..4294967295]");
    CB_CHECK_EQ(s.issues("i"), "stdlib:re.match");
    CB_CHECK_EQ(s.issues("j"), "stdlib:struct.unpack('Z')");
});

CB_TEST("translate.python.methods.a_lambda_where_a_call_takes_none_is_dynamic", [] {
    Solved s("def a(xs: list[int]) -> int:\n    return len(sorted(xs, key=lambda v: v))\n"
             "def b(s: str) -> str:\n    return s.upper(lambda: 1)\n");
    CB_CHECK_EQ(s.issues("a"), "");
    CB_CHECK_EQ(s.issues("b"), "dynamic:a lambda or generator as an argument");
});
