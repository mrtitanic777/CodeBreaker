from codebreaker.formats.ne import NEFile
from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def infer_segment_sigs(
    f: NEFile,
    seg: Dict[str, <class 'Union'>],
    segb: bytes
) -> Dict[int, List[Tuple[str, str]]]: ...
