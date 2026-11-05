from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def _expr(name: str, slots: List[<class 'Union'>[Any, str]], fields: <class 'Union'>[None, List[str]] = ...) -> Any: ...


def _node(
    name: str,
    slots: List[<class 'Union'>[Any, str]],
    fields: <class 'Union'>[None, List[str]] = ...,
    base: <class 'Union'>[Type[codebreaker.translate.ir.Stmt], Type[codebreaker.translate.ir.Node], Type[codebreaker.translate.ir.Expr]] = ...
) -> Any: ...


def _stmt(name: str, slots: List[<class 'Union'>[Any, str]], fields: <class 'Union'>[None, List[str]] = ...) -> Any: ...


class Type:
    def __init__(
        self,
        kind: str,
        name: None = ...,
        args: Tuple[()] = ...,
        elem: None = ...,
        bound: None = ...,
        **info
    ) -> None: ...
