from codebreaker.java.profiles import Profile
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def _build_parent_map(proj_dir: str) -> Dict[str, str]: ...


def _classify(basename: str, parent_of: <class 'Union'>[Dict[str, str], None] = ...) -> str: ...


def _enum_sources(proj_dir: str) -> <class 'Union'>[Tuple[List[str], List[str]], Tuple[List[str], List[Any]]]: ...


def _find_vcxproj(proj_dir: str, name: <class 'Union'>[str, None] = ...) -> str: ...


def _guid(name: str) -> str: ...


def _reseed(p: Profile) -> None: ...


def _title_id_for(proj_dir: str) -> Tuple[None, str]: ...


def run(
    proj_dir: <class 'Union'>[str, None] = ...,
    name: <class 'Union'>[str, None] = ...,
    write_filters_too: bool = ...
) -> None: ...


def write_filters(
    proj_dir: str,
    proj_name: str,
    cpps: List[str],
    hs: List[<class 'Union'>[Any, str]]
) -> Tuple[str, int]: ...

from codebreaker.java.profiles import Profile
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def _build_parent_map(proj_dir: str) -> Dict[str, str]: ...


def _classify(basename: str, parent_of: <class 'Union'>[Dict[str, str], None] = ...) -> str: ...


def _enum_sources(proj_dir: str) -> <class 'Union'>[Tuple[List[str], List[str]], Tuple[List[str], List[Any]]]: ...


def _find_vcxproj(proj_dir: str, name: <class 'Union'>[str, None] = ...) -> str: ...


def _guid(name: str) -> str: ...


def _reseed(p: Profile) -> None: ...


def _title_id_for(proj_dir: str) -> Tuple[None, str]: ...


def run(
    proj_dir: <class 'Union'>[str, None] = ...,
    name: <class 'Union'>[str, None] = ...,
    write_filters_too: bool = ...
) -> None: ...


def write_filters(
    proj_dir: str,
    proj_name: str,
    cpps: List[str],
    hs: List[<class 'Union'>[Any, str]]
) -> Tuple[str, int]: ...
