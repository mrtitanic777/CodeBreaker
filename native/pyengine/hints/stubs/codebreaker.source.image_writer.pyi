from codebreaker.translate.l2 import Assembly
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Union,
)


def _items(row: List[int]) -> List[str]: ...


def _printable(b: int) -> bool: ...


def _size(
    insn: <class 'Union'>[codebreaker.decompile.m6502.Insn, codebreaker.decompile.sm83.Insn, codebreaker.decompile.z80.Insn]
) -> int: ...


def assembled(
    r: Dict[str, <class 'Union'>],
    build: Callable,
    original: bytes,
    built_path: <class 'Union'>[str, None] = ...,
    what: str = ...,
    same: <class 'Union'>[Callable, None] = ...
) -> bytes: ...


def commented(lines: List[str], what: str) -> List[str]: ...


def covered(
    code: Dict[int, <class 'Union'>[codebreaker.decompile.m68k.Insn, codebreaker.decompile.sm83.Insn, codebreaker.decompile.z80.Insn, codebreaker.decompile.m6502.Insn]],
    size: Callable = ...
) -> Set[int]: ...


def data_lines(data: List[<class 'Union'>[int, Any]], db: str = ..., fill: str = ...) -> List[<class 'Union'>[str, Any]]: ...


def emit(
    out: Assembly,
    start: int,
    end: int,
    byte: Callable,
    address: Callable,
    code_at: Callable,
    labels: Dict[int, str],
    functions: Dict[int, Function],
    text: Callable,
    lines: Callable,
    special: None = ...,
    size: Callable = ...
) -> None: ...


def functions(starts: List[int], names: Dict[int, str], fmt: str) -> Dict[int, Function]: ...


def labels(targets: Set[int], names: Dict[int, str], line_start: Callable, fmt: str) -> Dict[int, str]: ...


def lines(
    first: List[str] = ...,
    syntax: <class 'Union'>[str, None] = ...,
    module: <class 'Union'>[str, None] = ...
) -> Assembly: ...


def result(
    folder: str,
    name: str,
    source: str,
    functions: Dict[int, Function],
    code: Dict[int, <class 'Union'>[codebreaker.decompile.m68k.Insn, codebreaker.decompile.sm83.Insn, codebreaker.decompile.z80.Insn, codebreaker.decompile.m6502.Insn]],
    size: Callable = ...,
    **more
) -> Dict[str, <class 'Union'>]: ...


def text(out: Assembly) -> str: ...


def walk(
    starts: List[int],
    decode: Callable,
    inside: Callable,
    fits: Callable,
    follows: Callable,
    stops: Callable,
    key: <class 'Union'>[Callable, None] = ...,
    calls: <class 'Union'>[Callable, None] = ...,
    size: Callable = ...
) -> <class 'Union'>[Tuple[Dict[int, codebreaker.decompile.m6502.Insn], Set[int], List[int]], Tuple[Dict[int, codebreaker.decompile.m68k.Insn], Set[int], List[int]], Tuple[Dict[int, codebreaker.decompile.sm83.Insn], Set[int], List[int]], Tuple[Dict[int, codebreaker.decompile.z80.Insn], Set[int], List[int]]]: ...


class Function:
    def __init__(self, name: str) -> None: ...

from codebreaker.decompile.m6502 import Insn
from codebreaker.translate.l2 import Assembly
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Union,
)


def _items(row: List[int]) -> List[str]: ...


def _printable(b: int) -> bool: ...


def _size(insn: Insn) -> int: ...


def assembled(
    r: Dict[str, <class 'Union'>],
    build: Callable,
    original: bytes,
    built_path: <class 'Union'>[str, None] = ...,
    what: str = ...,
    same: None = ...
) -> bytes: ...


def commented(lines: List[str], what: str) -> List[str]: ...


def covered(
    code: Dict[int, <class 'Union'>[codebreaker.decompile.m68k.Insn, codebreaker.decompile.m6502.Insn]],
    size: Callable = ...
) -> Set[int]: ...


def data_lines(data: List[<class 'Union'>[Any, int]], db: str = ..., fill: str = ...) -> List[<class 'Union'>[Any, str]]: ...


