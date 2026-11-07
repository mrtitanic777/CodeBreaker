from codebreaker.translate.ir import Unit
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
)


class Minecraft:
    def after_construction(
        self,
        out_dir: <class 'Union'>[pathlib.WindowsPath, str],
        platform: None,
        prog: Callable
    ) -> None: ...
    def after_emit(
        self,
        out_dir: <class 'Union'>[pathlib.WindowsPath, str],
        platform: None,
        patched: Dict[Any, Any],
        prog: Callable
    ) -> None: ...
    def ir_patches(self, units: List[Unit], platform: None) -> Dict[Any, Any]: ...
    def profile_for(
        self,
        edition: <class 'Union'>[str, None] = ...,
        mc_version: <class 'Union'>[str, None] = ...,
        classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None] = ...
    ) -> str: ...

from codebreaker.translate.ir import Unit
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
)


class Minecraft:
    def after_construction(
        self,
        out_dir: <class 'Union'>[pathlib.WindowsPath, str],
        platform: None,
        prog: Callable
    ) -> None: ...
    def after_emit(
        self,
        out_dir: <class 'Union'>[pathlib.WindowsPath, str],
        platform: None,
        patched: Dict[Any, Any],
        prog: Callable
    ) -> None: ...
    def ir_patches(self, units: List[Unit], platform: None) -> Dict[Any, Any]: ...
    def profile_for(
        self,
        edition: <class 'Union'>[str, None] = ...,
        mc_version: <class 'Union'>[str, None] = ...,
        classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None] = ...
    ) -> str: ...
