from codebreaker.java.ir import JClass
from typing import (
    Dict,
    List,
    Set,
    Tuple,
    Union,
)


def _close(text: str, i: int) -> int: ...


def _ctor_text(cls: JClass) -> str: ...


def compute(classes: List[JClass], values: Tuple[()] = ...) -> Set[str]: ...


def rewrite_news(text: str, names: <class 'Union'>[Set[str], List[str]]) -> Tuple[str, int]: ...


def rewrite_stack(text: str, names: Set[str]) -> Tuple[str, int]: ...


def run(out_dir: <class 'Union'>[pathlib.WindowsPath, str], names: Set[str]) -> Dict[str, <class 'Union'>]: ...
