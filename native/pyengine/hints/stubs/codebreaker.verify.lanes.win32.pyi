from codebreaker.emulate.run import Run
from codebreaker.verify.concolic import Exploration
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _argc(dll: str, name: str) -> <class 'Union'>[int, None]: ...


def _branch_run(
    path: str,
    func: int,
    spec: Dict[str, <class 'Union'>[<class 'Union'>, List[int], List[<class 'Union'>]]]
) -> Dict[str, <class 'Union'>]: ...


def _confirm_vectors(arity: int, lane: str) -> List[<class 'Union'>]: ...


def _coverage(r: Run) -> Any: ...


def _derive(
    v: Dict[str, List[int]],
    values: Dict[str, int],
    names: Dict[str, <class 'Union'>],
    shape: None
) -> Dict[str, <class 'Union'>]: ...


def _effects(r: Run, own: <class 'Union'>[None, Tuple[int, int]]) -> str: ...


def _mode() -> str: ...


def _paths_profile() -> List[<class 'Union'>]: ...


def _prov_meminit() -> List[Tuple[int, bytes]]: ...


def _stub_options(original: str) -> Dict[str, <class 'Union'>]: ...


def _thunk_stubs(original: str) -> Tuple[Dict[int, Tuple[str, str]], Dict[str, int]]: ...


def _vectors(arity: int, lane: str) -> List[<class 'Union'>]: ...


def _writable(path: str) -> List[Tuple[int, int]]: ...


def concolic_ready() -> bool: ...


def explore_vectors(
    path: str,
    func: int,
    seeds: List[<class 'Union'>],
    shape: None = ...
) -> Tuple[List[Dict[str, <class 'Union'>]], Exploration]: ...


def profile(module_path: str) -> Tuple[str, str]: ...


def settings_profile(lane: str) -> Tuple[str, str]: ...


class PELane:
    def functions(self, path: str) -> List[int]: ...
    def key(self, va: int) -> str: ...
