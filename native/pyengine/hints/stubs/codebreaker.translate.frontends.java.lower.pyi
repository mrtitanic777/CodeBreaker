from codebreaker.translate.ir import (
    Block,
    Call,
    Case,
    ClassDecl,
    Expr,
    Lit,
    MethodDecl,
    Stmt,
    Sym,
    Type,
    Unit,
)
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _type(s: str, i: int, tv: frozenset) -> Tuple[Type, int]: ...


def lower(tree: Dict[str, <class 'Union'>], path: <class 'Union'>[str, None] = ...) -> Unit: ...


def parse_type(s: str, typevars: Tuple[()] = ...) -> Type: ...


def record_members(c: ClassDecl) -> None: ...


def sym(d: Dict[str, <class 'Union'>[<class 'Union'>, str]]) -> Sym: ...


class Lowering:
    def __init__(self, path: <class 'Union'>[str, None] = ...) -> None: ...
    def block(self, n: Dict[str, <class 'Union'>], ctor_body: bool = ...) -> Block: ...
    def call(
        self,
        n: Dict[str, <class 'Union'>],
        t: Type,
        p: Dict[str, int]
    ) -> Call: ...
    def case(self, c: Dict[str, <class 'Union'>], in_expr: bool = ...) -> Case: ...
    def decl(
        self,
        n: Dict[str, <class 'Union'>],
        outer: <class 'Union'>[str, None]
    ) -> ClassDecl: ...
    def expr(self, n: Dict[str, <class 'Union'>]) -> Expr: ...
    def identifier(
        self,
        n: Dict[str, <class 'Union'>],
        t: Type,
        p: Dict[str, int]
    ) -> <class 'Union'>[Name, Super, Select, TypeName, This]: ...
    def literal(
        self,
        n: Dict[str, <class 'Union'>],
        t: Type,
        p: Dict[str, int]
    ) -> Lit: ...
    def member(
        self,
        m: Dict[str, <class 'Union'>],
        cls: ClassDecl,
        raw_cls: Dict[str, <class 'Union'>]
    ) -> <class 'Union'>[ClassDecl, EnumConst, MethodDecl, FieldDecl]: ...
    def member_select(
        self,
        n: Dict[str, <class 'Union'>],
        t: Type,
        p: Dict[str, int]
    ) -> <class 'Union'>[Select, ClassLit, TypeName, Length]: ...
    def method(
        self,
        m: Dict[str, <class 'Union'>],
        raw_cls: Dict[str, <class 'Union'>]
    ) -> MethodDecl: ...
    def mods(self, n: Dict[str, <class 'Union'>]) -> List[<class 'Union'>[Any, str]]: ...
    def pos(self, n: Dict[str, <class 'Union'>]) -> Dict[str, int]: ...
    def qualified(self, n: Dict[str, <class 'Union'>]) -> str: ...
    def stmt(self, n: Dict[str, <class 'Union'>]) -> Stmt: ...
    def ty(self, n: Dict[str, <class 'Union'>]) -> Type: ...
    def unit(self, n: Dict[str, <class 'Union'>]) -> Unit: ...
