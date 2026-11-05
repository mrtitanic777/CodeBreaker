from typing import (
    Any,
    Callable,
    Dict,
    Optional,
    Tuple,
)


def _shift(kind: str) -> Callable: ...


def binary(op: str, a: Any, b: Any) -> Any: ...


def bits(e: Any) -> int: ...


def compare(op: str, a: Any, b: Any) -> Any: ...


def concat(
    hi: <class 'Union'>[Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, str]],
    lo: <class 'Union'>[Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, str]]
) -> <class 'Union'>[Tuple[str, int, Tuple[str, int, str], Tuple[str, int, str]], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, str]]: ...


def const(value: int, bits: int) -> Tuple[str, int, int]: ...


def extract(a: Any, lo: int, width: int) -> Any: ...


def inputs(
    e: <class 'Union'>[Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int]]], Tuple[str, int, str], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int]]],
    memo: Dict[int, Tuple[Tuple[str, int, str], frozenset]]
) -> frozenset: ...


def logic(
    op: str,
    x: <class 'Union'>[Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]]]], int]], Tuple[str, int, Tuple[str, int, str], Tuple[str, int, int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]], Tuple[str, int, int]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]]],
    y: <class 'Union'>[Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]]]], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]]]], int]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]], Tuple[str, int, int]]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], Tuple[str, int, int]], Tuple[str, int, int]]]
) -> <class 'Union'>[Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]]]], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]]]], int]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]], Tuple[str, int, int]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]]]], int]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, int], Tuple[str, int, str]], Tuple[str, int, int]], Tuple[str, int, int]]]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]]], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], Tuple[str, int, int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, str], Tuple[str, int, int]], Tuple[str, int, int]]]]: ...


def negate(c: Any) -> Any: ...


def nonzero(e: Any) -> Any: ...


def sext(
    a: Tuple[str, int, Tuple[str, int, str], int],
    to: int
) -> Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int]]: ...


def to_bool(c: Any) -> Any: ...


def unary(
    op: str,
    a: Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]]
) -> Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, Tuple[str, int, str], int]], Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, Tuple[str, int, str], int], Tuple[str, int, int]]]]]]: ...


def var(name: str, bits: int) -> Tuple[str, int, str]: ...


def z3_of(
    e: Any,
    memo: Dict[int, <class 'Union'>[Tuple[Tuple[str, int, str], z3.z3.BitVecRef], <class 'Union'>, Tuple[Tuple[str, int, int], z3.z3.BitVecNumRef]]],
    ctx: <class 'Union'>[z3.z3.Context, None] = ...
) -> <class 'Union'>[z3.z3.BitVecNumRef, z3.z3.BoolRef, z3.z3.BitVecRef]: ...


def zext(a: Any, to: int) -> Any: ...
