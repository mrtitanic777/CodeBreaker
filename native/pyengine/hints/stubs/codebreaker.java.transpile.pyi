from codebreaker.java.ir import (
    JClass,
    JType,
)
from codebreaker.java.profiles import Profile
from codebreaker.java.types import TypeResolver
from typing import (
    Callable,
    Dict,
    Iterator,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _coalesce_forwarders(out_dir: str, prog: Callable) -> List[<class 'Union'>[Any, str]]: ...


def _convert(
    java_dir: <class 'Union'>[str, pathlib.WindowsPath],
    out_dir: <class 'Union'>[str, pathlib.WindowsPath],
    value_types: None = ...,
    ledger: None = ...,
    write_report: bool = ...,
    cpp_std: str = ...,
    platform: <class 'Union'>[str, None] = ...,
    progress_cb: <class 'Union'>[Callable, None] = ...,
    platform_refs: bool = ...,
    sdk_override: None = ...,
    emit_debug: bool = ...,
    overlay_dir: None = ...,
    edition: <class 'Union'>[str, None] = ...,
    mc_version: <class 'Union'>[str, None] = ...
) -> Dict[str, <class 'Union'>]: ...


def _convert_restoring(
    java_dir: <class 'Union'>[str, pathlib.WindowsPath],
    out_dir: <class 'Union'>[str, pathlib.WindowsPath],
    value_types: None,
    ledger: None,
    write_report: bool,
    cpp_std: str,
    platform: <class 'Union'>[str, None],
    progress_cb: <class 'Union'>[Callable, None],
    platform_refs: bool,
    sdk_override: None,
    emit_debug: bool,
    overlay_dir: None,
    edition: <class 'Union'>[str, None],
    mc_version: <class 'Union'>[str, None]
) -> Dict[str, <class 'Union'>]: ...


def _defined_symbols(text: str) -> Set[str]: ...


def _finalize_emitted(text: str) -> str: ...


def _iter_call_args(body: str, name: str, arity: int) -> Iterator[Tuple[int, str]]: ...


def _parse_std_version(label: str) -> CppStandard: ...


def _prune_unreferenced(
    out_dir: str,
    game_classes: Set[str],
    prog: Callable,
    overlay_files: <class 'Union'>[None, List[str]] = ...
) -> List[<class 'Union'>[Any, str]]: ...


def _read(path: str) -> str: ...


def _reseed_static_init_order(p: Profile) -> None: ...


def _stub_return_jtype(cpp: str) -> <class 'Union'>[None, codebreaker.java.ir.JType]: ...


def _write(path: str, text: str) -> None: ...


def _write_init_driver(
    out_dir: <class 'Union'>[str, pathlib.WindowsPath],
    init_classes: List[<class 'Union'>[Any, str]],
    classes: List[JClass]
) -> None: ...


def build_member_env(classes: List[<class 'Union'>[codebreaker.java.ir.JClass, Any]]) -> Dict[str, <class 'Union'>]: ...


def build_method_renames(
    classes: List[JClass]
) -> Dict[str, <class 'Union'>[<class 'Union'>, Set[Any], Set[str]]]: ...


def build_resolver(
    classes: List[JClass],
    value_types: None = ...
) -> TypeResolver: ...


def compute_collection_out_params(
    classes: List[JClass],
    resolver: TypeResolver
) -> int: ...


def compute_mi_forwards(classes: List[JClass]) -> None: ...


def compute_static_init_order(
    classes: List[JClass],
    callable_init: Set[str]
) -> List[<class 'Union'>[Any, str]]: ...


def compute_this_escapes(classes: List[JClass]) -> Set[str]: ...


def compute_thread_like(classes: List[JClass]) -> Set[str]: ...


def convert(
    java_dir: <class 'Union'>[str, pathlib.WindowsPath],
    out_dir: <class 'Union'>[str, pathlib.WindowsPath],
    value_types: None = ...,
    ledger: None = ...,
    write_report: bool = ...,
    cpp_std: str = ...,
    platform: <class 'Union'>[str, None] = ...,
    progress_cb: <class 'Union'>[Callable, None] = ...,
    platform_refs: bool = ...,
    sdk_override: None = ...,
    emit_debug: bool = ...,
    overlay_dir: None = ...,
    edition: <class 'Union'>[str, None] = ...,
    mc_version: <class 'Union'>[str, None] = ...
) -> Dict[str, <class 'Union'>]: ...


def derive_nullability(
    classes: List[JClass],
    resolver: TypeResolver
) -> <class 'Union'>[Tuple[Set[str], Dict[str, str]], Tuple[Set[Any], Dict[Any, Any]]]: ...


def discover(java_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> List[str]: ...


def drop_synthetic_artifacts(
    classes: List[JClass],
    ledger: <class 'Union'>[None, codebreaker.java.ledger.Ledger] = ...
) -> <class 'Union'>[Tuple[List[JClass], Set[Any]], Tuple[List[JClass], Set[str]]]: ...


def resolve_inheritance(classes: List[JClass]) -> None: ...


class CppStandard:
    def __init__(self, year: int) -> None: ...


class _StubClass:
    def __init__(
        self,
        name: str,
        parent: <class 'Union'>[str, None],
        methods: List[<class 'Union'>[codebreaker.java.transpile._StubMethod, Any]]
    ) -> None: ...


class _StubMethod:
    def __init__(self, name: str, return_type: JType) -> None: ...

from codebreaker.java.ir import (
    JClass,
    JType,
)
from codebreaker.java.profiles import Profile
from codebreaker.java.types import TypeResolver
from typing import (
    Callable,
    Dict,
    Iterator,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _coalesce_forwarders(out_dir: str, prog: Callable) -> List[<class 'Union'>[str, Any]]: ...


def _convert(
    java_dir: <class 'Union'>[pathlib.WindowsPath, str],
    out_dir: <class 'Union'>[pathlib.WindowsPath, str],
    value_types: None = ...,
    ledger: None = ...,
    write_report: bool = ...,
    cpp_std: str = ...,
    platform: <class 'Union'>[str, None] = ...,
    progress_cb: <class 'Union'>[Callable, None] = ...,
    platform_refs: bool = ...,
    sdk_override: None = ...,
    emit_debug: bool = ...,
    overlay_dir: None = ...,
    edition: <class 'Union'>[str, None] = ...,
    mc_version: <class 'Union'>[str, None] = ...
) -> Dict[str, <class 'Union'>]: ...


def _convert_restoring(
    java_dir: <class 'Union'>[pathlib.WindowsPath, str],
    out_dir: <class 'Union'>[pathlib.WindowsPath, str],
    value_types: None,
    ledger: None,
    write_report: bool,
    cpp_std: str,
    platform: <class 'Union'>[str, None],
    progress_cb: <class 'Union'>[Callable, None],
    platform_refs: bool,
    sdk_override: None,
    emit_debug: bool,
    overlay_dir: None,
    edition: <class 'Union'>[str, None],
    mc_version: <class 'Union'>[str, None]
) -> Dict[str, <class 'Union'>]: ...


def _defined_symbols(text: str) -> Set[str]: ...


def _finalize_emitted(text: str) -> str: ...


def _iter_call_args(body: str, name: str, arity: int) -> Iterator[Tuple[int, str]]: ...


def _parse_std_version(label: str) -> CppStandard: ...


def _prune_unreferenced(
    out_dir: str,
    game_classes: Set[str],
    prog: Callable,
    overlay_files: <class 'Union'>[List[str], None] = ...
) -> List[<class 'Union'>[str, Any]]: ...


def _read(path: str) -> str: ...


def _reseed_static_init_order(p: Profile) -> None: ...


def _stub_return_jtype(cpp: str) -> <class 'Union'>[codebreaker.java.ir.JType, None]: ...


def _write(path: str, text: str) -> None: ...


def _write_init_driver(
    out_dir: <class 'Union'>[pathlib.WindowsPath, str],
    init_classes: List[<class 'Union'>[str, Any]],
    classes: List[JClass]
) -> None: ...


def build_member_env(classes: List[<class 'Union'>[codebreaker.java.ir.JClass, Any]]) -> Dict[str, <class 'Union'>]: ...


def build_method_renames(
    classes: List[JClass]
) -> Dict[str, <class 'Union'>[<class 'Union'>, Set[Any], Set[str]]]: ...


def build_resolver(
    classes: List[JClass],
    value_types: None = ...
) -> TypeResolver: ...


def compute_collection_out_params(
    classes: List[JClass],
    resolver: TypeResolver
) -> int: ...


def compute_mi_forwards(classes: List[JClass]) -> None: ...


def compute_static_init_order(
    classes: List[JClass],
    callable_init: Set[str]
) -> List[<class 'Union'>[str, Any]]: ...


def compute_this_escapes(classes: List[JClass]) -> Set[str]: ...


def compute_thread_like(classes: List[JClass]) -> Set[str]: ...


def convert(
    java_dir: <class 'Union'>[pathlib.WindowsPath, str],
    out_dir: <class 'Union'>[pathlib.WindowsPath, str],
    value_types: None = ...,
    ledger: None = ...,
    write_report: bool = ...,
    cpp_std: str = ...,
    platform: <class 'Union'>[str, None] = ...,
    progress_cb: <class 'Union'>[Callable, None] = ...,
    platform_refs: bool = ...,
    sdk_override: None = ...,
    emit_debug: bool = ...,
    overlay_dir: None = ...,
    edition: <class 'Union'>[str, None] = ...,
    mc_version: <class 'Union'>[str, None] = ...
) -> Dict[str, <class 'Union'>]: ...


def derive_nullability(
    classes: List[JClass],
    resolver: TypeResolver
) -> <class 'Union'>[Tuple[Set[str], Dict[str, str]], Tuple[Set[Any], Dict[Any, Any]]]: ...


def discover(java_dir: <class 'Union'>[pathlib.WindowsPath, str]) -> List[str]: ...


def drop_synthetic_artifacts(
    classes: List[JClass],
    ledger: <class 'Union'>[None, codebreaker.java.ledger.Ledger] = ...
) -> <class 'Union'>[Tuple[List[JClass], Set[Any]], Tuple[List[JClass], Set[str]]]: ...


def resolve_inheritance(classes: List[JClass]) -> None: ...


class CppStandard:
    def __init__(self, year: int) -> None: ...


class _StubClass:
    def __init__(
        self,
        name: str,
        parent: <class 'Union'>[str, None],
        methods: List[<class 'Union'>[codebreaker.java.transpile._StubMethod, Any]]
    ) -> None: ...


class _StubMethod:
    def __init__(self, name: str, return_type: JType) -> None: ...
