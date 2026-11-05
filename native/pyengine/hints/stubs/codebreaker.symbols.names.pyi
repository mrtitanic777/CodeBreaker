from codebreaker.engine.native_engine import FunctionStore
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
)


def _bodies32(path: str) -> Dict[int, str]: ...


def _by_segment(symbols: Dict[Any, Any]) -> List[Any]: ...


def _debug(module_path: str) -> Tuple[Dict[Any, Any], Dict[Any, Any]]: ...


def _debug_names(module_path: str, files: Dict[Any, Any]) -> Dict[Any, Any]: ...


def _digest(code: bytes, masked: Set[int], last: int) -> <class 'Union'>[str, None]: ...


def _plain(name: str) -> str: ...


def _read_sym(path: str) -> List[Tuple[int, Dict[int, str]]]: ...


def _starts(module_path: str) -> List[<class 'Union'>[int, Any]]: ...


def _sym_file(stem: str) -> List[str]: ...


def bodies(module_path: str) -> Dict[int, str]: ...


def codeview(data: bytes) -> Dict[Any, Any]: ...


def debug_names(module_path: str) -> Dict[Any, Any]: ...


def index(
    module_path: str,
    fs: <class 'Union'>[FunctionStore, None] = ...,
    own: <class 'Union'>[Dict[int, Tuple[str, str]], None] = ...
) -> int: ...


def key(module_path: str, address: int) -> str: ...


def main(argv: List[str]) -> int: ...


def of_module(
    module_path: str,
    fs: <class 'Union'>[FunctionStore, None] = ...
) -> Dict[str, <class 'Union'>[List[Any], List[<class 'Union'>]]]: ...


def own_names(module_path: str) -> Dict[int, Tuple[str, str]]: ...


def read_sym(path: str) -> List[Tuple[int, Dict[int, str]]]: ...


def sym_objects(path: str) -> Dict[int, str]: ...
