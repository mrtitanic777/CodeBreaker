from capstone import (
    Cs,
    CsInsn,
)
from codebreaker.formats.ne import NEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
)


def _decoded(md: Cs, data: bytes, at: int, bits: int = ...) -> CsInsn: ...


def check_piece(obj: bytes, f: Function, lo: int, hi: int) -> str: ...


def differences(code: bytes, mask: bytes, f: Function) -> List[Any]: ...


class Function:
    def __init__(
        self,
        nef: NEFile,
        seg: int,
        start: int,
        end: int,
        public: str,
        symbol_for: Callable,
        cs_ok: int = ...,
        extra_entries: Tuple[()] = ...,
        whole: bool = ...,
        holes: Tuple[()] = ...
    ) -> None: ...
    def _far_pointer_offsets(self, md: Cs, symbol_for: Callable, selectors: Dict[Any, Any], seg: int) -> None: ...
    def _raw_far_pointers(self, symbol_for: Callable, internal_selector: Dict[Any, Any]) -> None: ...
    def _stays_inside(self, at: int, lo: int, hi: int) -> bool: ...
    def _used(self, label: int, lo: int, hi: int) -> bool: ...
    def mnemonic(self, at: int) -> str: ...
    def write(self, good: Set[int], lo: <class 'Union'>[int, None] = ..., hi: <class 'Union'>[int, None] = ...) -> str: ...

from capstone import (
    Cs,
    CsInsn,
)
from codebreaker.formats.ne import NEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
)


def _decoded(md: Cs, data: bytes, at: int, bits: int = ...) -> CsInsn: ...


def check_piece(obj: bytes, f: Function, lo: int, hi: int) -> str: ...


def differences(code: bytes, mask: bytes, f: Function) -> List[Any]: ...


class Function:
    def __init__(
        self,
        nef: NEFile,
        seg: int,
        start: int,
        end: int,
        public: str,
        symbol_for: Callable,
        cs_ok: int = ...,
        extra_entries: Tuple[()] = ...,
        whole: bool = ...,
        holes: Tuple[()] = ...
    ) -> None: ...
    def _far_pointer_offsets(self, md: Cs, symbol_for: Callable, selectors: Dict[Any, Any], seg: int) -> None: ...
    def _raw_far_pointers(self, symbol_for: Callable, internal_selector: Dict[Any, Any]) -> None: ...
    def _stays_inside(self, at: int, lo: int, hi: int) -> bool: ...
    def _used(self, label: int, lo: int, hi: int) -> bool: ...
    def mnemonic(self, at: int) -> str: ...
    def write(self, good: Set[int], lo: <class 'Union'>[int, None] = ..., hi: <class 'Union'>[int, None] = ...) -> str: ...
