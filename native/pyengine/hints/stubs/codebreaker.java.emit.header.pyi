from codebreaker.java.ir import (
    Field,
    JClass,
    Method,
)
from codebreaker.java.ledger import Ledger
from codebreaker.java.types import TypeResolver
from typing import (
    Any,
    Optional,
)


def _banner(
    cls_name: str,
    cpp_std: <class 'Union'>[None, codebreaker.java.transpile.CppStandard] = ...,
    platform_label: <class 'Union'>[str, None] = ...
) -> str: ...


def _base_type_spelling(parent: str) -> str: ...


def _collect_dependencies(cls: JClass, resolver: TypeResolver) -> Any: ...


def _field_type(
    cls: JClass,
    f: Field,
    resolver: TypeResolver
) -> str: ...


def _method_decl(
    cls: JClass,
    m: Method,
    resolver: TypeResolver
) -> str: ...


def _render_typed_enum_header(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None],
    cpp_std: <class 'Union'>[codebreaker.java.transpile.CppStandard, None],
    platform_label: None
) -> str: ...


def _report(
    cls: JClass,
    resolver: TypeResolver,
    ledger: Ledger
) -> None: ...


def _std_header_for(cpp_base: str) -> <class 'Union'>[str, None]: ...


def render_header(
    cls: JClass,
    resolver: TypeResolver,
    ledger: <class 'Union'>[codebreaker.java.ledger.Ledger, None] = ...,
    cpp_std: <class 'Union'>[None, codebreaker.java.transpile.CppStandard] = ...,
    platform_label: <class 'Union'>[str, None] = ...
) -> str: ...
