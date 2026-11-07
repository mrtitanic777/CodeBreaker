from codebreaker.build.definitions import Export
from typing import (
    Dict,
    Tuple,
)


def call(path: str, export: Export) -> Tuple[str, int]: ...


def tools() -> Dict[str, str]: ...
