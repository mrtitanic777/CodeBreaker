from codebreaker.translate.ir import Unit
from re import Match
from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _hurt_repl(m: Match) -> str: ...


def _update_renderers_repl(m: Match) -> str: ...


def patch(units: List[Unit]) -> Dict[str, <class 'Union'>]: ...


def process(text: str) -> Tuple[str, int, int]: ...


def run(out_dir: str) -> Dict[str, <class 'Union'>]: ...
