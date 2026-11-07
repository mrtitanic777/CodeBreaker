from codebreaker.java.ir import JClass
from codebreaker.translate.ir import Unit
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def choose(
    classes: List[JClass],
    project: <class 'Union'>[str, None] = ...,
    **options
) -> <class 'Union'>[codebreaker.java.projects.Project, codebreaker.java.projects.minecraft.Minecraft]: ...


def load(name: str) -> <class 'Union'>[codebreaker.java.projects.Project, codebreaker.java.projects.minecraft.Minecraft]: ...


def manifests() -> List[Dict[str, <class 'Union'>]]: ...


def matches(manifest: Dict[str, <class 'Union'>], classes: List[JClass]) -> bool: ...


class Project:
    def after_construction(
        self,
        out_dir: <class 'Union'>[str, pathlib.WindowsPath],
        platform: <class 'Union'>[str, None],
        prog: Callable
    ) -> None: ...
    def after_emit(
        self,
        out_dir: <class 'Union'>[str, pathlib.WindowsPath],
        platform: <class 'Union'>[str, None],
        patched: Dict[Any, Any],
        prog: Callable
    ) -> None: ...
    def ir_patches(
        self,
        units: List[Unit],
        platform: <class 'Union'>[str, None]
    ) -> Dict[Any, Any]: ...
    def profile_for(
        self,
        edition: <class 'Union'>[str, None] = ...,
        mc_version: None = ...,
        classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None] = ...
    ) -> None: ...

from codebreaker.java.ir import JClass
from codebreaker.translate.ir import Unit
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def choose(
    classes: List[JClass],
    project: <class 'Union'>[str, None] = ...,
    **options
) -> <class 'Union'>[codebreaker.java.projects.minecraft.Minecraft, codebreaker.java.projects.Project]: ...


def load(name: str) -> <class 'Union'>[codebreaker.java.projects.minecraft.Minecraft, codebreaker.java.projects.Project]: ...


def manifests() -> List[Dict[str, <class 'Union'>]]: ...


def matches(manifest: Dict[str, <class 'Union'>], classes: List[JClass]) -> bool: ...


class Project:
    def after_construction(
        self,
        out_dir: <class 'Union'>[pathlib.WindowsPath, str],
        platform: <class 'Union'>[str, None],
        prog: Callable
    ) -> None: ...
    def after_emit(
        self,
        out_dir: <class 'Union'>[pathlib.WindowsPath, str],
        platform: <class 'Union'>[str, None],
        patched: Dict[Any, Any],
        prog: Callable
    ) -> None: ...
    def ir_patches(
        self,
        units: List[Unit],
        platform: <class 'Union'>[str, None]
    ) -> Dict[Any, Any]: ...
    def profile_for(
        self,
        edition: <class 'Union'>[str, None] = ...,
        mc_version: None = ...,
        classes: <class 'Union'>[None, List[codebreaker.java.ir.JClass]] = ...
    ) -> None: ...
