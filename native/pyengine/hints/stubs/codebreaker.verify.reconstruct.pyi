from codebreaker.engine.native_engine import FunctionStore
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
)


def _kinds() -> Dict[str, str]: ...


def _reaches(
    texts: <class 'Union'>[Dict[str, str], codebreaker.translate.l2._Texts],
    built: List[str],
    dropped: Dict[Any, Any]
) -> Set[Any]: ...


def digests() -> Dict[str, str]: ...


def identity(module_path: str) -> Tuple[str, str]: ...


def kind_name(module_path: str) -> <class 'Union'>[str, None]: ...


def run(
    module_path: str,
    fs: <class 'Union'>[FunctionStore, None] = ...,
    log: Callable = ...
) -> Tuple[str, str]: ...


def signatures(
    m: <class 'Union'>[codebreaker.verify.lanes.java.Prepared, codebreaker.translate.l2.Made],
    whole: <class 'Union'>[str, None],
    own: <class 'Union'>[Dict[str, str], None] = ...
) -> Dict[str, str]: ...

from typing import (
    Dict,
    Tuple,
)


def _kinds() -> Dict[str, str]: ...


def digests() -> Dict[str, str]: ...


def identity(module_path: str) -> Tuple[str, str]: ...


def kind_name(module_path: str) -> str: ...

from typing import Dict


def _kinds() -> Dict[str, str]: ...
