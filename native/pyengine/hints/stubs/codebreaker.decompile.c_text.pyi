from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _far_loads(code: bytes, start: int, end: int) -> Set[int]: ...


def _global_type(prefix: str, size: int, elem: None = ...) -> str: ...


def _loads_dgroup(code: bytes, off: int, dgroup: int) -> bool: ...


def _more_helpers(have: str) -> str: ...


def _ram_type(prefix: str, size: int, called: bool) -> str: ...


def _struct_decls(structs: List[Any], header: str, far16: bool = ...) -> List[Any]: ...


def _struct_sizes(structs: List[Any]) -> Dict[Any, Any]: ...


def compilable32(
    module_path: str,
    funcs: List[int],
    outdir: str
) -> Tuple[str, Dict[str, str], List[str], Dict[int, str], List[str], Set[Any], Set[Any], Dict[str, <class 'Union'>], Dict[str, str]]: ...


def interrupt_helper_name(vector: int, regs: Tuple[str, Ellipsis]) -> str: ...


def interrupt_helpers(
    used: <class 'Union'>[Set[Tuple[int, Tuple[str, str]]], Set[<class 'Union'>]]
) -> List[<class 'Union'>[Any, str]]: ...


def interrupt_routines(vectors: Set[int]) -> Tuple[str, str]: ...
