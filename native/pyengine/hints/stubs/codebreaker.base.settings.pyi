from ctypes import CDLL
from typing import (
    Dict,
    Optional,
    Union,
)


def _load() -> Dict[str, <class 'Union'>]: ...


def build_cache() -> str: ...


def corpora() -> Dict[str, str]: ...


def data_root() -> str: ...


def dosbox_x(require: bool = ...) -> str: ...


def export_for_carried_over_code() -> None: ...


def get(key: str) -> str: ...


def load_native(path: str) -> CDLL: ...


def main_checkout() -> str: ...


def repo_root() -> str: ...


def state(*parts) -> str: ...


def tool(key: str, name: None = ...) -> str: ...

from typing import (
    Dict,
    Union,
)


def _load() -> Dict[str, <class 'Union'>]: ...


def corpora() -> Dict[str, str]: ...


def data_root() -> str: ...


def dosbox_x(require: bool = ...) -> str: ...


def export_for_carried_over_code() -> None: ...


def get(key: str) -> <class 'Union'>[int, str]: ...


def repo_root() -> str: ...


def state(*parts) -> str: ...
