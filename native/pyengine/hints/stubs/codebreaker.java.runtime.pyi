from typing import (
    Any,
    Dict,
    List,
    Optional,
)


def _buffer_stub(name: str) -> str: ...


def _class_stub(
    name: str,
    void_methods: <class 'Union'>[Tuple[()], List[str]] = ...,
    ret_methods: <class 'Union'>[Tuple[()], List[Tuple[str, Tuple[str, str]]]] = ...,
    ctors: bool = ...,
    bases: <class 'Union'>[Tuple[()], List[str]] = ...,
    includes: <class 'Union'>[Tuple[()], List[str]] = ...,
    fwds: <class 'Union'>[Tuple[str, str], Tuple[()], List[str]] = ...,
    extra: <class 'Union'>[Tuple[()], List[str]] = ...
) -> str: ...


def _ext_banner(name: str) -> List[str]: ...


def _guard(name: str) -> str: ...


def _load_canonical(header: str) -> str: ...


def _ns_stub(
    name: str,
    consts: <class 'Union'>[List[Tuple[str, str]], Tuple[()], List[Tuple[str, int]]] = ...,
    void_fns: <class 'Union'>[Tuple[()], List[str]] = ...,
    ret_fns: <class 'Union'>[Tuple[()], List[Tuple[str, Tuple[str, str]]]] = ...,
    includes: <class 'Union'>[Tuple[()], List[str]] = ...,
    fwds: <class 'Union'>[Tuple[()], List[str]] = ...
) -> str: ...


def _patch_timer_for_platform(content: str, platform_id: str) -> str: ...


def annotate_stub(text: str, class_name: str, platform_id: str) -> str: ...


def generate_files(
    cpp_std: <class 'Union'>[None, codebreaker.java.transpile.CppStandard] = ...,
    platform_id: <class 'Union'>[str, None] = ...
) -> Dict[str, str]: ...


def unwired_canonicals() -> List[Any]: ...
