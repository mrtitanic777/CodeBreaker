from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _provided_symbols(header_text: str) -> Set[str]: ...


def _read(path: str) -> str: ...


def _render_stub_markers(markers: Dict[Any, Any]) -> List[Any]: ...


def _type_usage_count(game_text: str, typ: str) -> int: ...


def _used_holder_symbols(game_text: str, holder: str) -> Dict[str, int]: ...


def analyze(
    output_dir: <class 'Union'>[str, pathlib.WindowsPath],
    reference_dir: <class 'Union'>[str, None] = ...,
    runtime_files: <class 'Union'>[Set[str], None] = ...
) -> List[<class 'Union'>]: ...


def render_report(
    results: List[<class 'Union'>],
    reference_dir: <class 'Union'>[str, None] = ...,
    stub_markers: <class 'Union'>[Dict[Any, Any], None] = ...,
    sdk_caps: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...
) -> str: ...


def scan_stub_markers(output_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> Dict[Any, Any]: ...


def write_report(
    output_dir: <class 'Union'>[str, pathlib.WindowsPath],
    reference_dir: <class 'Union'>[str, None] = ...,
    runtime_files: <class 'Union'>[Set[str], None] = ...,
    sdk_root: None = ...
) -> Tuple[str, List[<class 'Union'>]]: ...
