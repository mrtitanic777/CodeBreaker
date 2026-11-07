from capstone import (
    Cs,
    CsInsn,
)
from codebreaker.formats.le import LEFile
from codebreaker.formats.ne import NEFile
from codebreaker.platforms import Cpu
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _arch(machine: int) -> Cpu: ...


def _bare(name: str) -> str: ...


def _caller_cleanup(ins: List[CsInsn], j: int) -> int: ...


def _calls_followed(code: bytes, start: int, limit: int = ...) -> Set[int]: ...


def _cpu(name: str) -> Cpu: ...


def _far_pointer_targets(
    nef: NEFile,
    s: Dict[str, <class 'Union'>],
    code: bytes,
    segs: Dict[int, Dict[str, <class 'Union'>]]
) -> Set[Tuple[int, int]]: ...


def _ident(name: str) -> str: ...


def _is_prologue(code: bytes, o: int) -> bool: ...


def _leave_follows(ins: List[CsInsn], j: int) -> bool: ...


def _near_call_targets(code: bytes) -> Set[int]: ...


def _one(code: bytes, at: int) -> <class 'Union'>[capstone.CsInsn, None]: ...


def _push_bytes(i: CsInsn) -> int: ...


def _return_pops(i: CsInsn) -> int: ...


def address(seg: int, off: int) -> int: ...


def args_read(code: bytes, start: int, end: int, kind: str) -> None: ...


def call_overrides(d: Description) -> List[str]: ...


def callers_read_ax(d: Description, places: Set[Tuple[int, int]]) -> Set[Any]: ...


def decoder16(detail: bool = ...) -> Cs: ...


def describe16(module_path: str, outdir: str) -> Description: ...


def describe32(module_path: str, funcs: List[int], outdir: str) -> Tuple[str, Dict[int, str]]: ...


def describe_dos(module_path: str, outdir: str) -> Description: ...


def describe_le(lf: LEFile, name: str, outdir: str) -> Description: ...


def describe_nes(module_path: str, outdir: str) -> Tuple[str, Dict[int, str]]: ...


def forwards(d: Description, seg: int, off: int, kind: str) -> <class 'Union'>[int, None]: ...


def function_names(module_path: str, funcs: List[int]) -> Dict[int, str]: ...


def function_starts(module_path: str) -> List[int]: ...


def functions(nef: NEFile) -> Dict[Tuple[int, int], str]: ...


def hidden_functions(nef: NEFile, known: Set[Tuple[int, int]]) -> List[Any]: ...


def import_names16(nef: NEFile) -> Dict[str, str]: ...


def jump_by_return(ins: List[CsInsn], k: int, targets: Set[int] = ...) -> bool: ...


def jump_tables(d: Description) -> List[<class 'Union'>[Any, str]]: ...


def le_address(obj: int, off: int) -> int: ...


def model_line(d: Description, va: int, name: str, default: str = ...) -> str: ...


def popped_by_hand(code: bytes, off: int, kind: str) -> None: ...


def prologues_after_ends(code: bytes) -> List[<class 'Union'>[int, Any]]: ...


def register_arguments(code: bytes, off: int) -> bool: ...


def register_helper(code: bytes, off: int, end: <class 'Union'>[int, None] = ...) -> bool: ...


def result_bytes(
    code: bytes,
    start: int,
    end: int,
    passes: <class 'Union'>[List[Any], None] = ...,
    voided: <class 'Union'>[List[Any], None] = ...
) -> int: ...


def results(d: Description) -> Dict[int, int]: ...


def return_jumps(d: Description) -> List[Any]: ...


