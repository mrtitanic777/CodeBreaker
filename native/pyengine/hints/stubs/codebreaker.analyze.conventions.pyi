from codebreaker.analyze.call_graph import Graph
from codebreaker.analyze.image import (
    Function,
    Image,
    Ins,
)
from typing import (
    Any,
    Dict,
    List,
    Set,
    Union,
)


def _argument_slots(fn: Function, framed: bool, first: int, word: int) -> Dict[int, int]: ...


def _callback(
    name: str,
    apis: List[<class 'Union'>[Any, str]],
    far: bool,
    pop: <class 'Union'>[int, None]
) -> <class 'Union'>[str, None]: ...


def _cleanup(
    img: Image,
    graph: Graph
) -> Dict[int, Dict[int, int]]: ...


def _entry16(
    img: Image,
    fn: Function
) -> List[<class 'Union'>[Any, str]]: ...


def _frame(
    fn: Function,
    word: int
) -> <class 'Union'>[Tuple[bool, int, List[Any]], Tuple[bool, int, List[str]]]: ...


def _functions(img: Image) -> Dict[int, <class 'Union'>]: ...


def _loaded_before(
    img: Image,
    graph: Graph
) -> Dict[int, Dict[str, int]]: ...


def _pushed_inputs(fn: Function, loads: <class 'Union'>[Dict[str, int], None]) -> Set[Any]: ...


def _register_inputs(fn: Function) -> Set[str]: ...


def _result_use(
    img: Image,
    graph: Graph
) -> Dict[int, List[int]]: ...


def _returns(
    fn: Function
) -> <class 'Union'>[Tuple[List[int], List[Tuple[Tuple[str, int], bool, bool]]], Tuple[List[int], List[Tuple[str, bool, bool]]], Tuple[List[Any], List[Any]]]: ...


def _sets_constant(ins: Ins) -> bool: ...


def _signature(img: Image, fn: Function) -> str: ...


def analyse(img: Image) -> Dict[str, <class 'Union'>]: ...


def duplicates(img: Image, least: int = ...) -> List[Any]: ...


def functions(img: Image) -> Dict[int, <class 'Union'>]: ...


def stack_delta(ins: Ins) -> int: ...


def thunk_layer(img: Image) -> List[<class 'Union'>[Any, str]]: ...
