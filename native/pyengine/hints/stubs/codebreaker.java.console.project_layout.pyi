from typing import (
    Dict,
    Optional,
    Union,
)


def _existing_guid(vcx_path: str) -> <class 'Union'>[str, None]: ...


def _has_flat_sources(d: str) -> bool: ...


def _move_flat_into(root: str, proj_dir: str, name: str) -> int: ...


def _sln_text(name: str, guid: str) -> str: ...


def _write_vcxproj_from_template(vcx_path: str, guid: str) -> None: ...


def layout(root: str, name: None = ..., regen: bool = ...) -> Dict[str, <class 'Union'>]: ...


def project_guid(name: str) -> str: ...


def write_solution(root: str, name: str, guid: str) -> str: ...

from typing import (
    Dict,
    Optional,
    Union,
)


def _existing_guid(vcx_path: str) -> <class 'Union'>[str, None]: ...


def _has_flat_sources(d: str) -> bool: ...


def _move_flat_into(root: str, proj_dir: str, name: str) -> int: ...


def _sln_text(name: str, guid: str) -> str: ...


def _write_vcxproj_from_template(vcx_path: str, guid: str) -> None: ...


def layout(root: str, name: None = ..., regen: bool = ...) -> Dict[str, <class 'Union'>]: ...


def project_guid(name: str) -> str: ...


def write_solution(root: str, name: str, guid: str) -> str: ...
