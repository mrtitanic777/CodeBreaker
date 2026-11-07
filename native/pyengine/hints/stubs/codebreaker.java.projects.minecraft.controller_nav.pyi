from typing import (
    Dict,
    Tuple,
    Union,
)


def patch_entityrenderer_pointer(text: str) -> Tuple[str, bool]: ...


def patch_guiscreen_cpp(text: str) -> Tuple[str, bool]: ...


def patch_guiscreen_h(text: str) -> Tuple[str, bool]: ...


def patch_minecraft_callsite(text: str) -> Tuple[str, bool]: ...


def patch_minecraft_displayguiscreen(text: str, has_container: bool) -> Tuple[str, bool]: ...


def patch_mousehelper_grab(text: str) -> Tuple[str, bool]: ...


def run(out_dir: str) -> Dict[str, <class 'Union'>]: ...

from typing import (
    Dict,
    Tuple,
    Union,
)


def patch_entityrenderer_pointer(text: str) -> Tuple[str, bool]: ...


def patch_guiscreen_cpp(text: str) -> Tuple[str, bool]: ...


def patch_guiscreen_h(text: str) -> Tuple[str, bool]: ...


def patch_minecraft_callsite(text: str) -> Tuple[str, bool]: ...


def patch_minecraft_displayguiscreen(text: str, has_container: bool) -> Tuple[str, bool]: ...


def patch_mousehelper_grab(text: str) -> Tuple[str, bool]: ...


def run(out_dir: str) -> Dict[str, <class 'Union'>]: ...
