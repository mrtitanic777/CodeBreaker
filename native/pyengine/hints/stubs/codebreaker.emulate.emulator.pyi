from codebreaker.emulate.run import Run
from codebreaker.formats.pe import PEFile
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _builtin_argc(name: str) -> None: ...


def _load(module: str) -> Tuple[str, int, bytes, str, Dict[int, Dict[str, int]], str, Dict[int, Tuple[str, str]]]: ...


def _outs(name: str) -> List[Any]: ...


def _pe_code_image(
    module: str,
    path: str,
    pe: PEFile
) -> Tuple[int, bytearray, str, Dict[int, Dict[str, int]]]: ...


def _resolve_func(module: str, func: int) -> Tuple[int, int, bytes, int, str]: ...


def _resolve_func_(module: str, func: int) -> Tuple[int, int, bytes, int, str]: ...


def _sha256(path: str) -> str: ...


def record(rec: Dict[str, <class 'Union'>], sentinels: None = ...) -> Run: ...


def recorded(module: str, **kw) -> Run: ...


def trace_function(
    module: str,
    func: <class 'Union'>[int, None] = ...,
    args: <class 'Union'>[List[int], None] = ...,
    arity: None = ...,
    insn_cap: int = ...,
    mem_init: <class 'Union'>[List[Tuple[int, bytes]], None] = ...,
    stub_externals: <class 'Union'>[bool, None] = ...,
    stub_ret: int = ...,
    stub_cleanup: int = ...,
    sig_resolver: <class 'Union'>[Callable, None] = ...,
    stub_ret_unique: bool = ...,
    overlay: None = ...,
    sentinels: None = ...,
    reads: bool = ...,
    stop_wild: bool = ...,
    quiet_stack: bool = ...,
    reuse: bool = ...,
    fast: bool = ...,
    lean: bool = ...,
    ecx: None = ...,
    edx: None = ...,
    pads: <class 'Union'>[None, Tuple[int, int]] = ...,
    branches: bool = ...,
    stub_at: <class 'Union'>[None, Dict[int, Tuple[str, str]]] = ...,
    shadow: <class 'Union'>[codebreaker.emulate.native_shadow.Shadow, None] = ...
) -> Dict[str, <class 'Union'>]: ...

from codebreaker.emulate.run import Run
from codebreaker.formats.pe import PEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _builtin_argc(name: str) -> None: ...


def _load(module: str) -> Tuple[str, int, bytes, str, Dict[int, Dict[str, int]], str, Dict[int, Tuple[str, str]]]: ...


def _outs(name: str) -> List[Any]: ...


def _pe_code_image(
    module: str,
    path: str,
    pe: PEFile
) -> Tuple[int, bytearray, str, Dict[int, Dict[str, int]]]: ...


def _resolve_func(module: str, func: int) -> Tuple[int, int, bytes, int, str]: ...


def _resolve_func_(module: str, func: int) -> Tuple[int, int, bytes, int, str]: ...


def _sha256(path: str) -> str: ...


def record(rec: Dict[str, <class 'Union'>], sentinels: None = ...) -> Run: ...


def recorded(module: str, **kw) -> Run: ...


def trace_function(
    module: str,
    func: <class 'Union'>[int, None] = ...,
    args: <class 'Union'>[List[int], None] = ...,
    arity: None = ...,
    insn_cap: int = ...,
    mem_init: <class 'Union'>[List[Tuple[int, bytes]], None] = ...,
    stub_externals: <class 'Union'>[bool, None] = ...,
    stub_ret: int = ...,
    stub_cleanup: int = ...,
    sig_resolver: <class 'Union'>[Callable, None] = ...,
    stub_ret_unique: bool = ...,
    overlay: None = ...,
    sentinels: None = ...,
    reads: bool = ...,
    stop_wild: bool = ...,
    quiet_stack: bool = ...,
    reuse: bool = ...,
    fast: bool = ...,
    lean: bool = ...,
    ecx: None = ...,
    edx: None = ...,
    pads: None = ...,
    branches: bool = ...,
    stub_at: None = ...,
    shadow: None = ...
) -> Dict[str, <class 'Union'>]: ...

from capstone import Cs
from codebreaker.emulate.run import Run
from codebreaker.formats.native_pe import PEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _address(
    uc: UcIntel,
    mem: <class 'Union'>[Tuple[int, None, int, int], Tuple[None, None, int, int]]
) -> int: ...


def _branch_hook(rec: Dict[str, <class 'Union'>], md: Cs, sha: str, lo: int, hi: int) -> Callable: ...


def _builtin_argc(name: str) -> None: ...


def _callee_pops(uc: UcIntel, ret: int) -> int: ...


def _decode_branchy(uc: UcIntel, md: Cs, sha: str, address: int) -> Any: ...


def _load(module: str) -> Tuple[str, int, bytes, str, Dict[int, Dict[str, int]], str, Dict[int, Tuple[str, str]]]: ...


def _md() -> Cs: ...


def _operand_value(
    uc: UcIntel,
    op: <class 'Union'>[Tuple[str, int, int], Tuple[str, Tuple[int, str], int]],
    loaded: Dict[str, Tuple[int, int]]
) -> <class 'Union'>[Tuple[int, None], Tuple[int, int]]: ...


def _outs(name: str) -> List[Any]: ...


def _pattern(base: int) -> bytes: ...


def _pe_code_image(
    module: str,
    path: str,
    pe: PEFile
) -> Tuple[int, bytearray, str, Dict[int, Dict[str, int]]]: ...


def _resolve_func(module: str, func: int) -> Tuple[int, int, bytes, int, str]: ...


def _resolve_func_(module: str, func: int) -> Tuple[int, int, bytes, int, str]: ...


def _sha256(path: str) -> str: ...


def _straight_run(md: Cs, image: bytes, lo: int, address: int, size: int) -> Any: ...


def _uc_reg(name: str) -> int: ...


def record(rec: Dict[str, <class 'Union'>], sentinels: None = ...) -> Run: ...


def recorded(module: str, **kw) -> Run: ...


def trace_function(
    module: str,
    func: <class 'Union'>[int, None] = ...,
    args: <class 'Union'>[List[int], None] = ...,
    arity: None = ...,
    insn_cap: int = ...,
    mem_init: <class 'Union'>[List[Tuple[int, bytes]], None] = ...,
    stub_externals: <class 'Union'>[bool, None] = ...,
    stub_ret: int = ...,
    stub_cleanup: int = ...,
    sig_resolver: <class 'Union'>[None, Callable] = ...,
    stub_ret_unique: bool = ...,
    overlay: None = ...,
    sentinels: None = ...,
    reads: bool = ...,
    stop_wild: bool = ...,
    quiet_stack: bool = ...,
    reuse: bool = ...,
    fast: bool = ...,
    lean: bool = ...,
    ecx: None = ...,
    edx: None = ...,
    pads: <class 'Union'>[None, Tuple[int, int]] = ...,
    branches: bool = ...,
    stub_at: <class 'Union'>[None, Dict[int, Tuple[str, str]]] = ...,
    shadow: <class 'Union'>[codebreaker.emulate.shadow.Shadow32, None] = ...
) -> Dict[str, <class 'Union'>]: ...
