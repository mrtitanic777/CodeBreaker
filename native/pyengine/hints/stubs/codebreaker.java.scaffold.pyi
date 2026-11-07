from codebreaker.java.ir import (
    JClass,
    JType,
    Method,
)
from typing import (
    Callable,
    Dict,
    List,
    Tuple,
    Union,
)


def _arr(jt: JType) -> int: ...


def _cstype(jt: JType) -> str: ...


def _ctype(jt: JType) -> str: ...


def _discover(java_dir: str) -> List[str]: ...


def _emit_c(cls: JClass) -> Tuple[str, str]: ...


def _emit_csharp(cls: JClass) -> Tuple[str, str]: ...


def _emit_rust(cls: JClass) -> Tuple[str, str]: ...


def _jtype_name(jt: JType) -> str: ...


def _params(method: Method, render: Callable) -> str: ...


def _rparams(method: Method) -> str: ...


def _rstype(jt: JType) -> str: ...


def _snake(name: str) -> str: ...


def run(java_dir: str, out_dir: str, language: str) -> Dict[str, <class 'Union'>]: ...
