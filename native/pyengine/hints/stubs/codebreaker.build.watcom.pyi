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
