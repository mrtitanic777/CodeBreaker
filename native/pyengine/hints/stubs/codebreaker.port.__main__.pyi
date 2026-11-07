from codebreaker.port.pre_ansi import Port
from subprocess import CompletedProcess
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def args(tpl: List[str], **kw) -> List[str]: ...


def arith16_pass(
    port: Port,
    out: str,
    res: Dict[str, str],
    compilers: Dict[str, <class 'Union'>]
) -> int: ...


def build(
    spec: Dict[str, <class 'Union'>],
    cc: str,
    tpl: List[str],
    out: str,
    srcs: List[str],
    exe: str,
    objdir: str
) -> CompletedProcess: ...


def changed_files(res: Dict[str, str], before: Dict[str, str], cfiles: List[str]) -> List[str]: ...


def check_key(spec: Dict[str, <class 'Union'>], cc: str, n: str, res: Dict[str, str]) -> str: ...


def env_for(cc: str) -> Dict[str, str]: ...


def find(names: List[str], given: <class 'Union'>[str, None] = ...) -> str: ...


def load_compilers() -> Dict[str, <class 'Union'>]: ...


def main(argv: List[str]) -> int: ...


def memory_guard(out: str) -> None: ...


def memory_used() -> int: ...


def oldlayout_pass(
    port: Port,
    out: str,
    res: Dict[str, str],
    compilers: Dict[str, <class 'Union'>]
) -> int: ...


def read_bss(src: str) -> Dict[Any, Any]: ...


def read_symbols(src: str) -> Dict[Any, Any]: ...


def read_tree(src: str) -> Tuple[Dict[str, str], Dict[Any, Any], Dict[Any, Any]]: ...


def report(
    out: str,
    src: str,
    comp: str,
    cc: str,
    port: Port,
    roles: Dict[Any, Any],
    diags: List[<class 'Union'>],
    rounds: int,
    built: bool,
    blog: str,
    profile: Dict[str, <class 'Union'>],
    sanitized: List[Any] = ...,
    asm_notes: List[Any] = ...
) -> None: ...


def run(spec: Dict[str, <class 'Union'>], argv: List[str], cwd: str, cc: str) -> CompletedProcess: ...
