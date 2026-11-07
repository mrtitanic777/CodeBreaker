from codebreaker.translate.frontends.python.lower import (
    Context,
    ModuleResult,
)
from codebreaker.translate.frontends.python.program import (
    Class,
    Program,
)
from codebreaker.translate.ir import (
    MethodDecl,
    Type,
)
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
)


def _attr_order(
    prog: Program,
    c: Class,
    attr: str
) -> int: ...


def _types_in(md: MethodDecl) -> Set[str]: ...


def _types_of(t: Type, out: <class 'Union'>[Set[str], None] = ...) -> Set[str]: ...


def build_unit(
    prog: Program,
    ctx: Context,
    mod: str,
    lowered: Dict[str, <class 'Union'>[Tuple[Refused, FnLower], Tuple[codebreaker.translate.ir.MethodDecl, FnLower], <class 'Union'>]],
    global_decls: Dict[Any, Any],
    used_all: <class 'Union'>[Set[str], None] = ...
) -> ModuleResult: ...


def program_of_sources(sources: Dict[str, str]) -> Program: ...


def translate(
    prog: Program,
    modules: <class 'Union'>[List[str], None] = ...,
    int_policy: str = ...,
    fns: None = ...
) -> Dict[str, ModuleResult]: ...


def used_types(
    prog: Program,
    ctx: Context,
    lowered: Dict[str, <class 'Union'>[Tuple[Refused, FnLower], Tuple[codebreaker.translate.ir.MethodDecl, FnLower], <class 'Union'>]],
    global_decls: Dict[Any, Any]
) -> Set[str]: ...