def functions(starts: List[int], names: Dict[int, str], fmt: str) -> Dict[int, Function]: ...


def labels(targets: Set[int], names: Dict[int, str], line_start: Callable, fmt: str) -> Dict[int, str]: ...


def lines(
    first: List[str] = ...,
    syntax: <class 'Union'>[str, None] = ...,
    module: <class 'Union'>[str, None] = ...
) -> Assembly: ...


def result(
    folder: str,
    name: str,
    source: str,
    functions: Dict[int, Function],
    code: Dict[int, <class 'Union'>[codebreaker.decompile.m68k.Insn, codebreaker.decompile.m6502.Insn]],
    size: Callable = ...,
    **more
) -> Dict[str, <class 'Union'>]: ...


def text(out: Assembly) -> str: ...


def walk(
    starts: List[int],
    decode: Callable,
    inside: Callable,
    fits: Callable,
    follows: Callable,
    stops: Callable,
    key: <class 'Union'>[Callable, None] = ...,
    calls: None = ...,
    size: Callable = ...
) -> <class 'Union'>[Tuple[Dict[int, codebreaker.decompile.m68k.Insn], Set[int], List[int]], Tuple[Dict[int, codebreaker.decompile.m6502.Insn], Set[int], List[int]]]: ...


class Function:
    def __init__(self, name: str) -> None: ...

from codebreaker.translate.l2 import Assembly
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Union,
)


def _items(row: List[int]) -> List[str]: ...


def _printable(b: int) -> bool: ...


def _size(
    insn: <class 'Union'>[codebreaker.decompile.z80.Insn, codebreaker.decompile.m6502.Insn, codebreaker.decompile.sm83.Insn]
) -> int: ...


def assembled(
    r: Dict[str, <class 'Union'>],
    build: Callable,
    original: bytes,
    built_path: <class 'Union'>[str, None] = ...,
    what: str = ...,
    same: <class 'Union'>[Callable, None] = ...
) -> bytes: ...


def commented(lines: List[str], what: str) -> List[str]: ...


def covered(
    code: Dict[int, <class 'Union'>[codebreaker.decompile.m6502.Insn, codebreaker.decompile.z80.Insn, codebreaker.decompile.sm83.Insn]],
    size: Callable = ...
) -> Set[int]: ...


def data_lines(data: List[<class 'Union'>[Any, int]], db: str = ..., fill: str = ...) -> List[<class 'Union'>[Any, str]]: ...


def emit(
    out: Assembly,
    start: int,
    end: int,
    byte: Callable,
    address: Callable,
    code_at: Callable,
    labels: Dict[int, str],
    functions: Dict[int, Function],
    text: Callable,
    lines: Callable,
    special: None = ...,
    size: Callable = ...
) -> None: ...


def functions(starts: List[int], names: Dict[int, str], fmt: str) -> Dict[int, Function]: ...


def labels(targets: Set[int], names: Dict[int, str], line_start: Callable, fmt: str) -> Dict[int, str]: ...


def lines(
    first: List[str] = ...,
    syntax: <class 'Union'>[str, None] = ...,
    module: <class 'Union'>[str, None] = ...
) -> Assembly: ...


def result(
    folder: str,
    name: str,
    source: str,
    functions: Dict[int, Function],
    code: Dict[int, <class 'Union'>[codebreaker.decompile.m6502.Insn, codebreaker.decompile.z80.Insn, codebreaker.decompile.sm83.Insn]],
    size: Callable = ...,
    **more
) -> Dict[str, <class 'Union'>]: ...


def text(out: Assembly) -> str: ...


def walk(
    starts: List[int],
    decode: Callable,
    inside: Callable,
    fits: Callable,
    follows: Callable,
    stops: Callable,
    key: <class 'Union'>[Callable, None] = ...,
    calls: <class 'Union'>[Callable, None] = ...,
    size: Callable = ...
) -> <class 'Union'>[Tuple[Dict[int, codebreaker.decompile.m6502.Insn], Set[int], List[int]], Tuple[Dict[int, codebreaker.decompile.sm83.Insn], Set[int], List[int]], Tuple[Dict[int, codebreaker.decompile.z80.Insn], Set[int], List[int]]]: ...


class Function:
    def __init__(self, name: str) -> None: ...
