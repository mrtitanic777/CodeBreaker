from codebreaker.emulate.run import Run
from codebreaker.formats.native_ne import NEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _api(hit: Dict[str, <class 'Union'>]) -> str: ...


def _apply_relocs(
    uc: UcIntel,
    nef: NEFile,
    sels: Dict[int, int],
    seg_bases: Dict[int, int],
    sent_sel: int,
    imports: Dict[Any, Any]
) -> None: ...


def _desc(base: int, limit: int, code: bool, dpl: int = ..., big: bool = ...) -> bytes: ...


def _import_namer(nef: NEFile) -> Callable: ...


def _kernel_constant(api: str) -> None: ...


def _patch_chain(
    uc: UcIntel,
    base: int,
    r: Dict[str, <class 'Union'>],
    tsel: int,
    toff: int
) -> None: ...


def _sel(index: int, rpl: int = ...) -> int: ...


def probe(
    dll: str,
    ordinal: None = ...,
    seg: <class 'Union'>[int, None] = ...,
    offset: <class 'Union'>[int, None] = ...,
    args: <class 'Union'>[List[int], None] = ...,
    stub: Callable = ...,
    insn_cap: <class 'Union'>[int, None] = ...,
    prelude: None = ...,
    cdecl: bool = ...,
    near: bool = ...,
    data_image: None = ...,
    callees: None = ...,
    data_end: None = ...,
    context: None = ...,
    stub_ret: <class 'Union'>[int, None] = ...,
    mem_init: <class 'Union'>[Tuple[Tuple[Tuple[str, int], bytes]], None] = ...,
    branches: None = ...,
    bp: None = ...,
    ports: bool = ...,
    shadow: <class 'Union'>[codebreaker.emulate.shadow.Shadow16, None] = ...
) -> Dict[str, <class 'Union'>]: ...


def record(rec: Dict[str, <class 'Union'>]) -> Run: ...


def recorded(dll: str, **kw) -> Run: ...


def segment_base(idx: int) -> int: ...


def selector_base(uc: UcIntel, sel: int) -> int: ...


def trace_function16(
    path: str,
    ordinal: None = ...,
    seg: <class 'Union'>[int, None] = ...,
    offset: <class 'Union'>[int, None] = ...,
    args: <class 'Union'>[List[int], None] = ...,
    arity: int = ...,
    insn_cap: int = ...,
    import_stub: <class 'Union'>[Callable, None] = ...,
    prelude: None = ...,
    near: bool = ...,
    data_image: None = ...,
    callees: None = ...,
    data_end: None = ...,
    context: None = ...,
    mem_init: <class 'Union'>[Tuple[Tuple[Tuple[str, int], bytes]], None] = ...,
    branches: None = ...,
    bp: None = ...,
    ports: bool = ...,
    shadow: <class 'Union'>[codebreaker.emulate.shadow.Shadow16, None] = ...
) -> Dict[str, <class 'Union'>]: ...


class _Ctx:
    def alloc_heap(self, size: int = ...) -> int: ...
