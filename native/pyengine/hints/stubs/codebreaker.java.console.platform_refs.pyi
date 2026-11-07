from codebreaker.java.ir import JClass
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Union,
)


def _banner(base: str, ext: str, platform: str = ...) -> str: ...


def _companion_banner(fn: str) -> str: ...


def _runnable_ctor_args(cls: JClass) -> str: ...


def _write_platform_layer_md(out_dir: str, emitted: List[str], files_written: List[str]) -> None: ...


def audit_references(reference_dir: str) -> Dict[str, <class 'Union'>]: ...


def available_references(reference_dir: str) -> Set[str]: ...


def detect_main_class(classes: List[JClass]) -> <class 'Union'>[str, None]: ...


def detect_runnable_game_class(
    classes: List[JClass]
) -> <class 'Union'>[JClass, None]: ...


def emit_references(
    out_dir: str,
    reference_dir: str,
    game_classes: Set[str] = ...,
    main_class: <class 'Union'>[str, None] = ...,
    runnable_game: None = ...,
    platform: str = ...
) -> Set[str]: ...


def generate_entry_point(main_class: str) -> str: ...


def generate_pc_entry_point(main_class: str) -> str: ...


def generate_runnable_entry_point(cls: JClass) -> str: ...


def verify_integrated(out_dir: str, bases: List[str]) -> List[Any]: ...

from codebreaker.java.ir import JClass
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Union,
)


def _banner(base: str, ext: str, platform: str = ...) -> str: ...


def _companion_banner(fn: str) -> str: ...


def _runnable_ctor_args(cls: JClass) -> str: ...


def _write_platform_layer_md(out_dir: str, emitted: List[str], files_written: List[str]) -> None: ...


def audit_references(reference_dir: str) -> Dict[str, <class 'Union'>]: ...


def available_references(reference_dir: str) -> Set[str]: ...


def detect_main_class(classes: List[JClass]) -> <class 'Union'>[str, None]: ...


def detect_runnable_game_class(
    classes: List[JClass]
) -> <class 'Union'>[JClass, None]: ...


def emit_references(
    out_dir: str,
    reference_dir: str,
    game_classes: Set[str] = ...,
    main_class: <class 'Union'>[str, None] = ...,
    runnable_game: None = ...,
    platform: str = ...
) -> Set[str]: ...


def generate_entry_point(main_class: str) -> str: ...


def generate_pc_entry_point(main_class: str) -> str: ...


def generate_runnable_entry_point(cls: JClass) -> str: ...


def verify_integrated(out_dir: str, bases: List[str]) -> List[Any]: ...
