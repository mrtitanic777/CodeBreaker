from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def _feedback(result: Dict[str, <class 'Union'>]) -> List[str]: ...


def run(
    module: str,
    what: <class 'Union'>[Tuple[str], Tuple[str, str, str], None] = ...,
    store: bool = ...,
    observed: None = ...
) -> Dict[str, <class 'Union'>]: ...


def text(result: Dict[str, <class 'Union'>], all_facts: bool = ...) -> str: ...

from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def _feedback(result: Dict[str, <class 'Union'>]) -> List[str]: ...


def main(argv: List[str]) -> int: ...


def run(
    module: str,
    what: <class 'Union'>[Tuple[str], Tuple[str, str, str], List[str], None] = ...,
    store: bool = ...,
    observed: None = ...
) -> Dict[str, <class 'Union'>]: ...


def text(result: Dict[str, <class 'Union'>], all_facts: bool = ...) -> str: ...
