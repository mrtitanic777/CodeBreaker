from codebreaker.java.ir import (
    JClass,
    JType,
    Method,
)
from codebreaker.java.ledger import Ledger
from codebreaker.java.transpile import CppStandard
from codebreaker.java.types import TypeResolver
from typing import (
    List,
    Optional,
)


def _array_elem_cpp(jtype: JType, resolver: TypeResolver) -> str: ...


def _banner(
    cls_name: str,
    cpp_std: <class 'Union'>[codebreaker.java.transpile.CppStandard, None] = ...,
    platform_label: <class 'Union'>[str, None] = ...
) -> str: ...


def _default_value(cpp_type: str) -> <class 'Union'>[str, None]: ...


def _detect_self_singleton(clsname: str, static_init_lines: List[<class 'Union'>[Any, str]]) -> None: ...


def _gap(
    cls: JClass,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None],
    what: str,
    line: int = ...
) -> List[str]: ...


def _init_list(
    cls: JClass,
    resolver: TypeResolver
) -> List[<class 'Union'>[Any, str]]: ...


def _instance_init_lines(
    cls: JClass,
    resolver: TypeResolver,
    ledger: Ledger
) -> List[<class 'Union'>[Any, str]]: ...


def _is_deferrable_static_init(init: str, ft: str) -> bool: ...


def _referenced_types(
    cls: JClass,
    resolver: TypeResolver,
    body_text: str
) -> List[<class 'Union'>[Any, str]]: ...


def _render_ctor(
    cls: JClass,
    m: Method,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[str]: ...


def _render_dtor(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[str]: ...


def _render_method(
    cls: JClass,
    m: Method,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[str]: ...


def _render_statics(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> <class 'Union'>[Tuple[List[str], List[str]], Tuple[List[str], List[Any]], Tuple[List[Any], List[Any]]]: ...


def _render_two_phase(
    cls: JClass,
    resolver: TypeResolver,
    ledger: Ledger
) -> List[str]: ...


def _render_typed_enum_source(
    cls: JClass,
    resolver: TypeResolver,
    ledger: Ledger,
    cpp_std: CppStandard,
    platform_label: None
) -> str: ...


def _static_block_lines(
    cls: JClass,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[<class 'Union'>[Any, str]]: ...


def render_source(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None] = ...,
    cpp_std: <class 'Union'>[codebreaker.java.transpile.CppStandard, None] = ...,
    platform_label: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[str, None]: ...

from codebreaker.java.ir import (
    JClass,
    JType,
    Method,
)
from codebreaker.java.ledger import Ledger
from codebreaker.java.transpile import CppStandard
from codebreaker.java.types import TypeResolver
from typing import (
    List,
    Optional,
)


def _array_elem_cpp(jtype: JType, resolver: TypeResolver) -> str: ...


def _banner(
    cls_name: str,
    cpp_std: <class 'Union'>[None, codebreaker.java.transpile.CppStandard] = ...,
    platform_label: <class 'Union'>[str, None] = ...
) -> str: ...


def _default_value(cpp_type: str) -> <class 'Union'>[str, None]: ...


def _detect_self_singleton(clsname: str, static_init_lines: List[<class 'Union'>[Any, str]]) -> None: ...


def _gap(
    cls: JClass,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None],
    what: str,
    line: int = ...
) -> List[str]: ...


def _init_list(
    cls: JClass,
    resolver: TypeResolver
) -> List[<class 'Union'>[Any, str]]: ...


def _instance_init_lines(
    cls: JClass,
    resolver: TypeResolver,
    ledger: Ledger
) -> List[<class 'Union'>[Any, str]]: ...


def _is_deferrable_static_init(init: str, ft: str) -> bool: ...


def _referenced_types(
    cls: JClass,
    resolver: TypeResolver,
    body_text: str
) -> List[<class 'Union'>[Any, str]]: ...


def _render_ctor(
    cls: JClass,
    m: Method,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[str]: ...


def _render_dtor(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[str]: ...


def _render_method(
    cls: JClass,
    m: Method,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[str]: ...


def _render_statics(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> <class 'Union'>[Tuple[List[str], List[str]], Tuple[List[str], List[Any]], Tuple[List[Any], List[Any]]]: ...


def _render_two_phase(
    cls: JClass,
    resolver: TypeResolver,
    ledger: Ledger
) -> List[str]: ...


def _render_typed_enum_source(
    cls: JClass,
    resolver: TypeResolver,
    ledger: Ledger,
    cpp_std: CppStandard,
    platform_label: None
) -> str: ...


def _static_block_lines(
    cls: JClass,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None]
) -> List[<class 'Union'>[Any, str]]: ...


def render_source(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None] = ...,
    cpp_std: <class 'Union'>[None, codebreaker.java.transpile.CppStandard] = ...,
    platform_label: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[str, None]: ...
