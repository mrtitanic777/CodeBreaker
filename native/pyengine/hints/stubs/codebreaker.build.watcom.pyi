from typing import (
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _first(*cands) -> str: ...


def _import_lines(
    obj: bytes,
    names: Set[str],
    imports: Dict[str, str],
    numbered: <class 'Union'>[Dict[str, Tuple[str, int, None]], None] = ...
) -> str: ...


def _ow() -> Dict[str, <class 'Union'>]: ...


def _ow_env(ow: Dict[str, <class 'Union'>]) -> Dict[str, str]: ...


def _scratch(*parts) -> str: ...


def import_map(module: str) -> Dict[str, str]: ...


def openwatcom_root() -> str: ...


def ow_cc(cls: str) -> str: ...


def ow_env(cls: str) -> Dict[str, str]: ...


def ow_include(cls: str) -> List[str]: ...


def ow_lib(cls: str) -> str: ...


def ow_tool(name: str) -> str: ...

from typing import (
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _first(*cands) -> str: ...


def _import_lines(
    obj: bytes,
    names: Set[str],
    imports: Dict[str, str],
    numbered: <class 'Union'>[Dict[str, Tuple[str, int, None]], None] = ...
) -> str: ...


def _ow() -> Dict[str, <class 'Union'>]: ...


def _ow_env(ow: Dict[str, <class 'Union'>]) -> Dict[str, str]: ...


def _scratch(*parts) -> str: ...


def import_map(module: str) -> Dict[str, str]: ...


def openwatcom_root() -> str: ...


def ow_cc(cls: str) -> str: ...


def ow_env(cls: str) -> Dict[str, str]: ...


def ow_include(cls: str) -> List[str]: ...


def ow_lib(cls: str) -> str: ...


def ow_tool(name: str) -> str: ...

from typing import (
    Dict,
    List,
)


def _first(*cands) -> str: ...


def _scratch(*parts) -> str: ...


def import_map(module: str) -> Dict[str, str]: ...


def openwatcom_root() -> str: ...


def ow_cc(cls: str) -> str: ...


def ow_env(cls: str) -> Dict[str, str]: ...


def ow_include(cls: str) -> List[str]: ...


def ow_lib(cls: str) -> str: ...


def ow_tool(name: str) -> str: ...
