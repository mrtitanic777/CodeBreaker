from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _cmdline(stem: str) -> str: ...


def _masm_exe(work: str, masmroot: str) -> str: ...


def _read_out(work: str, stem: str) -> Dict[str, <class 'Union'>]: ...


def _run_tool(exe: str, work: str, cmdline: str, fast: bool = ...) -> Tuple[int, bytes]: ...


def _stem_path(work: str, stem: str, ext: str) -> str: ...


def assemble(asm_src: str, stem: str = ..., masmroot: <class 'Union'>[str, None] = ...) -> Dict[str, <class 'Union'>]: ...


def available() -> bool: ...


def enabled(tool: str = ...) -> bool: ...


def ml(args: str, files: Dict[str, str], artifacts: List[str], root: None = ...) -> Tuple[Dict[str, bytes], str]: ...


def ml_files(root: None = ...) -> Dict[str, str]: ...


def run_extended(
    tool_files: Dict[str, str],
    exe: str,
    cmdline: str,
    files: Dict[str, str],
    artifacts: List[str],
    env: bytes = ...
) -> Tuple[Dict[str, bytes], bytes]: ...

from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _cmdline(stem: str) -> str: ...


def _masm_exe(work: str, masmroot: str) -> str: ...


def _read_out(work: str, stem: str) -> Dict[str, <class 'Union'>]: ...


def _run_tool(exe: str, work: str, cmdline: str, fast: bool = ...) -> Tuple[int, bytes]: ...


def _stem_path(work: str, stem: str, ext: str) -> str: ...


def assemble(asm_src: str, stem: str = ..., masmroot: <class 'Union'>[str, None] = ...) -> Dict[str, <class 'Union'>]: ...


def available() -> bool: ...


def enabled(tool: str = ...) -> bool: ...


def ml(args: str, files: Dict[str, str], artifacts: List[str], root: None = ...) -> Tuple[Dict[str, bytes], str]: ...


def ml_files(root: None = ...) -> Dict[str, str]: ...


def run_extended(
    tool_files: Dict[str, str],
    exe: str,
    cmdline: str,
    files: Dict[str, str],
    artifacts: List[str],
    env: bytes = ...
) -> Tuple[Dict[str, bytes], bytes]: ...
