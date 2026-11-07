from typing import (
    Any,
    List,
    Tuple,
)


def _declared_type(text: str, var: str, before: int) -> str: ...


def _enclosing_brace(text: str, pos: int) -> int: ...


def _matching(text: str, open_paren: int) -> Tuple[str, int]: ...


def constructors(text: str) -> Tuple[str, int, List[Any]]: ...


def data_holders(text: str) -> Tuple[str, int]: ...


def ref_casts(text: str) -> Tuple[str, int]: ...

from typing import (
    Any,
    List,
    Tuple,
)


def _declared_type(text: str, var: str, before: int) -> str: ...


def _enclosing_brace(text: str, pos: int) -> int: ...


def _matching(text: str, open_paren: int) -> Tuple[str, int]: ...


def constructors(text: str) -> Tuple[str, int, List[Any]]: ...


def data_holders(text: str) -> Tuple[str, int]: ...


def ref_casts(text: str) -> Tuple[str, int]: ...
