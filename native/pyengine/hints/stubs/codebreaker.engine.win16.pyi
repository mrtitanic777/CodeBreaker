from codebreaker.build.module import Built
from codebreaker.engine.native_engine import (
    Context,
    Database,
)
from codebreaker.translate.l2 import Made
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _copy(m: Made) -> Made: ...


def _gate_stamp() -> Tuple[str, List[str], str, bool, int, int, str, bool, int, int, Tuple[str, str]]: ...


def _image_stamp() -> Tuple[str, List[str], str, List[str]]: ...


def _keepable(b: Built) -> Built: ...


def _module_stamp() -> Tuple[str, str, str, List[str], str, str]: ...


def _served_compile(cc: str, cflags: List[str], env: Dict[str, str], outdir: str, stem: str, text: str) -> str: ...


def _sources(*patterns) -> str: ...


def _wcc() -> str: ...


def build(
    module_name: str,
    outdir: <class 'Union'>[str, None] = ...,
    call: bool = ...,
    log: Callable = ...
) -> Built: ...


def facts_folder(db: Database) -> str: ...


def function_gate(
    ctxs: Dict[str, Context],
    hint: Dict[str, <class 'Union'>]
) -> Dict[str, List[str]]: ...


def ne_image(ctx: Context, key: str) -> Dict[str, <class 'Union'>]: ...

from codebreaker.build.module import Built
from codebreaker.engine.native_engine import (
    Context,
    Database,
)
from codebreaker.translate.l2 import Made
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _copy(m: Made) -> Made: ...


def _gate_stamp() -> Tuple[str, List[str], str, bool, int, int, str, bool, int, int, Tuple[str, str]]: ...


def _image_stamp() -> Tuple[str, List[str], str, List[str]]: ...


def _keepable(b: Built) -> Built: ...


def _module_stamp() -> Tuple[str, str, str, List[str], str, str]: ...


def _served_compile(cc: str, cflags: List[str], env: Dict[str, str], outdir: str, stem: str, text: str) -> str: ...


def _sources(*patterns) -> str: ...


def _wcc() -> str: ...


def build(
    module_name: str,
    outdir: <class 'Union'>[str, None] = ...,
    call: bool = ...,
    log: Callable = ...
) -> Built: ...


def facts_folder(db: Database) -> str: ...


def function_gate(
    ctxs: Dict[str, Context],
    hint: Dict[str, <class 'Union'>]
) -> Dict[str, List[str]]: ...


def ne_image(ctx: Context, key: str) -> Dict[str, <class 'Union'>]: ...