def returns(
    code: bytes,
    off: int,
    limit: int = ...,
    want: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[Tuple[None, int], Tuple[str, int]]: ...


def selector(seg: int) -> int: ...


def where(va: int) -> Tuple[int, int]: ...


def windows_far_prologue(code: bytes, off: int) -> bool: ...


class Description:
    def __init__(
        self,
        path: str,
        names: Dict[int, str],
        where: Dict[int, Tuple[int, int]],
        imports: List[<class 'Union'>[Any, str]]
    ) -> None: ...
    def records(self, *kinds) -> List[List[str]]: ...
    def write(self, path: <class 'Union'>[str, None] = ...) -> str: ...


class _Into:
    def __init__(self, address: int) -> None: ...

from capstone import (
    Cs,
    CsInsn,
)
from codebreaker.formats.native_le import LEFile
from codebreaker.formats.native_ne import NEFile
from codebreaker.platforms import Cpu
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _arch(machine: int) -> <class 'Union'>[codebreaker.platforms.Cpu, None]: ...


def _bare(name: str) -> str: ...


def _caller_cleanup(ins: List[CsInsn], j: int) -> int: ...


def _calls_followed(code: bytes, start: int, limit: int = ...) -> Set[int]: ...


def _cpu(name: str) -> Cpu: ...


def _far_pointer_targets(
    nef: NEFile,
    s: Dict[str, <class 'Union'>],
    code: bytes,
    segs: Dict[int, Dict[str, <class 'Union'>]]
) -> Set[Any]: ...


def _ident(name: str) -> str: ...


def _is_prologue(code: bytes, o: int) -> bool: ...


def _leave_follows(ins: List[CsInsn], j: int) -> bool: ...


def _near_call_targets(code: bytes) -> Set[int]: ...


def _one(code: bytes, at: int) -> <class 'Union'>[capstone.CsInsn, None]: ...


def _push_bytes(i: CsInsn) -> int: ...


def _return_pops(i: CsInsn) -> int: ...


def address(seg: int, off: int) -> int: ...


def call_overrides(d: Description) -> List[str]: ...


def callers_read_ax(d: Description, places: Set[Tuple[int, int]]) -> Set[Any]: ...


def decoder16(detail: bool = ...) -> Cs: ...


def describe16(module_path: str, outdir: str) -> Description: ...


def describe32(module_path: str, funcs: List[int], outdir: str) -> Tuple[str, Dict[int, str]]: ...


def describe_dos(module_path: str, outdir: str) -> Description: ...


def describe_le(
    lf: LEFile,
    name: str,
    outdir: str
) -> Description: ...


def forwards(d: Description, seg: int, off: int, kind: str) -> None: ...


def function_names(module_path: str, funcs: List[int]) -> Dict[int, str]: ...


def function_starts(module_path: str) -> List[int]: ...


def functions(nef: NEFile) -> Dict[Tuple[int, int], str]: ...


def import_names16(nef: NEFile) -> Dict[str, str]: ...


def jump_by_return(ins: List[CsInsn], k: int, targets: Set[int] = ...) -> bool: ...


def jump_tables(d: Description) -> List[Any]: ...


def le_address(obj: int, off: int) -> int: ...


def model_line(d: Description, va: int, name: str, default: str = ...) -> str: ...


def popped_by_hand(code: bytes, off: int, kind: str) -> None: ...


def prologues_after_ends(code: bytes) -> List[<class 'Union'>[int, Any]]: ...


def register_arguments(code: bytes, off: int) -> bool: ...


def register_helper(code: bytes, off: int, end: <class 'Union'>[int, None] = ...) -> bool: ...


def result_bytes(
    code: bytes,
    start: int,
    end: int,
    passes: <class 'Union'>[List[Any], None] = ...,
    voided: <class 'Union'>[List[Any], None] = ...
) -> int: ...


def results(d: Description) -> Dict[int, int]: ...


def return_jumps(d: Description) -> List[Any]: ...


def returns(
    code: bytes,
    off: int,
    limit: int = ...,
    want: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[Tuple[None, int], Tuple[str, int]]: ...


def selector(seg: int) -> int: ...


def where(va: int) -> Tuple[int, int]: ...


def windows_far_prologue(code: bytes, off: int) -> bool: ...


class Description:
    def __init__(
        self,
        path: str,
        names: Dict[int, str],
        where: Dict[int, Tuple[int, int]],
        imports: List[<class 'Union'>[Any, str]]
    ) -> None: ...
    def write(self, path: <class 'Union'>[str, None] = ...) -> str: ...


class _Into:
    def __init__(self, address: int) -> None: ...
