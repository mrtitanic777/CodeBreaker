from codebreaker.base.converted import Project
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _args(text: str, open_paren: int) -> Tuple[List[Tuple[int, int]], int]: ...


def _build_once(
    port: str,
    name: str
) -> <class 'Union'>[Tuple[bool, List[Dict[str, str]], str], Tuple[bool, List[Any], str]]: ...


def _copy_tree(src: str, dst: str) -> None: ...


def _csproj(name: str, exe_rel: str, assemblies: List[str], defines: Tuple[()]) -> str: ...


def _edit_lines(errors: List[Dict[str, str]], code: str, wanted: str, edit: Callable) -> int: ...


def _effect_bytes(text: str) -> Tuple[str, int]: ...


def _env() -> Dict[str, str]: ...


def _overlay_dir(project: Project, overlay: None) -> None: ...


def build(
    project: Project,
    overlay: None = ...,
    rounds: int = ...
) -> Dict[str, <class 'Union'>]: ...


def check_content(exe: str, seconds: int = ...) -> Dict[str, str]: ...


def compile_effects(converted: str, shaders: str) -> Dict[str, <class 'Union'>]: ...


def convert_songs(game_dir: str, converted: str) -> int: ...


def convert_xact(game_dir: str, converted: str) -> Dict[str, int]: ...


def declared_types(folder: str) -> Set[str]: ...


def extension_methods(folder: str) -> Dict[str, List[Tuple[str, str]]]: ...


def fix_access(errors: List[Dict[str, str]]) -> int: ...


def fix_color(errors: List[Dict[str, str]]) -> int: ...


def fix_moved_members(errors: List[Dict[str, str]]) -> int: ...


def fix_named_arguments(errors: List[Dict[str, str]]) -> int: ...


def fix_ref_out(errors: List[Dict[str, str]]) -> int: ...


def fix_shadowed_extensions(errors: List[Dict[str, str]], extensions: Dict[str, List[Tuple[str, str]]]) -> int: ...


def fix_vertex_binding(errors: List[Dict[str, str]]) -> int: ...


def port_reader_name(name: str, program: str) -> str: ...


def port_reader_names(game_dir: str, program: str) -> int: ...


def prepare(
    project: Project,
    overlay: None = ...
) -> Tuple[str, str, Dict[str, <class 'Union'>]]: ...


def rewrite(
    text: str,
    own: <class 'Union'>[Set[str], frozenset] = ...
) -> <class 'Union'>[Tuple[str, List[Any]], Tuple[str, List[str]]]: ...


def run_for(exe: str, seconds: int, env: <class 'Union'>[Dict[str, str], None] = ...) -> Dict[str, <class 'Union'>]: ...


def stubs(out_dir: str) -> None: ...

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
