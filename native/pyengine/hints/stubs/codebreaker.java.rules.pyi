from codebreaker.java.ledger import Ledger
from codebreaker.java.profiles import Profile
from typing import (
    Iterator,
    Optional,
    Tuple,
)


def _build_override_pairs(output_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> frozenset: ...


def _ctor_bodies(text: str, cls: str) -> Iterator[Tuple[int, str]]: ...


def _iter_code_lines(text: str) -> Iterator[Tuple[int, str, str]]: ...


def _iter_functions(text: str) -> Iterator[Tuple[str, str, int]]: ...


def _loop_body_span(text: str, hdr_open: int) -> Tuple[int, int]: ...


def _match_brace(src: str, open_idx: int) -> int: ...


def _read(path: str) -> str: ...


def _reseed(p: Profile) -> None: ...


def _scan_odr(ledger: Ledger, output_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> int: ...


def _strip_block_comments(text: str) -> str: ...


def rule_ctor_stub(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_debug_bypass(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_endianness_raw_cast(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_hotpath_alloc(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_infinite_loop(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_leak_drop_no_delete(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_leak_dtor_member(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_leak_loop_reassign(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_leak_temp_new(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_map_read_insert(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_null_deref_field(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_null_deref_getter(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_ptr_container_compare(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_rule_of_three(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_side_effect_in_branch(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_silent_dispatch(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_static_init_order(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_throw_fallthrough(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_uninit_array(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_value_copy_owner(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_virtual_call_in_ctor(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def scan_rules(
    ledger: Ledger,
    output_dir: <class 'Union'>[str, pathlib.WindowsPath],
    platform: <class 'Union'>[str, None] = ...
) -> int: ...


class _PlatCtx:
    def __init__(self, platform_id: <class 'Union'>[str, None]) -> None: ...

from codebreaker.java.ledger import Ledger
from codebreaker.java.profiles import Profile
from typing import (
    Iterator,
    Optional,
    Tuple,
)


def _build_override_pairs(output_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> frozenset: ...


def _ctor_bodies(text: str, cls: str) -> Iterator[Tuple[int, str]]: ...


def _iter_code_lines(text: str) -> Iterator[Tuple[int, str, str]]: ...


def _iter_functions(text: str) -> Iterator[Tuple[str, str, int]]: ...


def _loop_body_span(text: str, hdr_open: int) -> Tuple[int, int]: ...


def _match_brace(src: str, open_idx: int) -> int: ...


def _read(path: str) -> str: ...


def _reseed(p: Profile) -> None: ...


def _scan_odr(ledger: Ledger, output_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> int: ...


def _strip_block_comments(text: str) -> str: ...


def rule_ctor_stub(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_debug_bypass(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_endianness_raw_cast(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_hotpath_alloc(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_infinite_loop(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_leak_drop_no_delete(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_leak_dtor_member(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_leak_loop_reassign(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_leak_temp_new(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_map_read_insert(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_null_deref_field(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_null_deref_getter(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_ptr_container_compare(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_rule_of_three(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_side_effect_in_branch(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_silent_dispatch(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_static_init_order(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_throw_fallthrough(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_uninit_array(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def rule_value_copy_owner(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> None: ...


def rule_virtual_call_in_ctor(
    fname: str,
    text: str,
    header_text: <class 'Union'>[str, None],
    plat: _PlatCtx
) -> Iterator[Tuple[int, str, str, str, str, str]]: ...


def scan_rules(
    ledger: Ledger,
    output_dir: <class 'Union'>[str, pathlib.WindowsPath],
    platform: <class 'Union'>[str, None] = ...
) -> int: ...


class _PlatCtx:
    def __init__(self, platform_id: <class 'Union'>[str, None]) -> None: ...
