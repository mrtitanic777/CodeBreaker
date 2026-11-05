from typing import (
    Callable,
    Dict,
    List,
    Tuple,
)


def _args(text: str, open_paren: int) -> Tuple[List[Tuple[int, int]], int]: ...


def _edit_lines(errors: List[Dict[str, str]], code: str, wanted: str, edit: Callable) -> int: ...


def _effect_bytes(text: str) -> Tuple[str, int]: ...


def fix_color(errors: List[Dict[str, str]]) -> int: ...


def fix_moved_members(errors: List[Dict[str, str]]) -> int: ...


def fix_named_arguments(errors: List[Dict[str, str]]) -> int: ...


def fix_shadowed_extensions(errors: List[Dict[str, str]], extensions: Dict[str, List[Tuple[str, str]]]) -> int: ...


def fix_vertex_binding(errors: List[Dict[str, str]]) -> int: ...


def port_reader_name(name: str, program: str) -> str: ...


def rewrite(
    text: str,
    own: <class 'Union'>[Set[str], frozenset] = ...
) -> <class 'Union'>[Tuple[str, List[Any]], Tuple[str, List[str]]]: ...
